/* @License 
 -------------------------------------------------------------------------------
 | Chaskii - Copyright (C) 2010  Don Burns, Andes Computer Engineering, LLC    |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
#ifndef CHASKII_MEM_BUFFER_TEMPLATE_DEF
#define CHASKII_MEM_BUFFER_TEMPLATE_DEF 1

#include <string.h> // for memset
#include <assert.h>

#include <Chaskii/Export.h>
#include <Chaskii/Mem/ptr.h>

namespace iiMem {

class II_EXPORT Buffer
{
    public:
        Buffer():
            _size(0)
        {}

        Buffer( size_t size ):
            _size(size) 
        {
            if( _size == 0 )
                _buffer = 0L;
            else
                _buffer = new unsigned char[_size];
        }

        Buffer( size_t size, void *ptr):
            _size(size)
        {
            _buffer = new unsigned char[_size];
            copy( ptr );
        }

        Buffer( const Buffer &b ):
            _size(b._size),
            _buffer(b._buffer)
        {}

        ~Buffer()
        {
        }

        void setSize( size_t size )
        {
            _size = size;
            _buffer = new unsigned char[_size];
        }

        size_t size() { return _size; }
        size_t getSize() { return _size; }

        void *ptr() { return _buffer.get(); }
        void *data() { return _buffer.get(); }
        void *offset( size_t off ) { return &(_buffer.get())[off]; }

        void copy( void *ptr )
        {
            memcpy( _buffer.get(), ptr, _size );
        }

        void copy( void *ptr, size_t size )
        {
            memcpy( _buffer.get(), ptr, _size );
        }

        void copy( void *ptr, size_t size, size_t off )
        {
            memcpy( offset( off ), ptr, size );
        }

    private:
        size_t _size;
        iiMem::ptr<unsigned char>::shared_array_ptr _buffer;
};

typedef ptr<Buffer>::shared_ptr Buffer_ptr;


}

#endif
