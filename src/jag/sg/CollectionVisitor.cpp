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

#include <jag/sg/CollectionVisitor.h>
#include <jag/sg/Node.h>
#include <jag/draw/DrawNode.h>
#include <jag/draw/CommandNode.h>
#include <jag/base/gmtlSupport.h>
#include <jag/draw/Error.h>
#include <jag/base/Profile.h>
#include <jag/base/LogMacros.h>

#include <gmtl/gmtl.h>

#include <cfloat>


namespace jag {
namespace sg {


CollectionVisitor::CollectionVisitor()
  : Visitor( "collect" ),
    _currentNodes( NULL ),
    _nearFarOps( Default ),
    _frustumPlanes( LeftRightBottomTopPlanes )
{
    reset();
}
CollectionVisitor::CollectionVisitor( jag::sg::Node& node )
  : Visitor( "collect" ),
    _currentNodes( NULL ),
    _nearFarOps( Default ),
    _frustumPlanes( LeftRightBottomTopPlanes )
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

    if( _commandGraphRoot == NULL )
        _commandGraphRoot.reset( new jag::draw::CommandNode() );
    _commandNode = _commandGraphRoot.get();

    resetCommandMap();
    resetMatrix();

    resetPlanes();

    // TBD This seems very bad that we're allocating a new one of
    // these in reset(), which means once per frame.
    if( _drawTransformCallback == NULL )
    {
        _drawTransformCallback = jag::draw::TransformCallbackPtr(
            new jag::draw::TransformCallback() );
    }
    else
    {
        // Use copy constructor to preserve any pre-existing settings
        // (like custom matrix uniform names).
        _drawTransformCallback.reset(
            new jag::draw::TransformCallback( *_drawTransformCallback ) );
    }

    _currentID = 0;
    if( _drawGraph != NULL )
    {
        BOOST_FOREACH( jag::draw::DrawGraph::value_type& data, *_drawGraph )
        {
            data.reset();
        }
        if( _currentID < _drawGraph->size() )
            _currentNodes = &( (*_drawGraph)[ _currentID ] );

        _drawGraph->setTransformCallback( _drawTransformCallback );
    }

    _infoPtr = CollectionInfoPtr( new CollectionInfo( _transform ) );

    _minECZ = DBL_MAX;
    _maxECZ = -DBL_MAX;
}


void CollectionVisitor::visit( jag::sg::Node& node )
{
    if( node.getCommandMap() != NULL )
        pushCommandGraph( node.getCommandMap() );

    pushPlanes();

    {
    if( JAG3D_LOG_TRACE )
    {
        JAG3D_TRACE( "visit() - \"" + node.getUserDataName() + "\"" );
    }
    JAG3D_PROFILE( "setup" );


    _infoPtr->setNode( &node );
    jag::draw::BoundOwner* boundOwner( &node );
    _infoPtr->setBound( boundOwner->getBound( _commandStack.back() ).get() );
    _infoPtr->setContainmentPlanes( getCurrentPlanes() );
    }
    Node::CallbackInfo* info( _infoPtr.get() );

    bool collect( true );
    {
    JAG3D_PROFILE( "callbacks" );
    jag::sg::Node::CollectionCallbacks& callbacks( node.getCollectionCallbacks() );
    BOOST_FOREACH( jag::sg::Node::CallbackPtr cb, callbacks )
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
    popPlanes();

    if( node.getCommandMap() != NULL )
        popCommandGraph();
}

void CollectionVisitor::collectAndTraverse( jag::sg::Node& node )
{
    MatrixStackHelper msh( *this, node.getTransform() );

    {
    JAG3D_PROFILE( "collect operations" );
    if( msh.getDirty() )
    {
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
        {
        JAG3D_PROFILE( "collect min/maxZ" );
        if( ( _nearFarOps & AutoCompute ) != 0 )
        {
            // Record changes to min near / max far.
            _minECZ = std::min< double >( _minECZ, _infoPtr->getECBoundDistance() - _infoPtr->getECBoundRadius() );
            _maxECZ = std::max< double >( _maxECZ, maxDrawableECZ );
        }
        }

        // Get a new jag::draw::DrawNode.
        // Note: We can't re-use an existing jag::draw::DrawNode, as that
        // potentially breaks DrawGraph sorting by depth.
        if( _currentNodes == NULL )
            setCurrentNodeContainer( 0 );
        jag::draw::DrawNodePtr& drawNode( _currentNodes->grow() );

        // Init the jag::draw::DrawNode.
        {
            {
            JAG3D_PROFILE( "collect node setup" );
            // Set eye coord distance (for distance-based render order control ).
            drawNode->setDistance( _infoPtr->getECBoundDistance() );

            // Model matrix changed; add callback to update the transform uniforms.
            drawNode->getExecuteCallbacks().push_back( _drawTransformCallback );
            // Set the model matrix.
            drawNode->setTransform( _matrixStack.back() );

            // Assign accumulated CommandMap from command graph.
            {
            JAG3D_PROFILE( "CM copy" );
            drawNode->setCommandMap( _commandNode->getAccumulation() );
            }
            }
        }

        for( unsigned int idx=0; idx < numDrawables; ++idx )
        {
            drawNode->addDrawable( node.getDrawable( idx ) );
        }
    }
    }

    Visitor::visit( node );

    if( msh.getDirty() )
    {
        _transform.setModel( _matrixStack[ _matrixStack.size()-2 ] );
    }
}


jag::draw::DrawGraphPtr CollectionVisitor::getDrawGraph() const
{
    return( _drawGraph );
}

void CollectionVisitor::setDrawGraphTemplate( jag::draw::DrawGraphPtr drawGraphTemplate )
{
    _drawGraphTemplate = drawGraphTemplate;
    _drawGraph = jag::draw::DrawGraphPtr( new jag::draw::DrawGraph( *_drawGraphTemplate ) );

    if( _drawTransformCallback != NULL )
        _drawGraph->setTransformCallback( _drawTransformCallback );
}
const jag::draw::DrawGraphPtr CollectionVisitor::getDrawGraphTemplate() const
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
void CollectionVisitor::recomputeNearFar( double& minECNear, double& maxECFar, const gmtl::Matrix44d& view, const double ratio )
{
    // The transform we're interested in is Vd = V * Vn(-1), where:
    //   V      = Original view matrix used during collection
    //   Vn(-1) = New view matrix the app is using for next draw traversal.
    //   Vd     = Inverse of the difference between the two matrices.
    // Multiplying the eye coord origin (0,0,0) by Vd produces the
    // change in view position from the original view matrix to the new
    // view matrix. We add the z component of that change to the old
    // computed near/far values to yield an approximation of the new
    // near/far values.

    gmtl::Matrix44d viewInv;
    gmtl::invert( viewInv, view );
    const gmtl::Point4d deltaEC = _transform.getView() * viewInv * gmtl::Point4d( 0., 0., 0., 1. );
    const double newMinZ = _minECZ + deltaEC[ 2 ];
    const double newMaxZ = _maxECZ + deltaEC[ 2 ];

    // Scale and clamp as usual.
    const double scaledNear( newMaxZ * ratio );
    if( newMinZ < scaledNear )
        minECNear = scaledNear;
    else
        minECNear = newMinZ;
    maxECFar = newMaxZ;
}


void CollectionVisitor::setFrustumPlanes( const FrustumPlanes frustumPlanes )
{
    _frustumPlanes = frustumPlanes;
    resetPlanes();
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
            _drawGraph = jag::draw::DrawGraphPtr( new jag::draw::DrawGraph() );

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

void CollectionVisitor::pushCommandGraph( jag::draw::CommandMapPtr& commands )
{
    JAG3D_PROFILE( "Command graph processing" );
    _commandNode = _commandNode->findOrCreateChild( commands );
}
void CollectionVisitor::popCommandGraph()
{
    JAG3D_PROFILE( "Command graph processing" );

    if( _commandNode->getParent() != NULL )
    {
        _commandNode = _commandNode->getParent();
        return;
    }

    JAG3D_ERROR( "popCommandGraph: Popping with NULL CommandNode parent." );
    _commandNode = NULL;
}





CollectionVisitor::CollectionInfo::CollectionInfo( jag::base::TransformD& transform )
    : Node::CallbackInfo(),
      _transform( transform ),
      _bound( NULL )
    // Initialization of these variables is handled in setBound():
    //   _ecDistance
    //   _ecDistanceDirty
    //   _ecRadius
    //   _ecRadiusDirty
    //   _wcLengthCoeff
    //   _wcLengthCoeffDirty
    // Initialization of these variables is handled in setContainmentPlanes():
    //   _planes
    //   _indices
{
}
CollectionVisitor::CollectionInfo::CollectionInfo( const CollectionInfo& rhs )
    : Node::CallbackInfo( rhs ),
      _transform( rhs._transform ),
      _bound( rhs._bound )
    // Initialization of these variables is handled in setBound():
    //   _ecDistance
    //   _ecDistanceDirty
    //   _ecRadius
    //   _ecRadiusDirty
    //   _wcLengthCoeff
    //   _wcLengthCoeffDirty
    // Initialization of these variables is handled in setContainmentPlanes():
    //   _planes
    //   _indices
{
}
CollectionVisitor::CollectionInfo::~CollectionInfo()
{
}


void CollectionVisitor::CollectionInfo::setBound( jag::draw::Bound* bound )
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
        const gmtl::Matrix44d& mv( _transform.getModelView() );
        const gmtl::Point3d& c( _bound->getCenter() );
        _ecDistance = -( mv( 2, 0 ) * c[ 0 ] +
            mv( 2, 1 ) * c[ 1 ] +
            mv( 2, 2 ) * c[ 2 ] +
            mv( 2, 3 ) );
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
        double ecZ( -getECBoundDistance() );

        if( ecZ >= 0. )
            _wcLengthCoeff = FLT_MAX;
        else
            _wcLengthCoeff = ( 0.5 * w * p[0] ) / ( ecZ * p[11] + p[15] );
        _wcLengthCoeffDirty = false;
    }
    return( _wcLengthCoeff * ecSegmentLength );
}

void CollectionVisitor::CollectionInfo::setContainmentPlanes( CollectionVisitor::IndexList* indices )
{
    _indices = indices;

    const gmtl::Frustumd& f( _transform.getFrustum() );
    _planes.clear();
    for( unsigned int idx=0; idx<6; ++idx )
        _planes.push_back( f.mPlanes[ idx ] );
}
bool CollectionVisitor::CollectionInfo::isContained() const
{
    const jag::base::TransformD::FTYPE frustum( _transform.getFrustum() );
    if( _bound->getType() == jag::draw::Bound::Box_t )
        return( gmtl::contains( _planes, *_indices, _bound->asAABox() ) );
    else
        return( gmtl::contains( _planes, *_indices, _bound->asSphere() ) );
}


// namespace jag::sg::
}
}
