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
#include <jagBase/LogMacros.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>

#include <boost/foreach.hpp>

#include <sstream>


namespace jagDraw {


Drawable::Drawable()
  : jagBase::LogBase( "jag.draw.drawable" ),
    _boundDirty( true )
{
}
Drawable::Drawable( const Drawable& rhs )
  : jagBase::LogBase( rhs ),
    _drawCommands( rhs._drawCommands ),
    _bound( rhs._bound ),
    _boundDirty( rhs._boundDirty ),
    _computeBoundCallback( rhs._computeBoundCallback )
{
}
Drawable::~Drawable()
{
}


void Drawable::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        dcp->execute( drawInfo );
    }

    JAG3D_ERROR_CHECK( "Drawable::execute()" );
}

BoundPtr Drawable::getBound( const CommandMap& commands )
{
    if( getBoundDirty( commands ) )
    {
        if( _bound == NULL )
            _bound = BoundPtr( new BoundAABox() );
        if( _computeBoundCallback != NULL )
            (*_computeBoundCallback)( _bound, commands );
        else
            computeBounds( _bound, commands );
        setBoundDirty( commands, false );
    }

    return( _bound );
}

void Drawable::setBoundDirty( const CommandMap& commands, const bool dirty )
{
    _boundDirty = dirty;
}
bool Drawable::getBoundDirty( const CommandMap& commands ) const
{
    return( _boundDirty );
}

void Drawable::computeBounds( BoundPtr _bound, const CommandMap& commands )
{
    if( ! commands.contains( DrawablePrep::VertexArrayObject_t ) )
    {
        JAG3D_WARNING( "computeBounds() encountered NULL vertex array object." );
        return;
    }
    const DrawablePrepPtr& drawablePrep( commands[ DrawablePrep::VertexArrayObject_t ] );
    const VertexArrayObjectPtr vaop( boost::dynamic_pointer_cast< VertexArrayObject >( drawablePrep ) );

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
