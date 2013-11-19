/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
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

#ifndef __JAGBASE_LOG_H__
#define __JAGBASE_LOG_H__ 1


#include <jagBase/Export.h>
#include <Poco/Message.h>
#include <string>


namespace Poco {
    class FormattingChannel;
}


namespace jagBase
{


/** \addtogroup jagBaseLogging Message Logging Utilities */
/*@{*/


/** \class Log Log.h <jagBase/Log.h>
\brief Interface for setting message logging parameters.
\details Log is a singleton.
This class provides a single interface for managing message logging.
It maintains pointers to the console and log file channels, allows specifying
a log file name, and allows specifying Logger priority and channel destinations.
Under the hood, it provides uniform message formatting for all Jag3D Loggers.
*/
class JAGBASE_EXPORT Log
{
public:
    static Log* instance();


    /** \brief Sets the default log file name.
    \details By default, the default log file name is "jag3d.log". You can change this
    default with the JAG3D_LOG_FILE_NAME environment variable, and change the name at
    run-time by calling this function prior to invocation of any other interaction with
    Jag3D. (Specifically, setLogFileName() must be called before creating a class
    derived from LogBase that uses LogFile as a destination.)

    \envvar JAG3D_LOG_FILE_NAME */
    void setLogFileName( const std::string& logFileName );

    enum {
        PrioSilent = 0,
        PrioFatal = Poco::Message::PRIO_FATAL,
        PrioCritical = Poco::Message::PRIO_CRITICAL,
        PrioError = Poco::Message::PRIO_ERROR,
        PrioWarning = Poco::Message::PRIO_WARNING,
        PrioNotice = Poco::Message::PRIO_NOTICE,
        PrioInfo = Poco::Message::PRIO_INFORMATION,
        PrioDebug = Poco::Message::PRIO_DEBUG,
        PrioTrace = Poco::Message::PRIO_TRACE
    };

    /** \brief TBD
    \details TBD
    */
    enum DestinationType {
        Console,
        LogFile
    };

    /** \brief Set the logging priority and logging destination.
    \details
    \param prio Logging verbosity. 0 is silent, 8 is verbose. These values
    map directly to the Poco logging priorities in Poco/Message.h. If
    setPriority() is never called, the default logging priority for all
    Jag3D modules is PrioWarning.
    \param dest Logging destination, either \c Console or \c LogFile.
    Specify the file name for \c LogFile using setLogFileName().
    \param logName Specifies the jag modules or class to have logging set
    at \c prio.

    \envvar JAG3D_LOG_PRIORITY
    \envvar JAG3D_LOG_PRIORITIES
    \envvar JAG3D_LOG_DESTINATION

    Note: The default priority for the global "jag" logger is PrioWarning, and the
    default destination is Console. The default can be changed using the
    JAG3D_LOG_PRIORITY and JAG3D_LOG_DESTINATION environment variables. For example:
    \code
    export JAG3D_LOG_PRIORITY=info
    export JAG3D_LOG_DESTINATION=logfile
    \endcode */
    void setPriority( int prio, const DestinationType dest, const std::string& logName="jag" );
    /** \overload
    Use this convenience overload to set the priority of a logger without
    modifying the DestinationType.

    \envvar JAG3D_LOG_PRIORITIES

    The default priority for any Logger comes from the global 'jag" Logger.
    The default for a specific Logger can changes using the JAG3D_LOG_PRIORITIES
    environment variable. JAG3D_LOG_PRIORITIES takes a comma-separated list of
    logger name / priority pairs. For example:
    \code
    export JAG3D_LOG_PRIORITIES=jag.base.version info,jag.draw.ctx debug,jag.disk critical
    \endcode
    */
    void setPriority( int prio, const std::string& logName="jag" );

    static std::string parsePriority( const int prio );
    static int parsePriority( const std::string& prio );

protected:
    Log();
    ~Log();
    static Log* _s_instance;

    std::string _logFileName;

    // TBD should these be smart pointers?
    Poco::FormattingChannel* _console;
    Poco::FormattingChannel* _file;
};

/*@}*/


// jagBase
}


// __JAGBASE_LOG_H__
#endif
