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

#ifndef __JAGSG_FRUSTUM_CULL_CALLBACK_H__
#define __JAGSG_FRUSTUM_CULL_CALLBACK_H__ 1

#include <jagSG/Export.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>



namespace jagSG {


/** \class FrustumCullCallback FrustumCullCallback.h <jagSG/FrustumCullCallback.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT FrustumCullCallback : protected jagBase::LogBase,
        public jagSG::Node::Callback
{
public:
    FrustumCullCallback();
    FrustumCullCallback( const FrustumCullCallback& rhs );
    ~FrustumCullCallback();

    /** \brief TBD
    \details TBD */
    virtual bool operator()( jagSG::VisitorBase* visitor, jagSG::Node::CallbackInfo* info );

protected:
};

typedef jagBase::ptr< FrustumCullCallback >::shared_ptr FrustumCullCallbackPtr;



/** \class FrustumCullDistributionVisitor FrustumCullCallback.h <jagSG/FrustumCullCallback.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT FrustumCullDistributionVisitor : public jagSG::VisitorBase
{
public:
    FrustumCullDistributionVisitor();
    FrustumCullDistributionVisitor( const FrustumCullDistributionVisitor& rhs );
    ~FrustumCullDistributionVisitor();

    virtual void visit( jagSG::Node& node );

protected:
    FrustumCullCallbackPtr _cb;
};


// jagSG
}


// __JAGSG_FRUSTUM_CULL_CALLBACK_H__
#endif
