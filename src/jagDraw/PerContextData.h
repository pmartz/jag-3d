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
