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

#ifndef __JAGDRAW_PLATFORM_OPENGL_H__
#define __JAGDRAW_PLATFORM_OPENGL_H__ 1


// For now, everyone has to use GLEW. Until we have a better solution.
#define USE_GLEW 1

#if defined( USE_GLEW )
#  include <jagDraw/PlatformGLEW.h>
#elif defined( WIN32 )
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#  include <windows.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#else
#  ifndef GL_GLEXT_PROTOTYPES
#    define GL_GLEXT_PROTOTYPES
#  endif
#  if defined( __APPLE__)
#    include<OpenGL/OpenGL.h>
#    include<OpenGL/glu.h>
#  else
#    ifdef USE_EGL
#      include <EGL/egl.h>
#      include <EGL/eglext.h>
#      include <GLES2/gl2.h>
#      include <GLES2/gl2ext.h>
#    else
#      include <GL/gl.h>
#      include <GL/glu.h>
#      include <GL/glext.h>
#    endif
#  endif
#endif


// __JAGDRAW_PLATFORM_OPENGL_H__
#endif
