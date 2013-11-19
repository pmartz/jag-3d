/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 arising from the use of this software.
 
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
    _dirty( rhs._dirty ),
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
    _dirty._data.clear();
    markAllDirty();
    _commands = rhs._commands;
    _vertices = rhs._vertices;

    return( *this );
}


void VertexArrayObject::execute( DrawInfo& drawInfo )
{
    const unsigned int contextID( drawInfo._id );
    glBindVertexArray( getID( contextID ) );

    bool dirty( false );
    BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
    {
        if( dirty = vac->isDirty( contextID ) )
            break;
    }
    if( dirty || _dirty[ contextID ] )
    {
        BOOST_FOREACH( VertexArrayCommandPtr& vac, _commands )
        {
            vac->execute( drawInfo );
        }
        _dirty[ contextID ] = false;
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

    _dirty._data.resize( numContexts );
    markAllDirty();

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
    if( usage == Vertex )
        _vertices.push_back( vacp );

    markAllDirty();
}
void VertexArrayObject::removeVertexArrayCommand( const VertexArrayCommandPtr vacp )
{
    VertexArrayCommandVec::iterator it;
    for( it = _commands.begin(); it != _commands.end(); ++it )
    {
        if( vacp == *it )
        {
            _commands.erase( it );

            // Also erase from vertices, if present. We don't know the actual
            // usage, but if it was added with usage VERTICES, then we'll
            // find it here and erase it.
            VertexArrayCommandVec::iterator vIt;
            for( vIt = _vertices.begin(); vIt != _vertices.end(); ++vIt )
            if( vacp == *vIt )
                _vertices.erase( vIt );

            markAllDirty();
        }
    }
}
void VertexArrayObject::removeVertexArrayCommand( const std::string& userDataName )
{
    BOOST_FOREACH( VertexArrayCommandPtr item, _commands )
    {
        const std::string targetName( item->getUserDataName() );
        if( targetName == userDataName )
            removeVertexArrayCommand( item );
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

void VertexArrayObject::markAllDirty( const bool dirty )
{
    for( unsigned int idx=0; idx < _dirty._data.size(); idx++ )
    {
        _dirty[ idx ] = dirty;
    }
}
bool VertexArrayObject::isDirty( const unsigned int contextID ) const
{
    if( contextID < _dirty._data.size() )
        return( _dirty[ contextID ] != 0 );
    else
        return( true );
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
    _dirty[ contextID ] = true;

    JAG3D_ERROR_CHECK( "VertexArrayObject::internalInit()" );
}


// jagDraw
}
