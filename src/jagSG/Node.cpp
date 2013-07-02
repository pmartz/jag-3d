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
    jagDraw::BoundOwner(),
    _matrix( gmtl::MAT_IDENTITY44D )
{
}
Node::Node( const Node& rhs )
  : jagBase::LogBase( "jag.sg.node" ),
    ObjectIDOwner( rhs ),
    jagDraw::BoundOwner( rhs ),
    _matrix( rhs._matrix ),
    _commands( rhs._commands ),
    _drawables( rhs._drawables ),
    _children( rhs._children ),
    _parents( rhs._parents ),
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


    // If dirty==true, BoundOwner::getBound() will call computeBound().
    // For jagSG::Node, computeBound() will compute the bound of the
    // Node's Drawables. Then we can include the bound of the child
    // Nodes.
    //
    // If dirty==false, BoundOwner::getBound() will return the complete
    // subgraph bound, and our work is done.

    bool dirty( getBoundDirty( vaop.get() ) );
    jagDraw::BoundPtr bound( jagDraw::BoundOwner::getBound( vaop.get() ) );

    if( dirty )
    {
        // Compute the average center of the drawable bounds and
        // all child node bounds.
        gmtl::Point3d averageCenter( bound->getEmpty() ? gmtl::Point3d( 0., 0., 0. ) : bound->getCenter() );
        BOOST_FOREACH( NodePtr& node, _children )
        {
            averageCenter += node->getBound( newCommands )->getCenter();
        }
        averageCenter /= ( getNumChildren() + ( bound->getEmpty() ? 0 : 1 ) );

        // Create a new bound.
        jagDraw::BoundPtr fullBound;
        if( _initialBound == NULL )
            fullBound = newBound();
        else
            fullBound = _initialBound->clone();

        // Center a bound on the average center, then expand it by
        // all child nodes and all attached Drawables.
        fullBound->setCenter( averageCenter );
        fullBound->setEmpty( false );
        BOOST_FOREACH( NodePtr& node, _children )
        {
            fullBound->expand( *( node->getBound( newCommands ) ) );
        }
        fullBound->expand( *bound );

        // Transform
        fullBound->transform( _matrix );

        // Store in BoundMap
        _bounds[ vaop.get() ]._bound = fullBound;
        return( fullBound );
    }
    else
    {
        // Bound was not dirty.
        return( bound );
    }
}

jagDraw::BoundPtr Node::newBound()
{
    return( jagDraw::BoundPtr( new jagDraw::BoundSphere() ) );
}

void Node::computeBound( jagDraw::BoundPtr bound, const jagDraw::VertexArrayObject* vao, BoundOwner* owner )
{
    JAG3D_PROFILE( "SGNode::computeBound" );

    bound->setEmpty();
    if( getNumDrawables() == 0 )
        return;

    gmtl::Point3d averageCenter( 0., 0., 0. );
    BOOST_FOREACH( jagDraw::DrawablePtr& drawable, _drawables )
    {
        averageCenter += drawable->getBound( vao )->getCenter();
    }
    averageCenter /= getNumDrawables();

    bound->setCenter( averageCenter );
    bound->setEmpty( false );
    BOOST_FOREACH( jagDraw::DrawablePtr& drawable, _drawables )
    {
        bound->expand( *( drawable->getBound( vao ) ) );
    }
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
