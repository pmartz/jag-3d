// Copyright

#ifndef __JAGBASE_BUFFER_H__
#define __JAGBASE_BUFFER_H__ 1


#include <jagBase/ptr.h>
#include <jagBase/types.h>

#include <string.h> // for memset
#include <assert.h>


namespace jagBase {


/** \class Buffer Buffer.h <jagBase/Buffer.h>
\brief A generic memory buffer.
*/
class Buffer
{
public:
    Buffer()
      : _size( 0 ),
        _buffer( NULL )
    {
    }

    Buffer( size_t size )
      : _size( size ) ,
        _buffer( size==0 ? NULL : new unsigned char[ size ] )
    {
    }

    Buffer( size_t size, void *ptr )
      : _size( size ),
        _buffer( size==0 ? NULL : new unsigned char[ size ] )
    {
        copy( ptr );
    }

    Buffer( const Buffer& b ):
        _size( b._size ),
        _buffer( b._buffer )
    {
    }

    ~Buffer()
    {
    }

    void setSize( size_t size )
    {
        _size = size;
        _buffer.reset( new unsigned char[ _size ] );
    }

    size_t size() { return( _size ); }
    size_t getSize() { return( _size ); }

    void* ptr() { return( _buffer.get() ); }
    void* data() { return( _buffer.get() ); }
    void* offset( size_t off ) { return( &( _buffer.get() )[ off ] ); }

    void copy( void* ptr )
    {
        memcpy( _buffer.get(), ptr, _size );
    }

    void copy( void* ptr, size_t size )
    {
        memcpy( _buffer.get(), ptr, _size );
    }

    void copy( void* ptr, size_t size, size_t off )
    {
        memcpy( offset( off ), ptr, size );
    }

private:
    size_t _size;

    jagBase::UCharArray _buffer;
};

typedef jagBase::ptr< jagBase::Buffer >::shared_ptr BufferPtr;


}


// __JAGBASE_BUFFER_H__
#endif
