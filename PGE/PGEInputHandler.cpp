#pragma once

/*
    ###################################################################################
    PGEInputHandler.cpp
    This file is part of PGE.
    Input handler classes for PGE.
    Keyboard handling class, mouse handling class, and an encapsulating class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "PGEInputHandler.h"

#include <chrono>

#include "PGEincludes.h"
#include "PGEpragmas.h"
#include "../../Console/CConsole/src/CConsole.h"


// following 2 macros are for raw mouse input
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif


/*
   PGEInputMouseImpl
   ###########################################################################
*/


class PGEInputMouseImpl :
    public PGEInputMouse
{

public:

    // ---------------------------------------------------------------------------

    virtual ~PGEInputMouseImpl();

    bool initialize(HWND hWindow = NULL);

    bool isPreciseMovementAvailable() const;
    bool isPreciseMovementActive() const;
    void SetPreciseMovementActive(bool state);

    int getCursorPosX() const;
    int getCursorPosY() const;
    void SetCursorPos(int x, int y);

    void ReceiveAbsoluteInput(int x, int y);
    void ReceiveRelativeInput(int x, int y);

    void ApplyRelativeInput();

    short int getWheel();
    void ReceiveWheel(short int amount);

    bool isButtonPressed(MouseButton mbtn) const;
    void SetButtonPressed(MouseButton mbtn, bool pressed);

protected:

private:

    // ---------------------------------------------------------------------------

    PGEcfgProfiles& m_cfgProfiles;
    std::map<MouseButton, bool> mapButtonsPressed;
    RAWINPUTDEVICE Rid[1];
    int  mx, my;
    int  tempMovementX, tempMovementY;
    bool bPreciseMovementAvailable;
    bool bPreciseMovementActive;
    short int mWheel;

    // ---------------------------------------------------------------------------

    explicit PGEInputMouseImpl(PGEcfgProfiles& cfgProfiles);

    PGEInputMouseImpl(const PGEInputMouseImpl&);
    PGEInputMouseImpl& operator=(const PGEInputMouseImpl&);

    friend class PGEInputMouse;

}; 


// ############################### PUBLIC ################################


PGEInputMouseImpl::~PGEInputMouseImpl()
{

}


/**
    Initializes mouse handling.

    @param hWindow The window of which input should be handled.
                   Optional. If specified, the mouse movement can be handled more precisely.

    @return True if initialization successful, false otherwise.
            False initialization doesn't mean total failure. It just means that mouse movements
            cannot be handled in the more precise way.
*/
bool PGEInputMouseImpl::initialize(HWND hWindow)
{
    getConsole().OLn("PGEInputMouse::initialize()");

    mapButtonsPressed[MBTN_LEFT] = false;
    mapButtonsPressed[MBTN_MIDDLE] = false;
    mapButtonsPressed[MBTN_RIGHT] = false;

    if ( hWindow != PGENULL )
    {
        Rid[0].hwndTarget = hWindow;
        bPreciseMovementAvailable = (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == TRUE);
        if ( bPreciseMovementAvailable )
            getConsole().SOLn("  Raw input device registration okay!");
        else
            getConsole().EOLn("  Raw input device registration failed!");
        SetPreciseMovementActive(bPreciseMovementAvailable);
        return bPreciseMovementAvailable;
    }
    return true;
} // initialize()


bool PGEInputMouseImpl::isPreciseMovementAvailable() const
{
    return bPreciseMovementAvailable;
}


bool PGEInputMouseImpl::isPreciseMovementActive() const
{
    return bPreciseMovementActive;
}


void PGEInputMouseImpl::SetPreciseMovementActive(bool state)
{
    bPreciseMovementActive = state && bPreciseMovementAvailable;
}


int PGEInputMouseImpl::getCursorPosX() const
{
    return mx;
}


int PGEInputMouseImpl::getCursorPosY() const
{
    return my;
}


/**
    Invokes ApplyRelativeInput() first to null out pending input and sets cursor to given screen coordinates.

    @param x The target screen X-coordinate for the cursor to set.
    @param y The target screen Y-coordinate for the cursor to set.
*/
void PGEInputMouseImpl::SetCursorPos(int x, int y)
{
    ApplyRelativeInput();  // let the tempMovementX,Y variables be "emptied" in case of raw mouse input
    mx = x;
    my = y;
    ::SetCursorPos(x, y);
}


/**
    The cursor position will be updated by setting the given parameters.
*/
void PGEInputMouseImpl::ReceiveAbsoluteInput(int x, int y)
{
    if ( bPreciseMovementActive )
        return;

    mx = x;
    my = y;
}


/**
    The cursor position won't be updated, just adding the given parameters to temp vars.
*/
void PGEInputMouseImpl::ReceiveRelativeInput(int x, int y)
{
    if ( !bPreciseMovementActive )
        return;

    tempMovementX += x;
    tempMovementY += y;
}


/**
    Applies the previously received total relative input to mouse cursor position.
*/
void PGEInputMouseImpl::ApplyRelativeInput()
{
    if ( !bPreciseMovementActive )
        return;

    mx += tempMovementX;
    my += tempMovementY;
    tempMovementX = 0;
    tempMovementY = 0;
}


/**
    Returns the amount of mouse wheel rotation and zeros out this value.
    It is recommended to call this function in every iteration of your game loop.

    @return Amount of wheel rotation.
            A positive value indicates that the wheel was rotated forward, away from the user.
            A negative value indicates that the wheel was rotated backward, toward the user.
*/
short int PGEInputMouseImpl::getWheel()
{
    const short int nRetVal = mWheel / 120 /* WHEEL_DELTA from WinUser.h, see manual for WM_MOUSEWHEEL */;
    if (nRetVal != 0)
    {
        // If at least WHEEL_DELTA is not yet accumulated in mWheel, don't zero mWheel out yet!
        // This mechanism is needed so that notch-less mouses generating frequent messages with rotation amount
        // less than WHEEL_DELTA can accumulate their values up to +/- WHEEL_DELTA.
        mWheel %= 120;
    }
    return nRetVal;
}


/**
    Adds the given amount of rotation to the accumulated rotation.
    The accumulated rotation can be read and zeroed out with the getWheel() function.
    The accumulation is implemented to support notch-less mouses i.e. mouses having freely rotation wheels.
*/
void PGEInputMouseImpl::ReceiveWheel(short int amount)
{
    mWheel += amount;
}


bool PGEInputMouseImpl::isButtonPressed(MouseButton mbtn) const
{
    return mapButtonsPressed.at(mbtn);
}


void PGEInputMouseImpl::SetButtonPressed(MouseButton mbtn, bool pressed)
{
    mapButtonsPressed[mbtn] = pressed;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGEInputMouseImpl::PGEInputMouseImpl(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles)
{
    mapButtonsPressed[MBTN_LEFT] = false;
    mapButtonsPressed[MBTN_MIDDLE] = false;
    mapButtonsPressed[MBTN_RIGHT] = false;

    bPreciseMovementActive = bPreciseMovementAvailable = false;
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    mx = my = tempMovementX = tempMovementY = 0;
}


PGEInputMouseImpl::PGEInputMouseImpl(const PGEInputMouseImpl& other) :
    m_cfgProfiles(other.m_cfgProfiles)
{

} 


PGEInputMouseImpl& PGEInputMouseImpl::operator=(const PGEInputMouseImpl&)
{
    return *this;
}



/*
   PGEInputMouse
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PGEInputMouse& PGEInputMouse::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PGEInputMouseImpl inMouseInstance(cfgProfiles);
    return inMouseInstance;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEInputMouse::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEInputMouse::getLoggerModuleName()
{
    return "PGEInputMouse";
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################



/*
   PGEInputKeyboardImpl
   ###########################################################################
*/


class PGEInputKeyboardImpl :
    public PGEInputKeyboard
{

public:

    // ---------------------------------------------------------------------------

    virtual ~PGEInputKeyboardImpl();

    bool isKeyPressed(unsigned char key, std::chrono::milliseconds::rep nFilterMillisecs);
    bool isKeyPressedOnce(unsigned char key, std::chrono::milliseconds::rep nFilterMillisecs);
    void SetKeyPressed(unsigned char key, bool state);

protected:

private:

    // ---------------------------------------------------------------------------

    PGEcfgProfiles& m_cfgProfiles;
    bool m_bKeysDown[256];
    bool m_bKeysReleasedSinceLastRead[256];
    std::chrono::time_point<std::chrono::steady_clock> m_timeKeysDownAccepted[256];

    // ---------------------------------------------------------------------------

    explicit PGEInputKeyboardImpl(PGEcfgProfiles& cfgProfiles);

    PGEInputKeyboardImpl(const PGEInputKeyboardImpl&);     
    PGEInputKeyboardImpl& operator=(const PGEInputKeyboardImpl&);
    
    
    friend class PGEInputKeyboard;

};


// ############################### PUBLIC ################################


PGEInputKeyboardImpl::~PGEInputKeyboardImpl()
{

}


bool PGEInputKeyboardImpl::isKeyPressed(unsigned char key, std::chrono::milliseconds::rep nFilterMillisecs)
{
    const std::chrono::time_point<std::chrono::steady_clock> timeNow = std::chrono::steady_clock::now();
    const bool bTimeFilterOk =
        nFilterMillisecs > 0 ?
        (std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - m_timeKeysDownAccepted[key]).count() >= nFilterMillisecs) :
        true;

    const bool bPressAccepted = m_bKeysDown[key] && bTimeFilterOk;
    if (bPressAccepted)
    {
        // maybe I should also m_bKeysReleasedSinceLastRead[key] = false;
        m_timeKeysDownAccepted[key] = timeNow;
        // maybe I should return with true here
    }
    // maybe I should return with m_bKeysReleasedSinceLastRead[key] here
    return bPressAccepted;
}

bool PGEInputKeyboardImpl::isKeyPressedOnce(unsigned char key, std::chrono::milliseconds::rep nFilterMillisecs)
{
    const std::chrono::time_point<std::chrono::steady_clock> timeNow = std::chrono::steady_clock::now();
    const bool bTimeFilterOk =
        nFilterMillisecs > 0 ?
        (std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - m_timeKeysDownAccepted[key]).count() >= nFilterMillisecs) :
        true;

    const bool bPressedOnce = m_bKeysDown[key] && m_bKeysReleasedSinceLastRead[key] && bTimeFilterOk;
    if (bPressedOnce)
    {
        m_bKeysReleasedSinceLastRead[key] = false;
        m_timeKeysDownAccepted[key] = timeNow;
    }
    return bPressedOnce;
}

void PGEInputKeyboardImpl::SetKeyPressed(unsigned char key, bool state)
{
    m_bKeysDown[key] = state;
    if (!state)
    {
        m_bKeysReleasedSinceLastRead[key] = true;
    }
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGEInputKeyboardImpl::PGEInputKeyboardImpl(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles)
{
    // bKeysDown could be initialized in class definition this way too: bKeysDown = {false}, however
    // I cannot initialize bKeysReleasedSinceLastRead elements to true in similar way, so I keep
    // setting both here:
    for (int i = 0; i < 256; i++)
    {
        m_bKeysDown[i] = false;
        m_bKeysReleasedSinceLastRead[i] = true;
    }
}


PGEInputKeyboardImpl::PGEInputKeyboardImpl(const PGEInputKeyboardImpl& other) :
    m_cfgProfiles(other.m_cfgProfiles)
{

}         


PGEInputKeyboardImpl& PGEInputKeyboardImpl::operator=(const PGEInputKeyboardImpl&)
{
    return *this;
}



/*
   PGEInputKeyboard
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PGEInputKeyboard& PGEInputKeyboard::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PGEInputKeyboardImpl inKeyboardInstance(cfgProfiles);
    return inKeyboardInstance;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEInputKeyboard::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEInputKeyboard::getLoggerModuleName()
{
    return "PGEInputKeyboard";
} // getLoggerModuleName()



// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################






/*
   PGEInputHandlerImpl
   ###########################################################################
*/


class PGEInputHandlerImpl :
    public PGEInputHandler
{

public:

    // ---------------------------------------------------------------------------

    virtual ~PGEInputHandlerImpl();

    bool initialize(HWND hWindow = PGENULL);

    PGEInputKeyboard& getKeyboard() const;
    PGEInputMouse& getMouse() const;

protected:

private:

    // ---------------------------------------------------------------------------

    PGEcfgProfiles& m_cfgProfiles;
    PGEInputKeyboard& keyboard;
    PGEInputMouse&    mouse;

    // ---------------------------------------------------------------------------

    explicit PGEInputHandlerImpl(PGEcfgProfiles& cfgProfiles);

    PGEInputHandlerImpl(const PGEInputHandlerImpl&);   
    PGEInputHandlerImpl& operator=(const PGEInputHandlerImpl&);
    
   
    friend class PGEInputHandler;

};


// ############################### PUBLIC ################################


PGEInputHandlerImpl::~PGEInputHandlerImpl()
{

}


/**
    Initializes input handling.

    @param hWindow The window of which input should be handled. Optional.

    @return True if initialization successful, false otherwise.
            False initialization doesn't mean total failure.
            See PGEInputMouse and PGEInputKeyboard for details.
*/
bool PGEInputHandlerImpl::initialize(HWND hWindow)
{
    getConsole().OLnOI("PGEInputHandler::initialize()");
    bool b = mouse.initialize(hWindow);
    getConsole().OLnOO("");
    return b;
} // initialize()


PGEInputKeyboard& PGEInputHandlerImpl::getKeyboard() const
{
    return keyboard;
}

PGEInputMouse& PGEInputHandlerImpl::getMouse() const
{
    return mouse;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGEInputHandlerImpl::PGEInputHandlerImpl(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    keyboard( PGEInputKeyboard::createAndGet(cfgProfiles) ),
    mouse( PGEInputMouse::createAndGet(cfgProfiles) )
{

};


PGEInputHandlerImpl::PGEInputHandlerImpl(const PGEInputHandlerImpl& other) :
    m_cfgProfiles(other.m_cfgProfiles),
    keyboard( PGEInputKeyboard::createAndGet(other.m_cfgProfiles) ),
    mouse( PGEInputMouse::createAndGet(other.m_cfgProfiles) )
{};     


PGEInputHandlerImpl& PGEInputHandlerImpl::operator=(const PGEInputHandlerImpl&)
{
    return *this;
}



/*
   PGEInputHandler
   ###########################################################################
*/


// ############################### PUBLIC ################################


/** Creates and gets the singleton instance. */
PGEInputHandler& PGEInputHandler::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PGEInputHandlerImpl inHndlrInstance(cfgProfiles);
    return inHndlrInstance;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEInputHandler::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEInputHandler::getLoggerModuleName()
{
    return "PGEInputHandler";
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


