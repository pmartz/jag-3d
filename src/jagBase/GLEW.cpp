// Copyright

#include <jagBase/PlatformGLEW.h>


// TBD
// Eventually, need to properly support multiple threads / contexts.
// For now, assume just one.


static GLEWContext s_glewCtx;

GLEWContext* glewGetContext()
{
    return( &s_glewCtx );
}


#ifdef _WIN32


static WGLEWContext s_wglewCtx;

WGLEWContext* wglewGetContext()
{
    return( &s_wglewCtx );
}


#elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)


static GLXEWContext s_glxewCtx;

GLXEWContext* glxewGetContext()
{
    return( &s_glxewCtx );
}


#endif

