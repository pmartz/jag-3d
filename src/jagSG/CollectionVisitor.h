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
#include <jagBase/gmtlSupport.h>


namespace jagSG {


/** \addtogroup jagSGCollection CollectionVisitor and Controls */
/*@{*/

/** \class CollectionVisitor CollectionVisitor.h <jagSG/CollectionVisitor.h>
\brief TBD
\details TBD

\logname jag.sg.visit.collect
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

    /** \brief Recompute near/far given a new view matrix.
    \details This function computes the delta viewpoint motion from
    the setViewProj() view matrix and the given \c view matrix, and
    resturns as estimate of the optimal near and far values. This is
    useful for performing collection once and draw multiple times. */
    void recomputeNearFar( double& minECNear, double& maxECFar, const gmtl::Matrix44d& view, const double ratio=( 1. / 2000. ) );

    /** \brief TBD
    \details Note: Shift amounts must match enumerants in GMTL Frustum.h */
    typedef enum {
        LeftPlane = ( 0x1 << 0 ), // gmtl::Frustumd::PLANE_LEFT
        RightPlane = ( 0x1 << 1 ), // gmtl::Frustumd::PLANE_RIGHT
        BottomPlane = ( 0x1 << 2 ), // gmtl::Frustumd::PLANE_BOTTOM
        TopPlane = ( 0x1 << 3 ), // gmtl::Frustumd::PLANE_TOP
        NearPlane = ( 0x1 << 4 ), // gmtl::Frustumd::PLANE_NEAR
        FarPlane = ( 0x1 << 5 ), // gmtl::Frustumd::PLANE_FAR
        LeftRightBottomTopPlanes = ( LeftPlane | RightPlane | BottomPlane | TopPlane ),
        AllPlanes = ( LeftRightBottomTopPlanes | NearPlane | FarPlane )
    } FrustumPlanes;
    /** \brief TBD
    \details TBD */
    void setFrustumPlanes( const FrustumPlanes frustumPlanes );
    /** \brief TBD
    \details TBD */
    FrustumPlanes getFrustumPlanes() const;

    typedef std::list< unsigned int > IndexList;
    typedef std::vector< IndexList > PlaneIndexStack;
    typedef std::vector< gmtl::Planed > PlaneVec;

    void pushPlanes()
    {
        const IndexList& stackTop( _planeStack[ _planeStack.size() - 1 ] );
        _planeStack.push_back( stackTop );
    }
    void popPlanes()
    {
        _planeStack.pop_back();
    }
    void resetPlanes()
    {
        _planeStack.resize( 1 );
        IndexList& indices( _planeStack[ 0 ] );
        indices.clear();
        for( unsigned int bit=0; bit<6; ++bit )
        {
            if( ( _frustumPlanes & ( 0x1 << bit ) ) != 0 )
                indices.push_back( bit );
        }
    }
    const PlaneIndexStack& getPlanes() const
    {
        return( _planeStack );
    }
    IndexList* getCurrentPlanes()
    {
        return( &( _planeStack[ _planeStack.size() - 1 ] ) );
    }


    /** \brief TBD
    \details TBD */
    void setCurrentNodeContainer( const unsigned int currentID );
    /** \brief TBD
    \details TBD */
    unsigned int getCurrentNodeContainer() const;


    /** \class CollectionInfo CollectionVisitor.h <jagBase/CollectionVisitor.h>
    \brief Set of information passed to collection callbacks.
    \details I'm starting to like this less and less. Seems like CollectionInfo
    just has copies or references to info in the CollectionVisitor. Why not just
    pass the CollectionVisitor to the collection callback?
    This decision is TBD. */
    class JAGSG_EXPORT CollectionInfo : public Node::CallbackInfo
    {
    public:
        CollectionInfo( jagBase::TransformD& transform );
        CollectionInfo( const CollectionInfo& rhs );
        virtual ~CollectionInfo();

        /** \brief TBD
        \details Set by CollectionVisitor. */
        void setBound( jagDraw::Bound* Bound );

        double getECBoundDistance() const;
        double getECBoundRadius() const;
        double getWinCLength( double ecSegmentLength ) const;

        /** \brief TBD
        \details Set by CollectionVisitor. */
        void setContainmentPlanes( IndexList* indices );
        /** \brief Test bound against containment planes
        \details TBD */
        bool isContained() const;

    protected:
        jagBase::TransformD& _transform;

        jagDraw::Bound* _bound;

        mutable double _ecDistance;
        mutable bool _ecDistanceDirty;
        mutable double _ecRadius;
        mutable bool _ecRadiusDirty;
        mutable double _wcLengthCoeff;
        mutable bool _wcLengthCoeffDirty;

        PlaneVec _planes;
        IndexList* _indices;
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
    PlaneIndexStack _planeStack;
};

typedef jagBase::ptr< jagSG::CollectionVisitor >::shared_ptr CollectionVisitorPtr;


/*@}*/


// jagSG
}


// __JAGSG_COLLECTION_VISITOR_H__
#endif
