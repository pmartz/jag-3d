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

#ifndef __JAG_DRAW_PLATFORM_OPENGL_H__
#define __JAG_DRAW_PLATFORM_OPENGL_H__ 1


// This header defines use of GL3W.
#include <jag/base/Config.h>

#if defined( JAG3D_USE_GL3W )

   // GL3W filles in OpenGL function points and is primarily for Windows.
#  include <jag/draw/gl3w.h>

#elif( defined( __APPLE__ ) && defined( JAG3D_USE_GLES3 ) )

   // GL ES 3 on iOS build.
#  ifndef GL_GLEXT_PROTOTYPES
#    define GL_GLEXT_PROTOTYPES
#  endif
#  include <OpenGLES/ES3/gl.h>
   typedef GLfloat GLdouble;
   typedef GLclampf GLclampd;
#  define GL_TEXTURE_BUFFER 0x8C2A

#elif defined( __APPLE__ )

   // OSX
#  ifndef GL_GLEXT_PROTOTYPES
#    define GL_GLEXT_PROTOTYPES
#  endif
#  include<OpenGL/gl3.h>

#else

   // Linux, etc.
#  ifndef GL_GLEXT_PROTOTYPES
#    define GL_GLEXT_PROTOTYPES
#  endif
#  define GL3_PROTOTYPES 1
#  include <GL/gl3.h>

#endif


// __JAG_DRAW_PLATFORM_OPENGL_H__
#endif
