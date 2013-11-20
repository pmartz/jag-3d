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

#include <jagDraw/Node.h>
#include <jagDraw/DrawablePrep.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/DrawCommand.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Program.h>
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

    if( drawInfo._controlFlags & DrawInfo::DRAW_BOUND )
    {
        // This draw traversal renders bounding volumes only.
        if( _boundDrawable == NULL )
            initBoundDrawable();
        setBoundUniforms( drawInfo );
        _boundDrawable->execute( drawInfo );
    }
    else
    {
        // Default draw traversal. Process attached CommandMap and Drawables
        // in the usual manner.
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

void Node::initBoundDrawable()
{
    _boundDrawable.reset( new jagDraw::Drawable() );
    _boundDrawable->addDrawCommand( jagDraw::DrawCommandPtr(
        new jagDraw::DrawArrays( GL_POINTS, 0, 1 ) ) );
}
void Node::setBoundUniforms( DrawInfo& drawInfo )
{
    CommandMap& current( drawInfo._current );
    if( !( current.contains( DrawablePrep::Program_t ) ) )
    {
        JAG3D_ERROR( "setBoundUniforms(): Current CommandMap does not contain Program_t." );
        return;
    }

    gmtl::AABoxd totalBound;
    BOOST_FOREACH( DrawablePtr drawable, _drawables )
        gmtl::extendVolume( totalBound, drawable->getBound( current )->asAABox() );
    float bbMinFloats[ 3 ], bbMaxFloats[ 3 ];
    gmtl::convert( bbMinFloats, totalBound.getMin() );
    gmtl::convert( bbMaxFloats, totalBound.getMax() );

    static Program::HashValue bbMinHash( Program::createHash( "jag3d_bbMin" ) );
    static Program::HashValue bbMaxHash( Program::createHash( "jag3d_bbMax" ) );

    jagDraw::ProgramPtr prog( boost::dynamic_pointer_cast< Program >( current[ DrawablePrep::Program_t ] ) );
    GLint loc( prog->getUniformLocation( bbMinHash ) );
    if( loc != -1 )
        glUniform3fv( loc, 1, bbMinFloats );
    loc = prog->getUniformLocation( bbMaxHash );
    if( loc != -1 )
        glUniform3fv( loc, 1, bbMaxFloats );
}


// jagDraw
}
