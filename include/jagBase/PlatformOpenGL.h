// Copyright

#ifndef __JAGBASE_PLATFORM_OPENGL_H__
#define __JAGBASE_PLATFORM_OPENGL_H__ 1


// For now, everyone has to use GLEW. Until we have a better solution.
#define USE_GLEW 1

#if defined( USE_GLEW )
#  include <jagBase/PlatformGLEW.h>
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


// __JAGBASE_PLATFORM_OPENGL_H__
#endif
