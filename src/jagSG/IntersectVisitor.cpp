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
        
         const jagDraw::DrawablePrepPtr& drawablePrep( _commandStack.back()[ jagDraw::DrawablePrep::VertexArrayObject_t ] );
         const jagDraw::VertexArrayObjectPtr vaop( boost::static_pointer_cast< jagDraw::VertexArrayObject >( drawablePrep ) );
        

          jagDraw::BufferObjectPtr bop( boost::dynamic_pointer_cast< jagDraw::BufferObject >(
        vaop->getVertexArrayCommand( jagDraw::VertexArrayCommand::BufferObject_t, jagDraw::VertexArrayObject::Vertex ) ) );
           
    jagDraw::VertexAttribPtr verts( boost::dynamic_pointer_cast< jagDraw::VertexAttrib >(
        vaop->getVertexArrayCommand( jagDraw::VertexArrayCommand::VertexAttrib_t, jagDraw::VertexArrayObject::Vertex ) ) );
        //3. intersect the triangles with the ray and record the hits
	bool boundIntersect;
	if(node.getBound()->getType()==jagDraw::Bound::Box_t)  {
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
            jagDraw::DrawCommandVec drawCommands = node.getDrawable(i)->getDrawCommandVec();
            BOOST_FOREACH( jagDraw::DrawCommandPtr dcp, drawCommands )
            {
                jagDraw::TriangleSurfer<gmtl::Point3f> ts(bop,verts,dcp);
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
