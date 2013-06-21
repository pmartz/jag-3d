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

    /** \brief TBD
    \details TBD */
    void getNearFar( double& minECNear, double& maxECFar, const double ratio=( 1. / 2000. ) );

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
        CollectionInfo( jagBase::TransformD& transform, jagSG::Node* node=NULL );
        CollectionInfo( const CollectionInfo& rhs );
        virtual ~CollectionInfo();

        virtual void setNode( jagSG::Node* node );
        /** \brief Set the bound without setting the node.
        \details This is primarily for use by unit tests. */
        void setBound( jagDraw::Bound* Bound );

        double getECBoundDistance() const;
        double getECBoundRadius() const;
        double getWinCLength( double ecSegmentLength ) const;

    protected:
        jagBase::TransformD& _transform;
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
    double _minNear, _maxFar;
};

typedef jagBase::ptr< jagSG::CollectionVisitor >::shared_ptr CollectionVisitorPtr;


// jagSG
}


// __JAGSG_COLLECTION_VISITOR_H__
#endif
