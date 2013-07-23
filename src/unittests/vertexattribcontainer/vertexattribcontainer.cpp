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

#include <jagDraw/DrawCommand.h>
#include <jagDraw/VertexAttribContainer.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagUtil/Shapes.h>

#include <iostream>


using jagDraw::BufferObject;
using jagDraw::VertexAttrib;
using jagDraw::VertexAttribContainer;
using jagDraw::VertexArrayObject;
using jagDraw::VertexArrayObjectPtr;
using jagDraw::DrawCommand;
using jagDraw::DrawCommandPtr;
using jagDraw::Drawable;
using jagDraw::DrawablePtr;
using jagDraw::DrawArrays;
using jagDraw::DrawElements;


DrawablePtr createQuad( jagUtil::VNTCVec& data, const gmtl::Point3f& center, const float extent )
{
    // Create the quad and its data
    const gmtl::Point3f corner( center -
        gmtl::Point3f( extent/2.f, extent/2.f, 0.f ) );
    const gmtl::Vec3f uVec( extent, 0.f, 0.f );
    const gmtl::Vec3f vVec( 0.f, extent, 0.f );
    jagDraw::DrawablePtr quad( jagUtil::makePlane(
        data, corner, uVec, vVec ) );

    return( quad );
}


bool test()
{
    std::cout << "Testing: Iterate over simple quad..." << std::endl;
    {
        jagUtil::VNTCVec data;
        DrawablePtr quad( createQuad( data, gmtl::Point3f( 0., 0., 0. ), 10.f ) );
        VertexArrayObjectPtr vaop( jagUtil::createVertexArrayObject( data ) );
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
