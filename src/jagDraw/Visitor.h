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

#ifndef __JAGDRAW_VISITOR_H__
#define __JAGDRAW_VISITOR_H__ 1

#include <jagDraw/DrawGraph.h>
#include <jagDraw/DrawNodeContainer.h>
#include <jagDraw/DrawNode.h>
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
            BOOST_FOREACH( jagDraw::DrawNodeContainer::value_type& node, nc )
            {
                if( !( visit( *node, nc ) ) )
                    break;
            }
        }
    }


    virtual bool visit( jagDraw::DrawNodeContainer& /*nc*/ )
    {
        return( true );
    }

    virtual bool visit( jagDraw::DrawNode& /*node*/, jagDraw::DrawNodeContainer& /*nc*/ )
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
