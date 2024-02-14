#pragma once

/*
    ###################################################################################
    PureScreen.h
    This file is part of PURE.
    External header.
    Screen handler class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"

// todo: see comment for this in PureWindow.h
#include "../../../../../../PFL/PFL/winproof88.h"
#include <WinUser.h>
#include <windowsx.h>


/**
    What pixelformat to be set.
*/
enum TPURE_SCREEN_PF
{
    PURE_SCREEN_PF_SIMPLE,      /**< Simple pixelformat for usual HW-accelerated rendering. */
    PURE_SCREEN_PF_ADVANCED,    /**< Advanced HW-accelerated pixelformat with e.g. MSAA support. */
    PURE_SCREEN_PF_NONE         /**< No need to set pixelformat, i.e. software rendering is used. */
}; // TPURE_SCREEN_PF


/**
    Screen handler class.
    Screen resolution, FSAA, etc ...
*/
class PureScreen
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureScreen is included")
#endif

public:

    static constexpr char* CVAR_GFX_VSYNC = "gfx_vsync";

    /**
        Creates and gets the singleton instance.
    */
    static PureScreen& createAndGet();

    static const char* getLoggerModuleName();         /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    /**
        Applies the set settings and the appropriate pixel format for the given HDC.
        The following functions must be called before this: 
         - SetResolution()
         - SetColorBits()
         - SetDepthBits()
        The result of not calling any of the mentioned functions is the function has no effect.
        Changes screen resolution, color depth and refresh rate ONLY IF fullscreen mode is requested.
        If fullscreen mode is requested without calling SetFreq() with an available value, like 60, the greatest 
        available refresh rate will be applied.

        Requesting an FSAA mode is complex a little bit: first call this method with pixelFormat = simple,
        then after initializing OpenGL, call this method again so an advanced pixel format will be selected.
        Then shutdown OpenGL, and close the previously created window, create a new window and call this
        method with pixelFormat = advanced. This time an FSAA-ready pixel format will be set so you can
        reinitialize OpenGL and enable FSAA in it.

        @return True if succeeds, false on error.
    */
    virtual TPureBool applyDisplaySettings(HDC dc, TPURE_SCREEN_PF pixelFormat = PURE_SCREEN_PF_SIMPLE) = 0;


    /**
        Resets the original display settings.
        If no settings have been applied yet or not in fullscreen mode, the function has no effect.
    */
    virtual void ResetDisplaySettings() = 0;


    /**
        Gets whether display settings are applied.
    */
    virtual TPureBool isInitialized() const = 0;


    /**
        Gets the horizontal display resolution.
        Should be used only after a successful applyDisplaySettings().

        @return The horizontal display resolution in pixels.
    */
    virtual TPureUInt getResWidth() const = 0;


    /**
        Gets the vertical display resolution.
        Should be used only after a successful applyDisplaySettings().

        @return The vertical display resolution in pixels.
    */
    virtual TPureUInt getResHeight() const = 0;


    /**
        Stores the given display resolution.
        Can be used only before applyDisplaySettings(), no effect otherwise.
        If w or h is 0, the target width and height will be the current screen resolution.
        If the current screen resolution can't be queried, the target will be 800x600.
        A call to applyDisplaySettings() is needed to apply the stored display resolution.
    */
    virtual void SetResolution(TPureUInt w, TPureUInt h) = 0;


    /**
        Gets whether we wanted fullscreen or not.
        @return True, if we wanted fullscreen mode, otherwise false.
    */
    virtual TPureBool isFullScreened() const = 0;


    /**
        Stores whether we want fullscreen mode or not.
        Can be used only before applyDisplaySettings(), no effect otherwise.
        A call to applyDisplaySettings() is needed to apply the setting.
    */
    virtual void SetFullScreened(TPureBool fs) = 0;


    /**
        Gets the stored display refresh rate.

        @return 0 or the stored value, if called before a successful applyDisplaySettings(), otherwise
                the finally selected refreshrate after a successful applyDisplaySettings().
    */
    virtual TPureUInt getFreq() const = 0;


    /**
        Stores the needed display refresh rate.
        Can be used only before applyDisplaySettings(), no effect otherwise.
        The stored display refresh rate will be applied only in fullscreen mode by a successful applyDisplaySettings().
    */
    virtual void SetFreq(TPureUInt f) = 0;


    /**
        Gets the stored display color depth.
        @return 0 before a call to applyDisplaySettings() or SetColorBits().
                The stored value after SetColorBits() and before applyDisplaySettings().
                The finally selected value after a successful applyDisplaySettings().
    */
    virtual TPureInt getColorBits() const = 0;


    /**
        Stores the needed display color depth.
        Can be used only before applyDisplaySettings(), no effect otherwise.
        A call to applyDisplaySettings() is needed to apply the setting.
    */
    virtual void SetColorBits(TPureInt c) = 0;


    /**
        Gets the needed Z-buffer depth.

        @return 0 before a call to applyDisplaySettings() or SetDepthBits().
                The stored value after SetColorBits() and before applyDisplaySettings().
                The finally selected value after a successful applyDisplaySettings(). This value maybe lower than the needed value because
                the engine automatically tries to apply a lower value if the needed value is not supported by the driver.
    */
    virtual TPureInt getDepthBits() const = 0;


    /**
        Stores the needed Z-Buffer depth.
        Can be used only before applyDisplaySettings(), no effect otherwise.
        A call to applyDisplaySettings() is needed to apply the setting.
    */
    virtual void SetDepthBits(TPureInt d) = 0;


    /**
        Gets the stored Stencil Buffer depth.
                                      
        @return 0 before a call to applyDisplaySettings() or SetStencilBits().
                The stored value after SetStencilBits() and before applyDisplaySettings().
                The finally selected value after a successful applyDisplaySettings().
    */
    virtual TPureInt getStencilBits() const = 0;


    /**
        Stores the needed Stencil Buffer depth.
        Can be used only before applyDisplaySettings(), no effect otherwise.
        A call to applyDisplaySettings() is needed to apply the setting.
    */
    virtual void SetStencilBits(TPureInt s) = 0;


    /**
        Gets whether the screensaver is allowed or not while the engine is running.
        Disabled by default.

        @return True if screensaver is allowed, false otherwise. 
    */
    virtual TPureBool isScreensaverEnabled() const = 0;


    /**
        Sets whether the screensaver is allowed or not while the engine is running.
        Disabled by default.
    */
    virtual void SetScreensaverEnabled(TPureBool state) = 0;


    /**
        Gets whether monitor power saving is allowed or not while the engine is running.
        Enabled by default.

        @return True if monitor power saving is allowed, false otherwise.
    */
    virtual TPureBool isMonitorPowersaveEnabled() const = 0;


    /**
        Sets whether monitor power saving is allowed or not while the engine is running.
        Enabled by default.
    */
    virtual void SetMonitorPowersaveEnabled(TPureBool state) = 0;


    /**
        Gets whether computer standy is allowed or not while the engine is running.
        Disabled by default.

        @return True if computer standy is allowed, false otherwise.
    */
    virtual TPureBool isStandbyEnabled() const = 0;


    /**
        Sets whether computer standy is allowed or not while the engine is running.
        Disabled by default.
    */
    virtual void SetStandbyEnabled(TPureBool state) = 0;


    /**
        Gets whether the engine is ready to use FSAA.
        @return True if FSAA can be used, false otherwise. 
    */
    virtual TPureBool isFSAAready() const = 0;


    /**
        Gets the actual FSAA-level. Default value is 0.
        @return The actual FSAA-value, 0 means no FSAA.
    */
    virtual TPureInt getFSAAlevel() const = 0;


    /**
        Sets the needed FSAA-level. Default value is 0.
        Can be used only before applyDisplaySettings(), no effect otherwise.
        A call to applyDisplaySettings() is needed to apply the setting.
    */
    virtual void SetFSAAlevel(TPureInt level) = 0;


    /**
        Gets whether VSync is enabled or not.
        Disabled by default.

        @return True if VSync is currently enabled, otherwise false.
    */
    virtual TPureBool isVSyncEnabled() const = 0;


    /**
        Sets the state of VSync.
        Default state is false.
        No effect if VSync is not supported.

        @return The new state. This implies that if requested enabled but a failure happened, false is returned.
    */
    virtual TPureBool setVSyncEnabled(TPureBool state) = 0;


}; // class PureScreen
