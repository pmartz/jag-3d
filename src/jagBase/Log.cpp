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
        int prio( parsePriority( globalPriority ) );
        if( prio < 0 )
            prio = PrioWarning;
        setPriority( prio, dest );
    }
    else
        setPriority( PrioWarning, dest );


    // Special default priority for profiling.
    Log::instance()->setPriority( Log::PrioInfo, "jag.prof" );

    std::string priorities;
    try {
        priorities = Poco::Environment::get( "JAG3D_LOG_PRIORITIES" );
    } catch (...) {}
    while( !priorities.empty() )
    {
        size_t commaPos( priorities.find_first_of( ',' ) );
        std::string logPrioPair;
        if( commaPos != std::string::npos )
        {
            logPrioPair = priorities.substr( 0, commaPos );
            priorities = priorities.substr( commaPos+1 );
        }
        else
        {
            logPrioPair = priorities;
            priorities = "";
        }

        size_t spacePos( logPrioPair.find_first_of( ' ' ) );
        std::string logName( logPrioPair.substr( 0, spacePos ) );
        std::string priority( logPrioPair.substr( spacePos+1 ) );
        int prio( parsePriority( priority ) );
        if( prio < 0 )
            prio = PrioSilent;
        setPriority( prio, logName );
    }
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


std::string Log::parsePriority( const int prio )
{
    std::string returnVal;
    switch( prio )
    {
    case PrioSilent: returnVal = "silent"; break;
    case PrioFatal: returnVal = "fatal"; break;
    case PrioCritical: returnVal = "critical"; break;
    case PrioError: returnVal = "error"; break;
    case PrioWarning: returnVal = "warning"; break;
    case PrioNotice: returnVal = "notice"; break;
    case PrioInfo: returnVal = "info"; break;
    case PrioDebug: returnVal = "debug"; break;
    case PrioTrace: returnVal = "trace"; break;
    default: std::cerr << prio << ": Unsupported log priority." << std::endl;
    }
    return( returnVal );
}
int Log::parsePriority( const std::string& prio )
{
    int returnVal( -1 );
    const std::string lcPrio = Poco::toLower( prio );
    if( lcPrio == "silent" )
        returnVal = PrioSilent;
    else if( lcPrio == "fatal" )
        returnVal = PrioFatal;
    else if( lcPrio == "critical" )
        returnVal = PrioCritical;
    else if( lcPrio == "error" )
        returnVal = PrioError;
    else if( lcPrio == "warning" )
        returnVal = PrioWarning;
    else if( lcPrio == "notice" )
        returnVal = PrioNotice;
    else if( lcPrio == "info" )
        returnVal = PrioInfo;
    else if( lcPrio == "debug" )
        returnVal = PrioDebug;
    else if( lcPrio == "trace" )
        returnVal = PrioTrace;
    else
        std::cerr << prio << ": Unsupported log priority." << std::endl;
    return( returnVal );
}



// jagBase
}
