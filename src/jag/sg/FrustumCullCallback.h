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

#ifndef __JAG_SG_FRUSTUM_CULL_CALLBACK_H__
#define __JAG_SG_FRUSTUM_CULL_CALLBACK_H__ 1

#include <jag/sg/Export.h>
#include <jag/sg/Node.h>
#include <jag/sg/Visitor.h>
#include <jag/base/LogBase.h>
#include <jag/base/ptr.h>



namespace jag {
namespace sg {


/** \addtogroup jagSGCollection CollectionVisitor and Controls */
/*@{*/

/** \class FrustumCullCallback FrustumCullCallback.h <jag/sg/FrustumCullCallback.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT FrustumCullCallback : protected jag::base::LogBase,
        public jag::sg::Node::Callback
{
public:
    FrustumCullCallback( const std::string& logName=std::string( "" ) );
    FrustumCullCallback( const FrustumCullCallback& rhs );
    ~FrustumCullCallback();

    /** \brief TBD
    \details TBD */
    virtual bool operator()( jag::sg::VisitorBase* visitor, jag::sg::Node::CallbackInfo* info );

protected:
};

typedef jag::base::ptr< FrustumCullCallback >::shared_ptr FrustumCullCallbackPtr;



/** \class FrustumCullDistributionVisitor FrustumCullCallback.h <jag/sg/FrustumCullCallback.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT FrustumCullDistributionVisitor : public jag::sg::VisitorBase
{
public:
    FrustumCullDistributionVisitor();
    FrustumCullDistributionVisitor( const FrustumCullDistributionVisitor& rhs );
    ~FrustumCullDistributionVisitor();

    virtual void visit( jag::sg::Node& node );

protected:
    FrustumCullCallbackPtr _cb;
};


/*@}*/


// namespace jag::sg::
}
}


// __JAG_SG_FRUSTUM_CULL_CALLBACK_H__
#endif
