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

#ifndef __JAGSG_SMALL_FEATURE_CALLBACK_H__
#define __JAGSG_SMALL_FEATURE_CALLBACK_H__ 1

#include <jagSG/Export.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>



namespace jagSG {


/** \addtogroup jagSGCollection CollectionVisitor and Controls */
/*@{*/

/** \class SmallFeatureCallback SmallFeatureCallback.h <jagSG/SmallFeatureCallback.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT SmallFeatureCallback : protected jagBase::LogBase,
        public jagSG::Node::Callback
{
public:
    SmallFeatureCallback( const std::string& logName=std::string( "" ) );
    SmallFeatureCallback( const SmallFeatureCallback& rhs );
    ~SmallFeatureCallback();

    /** \brief TBD
    \details TBD */
    virtual bool operator()( jagSG::VisitorBase* visitor, jagSG::Node::CallbackInfo* info );

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

typedef jagBase::ptr< SmallFeatureCallback >::shared_ptr SmallFeatureCallbackPtr;



/** \class SmallFeatureDistributionVisitor SmallFeatureCallback.h <jagSG/SmallFeatureCallback.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT SmallFeatureDistributionVisitor : public jagSG::VisitorBase
{
public:
    SmallFeatureDistributionVisitor();
    SmallFeatureDistributionVisitor( const SmallFeatureDistributionVisitor& rhs );
    ~SmallFeatureDistributionVisitor();

    virtual void visit( jagSG::Node& node );

protected:
    SmallFeatureCallbackPtr _cb;
};


/*@}*/


// jagSG
}


// __JAGSG_SMALL_FEATURE_CALLBACK_H__
#endif
