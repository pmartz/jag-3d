/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:
 
    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.
 
    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.
 
    3. This notice may not be removed or altered from any source
    distribution.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#include <jag/sg/Node.h>
#include <jag/sg/Visitor.h>
#include <jag/draw/DrawInfo.h>
#include <jag/draw/Command.h>
#include <jag/draw/CommandMap.h>
#include <jag/draw/Error.h>
#include <jag/base/LogMacros.h>
#include <jag/base/Profile.h>
#include <jag/base/ptr.h>

#include <gmtl/gmtl.h>
#include <jag/base/gmtlSupport.h>

#include <boost/foreach.hpp>


namespace jag {
namespace sg {


Node::Node( const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.sg.node" : logName ),
    jag::base::UserDataOwner(),
    ObjectIDOwner(),
    SHARED_FROM_THIS( Node )(),
    jag::draw::BoundOwner(),
    _matrix( gmtl::MAT_IDENTITY44D )
{
    _boundDirtyCallback = BoundDirtyCallbackPtr( new BoundDirtyCallback( this ) );
}
Node::Node( const Node& rhs )
  : jag::base::LogBase( "jag.sg.node" ),
    jag::base::UserDataOwner( rhs ),
    ObjectIDOwner( rhs ),
    SHARED_FROM_THIS( Node )( rhs ),
    jag::draw::BoundOwner( rhs ),
    _matrix( rhs._matrix ),
    _commands( rhs._commands ),
    _drawables( rhs._drawables ),
    _children( rhs._children ),
    _parents( rhs._parents ),
    _traverseCallback( rhs._traverseCallback ),
    _collectionCallbacks( rhs._collectionCallbacks )
{
    _boundDirtyCallback = BoundDirtyCallbackPtr( new BoundDirtyCallback( this ) );
}
Node::~Node()
{
}


void Node::accept( jag::sg::VisitorBase& visitor )
{
    JAG3D_TRACE( "accept()" );
    visitor.visit( *this );
}
void Node::traverse( jag::sg::VisitorBase& visitor )
{
    JAG3D_TRACE( "traverse()" );

    BOOST_FOREACH( NodePtr& node, _children )
    {
        node->accept( visitor );
    }
}

void Node::setTraverseCallback( const CallbackPtr& traverseCallback )
{
    _traverseCallback = traverseCallback;
}
const Node::CallbackPtr& Node::getTraverseCallback() const
{
    return( _traverseCallback );
}
Node::CollectionCallbacks& Node::getCollectionCallbacks()
{
    return( _collectionCallbacks );
}

void Node::execute( jag::draw::DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    BOOST_FOREACH( jag::draw::DrawablePtr& drawable, _drawables )
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


jag::draw::BoundPtr Node::newBound()
{
    return( jag::draw::BoundPtr( new jag::draw::BoundSphere() ) );
}

void Node::computeBound( jag::draw::BoundPtr& bound, const jag::draw::CommandMap& commands )
{
    JAG3D_PROFILE( "SGNode::computeBound" );

    if( getNumDrawables() + getNumChildren() == 0 )
        return;

    bound->setEmpty();
    jag::draw::CommandMap newCommands( 
        ( _commands == NULL ) ? commands : commands + *_commands );

    gmtl::Point3d averageCenter( 0., 0., 0. );
    BOOST_FOREACH( jag::draw::DrawablePtr& drawable, _drawables )
    {
        averageCenter += drawable->getBound( newCommands )->getCenter();
    }
    BOOST_FOREACH( jag::sg::NodePtr& node, _children )
    {
        jag::draw::BoundOwner* boundOwner( node.get() );
        averageCenter += boundOwner->getBound( newCommands )->getCenter();
    }
    averageCenter /= ( getNumDrawables() + getNumChildren() );

    bound->setCenter( averageCenter );
    bound->setEmpty( false );
    BOOST_FOREACH( jag::draw::DrawablePtr& drawable, _drawables )
    {
        bound->expand( *( drawable->getBound( newCommands ) ) );
    }
    BOOST_FOREACH( jag::sg::NodePtr& node, _children )
    {
        jag::draw::BoundOwner* boundOwner( node.get() );
        bound->expand( *( boundOwner->getBound( newCommands ) ) );
    }

    bound->transform( _matrix );
}

void Node::setBoundDirty( const bool dirty )
{
    if( _dirty == dirty )
        return;

    BoundOwner::setBoundDirty( dirty );
    if( dirty )
    {
        BOOST_FOREACH( Node* parentNode, _parents )
        {
            parentNode->setBoundDirty();
        }
    }
}



void Node::setCommandMap( jag::draw::CommandMapPtr& commands )
{
    _commands = commands;
}
jag::draw::CommandMapPtr& Node::getCommandMap()
{
    return( _commands );
}
const jag::draw::CommandMapPtr& Node::getCommandMap() const
{
    return( _commands );
}
jag::draw::CommandMapPtr& Node::getOrCreateCommandMap()
{
    if( _commands == NULL )
        _commands = jag::draw::CommandMapPtr( new jag::draw::CommandMap() );
    return( _commands );
}


void Node::addDrawable( jag::draw::DrawablePtr& drawable )
{
    _drawables.push_back( drawable );
    drawable->getNotifierCallbacks().push_back( _boundDirtyCallback );
    setBoundDirty();
}
int Node::removeDrawable( jag::draw::DrawablePtr& drawable )
{
    jag::draw::DrawableVec::iterator it;
    for( it=_drawables.begin(); it != _drawables.end(); ++it )
    {
        if( *it == drawable )
        {
            drawable->removeNotifierCallback( _boundDirtyCallback );
            _drawables.erase( it );
            setBoundDirty();
            return( (int)( _drawables.size() ) );
        }
    }
    return( -1 );
}
int Node::removeDrawable( const unsigned int index )
{
    jag::draw::DrawablePtr drawable( getDrawable( index ) );
    if( drawable != NULL )
    {
        return( removeDrawable( drawable ) );
    }
    return( -1 );
}
unsigned int Node::getNumDrawables() const
{
    return( (unsigned int) _drawables.size() );
}
jag::draw::DrawablePtr& Node::getDrawable( const unsigned int idx )
{
    if( idx >= getNumDrawables() )
    {
        JAG3D_ERROR( "getDrawable() index out of range." );
    }
    return( _drawables[ idx ] );
}
const jag::draw::DrawablePtr& Node::getDrawable( const unsigned int idx ) const
{
    if( idx >= getNumDrawables() )
    {
        JAG3D_ERROR( "getDrawable() index out of range." );
    }
    return( _drawables[ idx ] );
}

    
void Node::addChild( NodePtr& node )
{
    if( node == NULL )
    {
        JAG3D_NOTICE( "addChild(): Attempt to add NULL child." );
        return;
    }

    _children.push_back( node );
    node->addParent( this );
    setBoundDirty();
}
int Node::removeChild( const NodePtr& node )
{
    NodeVec::iterator it;
    for( it = _children.begin(); it != _children.end(); ++it )
    {
        if( *it == node )
        {
            node->removeParent( this );
            _children.erase( it );
            setBoundDirty();
            return( (int)( _children.size() ) );
        }
    }
    return( -1 );
}
int Node::removeChild( const unsigned int index )
{
    NodePtr node( getChild( index ) );
    if( node != NULL )
    {
        return( removeChild( node ) );
    }
    return( -1 );
}
unsigned int Node::getNumChildren() const
{
    return( (unsigned int) _children.size() );
}
NodePtr& Node::getChild( const unsigned int idx )
{
    if( idx >= getNumChildren() )
    {
        JAG3D_ERROR( "getChild() index out of range." );
    }
    return( _children[ idx ] );
}
const NodePtr& Node::getChild( const unsigned int idx ) const
{
    if( idx >= getNumChildren() )
    {
        JAG3D_ERROR( "getChild() index out of range." );
    }
    return( _children[ idx ] );
}

void Node::addParent( Node* node )
{
    _parents.push_back( node );
}
int Node::removeParent( const Node* node )
{
    NodeSimplePtrVec::iterator it;
    for( it = _parents.begin(); it != _parents.end(); ++it )
    {
        if( *it == node )
        {
            _parents.erase( it );
            return( (int)( _parents.size() ) );
        }
    }
    return( -1 );
}
unsigned int Node::getNumParents() const
{
    return( (unsigned int) _parents.size() );
}
Node* Node::getParent( const unsigned int idx )
{
    if( idx >= getNumParents() )
    {
        JAG3D_ERROR( "getParent() index out of range." );
    }
    return( _parents[ idx ] );
}
const Node* Node::getParent( const unsigned int idx ) const
{
    if( idx >= getNumParents() )
    {
        JAG3D_ERROR( "getParent() index out of range." );
    }
    return( _parents[ idx ] );
}
const NodeSimplePtrVec& Node::getParents() const
{
    return( _parents );
}




void Node::setMaxContexts( const unsigned int numContexts )
{
    if( _commands != NULL )
        _commands->setMaxContexts( numContexts );

    BOOST_FOREACH( jag::draw::DrawablePtr& drawable, _drawables )
    {
        drawable->setMaxContexts( numContexts );
    }

    BOOST_FOREACH( NodePtr& node, _children )
    {
        node->setMaxContexts( numContexts );
    }
}
void Node::deleteID( const jag::draw::jagDrawContextID contextID )
{
    if( _commands != NULL )
        _commands->deleteID( contextID );

    BOOST_FOREACH( jag::draw::DrawablePtr& drawable, _drawables )
    {
        drawable->deleteID( contextID );
    }

    BOOST_FOREACH( NodePtr& node, _children )
    {
        node->deleteID( contextID );
    }
}



void Node::BoundDirtyCallback::operator()( jag::base::Notifier* notifier, const jag::base::Notifier::NotifierInfo& info )
{
    JAG3D_CRITICAL_STATIC( "jag.node.bounddirty", "I was notified." );

    const jag::draw::Drawable::BoundDirtyNotifyInfo* bdni( dynamic_cast<
        const jag::draw::Drawable::BoundDirtyNotifyInfo* >( &info ) );
    if( bdni != NULL )
    {
        _owner->setBoundDirty();
    }
}


// namespace jag::sg::
}
}
