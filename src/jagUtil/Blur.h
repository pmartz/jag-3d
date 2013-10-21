/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License version 2.1 as published by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*
*************** <auto-copyright.pl END do not edit this line> ***************/

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

    /** \brief TBD
    \details TBD */
    jagDraw::NodeContainer& getNodeContainer();

    /** \brief Call this function for a window resize event. */
    void reshape( const int w, const int h );

protected:
    void internalInit();

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
