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

#include <jagDraw/Error.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/Log.h>
#include <jagBase/LogMacros.h>
#include <string>



namespace jagDraw
{


GLenum errorCheck( const std::string& msg )
{
    const GLenum errorEnum = glGetError();

#ifdef JAG3D_ENABLE_LOGGING

    if( errorEnum == GL_NO_ERROR )
        return( errorEnum );

    std::string enumStr( "Unknown" );
    switch( errorEnum ) {
    case GL_INVALID_ENUM: enumStr = std::string( "GL_INVALID_ENUM" ); break;
    case GL_INVALID_VALUE: enumStr = std::string( "GL_INVALID_VALUE" ); break;
    case GL_INVALID_OPERATION: enumStr = std::string( "GL_INVALID_OPERATION" ); break;
    case GL_OUT_OF_MEMORY: enumStr = std::string( "GL_OUT_OF_MEMORY" ); break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: enumStr = std::string( "GL_INVALID_FRAMEBUFFER_OPERATION" ); break;
    }

    enumStr.append( ": " );
    enumStr.append( msg );
    JAG3D_ERROR_STATIC( "jag.draw.glerror", enumStr );

#endif

    return( errorEnum );
}


GLenum fboErrorCheck( const std::string& msg )
{
    const GLenum errorEnum( glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER ) );

#ifdef JAG3D_ENABLE_LOGGING

    if( errorEnum == GL_FRAMEBUFFER_COMPLETE )
        return( errorEnum );

    std::string enumStr( "Unknown" );
    switch( errorEnum ) {
    case GL_FRAMEBUFFER_UNSUPPORTED: enumStr = std::string( "GL_FRAMEBUFFER_UNSUPPORTED" ); break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: enumStr = std::string( "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" ); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: enumStr = std::string( "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" ); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: enumStr = std::string( "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" ); break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: enumStr = std::string( "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" ); break;
    }

    enumStr.append( ": " );
    enumStr.append( msg );
    JAG3D_ERROR_STATIC( "jag.draw.glerror.fbo", enumStr );

#endif

    return( errorEnum );
}


// jagDraw
}
