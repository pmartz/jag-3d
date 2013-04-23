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

#include <gmtl/gmtl.h>


namespace jagSG {


CollectionVisitor::CollectionVisitor()
  : Visitor( "collect" ),
    _currentNodes( NULL )
{
    reset();
}
CollectionVisitor::CollectionVisitor( const CollectionVisitor& rhs )
  : Visitor( rhs ),
    _currentID( rhs._currentID ),
    _currentNodes( NULL ),
    _transform( rhs._transform )
{
    setDrawGraphTemplate( rhs._drawGraphTemplate );

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
void CollectionVisitor::setViewport( const GLint x, const GLint y, const GLsizei width, const GLsizei height )
{
    _transform.setViewport( x, y, width, height );
}


void CollectionVisitor::reset()
{
    JAG3D_TRACE( "reset()" );

    resetCommandMap();
    resetMatrix();

    _currentID = 0;
    if( _drawGraph != NULL )
    {
        BOOST_FOREACH( jagDraw::DrawGraph::value_type& data, *_drawGraph )
        {
            data.clear();
        }
        if( _currentID < _drawGraph->size() )
            _currentNodes = &( (*_drawGraph)[ _currentID ] );
    }

    _infoPtr = CollectionInfoPtr( new CollectionInfo( _transform ) );
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

    _infoPtr->setBound( node.getBound() );
    Node::CallbackInfo* info( _infoPtr.get() );

    bool collect( true );
    jagSG::Node::CollectionCallbacks& callbacks( node.getCollectionCallbacks() );
    BOOST_FOREACH( jagSG::Node::CallbackPtr cb, callbacks )
    {
        if( !( (*cb)( *this, info ) ) )
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

            if( _currentNodes == NULL )
                setCurrentNodeContainer( 0 );

            _currentNodes->resize( _currentNodes->size()+1 );
            jagDraw::Node& drawNode( (*_currentNodes)[ _currentNodes->size()-1 ] );

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


jagDraw::DrawGraphPtr CollectionVisitor::getDrawGraph() const
{
    return( _drawGraph );
}

void CollectionVisitor::setDrawGraphTemplate( jagDraw::DrawGraphPtr drawGraphTemplate )
{
    _drawGraphTemplate = drawGraphTemplate;
    _drawGraph = jagDraw::DrawGraphPtr( new jagDraw::DrawGraph( *_drawGraphTemplate ) );
}
const jagDraw::DrawGraphPtr CollectionVisitor::getDrawGraphTemplate() const
{
    return( _drawGraphTemplate );
}

void CollectionVisitor::setCurrentNodeContainer( const unsigned int currentID )
{
    _currentID = currentID;

    if( ( _drawGraph == NULL ) ||
        ( _currentID >= _drawGraph->size() ) )
    {
        if( _drawGraph == NULL )
            // No draw graph template specified.
            _drawGraph = jagDraw::DrawGraphPtr( new jagDraw::DrawGraph() );

        // Either off the end of the draw graph,
        // or no draw graph template specified.
        _drawGraph->resize( _currentID + 1 );
    }
    _currentNodes = &( (*_drawGraph)[ _currentID ] );
}
unsigned int CollectionVisitor::getCurrentNodeContainer() const
{
    return( _currentID );
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



CollectionVisitor::CollectionInfo::CollectionInfo( jagBase::TransformD& transform )
    : Node::CallbackInfo(),
      _transform( transform )
    // Initialization of these variables is handled in setBound():
    //   _bound
    //   _ecDistance
    //   _ecDistanceDirty
    //   _ecRadius
    //   _ecRadiusDirty
    //   _wcLengthCoeff
    //   _wcLengthCoeffDirty
{
}
CollectionVisitor::CollectionInfo::CollectionInfo( const CollectionInfo& rhs )
    : Node::CallbackInfo( rhs ),
      _transform( rhs._transform )
    // Initialization of these variables is handled in setBound():
    //   _bound
    //   _ecDistance
    //   _ecDistanceDirty
    //   _ecRadius
    //   _ecRadiusDirty
    //   _wcLengthCoeff
    //   _wcLengthCoeffDirty
{
}
CollectionVisitor::CollectionInfo::~CollectionInfo()
{
}

void CollectionVisitor::CollectionInfo::setBound( jagDraw::BoundPtr bound )
{
    _bound = bound;
    _ecDistanceDirty = true;
    _ecRadiusDirty = true;
    _wcLengthCoeffDirty = true;
}

double CollectionVisitor::CollectionInfo::getECBoundDistance() const
{
    if( _ecDistanceDirty )
    {
        const gmtl::Point3d ecCenter( _transform.getModelView() * _bound->getCenter() );
        _ecDistance = -ecCenter[ 2 ];
        _ecDistanceDirty = false;
    }
    return( _ecDistance );
}
double CollectionVisitor::CollectionInfo::getECBoundRadius() const
{
    if( _ecRadiusDirty )
    {
        const gmtl::Vec3d radPrime( _transform.getModelView() *
            gmtl::Vec3d( _bound->getRadius(), 0., 0. ) );
        _ecRadius = gmtl::length< double >( radPrime );
        _ecRadiusDirty = false;
    }
    return( _ecRadius );
}
double CollectionVisitor::CollectionInfo::getWinCLength( double ecSegmentLength ) const
{
    if( _wcLengthCoeffDirty || _ecDistanceDirty )
    {
        // Formula to compute pixel distance from ec length:
        // ( d * 0.5 * w * p0 ) / ( z * p11 + p15 )
        //   d: ec length (parameter)
        //   z: ec z value
        //   w: viewport width
        //   p: projection matrix (16 elements, 0-based)

        int x, y, w, h;
        _transform.getViewport( x, y, w, h );
        const double* p( _transform.getProj().getData() );
        const double ecZ( -getECBoundDistance() );

        _wcLengthCoeff = ( 0.5 * w * p[0] ) / ( ecZ * p[11] + p[15] );
        _wcLengthCoeffDirty = false;
    }
    return( _wcLengthCoeff * ecSegmentLength );
}


// jagSG
}
