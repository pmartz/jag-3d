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


void PixelStore::operator()( const bool unpack )
{
    if( unpack )
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
    else /* pack */
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
}


// jagDraw
}
