#pragma once

/*
    ###################################################################################
    PureWindow.h
    This file is part of PURE.
    External header.
    Pure window.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include <string>

// todo:
// for HWND, HDC and WNDPROC ...
// later this class should be a pure virtual class and singleton
// and its implementation should be selected based on target OS
// so at least 2 implementations will exist: Win and Linux
// then Win-specific things will be moved to Win-implementation
// the Pure main class will pass this class to Screen and any other class who needs values like HDC
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>


/**
    Our main window, this is where we render to.
    Only have one window, so it is singleton.
*/
class PureWindow
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureWindow is included")
#endif

public:

    static PureWindow& createAndGet();               /**< Creates and gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual TPurebool initialize(
        TPureuint width, TPureuint height,
        TPurebool bFullscreen,
        const char* caption,
        HWND window = NULL) = 0;                                 /**< This creates the window actually. */
    virtual void      Deinitialize() = 0;                        /**< This deletes the window, then initialize() can be called again later. */
    virtual TPurebool isInitialized() const = 0;                 /**< Gets whether the window is initialized. */
    virtual void      ResetDefaults() = 0;                       /**< Resets the default settings. */
    virtual TPureuint getWidth() const = 0;                      /**< Gets window width. */
    virtual void      SetWidth(TPureuint newwidth) = 0;          /**< Sets window width. */
    virtual TPureuint getClientWidth() const = 0;                /**< Gets window client width. */
    virtual void      SetClientWidth(TPureuint newwidth) = 0;    /**< Sets window client width. */
    virtual TPureuint getHeight() const = 0;                     /**< Gets window height. */
    virtual void      SetHeight(TPureuint newheight) = 0;        /**< Sets window height. */
    virtual TPureuint getClientHeight() const = 0;               /**< Gets window client height. */
    virtual void      SetClientHeight(TPureuint newheight) = 0;  /**< Sets window client height. */
    virtual TPureuint getSideFrameWidth() const = 0;             /**< Gets window side frame width. */
    virtual TPureuint getBottomFrameHeight() const = 0;          /**< Gets window bottom frame height. */
    virtual TPureuint getCaptionHeight() const = 0;              /**< Gets window caption area height. */
    virtual TPureuint getTitleBarHeight() const = 0;             /**< Gets window title bar height. */
    
    virtual TPureuint getX() const = 0;             /**< Gets window X-coordinate. */
    virtual void      SetX(TPureuint newx) = 0;     /**< Sets window X-coordinate. */
    virtual TPureuint getY() const = 0;             /**< Gets window Y-coordinate. */
    virtual void      SetY(TPureuint newy) = 0;     /**< Sets window Y-coordinate. */
    
    virtual HWND      getWndHandle() const = 0;     /**< Gets window handle. */
    virtual HDC       getWndDC() const = 0;         /**< Gets window Device Context. */
    
    virtual TPurebool isActive() const = 0;             /**< Gets whether window is active or not. */
    virtual void      SetActive() = 0;                  /**< Sets the window active. */
    virtual TPurebool isTopMost() const = 0;            /**< Gets whether window is topmost or not. */
    virtual void      SetTopMost(TPurebool state) = 0;  /**< Sets the window topmost property. */
 
    virtual const std::string& getCaption() const = 0;                          /**< Gets the window caption. */
    virtual void               SetCaption(const std::string& newcaption) = 0;   /**< Sets the window caption. */
     
    virtual TPurebool isResizableH() const = 0;            /**< Gets whether window is resizable horizontally. */
    virtual void      SetResizableH(TPurebool state) = 0;  /**< Sets whether window is resizable horizontally. */
    virtual TPurebool isResizableV() const = 0;            /**< Gets whether window is resizable vertically. */
    virtual void      SetResizableV(TPurebool state) = 0;  /**< Sets whether window is resizable vertically. */
    virtual TPurebool isResizable() const = 0;             /**< Gets whether window is fully resizable. */
    virtual void      SetResizable(TPurebool state) = 0;   /**< Sets whether window is fully resizable. */
     
    virtual TPurebool hasBorder() const = 0;              /**< Gets whether window has border or not. */
    virtual void      SetBorder(TPurebool state) = 0;     /**< Sets whether window has border or not. */
    virtual TPurebool hasSysMenu() const = 0;             /**< Gets whether window has sysmenu or not. */
    virtual void      SetSysMenu(TPurebool state) = 0;    /**< Sets whether window has sysmenu or not. */
    virtual TPurebool hasMinButton() const = 0;           /**< Gets whether window has minimize button or not. */
    virtual void      SetMinButton(TPurebool state) = 0;  /**< Sets whether window has minimize button or not. */
    virtual TPurebool hasMaxButton() const = 0;           /**< Gets whether window has maximize/restore button or not. */
    virtual void      SetMaxButton(TPurebool state) = 0;  /**< Sets whether window has maximize button or not. */
     
    virtual TPurebool isEnabled() const = 0;            /**< Gets whether window is enabled or not. */
    virtual void      SetEnabled(TPurebool state) = 0;  /**< Sets whether window is enabled or not. */
     
    virtual WNDPROC   getWindowProc() const = 0;                    /**< Gets the message handling window procedure of this window. */
    virtual void      SetWindowProc(const WNDPROC windowproc) = 0;  /**< Sets the message handling window procedure of this window. */
    virtual void      ProcessMessages() = 0;                        /**< Processes the queued window messages. */
     
    virtual TPurebool hasCloseRequest() const = 0;                       /**< Gets whether the window has a close request or not. */
    virtual TPurebool isAutoRedrawAllOnPosChange() const = 0;            /**< Gets whether automatic redraw of window contents on position change is enabled or not. */
    virtual void      SetAutoRedrawAllOnPosChange(TPurebool state) = 0;  /**< Sets whether automatic redraw of window contents on position change is enabled or not. */
    virtual TPurebool isAutoCleanupOnQuitOn() const = 0;                 /**< Gets whether automatic cleanup of window on close request is enabled or not. */
    virtual void      SetAutoCleanupOnQuitOn(TPurebool state) = 0;       /**< Sets whether automatic cleanup of window on close request is enabled or not. */
    virtual TPurebool getAutoCameraWarningState() const = 0;             /**< Gets whether automatic camera warning on resize is enabled or not. */
    virtual void      SetAutoCameraWarningState(TPurebool state) = 0;    /**< Sets whether automatic camera warning on resize is enabled or not. */
 
    virtual void      Show() = 0;        /**< Shows the window. */
    virtual void      ShowFull() = 0;    /**< Shows and brings the window to front and sets the focus to it. */
    virtual void      Hide() = 0;        /**< Hides the window. */
    virtual void      Minimize() = 0;    /**< Minimizes the window, so it will be visible only on the Taskbar. */
    virtual void      Maximize() = 0;    /**< Maximizes the window. */
    virtual void      Restore() = 0;     /**< Restores the window to the previous state. */
    virtual void      Close() = 0;       /**< Closes the window. */
  
    virtual TPurebool isCursorVisible() const = 0;            /**< Gets whether the mouse cursor is visible or not within the window area. */
    virtual void      SetCursorVisible(TPurebool state) = 0;  /**< Sets whether the mouse cursor is visible or not within the window area. */
   
    virtual void WriteSettings() = 0;                      /**< Writes the window properties to the console window. */ 

}; // class PureWindow
