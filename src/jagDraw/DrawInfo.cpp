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

#include <jagDraw/DrawInfo.h>
#include <gmtl/gmtl.h>


namespace jagDraw {


DrawInfo::DrawInfo()
    : _id( 0 ),
    _elementBufferID( 0 ),
    _transform()
{
    // Clear the _transform to zeros so that it will not match the first
    // matrix and therefore trigger the transform uniform initialization.
    // See jagDraw::TransformCallback.
    for( unsigned int x = 0; x < 4; ++x )
        _transform( x, x ) = 0.;
}


// jagDraw
}
