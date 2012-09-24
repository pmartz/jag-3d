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

#ifndef __JAGDRAW_PIXEL_STORE_H__
#define __JAGDRAW_PIXEL_STORE_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \class PixelStore PixelStore.h <jagDraw/PixelStore.h>
\brief glPixelStore parameter container for sending or retrieving pixel data.
\details The glPixelStore pack and unpack parameters differ only by the \c pname
parameter to the glPixelStore function call. This class contains all the
parameters, and the operator() sets them as either pack or unpack parameters
based on the value of the specified \c unpack boolean.

The jagDraw::Image class contains a PixelStorePtr member variable. Prior to
calling glTexImage*D(), jagDraw::Texture uses the Image PixelStore object to
set the pixel unpack parameters in OpenGL state.

All parameter defaults are per OpenGL spec.
\gl{table 3.1}
\gl{table 4.7}
*/
struct JAGDRAW_EXPORT PixelStore
{
    PixelStore();
    PixelStore( const PixelStore& rhs );
    virtual ~PixelStore();

    /** \brief Set the pixel store parameters in OpenGL state.
    \details If \c unpack is true, the values are set as unpack parameters (for
    sending pixel data to OpenGL). If \c unpack is false, the values are set as
    pack parameters (for retrieving pixel data from OpenGL). The default for
    \c unpack is true. */
    void operator()( const bool unpack=true );

    GLboolean _swapBytes;
    GLboolean _lsbFirst;
    GLint _rowLength;
    GLint _skipRows;
    GLint _skipPixels;
    GLint _alignment;
    GLint _imageHeight;
    GLint _skipImages;
};

typedef jagBase::ptr< jagDraw::PixelStore >::shared_ptr PixelStorePtr;
typedef std::vector< PixelStorePtr > PixelStoreList;


// jagDraw
}


// __JAGDRAW_PIXEL_STORE_H__
#endif
