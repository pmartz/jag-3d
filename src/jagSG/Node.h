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

#ifndef __JAGSG_NODE_H__
#define __JAGSG_NODE_H__ 1

#include <jagSG/Export.h>
#include <jagDraw/ObjectID.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Bound.h>
#include <jagDraw/ContextSupport.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>

#include <gmtl/gmtl.h>

#include <vector>


namespace jagDraw {
    struct DrawInfo;
}

namespace jagSG {


class Node;
typedef jagBase::ptr< jagSG::Node >::shared_ptr NodePtr;
typedef std::vector< NodePtr > NodeVec;
typedef std::vector< Node > NodeSimpleVec;

class VisitorBase;


/** \class Node Node.h <jagSG/Node.h>
\brief TBD
\details TBD
*/
class JAGSG_EXPORT Node : protected jagBase::LogBase, public jagDraw::ObjectIDOwner
{
public:
    Node();
    Node( const Node& rhs );
    ~Node();


    /** \name Visitor Support */
    /**@{*/

    /** \brief TBD
    \details TBD */
    void accept( jagSG::VisitorBase& visitor );
    /** \brief TBD
    \details TBD */
    virtual void traverse( jagSG::VisitorBase& visitor );

    /** \struct Callback
    \brief TBD
    \details TBD */
    struct Callback {
        /** \brief TBD
        \details TBD */
        virtual void operator()( jagSG::VisitorBase& visitor ) const;
    };
    typedef jagBase::ptr< Callback >::shared_ptr CallbackPtr;

    /** \brief TBD
    \details TBD */
    void setTraverseCallback( const CallbackPtr traverseCallback );
    /** \brief TBD
    \details TBD */
    const CallbackPtr getTraverseCallback() const;


    /** \brief For immediate rendering.
    \details Requires a current context.
    TBD remove this and replace it with an ExecuteVisitor. */
    virtual void execute( jagDraw::DrawInfo& drawInfo );

    /**@}*/


    /** \brief Transform
    \details
    TBD Make this virtual, and have two derived classes,
    one for matrices and one for quats. */
    void setTransform( const gmtl::Matrix44d& matrix );
    /** \brief TBD
    \details TBD */
    gmtl::Matrix44d& getTransform();
    /** \overload */
    const gmtl::Matrix44d& getTransform() const;


    /** \brief TBD Bound
    \details TBD */
    virtual jagDraw::BoundPtr getBound( const jagDraw::CommandMap& commands );


    /** \brief CommandMap
    \details TBD */
    void setCommandMap( jagDraw::CommandMapPtr commands );
    /** \brief TBD
    \details TBD */
    jagDraw::CommandMapPtr getCommandMap();
    /** \overload */
    const jagDraw::CommandMapPtr getCommandMap() const;


    /** \brief Drawables
    \details TBD */
    void addDrawable( jagDraw::DrawablePtr node );
    /** \brief TBD
    \details TBD */
    unsigned int getNumDrawables() const;
    /** \brief TBD
    \details TBD */
    jagDraw::DrawablePtr getDrawable( const unsigned int idx );
    /** \overload */
    const jagDraw::DrawablePtr getDrawable( const unsigned int idx ) const;


    /** \brief Children
    \details TBD */
    void addChild( NodePtr node );
    /** \brief TBD
    \details TBD */
    unsigned int getNumChildren() const;
    /** \brief TBD
    \details TBD */
    NodePtr getChild( const unsigned int idx );
    /** \overload */
    const NodePtr getChild( const unsigned int idx ) const;



    /** \brief Tell the contained ObjectID objects how many contexts to expect.
    \details Inherited from ObjectIDOwner. */
    virtual void setMaxContexts( const unsigned int numContexts );

    /** \brief Delete the ID for the given \c contextID.
    \details Inherited from ObjectIDOwner.
    OpenGL object ID cleanup is not yet implemented. TBD. */
    virtual void deleteID( const jagDraw::jagDrawContextID contextID );

protected:
    gmtl::Matrix44d _matrix;
    jagDraw::CommandMapPtr _commands;
    jagDraw::DrawableVec _drawables;
    NodeVec _children;
    jagDraw::BoundPtr _bound;

    CallbackPtr _traverseCallback;
};


// jagSG
}


// __JAGSG_NODE_H__
#endif
