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

#include <jagDraw/BufferObject.h>
#include <jagDraw/DrawCommand.h>
#include <jagDraw/VertexAttribContainer.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagUtil/Shapes.h>

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <iostream>


using jagDraw::VertexArrayCommand;
using jagDraw::BufferObject;
using jagDraw::BufferObjectPtr;
using jagDraw::VertexAttrib;
using jagDraw::VertexAttribPtr;
using jagDraw::VertexAttribContainer;
using jagDraw::VertexArrayObject;
using jagDraw::VertexArrayObjectPtr;
using jagDraw::DrawCommand;
using jagDraw::DrawCommandPtr;
using jagDraw::Drawable;
using jagDraw::DrawablePtr;
using jagDraw::DrawArrays;
using jagDraw::DrawElements;


DrawablePtr createQuad( jagUtil::VNTCVec& data, const gmtl::Point3f& center, const float extent )
{
    // Create the quad and its data
    const gmtl::Point3f corner( center -
        gmtl::Point3f( extent/2.f, extent/2.f, 0.f ) );
    const gmtl::Vec3f uVec( extent, 0.f, 0.f );
    const gmtl::Vec3f vVec( 0.f, extent, 0.f );
    jagDraw::DrawablePtr quad( jagUtil::makePlane(
        data, corner, uVec, vVec ) );

    return( quad );
}

bool validate( BufferObjectPtr& bop, VertexAttribPtr& verts, const VertexArrayObjectPtr& vaop )
{
    if( vaop == NULL )
    {
        std::cout << "Null VertexArrayObject." << std::endl;
        return( false );
    }

    bop = ( boost::dynamic_pointer_cast< BufferObject >(
        vaop->getVertexArrayCommand( VertexArrayCommand::BufferObject_t, VertexArrayObject::Vertex ) ) );
    verts = ( boost::dynamic_pointer_cast< VertexAttrib >(
        vaop->getVertexArrayCommand( VertexArrayCommand::VertexAttrib_t, VertexArrayObject::Vertex ) ) );
    if( ( bop == NULL ) || ( verts == NULL ) )
    {
        std::cout << "NULL BufferObject or VertexAttrib." << std::endl;
        return( false );
    }
    GLint size;
    GLenum type;
    verts->getSizeType( size, type );
    if( ( size != 3 ) || ( type != GL_FLOAT ) )
    {
        std::cout << "Bad VertexAttrib size or type." << std::endl;
        return( false );
    }

    return( true );
}

bool test()
{
    std::cout << "Testing: Iterate over simple quad..." << std::endl;
    {
        jagUtil::VNTCVec data;
        DrawablePtr quad( createQuad( data, gmtl::Point3f( 0., 0., 0. ), 10.f ) );
        VertexArrayObjectPtr vaop( jagUtil::createVertexArrayObject( data ) );

        BufferObjectPtr bop;
        VertexAttribPtr verts;
        if( !( validate( bop, verts, vaop ) ) )
            return( false );

        BOOST_FOREACH( const DrawCommandPtr dcp, quad->getDrawCommandVec() )
        {
            if( dcp->getCount() != 6 )
            {
                std::cout << "DrawCommand with count " << dcp->getCount() << " expected 6." << std::endl;
                return( false );
            }
            gmtl::Point3f expectedVerts[] = {
                gmtl::Point3f( -5., 5., 0. ),
                gmtl::Point3f( -5., -5., 0. ),
                gmtl::Point3f( 5., 5., 0. ),
                gmtl::Point3f( 5., 5., 0. ),
                gmtl::Point3f( -5., -5., 0. ),
                gmtl::Point3f( 5., -5., 0. )
            };
            VertexAttribContainer< gmtl::Point3f > vac( bop, verts, dcp );
            VertexAttribContainer< gmtl::Point3f >::iterator pointIter;
            unsigned int count( 0 );
            for( pointIter = vac.begin(); pointIter != vac.end(); ++pointIter )
            {
                if( count >= dcp->getCount() )
                {
                    std::cout << "Exceeded array length." << std::endl;
                    return( false );
                }
                if( *pointIter != expectedVerts[ count ] )
                {
                    std::cout << "Vertex " << count << ", " << *pointIter << " mismatch." << std::endl;
                    return( false );
                }
                ++count;
            }
        }
    }

    return( true );
}

int main( int argc, char** argv )
{
    std::cout << "This is a CTest regression test. To launch under Visual Studio, build the" << std::endl;
    std::cout << "RUN_TESTS target. Under Linux, enter 'make test' at a shell prompt." << std::endl;


    std::cout << std::endl;
    if( test() )
    {
        std::cout << "Test passed." << std::endl;
        return( 0 );
    }
    else
    {
        std::cout << "Test failed." << std::endl;
        return( 1 );
    }
}
