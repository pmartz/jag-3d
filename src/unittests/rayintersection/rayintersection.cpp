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
#include <jagSG/Node.h>
#include <jagSG/CollectionVisitor.h>
#include <jagSG/NodeMaskCullCallback.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/Uniform.h>
#include <jag/draw/Program.h>
#include <jagUtil/Shapes.h>
#include <jagSG/IntersectVisitor.h>

#include <boost/foreach.hpp>
#include <gmtl/gmtl.h>
#include <gmtl/MatrixOps.h>

#include <iostream>


using jag::base::TransformD;

using jagSG::Node;
using jagSG::NodePtr;
using jagSG::CollectionVisitor;
using jagSG::NodeMaskCullCallback;
using jagSG::NodeMaskCullDistributionVisitor;

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


jagSG::NodePtr createQuad( const gmtl::Point3f& center, const float extent )
{
    jagSG::NodePtr quadRoot = jagSG::NodePtr( new jagSG::Node() );

    // Create container to store the quad vertex / normal / texcoord data.
    jagUtil::VNTCVec data;

    // Create the quad and its data
    const gmtl::Point3f corner( center -
        gmtl::Point3f( extent/2.f, extent/2.f, 0.f ) );
    const gmtl::Vec3f uVec( extent, 0.f, 0.f );
    const gmtl::Vec3f vVec( 0.f, extent, 0.f );
    jag::draw::DrawablePtr quad( jagUtil::makePlane(
        data, corner, uVec, vVec ) );
    quadRoot->addDrawable( quad );

    // Put the data in an array buffer object, and add it to
    // a VertexArrayObject.
    jag::draw::CommandMapPtr commands( quadRoot->getCommandMap() );
    if( commands == NULL )
    {
        commands = jag::draw::CommandMapPtr( new jag::draw::CommandMap() );
        quadRoot->setCommandMap( commands );
    }
    commands->insert( jagUtil::createVertexArrayObject( data ) );

    return( quadRoot );
}


bool test()
{
    {
        gmtl::Matrix44d view, proj;
        const gmtl::Point3d eye( 0., 0., 20. );
        const gmtl::Point3d center( 0., 0., 0. );
        const gmtl::Vec3d up( 0., 1., 0. );
        gmtl::setLookAt< double >( view, eye, center, up );
        gmtl::setPerspective< double >( proj, 10., 1., 1., 200. );

        CollectionVisitor cv;
        cv.setViewProj( view, proj );
        cv.setViewport( 0, 0, 100, 100 );

        jag::draw::DrawGraphPtr drawGraphTemplate( new jag::draw::DrawGraph() );
        drawGraphTemplate->resize( 1 );
        cv.setDrawGraphTemplate( drawGraphTemplate );



        std::cout << "testing a simple case of a ray intersection test" << std::endl;
        {
            NodePtr root( new Node() );
            root->addChild( createQuad( gmtl::Point3f( 0., 0., 0. ), 10.f ) );

            jagSG::IntersectVisitor iv(root, gmtl::Ray<double>(gmtl::Point3d(1,0,10), gmtl::Point3d(0,0,-1)));


            for(auto i = 0; i < iv.getHits().size(); i++)
                std::cout << iv.getHits()[i].hitPosition << std::endl;


        }

        std::cout << "testing a graph with multiple levels" << std::endl;
        {
            NodePtr root( new Node() );
            NodePtr xform = NodePtr(new Node());
            root->addChild( xform);
            xform->setTransform(gmtl::makeTrans<gmtl::Matrix44d>(gmtl::Vec3d(0,1.5,0)));
            xform->addChild( createQuad( gmtl::Point3f( 0., 0., 0. ), 10.f ) );

            jagSG::IntersectVisitor iv(root, gmtl::Ray<double>(gmtl::Point3d(1,0,10), gmtl::Point3d(0,0,-1)));


            for(auto i = 0; i < iv.getHits().size(); i++)
                std::cout << iv.getHits()[i].hitPosition << std::endl;
        }

        std::cout << "Testing: Single quad accept using non-modified NodeMask (default is true)..." << std::endl;
        {
            NodePtr root( new Node() );
            root->addChild( createQuad( gmtl::Point3f( 0., 0., 0. ), 10.f ) );

            NodeMaskCullDistributionVisitor fcdv;
            root->accept( fcdv );

            root->accept( cv );
            const jag::draw::DrawGraphPtr drawGraph( cv.getDrawGraph() );
            size_t size( drawGraph->size() );
            if( size != 1 )
            {
                std::cerr << "Draw graph size " << size << " != 1." << std::endl;
                return( false );
            }
            const DrawNodeContainer& nodes( (*drawGraph)[ 0 ] );
            if( nodes.size() != 1 )
            {
                std::cerr << "Failed: DrawNodeContainer incorrect size: " << nodes.size() << std::endl;
                return( false );
            }
            const jag::draw::DrawNodePtr& node( nodes[ 0 ] );
            if( node->getNumDrawables() != 1 )
            {
                std::cerr << "Node num drawables " << node->getNumDrawables() << " != 1." << std::endl;
                return( false );
            }
        }

        cv.reset();
        std::cout << "Testing: two quads, one with the default mask and the other with the nodemask set to false ..." << std::endl;
        {
            NodePtr root( new Node() );
            root->addChild( createQuad( gmtl::Point3f( 0., 0., 0. ), 10.f ) );

            // Second quad is in the same place but has it's NodeMask set to false
            NodePtr second = createQuad( gmtl::Point3f( 0., 0., 0. ), 10.f );
            second->setNodeMask(false);
            root->addChild( second );

            NodeMaskCullDistributionVisitor fcdv;
            root->accept( fcdv );

            root->accept( cv );
            const jag::draw::DrawGraphPtr drawGraph( cv.getDrawGraph() );
            const DrawNodeContainer& nodes( (*drawGraph)[ 0 ] );
            if( nodes.size() != 1 )
            {
                std::cerr << "Failed: DrawNodeContainer incorrect size: " << nodes.size() << std::endl;
                return( false );
            }
            const jag::draw::DrawNodePtr& node( nodes[ 0 ] );
            if( node->getNumDrawables() != 1 )
            {
                std::cerr << "Node num drawables " << node->getNumDrawables() << " != 1." << std::endl;
                return( false );
            }
        }

        cv.reset();
        std::cout << "Testing: ten quads quads, nine with the mask set to true and one with the nodemask set to false ..." << std::endl;
        {
            NodePtr root( new Node() );
            //root->addChild( createQuad( gmtl::Point3f( 0., 0., 0. ), 10.f ) );

            // All but the fifth child have the nodemask set to true explicitly
            for(unsigned int i = 0; i < 10; i++) {
                NodePtr child = createQuad( gmtl::Point3f( 0., 0., 0. ), 10.f );
                if(i==4)
                    child->setNodeMask(false);
                else
                    child->setNodeMask(true);
                root->addChild( child );
            }

            NodeMaskCullDistributionVisitor fcdv;
            root->accept( fcdv );

            root->accept( cv );
            const jag::draw::DrawGraphPtr drawGraph( cv.getDrawGraph() );
            const DrawNodeContainer& nodes( (*drawGraph)[ 0 ] );
            if( nodes.size() != 9 )
            {
                std::cerr << "Failed: DrawNodeContainer incorrect size: " << nodes.size() << std::endl;
                return( false );
            }
            const jag::draw::DrawNodePtr& node( nodes[ 0 ] );
            if( node->getNumDrawables() != 1 )
            {
                std::cerr << "Node num drawables " << node->getNumDrawables() << " != 1." << std::endl;
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
