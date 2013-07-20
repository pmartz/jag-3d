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

#include <jagUtil/BufferAggregationVisitor.h>
#include <jagDraw/Drawable.h>
#include <jagSG/Node.h>

#include <boost/foreach.hpp>

#include <sstream>


namespace jagUtil
{


BufferAggregationVisitor::BufferAggregationVisitor( jagSG::NodePtr node, const std::string& logName )
    : jagSG::VisitorBase( "bufagg", logName )
{
    _vaop = jagDraw::VertexArrayObjectPtr( new jagDraw::VertexArrayObject() );

    // Ensure bounds are clean.
    node->getBound();

    node->accept( *this );

    // This only resets the root node bounds. We need to reset bounds
    // on the entire scene graph.
    JAG3D_NOTICE( "Need a ResetBoundsVisitor." );
    node->resetBounds();

    if( _vaop->getVertexArrayCommandList().empty() )
    {
        JAG3D_INFO( "Result: VAO is empty." );
        return;
    }

    jagDraw::CommandMapPtr commands( node->getCommandMap() );
    if( commands == NULL )
    {
        commands.reset( new jagDraw::CommandMap() );
        node->setCommandMap( commands );
    }
    commands->insert( _vaop );

    JAG3D_DEBUG( "Final VAO/offset map:" );
    BOOST_FOREACH( const OffsetMap::value_type& data, _offsetMap )
    {
        std::ostringstream ostr;
        ostr << std::hex << data.first << std::dec << ", " << data.second;
        JAG3D_DEBUG( "  " + ostr.str() );
    }
}
BufferAggregationVisitor::BufferAggregationVisitor( const BufferAggregationVisitor& rhs )
    : jagSG::VisitorBase( rhs )
{
    reset();
}
BufferAggregationVisitor::~BufferAggregationVisitor()
{
}

void BufferAggregationVisitor::reset()
{
    _vaop.reset( ( jagDraw::VertexArrayObject* )NULL );
    _nodeSet.clear();
    _offsetMap.clear();
}



void BufferAggregationVisitor::visit( jagSG::Node& node )
{
    if( node.getAnyBoundDirty() )
    {
        JAG3D_WARNING( "visit(): Detected dirty bounds. App should call getBound() before invoking this visitor." );
    }

    if( node.getNumBounds() > 1 )
    {
        // Atypical but legal case: The node is multiparented and accessed
        // by more than one VertexArrayObject. We could handle this under certain
        // controlled conditions, but for simplicitly we leave this entire
        // subgraph alone.
        JAG3D_INFO( "Found node \"" + node.getUserDataName() + "\" with multiple bounds." );
        return;
    }

    if( _nodeSet.find( &node ) != _nodeSet.end() )
    {
        JAG3D_INFO( "Found shared node \"" + node.getUserDataName() + "\"." );
        return;
    }
    _nodeSet.insert( &node );

    pushCommandMap( node.getCommandMap() );


    jagDraw::CommandMap& commands( _commandStack.back() );
    jagDraw::DrawablePrepPtr& drawablePrep( commands[ jagDraw::DrawablePrep::VertexArrayObject_t ] );
    jagDraw::VertexArrayObjectPtr vaop( boost::static_pointer_cast< jagDraw::VertexArrayObject >( drawablePrep ) );

    if( ( vaop != NULL ) && _vaop->isSameKind( *vaop ) )
    {
        // We're going to aggregate the current VAO into the master.
        // Step 1: If the current VAO in the command stack matches the Node's
        // VAO, then remove the Node's VAO, because it will use the master
        // from not on.

        // Step 2: Combine the current VAO into the master VAO and
        // obtain the offset into the master VAO of the start of the
        // current VAO. We'll use this to modify the DrawCommand indices.
        std::ostringstream ostr;
        size_t offset;
        if( _offsetMap.find( vaop.get() ) != _offsetMap.end() )
        {
            offset = _offsetMap[ vaop.get() ];
            ostr << "Found mapped VAO, using offset " << offset;
        }
        else
        {
            offset = _vaop->combine( *vaop );
            _offsetMap[ vaop.get() ] = offset;
            ostr << "Did not find VAO, using offset " << offset;
        }
        JAG3D_INFO( ostr.str() );

        // Step 3: Modify the DrawCommand indices to access the new location
        // of the current VAO -- where it got copied to in the master VAO.
        for( unsigned int idx=0; idx < node.getNumDrawables(); ++idx )
        {
            handleDrawable( node.getDrawable( idx ) );
        }
    }

    node.traverse( *this );

    popCommandMap();
}


void BufferAggregationVisitor::handleDrawable( jagDraw::DrawablePtr draw )
{
}


// jagUtil
}
