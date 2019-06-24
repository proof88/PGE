#pragma once

/*
    ###################################################################################
    PRREuiText.h
    This file is part of PRRE.
    Internal header.
    PRRE User Interface Text class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREuiFontWin.h"
#include "PRREColor.h"


/**
    PRRE User Interface Text class.

    This class API usage?
*/
class PRREuiText
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREuiText is included")
#endif

public:

    static unsigned long getHash(const std::string& text, int x, int y, int height);

    // ---------------------------------------------------------------------------

 
    PRREuiText();
    PRREuiText(std::string text, int x, int y, PRREuiFontWin& font);

    PRREuiText(const PRREuiText& uiText);
    PRREuiText& PRREuiText::operator=(const PRREuiText& uiText);

    virtual ~PRREuiText();

    const std::string& getText() const;

    int  getX() const;
    void SetX(int newx);
    int  getY() const;
    void SetY(int newy);

    long getWidth() const;
    long getHeight() const;

    bool getDelete() const;
    void SetDelete(bool value);

    bool getPermanent() const;
    void SetPermanent(bool value);

    const PRREuiFontWin* getFont() const;

    const PRREColor& getColor() const;
          PRREColor& getColor();

    void PrintText() const;

protected:

    // ---------------------------------------------------------------------------

private:

    // ---------------------------------------------------------------------------

    std::string sText;
    int _x, _y;
    // unsigned long nHash;
    bool bDelete;
    bool bPermanent;
    PRREuiFontWin* uiFont;  // should be ptr, since uiFont instances should be unique managed by uimanager, this should point to there
    PRREColor clr;

    // ---------------------------------------------------------------------------

    static unsigned long calcHash(const std::string& text, int x, int y, int height);

};
