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

#ifndef __JAGSG_VISITOR_H__
#define __JAGSG_VISITOR_H__ 1

//#include <jagSG/Export.h>
#include <jagSG/Node.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>


namespace jagSG {


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

Nodes often contain a traversal callback to determine whyich children are traversed.
See the Visitor class. The Visitor::visit() function uses a Node's traversal callback
if present, and calls Node::traverse() otherwise. Typically, application classes
should derive from Visitor to take advantage of this, unless they explicitly need to
visit all children.
*/
class /*JAGSG_EXPORT*/ VisitorBase : protected jagBase::LogBase
{
public:
    VisitorBase( const std::string& logName )
      : jagBase::LogBase( "jag.sg.visit." + logName )
    {
    }

    VisitorBase( const std::string& logName, jagSG::Node& node )
      : jagBase::LogBase( "jag.sg.visit." + logName )
    {
        node.accept( *this );
    }

    VisitorBase( const VisitorBase& rhs )
      : jagBase::LogBase( rhs )
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
        node.traverse( *this );

        // Often, the following is a more useful implementation (as is
        // done in Visitor::visit() ).
        //   if( node.getTraverseCallback() != NULL )
        //       node.traverseCallback( *this );
        //   else
        //       node.traverse( *this );
    }

protected:
};

typedef jagBase::ptr< jagSG::VisitorBase >::shared_ptr VisitorBasePtr;



/** \class Visitor Visitor.h <jagSG/Visitor.h>
\brief %Visitor design pattern, %Visitor component base class.
\details Allows the Node's traversal callback to determine continued traversal.

Custom visitor classes should derive from VisitorBase to visit all Nodes, but should
derive from Visitor to use the Node's traversal callback to determine child traversal
(as for switch and sequence functionality).

Derived classes should override Visitor::visit(), perform any pre-traversal tasks,
call Visitor::visit() to effect traversal, then perform any post-traversal tasks. */
class /*JAGSG_EXPORT*/ Visitor : public VisitorBase
{
public:
    Visitor( const std::string& logName )
      : VisitorBase( logName )
    {
    }

    Visitor( const std::string& logName, jagSG::Node& node )
      : VisitorBase( logName, node )
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
        if( node.getTraverseCallback() != NULL )
            (*node.getTraverseCallback())( *this );
        else
            node.traverse( *this );
    }

protected:
};

typedef jagBase::ptr< jagSG::Visitor >::shared_ptr VisitorPtr;


// jagSG
}


// __JAGSG_VISITOR_H__
#endif
