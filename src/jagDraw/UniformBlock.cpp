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
    jagBase::LogBase( "jag.draw.ublock" ),
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


void UniformBlock::operator()( DrawInfo& drawInfo, const GLuint bindIndex ) const
{
    std::ostringstream ostr;
    ostr << bindIndex;
    JAG3D_TRACE( "operator(): " + _name + ", bindIndex: " + ostr.str() );
}
void UniformBlock::operator()( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "operator(): " + _name );
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
