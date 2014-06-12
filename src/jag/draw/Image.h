/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAG_DRAW_IMAGE_H__
#define __JAG_DRAW_IMAGE_H__ 1


#include <jag/draw/Export.h>
#include <jag/draw/PlatformOpenGL.h>
#include <jag/draw/PixelStore.h>
#include <jag/base/LogBase.h>
#include <jag/base/ptr.h>


namespace jag {
namespace draw {


/** \addtogroup OpenGLAbstraction Support for OpenGL Abstract Concepts */
/*@{*/

/** \class Image Image.h <jag/draw/Image.h>
\brief TBD
\details TBD
*/
class JAGDRAW_EXPORT Image : protected jag::base::LogBase
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
    \details Default value is USE_NEW_DELETE. */
    void setAllocationMode( const AllocationMode mode ) { _allocMode = mode; }
    AllocationMode getAllocationMode() const { return( _allocMode ); }

    /** \brief TBD
    \details If \c data is NULL, this function sets all values then
    implicitly calls allocate(). */
    void set( const GLint level, const GLenum internalFormat,
        const GLsizei width, const GLsizei height, const GLsizei depth,
        const GLint border, const GLenum format, const GLenum type,
        unsigned char* data=NULL );
    /** \brief TBD
    \details Allocate enough memory to hold a block of pixels sized according
    to current member variable values. */
    void allocate();

    /** \brief TBD
    \details TBD */
    void get( GLint& level, GLenum& internalFormat,
        GLsizei& width, GLsizei& height, GLsizei& depth,
        GLint& border, GLenum& format, GLenum& type,
        unsigned char** data );
    /** \brief TBD
    \details TBD */
    GLenum getInternalFormat() const { return( _internalFormat ); }
    /** \brief TBD
    \details TBD */
    GLenum getFormat() const { return( _format ); }
    /** \brief TBD
    \details TBD */
    GLenum getType() const { return( _type ); }
    /** \brief TBD
    \details TBD */
    unsigned char* getData() const { return( _data ); }
    /** \brief TBD
    \details TBD */
    void getDims( GLsizei& width, GLsizei& height )
    {
        width = _width;
        height = _height;
    }
    /** \brief TBD
    \details TBD */
    void getDims( GLsizei& width, GLsizei& height, GLsizei& depth )
    {
        width = _width;
        height = _height;
        depth = _depth;
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


    /** \brief TBD
    \details TBD */
    unsigned int computeDataSize();
    /** \brief TBD
    \details TBD */
    static unsigned int bytesPerPixel( const GLenum format, const GLenum type );
    /** \brief TBD
    \details TBD */
    static unsigned int componentsPerPixel( const GLenum format );

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

typedef jag::base::ptr< jag::draw::Image >::shared_ptr ImagePtr;
typedef std::vector< ImagePtr > ImageVec;


// namespace jag::draw::
}
}


// __JAG_DRAW_IMAGE_H__
#endif
