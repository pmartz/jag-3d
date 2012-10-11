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
Drawable::Drawable( const Drawable& rhs )
  : jagBase::LogBase( rhs ),
    _drawCommands( rhs._drawCommands )
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

void Drawable::getBound()
{
    JAG3D_NOTICE( "Drawable::getBound() is currently not implemented." );
}

void Drawable::setMaxContexts( const unsigned int numContexts )
{
    BOOST_FOREACH( DrawCommandPtr dcp, _drawCommands )
    {
        dcp->setMaxContexts( numContexts );
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


// jagDraw
}
