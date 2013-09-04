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
    Image( const std::string& logName=std::string( "" ) );
    Image( const Image& rhs );
    virtual ~Image();

    typedef enum {
        NO_DELETE,
        USE_NEW_DELETE,
        USE_MALLOC_FREE
    } AllocationMode;

    /** \brief TBD
    \details TBD */
    void setAllocationMode( const AllocationMode mode ) { _allocMode = mode; }
    AllocationMode getAllocationMode() const { return( _allocMode ); }

    /** \brief TBD
    \details TBD */
    void set( const GLint level, const GLenum internalFormat,
        const GLsizei width, const GLsizei height, const GLsizei depth,
        const GLint border, const GLenum format, const GLenum type,
        unsigned char* data );
    /** \brief TBD
    \details TBD */
    void get( GLint& level, GLenum& internalFormat,
        GLsizei& width, GLsizei& height, GLsizei& depth,
        GLint& border, GLenum& format, GLenum& type,
        unsigned char** data );

	void getDims(GLsizei& width, GLsizei& height) {
		width = _width;
		height = _height;
	}

    /** \brief TBD
    \details TBD */
    void setCompressed( const GLint level, const GLenum internalFormat,
        const GLsizei width, const GLsizei height, const GLsizei depth,
        const GLint border, const GLsizei imageSize,
        unsigned char* data );
    /** \brief TBD
    \details TBD */
    void getCompressed( GLint& level, GLenum& internalFormat,
        GLsizei& width, GLsizei& height, GLsizei& depth,
        GLint& border, GLsizei& imageSize,
        unsigned char** data );

    /** \brief TBD
    \details TBD */
    bool getCompressed() const { return( _compressed ); }

    /** \brief TBD
    \details TBD */
    void setPixelStore( PixelStorePtr pixelStore );
    /** \brief TBD
    \details TBD */
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

    /** Initial value: USE_NEW_DELETE */
    AllocationMode _allocMode;
    unsigned char* _data;

    PixelStorePtr _pixelStore;
};

/*@}*/

typedef jagBase::ptr< jagDraw::Image >::shared_ptr ImagePtr;
typedef std::vector< ImagePtr > ImageVec;


// jagDraw
}


// __JAGDRAW_IMAGE_H__
#endif
