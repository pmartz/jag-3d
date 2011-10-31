/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag-3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
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

#ifndef __JAGBASE_PLATFORM_POINTER_H__
#define __JAGBASE_PLATFORM_POINTER_H__ 1


#ifdef JAG3D_USE_BOOST_POINTERS
#  include <boost/smart_ptr/shared_ptr.hpp>
#  include <boost/smart_ptr/shared_array.hpp>
#else
#  ifdef WIN32
#    include <memory>
#  else
#    include <tr1/memory>
#  endif
#endif


namespace jagBase {


/** \class ptr PlatformPtr.h <jagBase/PlatformPtr.h>
\brief Defines the \c ptr template struct, supporting either Boost or TR1.
*/
template <typename T>
struct ptr
{
#ifdef JAG3D_USE_BOOST_POINTERS
    typedef boost::shared_ptr< T > shared_ptr;
    typedef boost::shared_array< T > shared_array_ptr;
#else
    typedef std::tr1::shared_ptr< T > shared_ptr;
    struct shared_array_ptr : public shared_ptr
    { 
        shared_array_ptr() : shared_ptr()
        {}
        shared_array_ptr( T *t )
          : shared_ptr( t, ptr< T >::__array_deleter() )
        {}
    };

    struct __array_deleter
    {
        void operator()( T *t )
        {
            delete[] t;
        }
    };
#endif
};


// jagBase
}


// __JAGBASE_PLATFORM_POINTER_H__
#endif
