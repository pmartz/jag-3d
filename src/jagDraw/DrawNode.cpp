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

#include <jagDraw/DrawNode.h>
#include <jagDraw/Command.h>
#include <jagDraw/DrawInfo.h>
#include <jagDraw/Drawable.h>
#include <jagDraw/DrawCommand.h>
#include <jagDraw/CommandMap.h>
#include <jagDraw/Program.h>
#include <jagDraw/Error.h>
#include <jag/base/Profile.h>
#include <jag/base/LogMacros.h>

#include <boost/foreach.hpp>


namespace jagDraw {


DrawNode::DrawNode( CommandMapPtr commands, const std::string& logName )
  : jag::base::LogBase( logName.empty() ? "jag.draw.node" : logName ),
    ObjectIDOwner(),
    _matrix( gmtl::MAT_IDENTITY44D ),
    _commands( commands ),
    _distance( 0. )
{
}
DrawNode::DrawNode( const std::string& logName )
    : jag::base::LogBase( logName ),
    ObjectIDOwner(),
    jag::base::UserDataOwner(),
    _matrix( gmtl::MAT_IDENTITY44D ),
    _commands( ( CommandMap* )NULL ),
    _distance( 0. )
{
}
DrawNode::DrawNode( const DrawNode& rhs )
  : jag::base::LogBase( rhs ),
    ObjectIDOwner( rhs ),
    jag::base::UserDataOwner( rhs ),
    _matrix( rhs._matrix ),
    _commands( rhs._commands ),
    _drawables( rhs._drawables ),
    _distance( rhs._distance ),
    _executeCallbacks( rhs._executeCallbacks )
{
}
DrawNode::~DrawNode()
{
}


void DrawNode::setTransform( const gmtl::Matrix44d& matrix )
{
    _matrix = matrix;
}
gmtl::Matrix44d& DrawNode::getTransform()
{
    return( _matrix );
}
const gmtl::Matrix44d& DrawNode::getTransform() const
{
    return( _matrix );
}


DrawNode::ExecuteCallbacks& DrawNode::getExecuteCallbacks()
{
    return( _executeCallbacks );
}
const DrawNode::ExecuteCallbacks& DrawNode::getExecuteCallbacks() const
{
    return( _executeCallbacks );
}


void DrawNode::execute( DrawInfo& drawInfo )
{
    JAG3D_TRACE( "execute()" );

    JAG3D_PROFILE( "DrawNode execute()" );

    {
    JAG3D_PROFILE( "callbacks" );
    const ExecuteCallbacks& callbacks( getExecuteCallbacks() );
    BOOST_FOREACH( const jagDraw::DrawNode::CallbackPtr& cb, callbacks )
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
        // Requires a program that accepts a single point as input,
        // with uniforms for MVP matrix and BB min & max. Generates
        // bounding volume primitives in geometry shader. See
        // data/bound.* shader source.
        setBoundUniforms( drawInfo );
        glDrawArrays( GL_POINTS, 0, 1 );
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

    JAG3D_ERROR_CHECK( "DrawNode::execute()" );
}

void DrawNode::setMaxContexts( const unsigned int numContexts )
{
    if( _commands == NULL )
    {
        JAG3D_ERROR( "setMaxContexts(): jagDraw::DrawNode must have non-NULL _commands." );
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
void DrawNode::deleteID( const jagDraw::jagDrawContextID contextID )
{
    _commands->deleteID( contextID );

    BOOST_FOREACH( DrawablePtr drawable, _drawables )
    {
        drawable->deleteID( contextID );
    }
}

void DrawNode::setBoundUniforms( DrawInfo& drawInfo )
{
    CommandMap& current( drawInfo._current );
    if( !( current.contains( Command::Program_t ) ) )
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

    jagDraw::ProgramPtr prog( boost::dynamic_pointer_cast< Program >( current[ Command::Program_t ] ) );
    GLint loc( prog->getUniformLocation( bbMinHash ) );
    if( loc != -1 )
        glUniform3fv( loc, 1, bbMinFloats );
    loc = prog->getUniformLocation( bbMaxHash );
    if( loc != -1 )
        glUniform3fv( loc, 1, bbMaxFloats );

    JAG3D_ERROR_CHECK( "setBoundUniforms()" );
}


// jagDraw
}
