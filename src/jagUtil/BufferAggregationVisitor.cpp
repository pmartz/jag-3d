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

#include <jagUtil/BufferAggregationVisitor.h>
#include <jagUtil/ResetBoundsVisitor.h>
#include <jag/draw/Drawable.h>
#include <jag/draw/DrawCommand.h>
#include <jag/sg/Node.h>

#include <boost/foreach.hpp>

#include <sstream>


namespace jagUtil
{


BufferAggregationVisitor::BufferAggregationVisitor( jag::sg::NodePtr node, const std::string& logName )
    : jag::sg::VisitorBase( ( logName.empty() ? "bufagg" : logName ),
                          ( logName.empty() ? "" : logName ) ),
    _combineElementBuffers( true )
{
    _vaop = jag::draw::VertexArrayObjectPtr( new jag::draw::VertexArrayObject() );

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
    jag::draw::CommandMapPtr commands( node->getCommandMap() );
    if( commands == NULL )
    {
        commands.reset( new jag::draw::CommandMap() );
        node->setCommandMap( commands );
    }
    commands->insert( _vaop );

    JAG3D_TRACE( "Finished." );
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
    : jag::sg::VisitorBase( rhs ),
    _combineElementBuffers( rhs._combineElementBuffers )
{
    reset();
}
BufferAggregationVisitor::~BufferAggregationVisitor()
{
}

void BufferAggregationVisitor::reset()
{
    _vaop.reset( ( jag::draw::VertexArrayObject* )NULL );
    _nodeSet.clear();
    _elementBuffers.clear();
    _offsetMap.clear();
}



void BufferAggregationVisitor::visit( jag::sg::Node& node )
{
    if( node.getBoundDirty() )
    {
        JAG3D_WARNING( "visit(): Detected dirty bounds. App should call getBound() before invoking this visitor." );
    }

    if( _nodeSet.find( &node ) != _nodeSet.end() )
    {
        JAG3D_INFO( "Found shared node \"" + node.getUserDataName() + "\"." );
        return;
    }
    _nodeSet.insert( &node );

    jag::draw::CommandMapPtr& localCommands( node.getCommandMap() );
    CommandMapStackHelper cmsh( *this, localCommands );


    // Get the VAO in effect at this Node.
    jag::draw::CommandMap& commands( _commandStack.back() );
    jag::draw::DrawablePrepPtr& drawablePrep( commands[ jag::draw::Command::VertexArrayObject_t ] );
    jag::draw::VertexArrayObjectPtr vaop( boost::static_pointer_cast< jag::draw::VertexArrayObject >( drawablePrep ) );

    if( ( vaop != NULL ) && _vaop->isSameKind( *vaop ) )
    {
        // We're going to aggregate the current VAO into the master.
        // Step 1: If the current VAO in the command stack matches the Node's
        // VAO, then remove the Node's VAO, because it will use the master
        // from now on.
        if( localCommands != NULL )
        {
            jag::draw::DrawablePrepPtr& dp( (*localCommands)[ jag::draw::Command::VertexArrayObject_t ] );
            jag::draw::VertexArrayObjectPtr localVAO( boost::static_pointer_cast< jag::draw::VertexArrayObject >( dp ) );
            if( localVAO == vaop )
            {
                localCommands->clear( jag::draw::Command::VertexArrayObject_t );
                if( localCommands->empty() )
                    node.setCommandMap( jag::draw::CommandMapPtr( NULL ) );
            }
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

        // Step 3: Modify the DrawCommand indices to access the new location
        // of the current VAO -- where it got copied to in the master VAO.
        for( unsigned int idx=0; idx < node.getNumDrawables(); ++idx )
        {
            handleDrawable( node.getDrawable( idx ), vaop.get() );
        }
    }

    node.traverse( *this );
}


void BufferAggregationVisitor::offsetDrawElements( jag::draw::DrawElementsBase* deBase, const size_t offset )
{
    jag::draw::BufferObjectPtr oldBuf( deBase->getElementBuffer() );
    const size_t oldByteSize( oldBuf->getBuffer()->getSize() );
    size_t numElements( 0 );
    switch( deBase->getType() )
    {
    case GL_UNSIGNED_BYTE: numElements = oldByteSize / sizeof( GLubyte ); break;
    case GL_UNSIGNED_SHORT: numElements = oldByteSize / sizeof( GLushort ); break;
    case GL_UNSIGNED_INT: numElements = oldByteSize / sizeof( GLuint ); break;
    }

    GLint* newData( (GLint*)malloc( numElements * sizeof( GLint ) ) );
    GLint* newPtr( newData );

    size_t count( numElements );
    switch( deBase->getType() )
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

    jag::base::BufferPtr buf( new jag::base::Buffer( numElements * sizeof( GLuint ), newData ) );
    jag::draw::BufferObjectPtr newBuf( new jag::draw::BufferObject( GL_ELEMENT_ARRAY_BUFFER, buf ) );
    deBase->setElementBuffer( newBuf );
    deBase->setType( GL_UNSIGNED_INT );

    free( newData );
}

void BufferAggregationVisitor::combineElementBuffer( jag::draw::DrawElementsBase* deBase )
{
    jag::draw::BufferObjectPtr rightBuf( deBase->getElementBuffer() );
    if( _elements == NULL )
    {
        // First buffer. Take it as the master, and return.
        _elements = rightBuf;
        return;
    }

    jag::base::BufferPtr lhs( _elements->getBuffer() );
    const jag::base::BufferPtr rhs( rightBuf->getBuffer() );
    const size_t requiredSize( lhs->getSize() + rhs->getSize() );
    if( lhs->getMaxSize() < requiredSize )
        lhs->setMaxSize( std::max< size_t >( lhs->getMaxSize() * 2, requiredSize ) );
    size_t lhsIndices( lhs->getSize() );
    lhs->append( *rhs );

    deBase->setElementBuffer( _elements );
    deBase->setIndices( ( const GLvoid* ) lhsIndices );
}

void BufferAggregationVisitor::handleDrawable( jag::draw::DrawablePtr draw, const jag::draw::VertexArrayObject* vaop )
{
    const size_t offset( _offsetMap[ const_cast<jag::draw::VertexArrayObject*>(vaop) ] );

    BOOST_FOREACH( jag::draw::DrawCommandPtr& dcp, draw->getDrawCommandVec() )
    {
        jag::draw::DrawElementsBase* deBase( dynamic_cast< jag::draw::DrawElementsBase* >( dcp.get() ) );
        jag::draw::DrawArraysBase* daBase( dynamic_cast< jag::draw::DrawArraysBase* >( dcp.get() ) );
        if( deBase != NULL )
        {
            jag::draw::BufferObjectPtr oldBuf( deBase->getElementBuffer() );
            if( _elementBuffers.find( oldBuf.get() ) != _elementBuffers.end() )
            {
                // Already processed this (shared) buffer object.
                continue;
            }
            _elementBuffers.insert( oldBuf.get() );

            offsetDrawElements( deBase, offset );
            if( _combineElementBuffers )
                combineElementBuffer( deBase );
        }
        else if( daBase != NULL )
        {
            daBase->setFirst( daBase->getFirst() + (GLint)( offset ) );
        }
        else
        {
            jag::draw::MultiDrawArrays* mda( dynamic_cast< jag::draw::MultiDrawArrays* >( dcp.get() ) );
            if( mda != NULL )
            {
                jag::draw::GLintVec& first( mda->getFirst() );
                for( GLsizei idx = 0; idx < mda->getPrimcount(); ++idx )
                    first[ idx ] += (GLint)( offset );
            }
            else
                JAG3D_CRITICAL( "Unsupported DrawCommand" );
        }
    }
}


// jagUtil
}
