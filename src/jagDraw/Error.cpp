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

#include <jagDraw/Error.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/Log.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <string>



namespace jagDraw
{


void errorCheck( const std::string& msg )
{
    const GLenum errorEnum = glGetError();
    if( errorEnum == GL_NO_ERROR )
        return;

    Poco::Logger* logger = Poco::Logger::has( "jag3d.jagDraw.GLError" );
    if( logger == NULL )
    {
        logger = &( Poco::Logger::create( "jag3d.jagDraw.GLError",
                (Poco::Channel*)( jagBase::Log::instance()->getConsole() ), Poco::Message::PRIO_ERROR ) );
    }

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
    logger->error( enumStr );
}


void fboErrorCheck( const std::string& msg )
{
    const GLenum errorEnum( glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER ) );
    if( errorEnum == GL_FRAMEBUFFER_COMPLETE )
        return;

    Poco::Logger* logger = Poco::Logger::has( "jag3d.jagDraw.GLError" );
    if( logger == NULL )
    {
        logger = &( Poco::Logger::create( "jag3d.jagDraw.GLError",
                (Poco::Channel*)( jagBase::Log::instance()->getConsole() ), Poco::Message::PRIO_ERROR ) );
    }

    std::string enumStr( "Unknown" );
    switch( errorEnum ) {
    case GL_FRAMEBUFFER_UNSUPPORTED: enumStr = std::string( "GL_FRAMEBUFFER_UNSUPPORTED" ); break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: enumStr = std::string( "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" ); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: enumStr = std::string( "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" ); break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: enumStr = std::string( "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" ); break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: enumStr = std::string( "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" ); break;
    }

#if( defined( JAG3D_USE_GLEW ) && defined( GLEW_EXT_framebuffer_object ) )
    // Check for an outdated error code. This might be useful for early JAG
    // development on GL2 systems.
    if( errorEnum == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT )
        enumStr = std::string( "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT" );
#endif

    std::string outMsg( "OpenGL FBO error " );
    outMsg.append( enumStr );
    outMsg.append( msg );
    logger->error( outMsg );
}


// jagDraw
}
