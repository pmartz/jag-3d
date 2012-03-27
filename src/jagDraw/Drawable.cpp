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
#include <jagBase/LogMacros.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>

#include <boost/foreach.hpp>


namespace jagDraw {


Drawable::Drawable()
  : jagBase::LogBase( "jag.draw.drawable" )
{
}
Drawable::~Drawable()
{
}


void Drawable::operator()( DrawInfo& drawInfo )
{
    BOOST_FOREACH( DrawablePrepPtr dpp, _drawablePrep )
    {
        (*dpp)( drawInfo );
    }

    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        (*dcp)( drawInfo );
    }

    JAG3D_ERROR_CHECK( "Drawable::operator()" );
}

void Drawable::getBound()
{
    JAG3D_NOTICE( "Drawable::getBound() is currently not implemented." );
}

void Drawable::setMaxContexts( const unsigned int numContexts )
{
    BOOST_FOREACH( DrawablePrepPtr dpp, _drawablePrep )
    {
        Program* prog( dynamic_cast< Program* >( dpp.get() ) );
        if( prog != NULL )
            prog->setMaxContexts( numContexts );
        VertexArrayObject* vaop( dynamic_cast< VertexArrayObject* >( dpp.get() ) );
        if( vaop != NULL )
            vaop->setMaxContexts( numContexts );
    }

    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        dcp->setMaxContexts( numContexts );
    }
}


void Drawable::addDrawablePrep( DrawablePrepPtr dpp )
{
    _drawablePrep.push_back( dpp );
}
void Drawable::insertDrawablePrep( DrawablePrepPtr dpp, unsigned int pos )
{
    if( pos >= _drawablePrep.size() )
    {
        _drawablePrep.resize( pos+1 );
    }
    else
    {
        _drawablePrep.resize( _drawablePrep.size()+1 );
        unsigned int idx;
        for( idx = _drawablePrep.size()-1; idx>pos; idx-- )
            _drawablePrep[ idx ] = _drawablePrep[ idx-1 ];
    }
    _drawablePrep[ pos ] = dpp;
}

DrawablePrepList& Drawable::getDrawablePrepList()
{
    return( _drawablePrep );
}
const DrawablePrepList& Drawable::getDrawablePrepList() const
{
    return( _drawablePrep );
}


void Drawable::addDrawCommand( DrawCommandPtr dcp )
{
    _drawCommands.push_back( dcp );
}

DrawCommandList& Drawable::getDrawCommandList()
{
    return( _drawCommands );
}
const DrawCommandList& Drawable::getDrawCommandList() const
{
    return( _drawCommands );
}


// jagDraw
}
