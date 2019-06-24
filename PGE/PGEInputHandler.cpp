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
#include "PGEpragmas.h"
#include "PGEInputHandler.h"
#include "PGEincludes.h"
#include "../CConsole/src/CConsole.h"


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

    CConsole& con;
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
    Initializes mouse handling.

    @param hWindow The window of which input should be handled.
                   Optional. If specified, the mouse movement can be handled more precisely.

    @return True if initialization successful, false otherwise.
            False initialization doesn't mean total failure. It just means that mouse movements
            cannot be handled in the more precise way.
*/
bool PGEInputMouseImpl::initialize(HWND hWindow)
{
    con.OLn("PGEInputMouse::initialize()");
    if ( hWindow != PGENULL )
    {
        Rid[0].hwndTarget = hWindow;
        if ( bPreciseMovementAvailable = (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == TRUE) )
            con.SOLn("  Raw input device registration okay!");
        else
            con.EOLn("  Raw input device registration failed!");
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


PGEInputMouseImpl::PGEInputMouseImpl() :
    con( CConsole::getConsoleInstance() )
{
    bPreciseMovementActive = bPreciseMovementAvailable = false;
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    mx = my = tempMovementX = tempMovementY = 0;
}


PGEInputMouseImpl::PGEInputMouseImpl(const PGEInputMouseImpl&) :
    con( CConsole::getConsoleInstance() )
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

    bool isKeyPressed(unsigned char key) const;
    void SetKeyPressed(unsigned char key, bool state);

protected:

private:

    // ---------------------------------------------------------------------------

    CConsole& con;
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


PGEInputKeyboardImpl::PGEInputKeyboardImpl() :
    con( CConsole::getConsoleInstance() )
{
    for (int i = 0; i < 256; i++)
        bKeys[i] = false;    
}


PGEInputKeyboardImpl::PGEInputKeyboardImpl(const PGEInputKeyboardImpl&) :
    con( CConsole::getConsoleInstance() )
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

    bool initialize(HWND hWindow = PGENULL);

    PGEInputKeyboard& getKeyboard() const;
    PGEInputMouse& getMouse() const;

protected:

private:

    // ---------------------------------------------------------------------------

    CConsole&         con;
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
    Initializes input handling.

    @param hWindow The window of which input should be handled. Optional.

    @return True if initialization successful, false otherwise.
            False initialization doesn't mean total failure.
            See PGEInputMouse and PGEInputKeyboard for details.
*/
bool PGEInputHandlerImpl::initialize(HWND hWindow)
{
    con.OLnOI("PGEInputHandler::initialize()");
    bool b = mouse.initialize(hWindow);
    con.OLnOO("");
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
    con( CConsole::getConsoleInstance() ),
    keyboard( PGEInputKeyboard::createAndGet() ),
    mouse( PGEInputMouse::createAndGet() )
{

};


PGEInputHandlerImpl::PGEInputHandlerImpl(const PGEInputHandlerImpl&) :
    con( CConsole::getConsoleInstance() ),
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


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


