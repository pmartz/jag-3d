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


namespace jagSG {


	struct hitRecord {
		jagSG::NodeVec nodeVec;
		jagDraw::DrawablePtr drawablePtr;
		gmtl::Point3f hitPosition;

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
		pushCommandMap(node.getCommandMap());
		pushMatrix(node.getTransform());
		intersect(node);
		checkMaskAndTraverse(node);
		popMatrix();
		popCommandMap();
	
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
