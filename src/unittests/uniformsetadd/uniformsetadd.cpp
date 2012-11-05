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
    UniformSet us0;
    us0.insert( u0 );
    us0.insert( u1 );

    GLint intVal;
    us0[ u0->getNameHash() ]->get( intVal );
    if( intVal != 1 )
        return( false );
    us0[ u1->getNameHash() ]->get( intVal );
    if( intVal != 2 )
        return( false );

    UniformPtr u2 = UniformPtr( new Uniform( "b", (GLint)10 ) );
    UniformPtr u3 = UniformPtr( new Uniform( "c", (GLint)11 ) );
    UniformSet us1;
    us1.insert( u2 );
    us1.insert( u3 );

    us1[ u2->getNameHash() ]->get( intVal );
    if( intVal != 10 )
        return( false );
    us1[ u3->getNameHash() ]->get( intVal );
    if( intVal != 11 )
        return( false );

    UniformSet addResult = us0 + us1;

    Program::HashValue hashVal;
    hashVal = Program::createHash( "a" );
    addResult[ hashVal ]->get( intVal );
    if( intVal != 1 )
        return( false );
    hashVal = Program::createHash( "b" );
    addResult[ hashVal ]->get( intVal );
    if( intVal != 10 )
        return( false );
    hashVal = Program::createHash( "c" );
    addResult[ hashVal ]->get( intVal );
    if( intVal != 11 )
        return( false );

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