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

#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>
#include <jagBase/Profile.h>
#include <jagBase/ptr.h>

#include <gmtl/gmtl.h>
#include <jagBase/gmtlSupport.h>

#include <boost/foreach.hpp>


namespace jagSG {


Node::Node()
  : jagBase::LogBase( "jag.sg.node" ),
    ObjectIDOwner(),
    _matrix( gmtl::MAT_IDENTITY44D )
{
}
Node::Node( const Node& rhs )
  : jagBase::LogBase( "jag.sg.node" ),
    ObjectIDOwner( rhs ),
    _matrix( rhs._matrix ),
    _commands( rhs._commands ),
    _drawables( rhs._drawables ),
    _children( rhs._children ),
    _bound( rhs._bound )
{
}
Node::~Node()
{
}


void Node::accept( jagSG::VisitorBase& visitor )
{
    JAG3D_TRACE( "accept()" );
    visitor.visit( *this );
}
void Node::traverse( jagSG::VisitorBase& visitor )
{
    JAG3D_TRACE( "traverse()" );

    BOOST_FOREACH( NodePtr& node, _children )
    {
        node->accept( visitor );
    }
}

void Node::setTraverseCallback( const CallbackPtr traverseCallback )
{
    _traverseCallback = traverseCallback;
}
const Node::CallbackPtr Node::getTraverseCallback() const
{
    return( _traverseCallback );
}

void Node::execute( jagDraw::DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    BOOST_FOREACH( jagDraw::DrawablePtr& drawable, _drawables )
    {
        drawable->execute( drawInfo );
    }

    JAG3D_ERROR_CHECK( "Node::execute()" );
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


jagDraw::BoundPtr Node::getBound( const jagDraw::CommandMap& commands )
{
    JAG3D_PROFILE( "SGNode::getBound" );

    const jagDraw::CommandMap newCommands( ( _commands != NULL ) ? commands + *_commands : commands );

    jagDraw::BoundSphere b;
    BOOST_FOREACH( NodePtr& node, _children )
    {
        b.expand( *( node->getBound( newCommands ) ) );
    }

    // Expand by Drawable bounds after expanding by child Node bounds
    // to increase precision.
    const jagDraw::DrawablePrepPtr& drawablePrep( newCommands[ jagDraw::DrawablePrep::VertexArrayObject_t ] );
    const jagDraw::VertexArrayObjectPtr vaop( boost::dynamic_pointer_cast< jagDraw::VertexArrayObject >( drawablePrep ) );
    b.expand( *( getDrawableBound( vaop ) ) );

    // Transform
    gmtl::Sphered newSphere = gmtl::transform( _matrix, b.asSphere() );
    // TBD ugly and expensive.
    _bound = jagDraw::BoundPtr( new jagDraw::BoundSphere( newSphere ) );

    return( _bound );
}
jagDraw::BoundPtr Node::getDrawableBound( const jagDraw::VertexArrayObjectPtr vaop )
{
    JAG3D_PROFILE( "SGNode::getDrawableBound" );

    jagDraw::BoundSphere b;
    BOOST_FOREACH( jagDraw::DrawablePtr& drawable, _drawables )
    {
        b.expand( *( drawable->getBound( vaop ) ) );
    }
    return( jagDraw::BoundPtr( new jagDraw::BoundSphere( b ) ) );
}



void Node::setCommandMap( jagDraw::CommandMapPtr commands )
{
    _commands = commands;
}
jagDraw::CommandMapPtr Node::getCommandMap()
{
    return( _commands );
}
const jagDraw::CommandMapPtr Node::getCommandMap() const
{
    return( _commands );
}


void Node::addDrawable( jagDraw::DrawablePtr node )
{
    _drawables.push_back( node );
}
unsigned int Node::getNumDrawables() const
{
    return( (unsigned int) _drawables.size() );
}
jagDraw::DrawablePtr Node::getDrawable( const unsigned int idx )
{
    if( idx >= getNumDrawables() )
        return( jagDraw::DrawablePtr( (jagDraw::Drawable*)NULL ) );
    return( _drawables[ idx ] );
}
const jagDraw::DrawablePtr Node::getDrawable( const unsigned int idx ) const
{
    if( idx >= getNumDrawables() )
        return( jagDraw::DrawablePtr( (jagDraw::Drawable*)NULL ) );
    return( _drawables[ idx ] );
}

    
void Node::addChild( NodePtr node )
{
    _children.push_back( node );
}
unsigned int Node::getNumChildren() const
{
    return( (unsigned int) _children.size() );
}
NodePtr Node::getChild( const unsigned int idx )
{
    if( idx >= getNumChildren() )
        return( NodePtr( (Node*)NULL ) );
    return( _children[ idx ] );
}
const NodePtr Node::getChild( const unsigned int idx ) const
{
    if( idx >= getNumChildren() )
        return( NodePtr( (Node*)NULL ) );
    return( _children[ idx ] );
}


void Node::setMaxContexts( const unsigned int numContexts )
{
    if( _commands != NULL )
        _commands->setMaxContexts( numContexts );

    BOOST_FOREACH( jagDraw::DrawablePtr& drawable, _drawables )
    {
        drawable->setMaxContexts( numContexts );
    }

    BOOST_FOREACH( NodePtr& node, _children )
    {
        node->setMaxContexts( numContexts );
    }
}
void Node::deleteID( const jagDraw::jagDrawContextID contextID )
{
    if( _commands != NULL )
        _commands->deleteID( contextID );

    BOOST_FOREACH( jagDraw::DrawablePtr& drawable, _drawables )
    {
        drawable->deleteID( contextID );
    }

    BOOST_FOREACH( NodePtr& node, _children )
    {
        node->deleteID( contextID );
    }
}


// jagSG
}
