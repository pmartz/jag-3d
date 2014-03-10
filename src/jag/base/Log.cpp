/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <jag/base/Log.h>
#include <jag/base/LogBase.h>
#include <jag/base/LogMacros.h>

#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/Environment.h>
#include <Poco/String.h>

#include <iostream>


namespace jag {
namespace base {



Log* Log::instance()
{
    // TBD Need singleton manager to cleanup/delete singletons.
    Log* _s_instance( new jag::base::Log() );

    return( _s_instance );
}


Log::Log()
  : _initState( Uninitialized ),
    _logFileName( "jag3d.log" ),
    _console( NULL ),
    _file( NULL )
{
}
Log::~Log()
{
}

void Log::internalInit()
{
    if( _initState != Uninitialized )
        return;
    _initState = Initializing;

    //
    // Check for user specified log file name.
    //
    std::string logFileName;
    try {
        logFileName = Poco::Environment::get( "JAG3D_LOG_FILE_NAME" );
    } catch (...) {}
    if( !logFileName.empty() )
        _logFileName = logFileName;

    //
    // Check for user specified log message destination.
    // 
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

    //
    // Check for user specified global log priority.
    //
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

    //
    // Check for user specified per-module log priority.
    //
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

    _initState = Initialized;
}

void Log::setLogFileName( const std::string& logFileName )
{
    internalInit();

    if( _file != NULL )
    {
        JAG3D_WARNING_STATIC( "jag.base.log", "Can't change log file name. Log file is already open." );
        return;
    }
    _logFileName = logFileName;
}

void Log::setPriority( int prio, const std::string& logName )
{
    internalInit();

    Poco::Logger& logger( Poco::Logger::get( logName ) );
    logger.setLevel( prio );
}
void Log::setPriority( int prio, const DestinationType dest, const std::string& logName )
{
    internalInit();

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



// jag/base
}
}
