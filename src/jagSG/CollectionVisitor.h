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
    CollectionVisitor( const CollectionVisitor& rhs );
    virtual ~CollectionVisitor();


    /** \brief TBD
    \details TBD */
    void setViewProj( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj );

    /** \brief TBD
    \details TBD */
    void reset();

    /** \brief TBD
    \details TBD */
    virtual void visit( jagSG::Node& node );

    /** \brief TBD
    \details TBD */
    const jagDraw::DrawNodeSimpleVec& getDrawGraph() const;

protected:
    void updateTransformUniforms();

    jagDraw::DrawNodeSimpleVec _drawGraph;

    jagBase::TransformD _transform;
};

typedef jagBase::ptr< jagSG::CollectionVisitor >::shared_ptr CullVisitorPtr;


// jagSG
}


// __JAGSG_COLLECTION_VISITOR_H__
#endif
