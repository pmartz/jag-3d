/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAG_DRAW_FRAMEBUFFER_H__
#define __JAG_DRAW_FRAMEBUFFER_H__ 1


#include <jag/draw/Export.h>
#include <jag/draw/PlatformOpenGL.h>
#include <jag/draw/Command.h>
#include <jag/draw/FramebufferAttachable.h>
#include <jag/draw/PerContextData.h>
#include <jag/draw/ObjectID.h>
#include <jag/base/LogBase.h>
#include <jag/base/ptr.h>

#include <map>


namespace jag {
namespace draw {


/** \addtogroup jagDrawGLCommands OpenGL Commands */
/*@{*/

class Renderbuffer;

class Framebuffer;
typedef jag::base::ptr< jag::draw::Framebuffer >::shared_ptr FramebufferPtr;


/** \class Framebuffer Framebuffer.h <jag/draw/Framebuffer.h>
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
class JAGDRAW_EXPORT Framebuffer : public Command,
        public ObjectID, protected jag::base::LogBase
{
public:
    Framebuffer( GLenum target=GL_FRAMEBUFFER, const std::string& logName=std::string( "" ) );
    Framebuffer( const Framebuffer& rhs );
    virtual ~Framebuffer();

    /** \brief TBD
    \details TBD */
    virtual CommandPtr clone() const { return( FramebufferPtr( new Framebuffer( *this ) ) ); }

    /** \brief Bind the framebuffer ID.
    \details This function is an override from Command.

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
    \details Override from ObjectID. */
    virtual GLuint getID( const jag::draw::jagDrawContextID contextID );

    /** \brief TBD
    \details Override from ObjectID. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief TBD
    \details Override from ObjectID. */
    virtual void deleteID( const jag::draw::jagDrawContextID contextID );


    /** \brief Specify a viewport
    \details If the viewport is enabled with the \c enable parameter, execute()
    issues a glViewport() call after binding the framebuffer.

    \specFuncBegin
    \specTableBegin
    \specGL{When enabled\, on each call to execute():
    \code
    glViewport( _vpX\, _vpY\, _vpWidth\, _vpHeight );
    \endcode }
    \specViolations{Need a way to change the viewport in the jag::sg::CollectionVisitor.}
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
    void cleanDirtyAttachments( const unsigned int contextID );

    GLenum _target;
    AttachmentMap _attachments;

    typedef std::map< GLenum, bool > DirtyAttachmentMap;
    typedef jag::draw::PerContextData< DirtyAttachmentMap > DirtyAttachments;
    DirtyAttachments _dirtyAttachments;


    bool _viewport, _clear;
    GLint _vpX, _vpY;
    GLsizei _vpWidth, _vpHeight;
    GLbitfield _clearMask;
    GLfloat _clearColor[ 4 ];
};

typedef std::vector< FramebufferPtr > FramebufferVec;


/** \class Renderbuffer Framebuffer.h <jag/draw/Framebuffer.h>
\brief TBD
\details TBD
\gl{section 4.4.2}.
*/
class JAGDRAW_EXPORT Renderbuffer : public ObjectID,
        public FramebufferAttachable, protected jag::base::LogBase
{
public:
    Renderbuffer( const std::string& logName );
    Renderbuffer( const GLenum internalFormat=GL_NONE, const GLsizei width=0, const GLsizei height=0, const GLsizei samples=0, const std::string& logName=std::string( "" ) );

    Renderbuffer( const Renderbuffer& rhs );
    virtual ~Renderbuffer();

    /** \brief TBD
    \details Override from Command. */
    virtual void execute( DrawInfo& drawInfo );

    /** \brief Check if the Renderbuffer is dirty for the given \c contextID.
    \details Override from base class FramebufferAttachable. */
    virtual bool isDirty( const unsigned int contextID ) const;

    /** \brief TBD
    \details Override from ObjectID. */
    virtual GLuint getID( const jag::draw::jagDrawContextID contextID );
    /** \brief TBD
    \details Override from ObjectID. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief TBD
    \details Override from FramebufferAttachable.
    \gl{section 4.4.2} */
    virtual void attachToFBO( const jag::draw::jagDrawContextID contextID, const GLenum attachment );

protected:
    void internalInit( const unsigned int contextID );

    GLsizei _samples;
    GLenum _internalFormat;
    GLsizei _width, _height;

    jag::draw::PerContextBool _dirty;
};

typedef jag::base::ptr< jag::draw::Renderbuffer >::shared_ptr RenderbufferPtr;
typedef std::vector< RenderbufferPtr > RenderbufferVec;


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_FRAMEBUFFER_H__
#endif
