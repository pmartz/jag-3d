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

#ifndef __JAGDRAW_VERTEX_ATTRIB_ITERATOR_H__
#define __JAGDRAW_VERTEX_ATTRIB_ITERATOR_H__ 1


#include <jagBase/Buffer.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/VertexAttrib.h>
#include <jagDraw/DrawCommand.h>

#include <iterator>


namespace jagDraw {


/** VertexAttribContainer iterator.h <jagDraw/iterator.h>
\brief TBD
\details TBD */
template< typename T >
class VertexAttribContainer
{
public:
    VertexAttribContainer( const BufferObjectPtr bop, const VertexAttribPtr vap, const DrawCommandPtr dcp )
      : _buffer( bop->getBuffer() ),
        _vap( vap ),
        _dcp( dcp ),
        _step( computeStep() )
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
        iterator( VertexAttribContainer<T>& vaCont )
          : _pos( NULL ),
            _counter( 0 ),
            _vaCont( vaCont )
        {
        }
        iterator( const iterator& rhs )
          : _pos( rhs._pos ),
            _counter( rhs._counter ),
            _vaCont( rhs._vaCont )
        {
        }
        ~iterator()
        {
        }

        T operator*() const
        {
            return( *_pos );
        }

        T& operator++() // prefix inc
        {
            int byteDistance( _vaCont.deltaByteCount( _counter++, _counter ) );
            _pos = (T*)( (unsigned char*)( _pos ) + byteDistance );
            return( *_pos );
        }
        T operator++( int ) // postfix inc
        {
            T result( *this );
            ++( *this );
            return( result );
        }
        T& operator--() // prefix dec
        {
            int byteDistance( _vaCont.deltaByteCount( _counter--, _counter ) );
            _pos = (T*)( (unsigned char*)( _pos ) + byteDistance );
            return( *_pos );
        }
        T operator--( int ) // postfix dec
        {
            T result( *this );
            --( *this );
            return( result );
        }

        bool operator==( const iterator& rhs ) const
        {
            return( ( _counter == rhs._counter ) &&
                ( _pos == rhs._pos ) );
        }
        bool operator!=( const iterator& rhs ) const
        {
            return( !( *this == rhs ) );
        }

        iterator& operator=( const iterator& rhs )
        {
            _pos = rhs._pos;
            _counter = rhs._counter;
            return( *this );
        }

    protected:
        iterator( VertexAttribContainer<T>& vaCont, T* pos, unsigned int counter )
          : _pos( pos ),
            _counter( counter ),
            _vaCont( vaCont )
        {
        }

        T* _pos;
        unsigned int _counter;

        VertexAttribContainer<T>& _vaCont;
    };



    int deltaByteCount( const unsigned int from, const unsigned int to ) const
    {
        const int indexFrom( _dcp->getIndex( from ) );
        const int indexTo( _dcp->getIndex( to ) );
        return( ( indexTo - indexFrom ) * (int)_step );
    }

    iterator begin()
    {
        iterator result( *this,
            (T*)( (unsigned char *)( _buffer->ptr() )
                + _offset + _dcp->getIndex( 0 ) * _step ),
            0 );
        return( result );
    }

    iterator end()
    {
        iterator result( *this,
            (T*)( (unsigned char *)( _buffer->ptr() )
                + _offset + _dcp->getIndex( _dcp->getNumIndices() ) * _step ),
            _dcp->getNumIndices() );
        return( result );
    }


protected:
    size_t computeStep()
    {
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        GLuint offset;
        _vap->getAll( size, type, normalized, stride, offset );
        _offset = (size_t) offset;

        if( stride != 0 )
        {
            return( stride );
        }

        unsigned int bytesPerElement( 0 );
        switch( type )
        {
        case GL_UNSIGNED_BYTE: bytesPerElement = sizeof( GLbyte ); break;
        case GL_FLOAT: bytesPerElement = sizeof( GLfloat ); break;
        case GL_DOUBLE: bytesPerElement = sizeof( GLdouble ); break;
        default:
            // TBD;
            bytesPerElement = 1;
            break;
        }
        return( size * bytesPerElement );
    }


    const jagBase::BufferPtr _buffer;
    const VertexAttribPtr _vap;
    const DrawCommandPtr _dcp;

    size_t _step;

    size_t _offset;
};


// jagDraw
}


// __JAGDRAW_VERTEX_ATTRIB_ITERATOR_H__
#endif
