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

#include <jagDraw/Node.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>
#include <jagBase/ptr.h>

#include <boost/foreach.hpp>


namespace jagDraw {


Node::Node( CommandMapPtr commands )
  : jagBase::LogBase( "jag.draw.node" ),
    ObjectIDOwner(),
    _commands( commands )
{
}
Node::Node( const Node& rhs )
  : jagBase::LogBase( "jag.draw.node" ),
    ObjectIDOwner( rhs ),
    _commands( rhs._commands ),
    _drawables( rhs._drawables )
{
}
Node::~Node()
{
}


void Node::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    CommandMap delta( drawInfo._current << (*_commands) );
    delta.execute( drawInfo );

    BOOST_FOREACH( DrawablePtr drawable, _drawables )
    {
        drawable->execute( drawInfo );
    }

    JAG3D_ERROR_CHECK( "Node::execute()" );
}

void Node::setMaxContexts( const unsigned int numContexts )
{
    if( _commands == NULL )
    {
        JAG3D_ERROR( "setMaxContexts(): jagDraw::Node must have non-NULL _commands." );
    }
    else
    {
        _commands->setMaxContexts( numContexts );
    }

    BOOST_FOREACH( DrawablePtr drawable, _drawables )
    {
        drawable->setMaxContexts( numContexts );
    }
}
void Node::deleteID( const jagDraw::jagDrawContextID contextID )
{
    _commands->deleteID( contextID );

    BOOST_FOREACH( DrawablePtr drawable, _drawables )
    {
        drawable->deleteID( contextID );
    }
}


// jagDraw
}
