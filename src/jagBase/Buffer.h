/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAGBASE_BUFFER_H__
#define __JAGBASE_BUFFER_H__ 1


#include <jagBase/ptr.h>
#include <jagBase/types.h>

#include <cstring>
#include <assert.h>


namespace jagBase {


/** \class Buffer Buffer.h <jagBase/Buffer.h>
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
    jagBase::UCharArray _data;
    size_t _size;
    size_t _maxSize;
};

typedef jagBase::ptr< jagBase::Buffer >::shared_ptr BufferPtr;


}


// __JAGBASE_BUFFER_H__
#endif
