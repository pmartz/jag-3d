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

#include <jagBase/Log.h>
#include <jagBase/LogBase.h>
#include <jagBase/LogMacros.h>

#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/Environment.h>
#include <Poco/String.h>

#include <iostream>


namespace jagBase {



// TBD Need singleton manager to cleanup/delete singletons.
Log* Log::_s_instance( new jagBase::Log() );

Log* Log::instance()
{
    return( _s_instance );
}


Log::Log()
  : _logFileName( "jag3d.log" ),
    _console( NULL ),
    _file( NULL )
{
    std::string logFileName;
    try {
        logFileName = Poco::Environment::get( "JAG3D_LOG_FILE_NAME" );
    } catch (...) {}
    if( !logFileName.empty() )
        _logFileName = logFileName;

    DestinationType dest( Console );
    std::string destination;
    try {
        destination = Poco::Environment::get( "JAG3D_LOG_DESTINATION" );
    } catch (...) {}
    if( !destination.empty() )
    {
        destination = Poco::toLower( destination );
        if( destination == "console" )
            dest = Console;
        else if( destination == "logfile" )
            dest = LogFile;
        else
            std::cerr << "JAG3D_LOG_DESTINATION=" << destination << ": Unsupported log destination." << std::endl;
    }

    std::string globalPriority;
    try {
        globalPriority = Poco::Environment::get( "JAG3D_LOG_PRIORITY" );
    } catch (...) {}
    if( !globalPriority.empty() )
    {
        globalPriority = Poco::toLower( globalPriority );
        int prio( PrioWarning );
        if( globalPriority == "silent" )
            prio = PrioSilent;
        else if( globalPriority == "fatal" )
            prio = PrioFatal;
        else if( globalPriority == "critical" )
            prio = PrioCritical;
        else if( globalPriority == "error" )
            prio = PrioError;
        else if( globalPriority == "warning" )
            prio = PrioWarning;
        else if( globalPriority == "notice" )
            prio = PrioNotice;
        else if( globalPriority == "info" )
            prio = PrioInfo;
        else if( globalPriority == "debug" )
            prio = PrioDebug;
        else if( globalPriority == "trace" )
            prio = PrioTrace;
        else
            std::cerr << "JAG3D_LOG_LEVEL=" << globalPriority << ": Unsupported log priority." << std::endl;

        setPriority( prio, dest );
    }
    else
        setPriority( PrioWarning, dest );
}
Log::~Log()
{
}

void Log::setLogFileName( const std::string& logFileName )
{
    if( _file != NULL )
    {
        JAG3D_WARNING_STATIC( "jag.base.log", "Can't change log file name. Log file is already open." );
        return;
    }
    _logFileName = logFileName;
}

void Log::setPriority( int prio, const std::string& logName )
{
    Poco::Logger& logger( Poco::Logger::get( logName ) );
    logger.setLevel( prio );
}
void Log::setPriority( int prio, const DestinationType dest, const std::string& logName )
{
    Poco::Logger& logger( Poco::Logger::get( logName ) );
    logger.setLevel( prio );
    if( dest == Console )
    {
        if( _console == NULL )
        {
            _console = new Poco::FormattingChannel( new Poco::PatternFormatter("%s: %t") );
	        _console->setChannel( new Poco::ConsoleChannel );
	        _console->open();
        }
        logger.setChannel( _console );
    }
    else if( dest == LogFile )
    {
        if( _file == NULL )
        {
            _file = new Poco::FormattingChannel( new Poco::PatternFormatter("%Y-%m-%d %H:%M:%S.%c %N[%P]:%s:%q:\t%t") );
            _file->setChannel( new Poco::FileChannel( _logFileName ) );
            _file->open();
        }
        logger.setChannel( _file );
    }
}


// jagBase
}
