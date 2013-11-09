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

#include <jagDraw/PixelStore.h>
#include <jagDraw/PlatformOpenGL.h>


namespace jagDraw {


PixelStore::PixelStore()
  : _swapBytes( GL_FALSE ),
    _lsbFirst( GL_FALSE ),
    _rowLength( 0 ),
    _skipRows( 0 ),
    _skipPixels( 0 ),
    _alignment( 4 ),
    _imageHeight( 0 ),
    _skipImages( 0 )
{
}
PixelStore::PixelStore( const PixelStore& rhs )
  : _swapBytes( rhs._swapBytes ),
    _lsbFirst( rhs._lsbFirst ),
    _rowLength( rhs._rowLength ),
    _skipRows( rhs._skipRows ),
    _skipPixels( rhs._skipPixels ),
    _alignment( rhs._alignment ),
    _imageHeight( rhs._imageHeight ),
    _skipImages( rhs._skipImages )
{
}
PixelStore::~PixelStore()
{
}


void PixelStore::pack()
{
    glPixelStorei( GL_PACK_SWAP_BYTES, (GLint)_swapBytes );
    glPixelStorei( GL_PACK_LSB_FIRST, (GLint)_lsbFirst );
    glPixelStorei( GL_PACK_ROW_LENGTH, _rowLength );
    glPixelStorei( GL_PACK_SKIP_ROWS, _skipRows );
    glPixelStorei( GL_PACK_SKIP_PIXELS, _skipPixels );
    glPixelStorei( GL_PACK_ALIGNMENT, _alignment );
    glPixelStorei( GL_PACK_IMAGE_HEIGHT, _imageHeight );
    glPixelStorei( GL_PACK_SKIP_IMAGES, _skipImages );
}
void PixelStore::unpack()
{
    glPixelStorei( GL_UNPACK_SWAP_BYTES, (GLint)_swapBytes );
    glPixelStorei( GL_UNPACK_LSB_FIRST, (GLint)_lsbFirst );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, _rowLength );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, _skipRows );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, _skipPixels );
    glPixelStorei( GL_UNPACK_ALIGNMENT, _alignment );
    glPixelStorei( GL_UNPACK_IMAGE_HEIGHT, _imageHeight );
    glPixelStorei( GL_UNPACK_SKIP_IMAGES, _skipImages );
}


// jagDraw
}
