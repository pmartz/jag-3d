/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 
 Copyright 2012-2014 by Ames Laboratory and Skew Matrix Software, LLC
 
 The MIT License (MIT)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 *************** <auto-copyright.rb END do not edit this line> ***************/

#ifndef __JAGMX_MX_GAME_PAD_DX_H__
#define __JAGMX_MX_GAME_PAD_DX_H__ 1


#include <jagMx/Export.h>
#include <jagMx/MxGamePad.h>
#include <jagBase/ptr.h>

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

typedef jagBase::ptr< jagMx::MxGamePadDX >::shared_ptr MxGamePadDXPtr;


/*@}*/


// jagMx
}


// __JAGMX_MX_GAME_PAD_DX_H__
#endif
