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
