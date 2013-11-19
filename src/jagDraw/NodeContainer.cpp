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

#include <jagDraw/NodeContainer.h>
#include <jagDraw/Node.h>
#include <jagDraw/Error.h>
#include <jagBase/Profile.h>
#include <jagBase/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


NodeContainer::NodeContainer( const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.draw.ncon" : logName ),
    ObjectIDOwner(),
    _resetEnable( true )
{
}
NodeContainer::NodeContainer( const NodeContainer& rhs )
  : DrawNodeVec( rhs ),
    jagBase::LogBase( "jag.draw.ncon" ),
    ObjectIDOwner( rhs ),
    _callbacks( rhs._callbacks ),
    _resetEnable( rhs._resetEnable )
{
}
NodeContainer::~NodeContainer()
{
}

void NodeContainer::reset()
{
    if( _resetEnable )
        clear();
}
void NodeContainer::setResetEnable( const bool enable )
{
    _resetEnable = enable;
}
bool NodeContainer::getResetEnable() const
{
    return( _resetEnable );
}

NodeContainer& NodeContainer::operator=( const NodeContainer& rhs )
{
    DrawNodeVec::operator=( rhs );
    ObjectIDOwner::operator=( rhs );
    // LogBase does not support (and doesn't need) assignment operator.
    //jagBase::LogBase::operator=( rhs );
    _callbacks = rhs._callbacks;
    _resetEnable = rhs._resetEnable;
    return( *this );
}


NodeContainer::CallbackVec& NodeContainer::getCallbacks()
{
    return( _callbacks );
}

void NodeContainer::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    JAG3D_PROFILE( "NodeContainer execute()" );

    BOOST_FOREACH( CallbackPtr cb, _callbacks )
    {
        if( !( (*cb)( *this, drawInfo ) ) )
            return;
    }

    internalExecute( drawInfo );

    JAG3D_ERROR_CHECK( "NodeContainer::execute()" );
}
void NodeContainer::internalExecute( DrawInfo& drawInfo )
{
    BOOST_FOREACH( DrawNodePtr& node, *this )
    {
        node->execute( drawInfo );
    }
}

void NodeContainer::setMaxContexts( const unsigned int numContexts )
{
    BOOST_FOREACH( DrawNodePtr& node, *this )
    {
        node->setMaxContexts( numContexts );
    }
}
void NodeContainer::deleteID( const jagDraw::jagDrawContextID contextID )
{
    BOOST_FOREACH( DrawNodePtr& node, *this )
    {
        node->deleteID( contextID );
    }
}


// jagDraw
}
