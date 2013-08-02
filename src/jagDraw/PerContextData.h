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

#ifndef __JAGDRAW_PER_CONTEXT_DATA_H__
#define __JAGDRAW_PER_CONTEXT_DATA_H__ 1


#include <vector>


namespace jagDraw {


/** \addtogroup OpenGLAbstraction Support for OpenGL Abstract Concepts */
/*@{*/

/** \struct PerContextData PerContextData.h <jagDraw/PerContextData.h>
*/
template< typename T >
struct PerContextData
{
    PerContextData()
    {
    }
    ~PerContextData()
    {
    }

    PerContextData& operator=( const PerContextData& rhs )
    {
        _data = rhs._data;
        return( *this );
    }

    T& operator[]( const unsigned int idx )
    {
        return( ( T& )( _data[ idx ] ) );
    }
    const T& operator[]( const unsigned int idx ) const
    {
        return( ( T& )( _data[ idx ] ) );
    }

    std::vector< T > _data;
};

/*@}*/


// jagDraw
}


// __JAGDRAW_PER_CONTEXT_DATA_H__
#endif
