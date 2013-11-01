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

#ifndef __JAGUTIL_SHAPES_H__
#define __JAGUTIL_SHAPES_H__ 1

#include <jagUtil/Export.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/VertexArrayObject.h>
#include <gmtl/gmtl.h>


namespace jagUtil
{


/** \addtogroup jagUtilShapes Rudimentary shape support
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
