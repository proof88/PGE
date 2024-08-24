#pragma once

/*
    ###################################################################################
    PureUiManager.h
    This file is part of PURE.
    External header.
    Pure User Interface Manager class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureAllHeaders.h"

#include <functional>  // Cpp11

#include "Material/PureColor.h"
#include "../internal/PureUiText.h"

#define PURE_UI_MGR_FONT_DEFAULT_FACE "Arial"
#define PURE_UI_MGR_FONT_DEFAULT_SIZE 12
#define PURE_UI_MGR_FONT_DEFAULT_BOLD false
#define PURE_UI_MGR_FONT_DEFAULT_ITALIC false
#define PURE_UI_MGR_FONT_DEFAULT_UNDERLINE false
#define PURE_UI_MGR_FONT_DEFAULT_STRIKEOUT false

/**
    Pure User Interface Manager class.
*/
class PureUiManager
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureUiManager is included")
#endif

public:

    /**
        Creates and gets the singleton instance.
    */
    static PureUiManager& createAndGet();

    /**
        Returns the logger module name of this class.
        Intentionally not virtual, so derived class should hide this instead of overriding.
        Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

        @return The logger module name of this class.
    */
    static const char* getLoggerModuleName();

    // ---------------------------------------------------------------------------

    /**
        Returns access to console preset with logger module name as this class.
        Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

        @return Console instance used by this class.
    */
    CConsole&  getConsole() const;

    /**
        Sets members to real values within the singleton instance.
        Does nothing if wnd_handle or wnd_dc is NULL.

        Application doesn't need to call this, PURE initialization takes care of this.
    */
    virtual void Initialize(HWND wnd_handle, HDC wnd_dc) = 0;


    /**
        This deletes all UI elements and allocated resources, Initialize() can be called again later.

        Application doesn't need to call this, PURE shutdown takes care of this.
    */
    virtual void Deinitialize() = 0;


    /**
        Gets whether we are initialized.
    */
    virtual TPureBool isInitialized() const = 0;


    /**
        Returns the previously set GUI callback function.
    */
    virtual const std::function<void()>& getGuiDrawCallback() const = 0;

    /**
        Sets the given GUI draw callback function to be invoked by the renderer in every frame.
        This callback should invoke Dear ImGui (https://github.com/ocornut/imgui) functions to draw GUI.
        The caller is not responsible for compiling or initializing Dear ImGui, PURE is in charge for that.
        PURE is already built with Dear ImGui code.
        Only the platform-agnostic, actual GUI drawing functions should be invoked that are listed in imgui.h.
        This header file is in the external include directory of PURE.
        None of these should be called either:
         - ImGui::NewFrame();
         - ImGui::EndFrame();
         - ImGui::Render().

        For an example on how to use this function, see a real-life game project: https://github.com/proof88/PRooFPS-dd .

        Note: this function is invoked every frame, AFTER rendering the 2D sticked-to-screen objects (PureObject3D::SetStickedToScreen()).
        Thus, if you also do calculations in the specified callback function that have effect on those PURE sticked-to-screen objects, the
        visible effects will be delayed to the next frame. So in general it is highly recommended to do ONLY Dear ImGui-specific stuff in the
        specified callback function.
    */
    virtual void setGuiDrawCallback(const std::function<void()>& cb) = 0;


    /**
        Adds permanent text to the UI.
        Permanent texts live until explicit deletion. This means that after you add a permanent text to the UI, it will be
        displayed in all next rendered frames.

        This function is considered as legacy now because it uses an old technique to render text, however
        it is still being used here and there (including the original PR00FPS thru the gfxcore2 wrapper library) thus I'm
        not planning to delete it soon. It depends on the platform-specific PureUiFontWin class and I'm not planning to
        create something similar for Linux, so multiplatform support is not possible now with this legacy way.

        Another way of text rendering can be done using Dear ImGui text-rendering functions that you can invoke in your
        own function that you pass to PURE using setGuiDrawCallback().

        This function has nothing to do with Dear ImGui.
        You can add text using this function or with Dear ImGui API, they can be rendered on-screen at the same time too.
    */
    virtual PureUiText* textPermanentLegacy(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) = 0;


    /**
        Adds permanent text to the UI with default properties.
        Basically this has the same effect as calling the other textPermanentLegacy() with default font properties.
    */
    virtual PureUiText* textPermanentLegacy(const std::string& txt, int x, int y) = 0;


    /**
        Deletes permanent text from the UI that was previously added by textPermanentLegacy().
        This command is not needed to be used on temporary texts added by textTemporalLegacy().
    */
    virtual void removeTextPermanentLegacy(const std::string& text, int x, int y, int height) = 0;

    /**
        Deletes all permanent texts from the UI that were previously added by textPermanentLegacy().
        This command is not needed to be used on temporary texts added by textTemporalLegacy().
    */
    virtual void removeAllTextPermanentLegacy() = 0;

    /**
        Adds temporary text to the UI.
        Temporary texts live only in the next rendered frame. This means that in order to display the same temporary text for
        more than 1 frame, you have to issue this command before rendering every single frame.

        If you want to display the same text in multiple frame, from performance perspective it is better to add permanent text
        using textPermanentLegacy() instead.

        This function is considered as legacy now because it uses an old technique to render text, however
        it is still being used here and there (including the original PR00FPS thru the gfxcore2 wrapper library) thus I'm
        not planning to delete it soon. It depends on the platform-specific PureUiFontWin class and I'm not planning to
        create something similar for Linux, so multiplatform support is not possible now with this legacy way.

        Another way of text rendering can be done using Dear ImGui text-rendering functions that you can invoke in your
        own function that you pass to PURE using setGuiDrawCallback().

        This function has nothing to do with Dear ImGui.
        You can add text using this function or with Dear ImGui API, they can be rendered on-screen at the same time too.
    */
    virtual PureUiText* textTemporalLegacy(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) = 0;

    /**
        Adds temporary text to the UI.
        Basically this has the same effect as calling the other textTemporalLegacy() with default font properties.
    */
    virtual PureUiText* textTemporalLegacy(const std::string& txt, int x, int y) = 0;

    /**
        Gets the default font face used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual const std::string& getDefaultFontFaceLegacy() const = 0;

    /**
        Sets the default font face used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual void setDefaultFontFaceLegacy(const std::string& face) = 0;

    /**
        Gets the default font size used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual int  getDefaultFontSizeLegacy() const = 0;

    /**
        Sets the default font size used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual void setDefaultFontSizeLegacy(int size) = 0;

    /**
        Gets the default font bold property used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual bool getDefaultFontBoldLegacy() const = 0;

    /**
        Sets the default font bold property used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual void setDefaultFontBoldLegacy(bool bold) = 0;

    /**
        Gets the default font italic property used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual bool getDefaultFontItalicLegacy() const = 0;

    /**
        Sets the default font italic property used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual void setDefaultFontItalicLegacy(bool italic) = 0;

    /**
        Gets the default font underlined property used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual bool getDefaultFontUnderlineLegacy() const = 0;

    /**
        Sets the default font underlined property used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual void setDefaultFontUnderlineLegacy(bool underline) = 0;

    /**
        Gets the default font strikeout property used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual bool getDefaultFontStrikeoutLegacy() const = 0;

    /**
        Sets the default font strikeout property used by legacy functions textPermanentLegacy() and textTemporalLegacy().
    */
    virtual void setDefaultFontStrikeoutLegacy(bool strikeout) = 0;

    /**
        Returns reference to default color used by legacy functions textPermanentLegacy() and textTemporalLegacy()
        This default color is black by default.
    */
    virtual const PureColor& getDefaultColorLegacy() const = 0;

    /**
        Returns reference to default color used by legacy functions textPermanentLegacy() and textTemporalLegacy()
        This default color is black by default.
    */
    virtual       PureColor& getDefaultColorLegacy() = 0;

    virtual void render() = 0;

};
