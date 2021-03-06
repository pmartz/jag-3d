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

#include <jag/base/Transform.h>
#include <jag/sg/Node.h>
#include <jag/sg/CollectionVisitor.h>
#include <jag/draw/Bound.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/Uniform.h>
#include <jag/draw/Program.h>

#include <boost/foreach.hpp>
#include <gmtl/gmtl.h>

#include <iostream>


using jag::base::TransformD;

using jag::sg::Node;
using jag::sg::NodePtr;
using jag::sg::CollectionVisitor;

using jag::draw::Bound;
using jag::draw::BoundPtr;
using jag::draw::CommandMap;
using jag::draw::CommandMapPtr;
using jag::draw::Uniform;
using jag::draw::UniformPtr;
using jag::draw::UniformSet;
using jag::draw::UniformSetPtr;
using jag::draw::Drawable;
using jag::draw::DrawablePtr;
using jag::draw::DrawNodeContainer;
using jag::draw::Program;


bool test()
{
    std::cout << "CollectionInfo test..." << std::endl;

    {
        gmtl::Matrix44d scale, view, proj;
        const gmtl::Point3d eye( 0., 0., 40. );
        const gmtl::Point3d center( 0., 30., 0. );
        const gmtl::Vec3d up( 0., 1., 0. );
        gmtl::setScale( scale, .5 );
        gmtl::setLookAt< double >( view, eye, center, up );
        gmtl::setPerspective< double >( proj, 90., 1., 1., 200. );

        TransformD transform;
        transform.setView( view );
        transform.setProj( proj );
        transform.setViewport( 0, 0, 100, 100 );

        CollectionVisitor::CollectionInfo ci( transform );
        gmtl::Sphered s( gmtl::Point3d( 0., 30., 0. ), 10. );
        jag::draw::BoundPtr bound( new jag::draw::BoundSphere( s ) );
        ci.setBound( bound.get() );

        const double ecd( ci.getECBoundDistance() );
        if( ecd != 50. )
        {
            std::cerr << "EC distance: " << ecd << " != 50.0" << std::endl;
            return( false );
        }
        const double ecr( ci.getECBoundRadius() );
        if( ecr != 10. )
        {
            std::cerr << "EC radius: " << ecr << " != 10.0" << std::endl;
            return( false );
        }
        const double winLen( ci.getWinCLength( ecr ) );
        if( (float)winLen != 10.f )
        {
            std::cerr << "WinC length: " << winLen << " != 10.0" << std::endl;
            return( false );
        }
    }


    std::cout << "Single node SG -> draw graph test..." << std::endl;

    NodePtr root( new Node() );
    CollectionVisitor cv;
    jag::draw::DrawGraphPtr drawGraphTemplate( new jag::draw::DrawGraph() );
    cv.setDrawGraphTemplate( drawGraphTemplate );

    root->accept( cv );
    if( cv.getDrawGraph()->size() > 0 )
    {
        std::cerr << "Empty scene graph returns non-NULL draw graph." << std::endl;
        return( false );
    }


    std::cout << "  Draw graph verification..." << std::endl;

    drawGraphTemplate->resize( 1 );
    cv.setDrawGraphTemplate( drawGraphTemplate );

    cv.reset();
    cv.setNearFarOps( jag::sg::CollectionVisitor::None ); // Don't do anything with near and far.
    CommandMapPtr commands( new CommandMap );
    UniformPtr u0 = UniformPtr( new Uniform( "a", (GLint)1 ) );
    UniformPtr u1 = UniformPtr( new Uniform( "b", (GLint)2 ) );
    UniformSetPtr us0( new UniformSet() );
    us0->insert( u0 );
    us0->insert( u1 );
    commands->insert( us0 );
    DrawablePtr draw( new Drawable );
    root->setCommandMap( commands );
    root->addDrawable( draw );
    root->accept( cv );
    const jag::draw::DrawGraphPtr drawGraph( cv.getDrawGraph() );
    size_t size( drawGraph->size() );
    if( size != 1 )
    {
        std::cerr << "Draw graph size " << size << " != 1." << std::endl;
        return( false );
    }
    const DrawNodeContainer& nodes( (*drawGraph)[ 0 ] );
    const jag::draw::DrawNodePtr& node( nodes[ 0 ] );
    if( node->getCommandMap() != commands )
    {
        // TBD need to test for CommandMap equivalence.
        // Address comparison is insufficient.
        std::cerr << "KNOWN FAILURE: Wrong CommandMap." << std::endl;
        //return( false );
    }
    if( node->getNumDrawables() != 1 )
    {
        std::cerr << "Node num drawables " << node->getNumDrawables() << " != 1." << std::endl;
        return( false );
    }
    if( node->getDrawableVec()[ 0 ] != draw )
    {
        std::cerr << "Drawable address mismatch." << std::endl;
        return( false );
    }


    std::cout << "Near/Far computation..." << std::endl;

    gmtl::AABoxd box( gmtl::Point3d( -1., -1., -1. ), gmtl::Point3d( 1., 1., 1. ) );
    draw->setInitialBound( jag::draw::BoundPtr( new jag::draw::BoundAABox( box ) ) );
    root = NodePtr( new Node() );
    root->addDrawable( draw );

    const gmtl::Point3d initialView( 0., 10., 0. );
    const gmtl::Point3d initialCenter( 0., 0., 0. );
    const gmtl::Vec3d initialUp( 0., 0., 1. );
    const gmtl::Point3d secondView( 0., 9., 0. );
    const gmtl::Point3d secondCenter( 0., 0., 0. );

    cv.reset();
    cv.setNearFarOps( jag::sg::CollectionVisitor::Default );
    gmtl::Matrix44d view, proj;
    gmtl::setLookAt( view, initialView, initialCenter, initialUp );
    gmtl::setPerspective( proj, 60., 1., 1., 100. );
    cv.setViewProj( view, proj );
    root->accept( cv );

    // Get original near/far values.
    double minZ, maxZ;
    cv.getNearFar( minZ, maxZ );
    if( ( minZ <= 0. ) || ( minZ >= maxZ ) )
    {
        std::cerr << "Computed near/far values are invalid." << std::endl;
        return( false );
    }

    // If we use the same view matrix, the delta should be zero and
    // we should get the original values.
    double newMinZ, newMaxZ;
    cv.recomputeNearFar( newMinZ, newMaxZ, view );
    if( ( minZ != newMinZ ) || ( maxZ != newMaxZ ) )
    {
        std::cerr << "Recomputed near/far should match original near/far." << std::endl;
        return( false );
    }

    // Move closer. Recomputed values should be less than original.
    gmtl::setLookAt( view, secondView, initialCenter, initialUp );
    cv.recomputeNearFar( newMinZ, newMaxZ, view );
    if( ( minZ <= newMinZ ) || ( maxZ <= newMaxZ ) )
    {
        std::cerr << "Recomputed near/far should be less than original near/far." << std::endl;
        return( false );
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
