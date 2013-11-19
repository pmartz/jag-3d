/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAGUTIL_BLUR_H__
#define __JAGUTIL_BLUR_H__ 1

#include <jagUtil/Export.h>
#include <jagDraw/DrawGraph.h>
#include <jagDraw/CommandMap.h>
#include <jagUtil/QuadNode.h>
#include <jagDraw/Texture.h>
#include <jagDraw/Program.h>
#include <jagDraw/PerContextData.h>
#include <jagBase/ptr.h>
#include <jagBase/LogBase.h>


// Forwaed
namespace jagSG {
    class CollectionVisitor;
}


namespace jagUtil
{


/** \class Blur Blur.h <jagUtil/Blur.h>
\brief Support for a simple 2-part blur effect.
\details

The default shaders blur color values with non-zero alpha and
ignore color values with zero alpha. For this to work properly,
the input color buffer should be cleared to a color value with
zero alpha, then color values to be blurred should be written
with non-zero alpha.

\logname jag.util.blur
*/
class JAGUTIL_EXPORT Blur : public jagBase::LogBase
{
public:
    Blur( const std::string& logName=std::string( "" ) );
    Blur( jagDraw::TexturePtr& inputBuffer, jagDraw::TexturePtr& outputBuffer,
        const std::string& logName=std::string( "" ) );
    Blur( const Blur& rhs );
    ~Blur();

    void setMaxContexts( const unsigned int numContexts );

    /** \brief Get the BLur NodeContainer.
    \details In typical usage, application code should copy this
    NodeContainer into the DrawGraph template passed to
    jagSG::CollectionVisitor. */
    jagDraw::NodeContainer& getNodeContainer();

    /** \brief Set custom fragment shaders by name.
    \details Use this method to override the default vlur effect fragment shaders.
    Fragment shader code should declare the following variables:
    \code
    uniform sampler2D texture0;
    in vec2 tc;
    \endcode
    Blue executes in two passes. The \c texture0 input to \c fragStage0 is
    the \c inputBuffer passed to the constructor. The \c texture0 input to
    \c fragStage1 is the output of \c fragStage0.

    The default shaders are:
    \li stage 0: data/blurHorizontal.frag
    \li stage 1: data/blurVertical.frag
    */
    void setShaders( const std::string& fragStage0, const std::string& fragStage1 );
    /** \overload */
    void setShaders( jagDraw::ShaderPtr& fragStage0, jagDraw::ShaderPtr& fragStage1 );

    /** \brief Call this function for a window resize event. */
    void reshape( const int w, const int h );

protected:
    void internalInit( jagDraw::ShaderPtr& fragStage0=jagDraw::ShaderPtr((jagDraw::Shader*)NULL),
        jagDraw::ShaderPtr& fragStage1=jagDraw::ShaderPtr((jagDraw::Shader*)NULL) );

    jagDraw::TexturePtr _inputBuffer, _outputBuffer;

    unsigned int _numContexts;
    int _width, _height;

    jagDraw::NodeContainer _container;
    jagUtil::QuadNodePtr _hQuad, _vQuad;
    jagDraw::TexturePtr _intermediateBuffer;
};

typedef jagBase::ptr< Blur >::shared_ptr BlurPtr;


// jagUtil
}


// __JAGUTIL_BLUR_H__
#endif
