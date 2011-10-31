/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag-3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
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

