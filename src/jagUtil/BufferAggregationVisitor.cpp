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
}
BufferAggregationVisitor::BufferAggregationVisitor( const BufferAggregationVisitor& rhs )
    : jagSG::VisitorBase( rhs )
{
}
BufferAggregationVisitor::~BufferAggregationVisitor()
{
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

    pushCommandMap( node.getCommandMap() );


    jagDraw::CommandMap& commands( _commandStack.back() );
    jagDraw::DrawablePrepPtr& drawablePrep( commands[ jagDraw::DrawablePrep::VertexArrayObject_t ] );
    jagDraw::VertexArrayObjectPtr vaop( boost::static_pointer_cast< jagDraw::VertexArrayObject >( drawablePrep ) );
    if( ( vaop != NULL ) && _vaop->isSameKind( *vaop ) )
        _vaop->combine( *vaop );


    for( unsigned int idx=0; idx < node.getNumDrawables(); ++idx )
    {
        handleDrawable( node.getDrawable( idx ) );
    }

    node.traverse( *this );

    popCommandMap();
}


void BufferAggregationVisitor::handleDrawable( jagDraw::DrawablePtr draw )
{
}


// jagUtil
}
