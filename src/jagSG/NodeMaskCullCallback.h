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

#ifndef __JAGSG_NODEMASK_CULL_CALLBACK_H__
#define __JAGSG_NODEMASK_CULL_CALLBACK_H__ 1

#include <jagSG/Export.h>
#include <jagSG/Node.h>
#include <jagSG/Visitor.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>



namespace jagSG {


/** \addtogroup jagSGCollection CollectionVisitor and Controls */
/*@{*/

/** \class NodeMaskCullCallback NodeMaskCullCallback.h <jagSG/NodeMaskCullCallback.h>
\brief Culls based on the NodeMask stored in a Node.
\details TBD
*/
class JAGSG_EXPORT NodeMaskCullCallback : protected jagBase::LogBase,
        public jagSG::Node::Callback
{
public:
    NodeMaskCullCallback( const std::string& logName=std::string( "" ) );
    NodeMaskCullCallback( const NodeMaskCullCallback& rhs );
    ~NodeMaskCullCallback();

    /** \brief returns false if culled and true if not culled
    \details TBD */
    virtual bool operator()( jagSG::VisitorBase* visitor, jagSG::Node::CallbackInfo* info );


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

typedef jagBase::ptr< NodeMaskCullCallback >::shared_ptr NodeMaskCullCallbackPtr;



/** \class NodeMaskCullDistributionVisitor NodeMaskCullCallback.h <jagSG/NodeMaskCullCallback.h>
\brief Distributes a NodeMaskCullCallback to the specified subgraph
\details TBD
*/
class JAGSG_EXPORT NodeMaskCullDistributionVisitor : public jagSG::VisitorBase
{
public:
    NodeMaskCullDistributionVisitor();
    NodeMaskCullDistributionVisitor( const NodeMaskCullDistributionVisitor& rhs );
    ~NodeMaskCullDistributionVisitor();

    virtual void visit( jagSG::Node& node );

protected:
    NodeMaskCullCallbackPtr _cb;
};


/*@}*/


// jagSG
}


// __JAGSG_NODEMASK_CULL_CALLBACK_H__
#endif
