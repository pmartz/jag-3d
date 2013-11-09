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

#ifndef __JAGUTIL_SHAPES_H__
#define __JAGUTIL_SHAPES_H__ 1

#include <jagUtil/Export.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/VertexArrayObject.h>
#include <gmtl/gmtl.h>


namespace jagUtil
{


/** \addtogroup jagUtilShapes Rudimentary Shape Support
/*@{*/

typedef struct {
    gmtl::Point3f _v;
    gmtl::Vec3f _n;
    gmtl::Point2f _tc;
} VertexNormalTexCoordStruct;

typedef std::vector< VertexNormalTexCoordStruct > VNTCVec;

JAGUTIL_EXPORT jagDraw::VertexArrayObjectPtr createVertexArrayObject( const VNTCVec& data );



JAGUTIL_EXPORT jagDraw::DrawablePtr makePlane( VNTCVec& data,
    const gmtl::Point3f& corner, const gmtl::Vec3f& u, const gmtl::Vec3f& v,
    const int subU=1, const int subV=1,
    jagDraw::DrawablePtr drawable=jagDraw::DrawablePtr( ( jagDraw::Drawable* )NULL ) );


/*@}*/


// jagUtil
}


// __JAGUTIL_SHAPES_H__
#endif
