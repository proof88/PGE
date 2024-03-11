#pragma once

/*
    ###################################################################################
    PureUiText.h
    This file is part of PURE.
    Internal header.
    Pure User Interface Text class.
    Made by PR00F88
    ###################################################################################
*/

#include "../external/PureAllHeaders.h"
#include "PureUiFontWin.h"
#include "../external/Material/PureColor.h"


/**
    Pure User Interface Text class.
*/
class PureUiText
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureUiText is included")
#endif

public:

    static unsigned long getHash(const std::string& text, int x, int y, int height);

    // ---------------------------------------------------------------------------

 
    PureUiText();
    PureUiText(std::string text, int x, int y, PureUiFontWin& font);

    PureUiText(const PureUiText& uiText);
    PureUiText& operator=(const PureUiText& uiText);

    virtual ~PureUiText();

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

    const PureUiFontWin* getFont() const;

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
    PureUiFontWin* uiFont;  // should be ptr, since uiFont instances should be unique managed by uimanager, this should point to there
    PureColor clr;

    // ---------------------------------------------------------------------------

    static unsigned long calcHash(const std::string& text, int x, int y, int height);

};
