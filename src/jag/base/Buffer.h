/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
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

#ifndef __JAG_BASE_BUFFER_H__
#define __JAG_BASE_BUFFER_H__ 1


#include <jag/base/ptr.h>
#include <jag/base/types.h>

#include <cstring>
#include <assert.h>


namespace jag {
namespace base {


/** \class Buffer Buffer.h <jag/base/Buffer.h>
\brief A generic memory buffer.
*/
class Buffer
{
public:
    Buffer()
      : _data(),
        _size( 0 ),
        _maxSize( 0 )
    {
    }
    Buffer( size_t size )
      : _data( size==0 ? NULL : new unsigned char[ size ] ),
        _size( size ),
        _maxSize( size )
    {
    }
    Buffer( size_t size, const void *ptr )
      : _data( size==0 ? NULL : new unsigned char[ size ] ),
        _size( size ),
        _maxSize( size )
    {
        copy( ptr );
    }
    Buffer( const Buffer& rhs )
      : _data( rhs._data ),
        _size( rhs._size ),
        _maxSize( rhs._maxSize )
    {
    }
    ~Buffer()
    {
    }

    void setSize( size_t size )
    {
        if( size > _maxSize )
            setMaxSize( size );
        _size = size;
    }
    size_t getSize() const
    {
        return( _size );
    }

    void setMaxSize( const size_t maxSize )
    {
        const size_t oldSize( _size );
        UCharArray oldData( _data );
        _maxSize = maxSize;
        _data.reset( new unsigned char[ _maxSize ] );
        std::memcpy( getData(), oldData.get(),
            ( oldSize < _maxSize ) ? oldSize : _maxSize );
    }
    size_t getMaxSize() const
    {
        return( _maxSize );
    }

    const void* getData() const
    {
        return( _data.get() );
    }
    void* getData()
    {
        return( _data.get() );
    }
    void* getOffset( size_t off )
    {
        return( &( _data.get() )[ off ] );
    }
    const void* getOffset( size_t off ) const
    {
        return( &( _data.get() )[ off ] );
    }

    void copy( const void* ptr )
    {
        std::memcpy( _data.get(), ptr, _size );
    }
    void copy( const void* ptr, const size_t size )
    {
        std::memcpy( _data.get(), ptr, _size );
    }
    void copy( const void* ptr, const size_t size, const size_t off )
    {
        std::memcpy( getOffset( off ), ptr, size );
    }

    void append( const Buffer& rhs )
    {
        const size_t oldSize( getSize() );
        const size_t newSize( oldSize + rhs.getSize() );
		
		//this may need to be changed to use a different method for adding headroom to the buffer
		//alternatively we could use something like std::vector which does this automatically
        if( newSize > _maxSize )
            setMaxSize( newSize*2 );
        copy( rhs.getData(), rhs.getSize(), oldSize );
        _size = newSize;
    }

protected:
    jag::base::UCharArray _data;
    size_t _size;
    size_t _maxSize;
};

typedef jag::base::ptr< jag::base::Buffer >::shared_ptr BufferPtr;


// namespace jag::base::
}
}


// __JAG_BASE_BUFFER_H__
#endif
