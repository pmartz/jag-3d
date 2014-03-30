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

#ifndef __JAG_DRAW_ERROR_H__
#define __JAG_DRAW_ERROR_H__ 1


#include <jag/base/Config.h>
#include <jag/draw/Export.h>
#include <jag/draw/PlatformOpenGL.h>
#include <string>



namespace jag {
namespace draw {


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
#  define JAG3D_ERROR_CHECK(msg) jag::draw::errorCheck( msg )
    /** \brief FBO status macro.
    \details By default, this macro calls fboErrorCheck().
    If JAG3D_ENABLE_ERROR_CHECKS is not defined, this macro is a no-op.
    */
#  define JAG3D_FBO_ERROR_CHECK(msg) jag::draw::fboErrorCheck( msg )
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


// namespace jag::draw::
}
}


// __JAG_DRAW_ERROR_H__
#endif
