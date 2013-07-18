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


// This header defines use of GL3W.
#include <jagBase/Config.h>

#if defined( JAG3D_USE_GL3W )
#  include <jagDraw/GL/gl3w.h>
#else
#  ifndef GL_GLEXT_PROTOTYPES
#    define GL_GLEXT_PROTOTYPES
#  endif
#  if defined( __APPLE__)
#    include<OpenGL/gl3.h>
#  elif defined( USE_EGL )
#    include <EGL/egl.h>
#    include <EGL/eglext.h>
#    include <GLES2/gl2.h>
#    include <GLES2/gl2ext.h>
#  else
#    define GL3_PROTOTYPES 1
#    include <GL/gl3.h>
#  endif
#endif


// __JAGDRAW_PLATFORM_OPENGL_H__
#endif
