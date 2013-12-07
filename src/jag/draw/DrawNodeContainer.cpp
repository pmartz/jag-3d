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

#include <jag/draw/DrawNodeContainer.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/Error.h>
#include <jag/base/Profile.h>
#include <jag/base/LogMacros.h>

#include <boost/foreach.hpp>


namespace jag {
namespace draw {


DrawNodeContainer::DrawNodeContainer( const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.draw.ncon" : logName ),
    ObjectIDOwner(),
    _resetEnable( true )
{
}
DrawNodeContainer::DrawNodeContainer( const DrawNodeContainer& rhs )
  : DrawNodeVec( rhs ),
    jag::base::LogBase( "jag.draw.ncon" ),
    ObjectIDOwner( rhs ),
    _callbacks( rhs._callbacks ),
    _resetEnable( rhs._resetEnable )
{
    // Note: Do not copy _nodeCache, cache of free draw Nodes.
}
DrawNodeContainer::~DrawNodeContainer()
{
}

void DrawNodeContainer::reset()
{
    if( _resetEnable )
    {
        BOOST_FOREACH( DrawNodePtr node, *this )
        {
            node->reset();
            _nodeCache.push_back( node );
        }
        clear();
    }
}
void DrawNodeContainer::setResetEnable( const bool enable )
{
    _resetEnable = enable;
}
bool DrawNodeContainer::getResetEnable() const
{
    return( _resetEnable );
}

DrawNodeContainer& DrawNodeContainer::operator=( const DrawNodeContainer& rhs )
{
    DrawNodeVec::operator=( rhs );
    ObjectIDOwner::operator=( rhs );
    // LogBase does not support (and doesn't need) assignment operator.
    //jag::base::LogBase::operator=( rhs );
    // Note: Do not copy _nodeCache, cache of free draw Nodes.
    _callbacks = rhs._callbacks;
    _resetEnable = rhs._resetEnable;
    return( *this );
}

jag::draw::DrawNodePtr& DrawNodeContainer::grow()
{
    if( _nodeCache.size() > 1 )
    {
        push_back( _nodeCache.back() );
        _nodeCache.pop_back();
    }
    else
        push_back( jag::draw::DrawNodePtr( new jag::draw::DrawNode() ) );
    return( back() );
}


DrawNodeContainer::CallbackVec& DrawNodeContainer::getCallbacks()
{
    return( _callbacks );
}

void DrawNodeContainer::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    JAG3D_PROFILE( "DrawNodeContainer execute()" );

    BOOST_FOREACH( CallbackPtr cb, _callbacks )
    {
        if( !( (*cb)( *this, drawInfo ) ) )
            return;
    }

    internalExecute( drawInfo );

    JAG3D_ERROR_CHECK( "DrawNodeContainer::execute()" );
}
void DrawNodeContainer::internalExecute( DrawInfo& drawInfo )
{
    BOOST_FOREACH( DrawNodePtr& node, *this )
    {
        node->execute( drawInfo );
    }
}

void DrawNodeContainer::setMaxContexts( const unsigned int numContexts )
{
    BOOST_FOREACH( DrawNodePtr& node, *this )
    {
        node->setMaxContexts( numContexts );
    }
}
void DrawNodeContainer::deleteID( const jag::draw::jagDrawContextID contextID )
{
    BOOST_FOREACH( DrawNodePtr& node, *this )
    {
        node->deleteID( contextID );
    }
}


// namespace jag::draw::
}
}
