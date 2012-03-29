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
#include <jagDraw/BufferObject.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>

#include <boost/foreach.hpp>


namespace jagDraw {


VertexArrayObject::VertexArrayObject()
  : DrawablePrep(),
    ObjectID()
{}

VertexArrayObject::VertexArrayObject( const VertexArrayObject& rhs )
  : DrawablePrep( rhs ),
    ObjectID( rhs )
{}


VertexArrayObject::~VertexArrayObject() 
{
    // TBD Handle object deletion
}


void VertexArrayObject::operator()( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );

    glBindVertexArray( getID( contextID ) );
    GLboolean& initialized( _initialized[ contextID ] );

    if( initialized == GL_FALSE )
    {
        BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
        {
            (*vac)( drawInfo );
        }
        initialized = GL_TRUE;
    }
}

GLuint VertexArrayObject::getID( const unsigned int contextID )
{
    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}

void VertexArrayObject::setMaxContexts( const unsigned int numContexts )
{
    _ids._data.resize( numContexts );
    _initialized._data.resize( numContexts );

    BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
    {
        vac->setMaxContexts( numContexts );
    }
}

void VertexArrayObject::addVertexArrayCommand( VertexArrayCommandPtr vacp, const VertexArrayCommand::UsageHint& usage )
{
    _commands.push_back( vacp );

    unsigned int idx;
    for( idx=0; idx < _initialized._data.size(); idx++ )
    {
        _initialized[ idx ] = false;
    }

    if( usage == VertexArrayCommand::Vertex )
    {
        _vertices.push_back( vacp );
    }
}
VertexArrayCommandList& VertexArrayObject::getVertexArrayCommandList()
{
    return( _commands );
}
const VertexArrayCommandList& VertexArrayObject::getVertexArrayCommandList() const
{
    return( _commands );
}


void VertexArrayObject::internalInit( const unsigned int contextID )
{
    glGenVertexArrays( 1, &( _ids[ contextID ] ) );
    _initialized[ contextID ] = GL_FALSE;

    JAG3D_ERROR_CHECK( "VertexArrayObject::internalInit()" );
}


// jagDraw
}
