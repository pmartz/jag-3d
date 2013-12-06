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


#include <jagMx/MxGamePad.h>
#include <jag/base/LogMacros.h>

#include <gmtl/gmtl.h>


namespace jagMx {


MxGamePad::MxGamePad()
  : _leftStick( 0.f, 0.f ),
    _rightStick( 0.f, 0.f ),
    _buttons( 0 ),
    _deadZone( 0.f ),
    _leftRate( 1. ),
    _rightRate( 60. ),
    _moveMode( FunctionalMap::MoveModeLocal ),
    _rotateMode( FunctionalMap::RotateModeLocal )
{
    _mxCore = MxCorePtr( new jagMx::MxCore );

    // Create a default functional map.
    _map = FunctionalMapPtr( new jagMx::FunctionalMap );
    _map->configure( Button0, FunctionalMap::MoveModifyUpDown );
    _map->configure( Button1, FunctionalMap::LevelView );
    _map->configure( Button2, FunctionalMap::JumpToWorldOrigin );
    _map->configure( Button3, FunctionalMap::JumpToHomePosition );
    _map->configure( Button4, FunctionalMap::MoveModifyScaleSpeedDown );
    _map->configure( Button5, FunctionalMap::MoveModifyScaleSpeedUp );
    _map->configure( Button6, FunctionalMap::RotateModeOrbit );
    _map->configure( Button7, FunctionalMap::RotateModeLocal );
}
MxGamePad::MxGamePad( const MxGamePad& rhs )
  : _leftStick( rhs._leftStick ),
    _rightStick( rhs._rightStick ),
    _buttons( rhs._buttons ),
    _deadZone( rhs._deadZone ),
    _leftRate( rhs._leftRate ),
    _rightRate( rhs._rightRate ),
    _mxCore( new jagMx::MxCore( *( rhs._mxCore ) ) ),
    _map( new jagMx::FunctionalMap( *( rhs._map ) ) ),
    _moveMode( rhs._moveMode ),
    _rotateMode( rhs._rotateMode )
{
}
MxGamePad::~MxGamePad()
{
}

bool MxGamePad::setLeftStick( const float x, const float y )
{
    _leftStick.set( x, y );

    // Zero the values if they fall within the dead zone.
    float myX( deadZone( x ) );
    float myY( deadZone( y ) );

    if( myX == 0 && myY == 0 )
    {
        return( false );
    }
    
    internalLeftStick( myX, myY );
    
    return( true );
}
bool MxGamePad::setLeftStick( const float x, const float y, const double deltaSeconds )
{
    _leftStick.set( x, y );

    // Zero the values if they fall within the dead zone.
    const float myX( deadZone( x ) );
    const float myY( deadZone( y ) );

    if( myX == 0 && myY == 0 )
    {
        return( false );
    }
    
    // How far do we go at 100% movement?
    const float maxDistance = (float)( _leftRate * deltaSeconds );

    internalLeftStick( myX * maxDistance, myY * maxDistance );
    
    return( true );
}
void MxGamePad::internalLeftStick( const float x, const float y )
{
    // Check for forward/backward or up/down.
    gmtl::Vec3d movement;
    if( _map->isSet( FunctionalMap::MoveModifyUpDown ) )
    {
        // Move left/right and up/down.
        // Positive values move up, so negate y.
        movement.set( x, -y, 0. );
    }
    else
    {
        // Move left/right and forwards/backwards.
        movement.set( x, 0., y );
    }

    switch( getMoveMode() )
    {
    default:
        {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad", "Unsupported move mode: \"" + FunctionalMap::asString( getMoveMode() ) + "\"." );
        // Intentional fallthrough.
        }
    case FunctionalMap::MoveModeLiteral:
        _mxCore->moveLiteral( movement );
        break;
    case FunctionalMap::MoveModeLocal:
        _mxCore->moveLocal( movement );
        break;
    case FunctionalMap::MoveModeConstrained:
        _mxCore->moveConstrained( movement );
        break;
    case FunctionalMap::MoveModeOriented:
        _mxCore->moveOriented( movement );
        break;
    case FunctionalMap::MoveModeWorld:
        _mxCore->moveWorld( movement );
        break;
    case FunctionalMap::MoveModeOrbit:
        _mxCore->moveOrbit( y );
        break;
    }
}

bool MxGamePad::setRightStick( const float x, const float y )
{
    _rightStick.set( x, y );

    // Zero the values if they fall within the dead zone.
    float myX( deadZone( x ) );
    float myY( deadZone( y ) );

    if( myX == 0 && myY == 0 )
    {
        return( false );
    }
    
    internalRightStick( myX, myY );
    
    return( true );
}
bool MxGamePad::setRightStick( const float x, const float y, const double deltaSeconds )
{
    _rightStick.set( x, y );

    // Zero the values if they fall within the dead zone.
    float myX( deadZone( x ) );
    float myY( deadZone( y ) );

    if( myX == 0 && myY == 0 )
    {
        return( false );
    }

    // How far do we turn at 100% rotation?
    const float maxDegrees = (float)( _rightRate * deltaSeconds );
    
    internalRightStick( myX * maxDegrees, myY * maxDegrees );
    
    return( true );
}
void MxGamePad::internalRightStick( const float x, const float y )
{
    // Input is degrees, but MxCore wants radians.
    const double myX = gmtl::Math::deg2Rad( x );
    const double myY = gmtl::Math::deg2Rad( y );

    if( _map->isSet( FunctionalMap::RotateModifyRoll ) )
    {
        _mxCore->rotateLocal( myX, _mxCore->getDir() );
    }
    else
    {
        switch( getRotateMode() )
        {
        default:
            {
            JAG3D_WARNING_STATIC( "jag.mx.gamepad", "Unsupported rotate mode: \"" + FunctionalMap::asString( getRotateMode() ) + "\"." );
            // Intentional fallthrough.
            }
        case FunctionalMap::RotateModeLocal:
            _mxCore->rotateLocal( myX, _mxCore->getUp() );
            _mxCore->rotateLocal( myY, _mxCore->getCross() );
            break;
        case FunctionalMap::RotateModeOrbit:
            _mxCore->rotateOrbit( myX, _mxCore->getUp() );
            _mxCore->rotateOrbit( myY, _mxCore->getCross() );
            break;
        case FunctionalMap::RotateModeArcball:
            {
            JAG3D_WARNING_STATIC( "jag.mx.gamepad", "RotateModeArcball not yet implemented." );
            break;
            }
        }
    }
}

void MxGamePad::setMoveMode( const FunctionalMap::FunctionType mode )
{
    if( FunctionalMap::validMoveMode( mode ) )
        _moveMode = mode;
    else
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad", "Invalid move mode: \"" +
            FunctionalMap::asString( mode ) + "\"." );
    }
}
void MxGamePad::cycleMoveMode()
{
    setMoveMode( FunctionalMap::cycleMoveMode( getMoveMode() ) );
}
void MxGamePad::setRotateMode( const FunctionalMap::FunctionType mode )
{
    if( FunctionalMap::validRotateMode( mode ) )
        _rotateMode = mode;
    else
        JAG3D_WARNING_STATIC( "jag.mx.gamepad", "Invalid rotate mode: \"" +
            FunctionalMap::asString( mode ) + "\"." );
}
void MxGamePad::cycleRotateMode()
{
    setRotateMode( FunctionalMap::cycleRotateMode( getRotateMode() ) );
}


void MxGamePad::setButtons( const unsigned int buttons )
{
    // Determine which buttons just entered a pressed or released state.
    const unsigned int deltaPressed = ( buttons ^ _buttons ) & buttons;
    const unsigned int deltaReleased = ( buttons ^ _buttons ) & _buttons;


    //
    // Handle buttons that have just been pressed.
    _map->setFromBitmask( deltaPressed );

    if( _map->isSet( FunctionalMap::JumpToHomePosition ) )
        _mxCore->reset();
    if( _map->isSet( FunctionalMap::JumpToWorldOrigin ) )
        _mxCore->setPosition( gmtl::Point3d( 0., 0., 0. ) );
    if( _map->isSet( FunctionalMap::LevelView ) )
        _mxCore->level();

    if( _map->isSet( FunctionalMap::CycleMoveMode ) )
        cycleMoveMode();
    if( _map->isSet( FunctionalMap::MoveModeLocal ) )
        setMoveMode( FunctionalMap::MoveModeLocal );
    if( _map->isSet( FunctionalMap::MoveModeConstrained ) )
        setMoveMode( FunctionalMap::MoveModeConstrained );
    if( _map->isSet( FunctionalMap::MoveModeOriented ) )
        setMoveMode( FunctionalMap::MoveModeOriented );
    if( _map->isSet( FunctionalMap::MoveModeWorld ) )
        setMoveMode( FunctionalMap::MoveModeWorld );
    if( _map->isSet( FunctionalMap::MoveModeOrbit ) )
        setMoveMode( FunctionalMap::MoveModeOrbit );

    if( _map->isSet( FunctionalMap::CycleRotateMode ) )
        cycleRotateMode();
    if( _map->isSet( FunctionalMap::RotateModeLocal ) )
        setRotateMode( FunctionalMap::RotateModeLocal );
    if( _map->isSet( FunctionalMap::RotateModeOrbit ) )
        setRotateMode( FunctionalMap::RotateModeOrbit );
    if( _map->isSet( FunctionalMap::RotateModeArcball ) )
        setRotateMode( FunctionalMap::RotateModeArcball );

    // Scale movement based on right shoulder button state.
    if( _map->isSet( FunctionalMap::MoveModifyScaleSpeedDown ) )
        _mxCore->setMoveScale( gmtl::Vec3d( .33, .33, .33 ) );
    else if( _map->isSet( FunctionalMap::MoveModifyScaleSpeedUp ) )
        _mxCore->setMoveScale( gmtl::Vec3d( 3., 3., 3. ) );


    //
    // Handle buttons that have just been released.
    _map->setFromBitmask( deltaReleased );

    // If either the ScaleUp or ScaleDown buttons were released, restore
    // the move speed.
    if( _map->isSet( FunctionalMap::MoveModifyScaleSpeedDown ) ||
        _map->isSet( FunctionalMap::MoveModifyScaleSpeedUp ) )
        _mxCore->setMoveScale( gmtl::Vec3d( 1., 1., 1. ) );


    _buttons = buttons;
    _map->setFromBitmask( buttons );
}

void MxGamePad::setButtons( const unsigned int buttons, const double deltaSeconds )
{
    setButtons( buttons );

    // How far do we go at 100% movement?
    const float maxDistance = (float)( _leftRate * deltaSeconds );

    gmtl::Vec3d movement;
    if( _map->isSet( FunctionalMap::MoveUpAtRate ) )
        movement[1] = -maxDistance;
    else if( _map->isSet( FunctionalMap::MoveDownAtRate ) )
        movement[1] = maxDistance;

    if( _map->isSet( FunctionalMap::MoveModeWorld ) )
    {
        _mxCore->moveWorld( movement );
        return;
    }
    if( _map->isSet( FunctionalMap::MoveModeConstrained ) )
    {
        _mxCore->moveConstrained( movement );
        return;
    }
    if( _map->isSet( FunctionalMap::MoveModeOriented ) )
    {
        _mxCore->moveOriented( movement );
        return;
    }
    
    // By default we will move in local coordinate space
    _mxCore->moveLocal( movement );
}


// jagMx
}
