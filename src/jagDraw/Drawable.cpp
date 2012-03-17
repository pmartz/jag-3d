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

    BOOST_FOREACH( VertexArrayCommandPtr vacp, _vertexArrayCommands )
    {
        (*vacp)( drawInfo );
    }

    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        (*dcp)( drawInfo );
    }

    JAG_ERROR_CHECK( "Drawable::operator()" );
}

void Drawable::getBound()
{
    JAG3D_NOTICE( "Drawable::getBound() is currently not implemented." );
}


void Drawable::addDrawablePrep( DrawablePrepPtr dpp )
{
    _drawablePrep.push_back( dpp );
}

DrawablePrepList& Drawable::getDrawablePrepList()
{
    return( _drawablePrep );
}
const DrawablePrepList& Drawable::getDrawablePrepList() const
{
    return( _drawablePrep );
}

void Drawable::addVertexArrayCommand( VertexArrayCommandPtr vacp, const VertexArrayCommand::UsageHint& usageHint )
{
    _vertexArrayCommands.push_back( vacp );

    if( usageHint == VertexArrayCommand::Vertex )
    {
        _vertices.push_back( vacp );
    }
}

VertexArrayCommandList& Drawable::getVertexArrayCommandList()
{
    return( _vertexArrayCommands );
}
const VertexArrayCommandList& Drawable::getVertexArrayCommandList() const
{
    return( _vertexArrayCommands );
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