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

#include <jag/draw/PixelStore.h>
#include <jag/draw/PlatformOpenGL.h>


namespace jag {
namespace draw {


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
    glPixelStorei( GL_PACK_ROW_LENGTH, _rowLength );
    glPixelStorei( GL_PACK_SKIP_ROWS, _skipRows );
    glPixelStorei( GL_PACK_SKIP_PIXELS, _skipPixels );
    glPixelStorei( GL_PACK_ALIGNMENT, _alignment );
#ifndef JAG3D_USE_GLES3
    glPixelStorei( GL_PACK_SWAP_BYTES, (GLint)_swapBytes );
    glPixelStorei( GL_PACK_LSB_FIRST, (GLint)_lsbFirst );
    glPixelStorei( GL_PACK_IMAGE_HEIGHT, _imageHeight );
    glPixelStorei( GL_PACK_SKIP_IMAGES, _skipImages );
#endif
}
void PixelStore::unpack()
{
    glPixelStorei( GL_UNPACK_ROW_LENGTH, _rowLength );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, _skipRows );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, _skipPixels );
    glPixelStorei( GL_UNPACK_ALIGNMENT, _alignment );
#ifndef JAG3D_USE_GLES3
    glPixelStorei( GL_UNPACK_SWAP_BYTES, (GLint)_swapBytes );
    glPixelStorei( GL_UNPACK_LSB_FIRST, (GLint)_lsbFirst );
    glPixelStorei( GL_UNPACK_IMAGE_HEIGHT, _imageHeight );
    glPixelStorei( GL_UNPACK_SKIP_IMAGES, _skipImages );
#endif
}


// namespace jag::draw::
}
}
