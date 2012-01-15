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

#include <boost/thread/tss.hpp>


#ifdef GLEW_MX

boost::thread_specific_ptr< GLEWContext > s_glewCtx;

GLEWContext* glewGetContext()
{
    return( s_glewCtx.get() );
}


#  ifdef _WIN32

boost::thread_specific_ptr< WGLEWContext > s_wglCtx;

WGLEWContext* wglewGetContext()
{
    return( s_wglCtx.get() );
}

#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)

boost::thread_specific_ptr< GLXEWContext > s_glxCtx;

GLXEWContext* glxewGetContext()
{
    return( s_glxCtx.get() );
}

#  endif

#endif



namespace jagDraw {


ContextSupportGLEW::ContextSupportGLEW()
{
}
ContextSupportGLEW::~ContextSupportGLEW()
{
}

jagDrawContextID ContextSupportGLEW::registerContext( const platformContextID pCtxId )
{
    jagDrawContextID contextID = ContextSupport::registerContext( pCtxId );

#ifdef GLEW_MX

    // TBD trace msg

    const unsigned int idAsSize = static_cast< unsigned int >( contextID );
    if( idAsSize < _glewContexts._data.size() )
        return( contextID );

    _glewContexts._data.resize( idAsSize + 1 );
    GLEWContextHandles& ctxs = _glewContexts._data[ idAsSize ];

    ctxs._glewCtx = new GLEWContext;
#  ifdef _WIN32
    ctxs._wglCtx = new WGLEWContext;
#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
    ctxs._glxCtx = new GLXEWContext;
#  endif

#endif

    return( contextID );
}

bool ContextSupportGLEW::setActiveContext( const jagDrawContextID contextID )
{
    bool result = ContextSupport::setActiveContext( contextID );
    if( !result )
        return( result );

#ifdef GLEW_MX

    // TBD trace msg

    const unsigned int idAsSize = static_cast< unsigned int >( contextID );
    GLEWContextHandles& ctxs = _glewContexts._data[ idAsSize ];

    s_glewCtx.reset( ctxs._glewCtx );

#  ifdef _WIN32
    s_wglCtx.reset( ctxs._wglCtx );
#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
    s_glxCtx.reset( ctxs._glxCtx );
#  endif

#endif

    return( result );
}

bool ContextSupportGLEW::initContext()
{
    bool result = ContextSupport::initContext();
    if( !result )
        return( result );

#ifdef GLEW_MX

    glewContextInit( glewGetContext() );

#  ifdef _WIN32
    wglewContextInit( wglewGetContext() );
#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
    glxewContextInit( glxewGetContext() );
#  endif

#else

    glewInit();

#endif

    JAG_ERROR_CHECK( "jagDraw::init()" );

    return( result );
}


// jagDraw
}
