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
#include <stdio.h>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>


namespace jagDraw {



void ShaderProgram::printInfoLog()
{
    const unsigned int contextID( 0 );
    const GLuint id( _ids[ contextID ].first );

    GLsizei bufLen = 0;       
    glGetProgramiv( id, GL_INFO_LOG_LENGTH, &bufLen );
    if( bufLen > 1 )
    {
        std::cerr << "\n==========  Shader Information Log ============= " << std::endl;
        GLsizei strLen = 0;        // strlen GL actually wrote to buffer
        char* infoLog = new char[bufLen];
        glGetProgramInfoLog( id, bufLen, &strLen, infoLog );
        if( strLen > 0 )
            std::cerr << infoLog << std::endl;
        std::cerr << "==================================================\n" << std::endl;
        delete [] infoLog;
    }
}

ShaderProgram::ShaderProgram()
    //: DrawableAttribute( ShaderProgram_t )
{
    m_stringToNameMap["ii_ModelViewProjectionMatrix"] = ModelViewProjectionMatrix;
    m_stringToNameMap["ii_NormalMatrix"] = NormalMatrix;
    m_nameToLocationMap[ModelViewProjectionMatrix] = -1;
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

void ShaderProgram::use( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );

    bool needLink( true );
    if( _ids._data.size() >= contextID+1 )
    {
        needLink = !( _ids[ contextID ].second );
    }
    if( needLink )
    {
        if( !( link( contextID ) ) )
            return;
    }

    const GLuint id = _ids[ contextID ].first;
    if( id == 0 )
        return;

    glUseProgram( id );

    // Record the currently used program in DrawInfo.
    // Downstream vertex attribs and uniforms will query
    // this for location values.
    drawInfo._program = shared_from_this();
}

GLuint ShaderProgram::getUniformLocation( const std::string &name )
{
    std::map<std::string, LocationTypePair>::iterator f =  m_nameToLocationTypeMap.find( name );
    if( f != m_nameToLocationTypeMap.end() )
        return f->second.loc;
    return -1;
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

GLint ShaderProgram::getId( unsigned int contextID )
{
    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );
    return( _ids[ contextID ].first );
}

bool ShaderProgram::link( unsigned int contextID )
{
    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );

    // idLink.first is OpenGL program ID.
    // idLink.second is link status: true after successful linked.
    IDLinkPair& idLink( _ids[ contextID ] );
    const GLuint id( idLink.first );

    // Loop over explicitly specified vertex attrib locations and
    // set them in this program object.
    BOOST_FOREACH( const ExplicitLocationMap::value_type& it, _explicitVertexAttribLocations )
    {
        glBindAttribLocation( id, it.second, it.first.c_str() );
    }
    
    // Attach all shaders to this program object.
    BOOST_FOREACH( const ShaderList::value_type& s, _shaders )
    {
        GLint shader = s->getId();
        if( shader != 0 )
            glAttachShader( id, shader );
    }
    // We no longer need the list of attached shaders. Clear the list and remove
    // references to those shaders (which will likely cause them to be deleted,
    // thereby reclaiming memory, unless the app still holds a reference).
    _shaders.clear();

    GLint status;
    glLinkProgram( id );
    glGetProgramiv( id, GL_LINK_STATUS, &status );
    if( status != GL_TRUE )
    {
        printInfoLog();
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
printf("Active Uniforms: (%d)\n", n );
        for( GLint i = 0; i < n; i++ )
        {
            GLenum type;
            std::string string;
            getActiveUniform( i, string, type );
            GLint location = glGetUniformLocation( id, string.c_str() );
printf("......... %s (loc: %d)\n", string.c_str(), location );
            m_nameToLocationTypeMap[string] = LocationTypePair( location, type );
            _uniformLocations[ createHash( string ) ] = location;

            std::map< std::string,  UniformLocationName >::iterator p = m_stringToNameMap.find( string ); 
            if( p != m_stringToNameMap.end() )
            {
                UniformLocationName name = p->second;
                m_nameToLocationMap[name] = location;
            }
        }

        glGetProgramiv( id, GL_ACTIVE_ATTRIBUTES, &n );
printf("Active Attributes: (%d)\n", n );
        for( GLint i = 0; i < n; i++ )
        {
            GLenum type;
            std::string name;

            char namebuff[256];
            GLsizei len;
            GLsizei isize = sizeof(namebuff);
            GLint osize;
            //getActiveAttrib( i, name, type );
            glGetActiveAttrib( id, i, isize, &len, &osize, &type, namebuff );
            name = std::string(namebuff );
            GLint loc = glGetAttribLocation( id, name.c_str() );
printf("......... %s (loc: %d)\n", name.c_str(), loc );
            GLint location = glGetUniformLocation( id, name.c_str() );
            m_nameToLocationTypeMap[name] = LocationTypePair( location, type );
            _vertexAttribLocations[ createHash( name ) ] = loc;
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
        printInfoLog();
        glDeleteProgram( id );
        _ids[ contextID ].first = 0;
        // TBD also set second (link status) to false?
    }
    return( status == GL_TRUE );
}


void ShaderProgram::getActiveUniform( GLuint index, std::string &name, GLenum &type )
{
    const unsigned int contextID( 0 );
    const GLuint id( _ids[ contextID ].first );

    char namebuff[256];
    GLsizei len;
    GLsizei isize = sizeof(namebuff);
    GLint osize;
    glGetActiveUniform( id, index, isize, &len, &osize, &type, namebuff );
    name = std::string(namebuff );
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
