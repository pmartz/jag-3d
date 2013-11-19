/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAGBASE_LOG_BASE_H__
#define __JAGBASE_LOG_BASE_H__ 1


#include <jagBase/Config.h>
#include <jagBase/ptr.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/NullChannel.h>


namespace jagBase
{


typedef jagBase::ptr< Poco::LogStream >::shared_ptr PocoLogStreamPtr;


/** \addtogroup jagBaseLogging Message Logging Utilities */
/*@{*/


/** \class LogBase LogBase.h <jagBase/LogBase.h>
\brief Enables message logging in Jag3D classes.
\details Keeps a reference to a Poco::Logger and a smart pointer to a Poco LogStream.
Derived classes pass the Logger name as a parameter to the LogBase constructor. All
classes using the same Logger name share a reference to that Logger.

Classes that derive from LogBase can use the macros in jagBase/LogMacros.h. See
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


// jagBase
}


// __JAGBASE_LOG_BASE_H__
#endif
