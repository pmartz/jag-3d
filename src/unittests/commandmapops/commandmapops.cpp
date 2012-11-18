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

#include <jagDraw/CommandMap.h>
#include <jagDraw/Program.h>
#include <jagDraw/VertexArrayObject.h>

#include <iostream>


using jagDraw::DrawablePrep;
using jagDraw::CommandMap;
using jagDraw::Program;
using jagDraw::ProgramPtr;
using jagDraw::VertexArrayObject;
using jagDraw::VertexArrayObjectPtr;


bool test()
{
    {
        CommandMap cmA, cmB;
        ProgramPtr progA( ProgramPtr( new Program() ) );
        progA->_nametbd = "progA";
        cmA.insert( progA );
        if( !cmA.contains( DrawablePrep::Program_t ) )
        {
            std::cerr << "cmA.contains( DrawablePrep::Program_t ) is false." << std::endl;
            return( false );
        }
        ProgramPtr progB( ProgramPtr( new Program() ) );
        progB->_nametbd = "progB";
        cmB.insert( progB );
        if( !cmB.contains( DrawablePrep::Program_t ) )
        {
            std::cerr << "cmB.contains( DrawablePrep::Program_t ) is false." << std::endl;
            return( false );
        }

        CommandMap cmResult( cmA + cmB );
        if( !cmResult.contains( DrawablePrep::Program_t ) )
        {
            std::cerr << "cmResult.contains( DrawablePrep::Program_t ) is false." << std::endl;
            return( false );
        }
        if( cmResult[ DrawablePrep::Program_t ]->_nametbd != "progB" )
        {
            std::cerr << "cmResult[ DrawablePrep::Program_t ]->_nametbd != \"progB\"." << std::endl;
            return( false );
        }
    }

    {
        CommandMap cmA, cmB;
        VertexArrayObjectPtr vaoA( VertexArrayObjectPtr( new VertexArrayObject() ) );
        vaoA->_nametbd = "vaoA";
        cmA.insert( vaoA );
        if( !cmA.contains( DrawablePrep::VertexArrayObject_t ) )
        {
            std::cerr << "cmA.contains( DrawablePrep::VertexArrayObject_t ) is false." << std::endl;
            return( false );
        }
        VertexArrayObjectPtr vaoB( VertexArrayObjectPtr( new VertexArrayObject() ) );
        vaoB->_nametbd = "vaoB";
        cmB.insert( vaoB );
        if( !cmB.contains( DrawablePrep::VertexArrayObject_t ) )
        {
            std::cerr << "cmB.contains( DrawablePrep::VertexArrayObject_t ) is false." << std::endl;
            return( false );
        }

        CommandMap cmResult( cmA + cmB );
        if( !cmResult.contains( DrawablePrep::VertexArrayObject_t ) )
        {
            std::cerr << "cmResult.contains( DrawablePrep::VertexArrayObject_t ) is false." << std::endl;
            return( false );
        }
        if( cmResult[ DrawablePrep::VertexArrayObject_t ]->_nametbd != "vaoB" )
        {
            std::cerr << "cmResult[ DrawablePrep::VertexArrayObject_t ]->_nametbd != \"vaoB\"." << std::endl;
            return( false );
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
