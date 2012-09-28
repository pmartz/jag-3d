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

#include <jagDraw/Program.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawInfo.h>
#include <jagBase/LogMacros.h>
#include <jagDraw/Error.h>
#include <stdio.h>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>


namespace jagDraw {


void Program::printInfoLog( const GLuint id )
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

Program::Program()
  : DrawablePrep(),
    ObjectID(),
    jagBase::LogBase( "jag.draw.program" )
{
}
Program::Program( const Program& rhs )
  : DrawablePrep( rhs ),
    ObjectID( rhs ),
    jagBase::LogBase( rhs )
{
}

Program::~Program()
{
    // TBD Handle object deletion
//    if( _ids[ 0 ] != 0 )
//        glDeleteProgram( _ids[ 0 ] );
}

void Program::attachShader( ShaderPtr shader )
{
    _shaders.push_back( shader );
}

void Program::operator()( DrawInfo& drawInfo )
{
    // Record the currently used program in DrawInfo.
    // Downstream vertex attribs and uniforms will query
    // this for location values.
    drawInfo._program = shared_from_this();

    const unsigned int contextID( ( unsigned int )( drawInfo._id ) );
    const GLuint id( getID( contextID ) );
    const GLboolean& linked( _linkStatus[ contextID ] );

    if( linked == GL_FALSE )
    {
        if( id == 0 )
        {
            // This is out of the ordinary. getID() should have called glCreateProgram().
            JAG3D_ERROR( "Program ID==0." );
            return;
        }
        if( !( link( contextID ) ) )
        {
            JAG3D_ERROR( "Program link failed." );
            return;
        }
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

    // Iterate over active uniform blocks. If an active uniform block matches a uniform
    // in the drawInfo._uniformBlockMap, bind the uniform buffer object.
    BOOST_FOREACH( const BlockInfoMap::value_type& it, _blockInfo )
    {
        const HashValue& hash( it.first );
        DrawInfo::UniformBlockMap::iterator ubIt( drawInfo._uniformBlockMap.find( hash ) );
        if( ubIt != drawInfo._uniformBlockMap.end() )
            (*( ubIt->second ))( drawInfo, it.second );
    }
}


void Program::setExplicitAttribLocation( GLuint index, const std::string& name )
{
    _explicitVertexAttribLocations[ name ] = index;
}

GLuint Program::getExplicitAttribLocation( const std::string& name ) const
{
    ExplicitLocationMap::const_iterator it(
        _explicitVertexAttribLocations.find( name ) );
    return( ( it != _explicitVertexAttribLocations.end() ) ? it->second : -1 );
}

#ifdef GL_VERSION_4_1
void Program::setParameter( GLenum pname, GLint value )
{
    const unsigned int contextID( 0 );
    const GLuint id( getID( contextID ) );

    glProgramParameteri( id, pname, value );
}
#endif

void Program::get( GLenum pname, GLint *params )
{
    const unsigned int contextID( 0 );
    const GLuint id( getID( contextID ) );

    glGetProgramiv( id, pname, params );
}

GLuint Program::getID( const unsigned int contextID )
{
    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}

void Program::setMaxContexts( const unsigned int numContexts )
{
    ObjectID::setMaxContexts( numContexts );

    _linkStatus._data.resize( numContexts );

    BOOST_FOREACH( const ShaderVec::value_type& shader, _shaders )
    {
        shader->setMaxContexts( numContexts );
    }
}

void logHeader( Poco::LogStream& ls )
{
    ls.width( 16 );
    ls << std::right << "hashcode";
    ls.width( 12 );
    ls << std::left << "  loc  off  ";
    ls.width( 30 );
    ls << "name";
    ls.width( 0 );
    ls << "desc" << std::endl;
}
void logValues( Poco::LogStream& ls, const Program::HashValue& hash, const GLint loc, const GLuint off,
    const std::string& name, const std::string& desc, const bool displayOffset=false )
{
    ls.width( 16 );
    ls << std::hex << std::right << hash;
    ls.width( 2 ); ls << "  ";
    ls.width( 3 );
    ls << std::dec << std::left << loc;
    ls.width( 2 ); ls << "  ";
    ls.width( 3 );
    if( displayOffset )
        ls << off;
    else
        ls << " ";
    ls.width( 2 ); ls << "  ";
    ls.width( 30 );
    ls << name;
    ls.width( 0 );
    ls << desc << std::endl;
}

bool Program::link( unsigned int contextID )
{
    JAG3D_TRACE( "Program::link" );

    if( JAG3D_LOG_DEBUG && ( _ids._data.size() < contextID+1 ) )
    {
        JAG3D_DEBUG( "link(): _ids._data too small." );
        return( false );
    }

    const GLuint id( getID( contextID ) );
    if( JAG3D_LOG_DEBUG )
    {
        _logStream->debug() << "  ContextID: " << contextID <<
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
    BOOST_FOREACH( const ShaderVec::value_type& shader, _shaders )
    {
        GLint shaderID = shader->getID( contextID );
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
        _ids[ contextID ] = 0;
        _linkStatus[ contextID ] = GL_FALSE;
        return( false );
    }

    _linkStatus[ contextID ] = GL_TRUE;
    {
        glUseProgram( id );

        const GLint maxLength( 128 );
        char buf[ maxLength ];

        Poco::LogStream& ls( _logStream->information() );

        // Query uniforms and their locations
        logHeader( ls );

        GLint numUniforms;
        glGetProgramiv( id, GL_ACTIVE_UNIFORMS, &numUniforms );
        GLuintVec namedBlockUniformIndices;
        for( GLint idx = 0; idx < numUniforms; idx++ )
        {
            GLint osize;
            GLenum type;
            glGetActiveUniform( id, idx, (GLsizei)maxLength, NULL, &osize, &type, buf );
            const GLint location = glGetUniformLocation( id, buf );
            if( location == -1 )
            {
                // Must be in a named uniform block.
                namedBlockUniformIndices.push_back( (GLuint)idx );
                continue;
            }
            std::string uniformName( buf );
            if( uniformName.find_last_of( '[' ) != uniformName.npos )
            {
                // Some drivers append "[...]" to end of array names. Strip this.
                uniformName = uniformName.substr( 0, uniformName.find_last_of( '[' ) );
            }
            const HashValue hash( createHash( uniformName ) );
            if( JAG3D_LOG_INFO )
                logValues( ls, hash, location, 0, uniformName, "default block" );
            _uniformLocations[ hash ] = location;
        }

        // Query uniform block info
        GLint numUniformBlocks;
        glGetProgramiv( id, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks );
        _blockInfo.clear();
        GLintVec nuIdx, nuOff;
        nuIdx.resize( namedBlockUniformIndices.size() );
        glGetActiveUniformsiv( id, (GLsizei)namedBlockUniformIndices.size(), namedBlockUniformIndices.data(),
            GL_UNIFORM_BLOCK_INDEX, nuIdx.data() );
        nuOff.resize( namedBlockUniformIndices.size() );
        glGetActiveUniformsiv( id, (GLsizei)namedBlockUniformIndices.size(), namedBlockUniformIndices.data(),
            GL_UNIFORM_OFFSET, nuOff.data() );
        for( GLuint idx = 0; idx < (GLuint)numUniformBlocks; idx++ )
        {
            glUniformBlockBinding( id, idx, idx );
            glGetActiveUniformBlockName( id, idx, maxLength, NULL, buf );
            const std::string blockName( buf );
            const HashValue hash( createHash( blockName ) );
            BlockInfo& bi( _blockInfo[ hash ] );
            bi._bindIndex = idx;
            GLint size;
            glGetActiveUniformBlockiv( id, idx, GL_UNIFORM_BLOCK_DATA_SIZE, &size );
            bi._minSize = (size_t)size;
            if( JAG3D_LOG_INFO )
                logValues( ls, hash, idx, 0, blockName, "block name" );
            unsigned int uidx( 0 );
            BOOST_FOREACH( GLuint blockIdx, nuIdx )
            {
                if( blockIdx == idx )
                {
                    glGetActiveUniformName( id, namedBlockUniformIndices[ uidx ], maxLength, NULL, buf );
                    std::string nubName( buf );
                    if( nubName.find_last_of( '[' ) != nubName.npos )
                    {
                        // Some drivers append "[...]" to end of array names. Strip this.
                        nubName = nubName.substr( 0, nubName.find_last_of( '[' ) );
                    }
                    const HashValue nubHash( createHash( nubName ) );
                    bi._offsets[ nubHash ] = nuOff[ uidx ];
                    if( JAG3D_LOG_INFO )
                        logValues( ls, nubHash, idx, nuOff[ uidx], nubName, "  block: " + blockName, true );
                }
                ++uidx;
            }
        }

        // Query generic vertex attributes and their locations
        GLint numAttribs;
        glGetProgramiv( id, GL_ACTIVE_ATTRIBUTES, &numAttribs );
        for( GLint idx = 0; idx < numAttribs; idx++ )
        {
            GLint osize;
            GLenum type;
            glGetActiveAttrib( id, idx, (GLsizei)maxLength, NULL, &osize, &type, buf );
            const GLint location = glGetAttribLocation( id, buf );
            const std::string attribName( buf );
            const HashValue hash( createHash( attribName ) );
            if( JAG3D_LOG_INFO )
                logValues( ls, hash, location, 0, attribName, "vertex attrib" );
            _vertexAttribLocations[ createHash( attribName ) ] = location;
        }
        glUseProgram( 0 );
    }

    return( true );
}

bool Program::validate( unsigned int contextID )
{
    const GLuint id( getID( contextID ) );

    GLint status;
    glValidateProgram( id );
    glGetProgramiv( id, GL_VALIDATE_STATUS, &status );
    if( status != GL_TRUE )
    {
        printInfoLog( id );
        glDeleteProgram( id );
        _ids[ contextID ] = 0;
        // TBD also set _linkStatus) to false?
    }
    return( status == GL_TRUE );
}


unsigned int Program::getNumActiveUniforms( const GLuint id ) const
{
    return( (unsigned int)( _uniformLocations.size() ) );
}
unsigned int Program::getNumActiveAttribs( const GLuint id ) const
{
    return( ( unsigned int )( _vertexAttribLocations.size() ) );
}


void Program::internalInit( const unsigned int contextID )
{
    const GLuint id( glCreateProgram() );
    JAG3D_ERROR_CHECK( "Program::internalInit()" );
    if( id == 0 )
        JAG3D_ERROR( "glCreateProgram() returned program ID 0." );

    _ids[ contextID ] = id;
    _linkStatus[ contextID ] = GL_FALSE;
}


Program::HashValue Program::createHash( const std::string& name )
{
    boost::hash< std::string > h;
    return( h( name ) );
}

GLint Program::getUniformLocation( const HashValue& h ) const
{
    LocationMap::const_iterator it( _uniformLocations.find( h ) );
    if( it != _uniformLocations.end() )
        return( it->second );
    else
        return( -1 );
}
GLint Program::getUniformLocation( const std::string& s ) const
{
    return( getVertexAttribLocation( createHash( s ) ) );
}
GLint Program::getVertexAttribLocation( const HashValue& h ) const
{
    LocationMap::const_iterator it( _vertexAttribLocations.find( h ) );
    if( it != _vertexAttribLocations.end() )
        return( it->second );
    else
        return( -1 );
}
GLint Program::getVertexAttribLocation( const std::string& s ) const
{
    return( getVertexAttribLocation( createHash( s ) ) );
}

const Program::BlockInfo& Program::getUniformBlockInfo( const HashValue& h ) const
{
    BlockInfoMap::const_iterator it( _blockInfo.find( h ) );
    return( it->second );
}
const Program::BlockInfo& Program::getUniformBlockInfo( const std::string& s ) const
{
    return( getUniformBlockInfo( createHash( s ) ) );
}


// jagDraw
}
