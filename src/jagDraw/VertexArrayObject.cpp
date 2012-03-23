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
  : VertexArrayCommand( VertexArrayCommand::VertexArrayObjectType )
{}

VertexArrayObject::VertexArrayObject( const VertexArrayObject& rhs )
  : VertexArrayCommand( rhs ),
    _ids( rhs._ids )
{}


VertexArrayObject::~VertexArrayObject() 
{
    // TBD Handle object deletion
}


void VertexArrayObject::operator()( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );

    glBindVertexArray( getId( contextID ) );

    IDStatusPair& idStatus = _ids[ contextID ];
    if( !( idStatus.second ) )
    {
        BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
        {
            (*vac)( drawInfo );
        }
        idStatus.second = true;
    }
}

GLint VertexArrayObject::getId( const unsigned int contextID )
{
    if( _ids[ contextID ].first == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ].first );
}

void VertexArrayObject::setMaxContexts( const unsigned int numContexts )
{
    while( _ids._data.size() < numContexts )
    {
        _ids._data.push_back( jagDraw::IDStatusPair( 0, false ) );
    }

    BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
    {
        BufferObject* bop( dynamic_cast< BufferObject* >( vac.get() ) );
        if( bop != NULL )
            bop->setMaxContexts( numContexts );
    }
}

void VertexArrayObject::addVertexArrayCommand( VertexArrayCommandPtr vacp, const VertexArrayCommand::UsageHint& usage )
{
    _commands.push_back( vacp );

    BOOST_FOREACH( IDStatusPair& idStatus, _ids._data )
    {
        idStatus.second = false;
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
    IDStatusPair& idStatus = _ids[ contextID ];

    glGenVertexArrays( 1, &( idStatus.first ) );
    idStatus.second = false;

    JAG3D_ERROR_CHECK( "VertexArrayObject::internalInit()" );
}


// jagDraw
}
