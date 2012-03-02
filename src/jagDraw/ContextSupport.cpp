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
#include <jagDraw/ContextSupportGl3w.h>

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
#elif defined( JAG3D_USE_GL3W )
        s_instance = new ContextSupportGl3w;
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

    if( isRegistered( pCtxId ) )
    {
        string msg( "registerContext: Context already registered: " );
        msg.append( idStr );
        _logger->warning( msg );

        contextID = getJagContextID( pCtxId );
    }
    else
    {
        contextID = static_cast< jagDrawContextID >( _contexts._data.size() );
        _contexts._data.resize( contextID + 1 );
        _contexts[ contextID ] = pCtxId;
    }

    return( contextID );
}

bool ContextSupport::setActiveContext( const jagDrawContextID contextID )
{
    const string idStr = asString( contextID );

    string infoMsg( "setActiveContext: " );
    infoMsg.append( idStr );
    _logger->information( infoMsg );

    unsigned int idAsSize = static_cast< unsigned int >( contextID );
    if( idAsSize >= _contexts._data.size() )
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
    unsigned int idAsSize = static_cast< unsigned int >( contextID );
    if( idAsSize < _contexts._data.size() )
        return( _contexts[ idAsSize ] );

    string msg( "getPlatformContextID: Unknown jagDraw contextID: " );
    msg.append( asString( contextID ) );
    _logger->error( msg );

    return( NULL );
}

jagDrawContextID ContextSupport::getJagContextID( const platformContextID pCtxId ) const
{
    unsigned int idx( 0 );
    BOOST_FOREACH( platformContextID value, _contexts._data )
    {
        if( value == pCtxId )
            return( static_cast< jagDrawContextID >( idx ) );
        idx++;
    }

    string msg( "getJagContextID: Unknown platform contextID: " );
    msg.append( asString( pCtxId ) );
    _logger->error( msg );

    return( static_cast< jagDrawContextID >( 0 ) );
}


std::string ContextSupport::asString( const platformContextID pCtxId )
{
    string idStr;
    Poco::format( idStr, "%016x", (unsigned int)pCtxId );
    return( idStr );
}
std::string ContextSupport::asString( const jagDrawContextID contextID )
{
    string idStr;
    Poco::format( idStr, "%016x", contextID );
    return( idStr );
}

bool ContextSupport::isRegistered( const platformContextID pCtxId ) const
{
    BOOST_FOREACH( platformContextID value, _contexts._data )
    {
        if( value == pCtxId )
            return( true );
    }
    return( false );
}


// jagDraw
}
