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
        _buffer( NULL )
    {
    }

    Buffer( size_t size )
      : _size( size ) ,
        _buffer( size==0 ? NULL : new unsigned char[ size ] )
    {
    }

    Buffer( size_t size, const void *ptr )
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

    void copy( const void* ptr )
    {
        std::memcpy( _buffer.get(), ptr, _size );
    }

    void copy( void* ptr, size_t size )
    {
        std::memcpy( _buffer.get(), ptr, _size );
    }

    void copy( void* ptr, size_t size, size_t off )
    {
        std::memcpy( offset( off ), ptr, size );
    }

private:
    size_t _size;

    jagBase::UCharArray _buffer;
};

typedef jagBase::ptr< jagBase::Buffer >::shared_ptr BufferPtr;


}


// __JAGBASE_BUFFER_H__
#endif
