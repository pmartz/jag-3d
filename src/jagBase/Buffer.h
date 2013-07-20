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
      : _size( 0 ),
        _data()
    {
    }
    Buffer( size_t size )
      : _size( size ) ,
        _data( size==0 ? NULL : new unsigned char[ size ] )
    {
    }
    Buffer( size_t size, const void *ptr )
      : _size( size ),
        _data( size==0 ? NULL : new unsigned char[ size ] )
    {
        copy( ptr );
    }
    Buffer( const Buffer& b ):
        _size( b._size ),
        _data( b._data )
    {
    }
    ~Buffer()
    {
    }

    void setSize( size_t size, bool copy=true )
    {
        const size_t oldSize( _size );
        UCharArray oldData( _data );
        _size = size;
        _data.reset( new unsigned char[ _size ] );
        if( copy )
        {
            memcpy( getData(), oldData.get(),
                ( oldSize < _size ) ? oldSize : _size );
        }
    }
    size_t getSize() const
    {
        return( _size );
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
        setSize( newSize );
        copy( rhs.getData(), rhs.getSize(), oldSize );
    }

protected:
    jagBase::UCharArray _data;
    size_t _size;
};

typedef jagBase::ptr< jagBase::Buffer >::shared_ptr BufferPtr;


}


// __JAGBASE_BUFFER_H__
#endif
