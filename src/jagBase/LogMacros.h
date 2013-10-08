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

#ifndef __JAGBASE_LOG_MACROS_H__
#define __JAGBASE_LOG_MACROS_H__ 1


#include <jagBase/Config.h>
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
    <li>base (the jagBase module)
    <ul>
      <li>\link jagBase::Log log \endlink
      <li>\ref jagBaseVersion "version"
    </ul>
    <li>draw (the jagDraw module)
    <ul>
      <li>\link jagDraw::BufferObject bufobj \endlink
      <li>\link jagDraw::ContextSupport ctx \endlink
      <li>\link jagDraw::Drawable drawable \endlink
      <li>\ref jagDrawError "glerror"
      <li>\link jagDraw::Program program \endlink
      <li>\link jagDraw::Shader shader \endlink
    </ul>
    <li>sg (the jagSG module)
    <li>util (the jagUtil module)
    <li>demo (demos, tests, examples)
    <ul>
      <li>various demos and tests
    </ul>
  </ul>
</ul>

Heirachy levels are separated by a period, so the name for the jagDraw::Drawable Poco::Logger is
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

Set a Poco::Logger's priority and destination (console or log file) using the jagBase::Log class.
\code
    // Set the global priority to 3 (error and above) and destination to Console.
    // There is an implicit 3rd parameter of "jag".
    jagBase::Log::instance()->setPriority( 3, jagBase::Log::Console );

    // Set the jagDraw module to priority 3 (error and above)
    jagBase::Log::instance()->setPriority( 4, "jag.draw" );

    // Send any OpenGL errors to the log file.
    jagBase::Log::instance()->setPriority( 3, jagBase::Log::LogFile, "jag.draw.glerror" );

    // Turn on debugging for jagDraw context handling.
    jagBase::Log::instance()->setPriority( 8, "jag.draw.ctx" );
\endcode

Poco Logger priority and destination inherit down the heirarchy. Your application should
set specific priority and destinations (as in the code above) at init time, before
Jag3D class constructors obtain references to subordinate Loggers.

By default, the root Logger ("jag") is configured with priority 0 (silent) and
destination jagBase::Log::Console.

The default log file name is "jag3d.log" and Jag3D writes it to the current directory.
You can change the default log file name with either of the following methods:
\li Call jagBase::Log::setLogFileName( const std::string& ).
\li Set the environment variable JAG3D_LOG_FILE_NAME to the path and file name prior
to invoking jagBase::Log::instance().

Jag3D internal code uses the macros defined in jagBase/logMatrix.h to display log messages at
verious levels. When using these macros, please observe the following rules of thumb:
\li Use the JAG3D_LOG_<prio> macros to avoid expensive message construction for messages that will not be logged due to their priority.
\li Use the JAG3D_<prio> macros from member functions of classes that derive from jagBase::LogBase.
\li Use the JAG3D_<prio>_STATIC macros from static functions, or from member functions of classes that don't derive from jagBase::LogBase.

Note that, even with priority set to 0, there is some non-zero overhead in handling
message logging. To eliminate all Jag3D message logging, set the CMake variable
JAG3D_ENABLE_LOGGING to OFF (or deselect its checkbox in cmake-gui). When this variable
is off, Jag3D's message logging facilities become no-ops and have zero computational cost.
Jag3d-dependent downstream projects can determine whether logging is enabled using
the Config.h header:
\code
#include <jagBase/Config.h>
\endcode
This file defines, or undefines, the JAG3D_ENABLE_LOGGING CPP variable as appropriate
based on how CMake was configured when Jag3d was built.

*/
/*@{*/


/** \def __JAG3D_LOG
\brief Shared internal logging macro.
\details Internal macro for efficient message logging for classes derived
from jagBase::LogBase. (Assumes member or local variables \c _logger and \c _logStream.)

This function is a no-op if JAG3D_ENABLE_LOGGING is not defined. The definition of
JAG3D_ENABLE_LOGGING is controlled using CMake.
\param prio Message priority based on Poco/Message.h.
\param msg Written to the \c _logStream if the \c _logger's priority is <= \c prio.
*/
/** \def __JAG3D_LOG_STATIC
\brief Shared internal logging macro.
\details Internal macro for message logging within static functions or classes
not derived from jagBase::LogBase.

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
    if( _logger.prio() ) \
        (*_logStream).prio() << ( msg ) << std::endl;
#  define __JAG3D_LOG_STATIC( prio, name, msg ) \
    { \
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
\details Efficient message logging for classes derived from jagBase::LogBase.
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
\details Message logging for static functions or classes not derived from jagBase::LogBase.
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


// __JAGBASE_LOG_MACROS_H__
#endif
