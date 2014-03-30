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

#ifndef __JAG_DRAW_PER_CONTEXT_DATA_H__
#define __JAG_DRAW_PER_CONTEXT_DATA_H__ 1


#include <vector>


namespace jag {
namespace draw {


/** \addtogroup OpenGLAbstraction Support for OpenGL Abstract Concepts */
/*@{*/

/** \struct PerContextData PerContextData.h <jag/draw/PerContextData.h>
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
        return( ( const T& )( _data[ idx ] ) );
    }

    std::vector< T > _data;
};

/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_PER_CONTEXT_DATA_H__
#endif
