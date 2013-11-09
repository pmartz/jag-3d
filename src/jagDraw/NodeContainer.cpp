/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
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
