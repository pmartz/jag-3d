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

#ifndef __JAG_DRAW_VERTEX_ATTRIB_CONTAINER_H__
#define __JAG_DRAW_VERTEX_ATTRIB_CONTAINER_H__ 1


#include <jag/base/Buffer.h>
#include <jag/draw/BufferObject.h>
#include <jag/draw/VertexAttrib.h>
#include <jag/draw/DrawCommand.h>

#include <iterator>
#include <iostream>


namespace jag {
namespace draw {


/** \addtogroup jagDrawVertexData Vertex Data and Vertex Attributes */
/*@{*/

/** VertexAttribContainer VertexAttribContainer.h <jag/draw/VertexAttribContainer.h>
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



    /** iterator iterator.h <jag/draw/iterator.h>
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
    jag::base::BufferPtr _buffer;
    VertexAttribPtr _vap;
    DrawCommandPtr _dcp;

    size_t _step;
    size_t _offset;
};


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_VERTEX_ATTRIB_CONTAINER_H__
#endif
