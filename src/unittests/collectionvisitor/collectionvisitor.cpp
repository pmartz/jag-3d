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

#include <jagBase/Transform.h>
#include <jagSG/Node.h>
#include <jagSG/CollectionVisitor.h>
#include <jagDraw/Bound.h>
#include <jagDraw/Node.h>
#include <jagDraw/Uniform.h>
#include <jagDraw/Program.h>

#include <boost/foreach.hpp>
#include <gmtl/gmtl.h>

#include <iostream>


using jagBase::TransformD;

using jagSG::Node;
using jagSG::NodePtr;
using jagSG::CollectionVisitor;

using jagDraw::Bound;
using jagDraw::BoundPtr;
using jagDraw::CommandMap;
using jagDraw::CommandMapPtr;
using jagDraw::Uniform;
using jagDraw::UniformPtr;
using jagDraw::UniformSet;
using jagDraw::UniformSetPtr;
using jagDraw::Drawable;
using jagDraw::DrawablePtr;
using jagDraw::NodeContainer;
using jagDraw::Program;


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
        jagDraw::BoundPtr bound( new jagDraw::BoundSphere( s ) );
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
    jagDraw::DrawGraphPtr drawGraphTemplate( new jagDraw::DrawGraph() );
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
    cv.setNearFarOps( jagSG::CollectionVisitor::None ); // Don't do anything with near and far.
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
    const jagDraw::DrawGraphPtr drawGraph( cv.getDrawGraph() );
    size_t size( drawGraph->size() );
    if( size != 1 )
    {
        std::cerr << "Draw graph size " << size << " != 1." << std::endl;
        return( false );
    }
    const NodeContainer& nodes( (*drawGraph)[ 0 ] );
    const jagDraw::DrawNodePtr& node( nodes[ 0 ] );
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
    draw->setInitialBound( jagDraw::BoundPtr( new jagDraw::BoundAABox( box ) ) );
    root = NodePtr( new Node() );
    root->addDrawable( draw );

    const gmtl::Point3d initialView( 0., 10., 0. );
    const gmtl::Point3d initialCenter( 0., 0., 0. );
    const gmtl::Vec3d initialUp( 0., 0., 1. );
    const gmtl::Point3d secondView( 0., 9., 0. );
    const gmtl::Point3d secondCenter( 0., 0., 0. );

    cv.reset();
    cv.setNearFarOps( jagSG::CollectionVisitor::Default );
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
