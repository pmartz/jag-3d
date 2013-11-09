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

#ifndef __JAGDRAW_ERROR_H__
#define __JAGDRAW_ERROR_H__ 1


#include <jagBase/Config.h>
#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <string>



namespace jagDraw
{


/** \defgroup jagDrawError Error Utilities
\logname jag.draw.glerror
\logname jag.draw.glerror.fbo
*/
/*@{*/

#ifndef JAG3D_ENABLE_ERROR_CHECKS
#  define JAG3D_ERROR_CHECK(msg) GL_NO_ERROR
#  define JAG3D_FBO_ERROR_CHECK(msg) GL_FRAMEBUFFER_COMPLETE
#else
    /** \brief Error macro.
    \details By default, this macro calls errorCheck().
    If JAG3D_ENABLE_ERROR_CHECKS is not defined, this macro is a no-op.
    */
#  define JAG3D_ERROR_CHECK(msg) jagDraw::errorCheck( msg )
    /** \brief FBO status macro.
    \details By default, this macro calls fboErrorCheck().
    If JAG3D_ENABLE_ERROR_CHECKS is not defined, this macro is a no-op.
    */
#  define JAG3D_FBO_ERROR_CHECK(msg) jagDraw::fboErrorCheck( msg )
#endif


/** \brief Check for an OpenGL error using glGetError().
\details Use JAG3D_ERROR_CHECK instead, which supports compile time
error check elimination when the CMake variable JAG3D_ENABLE_ERROR_CHECKS
is set to OFF. */
JAGDRAW_EXPORT GLenum errorCheck( const std::string& msg );

/** \brief Check the FBO status using glCheckFramebufferStatus().
\details Use JAG3D_FBO_ERROR_CHECK instead, which supports compile time
error check elimination when the CMake variable JAG3D_ENABLE_ERROR_CHECKS
is set to OFF. */
JAGDRAW_EXPORT GLenum fboErrorCheck( const std::string& msg );

/*@}*/


// jagDraw
}


// __JAGDRAW_ERROR_H__
#endif
