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

#ifndef __JAGBASE_LOG_BASE_H__
#define __JAGBASE_LOG_BASE_H__ 1


#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/NullChannel.h>
#include <jagBase/ptr.h>


namespace jagBase
{


typedef jagBase::ptr< Poco::LogStream >::shared_ptr PocoLogStreamPtr;


/** \class LogBase LogBase.h <jagBase/LogBase.h>
\brief
\details
*/
class LogBase
{
public:
    LogBase( const std::string& loggerName )
#ifdef JAG3D_DISABLE_LOGGING
      : _logger( Poco::Logger::get( "" ) ), // Must init a reference, even if we're not using it.
        _logStream( NULL )
    {
        _logger.setChannel( Poco::NullChannel() );
#else
      : _logger( Poco::Logger::get( loggerName ) ),
        _logStream( PocoLogStreamPtr( new Poco::LogStream( _logger ) ) )
    {
#endif
    }

    ~LogBase() {}

    Poco::Logger& _logger;
    PocoLogStreamPtr _logStream;
};


// jagBase
}


// __JAGBASE_LOG_BASE_H__
#endif
