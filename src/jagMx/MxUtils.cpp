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

#include <jagMx/MxUtils.h>
#include <jag/base/LogMacros.h>
#include <gmtl/gmtl.h>

#include <cmath>


namespace jagMx
{


double computeInitialFOVYFromDistance( const gmtl::Sphered& bs, const double distance )
{
    const double halfFOVYSine = bs.getRadius() / distance;
    const double halfFOVYRadians = asin( halfFOVYSine );
    return( 2. * halfFOVYRadians );
}
double computeInitialDistanceFromFOV( const gmtl::Sphered& bs, const double fov )
{
    return( bs.getRadius() / sin( gmtl::Math::deg2Rad( fov * .5 ) ) );
}


gmtl::Vec2d computeOptimalNearFar( const gmtl::Point3d& position,
    const gmtl::Sphered& bs, bool ortho )
{
    double zNear, zFar;

    gmtl::Vec3d eyeToCenter = bs.getCenter() - position;
    const double len( gmtl::length( eyeToCenter ) );
    if( ortho )
    {
        zNear = len - bs.getRadius();
        zFar = len + bs.getRadius();
    }
    else
    {
        zNear = len - bs.getRadius();
        zFar = zNear + ( bs.getRadius() * 2. );
        if( zNear < 0. )
            zNear = zFar / 2000.; // Default z ratio.
    }

    return( gmtl::Vec2d( zNear, zFar ) );
}


#if 0
// TBD MxCoe Port
gmtl::Vec4d computePanPlane( osg::Node* scene, const jagMx::MxCore* mxCore,
                 const double ndcX, const double ndcY )
{
    const osg::BoundingSphere& bs = scene->getBound();

    osg::Matrixd proj = mxCore->computeProjection(
        jagMx::computeOptimalNearFar( mxCore->getPosition(), bs, mxCore->getOrtho() ) );

    // Assume ortho, where ndc far plane == 1 and w always == 1.
    osg::Vec4d farPoint( ndcX, ndcY, 1., 1. );
    if( !( mxCore->getOrtho() ) )
    {
        // Not ortho, so w != 1.0. Multiply by the far plane distance.
        // This yields a value in clip coords.
        double fovy, aspect, zNear, zFar;
        proj.getPerspective( fovy, aspect, zNear, zFar );
        farPoint *= zFar;
    }

    // Get inverse view & proj matrices to back-transform the clip coord far point.
    osg::Matrixd v = mxCore->getMatrix(); // This is the inverse of the view matrix.
    osg::Matrixd p = osg::Matrixd::inverse( proj );

    osg::Vec4d wc = farPoint * p * v;
    osg::Vec3d wcFarPoint( wc.x(), wc.y(), wc.z() );

    // Define world coord plane orthogonal to view, which contains the picked point.
    osg::Vec3d pickPoint;
    if( !( intersect( pickPoint, wcFarPoint, scene, mxCore ) ) )
    {
        // Intersection failed, probably user clicked on background.
        // Use a point 10. units in front of the eye, better than nothing.
        pickPoint = mxCore->getPosition() + ( mxCore->getDir() * 10. );
        osg::notify( osg::DEBUG_FP ) << "MxUtil::setPanStart: Intersection failed. ";
    }

    const osg::Vec3d viewDir( mxCore->getDir() );
    osg::Vec4d panPlane( viewDir, -( pickPoint * viewDir ) );
    osg::notify( osg::DEBUG_FP ) << "Pick point " << pickPoint << std::endl;
    osg::notify( osg::DEBUG_FP ) << "  Plane " << panPlane << std::endl;

    return( panPlane );
}
#endif

#if 0
// TBD MxCoe Port
bool intersect( osg::Vec3d& result, const osg::Vec3d& farPoint, osg::Node* scene, const jagMx::MxCore* mxCore )
{
    if( scene == NULL )
    {
        osg::notify( osg::WARN ) << "MxUtil::intersect: _scene == NULL." << std::endl;
        return( false );
    }

    const osg::BoundingSphere& bs = scene->getBound();
    const double viewDistance = ( bs.center() - mxCore->getPosition() ).length();
    const double distance = viewDistance + bs._radius;

    osg::Vec3d startPoint = mxCore->getOrtho() ? farPoint - ( mxCore->getDir() * distance * 2. ) : mxCore->getPosition();
    osgUtil::LineSegmentIntersector* intersector = new osgUtil::LineSegmentIntersector(
        startPoint, farPoint );
    osgUtil::IntersectionVisitor intersectVisitor( intersector, NULL );
    scene->accept( intersectVisitor );

    osgUtil::LineSegmentIntersector::Intersections& intersections = intersector->getIntersections();
    if( intersections.empty() )
        return( false );

    const osgUtil::LineSegmentIntersector::Intersection& intersection = *( intersections.begin() );
    result = intersection.getWorldIntersectPoint();
    return( true );
}
#endif


gmtl::Vec3d JAGMX_EXPORT pan( /* const osg::Node* scene, */ const jagMx::MxCore* mxCore,
    const gmtl::Planed& panPlane, const double deltaNdcX, const double deltaNdcY )
{
#if 0
    const osg::BoundingSphere& bs = scene->getBound();

    // Get the view volume far plane value, and the distance from
    // the near to far plane.
    osg::Vec2d nearFar( jagMx::computeOptimalNearFar( mxCore->getPosition(), bs, mxCore->getOrtho() ) );
    double zNear( nearFar[ 0 ] ), zFar( nearFar[ 1 ] );
    const double distance = zFar - zNear;
#else
    const double zNear( 1. ), zFar( 500. );
    const double distance = zFar - zNear;
#endif

    // Create two points, both in NDC space, and lying on the far plane at the back
    // of the view volume. One is the xy origin, the other is the passed xy parameters.
    gmtl::Vec4d farPoint0 = gmtl::Vec4d( 0., 0., 1., 1. );
    gmtl::Vec4d farPoint1 = gmtl::Vec4d( deltaNdcX, deltaNdcY, 1., 1. );
    if( !( mxCore->getOrtho() ) )
    {
        // Not ortho, so w != 1.0. Multiply by the far plane distance.
        // This yields values in clip coordinates.
        farPoint0 *= zFar;
        farPoint1 *= zFar;
    }

    // Get inverse view & proj matrices to back-transform the
    // two clip coord far points into world space.
    gmtl::Matrix44d v( mxCore->getMatrix() ); // This is the inverse of the view matrix.
    gmtl::Matrix44d proj( mxCore->computeProjection( zNear, zFar ) );
    gmtl::Matrix44d p;
    gmtl::invert( p, proj );

    gmtl::Vec4d wc0 = v * p * farPoint0;
    gmtl::Vec4d wc1 = v * p * farPoint1;

    const gmtl::Vec3d& viewDir( mxCore->getDir() );
    const gmtl::Point3d& pos( mxCore->getPosition() );

    // Intersect the two world coord points with the pan plane.
    gmtl::Point3d result0, result1;
    gmtl::Point3d p1( wc0[0], wc0[1], wc0[2] );
    gmtl::Point3d p0 = mxCore->getOrtho() ? p1 - ( viewDir * distance ) : pos;
    intersectRayPlane( result0, panPlane, p0, p1 );
    p1.set( wc1[0], wc1[1], wc1[2] );
    p0 = mxCore->getOrtho() ? p1 - ( viewDir * distance ) : pos;
    intersectRayPlane( result1, panPlane, p0, p1 );

    // Subtract the two plane intersection points to get the delta world coord
    // motion and move the view center accordingly.
    gmtl::Vec3d delta = result1 - result0;
    
    return( delta );
}


#if 0
// TBD MxCoe Port
osg::Vec3d pickPoint( osg::Node* scene, const jagMx::MxCore* mxCore,
    const double ndcX, const double ndcY )
{
    const osg::BoundingSphere& bs = scene->getBound();
    osg::Vec2d nearFar( jagMx::computeOptimalNearFar( mxCore->getPosition(), bs, mxCore->getOrtho() ) );

    osg::Vec4d ccFarPoint( ndcX, ndcY, 1., 1. );
    if( !( mxCore->getOrtho() ) )
    {
        // Not ortho, so w != 1.0. Multiply by the far plane distance.
        // This yields a value in clip coords.
        ccFarPoint *= nearFar[ 1 ];
    }

    // Get inverse view & proj matrices to back-transform the clip coord point.
    const osg::Matrixd v = mxCore->getMatrix();
    const osg::Matrixd proj = mxCore->computeProjection( nearFar );
    osg::Matrixd p = osg::Matrixd::inverse( proj );

    osg::Vec4d wc = ccFarPoint * p * v;
    osg::Vec3d farPoint( wc.x(), wc.y(), wc.z() );

    osg::Vec3d pickResult;
    if( !( intersect( pickResult, farPoint, scene, mxCore ) ) )
        osg::notify( osg::WARN ) << "MxUtils::pickPoint: No intersections." << std::endl;

    return( pickResult );
}
#endif


bool intersectRayPlane( gmtl::Point3d& result, const gmtl::Planed& plane, const gmtl::Point3d& p0, const gmtl::Point3d& p1 )
{
    const gmtl::Rayd ray( p0, p1 - p0 );
    double distance;
    const bool intersected( gmtl::intersect( plane, ray, distance ) );
    if( !intersected )
        return( false );
    result = ray.getOrigin() + ray.getDir() * distance;
    return( true );
}



void computeTrackball( double& angle, gmtl::Vec3d& axis,
    const gmtl::Point2d& start, const gmtl::Vec2d& delta,
    const gmtl::Matrix44d& orientMat, const double sensitivity )
{
    // Take the spin direction 'delta' and rotate it 90 degrees
    // to get our base axis (still in the window plane).
    // Simultaneously convert to current view space.
    gmtl::Vec2d screenAxis( -delta[ 1 ], delta[ 0 ] );
    const gmtl::Vec3d baseAxis( orientMat * gmtl::Vec3d( screenAxis[ 0 ], screenAxis[ 1 ], 0. ) );
    gmtl::Vec3d dir3( orientMat * gmtl::Vec3d( delta[ 0 ], delta[ 1 ], 0. ) );
    gmtl::normalize( dir3 );

    // The distance from center, along with the roll sensitivity,
    // tells us how much to rotate the baseAxis (ballTouchAngle) to get
    // the actual ballAxis.
    const double distance = gmtl::length( gmtl::Vec2d( start ) );
    const double rotationDir( ( gmtl::dot( screenAxis, start ) > 0. ) ? -1. : 1. );
    const double ballTouchAngle = rotationDir * sensitivity * distance;
    const gmtl::AxisAngled aa( ballTouchAngle, dir3 );
    gmtl::Matrix44d r; gmtl::set( r, aa );
    gmtl::Vec3d ballAxis( r * baseAxis );
    gmtl::normalize( ballAxis );

    angle = -( gmtl::length( delta ) );
    axis = ballAxis;
}



FunctionalMap::FunctionalMap()
{
    reset();
}
FunctionalMap::FunctionalMap( const FunctionalMap& rhs )
  : _map( rhs._map ),
    _state( rhs._state )
{
}
FunctionalMap::~FunctionalMap()
{
}

void FunctionalMap::configure( const unsigned int key, FunctionType func )
{
    _map[ key ] = func;
}
FunctionalMap::FunctionType FunctionalMap::getConfiguration( const unsigned int key ) const
{
    FunctionMapType::const_iterator it = _map.find( key );
    if( it != _map.end() )
        return( it->second );
    else
        return( NoOp );
}

std::string FunctionalMap::asString( FunctionType func )
{
    switch( func )
    {
    case LevelView: return( "LevelView" ); break;
    case JumpToWorldOrigin: return( "JumpToWorldOrigin" ); break;
    case JumpToHomePosition: return( "JumpToHomePosition" ); break;
    case MoveModifyScaleSpeedDown: return( "MoveModifyScaleSpeedDown" ); break;
    case MoveModifyScaleSpeedUp: return( "MoveModifyScaleSpeedUp" ); break;
    case MoveModeLiteral: return( "MoveModeLiteral" ); break;
    case MoveModeLocal: return( "MoveModeLocal" ); break;
    case MoveModeConstrained: return( "MoveModeConstrained" ); break;
    case MoveModeOriented: return( "MoveModeOriented" ); break;
    case MoveModeWorld: return( "MoveModeWorld" ); break;
    case MoveModeOrbit: return( "MoveModeOrbit" ); break;
    case CycleMoveMode: return( "CycleMoveMode" ); break;
    case MoveModifyUpDown: return( "MoveModifyUpDown" ); break;
    case MoveUpAtRate: return( "MoveUpAtRate" ); break;
    case MoveDownAtRate: return( "MoveDownAtRate" ); break;
    case RotateModeLocal: return( "RotateModeLocal" ); break;
    case RotateModeOrbit: return( "RotateModeOrbit" ); break;
    case RotateModeArcball: return( "RotateModeArcball" ); break;
    case CycleRotateMode: return( "CycleRotateMode" ); break;
    case RotateModifyRoll: return( "RotateModifyRoll" ); break;
    default:
    case NoOp: return( "NoOp" ); break;
    }
}
FunctionalMap::FunctionType FunctionalMap::asFunctionType( const std::string& str )
{
    if( str == std::string( "LevelView" ) ) return( LevelView );
    else if( str == std::string( "JumpToWorldOrigin" ) ) return( JumpToWorldOrigin );
    else if( str == std::string( "JumpToHomePosition" ) ) return( JumpToHomePosition );
    else if( str == std::string( "MoveModifyScaleSpeedDown" ) ) return( MoveModifyScaleSpeedDown );
    else if( str == std::string( "MoveModifyScaleSpeedUp" ) ) return( MoveModifyScaleSpeedUp );
    else if( str == std::string( "MoveModeLiteral" ) ) return( MoveModeLiteral );
    else if( str == std::string( "MoveModeLocal" ) ) return( MoveModeLocal );
    else if( str == std::string( "MoveModeConstrained" ) ) return( MoveModeConstrained );
    else if( str == std::string( "MoveModeOriented" ) ) return( MoveModeOriented );
    else if( str == std::string( "MoveModeWorld" ) ) return( MoveModeWorld );
    else if( str == std::string( "MoveModeOrbit" ) ) return( MoveModeOrbit );
    else if( str == std::string( "CycleMoveMode" ) ) return( CycleMoveMode );
    else if( str == std::string( "MoveModifyUpDown" ) ) return( MoveModifyUpDown );
    else if( str == std::string( "MoveUpAtRate" ) ) return( MoveUpAtRate );
    else if( str == std::string( "MoveDownAtRate" ) ) return( MoveDownAtRate );
    else if( str == std::string( "RotateModeLocal" ) ) return( RotateModeLocal );
    else if( str == std::string( "RotateModeOrbit" ) ) return( RotateModeOrbit );
    else if( str == std::string( "RotateModeArcball" ) ) return( RotateModeArcball );
    else if( str == std::string( "CycleRotateMode" ) ) return( CycleRotateMode );
    else if( str == std::string( "RotateModifyRoll" ) ) return( RotateModifyRoll );
    else return( NoOp );
}

bool FunctionalMap::validMoveMode( const FunctionType func )
{
    return( ( func == MoveModeLiteral ) ||
        ( func == MoveModeLocal ) ||
        ( func == MoveModeConstrained ) ||
        ( func == MoveModeOriented ) ||
        ( func == MoveModeWorld ) ||
        ( func == MoveModeOrbit ) );
}
bool FunctionalMap::validRotateMode( const FunctionType func )
{
    return( ( func == RotateModeLocal ) ||
        ( func == RotateModeOrbit ) ||
        ( func == RotateModeArcball ) );
}

FunctionalMap::FunctionType FunctionalMap::cycleMoveMode( const FunctionType func )
{
    FunctionType returnFunc = MoveModeLocal;
    if( func == MoveModeLiteral )
        returnFunc = MoveModeLocal;
    else if( func == MoveModeLocal )
        returnFunc = MoveModeConstrained;
    else if( func == MoveModeConstrained )
        returnFunc = MoveModeOriented;
    else if( func == MoveModeOriented )
        returnFunc = MoveModeWorld;
    else if ( func == MoveModeWorld )
        returnFunc = MoveModeOrbit;
    else if ( func == MoveModeOrbit )
        returnFunc = MoveModeLiteral;
    else
    {
        JAG3D_WARNING_STATIC( "jag.mxcore", "Can't cycle move mode: \"" + asString( func ) + "\"" );
    }

    JAG3D_INFO_STATIC( "jag.mxcore", asString( func ) );

    return( returnFunc );
}
FunctionalMap::FunctionType FunctionalMap::cycleRotateMode( const FunctionType func )
{
    FunctionType returnFunc = RotateModeLocal;
    if( func == RotateModeLocal )
        returnFunc = RotateModeOrbit;
    else if( func == RotateModeOrbit )
        returnFunc = RotateModeArcball;
    else if ( func == RotateModeArcball )
        returnFunc = RotateModeLocal;
    else
    {
        JAG3D_WARNING_STATIC( "jag.mxcore", "Can't cycle move mode: \"" + asString( func ) + "\"" );
    }

    JAG3D_INFO_STATIC( "jag.mxcore", asString( returnFunc ) );

    return( returnFunc );
}


void FunctionalMap::set( const unsigned int key, bool enable )
{
    const FunctionType func = getConfiguration( key );
    if( func == NoOp )
        return;

    _state[ func ] = enable;
}
void FunctionalMap::setFromBitmask( const unsigned int mask )
{
    unsigned int key( 1 );
    int idx;
    for( idx=0; idx<32; idx++, key<<=1 )
        set( key, ( ( mask & key ) != 0 ) );
}

bool FunctionalMap::isSet( const unsigned int key ) const
{
    const FunctionType func = getConfiguration( key );
    if( func == NoOp )
        return( false );

    return isSet( func );
}
bool FunctionalMap::isSet( const FunctionType func ) const
{
    return( _state[ func ] );
}

void FunctionalMap::reset()
{
    _map.clear();

    const int maxEnum( (int)NoOp );
    _state.resize( maxEnum );
    for( int idx=0; idx<maxEnum; idx++ )
        _state[ idx ] = false;
}


// jagMx
}
