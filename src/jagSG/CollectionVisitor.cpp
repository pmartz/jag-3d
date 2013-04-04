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

#include <jagSG/CollectionVisitor.h>
#include <jagSG/Node.h>
#include <jagDraw/Node.h>
#include <jagBase/gmtlSupport.h>
#include <jagDraw/Error.h>
#include <jagBase/LogMacros.h>


namespace jagSG {


CollectionVisitor::CollectionVisitor()
  : Visitor( "collect" )
{
    reset();
}
CollectionVisitor::CollectionVisitor( const CollectionVisitor& rhs )
  : Visitor( rhs ),
    _transform( rhs._transform )
{
}
CollectionVisitor::~CollectionVisitor()
{
}


void CollectionVisitor::setViewProj( const gmtl::Matrix44d& view, const gmtl::Matrix44d& proj )
{
    _transform.setView( view );
    _transform.setProj( proj );
}


void CollectionVisitor::reset()
{
    JAG3D_TRACE( "reset()" );

    resetCommandMap();
    resetMatrix();

    _drawGraph.clear();
}


void CollectionVisitor::visit( jagSG::Node& node )
{
    JAG3D_TRACE( "visit()" );

    pushCommandMap( node.getCommandMap() );

    const bool modelDirty( node.getTransform() != gmtl::MAT_IDENTITY44D );
    if( modelDirty )
    {
        pushMatrix( node.getTransform() );
        _transform.setModel( _matrixStack.back() );
    }

    // TBD if all collection callbacks pass...
    {
        if( _transform.getDirty() != 0 )
            updateTransformUniforms();

        if( node.getNumDrawables() > 0 )
        {
            JAG3D_WARNING( "TBD Must allocate new CommandMapPtr?" );
            jagDraw::CommandMapPtr commands( new jagDraw::CommandMap(
                 _commandStack.back() ) );

            jagDraw::Node drawNode( commands );
            for( unsigned int idx=0; idx < node.getNumDrawables(); ++idx )
            {
                drawNode.addDrawable( node.getDrawable( idx ) );
            }
            _drawGraph.push_back( drawNode );
        }

        Visitor::visit( node );
    }


    if( modelDirty )
    {
        popMatrix();
        _transform.setModel( _matrixStack.empty() ?
            gmtl::MAT_IDENTITY44D : _matrixStack.back() );
    }

    popCommandMap();
}

const jagDraw::DrawNodeSimpleVec& CollectionVisitor::getDrawGraph() const
{
    return( _drawGraph );
}


void CollectionVisitor::updateTransformUniforms()
{
    jagDraw::UniformSetPtr usp( new jagDraw::UniformSet() );

    if( _transform.getDirty() & jagBase::TransformD::MODEL_VIEW_PROJ_DIRTY )
    {
        gmtl::Matrix44f mvpMat;
        gmtl::convert( mvpMat, _transform.getModelViewProj() );
        jagDraw::UniformPtr modelViewProj( new jagDraw::Uniform( "jagModelViewProjMatrix", mvpMat ) );
        usp->insert( modelViewProj );
    }
    if( _transform.getDirty() & jagBase::TransformD::MODEL_VIEW_INV_TRANS_DIRTY )
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
