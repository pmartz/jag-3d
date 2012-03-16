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

#define JAG3D_FATAL( __msg )     __JAG3D_LOG( fatal, __msg )
#define JAG3D_CRITICAL( __msg )  __JAG3D_LOG( critical, __msg )
#define JAG3D_ERROR( __msg )     __JAG3D_LOG( error, __msg )
#define JAG3D_WARNING( __msg )   __JAG3D_LOG( warning, __msg )
#define JAG3D_NOTICE( __msg )    __JAG3D_LOG( notice, __msg )
#define JAG3D_INFO( __msg )      __JAG3D_LOG( information, __msg )
#define JAG3D_DEBUG( __msg )     __JAG3D_LOG( debug, __msg )
#define JAG3D_TRACE( __msg )     __JAG3D_LOG( trace, __msg )

#define JAG3D_FATAL_STATIC( __name, __msg )     __JAG3D_LOG_STATIC( fatal, __name, __msg )
#define JAG3D_CRITICAL_STATIC( __name, __msg )  __JAG3D_LOG_STATIC( critical, __name, __msg )
#define JAG3D_ERROR_STATIC( __name, __msg )     __JAG3D_LOG_STATIC( error, __name, __msg )
#define JAG3D_WARNING_STATIC( __name, __msg )   __JAG3D_LOG_STATIC( warning, __name, __msg )
#define JAG3D_NOTICE_STATIC( __name, __msg )    __JAG3D_LOG_STATIC( notice, __name, __msg )
#define JAG3D_INFO_STATIC( __name, __msg )      __JAG3D_LOG_STATIC( information, __name, __msg )
#define JAG3D_DEBUG_STATIC( __name, __msg )     __JAG3D_LOG_STATIC( debug, __name, __msg )
#define JAG3D_TRACE_STATIC( __name, __msg )     __JAG3D_LOG_STATIC( trace, __name, __msg )


// __JAGBASE_LOG_MACROS_H__
#endif
