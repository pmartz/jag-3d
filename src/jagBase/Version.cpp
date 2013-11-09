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

#include <jagBase/Version.h>
#include <jagBase/LogMacros.h>
#include <string>
#include <sstream>
#include <iomanip>

#include <boost/version.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <Poco/Version.h>
#include <Poco/Environment.h>
#include <gmtl/Version.h>


namespace jagBase {


unsigned int getVersionNumber()
{
    return( JAG3D_VERSION );
}


static std::string s_jag3d_version( "" );

std::string getVersionString()
{
    if( s_jag3d_version.empty() )
    {
        std::ostringstream oStr;
        oStr.fill( '0' );

        oStr << to_simple_string( boost::posix_time::second_clock::universal_time() ) << " UTC" << std::endl;

        oStr << std::string( "Jag3D version " ) <<
            std::setw( 1 ) << JAG3D_MAJOR_VERSION << "." <<
            std::setw( 2 ) << JAG3D_MINOR_VERSION << "." <<
            std::setw( 2 ) << JAG3D_SUB_VERSION << " (" <<
            std::setw( 5 ) << getVersionNumber() << ").";
        s_jag3d_version = oStr.str();
        JAG3D_INFO_STATIC( "jag.base.version", s_jag3d_version );

        std::ostringstream oStr2;
        oStr2 << "Dependencies:\n";

        oStr2 << "\tGMTL: " << gmtl::getVersion() << "\n";

        oStr2 << "\tBoost: " << BOOST_VERSION / 100000 << "." <<
            BOOST_VERSION / 100 % 100 << "." << BOOST_VERSION % 100 <<
            " (" << BOOST_VERSION << "), ";
#if defined( BOOST_DATE_TIME_HAS_NANOSECONDS )
        oStr2 << "time config: nanoseconds\n";
#elif defined( BOOST_DATE_TIME_HAS_MICROSECONDS )
        oStr2 << "time config: microseconds\n";
#elif defined( BOOST_DATE_TIME_HAS_MILLISECONDS )
        oStr2 << "time config: milliseconds\n";
#endif

        oStr2 << "\tPoco: " << ((POCO_VERSION >> 24) & 0xff) << "." <<
            ((POCO_VERSION >> 16) & 0xff) << "." << ((POCO_VERSION >> 8) & 0xff) << "." <<
            ( POCO_VERSION & 0xff ) << " (0x" << std::hex << POCO_VERSION << std::dec << ")";

        JAG3D_INFO_STATIC( "jag.base.version", oStr2.str() );

        std::ostringstream oStr3;
        oStr3 << "System:\n";

        oStr3 << "\t" << Poco::Environment::osName() << " " <<
            Poco::Environment::osVersion() << " " <<
            Poco::Environment::osArchitecture();

        JAG3D_INFO_STATIC( "jag.base.version", oStr3.str() );
    }

    return( s_jag3d_version );
}


// namespace jagBase
}
