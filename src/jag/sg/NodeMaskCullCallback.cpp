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

#include <jag/sg/NodeMaskCullCallback.h>
#include <jag/sg/Node.h>
#include <jag/sg/Visitor.h>
#include <jag/sg/CollectionVisitor.h>


namespace jag {
namespace sg {


NodeMaskCullCallback::NodeMaskCullCallback( const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.sg.coll.nodemask" : logName ),
    jag::sg::Node::Callback(),
    _override( OVERRIDE_OFF )
{
}
NodeMaskCullCallback::NodeMaskCullCallback( const NodeMaskCullCallback& rhs )
  : jag::base::LogBase( "jag.sg.coll.nodemask" ),
    jag::sg::Node::Callback( rhs ),
    _override( rhs._override )
{
}
NodeMaskCullCallback::~NodeMaskCullCallback()
{
}


bool NodeMaskCullCallback::operator()( jag::sg::VisitorBase* /* visitor */, jag::sg::Node::CallbackInfo* info )
{
    if( _override == OVERRIDE_OFF )
    {
        jag::sg::CollectionVisitor::CollectionInfo* ci( static_cast<
            jag::sg::CollectionVisitor::CollectionInfo* >( info ) );

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
    : jag::sg::VisitorBase( "frustum" ),
      _cb( NodeMaskCullCallbackPtr( new NodeMaskCullCallback() ) )
{
}
NodeMaskCullDistributionVisitor::NodeMaskCullDistributionVisitor( const NodeMaskCullDistributionVisitor& rhs )
    : jag::sg::VisitorBase( rhs ),
      _cb( rhs._cb )
{
}
NodeMaskCullDistributionVisitor::~NodeMaskCullDistributionVisitor()
{
}

void NodeMaskCullDistributionVisitor::visit( jag::sg::Node& node )
{
    node.getCollectionCallbacks().push_back( _cb );

    node.traverse( *this );
}


// namespace jag::sg::
}
}
