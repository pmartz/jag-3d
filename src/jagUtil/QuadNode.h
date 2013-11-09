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

#ifndef __JAGUTIL_QUAD_NODE_H__
#define __JAGUTIL_QUAD_NODE_H__ 1

#include <jagUtil/Export.h>
#include <jagDraw/Node.h>
#include <jagDraw/Texture.h>
#include <jagDraw/Framebuffer.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Shader.h>
#include <jagDraw/Program.h>
#include <jagBase/ptr.h>
#include <jagBase/LogBase.h>



namespace jagUtil
{


/** \class QuadNode QuadNode.h <jagUtil/QuadNode.h>
\brief A jagDraw::Node for drawing a textured quad.
\details TBD

\logname jag.util.quadnode
*/
class JAGUTIL_EXPORT QuadNode : public jagDraw::Node
{
public:
    QuadNode( const std::string& logName=std::string( "" ) );
    /** Constructor for one input texture. */
    QuadNode( jagDraw::TexturePtr inputBuffer,
        jagDraw::TexturePtr outputBuffer=jagDraw::TexturePtr((jagDraw::Texture*)NULL),
        const std::string& logName=std::string( "" ) );
    /** Constructor for multiple input textures. */
    QuadNode( jagDraw::TextureVec& inputBuffers,
        jagDraw::TexturePtr outputBuffer=jagDraw::TexturePtr((jagDraw::Texture*)NULL),
        const std::string& logName=std::string( "" ) );
    QuadNode( const QuadNode& rhs );
    ~QuadNode();

    /** \brief Explicit rendering control.
    \details By default, QuadNode uses its own shaders, which adds samples from
    up to four textures to create the final color value. To produce a different final
    color value, the app should supply its own shaders or program.

    Note that the addShaders() methods are designed so that apps can use a default
    vertex shader and specify only the fragment shader. In this case, data/quadNode.vert
    is used as the vertex shader, which writes texture coord outputs as follow:
    \code
    out vec2 tc;
    \endcode
    Application fragment shader code should declare:
    \code
    in vec2 tc;
    \endcode
    to access these values.

    Furthermore, QuadNode will insert a CPP definition into fragment shader source
    as follows:
    \code
    #define TEXTURE COUNT <n>
    \endcode
    where \c <n> is the number of input textures.

    Finally, texture samplers should be defined in custom fragment shader source as follows:
    \code
    uniform sampler2D texture0;
    uniform sampler2D texture1;
    \\ etc.
    \endcode
    */
    void setShaders( const std::string& fragName, const std::string& vertName="" );
    /** \overload */
    void setShaders( jagDraw::ShaderPtr& frag,
        jagDraw::ShaderPtr& vert=jagDraw::ShaderPtr((jagDraw::Shader*)NULL) );
    /** \overload */
    void setProgram( jagDraw::ProgramPtr& program );

    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Call this function for a window resize event. */
    void reshape( const int w, const int h );

protected:
    void internalInit( jagDraw::ProgramPtr& program=jagDraw::ProgramPtr((jagDraw::Program*)NULL) );
    std::string getTextureCountString() const;

    jagDraw::TextureVec _inputBuffers;
    jagDraw::TexturePtr _outputBuffer;

    int _width, _height;

    jagDraw::FramebufferPtr _fbo;
    jagDraw::CommandMapPtr _commands;

    unsigned int _numContexts;
};

typedef jagBase::ptr< QuadNode >::shared_ptr QuadNodePtr;


// jagUtil
}


// __JAGUTIL_QUAD_NODE_H__
#endif
