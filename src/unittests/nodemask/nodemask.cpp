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
#include <jagSG/NodeMaskCullCallback.h>
#include <jagDraw/Node.h>
#include <jagDraw/Uniform.h>
#include <jagDraw/Program.h>
#include <jagUtil/Shapes.h>

#include <boost/foreach.hpp>
#include <gmtl/gmtl.h>

#include <iostream>


using jagBase::TransformD;

using jagSG::Node;
using jagSG::NodePtr;
using jagSG::CollectionVisitor;
using jagSG::NodeMaskCullCallback;
using jagSG::NodeMaskCullDistributionVisitor;

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
    jagDraw::DrawablePtr quad( jagUtil::makePlane(
        data, corner, uVec, vVec ) );
    quadRoot->addDrawable( quad );

    // Put the data in an array buffer object, and add it to
    // a VertexArrayObject.
    jagDraw::CommandMapPtr commands( quadRoot->getCommandMap() );
    if( commands == NULL )
    {
        commands = jagDraw::CommandMapPtr( new jagDraw::CommandMap() );
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

        jagDraw::DrawGraphPtr drawGraphTemplate( new jagDraw::DrawGraph() );
        drawGraphTemplate->resize( 1 );
        cv.setDrawGraphTemplate( drawGraphTemplate );

        std::cout << "Testing: Single quad accept using non-modified NodeMask (default is true)..." << std::endl;
        {
            NodePtr root( new Node() );
            root->addChild( createQuad( gmtl::Point3f( 0., 0., 0. ), 10.f ) );

            NodeMaskCullDistributionVisitor fcdv;
            root->accept( fcdv );

            root->accept( cv );
            const jagDraw::DrawGraphPtr drawGraph( cv.getDrawGraph() );
            size_t size( drawGraph->size() );
            if( size != 1 )
            {
                std::cerr << "Draw graph size " << size << " != 1." << std::endl;
                return( false );
            }
            const NodeContainer& nodes( (*drawGraph)[ 0 ] );
            if( nodes.size() != 1 )
            {
                std::cerr << "Failed: NodeContainer incorrect size: " << nodes.size() << std::endl;
                return( false );
            }
            const jagDraw::Node& node( nodes[ 0 ] );
            if( node.getNumDrawables() != 1 )
            {
                std::cerr << "Node num drawables " << node.getNumDrawables() << " != 1." << std::endl;
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
            const jagDraw::DrawGraphPtr drawGraph( cv.getDrawGraph() );
            const NodeContainer& nodes( (*drawGraph)[ 0 ] );
            if( nodes.size() != 1 )
            {
                std::cerr << "Failed: NodeContainer incorrect size: " << nodes.size() << std::endl;
                return( false );
            }
            const jagDraw::Node& node( nodes[ 0 ] );
            if( node.getNumDrawables() != 1 )
            {
                std::cerr << "Node num drawables " << node.getNumDrawables() << " != 1." << std::endl;
                return( false );
            }
        }

		cv.reset();
        std::cout << "Testing: ten quads quads, nine with the mask set to true and one with the nodemask set to false ..." << std::endl;
        {
            NodePtr root( new Node() );
            //root->addChild( createQuad( gmtl::Point3f( 0., 0., 0. ), 10.f ) );

            // All but the fifth child have the nodemask set to true explicitly
			for(auto i = 0; i < 10; i++) {
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
            const jagDraw::DrawGraphPtr drawGraph( cv.getDrawGraph() );
            const NodeContainer& nodes( (*drawGraph)[ 0 ] );
            if( nodes.size() != 9 )
            {
                std::cerr << "Failed: NodeContainer incorrect size: " << nodes.size() << std::endl;
                return( false );
            }
            const jagDraw::Node& node( nodes[ 0 ] );
            if( node.getNumDrawables() != 1 )
            {
                std::cerr << "Node num drawables " << node.getNumDrawables() << " != 1." << std::endl;
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
