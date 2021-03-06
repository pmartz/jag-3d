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

#include <jag/draw/Program.h>
#include <jag/draw/PlatformOpenGL.h>
#include <jag/draw/DrawInfo.h>
#include <jag/base/LogMacros.h>
#include <jag/draw/Error.h>
#include <stdio.h>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>


namespace jag {
namespace draw {


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

Program::Program( const std::string& logName )
  : Command( Program_t ),
    ObjectID(),
    jag::base::LogBase( logName.empty() ? "jag.draw.program" : logName )
{
}
Program::Program( const Program& rhs )
  : Command( rhs ),
    ObjectID( rhs ),
    SHARED_FROM_THIS( Program )( rhs ),
    jag::base::LogBase( rhs ),
    _shaders( rhs._shaders ),
    _parameters( rhs._parameters ),
    _explicitVertexAttribLocations( rhs._explicitVertexAttribLocations ),
    _uniformAliases( rhs._uniformAliases ),
    _vertexAttribAliases( rhs._vertexAttribAliases )
{
    unsigned int idx;
    _linkStatus._data.resize( rhs._linkStatus._data.size() );
    for( idx=0; idx < _linkStatus._data.size(); ++idx )
        _linkStatus[ idx ] = false;

    _needsDetach._data.resize( rhs._needsDetach._data.size() );
    for( idx=0; idx < _needsDetach._data.size(); ++idx )
        _needsDetach[ idx ] = false;

    _parametersDirty._data.resize( rhs._parametersDirty._data.size() );
    for( idx=0; idx < _parametersDirty._data.size(); ++idx )
        _parametersDirty[ idx ] = true;
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
    forceRelink();
}
void Program::detachAllShaders()
{
    BOOST_FOREACH( ShaderPtr& shader, _shaders )
    {
        _detachedShaders.push_back( shader );
    }
    for( unsigned int idx=0; idx<_needsDetach._data.size(); ++idx )
    {
        _needsDetach[ idx ] = true;
    }
    _shaders.clear();

    forceRelink();
}

void Program::forceRelink()
{
    for( unsigned int idx=0; idx<_linkStatus._data.size(); ++idx )
    {
        _linkStatus[ idx ] = false;
    }
}

void Program::execute( DrawInfo& drawInfo )
{
    // Record the currently used program in DrawInfo.
    // Downstream vertex attribs and uniforms will query
    // this for location values.
    drawInfo._current.insert( shared_from_this() );

    const unsigned int contextID( ( unsigned int )( drawInfo._id ) );
    const GLuint id( getID( contextID ) );
    const bool& linked( _linkStatus[ contextID ] != 0 );

    if( !linked )
    {
        if( id == 0 )
        {
            // This is out of the ordinary. getID() should have called glCreateProgram().
            JAG3D_ERROR( "Program ID==0." );
            return;
        }
        if( !( link( contextID ) ) )
        {
            std::ostringstream ostr;
            ostr << "Program link failed, id: " << id << ".";
            JAG3D_ERROR( ostr.str() );
            return;
        }
    }

    if( JAG3D_LOG_TRACE )
    {
        std::ostringstream ostr;
        ostr << "execute(): Executing glUseProgram( " << id << " );";
        JAG3D_TRACE( ostr.str() );
    }
    glUseProgram( id );

    // Iterate over active uniforms. If an active uniform matches a uniform
    // in the drawInfo._uniformMap, set its value in GL state.
    BOOST_FOREACH( const LocationMap::value_type& it, _uniformLocations )
    {
        const HashValue& hash( it.first );
        DrawInfo::UniformMap::iterator uIt( drawInfo._uniformMap.find( hash ) );
        if( uIt != drawInfo._uniformMap.end() )
            uIt->second->execute( drawInfo, it.second );
    }

    // Iterate over active uniform blocks. If an active uniform block matches a uniform
    // in the drawInfo._uniformBlockMap, bind the uniform buffer object.
    BOOST_FOREACH( const BlockInfoMap::value_type& it, _blockInfo )
    {
        const HashValue& hash( it.first );
        DrawInfo::UniformBlockMap::iterator ubIt( drawInfo._uniformBlockMap.find( hash ) );
        if( ubIt != drawInfo._uniformBlockMap.end() )
            ubIt->second->execute( drawInfo, it.second );
    }

    // Set program parameters, if dirty.
    if( _parametersDirty[ contextID ] != 0 )
    {
#ifdef GL_VERSION_4_1
        BOOST_FOREACH( const ParameterMap::value_type& item, _parameters )
            glProgramParameteri( id, item.first, item.second );
#endif
        _parametersDirty[ contextID ] = false;
    }

    JAG3D_ERROR_CHECK( "Program::execute()" );
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

void Program::setParameter( GLenum pname, GLint value )
{
    _parameters[ pname ] = value;
    for( unsigned int idx=0; idx < _parametersDirty._data.size(); ++idx )
        _parametersDirty[ idx ] = true;
}
bool Program::getParameter( GLenum pname, GLint *params ) const
{
    ParameterMap::const_iterator it( _parameters.find( pname ) );
    if( it == _parameters.end() )
        return( false );

    *params = it->second;
    return( true );
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

    unsigned int oldSize( ( const unsigned int )( _linkStatus._data.size() ) );
    _linkStatus._data.resize( numContexts );
    for( unsigned int idx = oldSize; idx < _linkStatus._data.size(); ++idx )
        _linkStatus[ idx ] = false;

    oldSize = ( const unsigned int )( _needsDetach._data.size() );
    _needsDetach._data.resize( numContexts );
    for( unsigned int idx = oldSize; idx < _needsDetach._data.size(); ++idx )
        _needsDetach[ idx ] = false;

    oldSize = ( const unsigned int )( _parametersDirty._data.size() );
    _parametersDirty._data.resize( numContexts );
    for( unsigned int idx = oldSize; idx < _parametersDirty._data.size(); ++idx )
        _parametersDirty[ idx ] = true;

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
    JAG3D_ERROR_CHECK( "Program::link(), after getID()" );

    // Loop over explicitly specified vertex attrib locations and
    // set them in this program object.
    JAG3D_TRACE( "  link(): Binding explicit attribute locations." );
    BOOST_FOREACH( const ExplicitLocationMap::value_type& it, _explicitVertexAttribLocations )
    {
        glBindAttribLocation( id, it.second, it.first.c_str() );
    }

    // Detach any shaders pending detachment.
    internalDetach( contextID );

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
        _linkStatus[ contextID ] = false;
        return( false );
    }

    _linkStatus[ contextID ] = true;
    {
        glUseProgram( id );

        const GLint maxLength( 128 );
        char buf[ maxLength ];

        Poco::LogStream& ls( _logStream->information() );


        logHeader( ls );

        // Query uniforms and their locations
        _uniformLocations.clear();
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

        // Make location entires for uniform aliases.
        BOOST_FOREACH( const NameMap::value_type& dataPair, _uniformAliases )
        {
            const HashValue nameHash( createHash( dataPair.first ) );
            const HashValue aliasHash( createHash( dataPair.second ) );
            LocationMap::const_iterator nameIt( _uniformLocations.find( nameHash ) );
            LocationMap::const_iterator aliasIt( _uniformLocations.find( aliasHash ) );

            if( ( nameIt != _uniformLocations.end() ) && ( aliasIt != _uniformLocations.end() ) )
            {
                // Both are already in the location map.
                JAG3D_WARNING( "Name \"" + dataPair.first + "\" and alias \"" + dataPair.second +
                    "\" already found in _uniformLocations." );
                continue;
            }
            if( nameIt != _uniformLocations.end() )
                // The name is used in this program. Add a location entry for the alias.
                _uniformLocations[ aliasHash ] = nameIt->second;
            else if( aliasIt != _uniformLocations.end() )
                // The alias is used in this program. Add a location entry for the name.
                _uniformLocations[ nameHash ] = aliasIt->second;
        }


        // Query uniform block info
        _blockInfo.clear();
        GLint numUniformBlocks;
        glGetProgramiv( id, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks );
        if( numUniformBlocks > 0 )
        {
            GLintVec nuIdx, nuOff;
            nuIdx.resize( namedBlockUniformIndices.size() );
            glGetActiveUniformsiv( id, (GLsizei)namedBlockUniformIndices.size(), &(namedBlockUniformIndices[0]),
                GL_UNIFORM_BLOCK_INDEX, &(nuIdx[0]) );
            nuOff.resize( namedBlockUniformIndices.size() );
            glGetActiveUniformsiv( id, (GLsizei)namedBlockUniformIndices.size(), &(namedBlockUniformIndices[0]),
                GL_UNIFORM_OFFSET, &(nuOff[0]) );
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
#ifndef JAG3D_USE_GLES3
                        glGetActiveUniformName( id, namedBlockUniformIndices[ uidx ], maxLength, NULL, buf );
#else
                        //GLES3 TODO, how to get uniform name?
#endif
                        std::string nubName( buf );
                        if( nubName.find_last_of( '[' ) != nubName.npos )
                        {
                            // Some drivers append "[...]" to end of array names. Strip this.
                            nubName = nubName.substr( 0, nubName.find_last_of( '[' ) );
                        }
                        if( nubName.find_last_of( '.' ) != nubName.npos )
                        {
                            // Some drivers combine block name and uniform name.
                            // Strip the block name, leave only the uniform name.
                            nubName = nubName.substr( nubName.find_last_of( '.' ) + 1 );
                        }
                        const HashValue nubHash( createHash( nubName ) );
                        bi._offsets[ nubHash ] = nuOff[ uidx ];
                        if( JAG3D_LOG_INFO )
                            logValues( ls, nubHash, idx, nuOff[ uidx], nubName, "  block: " + blockName, true );
                    }
                    ++uidx;
                }
            }
        }


        // Query generic vertex attributes and their locations
        _vertexAttribLocations.clear();
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

        // Make location entires for vertex attrib aliases.
        BOOST_FOREACH( const NameMap::value_type& dataPair, _vertexAttribAliases )
        {
            const HashValue nameHash( createHash( dataPair.first ) );
            const HashValue aliasHash( createHash( dataPair.second ) );
            LocationMap::const_iterator nameIt( _vertexAttribLocations.find( nameHash ) );
            LocationMap::const_iterator aliasIt( _vertexAttribLocations.find( aliasHash ) );

            if( ( nameIt != _vertexAttribLocations.end() ) && ( aliasIt != _vertexAttribLocations.end() ) )
            {
                // Both are already in the location map.
                JAG3D_WARNING( "Name \"" + dataPair.first + "\" and alias \"" + dataPair.second +
                    "\" already found in _vertexAttribLocations." );
                continue;
            }
            if( nameIt != _vertexAttribLocations.end() )
                // The name is used in this program. Add a location entry for the alias.
                _vertexAttribLocations[ aliasHash ] = nameIt->second;
            else if( aliasIt != _vertexAttribLocations.end() )
                // The alias is used in this program. Add a location entry for the name.
                _vertexAttribLocations[ nameHash ] = aliasIt->second;
        }

        glUseProgram( 0 );
    }

    JAG3D_ERROR_CHECK( "Program::link()" );
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
    if( id == 0 )
    {
        JAG3D_ERROR( "glCreateProgram() returned program ID 0." );
    }

    _ids[ contextID ] = id;
    _linkStatus[ contextID ] = false;

    JAG3D_ERROR_CHECK( "Program::internalInit()" );
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

bool Program::hasUniformBlock( const HashValue& h ) const
{
    return( _blockInfo.find( h ) != _blockInfo.end() );
}
bool Program::hasUniformBlock( const std::string& s ) const
{
    return( hasUniformBlock( createHash( s ) ) );
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

void Program::addUniformAlias( const std::string& name, const std::string& alias )
{
    _uniformAliases[ name ] = alias;
}
void Program::addVertexAttribAlias( const std::string& name, const std::string& alias )
{
    _vertexAttribAliases[ name ] = alias;
}


void Program::internalDetach( const unsigned int contextID )
{
    JAG3D_TRACE( "internalDetach()" );

    const GLuint id( getID( contextID ) );

    GLint numShaders;
    glGetProgramiv( id, GL_ATTACHED_SHADERS, &numShaders );
    if( numShaders > 0 )
    {
        // Get list of attached shader ID.
        GLuintVec idList; idList.resize( numShaders );
        GLsizei tempNum = numShaders;
        glGetAttachedShaders( id, GLsizei( numShaders ), &tempNum, &(idList[0]) );

        // Detach shaders for the specified context.
        if( _needsDetach[ contextID ] )
        {
            JAG3D_TRACE( "Found 'true' entry in _needsDetach" );
            _needsDetach[ contextID ] = false;
            BOOST_FOREACH( ShaderPtr& shader, _detachedShaders )
            {
                const GLuint shaderID( shader->getID( contextID ) );
                // Only detach the shader if it's already attached.
                // Calling glDetachShader on a shader that is not attached will
                // generate INVALID_OPERATION.
                BOOST_FOREACH( GLuint attachedShader, idList )
                {
                    if( attachedShader == shaderID )
                        glDetachShader( id, shaderID );
                }
            }
        }
    }

    // Clean up. If all shaders have been detached on all
    // contexts, then clear the list of detached shaders.
    bool allDetached( true );
    for( unsigned int idx = 0; idx < _needsDetach._data.size(); ++idx )
    {
        if( ( contextID == idx ) && ( numShaders == 0 ) )
        {
            // There were no shaders attached on this context. Could happen
            // if Shader objects were added, but replaced prior to link.
            _needsDetach[ idx ] = false;
        }
        else if( _needsDetach[ idx ] )
        {
            allDetached = false;
            break;
        }
    }
    if( allDetached )
        _detachedShaders.clear();

    JAG3D_ERROR_CHECK( "Program::internalDetach()" );
}


// namespace jag::draw::
}
}
