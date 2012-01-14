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

#include <jagDraw/ContextSupport.h>
#include <jagDraw/ContextSupportGLEW.h>

#include <jagBase/Log.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Format.h>
#include <boost/foreach.hpp>
#include <string>


using std::string;


namespace jagDraw
{


ContextSupport* ContextSupport::s_instance( NULL );

ContextSupport* ContextSupport::instance()
{
    if( s_instance == NULL )
    {
#ifdef JAG3D_USE_GLEW
        s_instance = new ContextSupportGLEW;
#else
        s_instance = new ContextSupport;
#endif
    }
    return( s_instance );
}

ContextSupport::ContextSupport()
{
    _logger = Poco::Logger::has( "jag3d.jagDraw.Ctx" );
    if( _logger == NULL )
    {
        _logger = &( Poco::Logger::create( "jag3d.jagDraw.Ctx",
                (Poco::Channel*)( jagBase::Log::instance()->getConsole() ), Poco::Message::PRIO_TRACE ) );
    }
}
ContextSupport::~ContextSupport()
{
}


jagDrawContextID ContextSupport::registerContext( const platformContextID pCtxId )
{
    // TBD Use boost exclusive lock here.

    const string idStr = asString( pCtxId );

    string infoMsg( "registerContext: " );
    infoMsg.append( idStr );
    _logger->information( infoMsg );

    jagDrawContextID contextID;

    if( _contexts.find( pCtxId ) != _contexts.end() )
    {
        string msg( "registerContext: Context already registered: " );
        msg.append( idStr );
        _logger->error( msg );

        contextID = _contexts[ pCtxId ];
    }
    else
    {
        contextID = static_cast< jagDrawContextID >( _contexts.size() );
        _contexts[ pCtxId ] = contextID;
    }

    return( contextID );
}

bool ContextSupport::setActiveContext( const jagDrawContextID contextID )
{
    const string idStr = asString( contextID );

    string infoMsg( "setActiveContext: " );
    infoMsg.append( idStr );
    _logger->information( infoMsg );

    ContextMap::size_type idAsSize = static_cast< ContextMap::size_type >( contextID );
    if( idAsSize >= _contexts.size() )
    {
        string msg( "setActiveContext: Invalid contextID: " );
        msg.append( idStr );
        _logger->error( msg );

        return( false );
    }

    jagDrawContextID* addr = new jagDrawContextID( contextID );
    _currentContext.reset( addr );
    return( true );
}

bool ContextSupport::initContext()
{
    _logger->information( string( "initContext" ) );

    return( true );
}

platformContextID ContextSupport::getPlatformContextID( const jagDrawContextID contextID ) const
{
    BOOST_FOREACH( ContextPair pair, _contexts )
    {
        if( pair.second == contextID )
            return( pair.first );
    }

    string msg( "getPlatformContextID: Unknown jagDraw contextID: " );
    msg.append( asString( contextID ) );
    _logger->error( msg );

    return( NULL );
}

jagDrawContextID ContextSupport::getJagContextID( const platformContextID pCtxId ) const
{
    jagDrawContextID contextID( 0 );

    ContextMap::const_iterator itr = _contexts.find( pCtxId );
    if( itr == _contexts.end() )
    {
        string msg( "getJagContextID: Unknown platform contextID: " );
        msg.append( asString( pCtxId ) );
        _logger->error( msg );
    }
    else
    {
        contextID = itr->second;
    }

    return( contextID );
}


std::string ContextSupport::asString( const platformContextID pCtxId )
{
    string idStr;
    // There must be a better way to format the pCtxId. But it's a platform-specific
    // value. For now we just assume the size if either 4 or 8 bytes.
    if( sizeof( platformContextID ) == 4 )
        Poco::format( idStr, "%08x", (unsigned int) pCtxId );
    else
        Poco::format( idStr, "%016x", (unsigned long) pCtxId );
    return( idStr );
}
std::string ContextSupport::asString( const jagDrawContextID contextID )
{
    string idStr;
    // There must be a better way to format the pCtxId. But it's a platform-specific
    // value. For now we just assume the size if either 4 or 8 bytes.
    if( sizeof( jagDrawContextID ) == 4 )
        Poco::format( idStr, "%08x", contextID );
    else
        Poco::format( idStr, "%016x", contextID );
    return( idStr );
}


// jagDraw
}
