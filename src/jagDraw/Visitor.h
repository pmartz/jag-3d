/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

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
