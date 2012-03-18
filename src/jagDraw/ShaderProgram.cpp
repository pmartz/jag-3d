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

#include <jagDraw/ShaderProgram.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawInfo.h>
#include <jagBase/LogMacros.h>
#include <stdio.h>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>


namespace jagDraw {



void ShaderProgram::printInfoLog( const GLuint id )
{
    if( !JAG3D_LOG_ERROR )
        return;

    Poco::LogStream& ls( _logStream->error() );
    ls << "Error: Program object ID: " << id << "." << std::endl;

    GLsizei bufLen = 0;
    glGetProgramiv( id, GL_INFO_LOG_LENGTH, &bufLen );
    if( bufLen > 1 )
    {
        ls << "----- BEGIN Program info log:" << std::endl;
        GLsizei strLen = 0;        // strlen GL actually wrote to buffer
        char* infoLog = new char[bufLen];
        glGetProgramInfoLog( id, bufLen, &strLen, infoLog );
        if( strLen > 0 )
            ls << infoLog << std::endl;
        ls << "----- END Program info log." << std::endl;
        delete [] infoLog;
    }
}

ShaderProgram::ShaderProgram()
  : jagBase::LogBase( "jag.draw.program" )
{
}

ShaderProgram::~ShaderProgram()
{
    // TBD Handle object deletion
//    if( _ids[ 0 ] != 0 )
//        glDeleteProgram( _ids[ 0 ] );
}

void ShaderProgram::attachShader( ShaderPtr shader )
{
    _shaders.push_back( shader );
}

void ShaderProgram::operator()( DrawInfo& drawInfo )
{
    // Record the currently used program in DrawInfo.
    // Downstream vertex attribs and uniforms will query
    // this for location values.
    drawInfo._program = shared_from_this();

    const unsigned int contextID( ( unsigned int)( drawInfo._id ) );

    bool needLink( true );
    if( _ids._data.size() >= contextID+1 )
    {
        needLink = !( _ids[ contextID ].second );
    }
    bool linkStatus( true );
    if( needLink )
    {
        linkStatus = link( contextID );
    }

    const GLuint id = _ids[ contextID ].first;

    if( ( id == 0 ) || !linkStatus )
    {
        JAG3D_WARNING( "Program ID==0 or link failed." );
        return;
    }

    glUseProgram( id );

    // Iterate over active uniforms. If an active uniform matches a uniform
    // in the drawInfo._uniformMap, set its value in GL state.
    BOOST_FOREACH( const LocationMap::value_type& it, _uniformLocations )
    {
        const HashValue& hash( it.first );
        DrawInfo::UniformMap::iterator uIt( drawInfo._uniformMap.find( hash ) );
        if( uIt != drawInfo._uniformMap.end() )
            (*( uIt->second ))( drawInfo, it.second );
    }
}


void ShaderProgram::setExplicitAttribLocation( GLuint index, const std::string& name )
{
    _explicitVertexAttribLocations[ name ] = index;
}

GLuint ShaderProgram::getExplicitAttribLocation( const std::string& name ) const
{
    ExplicitLocationMap::const_iterator it(
        _explicitVertexAttribLocations.find( name ) );
    return( ( it != _explicitVertexAttribLocations.end() ) ? it->second : -1 );
}

void ShaderProgram::setParameter( GLenum pname, GLint value )
{
    const unsigned int contextID( 0 );

    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );
    const GLuint id( _ids[ contextID ].first );

#if 0 // these are potentially not supported if not defined correctly.

    if( pname == GL_GEOMETRY_SHADER ) pname = GL_GEOMETRY_SHADER_ARB;
    else if( pname == GL_GEOMETRY_VERTICES_OUT ) pname = GL_GEOMETRY_VERTICES_OUT_ARB;
    else if( pname == GL_GEOMETRY_INPUT_TYPE ) pname = GL_GEOMETRY_INPUT_TYPE_ARB;
    else if( pname == GL_GEOMETRY_OUTPUT_TYPE ) pname = GL_GEOMETRY_OUTPUT_TYPE_ARB;

#endif

    glProgramParameteri( id, pname, value );
}

void ShaderProgram::get( GLenum pname, GLint *params )
{
    const unsigned int contextID( 0 );
    const GLuint id( _ids[ contextID ].first );

    glGetProgramiv( id, pname, params );
}

GLint ShaderProgram::getId( const unsigned int contextID )
{
    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );
    return( _ids[ contextID ].first );
}

bool ShaderProgram::link( unsigned int contextID )
{
    JAG3D_TRACE( "ShaderProgram::link" );

    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );

    // idLink.first is OpenGL program ID.
    // idLink.second is link status: true after successful linked.
    IDStatusPair& idLink( _ids[ contextID ] );
    const GLuint id( idLink.first );

    if( JAG3D_LOG_DEBUG )
    {
        _logStream->debug() << "  ShaderProgram: contextID: " << contextID <<
            ", object ID: " << id << std::endl;
    }

    // Loop over explicitly specified vertex attrib locations and
    // set them in this program object.
    JAG3D_TRACE( "  link(): Binding explicit attribute locations." );
    BOOST_FOREACH( const ExplicitLocationMap::value_type& it, _explicitVertexAttribLocations )
    {
        glBindAttribLocation( id, it.second, it.first.c_str() );
    }
    
    // Attach all shaders to this program object.
    bool abortLink( false );
    JAG3D_TRACE( "  link(): Attaching shaders." );
    BOOST_FOREACH( const ShaderList::value_type& shader, _shaders )
    {
        GLint shaderID = shader->getId( contextID );
        abortLink = abortLink || ( shaderID == 0 );
        if( !abortLink ) {
            glAttachShader( id, shaderID );
        }
    }
    if( abortLink )
    {
        // At least one attached shader failed to compile.
        JAG3D_WARNING( "  link(): Aborting because at least one shader failed init." );
        return( false );
    }

    JAG3D_TRACE( "  link(): Calling glLinkProgram()." );
    GLint status;
    glLinkProgram( id );
    glGetProgramiv( id, GL_LINK_STATUS, &status );
    if( status != GL_TRUE )
    {
        printInfoLog( id );
        glDeleteProgram( id );
        idLink.first = 0;
        idLink.second = false;
        return( false );
    }

    idLink.second = true;
    {
        GLint n;
        glUseProgram( id );
        glGetProgramiv( id, GL_ACTIVE_UNIFORMS, &n );
        if( JAG3D_LOG_INFO )
        {
            Poco::LogStream& ls( _logStream->information() );
            ls << "Active Uniforms (" << n << "):" << std::endl;
            if( n > 0 )
                ls << "        hashcode  loc  name" << std::endl;
        }
        for( GLint i = 0; i < n; i++ )
        {
            GLenum type;
            std::string uniformName;
            getActiveUniform( id, i, uniformName, type );
            GLint location = glGetUniformLocation( id, uniformName.c_str() );
            const HashValue hash( createHash( uniformName ) );
            if( JAG3D_LOG_INFO )
            {
                Poco::LogStream& ls( _logStream->information() );
                ls.width( 16 );
                ls << std::hex << std::right << hash << "  ";
                ls.width( 5 );
                ls << std::dec << std::left << location;
                ls.width( 0 );
                ls << uniformName << std::endl;
            }
            _uniformLocations[ hash ] = location;
        }

        glGetProgramiv( id, GL_ACTIVE_ATTRIBUTES, &n );
        if( JAG3D_LOG_INFO )
        {
            Poco::LogStream& ls( _logStream->information() );
            ls << "Active Attributes (" << n << "):" << std::endl;
            if( n > 0 )
                ls << "        hashcode  loc  name" << std::endl;
        }
        for( GLint i = 0; i < n; i++ )
        {
            GLenum type;
            std::string attribName;
            getActiveAttrib( id, i, attribName, type );
            GLint location = glGetAttribLocation( id, attribName.c_str() );
            const HashValue hash( createHash( attribName ) );
            if( JAG3D_LOG_INFO )
            {
                Poco::LogStream& ls( _logStream->information() );
                ls.width( 16 );
                ls << std::hex << std::right << hash << "  ";
                ls.width( 5 );
                ls << std::dec << std::left << location;
                ls.width( 0 );
                ls << attribName << std::endl;
            }
            _vertexAttribLocations[ createHash( attribName ) ] = location;
        }
        glUseProgram( 0 );
    }

    return( true );
}

bool ShaderProgram::validate( unsigned int contextID )
{
    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );
    const GLuint id( _ids[ contextID ].first );

    GLint status;
    glValidateProgram( id );
    glGetProgramiv( id, GL_VALIDATE_STATUS, &status );
    if( status != GL_TRUE )
    {
        printInfoLog( id );
        glDeleteProgram( id );
        _ids[ contextID ].first = 0;
        // TBD also set second (link status) to false?
    }
    return( status == GL_TRUE );
}


unsigned int ShaderProgram::getNumActiveUniforms( const GLuint id ) const
{
    return( _uniformLocations.size() );
}
void ShaderProgram::getActiveUniform( const GLuint id, const GLuint index, std::string& name, GLenum& type )
{
    char namebuff[ 256 ];
    GLsizei len;
    GLsizei isize = sizeof( namebuff );
    GLint osize;
    glGetActiveUniform( id, index, isize, &len, &osize, &type, namebuff );
    name = std::string( namebuff );
}
unsigned int ShaderProgram::getNumActiveAttribs( const GLuint id ) const
{
    return( _vertexAttribLocations.size() );
}
void ShaderProgram::getActiveAttrib( const GLuint id, const GLuint index, std::string& name, GLenum& type )
{
    char namebuff[ 256 ];
    GLsizei len;
    GLsizei isize = sizeof( namebuff );
    GLint osize;
    glGetActiveAttrib( id, index, isize, &len, &osize, &type, namebuff );
    name = std::string( namebuff );
}


void ShaderProgram::internalInit( const unsigned int contextID )
{
    _ids._data.resize( contextID + 1 );
    _ids[ contextID ].first = glCreateProgram();
    _ids[ contextID ].second = false;
}

void ShaderProgram::fromSourceFiles( const std::string &vertexShaderFile,
                              const std::string &fragmentShaderFile )
{
    jagDraw::ShaderPtr vshader(new jagDraw::Shader(GL_VERTEX_SHADER ));
    vshader->addSourceFile( vertexShaderFile );
    attachShader( vshader );

    jagDraw::ShaderPtr fshader(new jagDraw::Shader(GL_FRAGMENT_SHADER ));
    fshader->addSourceFile( fragmentShaderFile );
    attachShader( fshader );
}

void ShaderProgram::fromSourceStrings( const std::string &vertexShaderString,
                               const std::string &fragmentShaderString )
{
    jagDraw::ShaderPtr vshader(new jagDraw::Shader(GL_VERTEX_SHADER ));
    vshader->addSourceString( vertexShaderString );
    attachShader( vshader );

    jagDraw::ShaderPtr fshader(new jagDraw::Shader(GL_FRAGMENT_SHADER ));
    fshader->addSourceString( fragmentShaderString );
    attachShader( fshader );

}

void ShaderProgram::fromSourceFileList( const SourceList &l )
{
    for( SourceList::const_iterator p = l.begin(); p != l.end(); p++ )
    {
        jagDraw::ShaderPtr shader(new jagDraw::Shader( p->first ));
        shader->addSourceFile( p->second );
        attachShader( shader );
    }
}

void ShaderProgram::fromSourceStringList( const SourceList &l )
{
    for( SourceList::const_iterator p = l.begin(); p != l.end(); p++ )
    {
        jagDraw::ShaderPtr shader(new jagDraw::Shader( p->first ));
        shader->addSourceString( p->second );
        attachShader( shader );
    }
}


ShaderProgram::HashValue ShaderProgram::createHash( const std::string& name )
{
    boost::hash< std::string > h;
    return( h( name ) );
}

GLint ShaderProgram::getUniformLocation( const HashValue& h ) const
{
    LocationMap::const_iterator it( _uniformLocations.find( h ) );
    if( it != _uniformLocations.end() )
        return( it->second );
    else
        return( -1 );
}
GLint ShaderProgram::getUniformLocation( const std::string& s ) const
{
    return( getVertexAttribLocation( createHash( s ) ) );
}
GLint ShaderProgram::getVertexAttribLocation( const HashValue& h ) const
{
    LocationMap::const_iterator it( _vertexAttribLocations.find( h ) );
    if( it != _vertexAttribLocations.end() )
        return( it->second );
    else
        return( -1 );
}
GLint ShaderProgram::getVertexAttribLocation( const std::string& s ) const
{
    return( getVertexAttribLocation( createHash( s ) ) );
}


// jagDraw
}
