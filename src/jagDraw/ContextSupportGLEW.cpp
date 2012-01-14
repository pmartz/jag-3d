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

#include <jagDraw/ContextSupportGLEW.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/PlatformGLEW.h>
#include <jagDraw/Error.h>



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



namespace jagDraw {


ContextSupportGLEW::ContextSupportGLEW()
{
}
ContextSupportGLEW::~ContextSupportGLEW()
{
}

bool ContextSupportGLEW::initContext()
{
    ContextSupport::initContext();

# ifdef GLEW_MX
    glewContextInit( glewGetContext() );

#  ifdef _WIN32
    wglewContextInit( wglewGetContext() );
#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
    glxewContextInit( glxewGetContext() );
#  endif

# else
    glewInit();
# endif

    JAG_ERROR_CHECK( "jagDraw::init()" );

    return( true );
}


// jagDraw
}
