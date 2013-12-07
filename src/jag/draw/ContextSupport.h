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

#ifndef __JAG_DRAW_CONTEXT_SUPPORT_H__
#define __JAG_DRAW_CONTEXT_SUPPORT_H__ 1


#include <jag/draw/Export.h>
#include <jag/draw/PerContextData.h>
#include <jag/base/LogBase.h>
#include <boost/thread/tss.hpp>
#include <boost/thread/mutex.hpp>
#include <map>



namespace jag {
namespace draw {


typedef unsigned int jagDrawContextID;
typedef uint64_t platformContextID;


/** \addtogroup OpenGLAbstraction Support for Abstract OpenGL Concepts

Jag3D supports some abstract OpenGL concepts. Some of these are mentioned in the
spec, others simply support OpenGL usage. But in general they don't map directly
to specific OpenGL commands.

Abstract objects include the following:

\section ContextSupport

Base class for per-platform jag::draw::ContextSupport classes. jag::draw::ContextSupport
allows a mapping of per-platform context IDs to \c jagDrawContextID (Jag3D's 0-based
internal context ID representation), tracks the current context for a given
thread, etc.

\section FramebufferAttachable

The "framebuffer-attachable" concept is defined in the spec.
\glinline{section 4.4}. jag::draw::FramebufferAttachable is a base class for the
two Jag3D classes that can be attached to a framebuffer object: jag::draw::Texture
and jag::draw::Renderbuffer.

\section Image

jag::draw::Image maintains a block of image data and parameters for describing
the format and type of that data. jag::draw::Texture holds a shared pointer to
a jag::draw::Image, for example. In the future, jag::draw::Image might be used to
support pixel operations other than texture mapping (such as glReadPixels).

\section ObjectID

Many OpenGL objects have GLuint object IDs, such as texture objects, framebuffer
objects, and vertex array objects. jag::draw::ObjectID serves as a base class for
such classes in Jag3D, stores an ID per context, and provedes methods for accessing
and deleting the IDs.

\section ObjectIDOwner

Some Jag3D classes act as containers for class instances that have
OpenGL object IDs. jag::draw::Drawable and jag::draw::UniformBlock, for example.
These classes derive from ObjectIDOwner, which probides methods to allow these
classes to maintain class instances that own object IDs.

Some Jag3D container classes also have their own OpenGL object ID. One example
is Framebuffer, which has an OpenGL object ID and also contains a list of
jag::draw::FramebufferAttachable class instances. Such classes should simply
derive from ObjectID rather than ObjectIDOwner. Deriving from both ObjectID
and ObjectIDOwner could produce multiple inheritance issues and is not
supported.

\section PerContextData

A thread-safe vector of per-context data. jag::draw::PerContextData is
used by ObjectID to store OpenGL object IDs per-context.
*/
/*@{*/

/** \class ContextSupport ContextSupport.h <jag/draw/ContextSupport.h>

Still to-do:
\li initContext() should not be necessary. ContextSupport can easily keep a std::set
of Jag3D contextIDs that have been passed as parameters to setActiveContext() and
thereby know when a context is being made active/current for the first time.
\li multicontext test program, using modified versions of qtSupport.cpp and
freeglutSupport.cpp that use multiple threads to open multiple windows and
render concurrently.
*/
class JAGDRAW_EXPORT ContextSupport : protected jag::base::LogBase
{
public:
    static ContextSupport* instance();


    /** \brief Call this function after creating a new context.
    \details Call this for each new context your application creates, passing
    in the platform-specific context ID. Jag3D maintains a 1-to-1 mapping of
    platform-specific context IDs and jagDrawContextID values. This function
    creates a unique jagDrawContextID value for the given \c pCtxTd and
    returns it. If \c pCtxId has already been registered, registerContext()
    displays a warning and returns the existing jagDrawContextID.
    */
    virtual jagDrawContextID registerContext( const platformContextID pCtxId );

    /** \brief Return the number of registered contexts. */
    unsigned int getNumRegisteredContexts() const { return( (unsigned int)( _contexts._data.size() ) ); }

    /** \brief Call this function after making a context current.
    \details Call this when a thread makes a context current or switches to
    a new context. Apps that use only one context need call this function
    only once, when it is made current for the first time.

    ContextSupport maintains a static boost::thread_specific_ptr for the
    current active context, \c _currentContext. This allows multiple
    threads to each have their own active context.
    \returns false if the \c contextID is invalid, true otherwise.
    */
    virtual bool setActiveContext( const jagDrawContextID contextID );

    /** \brief Returns the active context for the calling thread.
    */
    inline jagDrawContextID getActiveContext() const;

    /** \brief Call this function prior to executing OpenGL or Jag3D calls.
    \details After making a context current for the first time, and after
    calling setActiveContext(), call initContext(). This allows Jag3D to
    perform any context-specific initialization.
    \returns True on success, false on error.
    */
    virtual bool initContext();

    /** \brief Return the platform-specific context ID associated with the
    given Jag3D \c contextID.
    */
    platformContextID getPlatformContextID( const jagDrawContextID contextID ) const;
    /** \brief Return the jagDrawContextID associated with the given
    platform-specific context ID \c pCtxId.
    */
    jagDrawContextID getJagContextID( const platformContextID pCtxId ) const;

    /** Returns true if \c pCtxId was previously passed as a parameter to
    registerContext().
    */
    bool isRegistered( const platformContextID pCtxId ) const;

protected:
    ContextSupport();
    virtual ~ContextSupport();

    static std::string asString( const platformContextID pCtxId );
    static std::string asString( const jagDrawContextID contextID );

    boost::thread_specific_ptr< jagDrawContextID > _currentContext;

    typedef PerContextData< platformContextID > ContextMap;
    ContextMap _contexts;

    boost::mutex _mutex;
};

/*@}*/


inline jagDrawContextID ContextSupport::getActiveContext() const
{
    return( *_currentContext );
}


// namespace jag::draw::
}
}


// __JAG_DRAW_CONTEXT_SUPPORT_H__
#endif
