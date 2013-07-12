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
#if defined( JAG3D_USE_GL3W )
#  include <jagDraw/ContextSupportGl3w.h>
#endif

#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Format.h>
#include <boost/foreach.hpp>
#include <string>


using std::string;


namespace jagDraw
{


ContextSupport* ContextSupport::instance()
{
    // TBD need to register with singleton manager, which doesn't exist yet.
    // Note: Do registration in constructor.
    static ContextSupport* s_instance =
#if defined( JAG3D_USE_GL3W )
        new ContextSupportGl3w;
#else
        new ContextSupport;
#endif
    return( s_instance );
}

ContextSupport::ContextSupport()
  : jagBase::LogBase( "jag.draw.ctx" )
{
}
ContextSupport::~ContextSupport()
{
}


jagDrawContextID ContextSupport::registerContext( const platformContextID pCtxId )
{
    boost::mutex::scoped_lock lock( _mutex );

    const string idStr = asString( pCtxId );

    if( JAG3D_LOG_INFO )
    {
        string infoMsg( "registerContext: " );
        infoMsg.append( idStr );
        JAG3D_INFO( infoMsg );
    }

    jagDrawContextID contextID;

    if( isRegistered( pCtxId ) )
    {
        if( JAG3D_LOG_WARNING )
        {
            string msg( "registerContext: Context already registered: " );
            msg.append( idStr );
            JAG3D_WARNING( msg );
        }

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
    if( JAG3D_LOG_INFO )
    {
        string infoMsg( "setActiveContext: " );
        infoMsg.append( asString( contextID ) );
        JAG3D_INFO( infoMsg );
    }

    unsigned int idAsSize = static_cast< unsigned int >( contextID );
    if( idAsSize >= _contexts._data.size() )
    {
        if( JAG3D_LOG_ERROR )
        {
            string msg( "setActiveContext: Invalid contextID: " );
            msg.append( asString( contextID ) );
            JAG3D_ERROR( msg );
        }
        return( false );
    }

    jagDrawContextID* addr = new jagDrawContextID( contextID );
    _currentContext.reset( addr );
    return( true );
}

bool ContextSupport::initContext()
{
    JAG3D_TRACE( string( "initContext" ) );

    return( true );
}

platformContextID ContextSupport::getPlatformContextID( const jagDrawContextID contextID ) const
{
    unsigned int idAsSize = static_cast< unsigned int >( contextID );
    if( idAsSize < _contexts._data.size() )
        return( _contexts[ idAsSize ] );

    if( JAG3D_LOG_ERROR )
    {
        string msg( "getPlatformContextID: Unknown jagDraw contextID: " );
        msg.append( asString( contextID ) );
        JAG3D_ERROR( msg );
    }

    return( ( platformContextID )0 );
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

    if( JAG3D_LOG_ERROR )
    {
        string msg( "getJagContextID: Unknown platform contextID: " );
        msg.append( asString( pCtxId ) );
        JAG3D_ERROR( msg );
    }

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
