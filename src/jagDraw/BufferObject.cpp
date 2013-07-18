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
#include <jagBase/LogMacros.h>


namespace jagDraw {


BufferObject::BufferObject( const GLenum target, const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.draw.bufobj" : logName ),
    ObjectID(),
    VertexArrayCommand( VertexArrayCommand::BufferObject_t ),
    _target( target ),
    _usage( GL_STATIC_DRAW )
{}

BufferObject::BufferObject( const GLenum target, const jagBase::BufferPtr b, const GLenum usage )
  : jagBase::LogBase( "jag.draw.bufobj" ),
    ObjectID(),
    VertexArrayCommand( VertexArrayCommand::BufferObject_t ),
    _target( target ),
    _usage( usage ),
    _buffer( b )
{}

BufferObject::BufferObject( const BufferObject& rhs )
  : jagBase::LogBase( rhs ),
    ObjectID( rhs ),
    VertexArrayCommand( rhs ),
    _target( rhs._target ),
    _usage( rhs._usage ),
    _buffer( rhs._buffer )
{}


BufferObject::~BufferObject() 
{
    // TBD Handle object deletion
}

GLuint BufferObject::getID( const unsigned int contextID )
{
    GLuint id( _ids[ contextID ] );
    if( id == 0 )
    {
        internalInit( contextID );
        id = _ids[ contextID ];
    }

    return( id );
}

void BufferObject::setBuffer( jagBase::BufferPtr b ) 
{ 
    _buffer = b;
}

size_t BufferObject::getBufferSize()
{
    return( ( _buffer != NULL ) ? _buffer->getSize() : 0 );
}

void BufferObject::setTarget( const GLenum target )
{
    _target = target;
}
void BufferObject::setUsage( const GLenum usage )
{
    _usage = usage; 
}

void BufferObject::execute( DrawInfo& drawInfo )
{
    const GLuint id( getID( drawInfo._id ) );
    if( _target == GL_ELEMENT_ARRAY_BUFFER )
    {
        if( drawInfo._elementBufferID == id )
        {
            // Do not rebind the same element buffer object.
            return;
        }
        drawInfo._elementBufferID = id;
        JAG3D_DEBUG( "Bound element buffer." );
    }

    glBindBuffer( _target, id );
}

void BufferObject::subData( GLsizeiptr offset, GLsizeiptr size, const GLvoid* data )
{
    const unsigned int contextID( 0 );

    const GLuint id( getID( contextID ) );

    glBindBuffer( _target, id );
    glBufferSubData( _target, offset, size, data );

    // Issue: Copy new data into _buffer?
    // No. _buffer is managed by the calling code that provided it to us.
}

GLbyte* BufferObject::map( const GLenum access )
{
    const unsigned int contextID( 0 );

    const GLuint id( getID( contextID ) );

    glBindBuffer( _target, id );
    GLbyte* addr = (GLbyte*)( glMapBuffer( _target, access ) );
    glBindBuffer( _target, 0 );
    return( addr );
}

void BufferObject::unmap()
{
    const unsigned int contextID( 0 );

    const GLuint id( getID( contextID ) );

    glBindBuffer( _target, id );
    glUnmapBuffer( _target );
    glBindBuffer( _target, 0 );
}


void BufferObject::internalInit( const unsigned int contextID )
{
    glGenBuffers( 1, &( _ids[ contextID ] ) );
    const GLuint id( _ids[ contextID ] );
    JAG3D_ERROR_CHECK( "BufferObject::internalInit() glGenBuffers()" );
    if( id == 0 )
    {
        JAG3D_ERROR( "glGenBuffers() generated buffer ID 0." );
    }

    glBindBuffer( _target, id );
    glBufferData( _target, _buffer->getSize(), _buffer->data(), _usage );
    glBindBuffer( _target, 0 );

    JAG3D_ERROR_CHECK( "BufferObject::internalInit() glBufferData()" );
}



IndexedBufferObject::IndexedBufferObject( const GLenum target )
  : BufferObject( target ),
    _index( 0 )
{
}
IndexedBufferObject::IndexedBufferObject( const GLenum target, const jagBase::BufferPtr b, const GLenum usage )
  : BufferObject( target, b, usage ),
    _index( 0 )
{
}
IndexedBufferObject::IndexedBufferObject( const IndexedBufferObject& rhs )
  : BufferObject( rhs ),
    _index( 0 )
{
}
IndexedBufferObject::~IndexedBufferObject()
{
}

void IndexedBufferObject::execute( DrawInfo& drawInfo )
{
    const GLuint id( getID( drawInfo._id ) );
    glBindBufferBase( _target, _index, id );
}


// jagDraw
}
