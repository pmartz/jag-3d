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

#ifndef __JAGDRAW_VERTEX_ATTRIB_CONTAINER_H__
#define __JAGDRAW_VERTEX_ATTRIB_CONTAINER_H__ 1


#include <jagBase/Buffer.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/VertexAttrib.h>
#include <jagDraw/DrawCommand.h>

#include <iterator>
#include <iostream>


namespace jagDraw {


/** \addtogroup jagDrawVertexData Vertex Data and Vertex Attributes */
/*@{*/

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


/*@}*/


// jagDraw
}


// __JAGDRAW_VERTEX_ATTRIB_CONTAINER_H__
#endif
