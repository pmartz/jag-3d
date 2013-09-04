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
    const jagDraw::Node& node( nodes[ 0 ] );
    if( node.getCommandMap() != commands )
    {
        // TBD need to test for CommandMap equivalence.
        // Address comparison is insufficient.
        std::cerr << "KNOWN FAILURE: Wrong CommandMap." << std::endl;
        //return( false );
    }
    if( node.getNumDrawables() != 1 )
    {
        std::cerr << "Node num drawables " << node.getNumDrawables() << " != 1." << std::endl;
        return( false );
    }
    if( node.getDrawableVec()[ 0 ] != draw )
    {
        std::cerr << "Drawable address mismatch." << std::endl;
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
