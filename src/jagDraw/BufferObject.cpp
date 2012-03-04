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
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>


namespace jagDraw {


BufferObject::BufferObject( const GLenum target )
  : _target( target ),
    _usage( GL_STATIC_DRAW )
{}

BufferObject::BufferObject( const GLenum target, const jagBase::BufferPtr b, const GLenum usage )
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
    // TBD Handle object deletion
}

void BufferObject::setBuffer( jagBase::BufferPtr b ) 
{ 
    _buffer = b;
}

size_t BufferObject::getBufferSize()
{
    return( ( _buffer != NULL ) ? _buffer->getSize() : 0 );
}

void BufferObject::setUsage( const GLenum usage )
{ 
    _usage = usage; 
}

void BufferObject::bind( const DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );

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

    // Issue: Copy new data into _buffer?
    // No. _buffer is managed by the calling code that provided it to us.
}

GLbyte* BufferObject::map( const GLenum access )
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

    JAG_ERROR_CHECK( "BufferObject::internalInit()" );
}


// jagDraw
}
