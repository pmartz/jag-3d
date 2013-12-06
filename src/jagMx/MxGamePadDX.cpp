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


#include <jagMx/MxGamePadDX.h>
#include <jag/base/LogMacros.h>

#include <windows.h>
#include <dinput.h>
#include <tchar.h>


namespace jagMx {


// DX GamePad axis min/max are customizable. Our code sets them
// large, so that we maintain prevision during normalization to
// the range -1 to 1 (required by the base class MxGamePad).
#define MIN_AXIS_VALUE -10000.f
#define MAX_AXIS_VALUE 10000.f
#define HALF_AXIS_RANGE 10000.f

    
// Statics
bool MxGamePadDX::s_winClassRegistered( false );

static LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );



// Forward
BOOL CALLBACK EnumAxisCallback(const DIDEVICEOBJECTINSTANCE* pDIDOI, void* pContext );
inline float normalizeAxisValue( LONG value );



MxGamePadDX::MxGamePadDX()
  : MxGamePad(),
    _hDIWindow( 0 ),
    _pDI( 0 ),
    _pDIDevice( 0 ),
    _devCaps( DIDEVCAPS() )
{
    // Base class stick units are normalized -1 to 1. Dead zone of 0.05
    // means that stick movement in range -0.05 to 0.05 will be ignored.
    setStickDeadZone( 0.05f );

    // DirectInput requires a Window from the app, so try to create one now.
    // This window will be a top-level window (not a child window), but will be
    // invisible to the user.
    if( ( _hDIWindow = createInvisiWindow() ) == 0 )
        return;

    // try to open DirectInput 8 for use. This works as far back as Windows 95 (DirectX 8.0a sdk).
    if( openDirectInput() )
        selectFirstDevice();    // try to select the first gaming device on the system.
}
MxGamePadDX::MxGamePadDX( const MxGamePadDX& rhs )
  : MxGamePad( rhs ),
    _hDIWindow( rhs._hDIWindow ),
    _pDI( rhs._pDI ),
    _pDIDevice( rhs._pDIDevice ),
    _devCaps( rhs._devCaps )
{
}
MxGamePadDX::~MxGamePadDX()
{
    freeDirectInput();
    destroyWindow();
}

bool MxGamePadDX::poll( const double deltaSeconds )
{
    if( _pDIDevice == 0 )
        return( false );

    HRESULT rc = _pDIDevice->Poll();
    if( ( rc != DI_OK ) && ( rc != DI_NOEFFECT ) )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX::poll: failure." );
        _pDIDevice->Acquire();            // try to reacquire device if lost it
        return( false );
    }

    // read the raw device state information in DirectInput format.
    DIJOYSTATE2 devState;
    if( _pDIDevice->GetDeviceState( sizeof( DIJOYSTATE2 ), &devState ) != DI_OK )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX::poll: GetDeviceState failed." );
        return( false ); // Something went wrong.
    }

    // Button pressed or not pressed could alter behavior of sticks/dpad,
    // so process buttons first.
    processButtons( devState, deltaSeconds );
    processSticks( devState, deltaSeconds );
    processDPad( devState );

    return( true ); // Success.
}

void MxGamePadDX::processButtons( const DIJOYSTATE2& devState, const double deltaSeconds )
{
    unsigned int buttons( 0 );

    // Map DX GamePad buttons to base class button enums.
    if( devState.rgbButtons[ 0 ] )
        buttons |= Button0;
    if( devState.rgbButtons[ 1 ] )
        buttons |= Button1;
    if( devState.rgbButtons[ 2 ] )
        buttons |= Button2;
    if( devState.rgbButtons[ 3 ] )
        buttons |= Button3;
    if( devState.rgbButtons[ 6 ] )
        buttons |= Button6;
    if( devState.rgbButtons[ 7 ] )
        buttons |= Button7;

    // The two Xbox 360 controller front firing buttons are handled as one button
    // in DirectInput, with negative and positive values.
    if( devState.lZ > 0 )
        buttons |= Button4;
    else if( devState.lZ < 0 )
        buttons |= Button5;

    // Must call into base class even if all buttons are zero
    // so that base class can detect deltas (press events).
    setButtons( buttons, deltaSeconds );
}

void MxGamePadDX::processSticks( const DIJOYSTATE2& devState, const double deltaSeconds )
{
    float x, y;

    // Left stick: Move.
    // Normalize values to range -1.0 to 1.0.
    // These are units to move in world coordinates per event or per frame.
    x = -normalizeAxisValue( devState.lX );
    y = normalizeAxisValue( devState.lY );
    setLeftStick( x, y, deltaSeconds );

    // Right stick: Rotate.
    // Base class angle values are in degrees. By calling
    // normalizeAxisValue, we pass in -1 to 1 degrees.
    // Compensate for rotation as well:
    //  x value around up vector, positive values counterclockwise
    //  y value around right/cross vector, positive values counterclockwise
    //    NOTE .lZ is positive when pulled back. This is the opposite of
    //    the left gamepad stick.
    x = -normalizeAxisValue( devState.lRx );
    y = normalizeAxisValue( devState.lRy );
    setRightStick( x, y, deltaSeconds );
}

void MxGamePadDX::processDPad( const DIJOYSTATE2& devState )
{
}



HWND MxGamePadDX::createInvisiWindow()
{
    HINSTANCE hMod = ( HINSTANCE )( GetModuleHandle( 0 ) );
    LPCTSTR className = _T( "MxInputAdapterGamePadDirectInput Window" );
    if( !s_winClassRegistered )
    {
        s_winClassRegistered = true;

        WNDCLASSEX wc;
        memset(&wc, 0, sizeof(wc));
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = &WinProc;
        wc.hInstance = hMod;
        wc.lpszClassName = className;
        if (RegisterClassEx(&wc) == 0)
            return 0;
    }
    return( ::CreateWindow( className, _T( "DirectInput Window" ), WS_POPUP, 0, 0, 1, 1, 0, 0, hMod, 0 ) );
}

bool MxGamePadDX::openDirectInput()
{
    if( _pDI != 0 )
        return true;

    HRESULT rc = DirectInput8Create( GetModuleHandle( 0 ),
        DIRECTINPUT_VERSION, IID_IDirectInput8, ( VOID ** )&_pDI, 0 );
    if( rc != DI_OK )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX: Failed to open Direct Input." );
    }

    return( rc == DI_OK );
}

bool MxGamePadDX::selectFirstDevice()
{
    // if no devices attached to the system, then return failure.
    enumDevices();
    if( _devList.empty() )
        return( false );

    return( selectDevice( *( _devList.begin() ) ) );
}

bool MxGamePadDX::selectDevice( const DIDEVICEINSTANCE& device )
{
    // release any currently held device.
    freeDevice();

    // create a device interface for the specified device.
    if( _pDI->CreateDevice( device.guidInstance, &_pDIDevice, 0 ) != DI_OK )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX: Unable to select device." );
        _pDIDevice = 0;
        return( false );
    }

    // set the type of data format we want from DirectInput.
    if( _pDIDevice->SetDataFormat( &c_dfDIJoystick2 ) != DI_OK )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX: SetDataFormat() failed." );
        freeDevice();
        return( false );
    }

    // use the device in a non-exclusive manner. No need to acquire it exclusively.
    // background access does not require that the window be the currently active window,
    // and will not automatically unacquire the device when the window is not the active window.
    if( _pDIDevice->SetCooperativeLevel( _hDIWindow, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE ) != DI_OK )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX: SetCooperativeLevel() failed." );
        freeDevice();
        return( false );
    }

    _devCaps.dwSize = sizeof( DIDEVCAPS );
    if( _pDIDevice->GetCapabilities( &_devCaps ) != DI_OK )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX: GetCapabilities() failed." );
        freeDevice();
        return( false );
    }

    // set range of axis
    if( _pDIDevice->EnumObjects( EnumAxisCallback, _pDIDevice, DIDFT_AXIS ) != DI_OK )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX: EnumObjects() failed." );
        freeDevice();
        return( false );
    }

    HRESULT rc;
    // try to acquire the specified device for use.
    if( ( ( rc = _pDIDevice->Acquire() ) != DI_OK ) &&
        ( rc != S_FALSE ) )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX: Failure to acquire device." );
        freeDevice();
        return( false );
    }

    return( true );
}

bool MxGamePadDX::enumDevices()
{
    if( _pDI == 0 )
        return( false );

    _devList.clear();

    HRESULT rc = _pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, enumDevicesCallback, this, DIEDFL_ATTACHEDONLY );
    if( rc != DI_OK )
    {
        JAG3D_WARNING_STATIC( "jag.mx.gamepad.dx", "MxGamePadDX: enumDevices failed." );
    }

    return( rc == DI_OK );
}

void MxGamePadDX::freeDevice()
{
    if( _pDIDevice )
    {
        _pDIDevice->Unacquire();
        _pDIDevice->Release();
        _pDIDevice = 0;
    }
}

void MxGamePadDX::freeDirectInput()
{
    freeDevice();
    if( _pDI )
    {
        _pDI->Release();
        _pDI = 0;
    }
}

void MxGamePadDX::destroyWindow()
{
    if( _hDIWindow )
    {
        ::DestroyWindow( _hDIWindow );
        _hDIWindow = 0;
    }
}



BOOL CALLBACK EnumAxisCallback( const DIDEVICEOBJECTINSTANCE* pDIDOI, void* pContext )
{
    DIPROPRANGE pr;
    pr.diph.dwSize = sizeof( DIPROPRANGE );
    pr.diph.dwHeaderSize = sizeof( DIPROPHEADER );
    pr.diph.dwHow = DIPH_BYID;
    pr.diph.dwObj = pDIDOI->dwType;           // the axis being enumerated
    // set min & max range values. this is the whole purpose of enumerating the axis.
    pr.lMin = (LONG)( MIN_AXIS_VALUE );
    pr.lMax = (LONG)( MAX_AXIS_VALUE );
    ( ( LPDIRECTINPUTDEVICE8 )pContext )->SetProperty( DIPROP_RANGE, &pr.diph );
    return( DIENUM_CONTINUE );                   // let's do 'em all, thank you
}

BOOL CALLBACK enumDevicesCallback( const DIDEVICEINSTANCE* pdidInstance, void* pUser )
{
    ( ( MxGamePadDX* )pUser )->_devList.push_back( *pdidInstance );
    return( DIENUM_CONTINUE );                   // next please
}

LRESULT CALLBACK WinProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
}

inline float normalizeAxisValue( LONG value )
{
    return( ( ( value - MIN_AXIS_VALUE ) / HALF_AXIS_RANGE ) - 1.f );
}


// osgwMx
}
