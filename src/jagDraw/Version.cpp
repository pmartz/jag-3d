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

#include <jagDraw/Version.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/LogMacros.h>
#include <string>
#include <sstream>


namespace jagDraw {


int getOpenGLVersionNumber()
{
    const std::string versionString( (char*) glGetString( GL_VERSION ) );
    if( versionString.empty() )
    {
        JAG3D_WARNING_STATIC( "jag.draw.version", "glGetString( GL_VERSION ) returned empty string. Check current context." );
        return( -1 );
    }

    std::istringstream istr( versionString );
    unsigned int major, minor, patch;
    char dot;
    istr >> major >> dot >> minor >> dot >> patch;
    const unsigned int versionNumber( major * 10000 + minor * 100 + patch );

    return( versionNumber );
}


std::string getOpenGLVersionString()
{
    const std::string version( (char*) glGetString( GL_VERSION ) );
    if( version.empty() )
    {
        JAG3D_WARNING_STATIC( "jag.draw.version", "glGetString( GL_VERSION ) returned empty string. Check current context." );
        return( "" );
    }

    const std::string vendor( (char*) glGetString( GL_VENDOR ) );
    const std::string renderer( (char*) glGetString( GL_RENDERER ) );
    const std::string glslVersion( (char*) glGetString( GL_SHADING_LANGUAGE_VERSION ) );

    std::ostringstream ostr;
    ostr << getOpenGLVersionNumber();

    JAG3D_INFO_STATIC( "jag.draw.version",
        "GL_VERSION: " + version + " (" + ostr.str() + ")\n" +
        "GL_VENDOR: " + vendor + "\n" +
        "GL_RENDERER: " + renderer + "\n" +
        "GL_SHADING_LANGUAGE_VERSION: " + glslVersion );

    return( version );
}


// jagDraw
}
