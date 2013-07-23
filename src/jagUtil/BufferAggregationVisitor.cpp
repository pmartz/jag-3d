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
#include <jagUtil/ResetBoundsVisitor.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/DrawCommand.h>
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

    {
        // Reset (clear) the map of VAOs to bounds in every Node and Drawable.
        jagUtil::ResetBoundsVisitor rbv( node );
    }

    if( _vaop->getVertexArrayCommandList().empty() )
    {
        JAG3D_INFO( "Result: VAO is empty." );
        return;
    }

    // Store the aggregated VAO in the root node's CommandMap.
    jagDraw::CommandMapPtr commands( node->getCommandMap() );
    if( commands == NULL )
    {
        commands.reset( new jagDraw::CommandMap() );
        node->setCommandMap( commands );
    }
    commands->insert( _vaop );

    JAG3D_CRITICAL( "Finished." );
#if 0
    // Debug.
    JAG3D_DEBUG( "Final VAO/offset map:" );
    BOOST_FOREACH( const OffsetMap::value_type& data, _offsetMap )
    {
        std::ostringstream ostr;
        ostr << std::hex << data.first << std::dec << ", " << data.second;
        JAG3D_DEBUG( "  " + ostr.str() );
    }
#endif
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
    _bufferObjectSet.clear();
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

    jagDraw::CommandMapPtr localCommands( node.getCommandMap() );
    pushCommandMap( localCommands );


    jagDraw::CommandMap& commands( _commandStack.back() );
    jagDraw::DrawablePrepPtr& drawablePrep( commands[ jagDraw::DrawablePrep::VertexArrayObject_t ] );
    jagDraw::VertexArrayObjectPtr vaop( boost::static_pointer_cast< jagDraw::VertexArrayObject >( drawablePrep ) );

    if( ( vaop != NULL ) && _vaop->isSameKind( *vaop ) )
    {
        // We're going to aggregate the current VAO into the master.
        // Step 1: If the current VAO in the command stack matches the Node's
        // VAO, then remove the Node's VAO, because it will use the master
        // from now on.
        if( localCommands != NULL )
        {
            jagDraw::DrawablePrepPtr& dp( (*localCommands)[ jagDraw::DrawablePrep::VertexArrayObject_t ] );
            jagDraw::VertexArrayObjectPtr localVAO( boost::static_pointer_cast< jagDraw::VertexArrayObject >( dp ) );
            if( localVAO == vaop )
                localCommands->clear( jagDraw::DrawablePrep::VertexArrayObject_t );
        }

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

        if( offset > 0 )
        {
            // Step 3: Modify the DrawCommand indices to access the new location
            // of the current VAO -- where it got copied to in the master VAO.
            for( unsigned int idx=0; idx < node.getNumDrawables(); ++idx )
            {
                handleDrawable( node.getDrawable( idx ), vaop.get() );
            }
        }
    }

    node.traverse( *this );

    popCommandMap();
}


void BufferAggregationVisitor::offsetDrawElements( jagDraw::DrawElements* de, const size_t offset )
{
    jagDraw::BufferObjectPtr oldBuf( de->getElementBuffer() );
    if( _bufferObjectSet.find( oldBuf.get() ) != _bufferObjectSet.end() )
    {
        // Already processed this (shared) buffer object.
        return;
    }
    _bufferObjectSet.insert( oldBuf.get() );

    const size_t oldByteSize( oldBuf->getBuffer()->getSize() );
    size_t numElements( 0 );
    switch( de->getType() )
    {
    case GL_UNSIGNED_BYTE: numElements = oldByteSize / sizeof( GLubyte ); break;
    case GL_UNSIGNED_SHORT: numElements = oldByteSize / sizeof( GLushort ); break;
    case GL_UNSIGNED_INT: numElements = oldByteSize / sizeof( GLuint ); break;
    }

    GLint* newData( (GLint*)malloc( numElements * sizeof( GLint ) ) );
    GLint* newPtr( newData );

    size_t count( numElements );
    switch( de->getType() )
    {
    case GL_UNSIGNED_BYTE:
    {
        unsigned char* ptr( ( unsigned char* )( oldBuf->getBuffer()->getData() ) );
        while( count-- > 0 )
            *newPtr++ = (unsigned int)( *ptr++ + offset );
        break;
    }
    case GL_UNSIGNED_SHORT:
    {
        unsigned short* ptr( ( unsigned short* )( oldBuf->getBuffer()->getData() ) );
        while( count-- > 0 )
            *newPtr++ = (unsigned int)( *ptr++ + offset );
        break;
    }
    case GL_UNSIGNED_INT:
    {
        unsigned int* ptr( ( unsigned int* )( oldBuf->getBuffer()->getData() ) );
        while( count-- > 0 )
            *newPtr++ = (unsigned int)( *ptr++ + offset );
        break;
    }
    }

    jagBase::BufferPtr buf( new jagBase::Buffer( numElements * sizeof( GLuint ), newData ) );
    jagDraw::BufferObjectPtr newBuf( new jagDraw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, buf ) );
    de->setElementBuffer( newBuf );
    de->setType( GL_UNSIGNED_INT );

    free( newData );
}

void BufferAggregationVisitor::handleDrawable( jagDraw::DrawablePtr draw, const jagDraw::VertexArrayObject* vaop )
{
    const size_t offset( _offsetMap[ const_cast<jagDraw::VertexArrayObject*>(vaop) ] );

    BOOST_FOREACH( jagDraw::DrawCommandPtr& dcp, draw->getDrawCommandVec() )
    {
        if( dcp->getDrawCommandType() == jagDraw::DrawCommand::DrawElementsType )
        {
            jagDraw::DrawElements* de( static_cast< jagDraw::DrawElements* >( dcp.get() ) );
            offsetDrawElements( de, offset );
        }
        else if( dcp->getDrawCommandType() == jagDraw::DrawCommand::DrawArraysType )
        {
            jagDraw::DrawArrays* da( static_cast< jagDraw::DrawArrays* >( dcp.get() ) );
            da->setFirst( da->getFirst() + (GLint)( offset ) );
        }
        else
            JAG3D_CRITICAL( "Unknown command" );
    }
}


// jagUtil
}
