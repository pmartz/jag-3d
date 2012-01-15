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


/**
*/
class JAGDRAW_EXPORT ContextSupportGLEW : public ContextSupport
{
public:
    ContextSupportGLEW();

    /**
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
