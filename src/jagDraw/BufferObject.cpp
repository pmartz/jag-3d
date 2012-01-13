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

#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/Error.h>


namespace jagDraw {


BufferObject::BufferObject( BufferObject::Target target )
  : _target( target ),
    _usage( DynamicDraw )
{}

BufferObject::BufferObject( BufferObject::Target target, jagBase::BufferPtr b, Usage usage )
  : _target( target ),
    _usage( usage ),
    _buffer( b )
{}

BufferObject::BufferObject( const BufferObject& rhs )
  : _target( rhs._target ),
    _usage( rhs._usage ),
    _buffer( rhs._buffer ),
    _ids( rhs._ids )
{}


BufferObject::~BufferObject() 
{
}

void BufferObject::setBuffer( jagBase::BufferPtr b ) 
{ 
    _buffer = b;
}

size_t BufferObject::getBufferSize()
{
    return( ( _buffer != NULL ) ? _buffer->getSize() : 0 );
}

void BufferObject::setUsage( const BufferObject::Usage usage ) 
{ 
    _usage = usage; 
}

void BufferObject::apply()
{
    const unsigned int contextID( 0 );

    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );

    glBindBuffer( _target, _ids[ contextID ] );
}

void BufferObject::subData( GLsizeiptr offset, GLsizeiptr size, const GLvoid* data )
{
    const unsigned int contextID( 0 );

    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );

    glBindBuffer( _target, _ids[ contextID ] );
    glBufferSubData( _target, offset, size, data );
}

GLbyte* BufferObject::map( BufferObject::Access access )
{
    const unsigned int contextID( 0 );

    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );

    glBindBuffer( _target, _ids[ contextID ] );
    GLbyte* addr = (GLbyte*)( glMapBuffer( _target, access ) );
    glBindBuffer( _target, 0 );
    return( addr );
}

void BufferObject::unmap()
{
    const unsigned int contextID( 0 );

    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );

    glBindBuffer( _target, _ids[ contextID ] );
    glUnmapBuffer( _target );
    glBindBuffer( _target, 0 );
}


void BufferObject::internalInit( const unsigned int contextID )
{
    _ids._data.resize( contextID + 1 );
    glGenBuffers( 1, &( _ids[ contextID ] ) );

    glBindBuffer( _target, _ids[ contextID ] );
    glBufferData( _target, _buffer->getSize(), _buffer->data(), _usage );
    glBindBuffer( _target, 0 );

    JAG_ERROR_CHECK( "BufferObject::p_internalInit()" );
}


// jagDraw
}
