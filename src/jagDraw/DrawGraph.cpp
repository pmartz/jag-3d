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
    _transformCallback->getTransform().setView( view );
    _transformCallback->getTransform().setProj( proj );
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
