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

#include <jagDraw/DrawGraph.h>
#include <jagDraw/NodeContainer.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/Profile.h>
#include <jagBase/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


DrawGraph::DrawGraph( const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.draw.graph" : logName ),
    ObjectIDOwner()
{
}
DrawGraph::DrawGraph( const DrawGraph& rhs )
  : NodeContainerSimpleVec( rhs ),
    jagBase::LogBase( "jag.draw.graph" ),
    ObjectIDOwner( rhs ),
    _callbacks( rhs._callbacks ),
    _transformCallback( rhs._transformCallback )
{
}
DrawGraph::~DrawGraph()
{
}

DrawGraph& DrawGraph::operator=( const DrawGraph& rhs )
{
    NodeContainerSimpleVec::operator=( rhs );
    ObjectIDOwner::operator=( rhs );
    // LogBase does not support (and doesn't need) assignment operator.
    //jagBase::LogBase::operator=( rhs );
    _callbacks = rhs._callbacks;
    _transformCallback = rhs._transformCallback;

    return( *this );
}


DrawGraph::CallbackVec& DrawGraph::getCallbacks()
{
    return( _callbacks );
}


void DrawGraph::setTransformCallback( TransformCallbackPtr transformCallback )
{
    _transformCallback = transformCallback;
}
TransformCallbackPtr DrawGraph::getTransformCallback()
{
    return( _transformCallback );
}
void DrawGraph::setViewProj( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    _transformCallback->_transform.setView( view );
    _transformCallback->_transform.setProj( proj );
}


void DrawGraph::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    JAG3D_PROFILE( "DrawGraph execute()" );

    drawInfo.startFrame();

    BOOST_FOREACH( CallbackPtr cb, _callbacks )
    {
        if( !( (*cb)( *this, drawInfo ) ) )
            return;
    }

    internalExecute( drawInfo );

    JAG3D_ERROR_CHECK( "DrawGraph::execute()" );
}
void DrawGraph::internalExecute( DrawInfo& drawInfo )
{
    BOOST_FOREACH( NodeContainer& node, *this )
    {
        node.execute( drawInfo );
    }
}

void DrawGraph::setMaxContexts( const unsigned int numContexts )
{
    BOOST_FOREACH( NodeContainer& node, *this )
    {
        node.setMaxContexts( numContexts );
    }
}
void DrawGraph::deleteID( const jagDraw::jagDrawContextID contextID )
{
    BOOST_FOREACH( NodeContainer& node, *this )
    {
        node.deleteID( contextID );
    }
}


// jagDraw
}
