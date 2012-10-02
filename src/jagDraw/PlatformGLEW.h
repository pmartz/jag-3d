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

#ifndef __JAGBASE_PLATFORM_GLEW_H__
#define __JAGBASE_PLATFORM_GLEW_H__ 1

#include <jagDraw/Export.h>


// TBD This should be set automatically if the user has the MX variant
// of GLEW available and has explicitly asked for it in the xmake-gui.
// This work is not yet implemented.
#define GLEW_MX


#include <GL/glew.h>

#if defined( _WIN32 )
#  include <GL/wglew.h>
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
#  include <GL/glxew.h>
#endif


// TBD Ideally, the three functions below should be #define macros rather
// than functions, to eliminate the per-OpenGL command function call
// overhead. This work is not yet implemented.
JAGDRAW_EXPORT GLEWContext* glewGetContext();
#ifdef _WIN32
    JAGDRAW_EXPORT WGLEWContext* wglewGetContext();
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
    GLXEWContext* glxewGetContext();
#endif



// __JAGBASE_PLATFORM_GLEW_H__
#endif
