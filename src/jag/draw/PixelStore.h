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

#ifndef __JAG_DRAW_PIXEL_STORE_H__
#define __JAG_DRAW_PIXEL_STORE_H__ 1


#include <jag/draw/Export.h>
#include <jag/draw/PlatformOpenGL.h>
#include <jag/base/ptr.h>

#include <vector>


namespace jag {
namespace draw {


/** \class PixelStore PixelStore.h <jag/draw/PixelStore.h>
\brief glPixelStore parameter container for sending or retrieving pixel data.
\details The glPixelStore pack and unpack parameters differ only by the \c pname
parameter to the glPixelStore function call. This class contains all the
parameters, and the pack() and unpack() functions set them as either pack or
unpack parameters respectively.

The jag::draw::Image class contains a PixelStorePtr member variable. Prior to
calling glTexImage*D(), jag::draw::Texture uses the Image PixelStore object to
set the pixel unpack parameters in OpenGL state.

All parameter defaults are per OpenGL spec.
\gl{table 3.1}
\gl{table 4.7}
*/
struct JAGDRAW_EXPORT PixelStore
{
    PixelStore();
    PixelStore( const PixelStore& rhs );
    virtual ~PixelStore();

    /** \brief Set the pixel store parameters in OpenGL state.
    \details Call pack() to set the parameters for
    retrieving pixel data from OpenGL. */
    void pack();
    /** \brief Set the pixel store parameters in OpenGL state.
    \details Call unpack() to set the parameters for
    sending pixel data to OpenGL. */
    void unpack();

    GLboolean _swapBytes;
    GLboolean _lsbFirst;
    GLint _rowLength;
    GLint _skipRows;
    GLint _skipPixels;
    GLint _alignment;
    GLint _imageHeight;
    GLint _skipImages;
};

typedef jag::base::ptr< jag::draw::PixelStore >::shared_ptr PixelStorePtr;
typedef std::vector< PixelStorePtr > PixelStoreVec;


// namespace jag::draw::
}
}


// __JAG_DRAW_PIXEL_STORE_H__
#endif
