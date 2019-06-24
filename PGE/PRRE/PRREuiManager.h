#pragma once

/*
    ###################################################################################
    PRREuiManager.h
    This file is part of PRRE.
    External header.
    PRRE User Interface Manager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREBaseClass.h"
#include "PRREColor.h"
#include "PRREuiText.h"


#define PRRE_UI_MGR_FONT_DEFAULT_FACE "Arial"
#define PRRE_UI_MGR_FONT_DEFAULT_SIZE 12
#define PRRE_UI_MGR_FONT_DEFAULT_BOLD false
#define PRRE_UI_MGR_FONT_DEFAULT_ITALIC false
#define PRRE_UI_MGR_FONT_DEFAULT_UNDERLINE false
#define PRRE_UI_MGR_FONT_DEFAULT_STRIKEOUT false

/**
    PRRE User Interface Manager class.

    todo: This class API usage?
*/
class PRREuiManager :
    public PRREBaseClass
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREuiManager is included")
#endif

public:

    /**
        Creates and gets the singleton instance.
    */
    static PRREuiManager& createAndGet();

    // ---------------------------------------------------------------------------

    /**
        Sets members to real values within the singleton instance.
        Does nothing if wnd_dc is NULL.
    */
    virtual void PRREuiManager::Initialize(HDC wnd_dc) = 0;


    /**
        This deletes all UI elements and allocated resources, Initialize() can be called again later.
    */
    virtual void Deinitialize() = 0;


    /**
        Gets whether we are initialized.
    */
    virtual TPRREbool isInitialized() const = 0;


    /**
        Adds permanent text to the UI.
        Permanent texts live until explicit deletion. This means that after you add a permanent text to the UI, it will be
        displayed in all next rendered frames.
    */
    virtual PRREuiText* addText(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) = 0;


    /**
        Adds permanent text to the UI with default properties.
        Permanent texts live until explicit deletion. This means that after you add a permanent text to the UI, it will be
        displayed in all next rendered frames.
    */
    virtual PRREuiText* addText(const std::string& txt, int x, int y) = 0;


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
    virtual PRREuiText* text(const std::string& txt, int x, int y, const std::string& fontface, int height, bool bold, bool italic, bool underline, bool strikeout) = 0;

    /**
        Adds temporary text to the UI.
        Temporary texts live until the next rendered frame. This means that in order to display the same temporary text for
        more than 1 frame, you have to issue this command before rendering every single frame. Although there isn't much
        performance issue with using temporary texts during multiple frames, adding permanent text could be a better choice.
    */
    virtual PRREuiText* text(const std::string& txt, int x, int y) = 0;

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
    virtual const PRREColor& getDefaultColor() const = 0;
    virtual       PRREColor& getDefaultColor() = 0;

    virtual void Render() = 0;

};
