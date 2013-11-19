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

#ifndef __JAGSG_INTERSECT_VISITOR_H__
#define __JAGSG_INTERSECT_VISITOR_H__ 1

#include <jagSG/Export.h>
#include <jagSG/Visitor.h>
#include <jagBase/Transform.h>
#include <jagDraw/Node.h>
#include <jagDraw/NodeContainer.h>
#include <jagDraw/DrawGraph.h>
#include <jagDraw/TransformCallback.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/Uniform.h>
#include <jagBase/ptr.h>
#include <gmtl/Ray.h>
#include <gmtl/Xforms.h>

#include <deque>


namespace jagSG {


    struct hitRecord {
        jagSG::NodeVec nodeVec;
        jagDraw::DrawablePtr drawablePtr;
        gmtl::Point3f hitPosition;
        jagDraw::VertexArrayObjectPtr vaop;
        int tri;
        jagDraw::DrawCommandPtr dcp;
        float u, v;

        //how do we decide type for the world coordinate point


    };

/** \class IntersectVisitor IntersectVisitor.h <jagSG/IntersectVisitor.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT IntersectVisitor : public Visitor
{
public:
    IntersectVisitor();
    IntersectVisitor( jagSG::NodePtr node, gmtl::Ray<double> ray );
    IntersectVisitor( const IntersectVisitor& rhs );
    virtual ~IntersectVisitor();


   

    /** \brief TBD
    \details TBD */
    void reset();


    /** \brief TBD
    \details TBD */
    virtual void visit( jagSG::Node& node ) {
        CommandMapStackHelper cmdh( *this, node.getCommandMap() );
        MatrixStackHelper msh( *this, node.getTransform() );
        intersect(node);
        checkMaskAndTraverse(node);
    }


    std::vector<hitRecord> getHits() {
        return hits;
    }
    

    


    

    

protected:
    
    void checkMaskAndTraverse(jagSG::Node& node) {
    }
    void pushRay( gmtl::Ray<double> );
    void popRay();
    void intersect(jagSG::Node& node);
    unsigned int _currentID;
    

    jagBase::TransformD _transform;
    std::vector<hitRecord> hits;
    std::deque< gmtl::Ray<double> > _rayDeque;
    gmtl::Ray<double> currentRay;
    
};

typedef jagBase::ptr< jagSG::IntersectVisitor >::shared_ptr IntersectVisitorPtr;


// jagSG
}



// __JAGSG_COLLECTION_VISITOR_H__
#endif
