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

#include <jagDraw/Shader.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagBase/LogMacros.h>
#include <boost/foreach.hpp>
#include <jagDraw/Error.h>
#include <iostream>
#include <fstream>


namespace jagDraw {


Shader::Shader( GLenum type )
  : jagBase::LogBase( "jag.draw.shader" ),
    _type( type )
{
}
Shader::Shader( const Shader& rhs )
  : jagBase::LogBase( rhs ),
    _type( rhs._type ),
    _sourceVec( rhs._sourceVec )
{
}
Shader::~Shader()
{
    // TBD Handle object deletion
//    if( _ids[ 0 ] != 0 )
//        glDeleteShader( _ids[ 0 ] );
}

void Shader::addSourceString( const std::string& source )
{
    _sourceVec.push_back( source );
}

GLuint Shader::getID( const unsigned int contextID )
{
    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}


void Shader::printInfoLog( const GLuint id )
{
    if( !JAG3D_LOG_ERROR )
        return;

    Poco::LogStream& ls( _logStream->error() );
    ls << "Error: Shader object ID: " << id << "." << std::endl;

    GLsizei bufLen = 0;
    const std::string typeStr = getTypeAsString( _type );

    glGetShaderiv( id, GL_INFO_LOG_LENGTH, &bufLen );
    if( bufLen > 1 )
    {
        ls << "----- BEGIN " << typeStr << " info log:" << std::endl;
        GLsizei strLen = 0;        // strlen GL actually wrote to buffer
        char* infoLog = new char[bufLen];
        glGetShaderInfoLog( id, bufLen, &strLen, infoLog );
        if( strLen > 0 )
            ls << infoLog << std::endl;
        ls << "----- END " << typeStr << " info log." << std::endl;
        delete [] infoLog;
    }

    ls << "----- BEGIN " << typeStr << " source dump:" << std::endl;
    const std::string source( getFullSource() );
    std::string::size_type start( 0 ), end( source.find( '\n' ) );
    unsigned int lineNum( 1 );
    while( start != std::string::npos )
    {
        ls.width( 5 );
        ls << std::right << lineNum++;
        ls.width( 0 );
        ls << std::left << " " << source.substr( start, end-start ) << std::endl;

        start = ( end == std::string::npos ) ? end : end+1;
        end = source.find( '\n', start );
    }
    //ls << getFullSource() << std::endl;
    ls << "----- END " << typeStr << " source dump." << std::endl;
}

void Shader::internalInit( const unsigned int contextID )
{
    std::vector< const char* > src;
    GLintVec length;
    BOOST_FOREACH( jagBase::StringVec::value_type& srcStr, _sourceVec )
    {
        //1. Scan for any built-in variables
        //   a. If OpenGL 3.1 replace deprecated 'gl_' variables with equivalent 'ii_' variables
        length.push_back( (GLint)( srcStr.size() ) );
        src.push_back( srcStr.c_str() );
    }

    const GLuint id( glCreateShader( _type ) );
    JAG3D_ERROR_CHECK( "Shader::internalInit()" );
    if( id == 0 )
        JAG3D_ERROR( "glCreateShader() returned shader ID 0." );
    _ids[ contextID ] = id;

    if( JAG3D_LOG_TRACE )
    {
        _logStream->trace() << std::string( "Compiling source for shader ID: " )
            << id << std::string( ", contextID: " ) << contextID << "." << std::endl;
    }

    glShaderSource( id, (GLsizei)( src.size() ), &src.front(), &length.front() );

    glCompileShader( id );

    GLint status;
    glGetShaderiv( id, GL_COMPILE_STATUS, &status );
    if( status != GL_TRUE )
    {
        printInfoLog( id );
        glDeleteShader( _ids[ contextID ] );
        _ids[ contextID ] = 0;
    }
}

std::string Shader::getFullSource() const
{
    std::string fullSource;
    BOOST_FOREACH( const jagBase::StringVec::value_type& srcStr, _sourceVec )
    {
        fullSource += srcStr;
    }
    return( fullSource );
}

std::string Shader::getTypeAsString( GLenum type )
{
#if !defined( __APPLE__ ) && !defined(USE_GLES2)
    std::string typeStr = type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" :
                          type == GL_GEOMETRY_SHADER ? "GL_GEOMETRY_SHADER" :
                          type == GL_FRAGMENT_SHADER ? "GL_FRAGMENT_SHADER" : "";
#else
    std::string typeStr = type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" :
                          type == GL_FRAGMENT_SHADER ? "GL_FRAGMENT_SHADER" : "";
#endif
    return( typeStr );
}


// jagDraw
}
