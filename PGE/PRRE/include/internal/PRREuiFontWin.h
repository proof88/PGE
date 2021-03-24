#pragma once

/*
    ###################################################################################
    PRREuiFontWin.h
    This file is part of PRRE.
    Internal header.
    PRRE User Interface Windows Font class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../external/PRREallHeaders.h"

/**
    PRRE User Interface Windows Font class.
*/
class PRREuiFontWin
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREuiFontWin is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PRREuiFontWin();

    PRREuiFontWin(
        const std::string& fontFaceName, int height, bool bold, bool italic, bool underline, bool strikeout,
        HDC wnd_dc );

    PRREuiFontWin(const PRREuiFontWin& uiFont);
    PRREuiFontWin& operator=(const PRREuiFontWin& uiFont);

    virtual ~PRREuiFontWin();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    const std::string& getFontFaceName() const;

    int  getHeight() const;
    bool getBold() const;
    bool getItalic() const;
    bool getUnderline() const;
    bool getStrikeOut() const;
    unsigned int getListBase() const;

    long getTextWidth(const std::string& text) const;
    long getTextHeight(const std::string& text) const;

protected:

    // ---------------------------------------------------------------------------

private:

    // ---------------------------------------------------------------------------

    HDC hDC;
    HFONT hFont;
    int nHeight;
    int nWeight;
    DWORD bItalic;
    DWORD bUnderline;
    DWORD bStrikeOut;
    std::string sFontFaceName;
    unsigned int base;

};
