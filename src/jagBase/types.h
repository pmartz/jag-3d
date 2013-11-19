/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
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

#ifndef __JAGBASE_TYPES_H__
#define __JAGBASE_TYPES_H__ 1


#include <jagBase/ptr.h>
#include <vector>
#include <string>
#include <gmtl/gmtl.h>


namespace jagBase {


#define DEFINE_TYPE_ARRAYS(__type,__name) \
    typedef std::vector< __type > __name##Vec; \
    typedef jagBase::ptr< __type >::shared_array_ptr __name##Array;

DEFINE_TYPE_ARRAYS(gmtl::Point2f,Point2f);
DEFINE_TYPE_ARRAYS(gmtl::Point3f,Point3f);
DEFINE_TYPE_ARRAYS(gmtl::Matrix44d,Matrix44d);
// WARNING: BoolVen and BoolArray are identical to UCharVec and
// UCharArray. This is so that T& operator[]() semantics will work
// (they would fail in, e.g., std::vector<bool>).
DEFINE_TYPE_ARRAYS(unsigned char,Bool);
DEFINE_TYPE_ARRAYS(unsigned char,UChar);
DEFINE_TYPE_ARRAYS(unsigned int,UIhar);
DEFINE_TYPE_ARRAYS(float,Float);
DEFINE_TYPE_ARRAYS(double,Double);
DEFINE_TYPE_ARRAYS(std::string,String);

#undef DEFINE_TYPE_ARRAYS


// jagBase
}


// __JAGBASE_TYPES_H__
#endif
