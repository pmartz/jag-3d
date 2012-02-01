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

#ifndef __JAGDRAW_CONTEXT_SUPPORT_H__
#define __JAGDRAW_CONTEXT_SUPPORT_H__ 1


#include <jagDraw/Export.h>
#include <jagDraw/PerContextData.h>

#include <Poco/Logger.h>
#include <boost/thread/tss.hpp>
#include <map>



namespace jagDraw
{


typedef unsigned int jagDrawContextID;
typedef GLint platformContextID;


/** \class ContextSupport ContextSupport.h <jagDraw/ContextSupport.h>

Still to-do:
\li initContext() should not be necessary. ContextSupport can easily keep a std::set
of JAG contextIDs that have been passed as parameters to setActiveContext() and
thereby know when a context is being made active/current for the first time.
\li multicontext test program, using modified versions of qtSupport.cpp and
freeglutSupport.cpp that use multiple threads to open multiple windows and
render concurrently.
*/
class JAGDRAW_EXPORT ContextSupport
{
public:
    static ContextSupport* instance();

    /** \brief Call this function after creating a new context.
    \details Call this for each new context your application creates, passing
    in the platform-specific context ID. JAG maintains a 1-to-1 mapping of
    platform-specific context IDs and jagDrawContextID values. This function
    creates a unique jagDrawContextID value for the given \c pCtxTd and
    returns it. If \c pCtxId has already been registered, registerContext()
    displays a warning and returns the existing jagDrawContextID.
    */
    virtual jagDrawContextID registerContext( const platformContextID pCtxId );

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

    /** \brief Call this function prior to executing OpenGL or JAG calls.
    \details After making a context current for the first time, and after
    calling setActiveContext(), call initContext(). This allows JAG to
    perform any context-specific initialization.
    \returns True on success, false on error.
    */
    virtual bool initContext();

    /** \brief Return the platform-specific context ID associated with the
    given JAG \c contextID.
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
    static ContextSupport* s_instance;

    ContextSupport();
    virtual ~ContextSupport();

    static std::string asString( const platformContextID pCtxId );
    static std::string asString( const jagDrawContextID contextID );

    boost::thread_specific_ptr< jagDrawContextID > _currentContext;

    typedef PerContextData< platformContextID > ContextMap;
    ContextMap _contexts;

    Poco::Logger* _logger;
};


inline jagDrawContextID ContextSupport::getActiveContext() const
{
    return( *_currentContext );
}


// jagDraw
}


// __JAGDRAW_CONTEXT_SUPPORT_H__
#endif
