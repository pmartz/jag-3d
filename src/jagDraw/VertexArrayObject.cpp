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
  : DrawablePrep( VertexArrayObject_t ),
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


void VertexArrayObject::execute( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );

    glBindVertexArray( getID( contextID ) );
    GLboolean& initialized( _initialized[ contextID ] );

    if( initialized == GL_FALSE )
    {
        BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
        {
            vac->execute( drawInfo );
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
    ObjectID::setMaxContexts( numContexts );

    _initialized._data.resize( numContexts );

    BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
    {
        // Not all vertex array commands have objectIDs. Only set the
        // max context count on commands that have IDs.
        ObjectIDPtr objID( boost::dynamic_pointer_cast< ObjectID >( vac ) );
        if( objID != NULL )
            objID->setMaxContexts( numContexts );
        else
        {
            ObjectIDOwnerPtr objIDOwner( boost::dynamic_pointer_cast< ObjectIDOwner >( vac ) );
            if( objIDOwner != NULL )
                objIDOwner->setMaxContexts( numContexts );
        }
    }
}

void VertexArrayObject::addVertexArrayCommand( VertexArrayCommandPtr vacp, const UsageHint& usage )
{
    _commands.push_back( vacp );

    unsigned int idx;
    for( idx=0; idx < _initialized._data.size(); idx++ )
    {
        _initialized[ idx ] = false;
    }

    if( usage == Vertex )
    {
        _vertices.push_back( vacp );
    }
}
VertexArrayCommandPtr VertexArrayObject::getVertexArrayCommand( const VertexArrayCommand::Type& type, const UsageHint& usage )
{
    if( usage == Vertex )
    {
        BOOST_FOREACH( VertexArrayCommandPtr vacp, _vertices )
        {
            if( vacp->getType() == type )
                return( vacp );
        }
    }
    else
    {
        BOOST_FOREACH( VertexArrayCommandPtr vacp, _commands )
        {
            if( vacp->getType() == type )
                return( vacp );
        }
    }
    return( VertexArrayCommandPtr( (VertexArrayCommand*)NULL ) );
}

VertexArrayCommandVec& VertexArrayObject::getVertexArrayCommandList()
{
    return( _commands );
}
const VertexArrayCommandVec& VertexArrayObject::getVertexArrayCommandList() const
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
