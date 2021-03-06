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

#include <jag/draw/Shader.h>
#include <jag/draw/PlatformOpenGL.h>
#include <jag/base/LogMacros.h>
#include <boost/foreach.hpp>
#include <jag/draw/Error.h>
#include <iostream>
#include <fstream>


namespace jag {
namespace draw {


Shader::Shader( GLenum type, const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.draw.shader" : logName ),
    _type( type )
{
}
Shader::Shader( const Shader& rhs )
  : ObjectID( rhs ),
    jag::base::LogBase( rhs ),
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
void Shader::insertSourceString( const std::string& source, const unsigned int index )
{
    _sourceVec.resize( _sourceVec.size() + 1 );
    for( size_t idx = _sourceVec.size()-1; idx > index; --idx )
        _sourceVec[ idx ] = _sourceVec[ idx - 1 ];
    _sourceVec[ index ] = source;
}
unsigned int Shader::getNumSourceStrings() const
{
    return( ( unsigned int )( _sourceVec.size() ) );
}
jag::base::StringVec& Shader::getSourceVec()
{
    return( _sourceVec );
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
    BOOST_FOREACH( jag::base::StringVec::value_type& srcStr, _sourceVec )
    {
        //1. Scan for any built-in variables
        //   a. If OpenGL 3.1 replace deprecated 'gl_' variables with equivalent 'ii_' variables
        length.push_back( (GLint)( srcStr.size() ) );
        src.push_back( srcStr.c_str() );
    }

    const GLuint id( glCreateShader( _type ) );
    JAG3D_ERROR_CHECK( "Shader::internalInit()" );
    if( id == 0 )
    {
        JAG3D_ERROR( "glCreateShader() returned shader ID 0." );
    }
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
    BOOST_FOREACH( const jag::base::StringVec::value_type& srcStr, _sourceVec )
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


// namespace jag::draw::
}
}
