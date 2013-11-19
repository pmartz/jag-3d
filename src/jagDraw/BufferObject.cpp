/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
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
    _usage( GL_STATIC_DRAW ),
    _dirtyOffset( 0 ),
    _dirtySize( 0 )
{
}
BufferObject::BufferObject( const GLenum target, const jagBase::BufferPtr b, const GLenum usage )
  : jagBase::LogBase( "jag.draw.bufobj" ),
    ObjectID(),
    VertexArrayCommand( VertexArrayCommand::BufferObject_t ),
    _target( target ),
    _usage( usage ),
    _buffer( b ),
    _dirtyOffset( 0 ),
    _dirtySize( 0 )
{
}
BufferObject::BufferObject( const BufferObject& rhs )
  : jagBase::LogBase( rhs ),
    ObjectID( rhs ),
    VertexArrayCommand( rhs ),
    _target( rhs._target ),
    _usage( rhs._usage ),
    _buffer( rhs._buffer ),
    _dirtyOffset( rhs._dirtyOffset ),
    _dirtySize( rhs._dirtySize )
{
    markAllDirty();
}
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

bool BufferObject::isSameKind( const VertexArrayCommand& rhs ) const
{
    const BufferObject* bo( static_cast< const BufferObject* >( &rhs ) );
    return( VertexArrayCommand::isSameKind( rhs ) &&
        ( _target == bo->_target ) &&
        ( _usage == bo->_usage ) );
}

void BufferObject::setBuffer( jagBase::BufferPtr b ) 
{
    _buffer = b;
    markAllDirty();
}
const jagBase::BufferPtr& BufferObject::getBuffer() const
{
    return( _buffer );
}

size_t BufferObject::getBufferSize()
{
    return( ( _buffer != NULL ) ? _buffer->getSize() : 0 );
}

void BufferObject::setTarget( const GLenum target )
{
    _target = target;
}
GLenum BufferObject::getTarget() const
{
    return( _target );
}

void BufferObject::setUsage( const GLenum usage )
{
    _usage = usage; 
}
GLenum BufferObject::getUsage() const
{
    return( _usage );
}

void BufferObject::execute( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );
    const GLuint id( getID( contextID ) );
    if( _target == GL_ELEMENT_ARRAY_BUFFER )
    {
        if( drawInfo._elementBufferID == id )
        {
            // Do not rebind the same element buffer object.
            return;
        }
        drawInfo._elementBufferID = id;
    }

    glBindBuffer( _target, id );

    if( _dirty[ contextID ] )
        sendDirtyBufferData( contextID );
}
void BufferObject::sendDirtyBufferData( const unsigned int contextID )
{
    if( _dirtySize == 0 )
        glBufferData( _target, _buffer->getSize(), _buffer->getData(), _usage );
    else
    {
        GLsizeiptr offset( (GLsizeiptr)_dirtyOffset );
        GLsizeiptr size( (GLsizeiptr)_dirtySize );
        glBufferSubData( _target, offset, size,
            _buffer->getOffset( _dirtyOffset ) );
    }
    _dirty[ contextID ] = false;
}


void BufferObject::setBufferRangeDirty( const size_t offset, const size_t size, const bool dirty )
{
    markAllDirty( dirty );
    _dirtyOffset = offset;
    _dirtySize = size;
}
void BufferObject::markAllDirty( const bool dirty )
{
    for( unsigned int idx=0; idx < _dirty._data.size(); ++idx )
        _dirty[ idx ] = dirty;
    _dirtyOffset = _dirtySize = 0;
}
bool BufferObject::isDirty( const unsigned int contextID )
{
    return( _dirty[ contextID ] != 0 );
}

void BufferObject::setMaxContexts( const unsigned int numContexts )
{
    ObjectID::setMaxContexts( numContexts );

    _dirty._data.resize( numContexts );
    markAllDirty();
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
    glBufferData( _target, _buffer->getSize(), _buffer->getData(), _usage );
    glBindBuffer( _target, 0 );
    _dirty[ contextID ] = false;

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
    const unsigned int contextID( drawInfo._id );
    const GLuint id( getID( contextID ) );
    glBindBufferBase( _target, _index, id );

    if( _dirty[ contextID ] )
        sendDirtyBufferData( contextID );
}


// jagDraw
}
