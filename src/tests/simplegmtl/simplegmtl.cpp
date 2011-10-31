/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag-3d is (C) Copyright 2011-2012 by Kenneth Mark Bryden and Paul Martz
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

#include <iostream>
#include <vector>
#include <gmtl/gmtl.h>

int main()
{
    std::cout << "Running simplegmtl." << std::endl;

    gmtl::Point3f p( 1., 2., 3. );
    std::cout << p << std::endl;

    gmtl::Matrix44f m = gmtl::makeTrans< gmtl::Matrix44f >( gmtl::Vec3f( 10., 0., 0. ) );
    std::cout << m << std::endl;
    std::cout << m * p << std::endl;

    gmtl::Point3i p2( 121, 18, 7 );
    int* pptr = p2.getData();
    std::cout << pptr[0] << " " << pptr[1] << " " << pptr[2] << std::endl;
    std::cout << p2 << std::endl;

    typedef std::vector< gmtl::Point3f > PointArray3f;
    PointArray3f pa3f;
    pa3f.push_back( gmtl::Point3f( 1.f, 2.f, 3.f ) );
    pa3f.push_back( gmtl::Point3f( 4.f, 5.f, 6.f ) );
    pa3f.push_back( gmtl::Point3f( 7.f, 8.f, 9.f ) );
    float* fptr = (float*)&pa3f[0];
    std::cout << fptr[0] << " " << fptr[1] << " " << fptr[2] << std::endl;
    std::cout << fptr[3] << " " << fptr[4] << " " << fptr[5] << std::endl;
    std::cout << fptr[6] << " " << fptr[7] << " " << fptr[8] << std::endl;

    return( 0 );
}
