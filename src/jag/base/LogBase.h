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

#ifndef __JAG_BASE_LOG_BASE_H__
#define __JAG_BASE_LOG_BASE_H__ 1


#include <jag/base/Config.h>
#include <jag/base/ptr.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/NullChannel.h>


namespace jag {
namespace base {


typedef jag::base::ptr< Poco::LogStream >::shared_ptr PocoLogStreamPtr;


/** \addtogroup jagBaseLogging Message Logging Utilities */
/*@{*/


/** \class LogBase LogBase.h <jag/base/LogBase.h>
\brief Enables message logging in Jag3D classes.
\details Keeps a reference to a Poco::Logger and a smart pointer to a Poco LogStream.
Derived classes pass the Logger name as a parameter to the LogBase constructor. All
classes using the same Logger name share a reference to that Logger.

Classes that derive from LogBase can use the macros in jag/base/LogMacros.h. See
\ref jagBaseLogging "Message Logging Utilities" for more information.
*/
class LogBase
{
public:
    LogBase( const std::string& loggerName )
      : _logger( Poco::Logger::get( loggerName ) ),
        _logStream( PocoLogStreamPtr( new Poco::LogStream( _logger ) ) )
    {
#ifndef JAG3D_ENABLE_LOGGING
        _logger.setChannel( new Poco::NullChannel() );
#endif
    }
    LogBase( const LogBase& rhs )
      : _logger( rhs._logger ),
        _logStream( rhs._logStream )
    {}

    ~LogBase() {}

    Poco::Logger& _logger;
    PocoLogStreamPtr _logStream;
};

/*@}*/


// namespace jag::base::
}
}


// __JAG_BASE_LOG_BASE_H__
#endif
