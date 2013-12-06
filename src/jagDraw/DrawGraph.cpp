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

#include <jagDraw/DrawGraph.h>
#include <jagDraw/DrawNodeContainer.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jag/base/Profile.h>
#include <jag/base/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


DrawGraph::DrawGraph( const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.draw.graph" : logName ),
    ObjectIDOwner()
{
}
DrawGraph::DrawGraph( const DrawGraph& rhs )
  : DrawNodeContainerSimpleVec( rhs ),
    jag::base::LogBase( "jag.draw.graph" ),
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
    DrawNodeContainerSimpleVec::operator=( rhs );
    ObjectIDOwner::operator=( rhs );
    // LogBase does not support (and doesn't need) assignment operator.
    //jag::base::LogBase::operator=( rhs );
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
    BOOST_FOREACH( DrawNodeContainer& node, *this )
    {
        node.execute( drawInfo );
    }
}

void DrawGraph::setMaxContexts( const unsigned int numContexts )
{
    BOOST_FOREACH( DrawNodeContainer& node, *this )
    {
        node.setMaxContexts( numContexts );
    }
}
void DrawGraph::deleteID( const jagDraw::jagDrawContextID contextID )
{
    BOOST_FOREACH( DrawNodeContainer& node, *this )
    {
        node.deleteID( contextID );
    }
}


// jagDraw
}
