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
