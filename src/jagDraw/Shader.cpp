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


Shader::Shader( GLenum type ):
    _initialized( false ),
    _type( type )
{
}

Shader::~Shader()
{
    if( _ids[ 0 ] != 0 )
        // TBD need to get context ID, probably as a param?
        glDeleteShader( _ids[ 0 ] );
}

void Shader::addSourceFile( const std::string& fileName )
{
    if( _initialized )
        return ;

    const std::string src = p_loadSource( fileName );
    if( !src.empty() )
        addSourceString( src );
}

void Shader::addSourceString( const std::string& source )
{
    if( _initialized )
        return;

    _sourceList.push_back( source );
}

GLuint Shader::getId()
{
    if( !_initialized )
        p_init();

    // TBD need to get context ID, probably as a param?
    return( _ids[ 0 ] );
}


void Shader::printInfoLog()
{
    GLsizei bufLen = 0;
#if !defined( __APPLE__ ) && !defined(USE_GLES2)
    std::string typeStr = _type == GL_VERTEX_SHADER ? "Vertex Shader" :
                          _type == GL_GEOMETRY_SHADER_EXT ? "Geometry Shader" :
                          _type == GL_FRAGMENT_SHADER ? "Fragment Shader" :"" ;
#else
    std::string typeStr = _type == GL_VERTEX_SHADER ? "Vertex Shader" :
                          _type == GL_FRAGMENT_SHADER ? "Fragment Shader" :"" ;

#endif
    // TBD need to get context ID, probably as a param?
    glGetShaderiv( _ids[ 0 ], GL_INFO_LOG_LENGTH, &bufLen );
    if( bufLen > 1 )
    {
        std::cerr << "\n==========  " << typeStr << " Information Log ============= " << std::endl;
        GLsizei strLen = 0;        // strlen GL actually wrote to buffer
        char* infoLog = new char[bufLen];
        // TBD need to get context ID, probably as a param?
        glGetShaderInfoLog( _ids[ 0 ], bufLen, &strLen, infoLog );
        if( strLen > 0 )
            std::cerr << infoLog << std::endl;
        std::cerr << "==================================================\n" << std::endl;
        delete [] infoLog;
    }
}

void Shader::p_init()
{
    if( _initialized )
        return;
    _initialized = true;

    std::vector< const char* > src;
    std::vector< GLint > length;
    for( std::vector< std::string >::iterator  s = _sourceList.begin(); s != _sourceList.end(); s++ )
    {
        //1. Scan for any built-in variables
        //   a. If OpenGL 3.1 replace deprecated 'gl_' variables with equivalent 'ii_' variables
        length.push_back( s->size() );
        src.push_back( s->c_str() );
    }

    _ids._data.resize( 1 );
    // TBD need to get context ID, probably as a param?
    _ids[ 0 ] = glCreateShader( _type );
    glShaderSource( _ids[ 0 ], src.size(), &src.front(), &length.front() );

    // TBD need to get context ID, probably as a param?
    glCompileShader( _ids[ 0 ] );

    GLint status;
    // TBD need to get context ID, probably as a param?
    glGetShaderiv( _ids[ 0 ], GL_COMPILE_STATUS, &status );
    if( status != GL_TRUE )
    {
        printInfoLog();
        // TBD need to get context ID, probably as a param?
        glDeleteShader( _ids[ 0 ] );
        _ids[ 0 ] = 0;
    }

    _sourceList.clear();
}

std::string Shader::p_loadSource( const std::string& fileName )
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
