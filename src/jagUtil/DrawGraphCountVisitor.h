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
