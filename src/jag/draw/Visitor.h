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

#ifndef __JAG_DRAW_VISITOR_H__
#define __JAG_DRAW_VISITOR_H__ 1

#include <jag/draw/DrawGraph.h>
#include <jag/draw/DrawNodeContainer.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/CommandMap.h>
#include <jag/base/LogBase.h>
#include <jag/base/ptr.h>

#include <boost/foreach.hpp>

#include <sstream>


namespace jag {
namespace draw {


/** \addtogroup jagDrawDrawGraph Draw Graph Data Structure */
/*@{*/

/** \class Visitor Visitor.h <jag/draw/Visitor.h>
\brief %Visitor design pattern, "visitor" component: base class.
\details See http://en.wikipedia.org/wiki/Visitor_pattern

jag::draw::Visitor visits all nodes in a draw graph. See also: jag::sg::Visitor
and jag::sg::VisitorBase, which visit nodes in a scene graph.

To begin traversal on a draw graph, call Visitor::traverse(), passing
the jag::draw::DrawGraph as the parameter. This calls Visitor::visit()
on each NodeContainer and Node.

Derived classes control traversal termination by overriding the
visit() method and returning true or false. Returning true continues
traversal. Returning false from Visitir::visit(Node&) halts traversal
for the current NodeContainer. Returning false from
Visitir::visit(NodeContainer&) halts traversal for the DrawGraph.
*/
class Visitor : protected jag::base::LogBase
{
public:
    Visitor( const std::string& logNameSuffix, const std::string& logName=std::string( "" ) )
      : jag::base::LogBase( logName.empty() ? std::string( "jag.draw.visit." ) + logNameSuffix : logName )
    {
    }

    Visitor( const jag::draw::Visitor& rhs )
      : jag::base::LogBase( rhs )
    {
    }

    ~Visitor()
    {
    }

    virtual void traverse( jag::draw::DrawGraph& drawGraph )
    {
        BOOST_FOREACH( jag::draw::DrawGraph::value_type& nc, drawGraph )
        {
            if( !( visit( nc ) ) )
                break;
            BOOST_FOREACH( jag::draw::DrawNodeContainer::value_type& node, nc )
            {
                if( !( visit( *node, nc ) ) )
                    break;
            }
        }
    }


    virtual bool visit( jag::draw::DrawNodeContainer& /*nc*/ )
    {
        return( true );
    }

    virtual bool visit( jag::draw::DrawNode& /*node*/, jag::draw::DrawNodeContainer& /*nc*/ )
    {
        return( true );
    }
};

typedef jag::base::ptr< jag::draw::Visitor >::shared_ptr VisitorPtr;


/*@}*/


// namespace jag::draw::
}
}


// __JAG_DRAW_VISITOR_H__
#endif
