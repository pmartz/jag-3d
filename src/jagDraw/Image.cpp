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

#include <jagDraw/Image.h>
#include <jagDraw/PixelStore.h>


namespace jagDraw {


Image::Image( const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.draw.image" : logName ),
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
