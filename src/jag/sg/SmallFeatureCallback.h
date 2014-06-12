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

#ifndef __JAG_SG_SMALL_FEATURE_CALLBACK_H__
#define __JAG_SG_SMALL_FEATURE_CALLBACK_H__ 1

#include <jag/sg/Export.h>
#include <jag/sg/Node.h>
#include <jag/sg/Visitor.h>
#include <jag/base/LogBase.h>
#include <jag/base/ptr.h>



namespace jag {
namespace sg {


/** \addtogroup jagSGCollection CollectionVisitor and Controls */
/*@{*/

/** \class SmallFeatureCallback SmallFeatureCallback.h <jag/sg/SmallFeatureCallback.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT SmallFeatureCallback : protected jag::base::LogBase,
        public jag::sg::Node::Callback
{
public:
    SmallFeatureCallback( const std::string& logName=std::string( "" ) );
    SmallFeatureCallback( const SmallFeatureCallback& rhs );
    ~SmallFeatureCallback();

    /** \brief TBD
    \details TBD */
    virtual bool operator()( jag::sg::VisitorBase* visitor, jag::sg::Node::CallbackInfo* info );

    /** \brief Set the screen space radius threshold.
    \details Sets the threshold used by operator()() to determine whether
    a bound passes the callback or not. A Node or Drawable will not be
    collected by the CollectionVisitor if it's screen space radius is
    smaller than this threshold value. Default: 2 pixel radius. */
    void setThreshold( const double threshold );
    /** \brief Get the screen space radius threshold. */
    double getThreshold() const;

protected:
    double _threshold;
};

typedef jag::base::ptr< SmallFeatureCallback >::shared_ptr SmallFeatureCallbackPtr;



/** \class SmallFeatureDistributionVisitor SmallFeatureCallback.h <jag/sg/SmallFeatureCallback.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT SmallFeatureDistributionVisitor : public jag::sg::VisitorBase
{
public:
    SmallFeatureDistributionVisitor();
    SmallFeatureDistributionVisitor( const SmallFeatureDistributionVisitor& rhs );
    ~SmallFeatureDistributionVisitor();

    virtual void visit( jag::sg::Node& node );

protected:
    SmallFeatureCallbackPtr _cb;
};


/*@}*/


// namespace jag::sg::
}
}


// __JAG_SG_SMALL_FEATURE_CALLBACK_H__
#endif
