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


#include <jagDraw/PlatformOpenGL.h>
#include <vector>


namespace jagDraw {


/** \struct PerContextData PerContextData.h <jagDraw/PerContextData.h>
*/
template< class T >
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

    T& operator[]( unsigned int idx )
    {
        return( _data[ idx ] );
    }
    const T& operator[]( unsigned int idx ) const
    {
        return( _data[ idx ] );
    }

    std::vector< T > _data;
};

typedef PerContextData< GLuint > PerContextGLuint;


// jagDraw
}


// __JAGDRAW_PER_CONTEXT_DATA_H__
#endif
