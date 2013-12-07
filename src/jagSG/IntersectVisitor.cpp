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

#include <jagSG/IntersectVisitor.h>
#include <jagSG/Node.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/BufferObject.h>
#include <jag/draw/VertexAttrib.h>
#include <jag/base/gmtlSupport.h>
#include <jag/draw/Error.h>
#include <jag/base/Profile.h>
#include <jag/base/LogMacros.h>
#include <boost/foreach.hpp>
#include <jag/draw/DrawCommand.h>
#include <jag/draw/Drawable.h>
#include <gmtl/gmtl.h>
#include <jag/draw/TriangleSurfer.h>
#include <gmtl/Xforms.h>
#include <cfloat>
#include <gmtl/Ray.h>
#include <gmtl/Intersection.h>
#include <gmtl/Vec.h>



using namespace gmtl;

/**
    * Tests if the given triangle and ray intersect with each other. Adapted from GMTL.
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
        
    
    if(_rayDeque.size() <1)  
        _rayDeque.push_back(currentRay);
    else
        _rayDeque.push_back(_rayDeque.back());

        //2. transform the current ray by the inverse of the transform
        gmtl::Matrix44d tMat = node.getTransform();
        gmtl::xform(_rayDeque.back(), gmtl::invert(tMat), _rayDeque.back());
        gmtl::Rayf rayf;

        //copy the transformed ray to a float version of the same ray
        rayf.mDir[0] = (float)( _rayDeque.back().mDir[0] );
        rayf.mDir[1] = (float)( _rayDeque.back().mDir[1] );
        rayf.mDir[2] = (float)( _rayDeque.back().mDir[2] );

        rayf.mOrigin[0] = (float)( _rayDeque.back().mOrigin[0] );
        rayf.mOrigin[1] = (float)( _rayDeque.back().mOrigin[1] );
        rayf.mOrigin[2] = (float)( _rayDeque.back().mOrigin[2] );

        int oldNumHits = (int)( hits.size() );
    
        node.traverse(*this);
        if(node.getNumDrawables() > 0) {
        
         const jag::draw::DrawablePrepPtr& drawablePrep( _commandStack.back()[ jag::draw::Command::VertexArrayObject_t ] );
         const jag::draw::VertexArrayObjectPtr vaop( boost::static_pointer_cast< jag::draw::VertexArrayObject >( drawablePrep ) );
        

          jag::draw::BufferObjectPtr bop( boost::dynamic_pointer_cast< jag::draw::BufferObject >(
        vaop->getVertexArrayCommand( jag::draw::VertexArrayCommand::BufferObject_t, jag::draw::VertexArrayObject::Vertex ) ) );
           
    jag::draw::VertexAttribPtr verts( boost::dynamic_pointer_cast< jag::draw::VertexAttrib >(
        vaop->getVertexArrayCommand( jag::draw::VertexArrayCommand::VertexAttrib_t, jag::draw::VertexArrayObject::Vertex ) ) );
        //3. intersect the triangles with the ray and record the hits
	bool boundIntersect;
	if(node.getBound()->getType()==jag::draw::Bound::Box_t)  {
		double a, b;
		boundIntersect = gmtl::intersectAABoxRay<double>(node.getBound()->asAABox(), _rayDeque.back(), a, b);
	}
	else
	{
		double a,b;
		int nh;
		boundIntersect = gmtl::intersectVolume<double>(node.getBound()->asSphere(), _rayDeque.back(), nh, a, b);
	}

	
	if(boundIntersect)
	{
		//std::cout << node.getUserDataName() << "NODENAME" << std::endl;
		//if(node.getUserDataName() != "") {
		//	numNamed++;
		//}
        for(auto i = 0; i < node.getNumDrawables(); i++) {
            jag::draw::DrawCommandVec drawCommands = node.getDrawable(i)->getDrawCommandVec();
            BOOST_FOREACH( jag::draw::DrawCommandPtr dcp, drawCommands )
            {
                jag::draw::TriangleSurfer<gmtl::Point3f> ts(bop,verts,dcp);
                Point3f  *a, *b, *c;
                double u, v, t;
               
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
                        hit.u = (float)( u );
                        hit.v = (float)( v );
                        hits.push_back(hit);
                    }
                }
                
            }
		}
		}
        
        }
        


        //4. pass the transformed ray to all children
        //in this case the recursion is handle by the ray stack and not by an explicit passign parameter
        gmtl::Matrix44f matf;
        gmtl::convert( matf, node.getTransform() );
        
        //4. transform all hits(at this level or lower) by the transform
        for(auto i = oldNumHits; i < hits.size(); i++) {
                
            gmtl::xform(hits[i].hitPosition, matf, hits[i].hitPosition);
                
        }
    _rayDeque.pop_back();
}

void IntersectVisitor::reset()
{
    JAG3D_TRACE( "reset()" );
    
    resetCommandMap();
    resetMatrix();

    
}


}
