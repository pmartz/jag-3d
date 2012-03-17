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


#include <Poco/Logger.h>
#include <Poco/LogStream.h>


/** \addtogroup jagBaseLogging Message Logging Utilities
\details Jag's message logging system is based on
<a href="http://pocoproject.org/slides/110-Logging.pdf" target=null>Poco</a>
and is modeled after
<a href="http://www.vesuite.org/" target=null>VE-Suite's</a> message logging system.

Jag's modules and classes are arranged in a heirarchy to allow message destination (console
or log file) and verbosity on a per-module and per-class basis. The heirarchy is arranged
as follows:
<ul>
  <li>jag3d
  <ul>
    <li>jagBase
    <ul>
      <li>Log
      <li>version
    </ul>
    <li>jagDraw
    <ul>
      <li>ctx
      <li>Drawable
      <li>GLError
    </ul>
    <li>jagSG
    <li>jagUtil
    <li>demo
    <ul>
      <li>various demos and tests
    </ul>
  </ul>
</ul>
*/
/*@{*/


/** \def __JAG3D_LOG
\brief Shared internal logging macro.
\details Internal macro for efficient message logging for classes derived
from jagBase::LogBase. (Assumes member or local variables \c _logger and \c _logStream.)

This function is a no-op if JAG3D_DISABLE_LOGGING is defined. The definition of
JAG3D_DISABLE_LOGGING is controlled using CMake.
\param prio Message priority based on Poco/Message.h.
\param msg Written to the \c _logStream if the \c _logger's priority is <= \c prio.
*/
/** \def __JAG3D_LOG_STATIC
\brief Shared internal logging macro.
\details Internal macro for message logging within static functions or classes
not derived from jagBase::LogBase.

This function is a no-op if JAG3D_DISABLE_LOGGING is defined. The definition of
JAG3D_DISABLE_LOGGING is controlled using CMake.
\param prio Message priority based on Poco/Message.h.
\param name Poco::Logger name.
\param msg Written to named Logger if the Logger's priority is <= \c prio.
*/
#ifdef JAG3D_DISABLE_LOGGING
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

If JAG3D_DISABLE_LOGGING is defined, JAG3D_LOG_FATAL is a runtime constane \c false. An optimizing
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
#ifdef JAG3D_DISABLE_LOGGING

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
