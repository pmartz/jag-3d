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

#ifndef __JAGUTIL_DRAW_GRAPH_COUNT_VISITOR_H__
#define __JAGUTIL_DRAW_GRAPH_COUNT_VISITOR_H__ 1

#include <jagUtil/Export.h>
#include <jagDraw/Visitor.h>
#include <jagDraw/DrawNodeContainer.h>
#include <jagDraw/DrawNode.h>
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

    virtual bool visit( jagDraw::DrawNodeContainer& nc );
    virtual bool visit( jagDraw::DrawNode& node, jagDraw::DrawNodeContainer& nc );

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

inline bool DrawGraphCountVisitor::visit( jagDraw::DrawNodeContainer& nc )
{
    ++_containers;
    return( true );
}
inline bool DrawGraphCountVisitor::visit( jagDraw::DrawNode& node, jagDraw::DrawNodeContainer& nc )
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
