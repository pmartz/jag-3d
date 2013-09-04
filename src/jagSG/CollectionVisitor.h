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

#ifndef __JAGSG_COLLECTION_VISITOR_H__
#define __JAGSG_COLLECTION_VISITOR_H__ 1

#include <jagSG/Export.h>
#include <jagSG/Visitor.h>
#include <jagBase/Transform.h>
#include <jagDraw/Node.h>
#include <jagDraw/NodeContainer.h>
#include <jagDraw/DrawGraph.h>
#include <jagDraw/TransformCallback.h>
#include <jagDraw/Uniform.h>
#include <jagBase/ptr.h>


namespace jagSG {


/** \class CollectionVisitor CollectionVisitor.h <jagSG/CollectionVisitor.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT CollectionVisitor : public Visitor
{
public:
    CollectionVisitor();
    CollectionVisitor( jagSG::Node& node );
    CollectionVisitor( const CollectionVisitor& rhs );
    virtual ~CollectionVisitor();


    /** \brief TBD
    \details TBD */
    void setViewProj( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );
    /** \brief TBD
    \details TBD */
    void setViewport( const GLint x, const GLint y, const GLsizei width, const GLsizei height );

    /** \brief TBD
    \details TBD */
    void reset();


    /** \brief TBD
    \details TBD */
    virtual void visit( jagSG::Node& node );


    /** \brief TBD
    \details TBD */
    void collectAndTraverse( jagSG::Node& node );


    /** \brief TBD
    \details TBD */
    jagDraw::DrawGraphPtr getDrawGraph() const;

    /** \brief TBD
    \details TBD */
    void setDrawGraphTemplate( jagDraw::DrawGraphPtr drawGraphTemplate );
    /** \brief TBD
    \details TBD */
    const jagDraw::DrawGraphPtr getDrawGraphTemplate() const;

    /** \brief Get the TransformCallback that controls draw-time matrix uniforms.
    \details CollectionVisitor inserts this into the DrawGraph to change
    transform matrix uniforms possibly per DrawNode. Application code access this
    object to specify which matrix uniforms are desired, and specify custom
    matrix uniform names. */
    jagDraw::TransformCallback* getTransformCallback()
    {
        return( _drawTransformCallback.get() );
    }



    typedef enum {
        None = 0,
        AutoCompute = ( 0x1 << 0 ),
        DiscardNegativeECZDrawables = ( 0x1 << 1 ),
        Default = ( AutoCompute | DiscardNegativeECZDrawables )
    } NearFarOps;
    /** \brief TBD
    \details TBD */
    void setNearFarOps( const NearFarOps nearFarOps );
    /** \brief TBD
    \details TBD */
    NearFarOps getNearFarOps() const;

    /** \brief TBD
    \details TBD */
    void getNearFar( double& minECNear, double& maxECFar, const double ratio=( 1. / 2000. ) );

    typedef enum {
        LeftPlane = ( 0x1 << 0 ),
        RightPlane = ( 0x1 << 1 ),
        TopPlane = ( 0x1 << 2 ),
        BottomPlane = ( 0x1 << 3 ),
        NearPlane = ( 0x1 << 4 ),
        FarPlane = ( 0x1 << 5 ),
        LeftRightTopBottomPlanes = ( LeftPlane | RightPlane | TopPlane | BottomPlane ),
        AllPlanes = ( LeftRightTopBottomPlanes | NearPlane | FarPlane )
    } FrustumPlanes;
    /** \brief TBD
    \details TBD */
    void setFrustumPlanes( const FrustumPlanes frustumPlanes );
    /** \brief TBD
    \details TBD */
    FrustumPlanes getFrustumPlanes() const;


    /** \brief TBD
    \details TBD */
    void setCurrentNodeContainer( const unsigned int currentID );
    /** \brief TBD
    \details TBD */
    unsigned int getCurrentNodeContainer() const;


    /** \class CollectionInfo CollectionVisitor.h <jagBase/CollectionVisitor.h>
    \brief TBD
    \details TBD */
    class JAGSG_EXPORT CollectionInfo : public Node::CallbackInfo
    {
    public:
        CollectionInfo( jagBase::TransformD& transform );
        CollectionInfo( const CollectionInfo& rhs );
        virtual ~CollectionInfo();

        /** \brief TBD
        \details Set by CollectionVisitor. */
        void setFrustumPlanes( const FrustumPlanes _frustumPlanes );

        /** \brief TBD
        \details Set by CollectionVisitor. */
        void setBound( jagDraw::Bound* Bound );

        double getECBoundDistance() const;
        double getECBoundRadius() const;
        double getWinCLength( double ecSegmentLength ) const;

        bool inFrustum() const;

    protected:
        jagBase::TransformD& _transform;

        FrustumPlanes _frustumPlanes;

        jagDraw::Bound* _bound;

        mutable double _ecDistance;
        mutable bool _ecDistanceDirty;
        mutable double _ecRadius;
        mutable bool _ecRadiusDirty;
        mutable double _wcLengthCoeff;
        mutable bool _wcLengthCoeffDirty;
    };
    typedef jagBase::ptr< CollectionInfo >::shared_ptr CollectionInfoPtr;

protected:
    jagDraw::DrawGraphPtr _drawGraphTemplate;
    jagDraw::DrawGraphPtr _drawGraph;
    unsigned int _currentID;
    jagDraw::NodeContainer* _currentNodes;

    jagBase::TransformD _transform;

    CollectionInfoPtr _infoPtr;

    jagDraw::TransformCallbackPtr _drawTransformCallback;

    NearFarOps _nearFarOps;
    double _minECZ, _maxECZ;
    FrustumPlanes _frustumPlanes;
};

typedef jagBase::ptr< jagSG::CollectionVisitor >::shared_ptr CollectionVisitorPtr;


// jagSG
}


// __JAGSG_COLLECTION_VISITOR_H__
#endif
