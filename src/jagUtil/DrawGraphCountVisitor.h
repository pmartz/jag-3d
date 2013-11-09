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

#ifndef __JAGUTIL_DRAW_GRAPH_COUNT_VISITOR_H__
#define __JAGUTIL_DRAW_GRAPH_COUNT_VISITOR_H__ 1

#include <jagUtil/Export.h>
#include <jagDraw/Visitor.h>
#include <jagDraw/NodeContainer.h>
#include <jagDraw/Node.h>
#include <jagDraw/CommandMap.h>

#include <iostream>


namespace jagUtil
{


class DrawGraphCountVisitor : public jagDraw::Visitor
{
public:
    DrawGraphCountVisitor();
    ~DrawGraphCountVisitor();

    void reset();
    void dump( std::ostream& ostr );

    virtual bool visit( jagDraw::NodeContainer& nc );
    virtual bool visit( jagDraw::Node& node, jagDraw::NodeContainer& nc );

protected:
    unsigned int _containers;
    unsigned int _nodes;
    unsigned int _drawables;

    unsigned int _nonEmptyCommandMapDeltas;
    jagDraw::CommandMap _commands;
};

inline DrawGraphCountVisitor::DrawGraphCountVisitor()
    : jagDraw::Visitor( "count" )
{
    reset();
}
inline DrawGraphCountVisitor::~DrawGraphCountVisitor()
{
}

inline void DrawGraphCountVisitor::reset()
{
    _containers = 0;
    _nodes = 0;
    _drawables = 0;
    _nonEmptyCommandMapDeltas = 0;
}
inline void DrawGraphCountVisitor::dump( std::ostream& ostr )
{
    ostr << "            \tTotal" << std::endl;
    ostr << "            \t-----" << std::endl;
    ostr << " Containers:\t" << _containers << std::endl;
    ostr << "      Nodes:\t" << _nodes << std::endl;
    ostr << "  Drawables:\t" << _drawables << std::endl;
    ostr << "  CM deltas:\t" << _nonEmptyCommandMapDeltas << std::endl;
}

inline bool DrawGraphCountVisitor::visit( jagDraw::NodeContainer& nc )
{
    ++_containers;
    return( true );
}
inline bool DrawGraphCountVisitor::visit( jagDraw::Node& node, jagDraw::NodeContainer& nc )
{
    ++_nodes;
    _drawables += node.getNumDrawables();

    if( node.getCommandMap() != NULL )
    {
        jagDraw::CommandMap delta( _commands << *( node.getCommandMap() ) );
        if( !( delta.empty() ) )
            ++_nonEmptyCommandMapDeltas;
    }

    return( true );
}


// jagUtil
}


// __JAGUTIL_DRAW_GRAPH_COUNT_VISITOR_H__
#endif
