// Copyright

#include <jagBase/Version.h>
#include <string>
#include <sstream>

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
        oStr << std::string( "Jag3D version " ) <<
            JAG3D_MAJOR_VERSION << "." <<
            JAG3D_MINOR_VERSION << "." <<
            JAG3D_SUB_VERSION << " (" <<
            getVersionNumber() << ").";
        s_jag3d_version = oStr.str();
    }
    return( s_jag3d_version );
}


// namespace jagBase
}
