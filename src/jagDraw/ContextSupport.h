/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAGDRAW_CONTEXT_SUPPORT_H__
#define __JAGDRAW_CONTEXT_SUPPORT_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PerContextData.h>
#include <jagBase/LogBase.h>
#include <boost/thread/tss.hpp>
#include <boost/thread/mutex.hpp>
#include <map>



namespace jagDraw
{


typedef unsigned int jagDrawContextID;
typedef uint64_t platformContextID;


/** \addtogroup OpenGLAbstraction Support for Abstract OpenGL Concepts

Jag3D supports some abstract OpenGL concepts. Some of these are mentioned in the
spec, others simply support OpenGL usage. But in general they don't map directly
to specific OpenGL commands.

Abstract objects include the following:

\section ContextSupport

Base class for per-platform jagDraw::ContextSupport classes. jagDraw::ContextSupport
allows a mapping of per-platform context IDs to \c jagDrawContextID (Jag3D's 0-based
internal context ID representation), tracks the current context for a given
thread, etc.

\section FramebufferAttachable

The "framebuffer-attachable" concept is defined in the spec.
\glinline{section 4.4}. jagDraw::FramebufferAttachable is a base class for the
two Jag3D classes that can be attached to a framebuffer object: jagDraw::Texture
and jagDraw::Renderbuffer.

\section Image

jagDraw::Image maintains a block of image data and parameters for describing
the format and type of that data. jagDraw::Texture holds a shared pointer to
a jagDraw::Image, for example. In the future, jagDraw::Image might be used to
support pixel operations other than texture mapping (such as glReadPixels).

\section ObjectID

Many OpenGL objects have GLuint object IDs, such as texture objects, framebuffer
objects, and vertex array objects. jagDraw::ObjectID serves as a base class for
such classes in Jag3D, stores an ID per context, and provedes methods for accessing
and deleting the IDs.

\section ObjectIDOwner

Some Jag3D classes act as containers for class instances that have
OpenGL object IDs. jagDraw::Drawable and jagDraw::UniformBlock, for example.
These classes derive from ObjectIDOwner, which probides methods to allow these
classes to maintain class instances that own object IDs.

Some Jag3D container classes also have their own OpenGL object ID. One example
is Framebuffer, which has an OpenGL object ID and also contains a list of
jagDraw::FramebufferAttachable class instances. Such classes should simply
derive from ObjectID rather than ObjectIDOwner. Deriving from both ObjectID
and ObjectIDOwner could produce multiple inheritance issues and is not
supported.

\section PerContextData

A thread-safe vector of per-context data. jagDraw::PerContextData is
used by ObjectID to store OpenGL object IDs per-context.
*/
/*@{*/

/** \class ContextSupport ContextSupport.h <jagDraw/ContextSupport.h>

Still to-do:
\li initContext() should not be necessary. ContextSupport can easily keep a std::set
of Jag3D contextIDs that have been passed as parameters to setActiveContext() and
thereby know when a context is being made active/current for the first time.
\li multicontext test program, using modified versions of qtSupport.cpp and
freeglutSupport.cpp that use multiple threads to open multiple windows and
render concurrently.
*/
class JAGDRAW_EXPORT ContextSupport : protected jagBase::LogBase
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


// jagDraw
}


// __JAGDRAW_CONTEXT_SUPPORT_H__
#endif
