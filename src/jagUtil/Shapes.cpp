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
#include <jagDraw/VertexAttrib.h>
#include <jagBase/LogMacros.h>
#include <gmtl/gmtl.h>


namespace jagUtil
{


jagDraw::VertexArrayObjectPtr createVertexArrayObject( const VNTCVec& data )
{
    jagBase::BufferPtr ibp( new jagBase::Buffer( data.size() * sizeof( jagUtil::VertexNormalTexCoordStruct ), (void*)&data[0] ) );
    jagDraw::BufferObjectPtr ibop( new jagDraw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

    const GLsizei stride( sizeof( jagUtil::VertexNormalTexCoordStruct ) );
    jagDraw::VertexAttribPtr vertAttrib( new jagDraw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
    jagDraw::VertexAttribPtr normAttrib( new jagDraw::VertexAttrib(
        "normal", 3, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );
    jagDraw::VertexAttribPtr tcAttrib( new jagDraw::VertexAttrib(
        "texcoord", 2, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 6 ) );

    jagDraw::VertexArrayObjectPtr vaop( new jagDraw::VertexArrayObject );
    // Bind the GL_ARRAY_BUFFER for interleaved vertices, normals, and texcoords
    vaop->addVertexArrayCommand( ibop, jagDraw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( vertAttrib, jagDraw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( normAttrib, jagDraw::VertexArrayObject::Normal );
    vaop->addVertexArrayCommand( tcAttrib, jagDraw::VertexArrayObject::TexCoord );

    return( vaop );
}


static void addPlaneData( VNTCVec& data,
    const gmtl::Point3f& corner,
    const gmtl::Vec3f& u, unsigned short uSteps,
    const gmtl::Vec3f& v, unsigned short vSteps,
    const gmtl::Vec3f& normal, jagDraw::Drawable* geom )
{
    std::vector< short > indices;

    unsigned short uIdx, vIdx;
    unsigned short count( 0 );
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

            if( ( vIdx < vSteps ) && ( uIdx < uSteps ) )
            {
                const short a( startIdx + idx );
                const short b( startIdx + idx + uSteps + 2 );

                indices.push_back( startIdx + idx + uSteps + 1 );
                indices.push_back( a );
                indices.push_back( b );

                indices.push_back( b );
                indices.push_back( a );
                indices.push_back( startIdx + idx + 1 );

                count += 6;
            }
            idx++;
        }
    }

    jagDraw::DrawElementsPtr de( jagDraw::DrawElementsPtr( new jagDraw::DrawElements
        ( GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0,
        jagDraw::BufferObjectPtr( new jagDraw::ElementArrayBuffer(
            jagBase::BufferPtr( new jagBase::Buffer( indices.size()*sizeof(unsigned short), &( indices[ 0 ] ) )))))));
    geom->addDrawCommand( de );
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
