// Copyright

#ifndef __JAGBASE_BUFFER_H__
#define __JAGBASE_BUFFER_H__ 1


#include <jagBase/ptr.h>

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
      : _size( 0 )
    {}

    Buffer( size_t size )
      : _size( size ) 
    {
        if( _size == 0 )
            _buffer = 0L;
        else
            _buffer = new unsigned char[ _size ];
    }

    Buffer( size_t size, void *ptr )
      : _size( size )
    {
        _buffer = new unsigned char[ _size ];
        copy( ptr );
    }

    Buffer( const Buffer& b ):
        _size( b._size ),
        _buffer( b._buffer )
    {}

    ~Buffer()
    {
    }

    void setSize( size_t size )
    {
        _size = size;
        _buffer = new unsigned char[ _size ];
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

    jagBase::ptr< unsigned char >::shared_array_ptr _buffer;
};

typedef jagBase::ptr< jagBase::Buffer >::shared_ptr BufferPtr;


}


// __JAGBASE_BUFFER_H__
#endif
