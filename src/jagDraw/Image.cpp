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

#include <jagDraw/Image.h>
#include <jagDraw/PixelStore.h>


namespace jagDraw {


Image::Image()
  : jagBase::LogBase( "jag.draw.image" ),
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
  : jagBase::LogBase( rhs ),
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
    _data = data;
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



// jagDraw
}
