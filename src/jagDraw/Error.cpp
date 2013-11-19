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
