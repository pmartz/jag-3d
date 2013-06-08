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

#include <jagUtil/Shapes.h>
#include <jagBase/LogMacros.h>
#include <gmtl/gmtl.h>


namespace jagUtil
{


static void addPlaneData( VNTCVec& data,
    const gmtl::Point3f& corner,
    const gmtl::Vec3f& u, unsigned short uSteps,
    const gmtl::Vec3f& v, unsigned short vSteps,
    const gmtl::Vec3f& normal, jagDraw::Drawable* geom )
{
    std::vector< short > indices;

    unsigned short uIdx, vIdx;
    for( vIdx=0; vIdx<=vSteps; vIdx++ )
    {
        const float vPct( (float)vIdx / (float)vSteps );
        const gmtl::Vec3f vVec( v * vPct );

        short startIdx( (short)( data.size() ) ), idx( 0 );
        for( uIdx=0; uIdx<=uSteps; uIdx++ )
        {
            VertexNormalTexCoordStruct vntc;

            const float uPct( (float)uIdx / (float)uSteps );
            vntc._v = corner + vVec + (u * uPct);
            vntc._n = normal;
            vntc._tc = gmtl::Vec2f( uPct, vPct );
            data.push_back( vntc );

            indices.push_back( startIdx + idx + uSteps + 1 );
            indices.push_back( startIdx + idx );
            idx++;
        }
        jagDraw::DrawElementsPtr de( jagDraw::DrawElementsPtr( new jagDraw::DrawElements
            ( GL_TRIANGLE_STRIP, idx-startIdx, GL_UNSIGNED_SHORT, 0,
            jagDraw::BufferObjectPtr( new jagDraw::ElementArrayBuffer(
                jagBase::BufferPtr( new jagBase::Buffer( indices.size()*sizeof(unsigned short), &( indices[ 0 ] ) )))))));
        geom->addDrawCommand( de );
    }
}

static bool buildPlaneData( VNTCVec& data,
    const gmtl::Point3f& corner, const gmtl::Vec3f& u, const gmtl::Vec3f& v,
    const short subU, const short subV, jagDraw::Drawable* drawable )
{
    if( ( subU <= 0 ) || ( subV <= 0 ) )
    {
        JAG3D_ERROR_STATIC( "jag.util.plane", "makePlane: Invalid subdivisions." );
        return( false );
    }

    gmtl::Vec3f normal( u ^ v );
    gmtl::normalize( normal );
    addPlaneData( data, corner, u, subU, v, subV, normal, drawable );

    return( true );
}

jagDraw::DrawablePtr makePlane( VNTCVec& data,
    const gmtl::Point3f& corner, const gmtl::Vec3f& u, const gmtl::Vec3f& v,
    const short subU, const short subV, jagDraw::DrawablePtr drawable )
{
    jagDraw::DrawablePtr draw( drawable );
    if( draw == NULL )
        draw = jagDraw::DrawablePtr( new jagDraw::Drawable() );

    if( buildPlaneData( data, corner, u, v, subU, subV, draw.get() ) )
        return( draw );

    JAG3D_ERROR_STATIC( "jag.util.plane", "makePlane: Error." );
    return( jagDraw::DrawablePtr( NULL ) );
}


// jagUtil
}
