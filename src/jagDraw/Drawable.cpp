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

#include <jagDraw/PlatformOpenGL.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/Program.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagDraw/BufferObject.h>
#include <jagDraw/DrawCommand.h>
#include <jagDraw/VertexArrayObject.h>
#include <jagDraw/VertexAttribContainer.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>
#include <jagBase/Profile.h>

#include <boost/foreach.hpp>

#include <sstream>


namespace jagDraw {


Drawable::Drawable()
  : jagBase::LogBase( "jag.draw.drawable" )
{
}
Drawable::Drawable( const Drawable& rhs )
  : jagBase::LogBase( rhs ),
    _drawCommands( rhs._drawCommands ),
    _bounds( rhs._bounds ),
    _initialBound( rhs._initialBound ),
    _computeBoundCallback( rhs._computeBoundCallback )
{
}
Drawable::~Drawable()
{
}


void Drawable::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "Drawable::execute" );

    const size_t sz( _drawCommands.size() );
    if( sz == 0 )
        return;

    DrawCommandPtr* ptr( &( _drawCommands[ 0 ] ) );
    const DrawCommandPtr* end( &( _drawCommands[ sz - 1 ] ) );
    while( ptr <= end )
    {
        (*ptr++)->execute( drawInfo );
    }

    JAG3D_ERROR_CHECK( "Drawable::execute()" );
}

BoundPtr Drawable::getBound( const VertexArrayObjectPtr vaop )
{
    BoundInfo& boundInfo( _bounds[ vaop.get() ] );
    if( boundInfo._dirty )
    {
        if( boundInfo._bound == NULL )
        {
            if( _initialBound == NULL )
                boundInfo._bound = BoundPtr( new BoundAABox() );
            else
                boundInfo._bound = _initialBound->clone();
        }
        if( _computeBoundCallback != NULL )
            (*_computeBoundCallback)( boundInfo._bound, vaop );
        else
            computeBounds( boundInfo._bound, vaop );
        boundInfo._dirty = false;
    }

    return( boundInfo._bound );
}

void Drawable::setInitialBound( BoundPtr initialBound )
{
    _initialBound = initialBound;

    BOOST_FOREACH( BoundMap::value_type& mapElement, _bounds )
    {
        mapElement.second._dirty = true;
    }
}
BoundPtr Drawable::getInitialBound() const
{
    return( _initialBound );
}

void Drawable::setBoundDirty( const VertexArrayObjectPtr vaop, const bool dirty )
{
    _bounds[ vaop.get() ]._dirty = true;
}
bool Drawable::getBoundDirty( const VertexArrayObjectPtr vaop ) const
{
    BoundMap::const_iterator it( _bounds.find( vaop.get() ) );
    if( it != _bounds.end() )
        return( it->second._dirty );
    else
        return( true );
}

void Drawable::computeBounds( BoundPtr _bound, const VertexArrayObjectPtr vaop )
{
    if( vaop == NULL )
    {
        JAG3D_WARNING( "computeBounds() encountered NULL vertex array object." );
        return;
    }

    BufferObjectPtr bop( boost::dynamic_pointer_cast< BufferObject >(
        vaop->getVertexArrayCommand( VertexArrayCommand::BufferObject_t, VertexArrayObject::Vertex ) ) );
    VertexAttribPtr verts( boost::dynamic_pointer_cast< VertexAttrib >(
        vaop->getVertexArrayCommand( VertexArrayCommand::VertexAttrib_t, VertexArrayObject::Vertex ) ) );
    if( ( bop == NULL ) || ( verts == NULL ) )
    {
        JAG3D_WARNING( "computeBounds(): NULL buffer object or vertex attrib (VAO has no vertex data)." );
        return;
    }

    GLint size;
    GLenum type;
    verts->getSizeType( size, type );
    typedef std::pair< GLint, GLenum > IntEnum;
    const IntEnum vertType( size, type );

    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        if( vertType == IntEnum( 3, GL_FLOAT ) )
        {
            VertexAttribContainer< gmtl::Point3f > vac( bop, verts, dcp );
            VertexAttribContainer< gmtl::Point3f >::iterator pointIter( vac );
            for( pointIter = vac.begin(); pointIter != vac.end(); ++pointIter )
                _bound->expand( *pointIter );
        }
        else if( vertType == IntEnum( 3, GL_DOUBLE ) )
        {
            VertexAttribContainer< gmtl::Point3d > vac( bop, verts, dcp );
            VertexAttribContainer< gmtl::Point3d >::iterator pointIter( vac );
            for( pointIter = vac.begin(); pointIter != vac.end(); ++pointIter )
                _bound->expand( *pointIter );
        }
        else
        {
            std::ostringstream ostr;
            ostr << "computeBounds(): Unsupported VertexAttrib type/size combination.\n";
            ostr << "\tType: " << std::hex << type << ", size: " << std::dec << size;
            JAG3D_ERROR( ostr.str() );
        }
    }
}

Drawable::BoundInfo::BoundInfo()
    : _dirty( true )
{
}



void Drawable::addDrawCommand( DrawCommandPtr dcp )
{
    _drawCommands.push_back( dcp );
}

DrawCommandVec& Drawable::getDrawCommandVec()
{
    return( _drawCommands );
}
const DrawCommandVec& Drawable::getDrawCommandVec() const
{
    return( _drawCommands );
}


void Drawable::setMaxContexts( const unsigned int numContexts )
{
    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        dcp->setMaxContexts( numContexts );
    }
}
void Drawable::deleteID( const jagDraw::jagDrawContextID contextID )
{
    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        dcp->deleteID( contextID );
    }
}


// jagDraw
}
