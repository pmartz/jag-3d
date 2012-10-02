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

#ifndef __JAGBASE_PTR_H__
#define __JAGBASE_PTR_H__ 1


#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/shared_array.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>


namespace jagBase {


#define SHARED_FROM_THIS(_t) boost::enable_shared_from_this<_t>

    
/** \class ptr ptr.h <jagBase/ptr.h>
\brief Defines the \c ptr template struct, supporting either Boost or TR1.
*/
template <typename T>
struct ptr
{
    typedef boost::shared_ptr< T > shared_ptr;
    typedef boost::shared_array< T > shared_array_ptr;
};


// jagBase
}


// __JAGBASE_PTR_H__
#endif
