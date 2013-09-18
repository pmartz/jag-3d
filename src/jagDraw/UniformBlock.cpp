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

#include <cstring>


namespace jagDraw {


UniformBlock::UniformBlock( const std::string& name, const std::string& logName )
  : DrawablePrep( UniformBlock_t ),
    ObjectIDOwner(),
    SHARED_FROM_THIS( UniformBlock )(),
    jagBase::LogBase( logName.empty() ? "jag.draw.ublock" : logName ),
    _name( name ),
    _dirty( true )
{
    _nameHash = Program::createHash( _name );
    JAG3D_TRACE( "Constructor: " + _name );
}
UniformBlock::UniformBlock( const UniformBlock& rhs )
  : DrawablePrep( rhs ),
    ObjectIDOwner( rhs ),
    SHARED_FROM_THIS( UniformBlock )( rhs ),
    jagBase::LogBase( rhs ),
    _name( rhs._name ),
    _nameHash( rhs._nameHash ),
    _uniforms( rhs._uniforms ),
    _bufferObject( rhs._bufferObject ),
    _buffer( rhs._buffer ),
    _dirty( rhs._dirty )
{
    JAG3D_TRACE( "Copy constructor: " + _name );
}
UniformBlock::~UniformBlock()
{
    JAG3D_TRACE( "Destructor: " + _name );
}


std::string UniformBlock::getName() const
{
    return( _name );
}
Program::HashValue UniformBlock::getNameHash() const
{
    return( _nameHash );
}
void UniformBlock::addUniform( UniformPtr uniform )
{
    _uniforms.push_back( uniform );
    _dirty = true;
}


void UniformBlock::execute( DrawInfo& drawInfo, const Program::BlockInfo& blockInfo ) const
{
    if( JAG3D_LOG_TRACE )
    {
        std::ostringstream ostr;
        ostr << blockInfo._bindIndex;
        JAG3D_TRACE( "execute(): " + _name + ", bindIndex: " + ostr.str() );
    }

    if( _dirty )
    {
        if( _buffer->getSize() < blockInfo._minSize )
            _buffer->setSize( blockInfo._minSize );

        // TBD load uniforms into buffer
        const Program::OffsetMap& offsets( blockInfo._offsets );
        BOOST_FOREACH( const UniformPtr& uniform, _uniforms )
        {
            Program::OffsetMap::const_iterator it( offsets.find( uniform->getNameHash() ) );
            if( it != offsets.end() )
            {
                const GLuint offsetValue( it->second );
                uniform->copyValue( _buffer->getOffset( offsetValue ) );
            }
        }
        _bufferObject->setBufferDirty();

        _dirty = false;
    }

    // bind 
    _bufferObject->setIndex( blockInfo._bindIndex );
    _bufferObject->execute( drawInfo );
}
void UniformBlock::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute(): " + _name );

    // Add this uniform block to the pool of potentially active uniform blocks
    // for the current frame and draw thread.
    DrawInfo::UniformBlockMap::iterator ubp( drawInfo._uniformBlockMap.find( _nameHash ) );
    UniformBlockPtr& sharedThis( shared_from_this() );
    if( ( ubp == drawInfo._uniformBlockMap.end() ) ||
        ( ubp->second != sharedThis ) )
        drawInfo._uniformBlockMap[ _nameHash ] = sharedThis;
    else
        // No change.
        return;

    // UniformBlock::execute() could execute before Program::execute(),
    // so only look up uniform block info if a Program is available.
    if( drawInfo._current.contains( Program_t ) )
    {
        ProgramPtr prog( boost::dynamic_pointer_cast< Program >( drawInfo._current[ Program_t ] ) );
        if( prog->hasUniformBlock( _nameHash ) )
        {
            const Program::BlockInfo& blockInfo( prog->getUniformBlockInfo( _nameHash ) );
            execute( drawInfo, blockInfo );
        }
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
