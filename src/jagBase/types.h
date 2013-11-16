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
DEFINE_TYPE_ARRAYS(bool,Bool);
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
