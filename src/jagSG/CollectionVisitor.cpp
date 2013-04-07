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
#include <jagBase/Profile.h>
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
    reset();
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

    _infoPtr = CollectionInfoPtr( new CollectionInfo() );
}


void CollectionVisitor::visit( jagSG::Node& node )
{
    JAG3D_TRACE( "visit()" );

    JAG3D_PROFILE( "CV visit()" );

    pushCommandMap( node.getCommandMap() );

    const bool modelDirty( node.getTransform() != gmtl::MAT_IDENTITY44D );
    if( modelDirty )
    {
        JAG3D_PROFILE( "CV transform" );
        pushMatrix( node.getTransform() );
        _transform.setModel( _matrixStack.back() );
    }

    // TBD do we have to always do this, or can we get away with only
    // doing it when modelDirty == true?
    _infoPtr->reset();
    Node::CallbackInfo* info( _infoPtr.get() );

    bool collect( true );
    jagSG::Node::CollectionCallbacks& callbacks( node.getCollectionCallbacks() );
    BOOST_FOREACH( jagSG::Node::CallbackPtr cb, callbacks )
    {
        if( !( (*cb)( *this, info ) ) )
            //boost::static_pointer_cast< Node::CallbackInfoPtr >( _infoPtr ) ) ) )
        {
            collect = false;
            break;
        }
    }
    if( collect )
    {
        {
        JAG3D_PROFILE( "CV transform" );
        if( _transform.getDirty() != 0 )
            updateTransformUniforms();
        }

        const unsigned int numDrawables( node.getNumDrawables() );
        if( numDrawables > 0 )
        {
            JAG3D_PROFILE( "CV DrawNode processing" );

            _drawGraph.resize( _drawGraph.size()+1 );
            jagDraw::Node& drawNode( _drawGraph[ _drawGraph.size()-1 ] );

            //JAG3D_WARNING( "TBD Must allocate new CommandMapPtr?" );
            jagDraw::CommandMapPtr commands( new jagDraw::CommandMap(
                 _commandStack.back() ) );
            drawNode.setCommandMap( commands );

            for( unsigned int idx=0; idx < numDrawables; ++idx )
            {
                drawNode.addDrawable( node.getDrawable( idx ) );
            }
        }

        Visitor::visit( node );
    }


    if( modelDirty )
    {
        JAG3D_PROFILE( "CV transform" );
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



CollectionVisitor::CollectionInfo::CollectionInfo()
    : Node::CallbackInfo()
{
}
CollectionVisitor::CollectionInfo::CollectionInfo( const CollectionInfo& rhs )
    : Node::CallbackInfo( rhs )
{
}
CollectionVisitor::CollectionInfo::~CollectionInfo()
{
}

void CollectionVisitor::CollectionInfo::reset()
{
}


// jagSG
}
