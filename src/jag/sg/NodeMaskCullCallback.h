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

#ifndef __JAG_SG_NODEMASK_CULL_CALLBACK_H__
#define __JAG_SG_NODEMASK_CULL_CALLBACK_H__ 1

#include <jag/sg/Export.h>
#include <jag/sg/Node.h>
#include <jag/sg/Visitor.h>
#include <jag/base/LogBase.h>
#include <jag/base/ptr.h>



namespace jag {
namespace sg {


/** \addtogroup jagSGCollection CollectionVisitor and Controls */
/*@{*/

/** \class NodeMaskCullCallback NodeMaskCullCallback.h <jag/sg/NodeMaskCullCallback.h>
\brief Culls based on the NodeMask stored in a Node.
\details TBD
*/
class JAGSG_EXPORT NodeMaskCullCallback : protected jag::base::LogBase,
        public jag::sg::Node::Callback
{
public:
    NodeMaskCullCallback( const std::string& logName=std::string( "" ) );
    NodeMaskCullCallback( const NodeMaskCullCallback& rhs );
    ~NodeMaskCullCallback();

    /** \brief returns false if culled and true if not culled
    \details TBD */
    virtual bool operator()( jag::sg::VisitorBase* visitor, jag::sg::Node::CallbackInfo* info );


    typedef enum {
        OVERRIDE_TRUE, ///< operator() always returns true. */
        OVERRIDE_FALSE, ///< operator() always returns false. */
        OVERRIDE_OFF ///< operator() uses the UserData node mask. */
    } OverrideMode;
    /** \brief Override the UserData node mask to force an enable or disable.
    \details Default value: OVERRIDE_OFF */
    void setOverride( const OverrideMode override );
    /** \brief Get the current override mode. */
    OverrideMode getOverride() const;

protected:
    OverrideMode _override;
};

typedef jag::base::ptr< NodeMaskCullCallback >::shared_ptr NodeMaskCullCallbackPtr;



/** \class NodeMaskCullDistributionVisitor NodeMaskCullCallback.h <jag/sg/NodeMaskCullCallback.h>
\brief Distributes a NodeMaskCullCallback to the specified subgraph
\details TBD
*/
class JAGSG_EXPORT NodeMaskCullDistributionVisitor : public jag::sg::VisitorBase
{
public:
    NodeMaskCullDistributionVisitor();
    NodeMaskCullDistributionVisitor( const NodeMaskCullDistributionVisitor& rhs );
    ~NodeMaskCullDistributionVisitor();

    virtual void visit( jag::sg::Node& node );

protected:
    NodeMaskCullCallbackPtr _cb;
};


/*@}*/


// namespace jag::sg::
}
}


// __JAG_SG_NODEMASK_CULL_CALLBACK_H__
#endif
