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
        if( newSize > _maxSize )
            setMaxSize( newSize );
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
