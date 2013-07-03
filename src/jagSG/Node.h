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
#include <jagDraw/BoundOwner.h>
#include <jagBase/MultiCallback.h>
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
\brief Scene graph element. "element" component of the %Visitor design pattern.
\details TBD
*/
class JAGSG_EXPORT Node : protected jagBase::LogBase,
        public jagDraw::ObjectIDOwner, public jagDraw::BoundOwner,
        public SHARED_FROM_THIS(Node)
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
    /** \brief Call accept() on all child Nodes.
    \details Normally this is only invoked if a traverse callback is not attached. */
    virtual void traverse( jagSG::VisitorBase& visitor );

    /** \class CallbackInfo
    \brief TBD
    \details TBD */
    class CallbackInfo {
    public:
        CallbackInfo( jagSG::Node* node=NULL )
          : _node( node )
        {
        }
        CallbackInfo( const CallbackInfo& /* rhs */ ) {}
        virtual ~CallbackInfo() {}

        virtual void setNode( jagSG::Node* node ) { _node = node; }
        jagSG::Node* getNode() const { return( _node ); }

    protected:
        jagSG::Node* _node;
    };
    typedef jagBase::ptr< CallbackInfo >::shared_ptr CallbackInfoPtr;

    /** \struct Callback
    \brief TBD
    \details TBD */
    struct Callback {
        /** \brief TBD
        \details TBD */
        virtual bool operator()( jagSG::VisitorBase* /* visitor */,
            CallbackInfo* info = NULL )
        {
            return( false );
        }
    };
    typedef jagBase::ptr< Callback >::shared_ptr CallbackPtr;

    /** \brief TBD
    \details TBD */
    void setTraverseCallback( const CallbackPtr traverseCallback );
    /** \brief TBD
    \details TBD */
    const CallbackPtr getTraverseCallback() const;


    /** \brief TBD
    \details TBD */
    typedef jagBase::MultiCallback< CallbackPtr > CollectionCallbacks;
    /** \brief TBD
    \details TBD */
    CollectionCallbacks& getCollectionCallbacks();


    /** \brief Calls the execute() function on all attached Drawables.
    \details Primarily for "immediate mode" (no cull traversal) rendering.
    See the ExecuteVisitor.

    Requires a current context. */
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


    /** \name Support for Node bound volume computation and the BoundOwner base-class.
    \details Override member functions from the BoundOwner base class.
    Also provide jagSG::Node-specific bound computation support. */
    /**@{*/

    /** \brief Get the bound from any node based on \c commands
    \details \c commands contains jagDraw::VertexArrayObjects, which contain
    the vertex data required to compute the bound. */
    virtual jagDraw::BoundPtr getBound( const jagDraw::CommandMap& commands );
    /** \brief Get bound from root node.
    \details Assumes no default state. */
    jagDraw::BoundPtr getBound()
    {
        return( getBound( jagDraw::CommandMap() ) );
    }

    /** \brief Return a new uninitialized bound.
    \details Override the base class BoundOwner::computeBound().
    Returns a new BoundSphere. */
    jagDraw::BoundPtr newBound();

    /** \brief Compute the Drawable's bounding volume.
    \details Override the base class BoundOwner::computeBound(). */
    virtual void computeBound( jagDraw::BoundPtr bound, const jagDraw::VertexArrayObject* vao, BoundOwner* owner );

    /**@}*/


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
    virtual void addDrawable( jagDraw::DrawablePtr node );
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
    virtual void addChild( NodePtr node );
    /** \brief TBD
    \details TBD */
    unsigned int getNumChildren() const;
    /** \brief TBD
    \details TBD */
    NodePtr getChild( const unsigned int idx );
    /** \overload */
    const NodePtr getChild( const unsigned int idx ) const;

    /** \brief Children
    \details TBD */
    virtual void addParent( NodePtr node );
    /** \brief TBD
    \details TBD */
    unsigned int getNumParents() const;
    /** \brief TBD
    \details TBD */
    NodePtr getParent( const unsigned int idx );
    /** \overload */
    const NodePtr getParent( const unsigned int idx ) const;
    /** \brief TBD
    \details TBD */
    const NodeVec& getParents() const;



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
    NodeVec _parents;

    CallbackPtr _traverseCallback;
    CollectionCallbacks _collectionCallbacks;
};


// jagSG
}


// __JAGSG_NODE_H__
#endif
