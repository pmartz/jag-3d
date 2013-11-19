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

#include <jagBase/Transform.h>
#include <gmtl/gmtl.h>

#include <iostream>


using jagBase::TransformF;
using jagBase::TransformD;
using gmtl::Matrix44f;
using gmtl::Matrix44d;


bool test()
{
    TransformF xform0;
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

    TransformD xform1;
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
