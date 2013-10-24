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

class Framebuffer;
typedef jagBase::ptr< jagDraw::Framebuffer >::shared_ptr FramebufferPtr;


/** \class Framebuffer Framebuffer.h <jagDraw/Framebuffer.h>
\brief Support for OpenGL Framebuffer objects
\details
\gl{section 4.4}.

\logname jag.draw.fbo

\specBegin Framebuffer

Framebuffer supports the following functionality:
\li Framebuffer object ID management.
\li Stores Framebuffer attachments (FramebufferAttachable).
\li Object ID management.

Each Framebuffer instance stores the following information:
\li A bind target (\a target parameter to glBindFramebuffer() ).
\li A list of buffers and their associated attachment points.
\li Viewport parameters and a viewport enable flag.
\li Clear mask bits and a clear enable flag.

\specTableBegin
\specLog{jag.draw.fbo}
\specThread{Unsafe}
\specGL{See execute()}
\specDepend{FramebufferAttachable}
\specUsage{TBD}
\specViolations{None}
\specTableEnd

See member functions for additional specification requirements.

\specEnd

*/
class JAGDRAW_EXPORT Framebuffer : public DrawablePrep,
        public ObjectID, protected jagBase::LogBase
{
public:
    Framebuffer( GLenum target=GL_FRAMEBUFFER, const std::string& logName=std::string( "" ) );
    Framebuffer( const Framebuffer& rhs );
    virtual ~Framebuffer();

    /** \brief TBD
    \details TBD */
    virtual DrawablePrepPtr clone() const { return( FramebufferPtr( new Framebuffer( *this ) ) ); }

    /** \brief Bind the framebuffer ID.
    \details This function is an override from DrawablePrep.

    This function obtains the per-context ID from getID(), which could issue OpenGL
    commands if necessary. After obtaining the ID, this function binds the Framebuffer
    ID, and attaches buffers if necessary. Then it optionally sets a viewport and
    clears, according to setViewport() and setClear().

    \specFuncBegin
    \specTableBegin
    \specThread{Context safe}
    \specGL{
    \code
    glBindFramebuffer();
    If viewport enabled:
        glViewport();
    If clear enabled:
        glClear();
    \endcode
    Other OpenGL commands might be issued indirectly via calls to
    getID() and attachAll().}
    \specViolations{glDrawBuffer() control is not yet specified.}
    \specTableEnd
    \specFuncEnd
    */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief TBD
    Override from ObjectID. */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );

    /** \brief TBD
    Override from ObjectID. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief TBD
    Override from ObjectID. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );


    /** \brief Specify a viewport
    \details If the viewport is enabled with the \c enable parameter, execute()
    issues a glViewport() call after binding the framebuffer.

    \specFuncBegin
    \specTableBegin
    \specGL{When enabled\, on each call to execute():
    \code
    glViewport( _vpX\, _vpY\, _vpWidth\, _vpHeight );
    \endcode }
    \specViolations{Need a way to change the viewport in the jagSG::CollectionVisitor.}
    \specTableEnd

    Member variable defaults:
    \li Framebuffer::_viewport = false (viewport not enabled)
    \li Framebuffer::_vpX = 0
    \li Framebuffer::_vpY = 0
    \li Framebuffer::_vpWidth = 0
    \li Framebuffer::_vpHeight = 0

    \specFuncEnd
    */
    void setViewport( const GLint x, const GLint y, const GLsizei width, const GLsizei height, const bool enable=true );
    /** \brief Specify clearing behavior.
    \details If clearing is enabled with the \c enable parameter, execute()
    issues a glClear() call after binding the framebuffer.

    \specFuncBegin
    \specTableBegin
    \specGL{When enabled\, on each call to execute():
    \code
    glClear( _clearMask );
    \endcode }
    \specTableEnd

    Member variable defaults:
    \li Framebuffer::_clear = false (clearing not enabled)
    \li Framebuffer::_clearMask = 0

    \specFuncEnd
    */
    void setClear( const GLbitfield mask, const bool enable=true );
    /** \brief TBD
    \details Default value is 0.f, 0.f, 0.f, 0.f. */
    void setClearColor( const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a );


    typedef std::map< GLenum, FramebufferAttachablePtr > AttachmentMap;

    /** \brief Specify Framebuffer object attachments.
    \details Associate a buffer with an attachment point.

    \c attachment can be one of:
    \li GL_COLOR_ATTACHMENTi
    \li GL_DEPTH_ATTACHMENT
    \li GL_STENCIL_ATTACHMENT
    \li GL_DEPTH_STENCIL_ATTACHMENT

    \gl{table 4.11}.

    \specFuncBegin
    All buffers are attached to their attachments points when the
    Framebuffer object ID is generated per context. Buffers are
    reattached as necessary (for example, if jagDraw client code
    calls setAttachment(), the following execute() call will
    effect this attachment change).

    \specTableBegin
    \specDepend{FramebufferArrachable}
    \specTableEnd
    \specFuncEnd
    */
    void addAttachment( const GLenum attachment, FramebufferAttachablePtr buffer );
    /** \brief Get Framebuffer object attachments. */
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


    bool _viewport, _clear;
    GLint _vpX, _vpY;
    GLsizei _vpWidth, _vpHeight;
    GLbitfield _clearMask;
    GLfloat _clearColor[ 4 ];
};

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
    Renderbuffer( const std::string& logName );
    Renderbuffer( const GLenum internalFormat=GL_NONE, const GLsizei width=0, const GLsizei height=0, const GLsizei samples=0, const std::string& logName=std::string( "" ) );

    Renderbuffer( const Renderbuffer& rhs );
    virtual ~Renderbuffer();

    /** \brief TBD
    Override from DrawablePrep. */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief TBD
    Override from ObjectID. */
    virtual GLuint getID( const jagDraw::jagDrawContextID contextID );

    /** \brief TBD
    \details Override from FramebufferAttachable.
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
