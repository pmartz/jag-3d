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

#ifndef __JAGDRAW_CONTEXT_SUPPORT_GLEW_H__
#define __JAGDRAW_CONTEXT_SUPPORT_GLEW_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/ContextSupport.h>
#include <jagDraw/PlatformOpenGL.h>



namespace jagDraw
{


/** \class ContextSupportGLEW ContextSupportGLEW.h <jagDraw/ContextSupportGLEW.h>

Still to-do:
\li Make GLEW_MX truly optional. This will require a change to the
CMakeModules/FindGLEW.cmake script to look for all variants of the GLEW library
and accept an option (from the CMake GUI) for which one to use, then also add
the GLEW_MX cpp definition as-needed automatically (instead of hardcoding it, as
is currently done in include/jagDraw/PlatformGLEW.h).
\li glewGetContext(), wglewGetContext(), and glxewGetContext() should be CPP
macros instead of functions. They are used in the GLEW_MX case, and currently
incur a function call per-OpenGL command; this is an unacceptable cost.
*/
class JAGDRAW_EXPORT ContextSupportGLEW : public ContextSupport
{
public:
    ContextSupportGLEW();

    /** Create the GLEWContext needed by GLEW_MX
    */
    virtual jagDrawContextID registerContext( const platformContextID pCtxId );

    /**
    */
    virtual bool setActiveContext( const jagDrawContextID contextID );

    /**
    */
    virtual bool initContext();

protected:
    virtual ~ContextSupportGLEW();

#ifdef GLEW_MX

    typedef struct {
        GLEWContext* _glewCtx;
#  ifdef _WIN32
        WGLEWContext* _wglCtx;
#  elif !defined(__APPLE__) || defined(GLEW_APPLE_GLX)
        GLXEWContext* _glxCtx;
#  endif
    } GLEWContextHandles;

    typedef PerContextData< GLEWContextHandles > GLEWContextMap;
    GLEWContextMap _glewContexts;

#endif
};


// jagDraw
}


// __JAGDRAW_CONTEXT_SUPPORT_GLEW_H__
#endif
