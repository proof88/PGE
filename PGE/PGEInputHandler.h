#pragma once

/*
    ###################################################################################
    PGEInputHandler.h
    This file is part of PGE.
    External header.
    Input handler classes for PGE.
    Keyboard handling class, mouse handling class, and an encapsulating class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"



/**
    PGE mouse handling class.
*/
class PGEInputMouse
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEInputMouse is included")   
#endif

public:
    /** Creates and gets the singleton instance. */
    static PGEInputMouse& createAndGet();

    // ---------------------------------------------------------------------------

    /**
        Initializes mouse handling.

        @param hWindow The window of which input should be handled.
                       Optional. If specified, the mouse movement can be handled more precisely.

        @return True if initialization successful, false otherwise.
                False initialization doesn't mean total failure. It just means that mouse movements
                cannot be handled in the more precise way.
    */
    virtual bool initialize(HWND hWindow = NULL) = 0;
     
    virtual bool isPreciseMovementAvailable() const = 0;
    virtual bool isPreciseMovementActive() const = 0;
    virtual void SetPreciseMovementActive(bool state) = 0;
     
    virtual int  getCursorPosX() const = 0;
    virtual int  getCursorPosY() const = 0;
    virtual void SetCursorPos(int x, int y) = 0;
     
    /**
        The cursor position will be updated by setting the given parameters.
    */
    virtual void ReceiveAbsoluteInput(int x, int y) = 0;
     
    /**
        The cursor position won't be updated, just adding the given parameters to temp vars.
    */
    virtual void ReceiveRelativeInput(int x, int y) = 0;
     
    /**
        Applies the previously received total relative input to mouse cursor position.
    */
    virtual void ApplyRelativeInput() = 0;

}; // class PGEInputMouse



/**
    PGE keyboard handling class.
*/
class PGEInputKeyboard
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEInputKeyboard is included")
#endif

public:
    /** Creates and gets the singleton instance. */
    static PGEInputKeyboard& createAndGet();

    // ---------------------------------------------------------------------------

    virtual bool isKeyPressed(unsigned char key) const = 0;
    virtual void SetKeyPressed(unsigned char key, bool state) = 0;

}; // class PGEInputKeyboard



/**
    PGE input handler class.
*/
class PGEInputHandler
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEInputHandler is included")
#endif

public:
    /** Creates and gets the singleton instance. */
    static PGEInputHandler& createAndGet();

    // ---------------------------------------------------------------------------

    /**
        Initializes input handling.

        @param hWindow The window of which input should be handled. Optional.

        @return True if initialization successful, false otherwise.
                False initialization doesn't mean total failure.
                See PGEInputMouse and PGEInputKeyboard for details.
    */
    virtual bool initialize(HWND hWindow = PGENULL) = 0;

    virtual PGEInputKeyboard& getKeyboard() const = 0;
    virtual PGEInputMouse& getMouse() const = 0;

}; // class PGEInputHandler