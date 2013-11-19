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

#include <jagSG/FrustumCullCallback.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagSG/CollectionVisitor.h>
#include <jagBase/Profile.h>


namespace jagSG {


FrustumCullCallback::FrustumCullCallback( const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.sg.coll.frustum" : logName ),
    jagSG::Node::Callback()
{
}
FrustumCullCallback::FrustumCullCallback( const FrustumCullCallback& rhs )
  : jagBase::LogBase( "jag.sg.coll.frustum" ),
    jagSG::Node::Callback( rhs )
{
}
FrustumCullCallback::~FrustumCullCallback()
{
}


bool FrustumCullCallback::operator()( jagSG::VisitorBase* /* visitor */, jagSG::Node::CallbackInfo* info )
{
    JAG3D_PROFILE( "frustum" );

    jagSG::CollectionVisitor::CollectionInfo* ci( static_cast<
        jagSG::CollectionVisitor::CollectionInfo* >( info ) );

    return( ci->isContained() );
}




FrustumCullDistributionVisitor::FrustumCullDistributionVisitor()
    : jagSG::VisitorBase( "frustum" ),
      _cb( FrustumCullCallbackPtr( new FrustumCullCallback() ) )
{
}
FrustumCullDistributionVisitor::FrustumCullDistributionVisitor( const FrustumCullDistributionVisitor& rhs )
    : jagSG::VisitorBase( rhs ),
      _cb( rhs._cb )
{
}
FrustumCullDistributionVisitor::~FrustumCullDistributionVisitor()
{
}

void FrustumCullDistributionVisitor::visit( jagSG::Node& node )
{
    node.getCollectionCallbacks().push_back( _cb );

    node.traverse( *this );
}


// jagSG
}
