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
#include <jagBase/UserDataOwner.h>
#include <jagBase/ptr.h>

#include <gmtl/gmtl.h>

#include <vector>


namespace jagDraw {
    struct DrawInfo;
}

namespace jagSG {


/** \addtogroup jagSGSceneGraph The Scene Graph */
/*@{*/

class Node;
typedef jagBase::ptr< jagSG::Node >::shared_ptr NodePtr;
typedef std::vector< NodePtr > NodeVec;
typedef std::vector< Node > NodeSimpleVec;
typedef std::vector< Node* > NodeSimplePtrVec;

class VisitorBase;


/** \class Node Node.h <jagSG/Node.h>
\brief Scene graph element. "element" component of the %Visitor design pattern.
\details TBD
*/
class JAGSG_EXPORT Node : protected jagBase::LogBase,
        public jagBase::UserDataOwner, 
        public jagDraw::ObjectIDOwner,
        public jagDraw::BoundOwner,
        public SHARED_FROM_THIS(Node)
{
public:
    Node( const std::string& logName=std::string( "" ) );
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
    void setTraverseCallback( const CallbackPtr& traverseCallback );
    /** \brief TBD
    \details TBD */
    const CallbackPtr& getTraverseCallback() const;


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

    /** \brief Get bound from root node.
    \details Assumes no default state. */
    const jagDraw::BoundPtr& getBound()
    {
        jagDraw::CommandMap commands;
        return( BoundOwner::getBound( commands ) );
    }

    /** \brief Return a new uninitialized bound.
    \details Override the base class BoundOwner::computeBound().
    Returns a new BoundSphere. */
    jagDraw::BoundPtr newBound();

    /** \brief Compute the Node's bounding volume.
    \details Override the base class BoundOwner::computeBound(). */
    virtual void computeBound( jagDraw::BoundPtr& bound, const jagDraw::CommandMap& commands );

    /** \brief Mark all bounds dirty and notify parents.
    \details Override the base class BoundOwner::setAllBoundsDirty()
    so that parent Nodes can be notified. */
    virtual void setBoundDirty( const bool dirty=true );

    /**@}*/


    /** \brief CommandMap
    \details TBD */
    void setCommandMap( jagDraw::CommandMapPtr& commands );
    /** \brief TBD
    \details TBD */
    jagDraw::CommandMapPtr& getCommandMap();
    /** \overload */
    const jagDraw::CommandMapPtr& getCommandMap() const;
    /** \brief TBD
    \details TBD */
    jagDraw::CommandMapPtr& getOrCreateCommandMap();


    /** \brief Drawables
    \details TBD */
    virtual void addDrawable( jagDraw::DrawablePtr& drawable );
    /** \brief TBD
    \details TBD */
    unsigned int getNumDrawables() const;
    /** \brief Remove a Drawable.
    \details Returns the total number of Drawables remaining
    after the deletion. Returns -1 if the specified Drawable 
    can't be found. */
    virtual int removeDrawable( jagDraw::DrawablePtr& drawable );
    /** \overload */
    virtual int removeDrawable( const unsigned int index );
    /** \brief TBD
    \details TBD */
    jagDraw::DrawablePtr& getDrawable( const unsigned int idx );
    /** \overload */
    const jagDraw::DrawablePtr& getDrawable( const unsigned int idx ) const;


    /** \brief Children
    \details TBD */
    virtual void addChild( NodePtr& node );
    /** \brief TBD
    \details TBD */
    virtual int removeChild( const NodePtr& node );
    /** \brief TBD
    \details TBD */
    virtual int removeChild( const unsigned int index );
    /** \brief TBD
    \details TBD */
    unsigned int getNumChildren() const;
    /** \brief TBD
    \details TBD */
    NodePtr& getChild( const unsigned int idx );
    /** \overload */
    const NodePtr& getChild( const unsigned int idx ) const;

    /** \brief Children
    \details TBD */
    virtual void addParent( Node* node );
    /** \brief TBD
    \details TBD */
    virtual int removeParent( const Node* node );
    /** \brief TBD
    \details TBD */
    unsigned int getNumParents() const;
    /** \brief TBD
    \details TBD */
    Node* getParent( const unsigned int idx );
    /** \overload */
    const Node* getParent( const unsigned int idx ) const;
    /** \brief TBD
    \details TBD */
    const NodeSimplePtrVec& getParents() const;



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
    NodeSimplePtrVec _parents;

    CallbackPtr _traverseCallback;
    CollectionCallbacks _collectionCallbacks;

    /** \struct Callback Notifier.h <jagBase/Notifier.h>
    \brief Notifies client code  */
    struct BoundDirtyCallback : jagBase::Notifier::NotifierCallback
    {
        BoundDirtyCallback( Node* owner )
            : _owner( owner )
        {}
        BoundDirtyCallback( const BoundDirtyCallback& rhs )
            : _owner( rhs._owner )
        {}
        virtual ~BoundDirtyCallback()
        {}

        /** \brief TBD
        \details TBD */
        virtual void operator()( jagBase::Notifier* notifier, const jagBase::Notifier::NotifierInfo& info );

        Node* _owner;
    };
    typedef jagBase::ptr< BoundDirtyCallback >::shared_ptr BoundDirtyCallbackPtr;

    BoundDirtyCallbackPtr _boundDirtyCallback;
};


/*@}*/


// jagSG
}


// __JAGSG_NODE_H__
#endif
