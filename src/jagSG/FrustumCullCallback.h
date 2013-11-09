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

#ifndef __JAGSG_FRUSTUM_CULL_CALLBACK_H__
#define __JAGSG_FRUSTUM_CULL_CALLBACK_H__ 1

#include <jagSG/Export.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>



namespace jagSG {


/** \addtogroup jagSGCollection CollectionVisitor and Controls */
/*@{*/

/** \class FrustumCullCallback FrustumCullCallback.h <jagSG/FrustumCullCallback.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT FrustumCullCallback : protected jagBase::LogBase,
        public jagSG::Node::Callback
{
public:
    FrustumCullCallback( const std::string& logName=std::string( "" ) );
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


/*@}*/


// jagSG
}


// __JAGSG_FRUSTUM_CULL_CALLBACK_H__
#endif
