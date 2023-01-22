#pragma once

/*
    ###################################################################################
    PureuiText.h
    This file is part of PURE.
    Internal header.
    Pure User Interface Text class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../external/PureAllHeaders.h"
#include "PureuiFontWin.h"
#include "../external/Material/PureColor.h"


/**
    Pure User Interface Text class.
*/
class PureuiText
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureuiText is included")
#endif

public:

    static unsigned long getHash(const std::string& text, int x, int y, int height);

    // ---------------------------------------------------------------------------

 
    PureuiText();
    PureuiText(std::string text, int x, int y, PureuiFontWin& font);

    PureuiText(const PureuiText& uiText);
    PureuiText& operator=(const PureuiText& uiText);

    virtual ~PureuiText();

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

    bool getDropShadow() const;
    void SetDropShadow(bool value);

    const PureuiFontWin* getFont() const;

    const PureColor& getColor() const;
          PureColor& getColor();

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
    bool bDropShadow;
    PureuiFontWin* uiFont;  // should be ptr, since uiFont instances should be unique managed by uimanager, this should point to there
    PureColor clr;

    // ---------------------------------------------------------------------------

    static unsigned long calcHash(const std::string& text, int x, int y, int height);

};
