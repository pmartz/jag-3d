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

#include <Poco/Logger.h>
#include <boost/thread/tss.hpp>
#include <map>



namespace jagDraw
{


typedef unsigned int jagDrawContextID;
typedef void* platformContextID;


/**
*/
class JAGDRAW_EXPORT ContextSupport
{
public:
    static ContextSupport* instance();

    /**
    */
    jagDrawContextID registerContext( const platformContextID pCtxId );

    /**
    */
    bool setActiveContext( const jagDrawContextID contextID );

    /**
    */
    inline jagDrawContextID getActiveContext() const;

    /**
    */
    virtual bool initContext();

    /**
    */
    platformContextID getPlatformContextID( const jagDrawContextID contextID ) const;
    /**
    */
    jagDrawContextID ContextSupport::getJagContextID( const platformContextID pCtxId ) const;

protected:
    static ContextSupport* s_instance;

    ContextSupport();
    virtual ~ContextSupport();

    static std::string asString( const platformContextID pCtxId );
    static std::string asString( const jagDrawContextID contextID );

    boost::thread_specific_ptr< jagDrawContextID > _currentContext;

    typedef std::pair< platformContextID, jagDrawContextID > ContextPair;
    typedef std::map< platformContextID, jagDrawContextID > ContextMap;
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
