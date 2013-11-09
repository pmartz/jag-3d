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

#ifndef __JAGDRAW_PIXEL_STORE_H__
#define __JAGDRAW_PIXEL_STORE_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/ptr.h>

#include <vector>


namespace jagDraw {


/** \class PixelStore PixelStore.h <jagDraw/PixelStore.h>
\brief glPixelStore parameter container for sending or retrieving pixel data.
\details The glPixelStore pack and unpack parameters differ only by the \c pname
parameter to the glPixelStore function call. This class contains all the
parameters, and the pack() and unpack() functions set them as either pack or
unpack parameters respectively.

The jagDraw::Image class contains a PixelStorePtr member variable. Prior to
calling glTexImage*D(), jagDraw::Texture uses the Image PixelStore object to
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

typedef jagBase::ptr< jagDraw::PixelStore >::shared_ptr PixelStorePtr;
typedef std::vector< PixelStorePtr > PixelStoreVec;


// jagDraw
}


// __JAGDRAW_PIXEL_STORE_H__
#endif
