#pragma once

/*
    ###################################################################################
    PureUiManager.h
    This file is part of PURE.
    External header.
    Pure User Interface Manager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureAllHeaders.h"
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

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    /**
        Sets members to real values within the singleton instance.
        Does nothing if wnd_dc is NULL.
    */
    virtual void Initialize(HDC wnd_dc) = 0;


    /**
        This deletes all UI elements and allocated resources, Initialize() can be called again later.
    */
    virtual void Deinitialize() = 0;


    /**
        Gets whether we are initialized.
    */
    virtual TPureBool isInitialized() const = 0;


    /**
        Adds permanent text to the UI.
        Permanent texts live until explicit deletion. This means that after you add a permanent text to the UI, it will be
        displayed in all next rendered frames.
    */
    virtual PureUiText* addText(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) = 0;


    /**
        Adds permanent text to the UI with default properties.
        Permanent texts live until explicit deletion. This means that after you add a permanent text to the UI, it will be
        displayed in all next rendered frames.
    */
    virtual PureUiText* addText(const std::string& txt, int x, int y) = 0;


    /**
        Deletes permanent text from the UI.
        Please note that this command is not needed to be used on temporary texts.
    */
    virtual void RemoveText(const std::string& text, int x, int y, int height) = 0;

    /**
        Adds temporary text to the UI.
        Temporary texts live until the next rendered frame. This means that in order to display the same temporary text for
        more than 1 frame, you have to issue this command before rendering every single frame. Although there isn't much
        performance issue with using temporary texts during multiple frames, adding permanent text could be a better choice.
    */
    virtual PureUiText* text(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) = 0;

    /**
        Adds temporary text to the UI.
        Temporary texts live until the next rendered frame. This means that in order to display the same temporary text for
        more than 1 frame, you have to issue this command before rendering every single frame. Although there isn't much
        performance issue with using temporary texts during multiple frames, adding permanent text could be a better choice.
    */
    virtual PureUiText* text(const std::string& txt, int x, int y) = 0;

    virtual const std::string& getDefaultFontFace() const = 0;

    virtual void SetDefaultFontFace(const std::string& face) = 0;

    virtual int  getDefaultFontSize() const = 0;
    virtual void SetDefaultFontSize(int size) = 0;

    virtual bool getDefaultFontBold() const = 0;
    virtual void SetDefaultFontBold(bool bold) = 0;

    virtual bool getDefaultFontItalic() const = 0;
    virtual void SetDefaultFontItalic(bool italic) = 0;

    virtual bool getDefaultFontUnderline() const = 0;
    virtual void SetDefaultFontUnderline(bool underline) = 0;

    virtual bool getDefaultFontStrikeout() const = 0;
    virtual void SetDefaultFontStrikeout(bool strikeout) = 0;


    /**
        Returns reference to default color.
        The default color is black by default.
    */
    virtual const PureColor& getDefaultColor() const = 0;
    virtual       PureColor& getDefaultColor() = 0;

    virtual void Render() = 0;

};
