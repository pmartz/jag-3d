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

#include <jag/sg/FrustumCullCallback.h>
#include <jag/sg/Node.h>
#include <jag/sg/Visitor.h>
#include <jag/sg/CollectionVisitor.h>
#include <jag/base/Profile.h>


namespace jag {
namespace sg {


FrustumCullCallback::FrustumCullCallback( const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.sg.coll.frustum" : logName ),
    jag::sg::Node::Callback()
{
}
FrustumCullCallback::FrustumCullCallback( const FrustumCullCallback& rhs )
  : jag::base::LogBase( "jag.sg.coll.frustum" ),
    jag::sg::Node::Callback( rhs )
{
}
FrustumCullCallback::~FrustumCullCallback()
{
}


bool FrustumCullCallback::operator()( jag::sg::VisitorBase* /* visitor */, jag::sg::Node::CallbackInfo* info )
{
    JAG3D_PROFILE( "frustum" );

    jag::sg::CollectionVisitor::CollectionInfo* ci( static_cast<
        jag::sg::CollectionVisitor::CollectionInfo* >( info ) );

    return( ci->isContained() );
}




FrustumCullDistributionVisitor::FrustumCullDistributionVisitor()
    : jag::sg::VisitorBase( "frustum" ),
      _cb( FrustumCullCallbackPtr( new FrustumCullCallback() ) )
{
}
FrustumCullDistributionVisitor::FrustumCullDistributionVisitor( const FrustumCullDistributionVisitor& rhs )
    : jag::sg::VisitorBase( rhs ),
      _cb( rhs._cb )
{
}
FrustumCullDistributionVisitor::~FrustumCullDistributionVisitor()
{
}

void FrustumCullDistributionVisitor::visit( jag::sg::Node& node )
{
    node.getCollectionCallbacks().push_back( _cb );

    node.traverse( *this );
}


// namespace jag::sg::
}
}
