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

#include <jagDraw/Node.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Error.h>
#include <jagBase/Profile.h>
#include <jagBase/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


Node::Node( CommandMapPtr commands, const std::string& logName )
  : jagBase::LogBase( logName.empty() ? "jag.draw.node" : logName ),
    ObjectIDOwner(),
    _matrix( gmtl::MAT_IDENTITY44D ),
    _commands( commands ),
    _distance( 0. )
{
}
Node::Node( const std::string& logName )
    : jagBase::LogBase( logName ),
    ObjectIDOwner(),
    jagBase::UserDataOwner(),
    _matrix( gmtl::MAT_IDENTITY44D ),
    _commands( ( CommandMap* )NULL ),
    _distance( 0. )
{
}
Node::Node( const Node& rhs )
  : jagBase::LogBase( rhs ),
    ObjectIDOwner( rhs ),
    jagBase::UserDataOwner( rhs ),
    _matrix( rhs._matrix ),
    _commands( rhs._commands ),
    _drawables( rhs._drawables ),
    _distance( rhs._distance ),
    _executeCallbacks( rhs._executeCallbacks )
{
}
Node::~Node()
{
}


void Node::setTransform( const gmtl::Matrix44d& matrix )
{
    _matrix = matrix;
}
gmtl::Matrix44d& Node::getTransform()
{
    return( _matrix );
}
const gmtl::Matrix44d& Node::getTransform() const
{
    return( _matrix );
}


Node::ExecuteCallbacks& Node::getExecuteCallbacks()
{
    return( _executeCallbacks );
}
const Node::ExecuteCallbacks& Node::getExecuteCallbacks() const
{
    return( _executeCallbacks );
}


void Node::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    JAG3D_PROFILE( "DrawNode execute()" );

    {
    JAG3D_PROFILE( "callbacks" );
    const ExecuteCallbacks& callbacks( getExecuteCallbacks() );
    BOOST_FOREACH( const jagDraw::Node::CallbackPtr& cb, callbacks )
    {
        if( !( (*cb)( *this, drawInfo ) ) )
        {
            return;
        }
    }
    }

    {
    JAG3D_PROFILE( "CommandMap" );
    CommandMap delta( drawInfo._current << (*_commands) );
    delta.execute( drawInfo );
    }

    {
        JAG3D_PROFILE( "Execute Drawable" );
        BOOST_FOREACH( DrawablePtr drawable, _drawables )
        {
            drawable->execute( drawInfo );
        }
    }

    JAG3D_ERROR_CHECK( "Node::execute()" );
}

void Node::setMaxContexts( const unsigned int numContexts )
{
    if( _commands == NULL )
    {
        JAG3D_ERROR( "setMaxContexts(): jagDraw::Node must have non-NULL _commands." );
    }
    else
    {
        _commands->setMaxContexts( numContexts );
    }

    BOOST_FOREACH( DrawablePtr drawable, _drawables )
    {
        drawable->setMaxContexts( numContexts );
    }
}
void Node::deleteID( const jagDraw::jagDrawContextID contextID )
{
    _commands->deleteID( contextID );

    BOOST_FOREACH( DrawablePtr drawable, _drawables )
    {
        drawable->deleteID( contextID );
    }
}


// jagDraw
}
