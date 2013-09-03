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

#include <jagSG/IntersectVisitor.h>
#include <jagSG/Node.h>
#include <jagDraw/Node.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/VertexAttrib.h>
#include <jagBase/gmtlSupport.h>
#include <jagDraw/Error.h>
#include <jagBase/Profile.h>
#include <jagBase/LogMacros.h>
#include <boost/foreach.hpp>
#include <jagDraw/DrawCommand.h>
#include <jagDraw/Drawable.h>
#include <gmtl/gmtl.h>
#include <jagDraw/TriangleSurfer.h>
#include <gmtl/Xforms.h>
#include <cfloat>
#include <gmtl/Ray.h>
#include <gmtl/Intersection.h>
#include <gmtl/Vec.h>



using namespace gmtl;

/**
    * Tests if the given triangle and ray intersect with each other.
    *
    *  @param a,b,c - the triangle vertices (ccw ordering)
    *  @param ray - the ray
    *  @param u,v - tangent space u/v coordinates of the intersection
    *  @param t - an indicator of the intersection location 
    *  @post t gives you the intersection point:
    *         isect = ray.dir * t + ray.origin
    *  @return true if the ray intersects the triangle.
    *  @see from http://www.acm.org/jgt/papers/MollerTrumbore97/code.html
    */
   

   template<class DATA_TYPE>
   bool intersectLocal(Point<DATA_TYPE,3> &a, Point<DATA_TYPE,3> &b, Point<DATA_TYPE,3> &c, const Ray<DATA_TYPE>& ray,
                  double& u, double& v, double& t)
   {
      const float EPSILON = static_cast<DATA_TYPE>(0.00001);
      Vec<DATA_TYPE, 3> edge1, edge2, tvec, pvec, qvec;
      float det,inv_det;

      /* find vectors for two edges sharing vert0 */
      edge1 = b - a;
      edge2 = c - a;

      /* begin calculating determinant - also used to calculate U parameter */
      gmtl::cross( pvec, ray.getDir(), edge2 );

      /* if determinant is near zero, ray lies in plane of triangle */
      det = gmtl::dot(edge1, pvec);

      if (det < EPSILON)
      {
         return false;
      }

      /* calculate distance from vert0 to ray origin */
      tvec = ray.getOrigin() - a;

      /* calculate U parameter and test bounds */
      u = gmtl::dot(tvec, pvec);
      if (u < 0.0 || u > det)
      {
         return false;
      }

      /* prepare to test V parameter */
      gmtl::cross(qvec, tvec, edge1);

      /* calculate V parameter and test bounds */
      v = gmtl::dot( ray.getDir(), qvec );
      if (v < 0.0 || u + v > det)
      {
         return false;
      }

      /* calculate t, scale parameters, ray intersects triangle */
      t = gmtl::dot( edge2, qvec );
      inv_det = static_cast<DATA_TYPE>(1.0) / det;
      t *= inv_det;
      u *= inv_det;
      v *= inv_det;

      // test if t is within the ray boundary (when t >= 0)
      return t >= static_cast<DATA_TYPE>(0);
   }

namespace jagSG {
	

	
	


IntersectVisitor::IntersectVisitor( jagSG::NodePtr node, gmtl::Ray<double> ray )
  : Visitor( "Intersect" )
    
{
    reset();
	currentRay = ray;
    node->accept( *this );
}
IntersectVisitor::IntersectVisitor( const IntersectVisitor& rhs )
  : Visitor( rhs )
    
{
    

    reset();
}
IntersectVisitor::~IntersectVisitor()
{
}


void IntersectVisitor::intersect(jagSG::Node& node) {
		numNodes++;
	
	if(_rayDeque.size() <1)  
		_rayDeque.push_back(currentRay);
	else
		_rayDeque.push_back(_rayDeque.back());

		//2. transform the current ray by the inverse of the transform
		gmtl::Matrix44d tMat = node.getTransform();
		gmtl::xform(_rayDeque.back(), gmtl::invert(tMat), _rayDeque.back());
		gmtl::Rayf rayf;

		//copy the transformed ray to a float version of the same ray
		rayf.mDir[0] = _rayDeque.back().mDir[0];
		rayf.mDir[1] = _rayDeque.back().mDir[1];
		rayf.mDir[2] = _rayDeque.back().mDir[2];

		rayf.mOrigin[0] = _rayDeque.back().mOrigin[0];
		rayf.mOrigin[1] = _rayDeque.back().mOrigin[1];
		rayf.mOrigin[2] = _rayDeque.back().mOrigin[2];

		int oldNumHits = hits.size();
	
		node.traverse(*this);
		if(node.getNumDrawables() > 0) {
		
		 const jagDraw::DrawablePrepPtr& drawablePrep( _commandStack.back()[ jagDraw::DrawablePrep::VertexArrayObject_t ] );
	     const jagDraw::VertexArrayObjectPtr vaop( boost::static_pointer_cast< jagDraw::VertexArrayObject >( drawablePrep ) );
		

		  jagDraw::BufferObjectPtr bop( boost::dynamic_pointer_cast< jagDraw::BufferObject >(
        vaop->getVertexArrayCommand( jagDraw::VertexArrayCommand::BufferObject_t, jagDraw::VertexArrayObject::Vertex ) ) );
		   
    jagDraw::VertexAttribPtr verts( boost::dynamic_pointer_cast< jagDraw::VertexAttrib >(
        vaop->getVertexArrayCommand( jagDraw::VertexArrayCommand::VertexAttrib_t, jagDraw::VertexArrayObject::Vertex ) ) );
		//3. intersect the triangles with the ray and record the hits

	
		for(auto i = 0; i < node.getNumDrawables(); i++) {
			jagDraw::DrawCommandVec drawCommands = node.getDrawable(i)->getDrawCommandVec();
			BOOST_FOREACH( jagDraw::DrawCommandPtr dcp, drawCommands )
			{
				jagDraw::TriangleSurfer<gmtl::Point3f> ts(bop,verts,dcp);
				Point3f  *a, *b, *c;
				double u, v, t;
				numTriangles+=ts.getNumTriangles();
				numDrawables++;
				for(auto j = 0; j < ts.getNumTriangles(); j++) {
					
					ts.getTriangle(j, a, b, c);
				
					if(intersectLocal(*a, *b, *c,rayf, u, v, t)) {
						
						hitRecord hit;
						hit.drawablePtr = node.getDrawable(i);
						hit.dcp = dcp;
						hit.vaop = vaop;
						hit.tri = j;
						hit.nodeVec = this->getNodes();
						hit.hitPosition = rayf.getOrigin() + ((float)t)*rayf.getDir();
						hit.u = u;
						hit.v = v;
						hits.push_back(hit);
					}
				}
				
			}
		}
		
		}
		


		//4. pass the transformed ray to all children
		//in this case the recursion is handle by the ray stack and not by an explicit passign parameter
		gmtl::Matrix44f matf;
		for (auto i = 0; i < 16; i++) {

			matf.mData[i] = node.getTransform().mData[i];
		}
		
		//4. transform all hits(at this level or lower) by the transform
			for(auto i = oldNumHits; i < hits.size(); i++) {
				
				gmtl::xform(hits[i].hitPosition, matf, hits[i].hitPosition);
				
				
			}
		_rayDeque.pop_back();

		
		
}

void IntersectVisitor::reset()
{
    JAG3D_TRACE( "reset()" );
	numTriangles = 0;
	numNodes = 0;
	numDrawables = 0;
    resetCommandMap();
    resetMatrix();

    
}


}
