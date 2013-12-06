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

#ifndef __JAGMX_MX_GAME_PAD_DX_H__
#define __JAGMX_MX_GAME_PAD_DX_H__ 1


#include <jagMx/Export.h>
#include <jagMx/MxGamePad.h>
#include <jag/base/ptr.h>

#include <dinput.h>

#include <list>


namespace jagMx {


/** \addtogroup jagMxGamepad Gamepad Device Support */
/*@{*/

/** \class MxGamePadDX MxGamePadDX.h <jagMx/MxGamePadDX.h>
\brief A DirectInput implementation of the MxGamePad class.
*/
class JAGMX_EXPORT MxGamePadDX : public MxGamePad
{
public:
    MxGamePadDX();
    MxGamePadDX( const MxGamePadDX& rhs );
    virtual ~MxGamePadDX();

    /** \brief Poll the gamepad and send device state to the base class.

    Calling application should invoke this function once per frame.

    If you are implementing another MxGamePad-based class, and it is
    non-polling, you'll need to code up some kind of function to handle
    your events. Use poll() as a model for how to call into the base
    class in response to the events you receive.
    */
    bool poll( const double elapsedSeconds );

protected:
    //
    // DX Support
    //

    // I feel so sorry for anyone who has to code to the Windows
    // API on a daily basis...

    static bool s_winClassRegistered;       // true if the window class has been registered

    friend static BOOL CALLBACK enumDevicesCallback(const DIDEVICEINSTANCE *pdidInstance, void *pContext);

    HWND _hDIWindow;                        // handle to the top-level window that DirectInput needs.
    LPDIRECTINPUT8 _pDI;                    // a pointer to the DirectInput v8 COM interface.
    LPDIRECTINPUTDEVICE8 _pDIDevice;        // pointer to the current device being used by this.
    DIDEVCAPS _devCaps;

    typedef std::list< DIDEVICEINSTANCE > DeviceList;
    DeviceList _devList;   // list of gaming devices on the system.

    HWND createInvisiWindow();
    bool openDirectInput();
    bool selectFirstDevice();
    bool enumDevices();
    bool selectDevice( const DIDEVICEINSTANCE& device );
    void freeDevice();
    void freeDirectInput();
    void destroyWindow();

    void processButtons( const DIJOYSTATE2& devState, const double deltaSeconds );
    void processSticks( const DIJOYSTATE2& devState, const double deltaSeconds );
    void processDPad( const DIJOYSTATE2& devState );
};

typedef jag::base::ptr< jagMx::MxGamePadDX >::shared_ptr MxGamePadDXPtr;


/*@}*/


// jagMx
}


// __JAGMX_MX_GAME_PAD_DX_H__
#endif
