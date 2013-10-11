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

#include <jagDraw/Uniform.h>
#include <jagDraw/Program.h>

#include <iostream>


using jagDraw::Uniform;
using jagDraw::UniformPtr;
using jagDraw::UniformSet;
using jagDraw::UniformSetPtr;
using jagDraw::Program;


bool test()
{
    UniformPtr u0 = UniformPtr( new Uniform( "a", (GLint)1 ) );
    UniformPtr u1 = UniformPtr( new Uniform( "b", (GLint)2 ) );
    UniformSetPtr us0( new UniformSet() );
    us0->insert( u0 );
    us0->insert( u1 );

    GLint intVal;
    (*us0)[ u0->getNameHash() ]->get( intVal );
    if( intVal != 1 )
    {
        std::cerr << "Looking for value 1, found " << intVal << "." << std::endl;
        return( false );
    }
    (*us0)[ u1->getNameHash() ]->get( intVal );
    if( intVal != 2 )
    {
        std::cerr << "Looking for value 2, found " << intVal << "." << std::endl;
        return( false );
    }

    UniformPtr u2 = UniformPtr( new Uniform( "b", (GLint)10 ) );
    UniformPtr u3 = UniformPtr( new Uniform( "c", (GLint)11 ) );
    UniformSetPtr us1( new UniformSet() );
    us1->insert( u2 );
    us1->insert( u3 );

    (*us1)[ u2->getNameHash() ]->get( intVal );
    if( intVal != 10 )
    {
        std::cerr << "Looking for value 10, found " << intVal << "." << std::endl;
        return( false );
    }
    (*us1)[ u3->getNameHash() ]->get( intVal );
    if( intVal != 11 )
    {
        std::cerr << "Looking for value 11, found " << intVal << "." << std::endl;
        return( false );
    }

    us0 = boost::dynamic_pointer_cast< UniformSet >( us0->combine( us1 ) );

    Program::HashValue hashVal;
    hashVal = Program::createHash( "a" );
    if( us0->find( hashVal ) == us0->end() )
    {
        std::cerr << "Add result: uniform 'a' not contained in set." << std::endl;
        return( false );
    }
    (*us0)[ hashVal ]->get( intVal );
    if( intVal != 1 )
    {
        std::cerr << "Add result: uniform 'a' is " << intVal << ", should be 1." << std::endl;
        return( false );
    }
    hashVal = Program::createHash( "b" );
    (*us0)[ hashVal ]->get( intVal );
    if( intVal != 10 )
    {
        std::cerr << "Add result: uniform 'b' is " << intVal << ", should be 10." << std::endl;
        return( false );
    }
    hashVal = Program::createHash( "c" );
    (*us0)[ hashVal ]->get( intVal );
    if( intVal != 11 )
    {
        std::cerr << "Add result: uniform 'c' is " << intVal << ", should be 11." << std::endl;
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
