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

#include <jagDraw/ContextSupport.h>
#if defined( JAG3D_USE_GL3W )
#  include <jagDraw/ContextSupportGl3w.h>
#endif

#include <Poco/Foundation.h>
#if( POCO_OS == POCO_OS_MAC_OS_X )
// Include this first to work around OSX gcc 4.2 build issue
// Put isinf(), isnan() in global namespace, which Poco assumes. 
#include <cmath>
using std::isinf;
using std::isnan;
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
