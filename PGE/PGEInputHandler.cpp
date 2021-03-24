#pragma once

/*
    ###################################################################################
    PGEInputHandler.cpp
    This file is part of PGE.
    Input handler classes for PGE.
    Keyboard handling class, mouse handling class, and an encapsulating class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PGEInputHandler.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"
#include "../../CConsole/CConsole/src/CConsole.h"


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

    virtual ~PGEInputMouseImpl();

    CConsole&   getConsole() const;            /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

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

protected:

private:

    // ---------------------------------------------------------------------------

    RAWINPUTDEVICE Rid[1];
    int  mx, my;
    int  tempMovementX, tempMovementY;
    bool bPreciseMovementAvailable;
    bool bPreciseMovementActive;

    // ---------------------------------------------------------------------------

    PGEInputMouseImpl();

    PGEInputMouseImpl(const PGEInputMouseImpl&);
    PGEInputMouseImpl& operator=(const PGEInputMouseImpl&);

    friend class PGEInputMouse;

}; 


// ############################### PUBLIC ################################


PGEInputMouseImpl::~PGEInputMouseImpl()
{

}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEInputMouseImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEInputMouseImpl::getLoggerModuleName() const
{
    return "PGEInputMouse";
} // getLoggerModuleName()


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


void PGEInputMouseImpl::SetCursorPos(int x, int y)
{
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


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGEInputMouseImpl::PGEInputMouseImpl()
{
    bPreciseMovementActive = bPreciseMovementAvailable = false;
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    mx = my = tempMovementX = tempMovementY = 0;
}


PGEInputMouseImpl::PGEInputMouseImpl(const PGEInputMouseImpl&)
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
PGEInputMouse& PGEInputMouse::createAndGet()
{
    static PGEInputMouseImpl inMouseInstance;
    return inMouseInstance;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
CConsole& PGEInputMouse::getConsole() const
{
    return createAndGet().getConsole();
    #pragma warning(default:4717)
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
const char* PGEInputMouse::getLoggerModuleName() const
{
    return createAndGet().getLoggerModuleName();
    #pragma warning(default:4717)
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

    virtual ~PGEInputKeyboardImpl();

    CConsole&   getConsole() const;            /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    bool isKeyPressed(unsigned char key) const;
    void SetKeyPressed(unsigned char key, bool state);

protected:

private:

    // ---------------------------------------------------------------------------

    bool bKeys[256];

    // ---------------------------------------------------------------------------

    PGEInputKeyboardImpl();

    PGEInputKeyboardImpl(const PGEInputKeyboardImpl&);     
    PGEInputKeyboardImpl& operator=(const PGEInputKeyboardImpl&);
    
    
    friend class PGEInputKeyboard;

};


// ############################### PUBLIC ################################


PGEInputKeyboardImpl::~PGEInputKeyboardImpl()
{

}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEInputKeyboardImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEInputKeyboardImpl::getLoggerModuleName() const
{
    return "PGEInputKeyboard";
} // getLoggerModuleName()


bool PGEInputKeyboardImpl::isKeyPressed(unsigned char key) const
{
    return bKeys[key];
}

void PGEInputKeyboardImpl::SetKeyPressed(unsigned char key, bool state)
{
    bKeys[key] = state;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGEInputKeyboardImpl::PGEInputKeyboardImpl()
{
    for (int i = 0; i < 256; i++)
        bKeys[i] = false;    
}


PGEInputKeyboardImpl::PGEInputKeyboardImpl(const PGEInputKeyboardImpl&)
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
PGEInputKeyboard& PGEInputKeyboard::createAndGet()
{
    static PGEInputKeyboardImpl inKeyboardInstance;
    return inKeyboardInstance;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
CConsole& PGEInputKeyboard::getConsole() const
{
    return createAndGet().getConsole();
    #pragma warning(default:4717)
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
const char* PGEInputKeyboard::getLoggerModuleName() const
{
    return createAndGet().getLoggerModuleName();
    #pragma warning(default:4717)
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

    virtual ~PGEInputHandlerImpl();

    CConsole&   getConsole() const;            /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    bool initialize(HWND hWindow = PGENULL);

    PGEInputKeyboard& getKeyboard() const;
    PGEInputMouse& getMouse() const;

protected:

private:

    // ---------------------------------------------------------------------------

    PGEInputKeyboard& keyboard;
    PGEInputMouse&    mouse;

    // ---------------------------------------------------------------------------

    PGEInputHandlerImpl();

    PGEInputHandlerImpl(const PGEInputHandlerImpl&);   
    PGEInputHandlerImpl& operator=(const PGEInputHandlerImpl&);
    
   
    friend class PGEInputHandler;

};


// ############################### PUBLIC ################################


PGEInputHandlerImpl::~PGEInputHandlerImpl()
{

}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PGEInputHandlerImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGEInputHandlerImpl::getLoggerModuleName() const
{
    return "PGEInputHandler";
} // getLoggerModuleName()


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


PGEInputHandlerImpl::PGEInputHandlerImpl() :
    keyboard( PGEInputKeyboard::createAndGet() ),
    mouse( PGEInputMouse::createAndGet() )
{

};


PGEInputHandlerImpl::PGEInputHandlerImpl(const PGEInputHandlerImpl&) :
    keyboard( PGEInputKeyboard::createAndGet() ),
    mouse( PGEInputMouse::createAndGet() )
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
PGEInputHandler& PGEInputHandler::createAndGet()
{
    static PGEInputHandlerImpl inHndlrInstance;
    return inHndlrInstance;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
CConsole& PGEInputHandler::getConsole() const
{
    return createAndGet().getConsole();
    #pragma warning(default:4717)
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
// temporarily disabling the "recursive on all control paths" warning since createAndGet() will actually return the impl instance!
#pragma warning(disable:4717)
const char* PGEInputHandler::getLoggerModuleName() const
{
    return createAndGet().getLoggerModuleName();
    #pragma warning(default:4717)
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


