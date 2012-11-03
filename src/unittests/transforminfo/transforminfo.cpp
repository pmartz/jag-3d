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
#include <gmtl/gmtl.h>

#include <iostream>


using jagBase::Transform44f;
using jagBase::Transform44d;
using gmtl::Matrix44f;
using gmtl::Matrix44d;


bool test()
{
    Transform44f xform0;
    Matrix44f viewf;
    gmtl::setLookAt( viewf, gmtl::Point3f( 3., 2., 1. ), gmtl::Point3f( 0., 0., 0. ), gmtl::Vec3f( 0., 0., 1. ) );
    xform0.setView( viewf );
    xform0.setProj( gmtl::MAT_IDENTITY44F );
    if( xform0.getViewProj() != viewf )
        return( false );

    const float floatEps( 0.00001f );
    const Matrix44f& identf( xform0.getViewInv() * viewf );
    if( !( gmtl::isEqual( identf, gmtl::MAT_IDENTITY44F, floatEps ) ) )
        return( false );

    Transform44d xform1;
    Matrix44d viewd;
    gmtl::setLookAt( viewd, gmtl::Point3d( 3., 2., 1. ), gmtl::Point3d( 0., 0., 0. ), gmtl::Vec3d( 0., 0., 1. ) );
    xform1.setView( viewd );
    xform1.setProj( gmtl::MAT_IDENTITY44D );
    if( xform1.getViewProj() != viewd )
        return( false );

    const double doubleEps( 0.0000001 );
    const Matrix44d& identd( xform1.getViewInv() * viewd );
    if( !( gmtl::isEqual( identd, gmtl::MAT_IDENTITY44D, doubleEps ) ) )
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
