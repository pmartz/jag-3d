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
#include <jagBase/Profile.h>
#include <jagBase/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


Node::Node( CommandMapPtr commands, const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.draw.node" : logName ),
    ObjectIDOwner(),
    _matrix( gmtl::MAT_IDENTITY44D ),
    _commands( commands ),
    _distance( 0. )
{
}
Node::Node( const std::string& logName )
    : jagBase::LogBase( logName ),
    ObjectIDOwner(),
    _matrix( gmtl::MAT_IDENTITY44D ),
    _commands( ( CommandMap* )NULL ),
    _distance( 0. )
{
}
Node::Node( const Node& rhs )
  : jagBase::LogBase( "jag.draw.node" ),
    ObjectIDOwner( rhs ),
    _matrix( rhs._matrix ),
    _commands( rhs._commands ),
    _drawables( rhs._drawables ),
    _distance( rhs._distance ),
    _executeCallbacks( rhs._executeCallbacks )
{
}
Node::~Node()
{
}


void Node::setTransform( const gmtl::Matrix44d& matrix )
{
    _matrix = matrix;
}
gmtl::Matrix44d& Node::getTransform()
{
    return( _matrix );
}
const gmtl::Matrix44d& Node::getTransform() const
{
    return( _matrix );
}


Node::ExecuteCallbacks& Node::getExecuteCallbacks()
{
    return( _executeCallbacks );
}
const Node::ExecuteCallbacks& Node::getExecuteCallbacks() const
{
    return( _executeCallbacks );
}


void Node::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    JAG3D_PROFILE( "DrawNode execute()" );

    {
    JAG3D_PROFILE( "callbacks" );
    const ExecuteCallbacks& callbacks( getExecuteCallbacks() );
    BOOST_FOREACH( const jagDraw::Node::CallbackPtr& cb, callbacks )
    {
        if( !( (*cb)( *this, drawInfo ) ) )
        {
            return;
        }
    }
    }

    {
    JAG3D_PROFILE( "CommandMap" );
    drawInfo._current.executeDelta( *_commands, drawInfo );
    }

    {
        JAG3D_PROFILE( "Execute Drawable" );
        BOOST_FOREACH( DrawablePtr drawable, _drawables )
        {
            drawable->execute( drawInfo );
        }
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
