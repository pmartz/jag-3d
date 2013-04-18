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

#include <jagDraw/NodeContainer.h>
#include <jagDraw/Node.h>
#include <jagDraw/Error.h>
#include <jagBase/Profile.h>
#include <jagBase/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


NodeContainer::NodeContainer()
  : jagBase::LogBase( "jag.draw.ncon" ),
    ObjectIDOwner()
{
}
NodeContainer::NodeContainer( const NodeContainer& rhs )
  : jagBase::LogBase( "jag.draw.ncon" ),
    ObjectIDOwner( rhs ),
    _callbacks( rhs._callbacks )
{
}
NodeContainer::~NodeContainer()
{
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

    executeDrawNodes( drawInfo );

    JAG3D_ERROR_CHECK( "NodeContainer::execute()" );
}
void NodeContainer::executeDrawNodes( DrawInfo& drawInfo )
{
    BOOST_FOREACH( Node& node, *this )
    {
        node.execute( drawInfo );
    }
}

void NodeContainer::setMaxContexts( const unsigned int numContexts )
{
    BOOST_FOREACH( Node& node, *this )
    {
        node.setMaxContexts( numContexts );
    }
}
void NodeContainer::deleteID( const jagDraw::jagDrawContextID contextID )
{
    BOOST_FOREACH( Node& node, *this )
    {
        node.deleteID( contextID );
    }
}


// jagDraw
}
