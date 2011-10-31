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
