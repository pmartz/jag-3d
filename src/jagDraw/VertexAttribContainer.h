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

#ifndef __JAGDRAW_VERTEX_ATTRIB_CONTAINER_H__
#define __JAGDRAW_VERTEX_ATTRIB_CONTAINER_H__ 1


#include <jagBase/Buffer.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/VertexAttrib.h>
#include <jagDraw/DrawCommand.h>

#include <iterator>
#include <iostream>


namespace jagDraw {


/** VertexAttribContainer VertexAttribContainer.h <jagDraw/VertexAttribContainer.h>
\brief Iteration over vertex attribute data
\details Given a buffer of data, a vertex attribute describing that data,
and an OpenGL drawing command, this class provides an iterator interface
for the contained vertex attribute data. Useful for enumerating vertices
to compute a bounding volume, for example. 

Future work:

There's no need for this class to be dependent on Jag3D. It should
provide a second constructor that takes equivalent OpenGL parameters.
*/
template< typename T >
class VertexAttribContainer
{
public:
    VertexAttribContainer( const BufferObjectPtr bop, const VertexAttribPtr vap, const DrawCommandPtr dcp )
      : _buffer( bop->getBuffer() ),
        _vap( vap ),
        _dcp( dcp ),
        _step( _vap->getActualStride() ),
        _offset( _vap->getOffset() )
    {
    }
    VertexAttribContainer( const VertexAttribContainer<T>& rhs )
      : _buffer( rhs._buffer ),
        _vap( rhs._vap ),
        _dcp( rhs._dcp ),
        _step( rhs._step )
    {
    }
    ~VertexAttribContainer()
    {
    }



    /** iterator iterator.h <jagDraw/iterator.h>
    \brief TBD
    \details TBD */
    class iterator : public std::iterator< std::bidirectional_iterator_tag, T >
    {
        friend class VertexAttribContainer;

    public:
        iterator()
          : _counter( 0 ),
            _vaCont( NULL )
        {
        }
        iterator( const iterator& rhs )
          : _counter( rhs._counter ),
            _vaCont( rhs._vaCont )
        {
        }
        ~iterator()
        {
        }

        T& operator*()
        {
            return( _vaCont->getVertexAttrib( _counter ) );
        }
        const T& operator*() const
        {
            return( _vaCont->getVertexAttrib( _counter ) );
        }

        iterator& operator++() // prefix inc
        {
            ++_counter;
            return( *this );
        }
        iterator operator++( int ) // postfix inc
        {
            const iterator& result( *this );
            ++( *this );
            return( result );
        }
        iterator& operator--() // prefix dec
        {
            --_counter;
            return( *this );
        }
        iterator operator--( int ) // postfix dec
        {
            const iterator& result( *this );
            --( *this );
            return( result );
        }

        bool operator==( const iterator& rhs ) const
        {
            return( ( _counter == rhs._counter ) &&
                ( _vaCont == rhs._vaCont ) );
        }
        bool operator!=( const iterator& rhs ) const
        {
            return( !( *this == rhs ) );
        }

        iterator& operator=( const iterator& rhs )
        {
            _counter = rhs._counter;
            _vaCont = rhs._vaCont;
            return( *this );
        }

    protected:
        iterator( VertexAttribContainer<T>* vaCont, const unsigned int counter )
          : _counter( counter ),
            _vaCont( vaCont )
        {
        }

        unsigned int _counter;
        VertexAttribContainer<T>* _vaCont;
    };



    iterator begin()
    {
        return( iterator( this, 0 ) );
    }

    iterator end()
    {
        return( iterator( this, _dcp->getNumIndices() ) );
    }

    T& getVertexAttrib( const unsigned int count )
    {
        return( *( (T*)(
            ( (unsigned char *)( _buffer->getData() ) + _offset
            + _dcp->getIndex( count ) * _step ) ) ) );
    }

    VertexAttribContainer& operator=( const VertexAttribContainer<T>& rhs )
    {
        _buffer = rhs._buffer;
        _vap = rhs._vap;
        _dcp = rhs._dcp;
        return( *this );
    }
    bool operator==( const VertexAttribContainer<T>& rhs ) const
    {
        return( ( _buffer == rhs._buffer ) &&
            ( _vap == rhs._vap ) &&
            ( _dcp == rhs._dcp ) );
    }


protected:
    jagBase::BufferPtr _buffer;
    VertexAttribPtr _vap;
    DrawCommandPtr _dcp;

    size_t _step;
    size_t _offset;
};


// jagDraw
}


// __JAGDRAW_VERTEX_ATTRIB_CONTAINER_H__
#endif
