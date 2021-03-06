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

#ifndef __JAG_BASE_LOG_MACROS_H__
#define __JAG_BASE_LOG_MACROS_H__ 1


#include <jag/base/Config.h>
#include <jag/base/Log.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>


/** \addtogroup jagBaseLogging Message Logging Utilities
\details Jag3D's message logging system is based on
<a href="http://pocoproject.org/slides/110-Logging.pdf" target=null>Poco</a>
and is modeled after
<a href="http://www.vesuite.org/" target=null>VE-Suite's</a> message logging system.

Jag3D's modules and classes are arranged in a heirarchy to allow message destination (console
or log file) and verbosity on a per-module and per-class basis. The heirarchy is arranged
as follows:
<ul>
  <li>jag
  <ul>
    <li>base (the jag/base module)
    <ul>
      <li>\link jag::base::Log log \endlink
      <li>\ref jagBaseVersion "version"
    </ul>
    <li>draw (the jagDraw module)
    <ul>
      <li>\link jag::draw::BufferObject bufobj \endlink
      <li>\link jag::draw::ContextSupport ctx \endlink
      <li>\link jag::draw::Drawable drawable \endlink
      <li>\ref jagDrawError "glerror"
      <li>\link jag::draw::Program program \endlink
      <li>\link jag::draw::Shader shader \endlink
    </ul>
    <li>sg (the jagSG module)
    <li>util (the jagUtil module)
    <li>demo (demos, tests, examples)
    <ul>
      <li>various demos and tests
    </ul>
  </ul>
</ul>

Heirachy levels are separated by a period, so the name for the jag::draw::Drawable Poco::Logger is
\c jag.draw.drawable.

Priority level match those used by Poco and are defined as follows:
\li 0 - Silent (no message logging)
\li 1 - Fatal
\li 2 - Critical
\li 3 - Error
\li 4 - Warning
\li 5 - Notice
\li 6 - Info
\li 7 - Debug
\li 8 - Trace

Set a Poco::Logger's priority and destination (console or log file) using the jag::base::Log class.
\code
    // Set the global priority to 3 (error and above) and destination to Console.
    // There is an implicit 3rd parameter of "jag".
    jag::base::Log::instance()->setPriority( 3, jag::base::Log::Console );

    // Set the jagDraw module to priority 3 (error and above)
    jag::base::Log::instance()->setPriority( 4, "jag.draw" );

    // Send any OpenGL errors to the log file.
    jag::base::Log::instance()->setPriority( 3, jag::base::Log::LogFile, "jag.draw.glerror" );

    // Turn on debugging for jagDraw context handling.
    jag::base::Log::instance()->setPriority( 8, "jag.draw.ctx" );
\endcode

Poco Logger priority and destination inherit down the heirarchy. Your application should
set specific priority and destinations (as in the code above) at init time, before
Jag3D class constructors obtain references to subordinate Loggers.

By default, the root Logger ("jag") is configured with priority 0 (silent) and
destination jag::base::Log::Console.

Jag3D support environment variables to change log defaults. To set the root Logger to
priority trace, export set JAG3D_LOG_PRIORITY to priority "notice":
\code
export JAG3D_LOG_PRIORITY=notice
\endcode
To set multiple Logger priorities, use JAG3D_LOG_PRIORITIES. For example:
\code
export JAG3D_LOG_PRIORITIES=jag.base.version info,jag.draw.ctx debug,jag.disk critical
\endcode
Note that environment variable values are case insensitive.

The default log file name is "jag3d.log" and Jag3D writes it to the current directory.
You can change the default log file name with either of the following methods:
\li Call jag::base::Log::setLogFileName( const std::string& ).
\li Set the environment variable JAG3D_LOG_FILE_NAME to the path and file name prior
to invoking jag::base::Log::instance().

Jag3D internal code uses the macros defined in jag/base/logMatrix.h to display log messages at
verious levels. When using these macros, please observe the following rules of thumb:
\li Use the JAG3D_LOG_<prio> macros to avoid expensive message construction for messages that will not be logged due to their priority.
\li Use the JAG3D_<prio> macros from member functions of classes that derive from jag::base::LogBase.
\li Use the JAG3D_<prio>_STATIC macros from static functions, or from member functions of classes that don't derive from jag::base::LogBase.

Note that, even with priority set to 0, there is some non-zero overhead in handling
message logging. To eliminate all Jag3D message logging, set the CMake variable
JAG3D_ENABLE_LOGGING to OFF (or deselect its checkbox in cmake-gui). When this variable
is off, Jag3D's message logging facilities become no-ops and have zero computational cost.
Jag3d-dependent downstream projects can determine whether logging is enabled using
the Config.h header:
\code
#include <jag/base/Config.h>
\endcode
This file defines, or undefines, the JAG3D_ENABLE_LOGGING CPP variable as appropriate
based on how CMake was configured when Jag3d was built.

*/
/*@{*/


/** \def __JAG3D_LOG
\brief Shared internal logging macro.
\details Internal macro for efficient message logging for classes derived
from jag::base::LogBase. (Assumes member or local variables \c _logger and \c _logStream.)

This function is a no-op if JAG3D_ENABLE_LOGGING is not defined. The definition of
JAG3D_ENABLE_LOGGING is controlled using CMake.
\param prio Message priority based on Poco/Message.h.
\param msg Written to the \c _logStream if the \c _logger's priority is <= \c prio.
*/
/** \def __JAG3D_LOG_STATIC
\brief Shared internal logging macro.
\details Internal macro for message logging within static functions or classes
not derived from jag::base::LogBase.

This function is a no-op if JAG3D_ENABLE_LOGGING is not defined. The definition of
JAG3D_ENABLE_LOGGING is controlled using CMake.
\param prio Message priority based on Poco/Message.h.
\param name Poco::Logger name.
\param msg Written to named Logger if the Logger's priority is <= \c prio.
*/
#ifndef JAG3D_ENABLE_LOGGING
#  define __JAG3D_LOG( prio, msg )
#  define __JAG3D_LOG_STATIC( prio, name, msg )
#else
#  define __JAG3D_LOG( prio, msg ) \
    { \
        jag::base::Log::instance()->internalInit(); \
        if( _logger.prio() ) \
            (*_logStream).prio() << ( msg ) << std::endl; \
    }
#  define __JAG3D_LOG_STATIC( prio, name, msg ) \
    { \
        jag::base::Log::instance()->internalInit(); \
        Poco::Logger& logger = Poco::Logger::get( name ); \
        if( logger.prio() ) \
        { \
            Poco::LogStream logstream( logger ); \
            logstream.prio() << ( msg ) << std::endl; \
        } \
    }
#endif


/** \def JAG3D_LOG_FATAL
\brief Boolean for quick abort of log message construction.
\details Constructing anything other than a trivial log message is typically an expensive
operation involving string concatenation, std::ostringstream writes, and other string
manipulation. Use this macro to check the Poco::Logger priority and avoid log message
construction when the log level would prevent display of the message.

If JAG3D_ENABLE_LOGGING is not defined, JAG3D_LOG_FATAL is a runtime constane \c false. An optimizing
compiler will recognize and eliminate log message construction as dead code. For example:
\code
    if( JAG3D_LOG_FATAL )
    {
        // Log message construction
        JAG3D_FATAL( msg );
    }
\endcode
becomes this:
\code
    if( false )
    {
        // Dead code block, optimized away at compile time.
    }
\endcode
*/
/** \def JAG3D_LOG_CRITICAL
\see JAG3D_LOG_FATAL */
/** \def JAG3D_LOG_ERROR
\see JAG3D_LOG_FATAL */
/** \def JAG3D_LOG_WARNING
\see JAG3D_LOG_FATAL */
/** \def JAG3D_LOG_NOTICE
\see JAG3D_LOG_FATAL */
/** \def JAG3D_LOG_INFO
\see JAG3D_LOG_FATAL */
/** \def JAG3D_LOG_DEBUG
\see JAG3D_LOG_FATAL */
/** \def JAG3D_LOG_TRACE
\see JAG3D_LOG_FATAL */
#ifndef JAG3D_ENABLE_LOGGING

#  define JAG3D_LOG_FATAL     false
#  define JAG3D_LOG_CRITICAL  false
#  define JAG3D_LOG_ERROR     false
#  define JAG3D_LOG_WARNING   false
#  define JAG3D_LOG_NOTICE    false
#  define JAG3D_LOG_INFO      false
#  define JAG3D_LOG_DEBUG     false
#  define JAG3D_LOG_TRACE     false

#else

/** \def __JAG3D_LOG_PRIO_CHECK
\brief Internal Boolean macro for checking \c _logger member variable logging priority.
*/
#  define __JAG3D_LOG_PRIO_CHECK( __prio )    ( _logger.__prio() )

#  define JAG3D_LOG_FATAL     __JAG3D_LOG_PRIO_CHECK( fatal )
#  define JAG3D_LOG_CRITICAL  __JAG3D_LOG_PRIO_CHECK( critical )
#  define JAG3D_LOG_ERROR     __JAG3D_LOG_PRIO_CHECK( error )
#  define JAG3D_LOG_WARNING   __JAG3D_LOG_PRIO_CHECK( warning )
#  define JAG3D_LOG_NOTICE    __JAG3D_LOG_PRIO_CHECK( notice )
#  define JAG3D_LOG_INFO      __JAG3D_LOG_PRIO_CHECK( information )
#  define JAG3D_LOG_DEBUG     __JAG3D_LOG_PRIO_CHECK( debug )
#  define JAG3D_LOG_TRACE     __JAG3D_LOG_PRIO_CHECK( trace )

#endif


/** \def JAG3D_FATAL
\brief Efficient message logging macros.
\details Efficient message logging for classes derived from jag::base::LogBase.
(Assumes member or local variables \c _logger and \c _logStream.)
*/
/** \def JAG3D_CRITICAL
\see JAG3D_FATAL */
/** \def JAG3D_ERROR
\see JAG3D_FATAL */
/** \def JAG3D_WARNING
\see JAG3D_FATAL */
/** \def JAG3D_NOTICE
\see JAG3D_FATAL */
/** \def JAG3D_INFO
\see JAG3D_FATAL */
/** \def JAG3D_DEBUG
\see JAG3D_FATAL */
/** \def JAG3D_TRACE
\see JAG3D_FATAL */
#define JAG3D_FATAL( __msg )     __JAG3D_LOG( fatal, __msg )
#define JAG3D_CRITICAL( __msg )  __JAG3D_LOG( critical, __msg )
#define JAG3D_ERROR( __msg )     __JAG3D_LOG( error, __msg )
#define JAG3D_WARNING( __msg )   __JAG3D_LOG( warning, __msg )
#define JAG3D_NOTICE( __msg )    __JAG3D_LOG( notice, __msg )
#define JAG3D_INFO( __msg )      __JAG3D_LOG( information, __msg )
#define JAG3D_DEBUG( __msg )     __JAG3D_LOG( debug, __msg )
#define JAG3D_TRACE( __msg )     __JAG3D_LOG( trace, __msg )

/** \def JAG3D_FATAL_STATIC
\brief Message logging macros.
\details Message logging for static functions or classes not derived from jag::base::LogBase.
These macros obtain a Logger from Poco's global map and construct a Poco::LogStream
on the fly. */
/** \def JAG3D_CRITICAL_STATIC
\see JAG3D_FATAL_STATIC */
/** \def JAG3D_ERROR_STATIC
\see JAG3D_FATAL_STATIC */
/** \def JAG3D_WARNING_STATIC
\see JAG3D_FATAL_STATIC */
/** \def JAG3D_NOTICE_STATIC
\see JAG3D_FATAL_STATIC */
/** \def JAG3D_INFO_STATIC
\see JAG3D_FATAL_STATIC */
/** \def JAG3D_DEBUG_STATIC
\see JAG3D_FATAL_STATIC */
/** \def JAG3D_TRACE_STATIC
\see JAG3D_FATAL_STATIC */

#define JAG3D_FATAL_STATIC( __name, __msg )     __JAG3D_LOG_STATIC( fatal, __name, __msg )
#define JAG3D_CRITICAL_STATIC( __name, __msg )  __JAG3D_LOG_STATIC( critical, __name, __msg )
#define JAG3D_ERROR_STATIC( __name, __msg )     __JAG3D_LOG_STATIC( error, __name, __msg )
#define JAG3D_WARNING_STATIC( __name, __msg )   __JAG3D_LOG_STATIC( warning, __name, __msg )
#define JAG3D_NOTICE_STATIC( __name, __msg )    __JAG3D_LOG_STATIC( notice, __name, __msg )
#define JAG3D_INFO_STATIC( __name, __msg )      __JAG3D_LOG_STATIC( information, __name, __msg )
#define JAG3D_DEBUG_STATIC( __name, __msg )     __JAG3D_LOG_STATIC( debug, __name, __msg )
#define JAG3D_TRACE_STATIC( __name, __msg )     __JAG3D_LOG_STATIC( trace, __name, __msg )

/*@}*/


// __JAG_BASE_LOG_MACROS_H__
#endif
