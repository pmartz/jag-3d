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

#include <jagSG/Node.h>
#include <jagSG/CollectionVisitor.h>
#include <jagDraw/Node.h>
#include <jagDraw/Uniform.h>
#include <jagDraw/Program.h>

#include <boost/foreach.hpp>

#include <iostream>


using jagSG::Node;
using jagSG::NodePtr;
using jagSG::CollectionVisitor;

using jagDraw::CommandMap;
using jagDraw::CommandMapPtr;
using jagDraw::Uniform;
using jagDraw::UniformPtr;
using jagDraw::UniformSet;
using jagDraw::UniformSetPtr;
using jagDraw::Drawable;
using jagDraw::DrawablePtr;
using jagDraw::DrawNodeSimpleVec;
using jagDraw::Program;


bool test()
{
    NodePtr root( new Node() );
    CollectionVisitor cv;
    root->accept( cv );
    if( cv.getDrawGraph().size() > 0 )
    {
        std::cerr << "Empty scene graph returns non-NULL draw graph." << std::endl;
        return( false );
    }


    cv.reset();
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
    const DrawNodeSimpleVec& drawGraph( cv.getDrawGraph() );
    size_t size( drawGraph.size() );
    if( size != 1 )
    {
        std::cerr << "Draw graph size " << size << " != 1." << std::endl;
        return( false );
    }
    const jagDraw::Node& node( drawGraph[ 0 ] );
    if( node.getCommandMap() != commands )
    {
        std::cerr << "Wrong CommandMap." << std::endl;
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
