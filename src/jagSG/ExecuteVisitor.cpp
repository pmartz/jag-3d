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

#include <jagSG/ExecuteVisitor.h>
#include <jagDraw/DrawInfo.h>
#include <jagBase/gmtlSupport.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>


namespace jagSG {


ExecuteVisitor::ExecuteVisitor( jagDraw::DrawInfo& drawInfo )
  : Visitor( "exec" ),
    _drawInfo( drawInfo )
{
    reset();
}
ExecuteVisitor::ExecuteVisitor( jagDraw::DrawInfo& drawInfo, jagSG::Node& node )
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

    jagDraw::CommandMap delta( _drawInfo._current << _commandStack.back() );
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
    jagDraw::UniformSetPtr usp( new jagDraw::UniformSet() );

    if( _transform.getDirty() & jagBase::TransformD::MODEL_VIEW_PROJ )
    {
        gmtl::Matrix44f mvpMat;
        gmtl::convert( mvpMat, _transform.getModelViewProj() );
        jagDraw::UniformPtr modelViewProj( new jagDraw::Uniform( "jagModelViewProjMatrix", mvpMat ) );
        usp->insert( modelViewProj );
    }
    if( _transform.getDirty() & jagBase::TransformD::MODEL_VIEW_INV_TRANS )
    {
        gmtl::Matrix33f mvitMat;
        gmtl::convert( mvitMat, _transform.getModelViewInvTrans() );
        jagDraw::UniformPtr modelViewInvTrans( new jagDraw::Uniform( "jagModelViewInvTransMatrix", mvitMat ) );
        usp->insert( modelViewInvTrans );
    }
    jagDraw::CommandMap& commands( _commandStack.back() );
    jagDraw::DrawablePrepPtr& uniformSet( commands[ jagDraw::DrawablePrep::UniformSet_t ] );
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
