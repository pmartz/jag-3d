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

#ifndef __JAG_UTIL_SHAPES_H__
#define __JAG_UTIL_SHAPES_H__ 1

#include <jag/util/Export.h>
#include <jag/draw/Drawable.h>
#include <jag/draw/VertexArrayObject.h>
#include <gmtl/gmtl.h>


namespace jag {
namespace util {


///\addtogroup jagUtilShapes Rudimentary Shape Support
///@{

typedef struct {
    gmtl::Point3f _v;
    gmtl::Vec3f _n;
    gmtl::Point2f _tc;
} VertexNormalTexCoordStruct;

typedef std::vector< VertexNormalTexCoordStruct > VNTCVec;

JAGUTIL_EXPORT jag::draw::VertexArrayObjectPtr createVertexArrayObject( const VNTCVec& data );



JAGUTIL_EXPORT jag::draw::DrawablePtr makePlane( VNTCVec& data,
    const gmtl::Point3f& corner, const gmtl::Vec3f& u, const gmtl::Vec3f& v,
    const int subU=1, const int subV=1,
    jag::draw::DrawablePtr drawable=jag::draw::DrawablePtr( ( jag::draw::Drawable* )NULL ) );


///@}


// namespace jag::util::
}
}


// __JAG_UTIL_SHAPES_H__
#endif
