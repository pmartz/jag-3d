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

#include <cfloat>


namespace jagSG {


CollectionVisitor::CollectionVisitor()
  : Visitor( "collect" ),
    _currentNodes( NULL ),
    _nearFarOps( Default ),
    _frustumPlanes( LeftRightTopBottomPlanes )
{
    reset();
}
CollectionVisitor::CollectionVisitor( jagSG::Node& node )
  : Visitor( "collect" ),
    _currentNodes( NULL ),
    _nearFarOps( Default ),
    _frustumPlanes( LeftRightTopBottomPlanes )
{
    reset();
    node.accept( *this );
}
CollectionVisitor::CollectionVisitor( const CollectionVisitor& rhs )
  : Visitor( rhs ),
    _currentID( rhs._currentID ),
    _currentNodes( NULL ),
    _transform( rhs._transform ),
    _nearFarOps( rhs._nearFarOps ),
    _frustumPlanes( rhs._frustumPlanes )
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

    _drawTransformCallback = jagDraw::TransformCallbackPtr(
        new jagDraw::TransformCallback() );

    _currentID = 0;
    if( _drawGraph != NULL )
    {
        BOOST_FOREACH( jagDraw::DrawGraph::value_type& data, *_drawGraph )
        {
            data.clear();
        }
        if( _currentID < _drawGraph->size() )
            _currentNodes = &( (*_drawGraph)[ _currentID ] );

        _drawGraph->setTransformCallback( _drawTransformCallback );
    }

    _infoPtr = CollectionInfoPtr( new CollectionInfo( _transform ) );

    _minECZ = DBL_MAX;
    _maxECZ = -DBL_MAX;
}


void CollectionVisitor::visit( jagSG::Node& node )
{
    {
    JAG3D_PROFILE( "setup" );
    JAG3D_TRACE( "visit()" );


    {
    JAG3D_PROFILE( "commandmap" );
    pushCommandMap( node.getCommandMap() );
    }

    _infoPtr->setNode( &node );
    jagDraw::BoundOwner* boundOwner( &node );
    _infoPtr->setBound( boundOwner->getBound( _commandStack.back() ).get() );
    }
    Node::CallbackInfo* info( _infoPtr.get() );

    bool collect( true );
    {
    JAG3D_PROFILE( "callbacks" );
    jagSG::Node::CollectionCallbacks& callbacks( node.getCollectionCallbacks() );
    BOOST_FOREACH( jagSG::Node::CallbackPtr cb, callbacks )
    {
        if( !( (*cb)( this, info ) ) )
        {
            collect = false;
            break;
        }
    }
    }
    if( collect )
    {
        collectAndTraverse( node );
    }

    {
    JAG3D_PROFILE( "wrapup" );
    popCommandMap();
    }
}

void CollectionVisitor::collectAndTraverse( jagSG::Node& node )
{
    const bool modelDirty( node.getTransform() != gmtl::MAT_IDENTITY44D );
    {
    JAG3D_PROFILE( "collect operations" );
    if( modelDirty )
    {
        pushMatrix( node.getTransform() );
        _transform.setModel( _matrixStack.back() );
    }

    // Calculate maxECZ, used to determine 1) if all Drawables
    // are behind the viewer, and 2) possible modification to the
    // computed _maxECZ value.
    const double maxDrawableECZ( _infoPtr->getECBoundDistance() + _infoPtr->getECBoundRadius() );
    // We will collectDrawables if either 1) we don't care about them
    // being behind the viewer, or 2) at least some of the Drawables are
    // in front of the viewer.
    const bool collectDrawables( ( ( _nearFarOps & DiscardNegativeECZDrawables ) == 0 ) ||
        ( maxDrawableECZ > 0.0 ) );

    const unsigned int numDrawables( node.getNumDrawables() );
    if( collectDrawables && ( numDrawables > 0 ) )
    {
        if( _currentNodes == NULL )
            setCurrentNodeContainer( 0 );

        _currentNodes->resize( _currentNodes->size()+1 );
        jagDraw::Node& drawNode( (*_currentNodes)[ _currentNodes->size()-1 ] );

        // Set eye coord distance (for distance-based render order control ).
        drawNode.setDistance( _infoPtr->getECBoundDistance() );

        // Set the model matrix.
        if( _matrixStack.size() > 0 )
            drawNode.setTransform( _matrixStack.back() );

        // TBD transform. Need to do this on first draw node and then
        //   only on other draw nodes if they have a dirty model matrix.
        //   But for now, do it everywhere.
        // Model matrix is dirty; add callback to update the transform uniforms.
        drawNode.getExecuteCallbacks().push_back( _drawTransformCallback );

        if( ( _nearFarOps & AutoCompute ) != 0 )
        {
            // Record changes to min near / max far.
            _minECZ = std::min< double >( _minECZ, _infoPtr->getECBoundDistance() - _infoPtr->getECBoundRadius() );
            _maxECZ = std::max< double >( _maxECZ, maxDrawableECZ );
        }


        //JAG3D_WARNING( "TBD Must allocate new CommandMapPtr?" );
        jagDraw::CommandMapPtr commands( new jagDraw::CommandMap(
                _commandStack.back() ) );
        drawNode.setCommandMap( commands );

        for( unsigned int idx=0; idx < numDrawables; ++idx )
        {
            drawNode.addDrawable( node.getDrawable( idx ) );
        }
    }
    }

    Visitor::visit( node );

    if( modelDirty )
    {
        popMatrix();
        _transform.setModel( _matrixStack.empty() ?
            gmtl::MAT_IDENTITY44D : _matrixStack.back() );
    }
}


jagDraw::DrawGraphPtr CollectionVisitor::getDrawGraph() const
{
    return( _drawGraph );
}

void CollectionVisitor::setDrawGraphTemplate( jagDraw::DrawGraphPtr drawGraphTemplate )
{
    _drawGraphTemplate = drawGraphTemplate;
    _drawGraph = jagDraw::DrawGraphPtr( new jagDraw::DrawGraph( *_drawGraphTemplate ) );

    if( _drawTransformCallback != NULL )
        _drawGraph->setTransformCallback( _drawTransformCallback );
}
const jagDraw::DrawGraphPtr CollectionVisitor::getDrawGraphTemplate() const
{
    return( _drawGraphTemplate );
}


void CollectionVisitor::setNearFarOps( const NearFarOps nearFarOps )
{
    _nearFarOps = nearFarOps;
}
CollectionVisitor::NearFarOps CollectionVisitor::getNearFarOps() const
{
    return( _nearFarOps );
}

void CollectionVisitor::getNearFar( double& minECNear, double& maxECFar, const double ratio )
{
    if( _minECZ > _maxECZ )
    {
        // This typically happens when everything is culled and no bounds
        // were used to modify the min and max eye coord z values. In
        // this case, the returned values are irrelevant.
        minECNear = 1.;
        maxECFar = 100.;
        return;
    }
         
    const double scaledNear( _maxECZ * ratio );
    if( _minECZ < scaledNear )
        minECNear = scaledNear;
    else
        minECNear = _minECZ;
    maxECFar = _maxECZ;
}

void CollectionVisitor::setFrustumPlanes( const FrustumPlanes frustumPlanes )
{
    _frustumPlanes = frustumPlanes;
}
CollectionVisitor::FrustumPlanes CollectionVisitor::getFrustumPlanes() const
{
    return( _frustumPlanes );
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




CollectionVisitor::CollectionInfo::CollectionInfo( jagBase::TransformD& transform )
    : Node::CallbackInfo(),
      _transform( transform ),
      _frustumPlanes( LeftRightTopBottomPlanes ),
      _bound( NULL )
    // Initialization of these variables is handled in setBound():
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
      _transform( rhs._transform ),
      _frustumPlanes( rhs._frustumPlanes ),
      _bound( rhs._bound )
    // Initialization of these variables is handled in setBound():
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

void CollectionVisitor::CollectionInfo::setBound( jagDraw::Bound* bound )
{
    _ecDistanceDirty = true;
    _ecRadiusDirty = true;
    _wcLengthCoeffDirty = true;

    _bound = bound;
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

bool CollectionVisitor::CollectionInfo::inFrustum() const
{
    const jagBase::TransformD::FTYPE frustum( _transform.getFrustum() );
    jagDraw::BoundPtr bound( _bound->clone() );
    bound->transform( _transform.getModel() );
    if( bound->getType() == jagDraw::Bound::Box_t )
        return( gmtl::isInVolume( frustum, bound->asAABox() ) );
    else
        return( gmtl::isInVolume( frustum, bound->asSphere() ) );
}



// jagSG
}
