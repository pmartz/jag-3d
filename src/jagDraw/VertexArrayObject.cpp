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

#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagDraw/Error.h>


namespace jagDraw {


VertexArrayObject::VertexArrayObject()
{}

VertexArrayObject::VertexArrayObject( const VertexArrayObject& rhs )
  : _ids( rhs._ids )
{}


VertexArrayObject::~VertexArrayObject() 
{
    // TBD Handle object deletion
}


void VertexArrayObject::bind()
{
    const unsigned int contextID( 0 );

    if( _ids._data.size() < contextID+1 )
        internalInit( contextID );

    glBindVertexArray( _ids[ contextID ] );
}


void VertexArrayObject::internalInit( const unsigned int contextID )
{
    _ids._data.resize( contextID + 1 );
    glGenVertexArrays( 1, &( _ids[ contextID ] ) );

    JAG_ERROR_CHECK( "VertexArrayObject::internalInit()" );
}


// jagDraw
}
