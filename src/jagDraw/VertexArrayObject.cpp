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

#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/VertexAttrib.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


VertexArrayObject::VertexArrayObject()
  : DrawablePrep( VertexArrayObject_t ),
    ObjectID()
{
}
VertexArrayObject::VertexArrayObject( const VertexArrayObject& rhs )
  : DrawablePrep( rhs ),
    ObjectID( rhs ),
    _initialized( rhs._initialized ),
    _commands( rhs._commands ),
    _vertices( rhs._vertices )
{
}
VertexArrayObject::~VertexArrayObject() 
{
    // TBD Handle object deletion
}


VertexArrayObject& VertexArrayObject::operator=( const VertexArrayObject& rhs )
{
    // TBD doesn't exist
    //DrawablePrep::operator=( rhs );
    // TBD doesn't exist, and probably not what we want anyway.
    //ObjectID::operator=( rhs );
    _initialized._data.clear();
    _commands = rhs._commands;
    _vertices = rhs._vertices;

    return( *this );
}


void VertexArrayObject::execute( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );

    glBindVertexArray( getID( contextID ) );
    GLboolean& initialized( _initialized[ contextID ] );

    if( initialized == GL_FALSE )
    {
        BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
        {
            vac->execute( drawInfo );
        }
        initialized = GL_TRUE;
    }
}

GLuint VertexArrayObject::getID( const unsigned int contextID )
{
    if( _ids[ contextID ] == 0 )
    {
        internalInit( contextID );
    }

    return( _ids[ contextID ] );
}

void VertexArrayObject::setMaxContexts( const unsigned int numContexts )
{
    ObjectID::setMaxContexts( numContexts );

    _initialized._data.resize( numContexts );

    BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
    {
        // Not all vertex array commands have objectIDs. Only set the
        // max context count on commands that have IDs.
        ObjectIDPtr objID( boost::dynamic_pointer_cast< ObjectID >( vac ) );
        if( objID != NULL )
            objID->setMaxContexts( numContexts );
        else
        {
            ObjectIDOwnerPtr objIDOwner( boost::dynamic_pointer_cast< ObjectIDOwner >( vac ) );
            if( objIDOwner != NULL )
                objIDOwner->setMaxContexts( numContexts );
        }
    }
}

void VertexArrayObject::addVertexArrayCommand( VertexArrayCommandPtr vacp, const UsageHint& usage )
{
    _commands.push_back( vacp );

    unsigned int idx;
    for( idx=0; idx < _initialized._data.size(); idx++ )
    {
        _initialized[ idx ] = GL_FALSE;
    }

    if( usage == Vertex )
    {
        _vertices.push_back( vacp );
    }
}
VertexArrayCommandPtr VertexArrayObject::getVertexArrayCommand( const VertexArrayCommand::Type& type, const UsageHint& usage ) const
{
    if( usage == Vertex )
    {
        BOOST_FOREACH( VertexArrayCommandPtr vacp, _vertices )
        {
            if( vacp->getType() == type )
                return( vacp );
        }
    }
    else
    {
        BOOST_FOREACH( VertexArrayCommandPtr vacp, _commands )
        {
            if( vacp->getType() == type )
                return( vacp );
        }
    }
    return( VertexArrayCommandPtr( (VertexArrayCommand*)NULL ) );
}

VertexArrayCommandVec& VertexArrayObject::getVertexArrayCommandList()
{
    return( _commands );
}
const VertexArrayCommandVec& VertexArrayObject::getVertexArrayCommandList() const
{
    return( _commands );
}


bool VertexArrayObject::isSameKind( const VertexArrayObject& rhs ) const
{
    if( _commands.empty() )
    {
        JAG3D_TRACE_STATIC( "jag.draw.vao", "isSameKind true (lhs is empty)." );
        return( true );
    }
    if( rhs._commands.empty() )
    {
        JAG3D_TRACE_STATIC( "jag.draw.vao", "isSameKind true (rhs is empty)." );
        return( true );
    }

    if( _commands.size() != rhs._commands.size() )
    {
        JAG3D_TRACE_STATIC( "jag.draw.vao", "isSameKind false (command sizes not equal)." );
        return( true );
    }

    for( size_t idx = 0; idx < _commands.size(); ++idx )
    {
        if( !( _commands[ idx ]->isSameKind( *( rhs._commands[ idx ] ) ) ) )
        {
            JAG3D_TRACE_STATIC( "jag.draw.vao", "isSameKind false (VAC mismatch)." );
            return( false );
        }
    }

    JAG3D_TRACE_STATIC( "jag.draw.vao", "isSameKind true." );
    return( true );
}

size_t VertexArrayObject::combine( const VertexArrayObject& rhs )
{
    if( _commands.empty() )
    {
        *this = rhs;
        return( 0 );
    }

    // First, check for consistent buffer sizes and establish the
    // return value.
    size_t elementCount( 0 );
    size_t lastBufferSizeBytes( 0 );
    for( size_t idx = 0; idx < _commands.size(); ++idx )
    {
        if( _commands[ idx ]->getType() == VertexArrayCommand::BufferObject_t )
        {
            BufferObjectPtr buf( boost::static_pointer_cast< BufferObject >( _commands[ idx ] ) );
            lastBufferSizeBytes = buf->getBuffer()->getSize();
        }
        else if( _commands[ idx ]->getType() == VertexArrayCommand::VertexAttrib_t )
        {
            VertexAttribPtr va( boost::static_pointer_cast< VertexAttrib >( _commands[ idx ] ) );
            size_t elements = lastBufferSizeBytes / va->getActualStride();
            if( elementCount == 0 )
            {
                elementCount = elements;
            }
            else if( elementCount != elements )
            {
                JAG3D_ERROR_STATIC( "jag.draw.vao", "combine(): inconsistent element count." );
                return( 0 );
            }
        }
    }

    // If there is nothing in the source VAO (rhs), nothing to do.
    if( rhs._commands.empty() )
    {
        return( elementCount );
    }

    for( size_t idx = 0; idx < _commands.size(); ++idx )
    {
        if( _commands[ idx ]->getType() == VertexArrayCommand::BufferObject_t )
        {
            // Append the right-hand buffer to the left-hand buffer.
            BufferObjectPtr leftBuf( boost::static_pointer_cast< BufferObject >( _commands[ idx ] ) );
            const BufferObjectPtr& rightBuf( boost::static_pointer_cast< BufferObject >( rhs._commands[ idx ] ) );
            leftBuf->getBuffer()->append( *( rightBuf->getBuffer() ) );
        }
        else if( _commands[ idx ]->getType() == VertexArrayCommand::VertexAttrib_t )
        {
            // Nothing to do?
            //VertexAttribPtr& leftAttrib( boost::static_pointer_cast< VertexAttrib >( _commands[ idx ] ) );
            //const VertexAttribPtr& rightAttrib( boost::static_pointer_cast< VertexAttrib >( rhs._commands[ idx ] ) );
        }
    }

    return( elementCount );
}


void VertexArrayObject::internalInit( const unsigned int contextID )
{
    glGenVertexArrays( 1, &( _ids[ contextID ] ) );
    _initialized[ contextID ] = GL_FALSE;

    JAG3D_ERROR_CHECK( "VertexArrayObject::internalInit()" );
}


// jagDraw
}
