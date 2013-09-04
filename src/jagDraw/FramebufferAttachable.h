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

#ifndef __JAGDRAW_FRAMEBUFFER_ATTACHABLE_H__
#define __JAGDRAW_FRAMEBUFFER_ATTACHABLE_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/ContextSupport.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/ptr.h>


namespace jagDraw {


/** \addtogroup OpenGLAbstraction Support for OpenGL Abstract Concepts */
/*@{*/

/** \class FramebufferAttachable FramebufferAttachable.h <jagDraw/FramebufferAttachable.h>
\brief TBD
\details TBD
\gl{section 4.4}.
*/
class JAGDRAW_EXPORT FramebufferAttachable
{
public:
    FramebufferAttachable( GLenum fboTarget=GL_DRAW_FRAMEBUFFER, GLint fboTextureLevel=0 )
      : _fboTarget( fboTarget ),
        _fboTextureLevel( fboTextureLevel )
    {}
    FramebufferAttachable( const FramebufferAttachable& rhs )
      : _fboTextureLevel( rhs._fboTextureLevel )
    {}
    virtual ~FramebufferAttachable()
    {}

    /** \brief TBD
    \details TBD */
    virtual void attachToFBO( const jagDraw::jagDrawContextID contextID, const GLenum attachment ) = 0;

    /** \brief TBD
    \details TBD */
    void setFBOTextureLevel( GLint fboTextureLevel )
    {
        _fboTextureLevel = fboTextureLevel;
    }
    GLint getFBOTextureLevel() const
    {
        return( _fboTextureLevel );
    }

protected:
    GLenum _fboTarget;
    GLint _fboTextureLevel;
};

/*@}*/

typedef jagBase::ptr< jagDraw::FramebufferAttachable >::shared_ptr FramebufferAttachablePtr;
typedef std::vector< FramebufferAttachablePtr > FramebufferAttachableVec;


// jagDraw
}


// __JAGDRAW_FRAMEBUFFER_ATTACHABLE_H__
#endif
