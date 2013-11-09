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
