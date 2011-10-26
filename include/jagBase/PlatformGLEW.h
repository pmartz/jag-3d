// Copyright

#ifndef __JAGBASE_PLATFORM_GLEW_H__
#define __JAGBASE_PLATFORM_GLEW_H__ 1


#define GLEW_MX

#include <GL/glew.h>

#if defined(_WIN32)
#  include <GL/wglew.h>
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
#  include <GL/glxew.h>
#endif

// Globals
#ifdef GLEW_MX
   static GLEWContext _glewctx;
#  define glewGetContext() (&_glewctx)
#  ifdef _WIN32
     static WGLEWContext _wglewctx;
#    define wglewGetContext() (&_wglewctx)
#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
     static GLXEWContext _glxewctx;
#    define glxewGetContext() (&_glxewctx)
#  endif
#endif


// __JAGBASE_PLATFORM_GLEW_H__
#endif
