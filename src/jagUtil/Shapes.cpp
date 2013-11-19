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
    const gmtl::Vec3f& u, unsigned int uSteps,
    const gmtl::Vec3f& v, unsigned int vSteps,
    const gmtl::Vec3f& normal, jagDraw::Drawable* geom )
{
    std::vector< unsigned int > indices;

    unsigned int uIdx, vIdx;
    unsigned int count( 0 );
    for( vIdx=0; vIdx<=vSteps; vIdx++ )
    {
        const float vPct( (float)vIdx / (float)vSteps );
        const gmtl::Vec3f vVec( v * vPct );

        int startIdx( (int)( data.size() ) ), idx( 0 );
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
                const int a( startIdx + idx );
                const int b( startIdx + idx + uSteps + 2 );

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
        ( GL_TRIANGLES, count, GL_UNSIGNED_INT, 0,
        jagDraw::BufferObjectPtr( new jagDraw::ElementArrayBuffer(
            jagBase::BufferPtr( new jagBase::Buffer( indices.size()*sizeof(unsigned int), &( indices[ 0 ] ) )))))));
    geom->addDrawCommand( de );
}

static bool buildPlaneData( VNTCVec& data,
    const gmtl::Point3f& corner, const gmtl::Vec3f& u, const gmtl::Vec3f& v,
    const int subU, const int subV, jagDraw::Drawable* drawable )
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
    const int subU, const int subV, jagDraw::DrawablePtr drawable )
{
    jagDraw::DrawablePtr draw( drawable );
    if( draw == NULL )
        draw = jagDraw::DrawablePtr( new jagDraw::Drawable() );

    if( buildPlaneData( data, corner, u, v, subU, subV, draw.get() ) )
        return( draw );

    JAG3D_ERROR_STATIC( "jag.util.plane", "makePlane: Error." );
    return( jagDraw::DrawablePtr( ( jagDraw::Drawable* ) NULL ) );
}


// jagUtil
}
