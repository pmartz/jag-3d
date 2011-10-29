// Copyright

#ifndef __JAGBASE_PLATFORM_GLEW_H__
#define __JAGBASE_PLATFORM_GLEW_H__ 1

#include <jagBase/Export.h>


#define GLEW_MX

#include <GL/glew.h>

#if defined( _WIN32 )
#  include <GL/wglew.h>
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
#  include <GL/glxew.h>
#endif


JAGBASE_EXPORT GLEWContext* glewGetContext();

#ifdef _WIN32
    JAGBASE_EXPORT WGLEWContext* wglewGetContext();
#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
    GLXEWContext* glxewGetContext();
#endif

// __JAGBASE_PLATFORM_GLEW_H__
#endif
