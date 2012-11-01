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

#ifndef __JAGDRAW_ERROR_H__
#define __JAGDRAW_ERROR_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <string>



namespace jagDraw
{


/** \defgroup jagDrawError Error Utilities */
/*@{*/

#ifdef JAG3D_DISABLE_ALL_ERROR_CHECKS
#  define JAG3D_ERROR_CHECK(msg) GL_NO_ERROR
#  define JAG3D_FBO_ERROR_CHECK(msg) GL_FRAMEBUFFER_COMPLETE
#else
    /** \brief Error macro.
    \details By default, this macro calls errorCheck().
    If JAG3D_DISABLE_ALL_ERROR_CHECKS is defined, this macro is a no-op.
    */
#  define JAG3D_ERROR_CHECK(msg) jagDraw::errorCheck( msg )
    /** \brief FBO status macro.
    \details By default, this macro calls fboErrorCheck().
    If JAG3D_DISABLE_ALL_ERROR_CHECKS is defined, this macro is a no-op.
    */
#  define JAG3D_FBO_ERROR_CHECK(msg) jagDraw::fboErrorCheck( msg )
#endif


/** \brief Check for an OpenGL error using glGetError().
\details Use JAG3D_ERROR_CHECK instead, which supports compile time
error check elimination with JAG3D_DISABLE_ALL_ERROR_CHECKS.
*/
JAGDRAW_EXPORT GLenum errorCheck( const std::string& msg );

/** \brief Check the FBO status using glCheckFramebufferStatus().
\details Use JAG3D_FBO_ERROR_CHECK instead, which supports compile time
error check elimination with JAG3D_DISABLE_ALL_ERROR_CHECKS.
*/
JAGDRAW_EXPORT GLenum fboErrorCheck( const std::string& msg );

/*@}*/


// jagDraw
}


// __JAGDRAW_ERROR_H__
#endif
