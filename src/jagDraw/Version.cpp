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

#include <jagDraw/Version.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jag/base/LogMacros.h>
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


std::string getOpenGLVersionString( const bool silent )
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

    if( !silent )
    {
        JAG3D_INFO_STATIC( "jag.draw.version",
            "GL_VERSION: " + version + " (" + ostr.str() + ")\n" +
            "GL_VENDOR: " + vendor + "\n" +
            "GL_RENDERER: " + renderer + "\n" +
            "GL_SHADING_LANGUAGE_VERSION: " + glslVersion );
    }

    return( version );
}


// jagDraw
}
