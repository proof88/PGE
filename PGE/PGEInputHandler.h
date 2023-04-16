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

#include "Config/PGEcfgProfiles.h"

/**
    PGE mouse handling class.
*/
class PGEInputMouse
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEInputMouse is included")   
#endif

public:
    enum MouseButton
    {
        MBTN_LEFT,
        MBTN_MIDDLE,
        MBTN_RIGHT
    };

    /** Creates and gets the singleton instance. */
    static PGEInputMouse& createAndGet(
        PGEcfgProfiles& cfgProfiles);

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&   getConsole() const;            /**< Returns access to console preset with logger module name as this class. */

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

    /**
        Mouse wheel interaction.
    */
    virtual short int getWheel() = 0;
    virtual void ReceiveWheel(short int amount) = 0;

    /**
        Mouse button interaction.
    */
    virtual bool isButtonPressed(MouseButton mbtn) const = 0;
    virtual void SetButtonPressed(MouseButton mbtn, bool pressed) = 0;

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
    static PGEInputKeyboard& createAndGet(
        PGEcfgProfiles& cfgProfiles);

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&   getConsole() const;                   /**< Returns access to console preset with logger module name as this class. */

    /**
    * @return True if given key is pressed, false otherwise.
    */
    virtual bool isKeyPressed(unsigned char key) const = 0;

    /**
    * Useful if we want to distinguish between different keypresses of the same key.
    * Unlike isKeyPressed(), this function also keeps track of the last released state of the key,
    * and will return true only if the state has just changed from released to pressed.
    * @return True if given key has just been changed to pressed state, false otherwise.
    */
    virtual bool isKeyPressedOnce(unsigned char key) = 0;
    
    /**
    * Sets the pressed state of the given key to the given value.
    * This is used by the message handler callback function of the game window.
    */
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
    static PGEInputHandler& createAndGet(PGEcfgProfiles& m_cfgProfiles);

    // ---------------------------------------------------------------------------

    CConsole&   getConsole() const;                   /**< Returns access to console preset with logger module name as this class. */
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

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