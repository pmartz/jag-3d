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

#ifndef __JAG_SG_INTERSECT_VISITOR_H__
#define __JAG_SG_INTERSECT_VISITOR_H__ 1

#include <jag/sg/Export.h>
#include <jag/sg/Visitor.h>
#include <jag/base/Transform.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/DrawNodeContainer.h>
#include <jag/draw/DrawGraph.h>
#include <jag/draw/TransformCallback.h>
#include <jag/draw/Drawable.h>
#include <jag/draw/Uniform.h>
#include <jag/base/ptr.h>
#include <gmtl/Ray.h>
#include <gmtl/Xforms.h>

#include <deque>


namespace jag {
namespace sg {


    struct hitRecord {
        jag::sg::NodeVec nodeVec;
        jag::draw::DrawablePtr drawablePtr;
        gmtl::Point3f hitPosition;
        jag::draw::VertexArrayObjectPtr vaop;
        int tri;
        jag::draw::DrawCommandPtr dcp;
        float u, v;

        //how do we decide type for the world coordinate point


    };

/** \class IntersectVisitor IntersectVisitor.h <jag/sg/IntersectVisitor.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT IntersectVisitor : public Visitor
{
public:
    IntersectVisitor();
    IntersectVisitor( jag::sg::NodePtr node, gmtl::Ray<double> ray );
    IntersectVisitor( const IntersectVisitor& rhs );
    virtual ~IntersectVisitor();


   

    /** \brief TBD
    \details TBD */
    void reset();


    /** \brief TBD
    \details TBD */
    virtual void visit( jag::sg::Node& node ) {
        CommandMapStackHelper cmdh( *this, node.getCommandMap() );
        MatrixStackHelper msh( *this, node.getTransform() );
        intersect(node);
        checkMaskAndTraverse(node);
    }


    std::vector<hitRecord> getHits() {
        return hits;
    }
    

    


    

    

protected:
    
    void checkMaskAndTraverse(jag::sg::Node& node) {
    }
    void pushRay( gmtl::Ray<double> );
    void popRay();
    void intersect(jag::sg::Node& node);
    unsigned int _currentID;
    

    jag::base::TransformD _transform;
    std::vector<hitRecord> hits;
    std::deque< gmtl::Ray<double> > _rayDeque;
    gmtl::Ray<double> currentRay;
    
};

typedef jag::base::ptr< jag::sg::IntersectVisitor >::shared_ptr IntersectVisitorPtr;


// namespace jag::sg::
}
}


// __JAG_SG_COLLECTION_VISITOR_H__
#endif
