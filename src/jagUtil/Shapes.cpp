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
#include <jag/draw/VertexAttrib.h>
#include <jag/base/LogMacros.h>
#include <gmtl/gmtl.h>


namespace jagUtil
{


jag::draw::VertexArrayObjectPtr createVertexArrayObject( const VNTCVec& data )
{
    jag::base::BufferPtr ibp( new jag::base::Buffer( data.size() * sizeof( jagUtil::VertexNormalTexCoordStruct ), (void*)&data[0] ) );
    jag::draw::BufferObjectPtr ibop( new jag::draw::BufferObject( GL_ARRAY_BUFFER, ibp ) );

    const GLsizei stride( sizeof( jagUtil::VertexNormalTexCoordStruct ) );
    jag::draw::VertexAttribPtr vertAttrib( new jag::draw::VertexAttrib(
        "vertex", 3, GL_FLOAT, GL_FALSE, stride, 0 ) );
    jag::draw::VertexAttribPtr normAttrib( new jag::draw::VertexAttrib(
        "normal", 3, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 3 ) );
    jag::draw::VertexAttribPtr tcAttrib( new jag::draw::VertexAttrib(
        "texcoord", 2, GL_FLOAT, GL_FALSE, stride, sizeof( GLfloat ) * 6 ) );

    jag::draw::VertexArrayObjectPtr vaop( new jag::draw::VertexArrayObject );
    // Bind the GL_ARRAY_BUFFER for interleaved vertices, normals, and texcoords
    vaop->addVertexArrayCommand( ibop, jag::draw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( vertAttrib, jag::draw::VertexArrayObject::Vertex );
    vaop->addVertexArrayCommand( normAttrib, jag::draw::VertexArrayObject::Normal );
    vaop->addVertexArrayCommand( tcAttrib, jag::draw::VertexArrayObject::TexCoord );

    return( vaop );
} 


static void addPlaneData( VNTCVec& data,
    const gmtl::Point3f& corner,
    const gmtl::Vec3f& u, unsigned int uSteps,
    const gmtl::Vec3f& v, unsigned int vSteps,
    const gmtl::Vec3f& normal, jag::draw::Drawable* geom )
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

    jag::draw::DrawElementsPtr de( jag::draw::DrawElementsPtr( new jag::draw::DrawElements
        ( GL_TRIANGLES, count, GL_UNSIGNED_INT, 0,
        jag::draw::BufferObjectPtr( new jag::draw::ElementArrayBuffer(
            jag::base::BufferPtr( new jag::base::Buffer( indices.size()*sizeof(unsigned int), &( indices[ 0 ] ) )))))));
    geom->addDrawCommand( de );
}

static bool buildPlaneData( VNTCVec& data,
    const gmtl::Point3f& corner, const gmtl::Vec3f& u, const gmtl::Vec3f& v,
    const int subU, const int subV, jag::draw::Drawable* drawable )
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

jag::draw::DrawablePtr makePlane( VNTCVec& data,
    const gmtl::Point3f& corner, const gmtl::Vec3f& u, const gmtl::Vec3f& v,
    const int subU, const int subV, jag::draw::DrawablePtr drawable )
{
    jag::draw::DrawablePtr draw( drawable );
    if( draw == NULL )
        draw = jag::draw::DrawablePtr( new jag::draw::Drawable() );

    if( buildPlaneData( data, corner, u, v, subU, subV, draw.get() ) )
        return( draw );

    JAG3D_ERROR_STATIC( "jag.util.plane", "makePlane: Error." );
    return( jag::draw::DrawablePtr( ( jag::draw::Drawable* ) NULL ) );
}


// jagUtil
}
