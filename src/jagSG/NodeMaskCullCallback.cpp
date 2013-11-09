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
