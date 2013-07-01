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
    _parents( rhs._parents ),
    _bounds( rhs._bounds ),
    _traverseCallback( rhs._traverseCallback ),
    _collectionCallbacks( rhs._collectionCallbacks )
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
Node::CollectionCallbacks& Node::getCollectionCallbacks()
{
    return( _collectionCallbacks );
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

    const jagDraw::DrawablePrepPtr& drawablePrep( newCommands[ jagDraw::DrawablePrep::VertexArrayObject_t ] );
    const jagDraw::VertexArrayObjectPtr vaop( boost::dynamic_pointer_cast< jagDraw::VertexArrayObject >( drawablePrep ) );

    boost::mutex::scoped_lock lock( _mutex );

    jagDraw::BoundInfo& bi( _bounds[ vaop.get() ] );
    if( !( bi._dirty ) )
        return( bi._bound );

    // Get bounds of all drawables.
    const jagDraw::BoundPtr drawableBound( getDrawableBound( vaop ) );

    // Compute the average center of the drawable bounds and
    // all child node bounds.
    gmtl::Point3d averageCenter( drawableBound->getEmpty() ? gmtl::Point3d( 0., 0., 0. ) : drawableBound->getCenter() );
    BOOST_FOREACH( NodePtr& node, _children )
    {
        averageCenter += node->getBound( newCommands )->getCenter();
    }
    averageCenter /= ( getNumChildren() + ( drawableBound->getEmpty() ? 0 : 1 ) );

    // Center a bound on the average center, then expand it by
    // all child nodes and all attached Drawables.
    jagDraw::BoundSphere b;
    b.setCenter( averageCenter );
    b.setEmpty( false );
    BOOST_FOREACH( NodePtr& node, _children )
    {
        b.expand( *( node->getBound( newCommands ) ) );
    }
    b.expand( *drawableBound );

    // Transform
    gmtl::Sphered newSphere = gmtl::transform( _matrix, b.asSphere() );
    // TBD ugly and expensive.
    bi._bound = jagDraw::BoundPtr( new jagDraw::BoundSphere( newSphere ) );
    bi._dirty = false;

    return( bi._bound );
}
jagDraw::BoundPtr Node::getDrawableBound( const jagDraw::VertexArrayObjectPtr vaop )
{
    JAG3D_PROFILE( "SGNode::getDrawableBound" );

    if( getNumDrawables() == 0 )
    {
        // Return an empty / uninitialized sphere.
        return( jagDraw::BoundPtr( new jagDraw::BoundSphere() ) );
    }

    gmtl::Point3d averageCenter( 0., 0., 0. );
    BOOST_FOREACH( jagDraw::DrawablePtr& drawable, _drawables )
    {
        averageCenter += drawable->getBound( vaop.get() )->getCenter();
    }
    averageCenter /= getNumDrawables();

    jagDraw::BoundSphere b;
    b.setCenter( averageCenter );
    b.setEmpty( false );
    BOOST_FOREACH( jagDraw::DrawablePtr& drawable, _drawables )
    {
        b.expand( *( drawable->getBound( vaop.get() ) ) );
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
    node->addParent( shared_from_this() );
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

void Node::addParent( NodePtr node )
{
    _parents.push_back( node );
}
unsigned int Node::getNumParents() const
{
    return( (unsigned int) _parents.size() );
}
NodePtr Node::getParent( const unsigned int idx )
{
    if( idx >= getNumParents() )
        return( NodePtr( (Node*)NULL ) );
    return( _parents[ idx ] );
}
const NodePtr Node::getParent( const unsigned int idx ) const
{
    if( idx >= getNumParents() )
        return( NodePtr( (Node*)NULL ) );
    return( _parents[ idx ] );
}
const NodeVec& Node::getParents() const
{
    return( _parents );
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
