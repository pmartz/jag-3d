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
#include <jagSG/FrustumCullCallback.h>
#include <jagDraw/Node.h>
#include <jagDraw/Uniform.h>
#include <jagDraw/Program.h>
#include <jagUtil/Shapes.h>

#include <boost/foreach.hpp>
#include <gmtl/gmtl.h>

#include <iostream>


using jagSG::Node;
using jagSG::NodePtr;

using jagDraw::Bound;
using jagDraw::BoundAABox;
using jagDraw::BoundSphere;
using jagDraw::BoundPtr;
using jagDraw::CommandMap;
using jagDraw::CommandMapPtr;
using jagDraw::Drawable;
using jagDraw::DrawablePtr;


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


bool test()
{
    std::cout << "Testing: Expand NULL sphere around sphere..." << std::endl;
    {
        const gmtl::Point3d center( 10., -5., -2. );

        BoundSphere sphereA;
        sphereA.setCenter( center );
        sphereA.setEmpty( false );

        const double radius( 10. );
        gmtl::Sphered inputSphere;
        inputSphere.setCenter( center );
        inputSphere.setRadius( radius );
        inputSphere.setInitialized( true );
        BoundSphere sphereB( inputSphere );

        sphereA.expand( sphereB );
        if( sphereA.getRadius() != radius )
        {
            std::cerr << "Incorrect sphere radius. " << sphereA.getRadius() << " != " << radius << std::endl;
            return( false );
        }
    }

    {
        NodePtr root( NodePtr( new jagSG::Node() ) );

        CommandMapPtr commands( CommandMapPtr( new CommandMap() ) );
        root->setCommandMap( commands );

        std::cout << "Testing: Obtain bound from quad..." << std::endl;
        {
            NodePtr nodeA( NodePtr( new Node() ) );
            jagUtil::VNTCVec data;
            DrawablePtr quad( createQuad( data, gmtl::Point3f( 0., 0., 0. ), 10.f ) );
            nodeA->addDrawable( quad );
            root->addChild( nodeA );
            commands->insert( jagUtil::createVertexArrayObject( data ) );

            BoundPtr bound( root->getBound() );
            double returnRadius( bound->getRadius() );
            if( ( returnRadius < 7.05 ) || ( returnRadius > 7.1 ) )
            {
                std::cerr << "Incorrect radius: " << returnRadius << std::endl;
                return( false );
            }
            if( root->getAnyBoundDirty() )
            {
                std::cerr << "Failed bound clean test." << std::endl;
                return( false );
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
