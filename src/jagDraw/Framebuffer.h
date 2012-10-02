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

#ifndef __JAGDRAW_FRAMEBUFFER_H__
#define __JAGDRAW_FRAMEBUFFER_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/FramebufferAttachable.h>
#include <jagDraw/PerContextData.h>
#include <jagDraw/ObjectID.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>

#include <map>


namespace jagDraw {


class Renderbuffer;


/** \class Framebuffer Framebuffer.h <jagDraw/Framebuffer.h>
\brief TBD
\details TBD
\gl{section 4.4}.
*/
class JAGDRAW_EXPORT Framebuffer : public DrawablePrep,
        public ObjectID, protected jagBase::LogBase
{
public:
    Framebuffer( GLenum target=GL_FRAMEBUFFER );
    Framebuffer( const Framebuffer& rhs );
    virtual ~Framebuffer();

    /** \brief TBD
    Override from DrawablePrep. */
    virtual void operator()( DrawInfo& drawInfo );

    /** \brief TBD
    Override from ObjectID. */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );

    /** \brief TBD
    Override from ObjectID. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief TBD
    Override from ObjectID. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );


    typedef std::map< GLenum, FramebufferAttachablePtr > AttachmentMap;

    /** \brief TBD
    \details \c attachment can be one of:
    \li GL_COLOR_ATTACHMENTi
    \li GL_DEPTH_ATTACHMENT
    \li GL_STENCIL_ATTACHMENT
    \li GL_DEPTH_STENCIL_ATTACHMENT
    \gl{table 4.11}.
    */
    void addAttachment( const GLenum attachment, FramebufferAttachablePtr buffer );
    FramebufferAttachablePtr getAttachment( const GLenum attachment );

protected:
    void internalInit( const unsigned int contextID );
    void attachAll( const unsigned int contextID );

    void dirtyAttachmentForAllContexts( const GLenum attachment );
    void dirtyAllAttachments( const unsigned int contextID, const bool dirty=true );
    bool anyDirty( const unsigned int contextID ) const;

    GLenum _target;
    AttachmentMap _attachments;

    typedef std::map< GLenum, bool > DirtyAttachmentMap;
    typedef jagDraw::PerContextData< DirtyAttachmentMap > DirtyAttachments;
    DirtyAttachments _dirtyAttachments;
};

typedef jagBase::ptr< jagDraw::Framebuffer >::shared_ptr FramebufferPtr;
typedef std::vector< FramebufferPtr > FramebufferVec;


/** \class Renderbuffer Framebuffer.h <jagDraw/Framebuffer.h>
\brief TBD
\details TBD
\gl{section 4.4.2}.
*/
class JAGDRAW_EXPORT Renderbuffer : public ObjectID,
        public FramebufferAttachable, protected jagBase::LogBase
{
public:
    Renderbuffer( const GLenum internalFormat=GL_NONE, const GLsizei width=0, const GLsizei height=0, const GLsizei samples=0 );

    Renderbuffer( const Renderbuffer& rhs );
    virtual ~Renderbuffer();

    /** \brief TBD
    Override from DrawablePrep. */
    virtual void operator()( DrawInfo& drawInfo );

    /** \brief TBD
    Override from ObjectID. */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );

    /** \brief TBD
    \detauls Override from FramebufferAttachable.
    \gl{section 4.4.2} */
    virtual void attachToFBO( const jagDraw::jagDrawContextID contextID, const GLenum attachment );

protected:
    void internalInit( const unsigned int contextID );

    GLsizei _samples;
    GLenum _internalFormat;
    GLsizei _width, _height;
};

typedef jagBase::ptr< jagDraw::Renderbuffer >::shared_ptr RenderbufferPtr;
typedef std::vector< RenderbufferPtr > RenderbufferVec;


// jagDraw
}


// __JAGDRAW_FRAMEBUFFER_H__
#endif
