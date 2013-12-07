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

#include <jagSG/ExecuteVisitor.h>
#include <jag/draw/DrawInfo.h>
#include <jag/base/gmtlSupport.h>
#include <jag/draw/Error.h>
#include <jag/base/LogMacros.h>


namespace jagSG {


ExecuteVisitor::ExecuteVisitor( jag::draw::DrawInfo& drawInfo )
  : Visitor( "exec" ),
    _drawInfo( drawInfo )
{
    reset();
}
ExecuteVisitor::ExecuteVisitor( jag::draw::DrawInfo& drawInfo, jagSG::Node& node )
  : Visitor( "exec" ),
    _drawInfo( drawInfo )
{
    reset();
    node.accept( *this );
}
ExecuteVisitor::ExecuteVisitor( const ExecuteVisitor& rhs )
  : Visitor( rhs ),
    _drawInfo( rhs._drawInfo ),
    _transform( rhs._transform )
{
}
ExecuteVisitor::~ExecuteVisitor()
{
}


void ExecuteVisitor::setViewProj( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    _transform.setView( view );
    _transform.setProj( proj );
}


void ExecuteVisitor::reset()
{
    JAG3D_TRACE( "reset()" );

    resetCommandMap();
    resetMatrix();
}


void ExecuteVisitor::visit( jagSG::Node& node )
{
    JAG3D_TRACE( "visit()" );

    CommandMapStackHelper cmsh( *this, node.getCommandMap() );
    MatrixStackHelper msh( *this, node.getTransform() );

    if( msh.getDirty() )
    {
        _transform.setModel( _matrixStack.back() );
    }
    if( _transform.getDirty() != 0 )
        updateTransformUniforms();

    jag::draw::CommandMap delta( _drawInfo._current << _commandStack.back() );
    delta.execute( _drawInfo );

    // Execute the drawables
    node.execute( _drawInfo );

    Visitor::visit( node );


    if( msh.getDirty() )
    {
        _transform.setModel( _matrixStack[ _matrixStack.size()-2 ] );
    }
}


void ExecuteVisitor::updateTransformUniforms()
{
    jag::draw::UniformSetPtr usp( new jag::draw::UniformSet() );

    if( _transform.getDirty() & jag::base::TransformD::MODEL_VIEW_PROJ )
    {
        gmtl::Matrix44f mvpMat;
        gmtl::convert( mvpMat, _transform.getModelViewProj() );
        jag::draw::UniformPtr modelViewProj( new jag::draw::Uniform( "jagModelViewProjMatrix", mvpMat ) );
        usp->insert( modelViewProj );
    }
    if( _transform.getDirty() & jag::base::TransformD::MODEL_VIEW_INV_TRANS )
    {
        gmtl::Matrix33f mvitMat;
        gmtl::convert( mvitMat, _transform.getModelViewInvTrans() );
        jag::draw::UniformPtr modelViewInvTrans( new jag::draw::Uniform( "jagModelViewInvTransMatrix", mvitMat ) );
        usp->insert( modelViewInvTrans );
    }
    jag::draw::CommandMap& commands( _commandStack.back() );
    jag::draw::DrawablePrepPtr& uniformSet( commands[ jag::draw::Command::UniformSet_t ] );
    if( uniformSet == NULL )
    {
        commands.insert( usp );
    }
    else
    {
        uniformSet = uniformSet->combine( usp );
    }

    _transform.setDirty( 0 );
}


// jagSG
}
