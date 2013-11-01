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

#ifndef __JAGDRAW_VISITOR_H__
#define __JAGDRAW_VISITOR_H__ 1

#include <jagDraw/DrawGraph.h>
#include <jagDraw/NodeContainer.h>
#include <jagDraw/Node.h>
#include <jagDraw/CommandMap.h>
#include <jagBase/LogBase.h>
#include <jagBase/ptr.h>

#include <boost/foreach.hpp>

#include <sstream>


namespace jagDraw {


/** \addtogroup jagDrawDrawGraph Draw Graph Data Structure */
/*@{*/

/** \class Visitor Visitor.h <jagDraw/Visitor.h>
\brief %Visitor design pattern, "visitor" component: base class.
\details See http://en.wikipedia.org/wiki/Visitor_pattern

jagDraw::Visitor visits all nodes in a draw graph. See also: jagSG::Visitor
and jagSG::VisitorBase, which visit nodes in a scene graph.

To begin traversal on a draw graph, call Visitor::traverse(), passing
the jagDraw::DrawGraph as the parameter. This calls Visitor::visit()
on each NodeContainer and Node.

Derived classes control traversal termination by overriding the
visit() method and returning true or false. Returning true continues
traversal. Returning false from Visitir::visit(Node&) halts traversal
for the current NodeContainer. Returning false from
Visitir::visit(NodeContainer&) halts traversal for the DrawGraph.
*/
class Visitor : protected jagBase::LogBase
{
public:
    Visitor( const std::string& logNameSuffix, const std::string& logName=std::string( "" ) )
      : jagBase::LogBase( logName.empty() ? std::string( "jag.draw.visit." ) + logNameSuffix : logName )
    {
    }

    Visitor( const jagDraw::Visitor& rhs )
      : jagBase::LogBase( rhs )
    {
    }

    ~Visitor()
    {
    }

    virtual void traverse( jagDraw::DrawGraph& drawGraph )
    {
        BOOST_FOREACH( jagDraw::DrawGraph::value_type& nc, drawGraph )
        {
            if( !( visit( nc ) ) )
                break;
            BOOST_FOREACH( jagDraw::NodeContainer::value_type& node, nc )
            {
                if( !( visit( *node, nc ) ) )
                    break;
            }
        }
    }


    virtual bool visit( jagDraw::NodeContainer& /*nc*/ )
    {
        return( true );
    }

    virtual bool visit( jagDraw::Node& /*node*/, jagDraw::NodeContainer& /*nc*/ )
    {
        return( true );
    }
};

typedef jagBase::ptr< jagDraw::Visitor >::shared_ptr VisitorPtr;


/*@}*/


// jagDraw
}


// __JAGDRAW_VISITOR_H__
#endif
