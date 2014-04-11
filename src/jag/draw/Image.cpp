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

#include <jag/draw/Image.h>
#include <jag/draw/PixelStore.h>
#include <jag/base/LogMacros.h>

#include <sstream>


namespace jag {
namespace draw {


Image::Image( const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.draw.image" : logName ),
    _level( 0 ),
    _internalFormat( GL_NONE ),
    _width( 0 ),
    _height( 0 ),
    _depth( 0 ),
    _border( 0 ),
    _format( GL_NONE ),
    _type( GL_NONE ),
    _imageSize( 0 ),
    _compressed( false ),
    _allocMode( USE_NEW_DELETE ),
    _data( NULL ),
    _pixelStore( PixelStorePtr( (PixelStore*)NULL ) )
{
}
Image::Image( const Image& rhs )
  : jag::base::LogBase( rhs ),
    _level( rhs._level ),
    _internalFormat( rhs._internalFormat ),
    _width( rhs._width ),
    _height( rhs._height ),
    _depth( rhs._depth ),
    _border( rhs._border ),
    _format( rhs._format ),
    _type( rhs._type ),
    _imageSize( rhs._imageSize ),
    _compressed( rhs._compressed ),
    _allocMode( rhs._allocMode ),
    _data( rhs._data ),
    _pixelStore( rhs._pixelStore )
{
}
Image::~Image()
{
    if( _data != NULL )
    {
        if( _allocMode == USE_NEW_DELETE )
            delete [] _data;
        else if( _allocMode == USE_MALLOC_FREE )
            ::free( _data );
        _data = NULL;
    }
}


void Image::set( const GLint level, const GLenum internalFormat,
    const GLsizei width, const GLsizei height, const GLsizei depth,
    const GLint border, const GLenum format, const GLenum type,
    unsigned char* data )
{
    _level = level;
    _internalFormat = internalFormat;
    _width = width;
    _height = height;
    _depth = depth;
    _border = border;
    _format = format;
    _type = type;

    if( data != NULL )
    {
        _data = data;
    }
    else
    {
        allocate();
    }
}
void Image::allocate()
{
    const int numBytes( computeDataSize() );
    switch( _allocMode )
    {
    case Image::USE_MALLOC_FREE:
        _data = (unsigned char*)( malloc( numBytes ) );
        break;
    case Image::NO_DELETE:
    case Image::USE_NEW_DELETE:
        _data = new unsigned char[ numBytes ];
        break;
    }
}

void Image::get( GLint& level, GLenum& internalFormat,
    GLsizei& width, GLsizei& height, GLsizei& depth,
    GLint& border, GLenum& format, GLenum& type,
    unsigned char** data )
{
    level = _level;
    internalFormat = _internalFormat;
    width = _width;
    height = _height;
    depth = _depth;
    border = _border;
    format = _format;
    type = _type;
    *data = _data;
}


void Image::setCompressed( const GLint level, const GLenum internalFormat,
    const GLsizei width, const GLsizei height, const GLsizei depth,
    const GLint border, const GLsizei imageSize,
    unsigned char* data )
{
    _level = level;
    _internalFormat = internalFormat;
    _width = width;
    _height = height;
    _depth = depth;
    _border = border;
    _imageSize = imageSize;
    _data = data;

    _compressed = true;
}
void Image::getCompressed( GLint& level, GLenum& internalFormat,
    GLsizei& width, GLsizei& height, GLsizei& depth,
    GLint& border, GLsizei& imageSize,
    unsigned char** data )
{
    level = _level;
    internalFormat = _internalFormat;
    width = _width;
    height = _height;
    depth = _depth;
    border = _border;
    imageSize = _imageSize;
    *data = _data;
}


void Image::setPixelStore( PixelStorePtr pixelStore )
{
    _pixelStore = pixelStore;
}
PixelStorePtr Image::getPixelStore()
{
    return( _pixelStore );
}


unsigned int Image::computeDataSize()
{
    if( _compressed )
        return( _imageSize );
    else
        return( _width * _height * _depth *
            bytesPerPixel( _format, _type ) );
}
unsigned int Image::bytesPerPixel( const GLenum format, const GLenum type )
{
    const unsigned int numComponents( componentsPerPixel( format ) );

    // OpenGL 4.1 spec table 3.2
    switch( type )
    {
    case GL_UNSIGNED_BYTE:
        return( sizeof( GLubyte ) * numComponents );
        break;
    case GL_UNSIGNED_BYTE_3_3_2:
    case GL_UNSIGNED_BYTE_2_3_3_REV:
        return( sizeof( GLubyte ) );
        break;
    case GL_BYTE:
        return( sizeof( GLbyte ) * numComponents );
        break;
    case GL_UNSIGNED_SHORT:
        return( sizeof( GLushort ) * numComponents );
        break;
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_5_6_5_REV:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_4_4_4_4_REV:
    case GL_UNSIGNED_SHORT_5_5_5_1:
    case GL_UNSIGNED_SHORT_1_5_5_5_REV:
        return( sizeof( GLushort ) );
        break;
    case GL_SHORT:
        return( sizeof( GLshort ) * numComponents );
        break;
    case GL_UNSIGNED_INT:
        return( sizeof( GLuint ) * numComponents );
        break;
    case GL_UNSIGNED_INT_8_8_8_8:
    case GL_UNSIGNED_INT_8_8_8_8_REV:
    case GL_UNSIGNED_INT_10_10_10_2:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_24_8:
    case GL_UNSIGNED_INT_10F_11F_11F_REV:
    case GL_UNSIGNED_INT_5_9_9_9_REV:
        return( sizeof( GLuint ) );
        break;
    case GL_INT:
        return( sizeof( GLint ) * numComponents );
        break;
    case GL_HALF_FLOAT:
        return( sizeof( GLhalf ) * numComponents );
        break;
    case GL_FLOAT:
        return( sizeof( GLfloat ) * numComponents );
        break;
    case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
    default:
        {
            std::ostringstream ostr;
            ostr << std::hex << "0x" << type;
            JAG3D_WARNING_STATIC( "jag.draw.image", "Unsupported image type in computePixelSize: " + ostr.str() );
            return( sizeof( GLuint ) * numComponents );
        }
    }
}
unsigned int Image::componentsPerPixel( const GLenum format )
{
    // OpenGL 4.1 spec table 3.3
    switch( format )
    {
    case GL_STENCIL_INDEX:
    case GL_DEPTH_COMPONENT:
    case GL_RED:
    case GL_GREEN:
    case GL_BLUE:
    case GL_RED_INTEGER:
    case GL_GREEN_INTEGER:
    case GL_BLUE_INTEGER:
        return( 1 );
        break;
    case GL_DEPTH_STENCIL:
    case GL_RG:
    case GL_RG_INTEGER:
        return( 2 );
        break;
    case GL_RGB:
    case GL_BGR:
    case GL_RGB_INTEGER:
    case GL_BGR_INTEGER:
        return( 3 );
        break;
    case GL_RGBA:
    case GL_BGRA:
    case GL_RGBA_INTEGER:
    case GL_BGRA_INTEGER:
        return( 4 );
        break;
    default:
        {
            std::ostringstream ostr;
            ostr << std::hex << "0x" << format;
            JAG3D_WARNING_STATIC( "jag.draw.image", "Unsupported image format in componentsPerPixel: " + ostr.str() );
            return( 4 );
        }
    }
}


// namespace jag::draw::
}
}
