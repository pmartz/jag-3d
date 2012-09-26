#include <stdio.h>
#include <cmath>
#include "Trackball.h"

// Hack... put in iiMath ?
#include <gmtl/Math.h>

namespace iiMath {
    template <typename T> inline T sqr( T t) { return gmtl::Math::sqr<T>(t); }
    template <typename T> inline T deg2rad(T t) { return gmtl::Math::deg2Rad(t); }
};


Trackball::Trackball( void ):
    EventResponder(true), // Auto register
    TX(defTX)
{ 
    gmtl::identity( R );
    gmtl::identity( O );
    gmtl::identity( T );
    gmtl::identity( S );
    gmtl::identity( Tr );
    gmtl::identity( Rr );

    _scale = 1.0;
    _minimum_scale_is_set = false;

    _rscale = 1.0;
    _distance_ref = 0.0;
    _distance_has_changed = true;
    _min_distance_is_set = false;
    _computeOrientation = true;
    
    _rotation = true;
    _panning  = true;
    _distancing = true;

    _throwThreshold = 0.0;


    _mx = _my = 0.0;
    _button = 0;
    _pan_fov = gmtl::Math::deg2Rad(45.0f);
    setOperationalMode( DefaultOperationalMode );
    reset();
}

void Trackball::setMinimumDistance( float dist )
{
    _min_distance = dist;
    _min_distance_is_set = true;
}

void Trackball::setDistance( float dist, bool do_update )
{
#define ALLOW_PAST_0
#ifdef ALLOW_PAST_0
    _distance = dist;
#else
    if( dist > 0.0 )
        _distance = dist;
    else
        return;
#endif

    _distance_has_changed = true;
    if( _auto_scale) 
        updateScale();

    if( do_update ) 
        update();
}

void Trackball::resetDistance(bool do_update)
{
    setDistance(_distance_ref, do_update );
}    

void Trackball::setOrientation( Orientation mode )
{
    _orientation = mode;
    switch (_orientation) 
    {
        case Y_UP:
                gmtl::identity( O );
                break;

        case Z_UP:
            {
                float x = -(float)(M_PI) / 2.0f;
                float y = 0.0f;
                float z = 0.0f;
                gmtl::setRot( O, gmtl::EulerAngleXYZf( x, y, z ));
            }
                break;

    }
}

void Trackball::setOperationalMode( OperationalMode mode )
{
    _operational_mode = mode;
    switch( mode )
    {
    case DefaultOperationalMode :
            _buttonMap[0] = UpdateNone;
            _buttonMap[1] = UpdateRotation;
            _buttonMap[2] = UpdatePan;
            _buttonMap[3] = UpdateNone;
            _buttonMap[4] = UpdateDistance;
            _buttonMap[5] = UpdateNone;
            _buttonMap[6] = UpdateNone;
            _buttonMap[7] = UpdateNone;

        setOrientation( Z_UP );
        setRotationalMode( Spherical ); 
        setTransformOrder( TranslateRotate );
        setThrowMode( ThrowRotationPanDistance );
        setAutoScale(true);
        break;
    case InventorLike:
            _buttonMap[0] = UpdateNone;
            _buttonMap[1] = UpdateRotation;
            _buttonMap[2] = UpdatePan;
            _buttonMap[3] = UpdateDistance;
            _buttonMap[4] = UpdateNone;
            _buttonMap[5] = UpdateRotation;
            _buttonMap[6] = UpdatePan;
            _buttonMap[7] = UpdateDistance;

        setOrientation( Y_UP );
        setRotationalMode( Spherical ); 
        setTransformOrder( TranslateRotate );
        setThrowMode( ThrowRotation );
        setAutoScale(true);
        break;

    case PerformerLike:
            _buttonMap[0] = UpdateNone;
            _buttonMap[1] = UpdatePan;
            _buttonMap[2] = UpdateRotation;
            _buttonMap[3] = UpdateNone;
            _buttonMap[4] = UpdateDistance;
            _buttonMap[5] = UpdateDistance;
            _buttonMap[6] = UpdateNone; 

            // Performer trackball rotates about 'z', 
            // when bits 2 and 3 are set, but this 
            // mode is unsupported
            _buttonMap[7] = UpdateNone;

        setOrientation( Z_UP );
        setRotationalMode( FixedAxis ); 
        setTransformOrder( RotateTranslate );
        setThrowMode( ThrowRotationPanDistance );
        setAutoScale(false);

        break;
    }
}

iiMath::matrix4f &Trackball::getMatrix( void )
{
    return TX;
}

const iiMath::matrix4f &Trackball::getMatrix( void ) const
{
    return TX;
}

void Trackball::setMatrix( const iiMath::matrix4f mat )
{
    iiMath::matrix4f _DD;
    if( _orientation == Y_UP )
        _DD = iiMath::Translatef( 0, 0, _distance );
    else if( _orientation == Z_UP )
        _DD = iiMath::Translatef( 0, -_distance, 0 );

    if( _computeOrientation )
    {
        iiMath::matrix4f _Oinv; 
        gmtl::invertFull( O, _Oinv );
        //_Oinv.invert(O);
        //R = mat * _Oinv * _DD;
        R = _DD * _Oinv * mat;
    }
    else
    {
        //R = mat * _DD;
        R = _DD * mat;
    }

    gmtl::identity( T );
    //T.makeIdentity();

    if( _transform_order == TranslateRotate )
    {
        iiMath::matrix4f t;
        gmtl::invertFull_orig( R, t );
        T(3,0) = -t(3,0);
        T(3,1) = -t(3,1);
        T(3,2) = -t(3,2);
    }
    else if( _transform_order == RotateTranslate )
    {
        T(3,0) = R(3,0);
        T(3,1) = R(3,1);
        T(3,2) = R(3,2);
    }
    //R(3,0) = R(3,1) = R(3,2) = 0.0;
    R(0,3) = R(1,3) = R(2,3) = 0.0;
}

void Trackball::restart(float x, float y)
{
    _lastx = x;
    _lasty = y;

}

void Trackball::setReference( void )
{
    Tr = T;
    Rr = R;
    _distance_ref = _distance;
    gmtl::identity( T );
    gmtl::identity( R );
    reset();
}

void Trackball::reset( bool do_update )
{
    T = Tr;
    R = Rr;
    _distance = _distance_ref;
    _distance_has_changed = true;
    _mbutton = 0;
    _update_mode = UpdateNone;
    _dx = _dy = 0.0f;
    restart( 0.0, 0.0);
    if( _auto_scale) 
        updateScale();
    if( do_update ) 
        update();
}

void Trackball::update(void)
{
    iiMath::matrix4f D;

    if( _orientation == Y_UP )
        D = iiMath::Translatef( 0, 0, -_distance );
    else if( _orientation == Z_UP )
        D = iiMath::Translatef( 0, _distance, 0 );


    if( _computeOrientation )
    {
        if( _transform_order == TranslateRotate )
        {
            TX = O * D * R * T * S;
            //TX = T * (S * (R * (D  * O ))); 
        }
        else if( _transform_order == RotateTranslate )
        {
            //TX = R * (T * (D  * O )); 
            TX = O * D * T * R;
        }
    }
    else
    {
        if( _transform_order == TranslateRotate )
        {
            TX = D * R * T * S;
            //TX = T * (S * (R * D)); 
        }
        else if( _transform_order == RotateTranslate )
        {
            TX = D * T * R * S;
            //TX = R * (S * (T * D)); 
        }
    }
}

void Trackball::input( float mx, float my, unsigned int mbutton, bool do_update )
{
    _distance_has_changed = false;
    if( _mbutton != mbutton )
    {
        _mbutton = mbutton;
        if( _mbutton ) 
        {
            restart( mx, my); 
        }
    }

    if( _mbutton )
    {
        _update_mode = getMappedButtonState( _mbutton );
        _dx = _lastx - mx;
        _dy = _lasty - my;
    }

    switch( _update_mode )
    {
        case UpdateRotation :
            if( _rotation == false ) 
            {
                _update_mode = UpdateNone;
                break;
            }

            if( _rotational_mode == FixedAxis )
            {
                float mag = sqrt(iiMath::sqr<float>(_dy)+iiMath::sqr<float>(_dx));
                //float mag = gmtl::Math::sqrt(gmtl::Math::sqr<float>(_dy)+gmtl::Math::sqr<float>(_dx));
                if( mag != 0.0 )
                {
                    if( _orientation == Y_UP )
                        R *= iiMath::Rotatef( 2.0 * asin(mag),(float)_dy, (float)-_dx, 0.0);
                    else if( _orientation == Z_UP )
                        R *= iiMath::Rotatef( 2.0 * asin(mag),(float)_dy, 0.0, (float)-_dx);
                }
            }
            else if( _rotational_mode == Spherical )
            {
                float ry = -(_lastx - _dx) * iiMath::deg2rad<float>(90.0);
                float rx =  (_lasty - _dy) * iiMath::deg2rad<float>(90.0);
                iiMath::vec3f a( std::cos(rx) * -std::sin(ry), std::sin(rx), std::cos(rx) * std::cos(ry) ); 
                ry = -_lastx * iiMath::deg2rad<float>(90.);
                rx =  _lasty * iiMath::deg2rad<float>(90.);
                iiMath::vec3f b( std::cos(rx) * -std::sin(ry), std::sin(rx), std::cos(rx) * std::cos(ry) ); 

                iiMath::vec3f n;
                gmtl::cross( n, b, a );
                float angle = std::sin(gmtl::length( n ));
                if( angle > 0.0001 )
                {
                    gmtl::normalize( n );
                    if( _orientation == Z_UP )
                    {
                        n = iiMath::Rotatef( M_PI*0.5, 1.0, 0.0, 0.0 ) * n;
                    }
                    /*
                    * This is an optional way of doing the above generically,
                    * but more expensive
                    iiMath::matrix4atrix o;
                    o.invert(O);
                    n = n * o;
                    */

                    angle *= _rscale;
                    R = iiMath::Rotatef( angle, n[0], n[1], n[2] ) * R;
                }
            }
            if( (_throw_mode & ThrowRotation) == 0 )
                _update_mode = UpdateNone;
            break;

        case UpdatePan :
          {
            if( _panning == false ) 
            {
                _update_mode = UpdateNone;
                break;
            }
            iiMath::vec3f pan;
            pan.set ( _dx * _scale, _dy * _scale, 0.0);

            /*
               iiMath::matrix4f o;
            o.invert(O);
            pan = pan * o;
            */
            if( _orientation == Z_UP )
            {
                //pan = pan * iiMath::Rotatef( M_PI*0.5f, 1.0f, 0.0f, 0.0f );
                //gmtl::xform( pan, iiMath::Rotate( M_PI*0.5, 1.0, 0.0, 0.0 ), pan );
                pan = iiMath::Rotatef( M_PI*0.5, 1.0, 0.0, 0.0 ) * pan;
            }

            if( _transform_order == TranslateRotate )
            {
                iiMath::matrix4f r;
                gmtl::invertFull_orig( r, R );
                pan = r * pan;
            }

            T(0,3) -= pan[0] / S(0,0);
            T(1,3) -= pan[1] / S(1,1);
            T(2,3) -= pan[2] / S(2,2);

            if( (_throw_mode & ThrowPan) == 0 )
                _update_mode = UpdateNone;
            break;
          }

        case UpdateDistance :
          {
            if( _distancing == false ) 
            {
                _update_mode = UpdateNone;
                break;
            }
#ifdef ALLOW_PAST_0
            _distance -= _dy * _scale * 10;
            if( _min_distance_is_set && _distance < _min_distance )
                _distance = _min_distance;
#else
            float dd = _dy * _scale * 10;
            if( _distance - dd <= 0.0 ) 
                break;
            _distance -= dd;
#endif
            _distance_has_changed = true;
            if( _auto_scale ) 
                updateScale();

            if( (_throw_mode & ThrowDistance) == 0 )
                _update_mode = UpdateNone;
          }
          break;

       default:
           break;
    }

    if( _mbutton )
    {
        _lastx = mx;
        _lasty = my;
    }

    if( fabs(_dx) < _throwThreshold && fabs(_dy) < _throwThreshold )
        _update_mode = UpdateNone;

    if( do_update )
        update();
}

void Trackball::translate( float x, float y, float z, bool do_update )
{
    T *= iiMath::Translatef( x, y, z);
    if( do_update ) 
        update();
}

void Trackball::rotate( float rad, float x, float y, float z, bool do_update)
{

    if( _orientation == Y_UP )
        R *= iiMath::Rotatef( rad, x, y, z );
    else if( _orientation == Z_UP )
        R *= iiMath::Rotatef( rad, x, z, y );
    if( do_update ) 
        update();
}

void Trackball::scale( float sx, float sy, float sz, bool do_update )
{
    S = iiMath::Scale(sx) * S;
    if( do_update ) 
        update();
}

void Trackball::setScale( float scale )
{
    if( !_minimum_scale_is_set || scale >= _minimum_scale )
    _scale = scale;
}

void Trackball::setMinimumScale( float minScale )
{
    _minimum_scale_is_set = true;
    _minimum_scale = minScale;
}

void Trackball::updateScale()
{
    _scale = _pan_fov * 0.5 * fabs(_distance);
    if( _scale < 1.0 ) _scale = 1.0;
}

bool Trackball::buttonPress( unsigned int mbutton, float mx, float my ) 
{
    _button |= (1<<(mbutton-1));
    _mx = mx;
    _my = my;
    return false;
}

bool Trackball::buttonRelease( unsigned int mbutton, float mx, float my )
{
    _button &= ~(1<<(mbutton-1));
    _mx = mx;
    _my = my;
    return false;
}

bool Trackball::mouseMotion( float mx, float my) 
{
    _mx = mx;
    _my = my;
    return false;
}

bool Trackball::keyPress( iiWS::KeyCharacter key, bool isRepeat ) 
{
    if( key == ' ') 
        reset();
    return false;
}

void Trackball::frame( unsigned int count )
{
    this->input( _mx, _my, _button, true );
}
