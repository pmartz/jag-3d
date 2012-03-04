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
#include <iostream>
#include <fstream>


namespace jagDraw {


Shader::Shader( GLenum type )
  : _type( type )
{
}

Shader::~Shader()
{
    // TBD Handle object deletion
//    if( _ids[ 0 ] != 0 )
//        glDeleteShader( _ids[ 0 ] );
}

void Shader::addSourceFile( const std::string& fileName )
{
    const std::string src = loadSource( fileName );
    if( !src.empty() )
        addSourceString( src );
}

void Shader::addSourceString( const std::string& source )
{
    _sourceList.push_back( source );
}

GLuint Shader::getId()
{
    const unsigned int contextID( 0 );

    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );

    return( _ids[ 0 ] );
}


void Shader::printInfoLog()
{
    const unsigned int contextID( 0 );

    GLsizei bufLen = 0;
#if !defined( __APPLE__ ) && !defined(USE_GLES2)
    std::string typeStr = _type == GL_VERTEX_SHADER ? "Vertex Shader" :
                          _type == GL_GEOMETRY_SHADER ? "Geometry Shader" :
                          _type == GL_FRAGMENT_SHADER ? "Fragment Shader" :"" ;
#else
    std::string typeStr = _type == GL_VERTEX_SHADER ? "Vertex Shader" :
                          _type == GL_FRAGMENT_SHADER ? "Fragment Shader" :"" ;

#endif

    glGetShaderiv( _ids[ contextID ], GL_INFO_LOG_LENGTH, &bufLen );
    if( bufLen > 1 )
    {
        std::cerr << "\n==========  " << typeStr << " Information Log ============= " << std::endl;
        GLsizei strLen = 0;        // strlen GL actually wrote to buffer
        char* infoLog = new char[bufLen];

        glGetShaderInfoLog( _ids[ contextID ], bufLen, &strLen, infoLog );
        if( strLen > 0 )
            std::cerr << infoLog << std::endl;
        std::cerr << "==================================================\n" << std::endl;
        delete [] infoLog;
    }
}

void Shader::internalInit( const unsigned int contextID )
{
    _ids._data.resize( contextID + 1 );

    std::vector< const char* > src;
    std::vector< GLint > length;
    for( std::vector< std::string >::iterator  s = _sourceList.begin(); s != _sourceList.end(); s++ )
    {
        //1. Scan for any built-in variables
        //   a. If OpenGL 3.1 replace deprecated 'gl_' variables with equivalent 'ii_' variables
        length.push_back( (GLint)( s->size() ) );
        src.push_back( s->c_str() );
    }

    _ids[ contextID ] = glCreateShader( _type );
    glShaderSource( _ids[ contextID ], (GLsizei)( src.size() ), &src.front(), &length.front() );

    glCompileShader( _ids[ contextID ] );

    GLint status;
    glGetShaderiv( _ids[ contextID ], GL_COMPILE_STATUS, &status );
    if( status != GL_TRUE )
    {
        printInfoLog();
        glDeleteShader( _ids[ contextID ] );
        _ids[ contextID ] = 0;
    }

    _sourceList.clear();
}

std::string Shader::loadSource( const std::string& fileName )
{
    std::ifstream in( fileName.c_str() );
    if( !in )
    {
        std::cerr << "Shader::p_loadSource() - unable to load file \"" << fileName << "\"." << std::endl;
        return( std::string() );
    }

    in.seekg( 0, std::ios::end );
    unsigned long size = (unsigned long)( in.tellg() );
    in.seekg( 0, std::ios::beg );
    char *buff = new char[size + 1];
    in.read( buff, size );
    in.close();
    buff[ size ] = 0;
    std::string str( buff );
    delete[] buff;

    return str;
}


// jagDraw
}
