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

#include <jagDraw/Bound.h>
#include <jagSG/Node.h>
#include <jagDraw/DrawCommand.h>
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
using jagDraw::DrawArrays;


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
        // Expand a zero-radius sphere by a sphere with radius 10.
        // The result should be a sphere with radius 10.

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
            std::cerr << "\tThis failues indicates that GGT/GMTL might not be up-to-date, as" << std::endl;
            std::cerr << "\ta patch was submitted to fix this issue in gmtl/Containment.h." << std::endl;
            return( false );
        }
    }

    {
        NodePtr root( NodePtr( new jagSG::Node() ) );

        CommandMapPtr commands( CommandMapPtr( new CommandMap() ) );
        root->setCommandMap( commands );

        std::cout << "Testing: Obtain bound from quad..." << std::endl;
        {
            // Test that attaching a non-empty Drawable to a scene graph Node
            // heirarchy will correctly expand the Node's bounding volume.
            // Ensure that querying the volume clears all bound dirty flags.

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
            if( root->getBoundDirty() )
            {
                std::cerr << "Failed bound clean test." << std::endl;
                return( false );
            }
        }
#if 0
        std::cout << "Testing: Repeat, with new VAO..." << std::endl;
        {
            // Remove the root node's child (from the previous test).
            // Then repeat the same test, but use a different VAO address.
            // The test should still pass. Failure indicates that the
            // CommandMap did not use Notifier to inform the Node of the
            // VAO change.

            root->removeChild( 0 ); // Remove node from previous test.
            if( root->getNumChildren() > 0 )
            {
                std::cerr << "Invalid initial conditions. Wrong number of children: " << root->getNumChildren() << std::endl;
                return( false );
            }

            NodePtr nodeA( NodePtr( new Node() ) );
            jagUtil::VNTCVec data;
            DrawablePtr drawable( createQuad( data, gmtl::Point3f( 0., 0., 0. ), 10.f ) );
            nodeA->addDrawable( drawable );
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
#endif
    }

    {
        NodePtr root( NodePtr( new jagSG::Node() ) );

        CommandMapPtr commands( CommandMapPtr( new CommandMap() ) );
        root->setCommandMap( commands );

        std::cout << "Testing: Adding DrawCommand dirties SG Node bound..." << std::endl;
        {
            // Add a non-empty Drawable to a Node and query the Node bound.
            // Then add a new DrawCommand to the Drawable. Test that this
            // correctly marks the Node bound as dirty.

            NodePtr nodeA( NodePtr( new Node() ) );
            jagUtil::VNTCVec data;
            DrawablePtr drawable( createQuad( data, gmtl::Point3f( 0., 0., 0. ), 10.f ) );
            nodeA->addDrawable( drawable );
            root->addChild( nodeA );
            commands->insert( jagUtil::createVertexArrayObject( data ) );

            BoundPtr bound( root->getBound() );
            double returnRadius( bound->getRadius() );
            if( ( returnRadius < 7.05 ) || ( returnRadius > 7.1 ) )
            {
                std::cerr << "Incorrect radius: " << returnRadius << std::endl;
                return( false );
            }
            if( root->getBoundDirty() )
            {
                std::cerr << "Failed bound clean test." << std::endl;
                return( false );
            }

            jagDraw::DrawArraysPtr da( jagDraw::DrawArraysPtr( new jagDraw::DrawArrays( GL_POINTS, 0, 1 ) ) );
            drawable->addDrawCommand( da );
            if( !( root->getBoundDirty() ) )
            {
                std::cerr << "Failed, bound should be dirty after adding DrawCommand." << std::endl;
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
