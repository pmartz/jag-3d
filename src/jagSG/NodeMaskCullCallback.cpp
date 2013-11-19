/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
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

#include <jagSG/NodeMaskCullCallback.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagSG/CollectionVisitor.h>


namespace jagSG {


NodeMaskCullCallback::NodeMaskCullCallback( const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.sg.coll.nodemask" : logName ),
    jagSG::Node::Callback(),
    _override( OVERRIDE_OFF )
{
}
NodeMaskCullCallback::NodeMaskCullCallback( const NodeMaskCullCallback& rhs )
  : jagBase::LogBase( "jag.sg.coll.nodemask" ),
    jagSG::Node::Callback( rhs ),
    _override( rhs._override )
{
}
NodeMaskCullCallback::~NodeMaskCullCallback()
{
}


bool NodeMaskCullCallback::operator()( jagSG::VisitorBase* /* visitor */, jagSG::Node::CallbackInfo* info )
{
    if( _override == OVERRIDE_OFF )
    {
        jagSG::CollectionVisitor::CollectionInfo* ci( static_cast<
            jagSG::CollectionVisitor::CollectionInfo* >( info ) );

        return( ci->getNode()->getNodeMask() );
    }
    else
    {
        return( _override == OVERRIDE_TRUE );
    }
}

void NodeMaskCullCallback::setOverride( const OverrideMode override )
{
    _override = override;
}
NodeMaskCullCallback::OverrideMode NodeMaskCullCallback::getOverride() const
{
    return( _override );
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
