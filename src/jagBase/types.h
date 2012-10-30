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

#ifndef __JAGBASE_TYPES_H__
#define __JAGBASE_TYPES_H__ 1


#include <jagBase/ptr.h>
#include <vector>
#include <deque>
#include <string>
#include <gmtl/gmtl.h>


namespace jagBase {


typedef jagBase::ptr< unsigned char >::shared_array_ptr UCharArray;
typedef jagBase::ptr< unsigned int >::shared_ptr UIntPtr;

typedef std::vector< std::string > StringVec;

typedef std::vector< gmtl::Point3f > Point3fVec;
typedef jagBase::ptr< gmtl::Point3f >::shared_array_ptr Point3fArray;

typedef std::deque< gmtl::Matrix44d > Matrix44dDeque;

// jagBase
}


// __JAGBASE_TYPES_H__
#endif
