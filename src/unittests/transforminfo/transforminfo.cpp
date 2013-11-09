/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
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
