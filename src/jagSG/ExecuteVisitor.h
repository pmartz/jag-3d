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

#ifndef __JAGSG_EXECUTE_VISITOR_H__
#define __JAGSG_EXECUTE_VISITOR_H__ 1

#include <jagSG/Export.h>
#include <jagSG/Visitor.h>
#include <jagBase/Transform.h>
#include <jagDraw/Uniform.h>
#include <jagBase/ptr.h>


namespace jagSG {


/** \class ExecuteVisitor ExecuteVisitor.h <jagSG/ExecuteVisitor.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT ExecuteVisitor : public Visitor
{
public:
    ExecuteVisitor( jagDraw::DrawInfo& drawInfo );
    ExecuteVisitor( jagDraw::DrawInfo& drawInfo, jagSG::Node& node );
    ExecuteVisitor( const ExecuteVisitor& rhs );
    virtual ~ExecuteVisitor();


    /** \brief TBD
    \details TBD */
    void setViewProj( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );

    /** \brief TBD
    \details TBD */
    void reset();

    /** \brief TBD
    \details TBD */
    virtual void visit( jagSG::Node& node );

protected:
    void updateTransformUniforms();

    jagDraw::DrawInfo& _drawInfo;
    jagBase::TransformD _transform;
};

typedef jagBase::ptr< jagSG::ExecuteVisitor >::shared_ptr ExecuteVisitorPtr;


// jagSG
}


// __JAGSG_EXECUTE_VISITOR_H__
#endif
