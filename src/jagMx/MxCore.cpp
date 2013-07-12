/*************** <auto-copyright.pl BEGIN do not edit this line> **************
*
* jag3d is (C) Copyright 2011-2013 by Kenneth Mark Bryden and Paul Martz
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

#include <jagMx/MxCore.h>
#include <jagMx/MxUtils.h>

#include <jagBase/LogMacros.h>
#include <gmtl/gmtl.h>

#include <cmath>
#include <iostream>


using gmtl::dot;
using gmtl::normalize;


namespace jagMx
{


MxCore::MxCore()
  : _viewUp( 0., 0., 1. ),
    _viewDir( 0., 1., 0. ),
    _position( 0., 0., 0. ),
    _initialUp( 0., 0., 1. ),
    _initialDir( 0., 1., 0. ),
    _initialPosition( 0., 0., 0. ),
    _orientedUp( 0., 0., 1. ),
    _orientedDir( 0., 1., 0. ),
    _orbitCenter( 0., 0., 0. ),
    _rotateScale( 1. ),
    _moveScale( 1., 1., 1. ),
    _ortho( false ),
    _aspect( 1.0 ),
    _fovy( 30.0 ),
    _initialFovy( 30.0 ),
    _fovyScale( 1.1 ),
    _clampFovyScale( true ),
    _clampFovyRange( 5.0, 160.0 ),
    _orthoBottom( 0.0 ),
    _orthoTop( 0.0 )
{
}
MxCore::MxCore( const MxCore& rhs )
  : _viewUp( rhs._viewUp ),
    _viewDir( rhs._viewDir ),
    _position( rhs._position ),
    _initialUp( rhs._initialUp ),
    _initialDir( rhs._initialDir ),
    _initialPosition( rhs._initialPosition ),
    _orientedUp( rhs._orientedUp ),
    _orientedDir( rhs._orientedDir ),
    _orbitCenter( rhs._orbitCenter ),
    _rotateScale( rhs._rotateScale ),
    _moveScale( rhs._moveScale ),
    _ortho( rhs._ortho ),
    _aspect( rhs._aspect ),
    _fovy( rhs._fovy ),
    _initialFovy( rhs._initialFovy ),
    _fovyScale( rhs._fovyScale ),
    _clampFovyScale( rhs._clampFovyScale ),
    _clampFovyRange( rhs._clampFovyRange ),
    _orthoBottom( rhs._orthoBottom ),
    _orthoTop( rhs._orthoTop )
{
}
MxCore::~MxCore()
{
}



//
// View matrix support
//

gmtl::Matrix44d MxCore::getMatrix() const
{
    const gmtl::Vec3d c = getCross();
    const gmtl::Vec3d& u = _viewUp;
    const gmtl::Vec3d& d = _viewDir;
    const gmtl::Vec3d p = _position;

    gmtl::Matrix44d m;
    m.set(
        c[0], u[0], -d[0], p[0],
        c[1], u[1], -d[1], p[1],
        c[2], u[2], -d[2], p[2],
        0.0, 0.0, 0.0, 1.0 );
    return( m );
}
gmtl::Matrix44d MxCore::getOrientationMatrix() const
{
    const gmtl::Vec3d c = getCross();
    const gmtl::Vec3d& u = _viewUp;
    const gmtl::Vec3d& negDir = -_viewDir;

    gmtl::Matrix44d m;
    gmtl::setAxes( m, c, u, negDir );
    return( m );
}
gmtl::Matrix44d MxCore::getInverseMatrix() const
{
    const gmtl::Vec3d c = getCross();
    const gmtl::Vec3d& u = _viewUp;
    const gmtl::Vec3d& d = _viewDir;

    gmtl::Matrix44d m;
    m.set(
        c[0], c[1], c[2], 0.0,
        u[0], u[1], u[2], 0.0,
        -d[0], -d[1], -d[2], 0.0,
        0.0, 0.0, 0.0, 1.0 );
    gmtl::Matrix44d t;
    gmtl::setTrans( t, -_position );
    return( m * t );
}

void MxCore::setInitialValues( const gmtl::Vec3d& up, const gmtl::Vec3d& dir,
    const gmtl::Point3d& pos, double fovy )
{
    _initialUp = up;
    _initialDir = dir;
    _initialPosition = pos;
    _initialFovy = fovy;

    // Error check.
    normalize( _initialUp );
    normalize( _initialDir );
    if( abs( dot( _initialUp, _initialDir ) ) > 0.99 )
    {
        JAG3D_WARNING_STATIC( "jag.mxcore", "MxCore::setInitialValues: Up and dir vectors are nearly coincident. Results are undefined." );
    }

    // orthonormalize
    const gmtl::Vec3d c = _initialDir ^ _initialUp;
    _initialUp = c ^ _initialDir;
    normalize( _initialUp );
    normalize( _initialDir );
}
void MxCore::getInitialValues( gmtl::Vec3d& up, gmtl::Vec3d& dir,
    gmtl::Point3d& pos, double& fovy )
{
    up = _initialUp;
    dir = _initialDir;
    pos = _initialPosition;
    fovy = _initialFovy;
}


void MxCore::reset()
{
    _viewUp = _initialUp;
    _viewDir = _initialDir;
    orthonormalize();

    _position = _initialPosition;
    _fovy = _initialFovy;
    setOrtho( false );
}


void MxCore::setOriented( const gmtl::Vec3d& up, const gmtl::Vec3d& dir )
{
    _orientedUp = up;
    _orientedDir = dir;

    // Error check.
    normalize( _orientedUp );
    normalize( _orientedDir );
    if( abs( dot( _orientedUp, _orientedDir ) ) > 0.99 )
    {
        JAG3D_WARNING_STATIC( "jag.mxcore", "MxCore::setOriented: Up and dir vectors are nearly coincident. Results are undefined." );
    }

    // orthonormalize
    const gmtl::Vec3d c = _orientedDir ^ _orientedUp;
    _orientedUp = c ^ _orientedDir;
    normalize( _orientedUp );
    normalize( _orientedDir );
}
void MxCore::setOriented( const gmtl::Vec3d& dir )
{
    setOriented( _orientedUp, dir );
}
void MxCore::getOriented( gmtl::Vec3d& up, gmtl::Vec3d& dir )
{
    up = _orientedUp;
    dir = _orientedDir;
}


void MxCore::setByMatrix( const gmtl::Matrix44d& m )
{
    const double* data( m.mData );
    _viewUp.set( data[ 4 ], data[ 5 ], data[ 6 ] );
    _viewDir.set( -data[ 8 ], -data[ 9 ], -data[ 10 ] );
    _position.set( data[ 12 ], data[ 13 ], data[ 14 ] );
    orthonormalize();
}
void MxCore::setByInverseMatrix( const gmtl::Matrix44d& m )
{
    gmtl::Matrix44d invM;
    gmtl::invert( invM, m );
    setByMatrix( invM );
}
void MxCore::setOrientationByMatrix( const gmtl::Matrix44d& m )
{
    const double* data( m.mData );
    _viewUp.set( data[ 4 ], data[ 5 ], data[ 6 ] );
    _viewDir.set( -data[ 8 ], -data[ 9 ], -data[ 10 ] );
    orthonormalize();
}
void MxCore::setOrientationByInverseMatrix( const gmtl::Matrix44d& m )
{
    gmtl::Matrix44d invM;
    gmtl::invert( invM, m );
    setOrientationByMatrix( invM );
}

void MxCore::level()
{
    _viewUp = _initialUp;

    // Check for vurrent view dir coincident with initial up vector. If so,
    // we can't preserve the current view dir and need to set it to the
    // initial view dir.
    if( abs( dot( _initialUp, _viewDir ) ) > 0.99 )
        _viewDir = _initialDir;
    else
        _viewDir = _viewUp ^ getCross();
    orthonormalize();
}

void MxCore::lookAtAndFit( const gmtl::Sphered& bs )
{
    // Look at the bounding sphere center.
    gmtl::Vec3d newDir = bs.getCenter() - _position;
    normalize( newDir );
    setDir( newDir );

    // Set the eve position distance so that the sphere fits into the minimum FOV.
    double minFov = ( _aspect < 1. ) ? ( _aspect * _fovy ) : _fovy;
    const double distance = jagMx::computeInitialDistanceFromFOV( bs, minFov );
    setPosition( bs.getCenter() - ( newDir * distance ) );
}

void MxCore::lookAtAndFit( const gmtl::AABoxd& bb )
{
    JAG3D_NOTICE_STATIC( "jag.mxcore", "lookAtAndFit( const gmtl::AABoxd& ):  TBD MxCore port." );
#if 0
    // We'll get the view matrix to project the bounding box, so pre-configure it
    // to point at the box center. Eye position doesn't matter at this point (we
    // compute the eye position at the end of the function).
    const gmtl::Point3d center( ( bb.getMax() - bb.getMin() ) / 2. + bb.getMin() );
    gmtl::Vec3d newDir = center - _position;
    normalize( newDir );
    setDir( newDir );


    // Ttransform the bounding box vertices into eye space,
    // then determine their x and y extents. We'll compare the eye
    // space bb aspect ratio against the projection _aspect to
    // determine the critical axis to fit.

    osg::ref_ptr< osg::Vec3Array > corners = new osg::Vec3Array;
    corners->resize( 8 );
    ( *corners )[ 0 ].set( bb._min );
    ( *corners )[ 1 ].set( bb._max.x(), bb._min.y(), bb._min.z() );
    ( *corners )[ 2 ].set( bb._max.x(), bb._min.y(), bb._max.z() );
    ( *corners )[ 3 ].set( bb._min.x(), bb._min.y(), bb._max.z() );
    ( *corners )[ 4 ].set( bb._max );
    ( *corners )[ 5 ].set( bb._min.x(), bb._max.y(), bb._max.z() );
    ( *corners )[ 6 ].set( bb._min.x(), bb._max.y(), bb._min.z() );
    ( *corners )[ 7 ].set( bb._max.x(), bb._max.y(), bb._min.z() );

    osgwTools::transform( getInverseMatrix(), corners.get() );
    // The 'corners' array of bb verts are now in eye space.

    // Determine max and min values for eye space x and y
    gmtl::Vec2f minEC( FLT_MAX, FLT_MAX ), maxEC( FLT_MIN, FLT_MIN );
    unsigned int idx;
    for( idx=0; idx<8; idx++ )
    {
        const osg::Vec3& v( ( *corners )[ idx ] );
        minEC[ 0 ] = min( v.x(), minEC[ 0 ] );
        minEC[ 1 ] = min( v.y(), minEC[ 1 ] );
        maxEC[ 0 ] = max( v.x(), maxEC[ 0 ] );
        maxEC[ 1 ] = max( v.y(), maxEC[ 1 ] );
    }
    // aspect is width (x) over height (y).
    const double ecWidth( maxEC[ 0 ] - minEC[ 0 ] );
    const double ecHeight( maxEC[ 1 ] - minEC[ 1 ] );
    const double ecAspect = ecWidth / ecHeight;


    // We'll store half the extent of interest into a dummy bounding sphere's radius.
    // We'll store the analogous fov in bestFov.
    gmtl::Sphered bs;
    double bestFov;
    if( ecAspect > _aspect )
    {
        // Fit eye space x to the view
        bs.radius() = ecWidth * .5;
        bestFov = _aspect * _fovy;
    }
    else
    {
        // Fit eye space y to the view
        bs.radius() = ecHeight * .5;
        bestFov = _fovy;
    }

    // The wrap-up code sets the eye position at the best distance from
    // the bb center. Extra distance is added in to account for the fact
    // that the input bound probably has a larger radius than the eye coord
    // bound that we're passing to computeInitialDistanceFromFOVY().
    const double extraDistance = bb.radius() - bs.radius();
    const double distance = extraDistance +
        jagMx::computeInitialDistanceFromFOV( bs, bestFov );
    setPosition( bs.getCenter() - ( newDir * distance ) );
#endif
}

void MxCore::lookAtOrbitCenter()
{
    gmtl::Vec3d newDir = _orbitCenter - _position;
    normalize( newDir );

    const gmtl::Matrix44d r( gmtl::makeRot< gmtl::Matrix44d >( _viewDir, newDir ) );
    _viewDir = r * _viewDir;
    _viewUp = r * _viewUp;
    orthonormalize();
}


void MxCore::rotateLocal( double angle, const gmtl::Vec3d& axis )
{
    gmtl::AxisAngled aa( angle * _rotateScale, axis );
    gmtl::Matrix44d r( gmtl::makeRot< gmtl::Matrix44d, gmtl::AxisAngled >( aa ) );
    gmtl::set( r, aa );

    _viewDir = r * _viewDir;
    _viewUp = r * _viewUp;
    orthonormalize();
}

void MxCore::rotateOrbit( double angle, const gmtl::Vec3d& axis )
{
    gmtl::AxisAngled aa( angle * _rotateScale, axis );
    gmtl::Matrix44d r( gmtl::makeRot< gmtl::Matrix44d, gmtl::AxisAngled >( aa ) );
    gmtl::set( r, aa );

    const gmtl::Vec3d vec( _position - _orbitCenter );
    _position = ( r * vec ) + _orbitCenter;
    _viewDir = r * _viewDir;
    _viewUp = r * _viewUp;
    orthonormalize();
}

void MxCore::moveLiteral( const gmtl::Vec3d& delta )
{
    _position += delta;
    _orbitCenter += delta;
}
void MxCore::moveLocal( const gmtl::Vec3d& delta )
{
    const gmtl::Vec3d scaledDelta( delta[0] * _moveScale[0],
        delta[1] * _moveScale[1], delta[2] * _moveScale[2] );
    const gmtl::Matrix44d orient( getOrientationMatrix() );
    _position += ( orient * scaledDelta );
    _orbitCenter += ( orient * scaledDelta );
}
void MxCore::moveConstrained( const gmtl::Vec3d& delta )
{
    const gmtl::Vec3d c = getCross();
    const gmtl::Vec3d& u = _initialUp;
    const gmtl::Vec3d back = c ^ u;
    gmtl::Matrix44d orient;
    gmtl::setAxes( orient, c, u, back );

    const gmtl::Vec3d scaledDelta( delta[0] * _moveScale[0],
        delta[1] * _moveScale[1], delta[2] * _moveScale[2] );
    _position += ( orient * scaledDelta );
    _orbitCenter += ( orient * scaledDelta );
}
void MxCore::moveOriented( const gmtl::Vec3d& delta, const bool orientedToWorld )
{
    const gmtl::Vec3d c = _orientedDir ^ _orientedUp;
    const gmtl::Vec3d& u = _orientedUp;
    const gmtl::Vec3d& negDir = -_orientedDir;
    gmtl::Matrix44d orient;
    gmtl::setAxes( orient, c, u, negDir );
    if( orientedToWorld )
    {
        const gmtl::Vec3d cl = getCross();
        const gmtl::Vec3d& ul = _viewUp;
        const gmtl::Vec3d& dl = _viewDir;
        gmtl::Matrix44d l2w;
        gmtl::setAxes( l2w, cl, dl, ul );
        orient = l2w * orient;
    }

    const gmtl::Vec3d scaledDelta( delta[0] * _moveScale[0],
        delta[1] * _moveScale[1], delta[2] * _moveScale[2] );
    _position += ( orient * scaledDelta );
    _orbitCenter += ( orient * scaledDelta );
}
void MxCore::moveWorld( const gmtl::Vec3d& delta )
{
    const gmtl::Vec3d& u = _initialUp;
    const gmtl::Vec3d& d = _initialDir;
    const gmtl::Vec3d& negDir = -d;
    const gmtl::Vec3d c = d ^ u;
    gmtl::Matrix44d orient;
    gmtl::setAxes( orient, c, u, negDir );

    const gmtl::Vec3d scaledDelta( delta[0] * _moveScale[0],
        delta[1] * _moveScale[1], delta[2] * _moveScale[2] );
    _position += ( orient * scaledDelta );
    _orbitCenter += ( orient * scaledDelta );
}
void MxCore::moveOrbit( const float distance )
{
#if 1
   // Original, optimized code
   gmtl::Vec3d moveAxis = _position - _orbitCenter;
   _position += ( distance * _moveScale[1] * moveAxis );
#else
   // For debugging
   const gmtl::Vec3d oldPos = _position;
   gmtl::Vec3d moveAxis = _position - _orbitCenter;
   _position += ( moveAxis * distance * _moveScale[1] );

   osg::notify( osg::NOTICE ) << "moveOrbit:\n" <<
       "\torbit center: " << _orbitCenter << "\n" <<
       "\told pos: " << oldPos << "\n" <<
       "\tnew pos: " << _position << "\n" <<
       "\tdistance: " << (oldPos-_position).length() << std::endl;
#endif
}



template< typename T >
T clampBetween( const T in, const T a, const T b )
{
    if( in < a ) return( a );
    if( in > b ) return( b );
    return( in );
}

void MxCore::getYawPitchRoll( double& yaw, double& pitch, double& roll, bool rightHanded ) const
{
#if 0
    osg::Matrix m( getOrientationMatrix() );
    // Reverse the view vector.
    m(2,0) = -m(2,0);
    m(2,1) = -m(2,1);
    m(2,2) = -m(2,2);

    // Orientation class assumes matrix contains only the delta transform
    // from base vectors to ypr, so subtract out the default orientation.
    const gmtl::Matrix44d baseInv( 1., 0., 0., 0.,
        0., 0., 1., 0.,
        0., 1., 0., 0.,
        0., 0., 0., 1. );

    osg::ref_ptr< osgwTools::Orientation > orient( new osgwTools::Orientation() );
    orient->setRightHanded( rightHanded );
    orient->getYPR( baseInv * m, yaw, pitch, roll );
#else
    // Temp var for cross products.
    gmtl::Vec3d right;

    const gmtl::Vec3d viewDirXBaseUp( _viewDir ^ _initialUp );


    // Yaw

    // Compute view direction, projected into plane defined by base up.
    // TBD what if _viewDir and _initialUp are coincident?
    gmtl::Vec3d projectedDir = _initialUp ^ viewDirXBaseUp;
    normalize( projectedDir );
    // Is the vector pointing to the left of north, or to the right?
    right = _initialDir ^ _initialUp;
    const double dotDirRight = dot( projectedDir, right );
    // Dot product of two unit vectors is the cosine of the angle between them.
    const double dotDirNorth = clampBetween< double >( dot( projectedDir, _initialDir ), -1., 1. );
    double yawRad = acos( dotDirNorth );
    if( dotDirRight > 0. )
        yawRad = gmtl::Math::PI + ( gmtl::Math::PI - yawRad );
    if( !rightHanded )
        yawRad = gmtl::Math::TWO_PI - yawRad;
    if( yawRad == gmtl::Math::TWO_PI )
        yawRad = 0.;
    yaw = gmtl::Math::rad2Deg( yawRad );


    // Pitch

    const double dotDirUp = dot( _viewDir, _initialUp );
    const double dotUpUp = clampBetween<double>( dot( _viewUp, _initialUp ), -1., 1. );
    double pitchRad = acos( abs( dotUpUp ) );
    if( dotDirUp < 0. )
        pitchRad *= -1.;
    pitch = gmtl::Math::rad2Deg( pitchRad );


    // Roll

    // Compute base up projected onto plane defined by view direction.
    // TBD what if _viewDir and _initialUp are coincident?
    gmtl::Vec3d projectedBaseUp( viewDirXBaseUp ^ _viewDir );
    normalize( projectedBaseUp );
    // Is the view up vector pointing to the left of the projected base up, or to the right?
    right = _viewDir ^ projectedBaseUp;
    const double dotUpRight = dot( _viewUp, right );
    // Dot product of two unit vectors is the cosine of the angle between them.
    const double dotUp( clampBetween<double>( dot( projectedBaseUp, _viewUp ), -1., 1. ) );
    double rollRad = acos( dotUp );
    if( dotUpRight > 0. )
        rollRad = gmtl::Math::PI + ( gmtl::Math::PI - rollRad );
    if( !rightHanded )
        rollRad = gmtl::Math::TWO_PI - rollRad;
    if( rollRad == gmtl::Math::TWO_PI )
        rollRad = 0.;
    roll = gmtl::Math::rad2Deg( rollRad );
#endif
}



//
// Projection / FOV support
//

void MxCore::setOrtho( bool ortho, const double viewDistance )
{
    _ortho = ortho;

    // tan (fovy/2) = a / e2c.len
    _orthoTop = tan( getFovyRadians() * .5 ) * viewDistance;
    _orthoBottom = -_orthoTop;
}

void MxCore::updateFovy( gmtl::Matrix44d& proj ) const
{
    JAG3D_NOTICE_STATIC( "jag.mxcore", "MxCore::updateFovy: TBD MxCore port. Not yet implemented." );
#if 0
    if( _ortho )
    {
        JAG3D_WARNING_STATIC( "jag.mxcore", "MxCore::updateFovy: Ortho is not yet implemented. TBD." );
    }
    else
    {
        double left, right, bottom, top, znear, zfar;
        proj.getFrustum( left, right, bottom, top, znear, zfar );

        const double fovBottom = atan( bottom / znear );
        const double fovTop = atan( top / znear );

        const double fovyRatio = getFovyRadians() /
            ( abs( fovBottom ) + abs( fovTop ) );

        const double newBottom = tan( fovBottom * fovyRatio ) * znear;
        const double newTop = tan( fovTop * fovyRatio ) * znear;
        const double xScale = newTop / top;
        left *= xScale;
        right *= xScale;
        proj = gmtl::Matrix44d::frustum( left, right, newBottom, newTop, znear, zfar );
    }
#endif
}
gmtl::Matrix44d MxCore::computeProjection( const double zNear, const double zFar ) const
{
    gmtl::Matrix44d ret;
    if( _ortho )
    {
        const double xRange = _aspect * ( _orthoTop - _orthoBottom );
        const double right = xRange * .5;

        gmtl::setOrtho( ret, -right, _orthoTop, right, _orthoBottom, zNear, zFar );
    }
    else
    {
        double myNear( zNear );
        if( zNear < 0. )
            myNear = zFar / 2000.;
        gmtl::setPerspective( ret, _fovy, _aspect, myNear, zFar );
    }
    return( ret );
}
gmtl::Matrix44d MxCore::computeProjection( const gmtl::Vec2d& nearFar ) const
{
    return( computeProjection( nearFar[ 0 ], nearFar[ 1 ] ) );
}

void MxCore::setFovy( double fovy )
{
    const double ratio = fovy / _fovy;
    _orthoBottom *= ratio;
    _orthoTop *= ratio;
    _fovy = fovy;
}
double MxCore::getFovyRadians() const
{
    return( gmtl::Math::deg2Rad( _fovy ) );
}
void MxCore::fovyScaleUp()
{
    _fovy *= _fovyScale;
    if( _clampFovyScale )
    {
        _fovy = min( _fovy, _clampFovyRange[ 1 ] );
    }

    _orthoBottom *= _fovyScale;
    _orthoTop *= _fovyScale;
}
void MxCore::fovyScaleDown()
{
    const double factor( 1.0 / _fovyScale );
    _fovy *= factor;
    if( _clampFovyScale )
    {
        _fovy = max( _fovy, _clampFovyRange[ 0 ] );
    }

    _orthoBottom *= factor;
    _orthoTop *= factor;
}
void MxCore::setClampFovyScale( bool clamp, gmtl::Vec2d clampFovyRange )
{
    _clampFovyScale = clamp;
    _clampFovyRange = clampFovyRange;
    if( _clampFovyScale )
    {
        _fovy = clampBetween< double >( _fovy, _clampFovyRange[ 0 ], _clampFovyRange[ 1 ] );
    }
}



void MxCore::orthonormalize()
{
    const gmtl::Vec3d c = getCross();
    _viewUp = c ^ _viewDir;
    normalize( _viewDir );
    normalize( _viewUp );
}


// jagMx
}
