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

#ifndef __JAGSG_VISITOR_H__
#define __JAGSG_VISITOR_H__ 1

#include <jag/base/types.h>
#include <jagSG/Node.h>
#include <jagDraw/CommandMap.h>
#include <jag/base/LogBase.h>
#include <jag/base/ptr.h>

#include <sstream>


namespace jagSG {


/** \addtogroup jagSGSceneGraph The Scene Graph */
/*@{*/

/** \class VisitorBase Visitor.h <jagSG/Visitor.h>
\brief %Visitor design pattern, "visitor" component: base class.
\details See http://en.wikipedia.org/wiki/Visitor_pattern

VisitorBase visits all nodes in a scene graph. See also: Visitor, which
provides control over visited children using Node::getTraverseCallback().

To begin traversal on a hierarchy, call Node::accept() on the root node, passing
a class derived from VisitorBase as the parameter. This results in the following
recursive call sequence:
<table border="0">
  <tr>
    <th><b>Node</b></th>
    <th><b>VisitorBase</b></th>
  </tr>
  <tr>
    <td>Node::accept()</td>
    <td></td>
  </tr>
  <tr>
    <td></td>
    <td>VisitorBase::visit()</td>
  </tr>
  <tr>
    <td>Node::traverse()</td>
    <td></td>
  </tr>
  <tr>
    <td>Node::accept()</td>
    <td>etc...</td>
  </tr>
</table>

Classes that derive from VisitorBase should override visit() to perform per-Node tasks.
This function also controls continued traversal. The easiest way to do this is to
call VisitorBase::visit(), which traverses all children.

Nodes often contain a traversal callback to determine which children are traversed.
See the Visitor class. The Visitor::visit() function uses a Node's traversal callback
if present, and calls Node::traverse() otherwise. Typically, application classes
should derive from Visitor to take advantage of this, unless they explicitly need to
visit all children.

VisitorBase contains support for jagSG::Node, matrix, and jagDraw::CommandMap stacks.
Not all visitors will need these stacks, so using them is the responsibility of the
derived class. As an example, a derived class might override visit() as follows:
\code
    virtual void visit( jagSG::Node& node )
    {
        pushNode( node.shared_from_this() );
        MatrixStackHelper msh( *this, node.getTransform() );
        CommandStackHelper cmsh( *this, node.getCommandMap() );

        node.traverse( *this );

        popNode();
    }
\endcode
*/
class VisitorBase : protected jag::base::LogBase
{
public:
    VisitorBase( const std::string& logNameSuffix, const std::string& logName=std::string( "" ) )
      : jag::base::LogBase( logName.empty() ? std::string( "jag.sg.visit." ) + logNameSuffix : logName )
    {
        _matrixStack.push_back( gmtl::MAT_IDENTITY44D );
        _commandStack.push_back( jagDraw::CommandMap() );
    }

    VisitorBase( jagSG::Node& node, const std::string& logNameSuffix, const std::string& logName=std::string( "" ) )
      : jag::base::LogBase( logName.empty() ? std::string( "jag.sg.visit." ) + logNameSuffix : logName )
    {
        _matrixStack.push_back( gmtl::MAT_IDENTITY44D );
        _commandStack.push_back( jagDraw::CommandMap() );

        node.accept( *this );
    }

    VisitorBase( const VisitorBase& rhs )
      : jag::base::LogBase( rhs ),
        _nodeStack( rhs._nodeStack ),
        _matrixStack( rhs._matrixStack ),
        _commandStack( rhs._commandStack )
    {
    }

    virtual ~VisitorBase()
    {
    }


    /** \brief Function for per-Node tasks and continued traversal.
    \details Derived classes should override this function to perform per-Node tasks
    and continue traversal if desired. Calling Node::traverse() will traverse all
    child Nodes. */
    virtual void visit( jagSG::Node& node )
    {
        // Push stacks, if desired or necessary.
        //pushNode( node.shared_from_this() );
        //MatrixStackHelper msh( *this, node.getTransform() );
        //CommandMapStackHelper cmsh( *this, node.getCommandMap() );

        node.traverse( *this );

        // Execute corresponding pop.
        //popNode();


        // Often, the following is a more useful implementation (as is
        // done in Visitor::visit() ).
        //   push stacks
        //   if( node.getTraverseCallback() != NULL )
        //       node.traverseCallback( *this );
        //   else
        //       node.traverse( *this );
        //   pop stacks
    }


    void pushNode( jagSG::NodePtr& node )
    {
        _nodeStack.push_back( node );
    }
    void popNode()
    {
        _nodeStack.pop_back();
    }
    void resetNode()
    {
        _nodeStack.clear();
    }
    const jagSG::NodeVec& getNodes() const
    {
        return( _nodeStack );
    }

    /** \brief Push a matrix onto the stack.
    \details If \c matrix is not identity, push the Matrix and return true.
    Otherwise, do nothing and return false. */
    bool pushMatrix( const gmtl::Matrix44d& matrix )
    {
        if( matrix.mState == gmtl::Matrix44d::IDENTITY )
            return( false );
        _matrixStack.push_back( _matrixStack.back() * matrix );
        return( true );
    }
    void popMatrix()
    {
        _matrixStack.pop_back();
    }
    void resetMatrix()
    {
        _matrixStack.resize( 1 );
    }
    const jag::base::Matrix44dVec& getMatrices() const
    {
        return( _matrixStack );
    }

    /** \brief Push a CommandMap onto the stack.
    \details If \c commands != NULL, push the CommandMap and return true.
    Otherwise, do nothing and return false. */
    bool pushCommandMap( const jagDraw::CommandMapPtr& commands )
    {
        JAG3D_PROFILE( "CM stack processing" );
        if( ( commands == NULL ) || commands->empty() )
            return( false );
        _commandStack.push_back( _commandStack.back() + *commands );
        return( true );
    }
    void popCommandMap()
    {
        JAG3D_PROFILE( "CM stack processing" );
        _commandStack.pop_back();
    }
    void resetCommandMap()
    {
        _commandStack.resize( 1 );
    }
    const jagDraw::CommandMapSimpleVec& getCommands() const
    {
        return( _commandStack );
    }


    /** class MatrixStackHelper Visitor.h <jagSG/Visitor.h>
    \brief Convenience function for pushing/popping the matrix stack. */
    class MatrixStackHelper
    {
    public:
        MatrixStackHelper( VisitorBase& visitorBase, const gmtl::Matrix44d& matrix )
            : _visitorBase( visitorBase ),
              _op( _visitorBase.pushMatrix( matrix ) )
        {}
        ~MatrixStackHelper()
        {
            if( _op )
                _visitorBase.popMatrix();
        }
        bool getDirty() const { return( _op ); }

    protected:
        VisitorBase& _visitorBase;
        bool _op;
    };

    /** class CommandMapStackHelper Visitor.h <jagSG/Visitor.h>
    \brief Convenience function for pushing/popping the CommandMap stack. */
    class CommandMapStackHelper
    {
    public:
        CommandMapStackHelper( VisitorBase& visitorBase, const jagDraw::CommandMapPtr& commands )
            : _visitorBase( visitorBase ),
              _op( _visitorBase.pushCommandMap( commands ) )
        {}
        ~CommandMapStackHelper()
        {
            if( _op )
                _visitorBase.popCommandMap();
        }
        bool getDirty() const { return( _op ); }

    protected:
        VisitorBase& _visitorBase;
        bool _op;
    };

protected:
    NodeVec _nodeStack;
    jag::base::Matrix44dVec _matrixStack;
    jagDraw::CommandMapSimpleVec _commandStack;
};

typedef jag::base::ptr< jagSG::VisitorBase >::shared_ptr VisitorBasePtr;



/** \class Visitor Visitor.h <jagSG/Visitor.h>
\brief %Visitor design pattern, %Visitor component base class.
\details Allows the Node's traversal callback to determine continued traversal.

Custom visitor classes should derive from VisitorBase to visit all Nodes, but should
derive from Visitor to use the Node's traversal callback to determine child traversal
(as for switch and sequence functionality).

Derived classes should override Visitor::visit(), perform any pre-traversal tasks,
call Visitor::visit() to effect traversal, then perform any post-traversal tasks. */
class Visitor : public VisitorBase
{
public:
    Visitor( const std::string& logNameSuffix, const std::string& logName=std::string( "" ) )
      : VisitorBase( logNameSuffix, logName )
    {
    }

    Visitor( jagSG::Node& node, const std::string& logNameSuffix, const std::string& logName=std::string( "" ) )
      : VisitorBase( node, logNameSuffix, logName )
    {
    }

    Visitor( const Visitor& rhs )
      : VisitorBase( rhs )
    {
    }

    virtual ~Visitor()
    {
    }


    /** \brief TBD
    \details TBD */
    virtual void visit( jagSG::Node& node )
    {
        // push stacks, if desired

        if( node.getTraverseCallback() != NULL )
            (*node.getTraverseCallback())( this );
        else
            node.traverse( *this );

        // If stack was pushed, execute corresponding pop.
    }

protected:
};

typedef jag::base::ptr< jagSG::Visitor >::shared_ptr VisitorPtr;


/*@}*/


// jagSG
}


// __JAGSG_VISITOR_H__
#endif
