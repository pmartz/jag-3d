/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag-3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
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

#include <jagBase/PlatformOpenGL.h>
#include <jagDraw/BufferObject.h>
#include <stdio.h>


namespace jagDraw {


BufferObject::BufferObject( BufferObject::Target target )
  : _target( target ),
    _gfxInited( false ),
    _usage( DynamicDraw )
{}

BufferObject::BufferObject( BufferObject::Target target, jagBase::BufferPtr b, Usage usage )
  : _target( target ),
    _gfxInited( false ),
    _usage( usage ),
    _buffer( b )
{}

BufferObject::BufferObject( const BufferObject& b )
  : _target( b._target ),
    _gfxInited( b._gfxInited ),
    _usage( b._usage ),
    _buffer( b._buffer ),
    _id( b._id )
{}


BufferObject::~BufferObject() 
{
}

void BufferObject::setBuffer( jagBase::BufferPtr b ) 
{ 
    _buffer = b;
}

jagBase::BufferPtr BufferObject::getBuffer() 
{ 
    return( _buffer );
}

void BufferObject::setUsage( BufferObject::Usage usage ) 
{ 
    _usage = usage; 
}

BufferObject::Usage BufferObject::getUsage() 
{
    return( _usage );
}

void BufferObject::apply()
{
    if( !_gfxInited && !p_gfxInit() )
        return;

//    stats();

    glBindBuffer( _target, _id );
}

void BufferObject::gfxInit()
{
    if( !_gfxInited )
        p_gfxInit();
}

void BufferObject::subData( GLsizeiptr offset, GLsizeiptr size, const GLvoid* data )
{
    glBindBuffer( _target, _id );
    glBufferSubData( _target, offset, size, data );
}

GLbyte* BufferObject::map( BufferObject::Access access )
{
    if( !_gfxInited )
        p_gfxInit();

    glBindBuffer( _target, _id );
    GLbyte* addr = (GLbyte*)( glMapBuffer( _target, access ) );
    glBindBuffer( _target, 0 );
    return( addr );
}

void BufferObject::unmap()
{
    if( !_gfxInited )
        p_gfxInit();

    glBindBuffer( _target, _id );
    glUnmapBuffer( _target );
    glBindBuffer( _target, 0 );
}


bool BufferObject::p_gfxInit()
{
    if( _gfxInited == true )
        return( _gfxInited );

    if( _buffer.get() == NULL )
        return( _gfxInited );

    glGenBuffers( 1, &_id );

    glBindBuffer( _target, _id );
    glBufferData( _target, _buffer->getSize(), _buffer->data(), _usage );
    glBindBuffer( _target, 0 );

    _bufferSize = _buffer->getSize();

    return( _gfxInited = true );
}


// jagDraw
}
