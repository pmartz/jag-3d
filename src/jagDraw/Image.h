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

#ifndef __JAGDRAW_IMAGE_H__
#define __JAGDRAW_IMAGE_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/Buffer.h>
#include <jagDraw/PixelStore.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>


namespace jagDraw {


/** \addtogroup OpenGLAbstraction Support for OpenGL Abstract Concepts */
/*@{*/

/** \class Image Image.h <jagDraw/Image.h>
\brief TBD
\details TBD
*/
class JAGDRAW_EXPORT Image : protected jagBase::LogBase
{
public:
    Image();
    Image( const Image& rhs );
    virtual ~Image();

    void set( const GLint level, const GLenum internalFormat,
        const GLsizei width, const GLsizei height, const GLsizei depth,
        const GLint border, const GLenum format, const GLenum type,
        jagBase::BufferPtr data );
    void get( GLint& level, GLenum& internalFormat,
        GLsizei& width, GLsizei& height, GLsizei& depth,
        GLint& border, GLenum& format, GLenum& type,
        jagBase::BufferPtr& data );

    void setCompressed( const GLint level, const GLenum internalFormat,
        const GLsizei width, const GLsizei height, const GLsizei depth,
        const GLint border, const GLsizei imageSize,
        jagBase::BufferPtr data );
    void getCompressed( GLint& level, GLenum& internalFormat,
        GLsizei& width, GLsizei& height, GLsizei& depth,
        GLint& border, GLsizei& imageSize,
        jagBase::BufferPtr& data );
    bool getCompressed() const
    {
        return( _compressed );
    }

    void setPixelStore( PixelStorePtr pixelStore );
    PixelStorePtr getPixelStore();

protected:
    GLint _level;
    GLenum _internalFormat;
    GLsizei _width, _height, _depth;
    GLint _border;
    GLenum _format;
    GLenum _type;

    GLsizei _imageSize;
    bool _compressed;

    jagBase::BufferPtr _data;

    PixelStorePtr _pixelStore;
};

/*@}*/

typedef jagBase::ptr< jagDraw::Image >::shared_ptr ImagePtr;
typedef std::vector< ImagePtr > ImageVec;


// jagDraw
}


// __JAGDRAW_IMAGE_H__
#endif
