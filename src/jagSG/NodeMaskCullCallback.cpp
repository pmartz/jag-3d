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

#include <jagSG/NodeMaskCullCallback.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagSG/CollectionVisitor.h>


namespace jagSG {


NodeMaskCullCallback::NodeMaskCullCallback( const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.sg.coll.nodemask" : logName ),
    jagSG::Node::Callback()
{
}
NodeMaskCullCallback::NodeMaskCullCallback( const NodeMaskCullCallback& rhs )
  : jagBase::LogBase( "jag.sg.coll.nodemask" ),
    jagSG::Node::Callback( rhs )
{
}
NodeMaskCullCallback::~NodeMaskCullCallback()
{
}


bool NodeMaskCullCallback::operator()( jagSG::VisitorBase* /* visitor */, jagSG::Node::CallbackInfo* info )
{
    jagSG::CollectionVisitor::CollectionInfo* ci( static_cast<
        jagSG::CollectionVisitor::CollectionInfo* >( info ) );

    return( ci->getNode()->getNodeMask() );
}




NodeMaskCullDistributionVisitor::NodeMaskCullDistributionVisitor()
    : jagSG::VisitorBase( "frustum" ),
      _cb( NodeMaskCullCallbackPtr( new NodeMaskCullCallback() ) )
{
}
NodeMaskCullDistributionVisitor::NodeMaskCullDistributionVisitor( const NodeMaskCullDistributionVisitor& rhs )
    : jagSG::VisitorBase( rhs ),
      _cb( rhs._cb )
{
}
NodeMaskCullDistributionVisitor::~NodeMaskCullDistributionVisitor()
{
}

void NodeMaskCullDistributionVisitor::visit( jagSG::Node& node )
{
    node.getCollectionCallbacks().push_back( _cb );

    node.traverse( *this );
}


// jagSG
}
