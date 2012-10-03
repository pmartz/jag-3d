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

#include <jagDraw/UniformBlock.h>
#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/DrawInfo.h>
#include <jagBase/LogMacros.h>
#include <jagDraw/Error.h>
#include <stdio.h>
#include <iostream>

#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>


namespace jagDraw {


UniformBlock::UniformBlock( const std::string& name )
  : DrawablePrep(),
    ObjectIDOwner(),
    jagBase::LogBase( "jag.draw.ublock" ),
    _name( name )
{
    _nameHash = Program::createHash( _name );
    JAG3D_TRACE( "Constructor: " + _name );
}
UniformBlock::UniformBlock( const UniformBlock& rhs )
  : DrawablePrep( rhs ),
    ObjectIDOwner( rhs ),
    jagBase::LogBase( rhs ),
    _name( rhs._name ),
    _nameHash( rhs._nameHash ),
    _uniforms( rhs._uniforms ),
    _bufferObject( rhs._bufferObject ),
    _buffer( rhs._buffer )
{
    JAG3D_TRACE( "Copy constructor: " + _name );
}
UniformBlock::~UniformBlock()
{
    JAG3D_TRACE( "Destructor: " + _name );
}


void UniformBlock::addUniform( UniformPtr uniform )
{
    _uniforms.push_back( uniform );
}


void UniformBlock::operator()( DrawInfo& drawInfo, const Program::BlockInfo& blockInfo ) const
{
    if( JAG3D_LOG_TRACE )
    {
        std::ostringstream ostr;
        ostr << blockInfo._bindIndex;
        JAG3D_TRACE( "operator(): " + _name + ", bindIndex: " + ostr.str() );
    }

    if( _buffer->size() < blockInfo._minSize )
        _buffer->setSize( blockInfo._minSize );

    // TBD load uniforms into buffer
    Program::OffsetMap offsets( blockInfo._offsets );
    BOOST_FOREACH( UniformPtr uniform, _uniforms )
    {
        Program::OffsetMap::const_iterator it( offsets.find( uniform->getNameHash() ) );
        if( it != offsets.end() )
        {
            const GLuint offsetValue( it->second );

#define HANDLE_CASE(__typeid,__type) \
            case __typeid: \
            { \
                __type value; \
                uniform->get( value ); \
                memcpy( _buffer->offset( offsetValue ), &value, sizeof( __type ) ); \
                break; \
            }

            switch( uniform->getType() )
            {
            HANDLE_CASE( GL_FLOAT, GLfloat )
            HANDLE_CASE( GL_FLOAT_VEC3, gmtl::Point3f )
            default:
                JAG3D_ERROR( "operator(): Unsupported uniform type." );
                break;
            }

#undef HANDLE_CASE

        }
    }

    // bind 
    _bufferObject->setIndex( blockInfo._bindIndex );
    (*_bufferObject)( drawInfo );
}
void UniformBlock::operator()( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "operator(): " + _name );

    // Add this uniform block to the pool of potentially active uniform blocks
    // for the current frame and draw thread.
    drawInfo._uniformBlockMap[ _nameHash ] = shared_from_this();

    // UniformBlock::operator() could execute before Program::operator(),
    // so only look up uniform block info if a Program is available.
    if( drawInfo._program != NULL )
    {
        Program::BlockInfo blockInfo( drawInfo._program->getUniformBlockInfo( _nameHash ) );
        operator()( drawInfo, blockInfo );
    }
}

void UniformBlock::setMaxContexts( const unsigned int numContexts )
{
    JAG3D_TRACE( "setMaxContexts: " + _name );

    if( _buffer == NULL )
        _buffer = jagBase::BufferPtr( new jagBase::Buffer() );
    if( _bufferObject == NULL )
        _bufferObject = UniformBufferPtr( new UniformBuffer( _buffer ) );

    _bufferObject->setMaxContexts( numContexts );
}
void UniformBlock::deleteID( const jagDraw::jagDrawContextID contextID )
{
    JAG3D_TRACE( "deleteID: " + _name );

    if( _bufferObject != NULL )
        _bufferObject->deleteID( contextID );
}


// jagDraw
}
