#pragma once

/*
    ###################################################################################
    PRREuiFontWin.cpp
    This file is part of PRRE.
    PRRE User Interface Windows Font class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREuiFontWin.h"
#include "Hardware/PRREhwInfo.h"


// ############################### PUBLIC ################################


PRREuiFontWin::PRREuiFontWin() :
    nHeight(0),
    nWeight(0),
    bItalic(FALSE),
    bUnderline(FALSE),
    bStrikeOut(FALSE),
    hDC(NULL),
    hFont(NULL),
    base(0)
{

}


PRREuiFontWin::PRREuiFontWin(
    const std::string& fontFaceName, int height, bool bold, bool italic, bool underline, bool strikeout,
    HDC wnd_dc )
{
    sFontFaceName = fontFaceName;
    nHeight = height;
    nWeight = bold ? FW_BOLD : FW_NORMAL;
    bItalic = italic ? TRUE : FALSE;
    bUnderline = underline ? TRUE : FALSE;
    bStrikeOut = strikeout ? TRUE : FALSE;

    hDC = wnd_dc;

    hFont = CreateFont(
        nHeight, 0, 0, 0, nWeight, bItalic, bUnderline, bStrikeOut, EASTEUROPE_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, sFontFaceName.c_str() );

    if ( !hFont )
    {
        getConsole().EOLn(
            "PRREuiFontWin(%s, %d, %b, %b, %b, %b) FAILED to create font!",
            fontFaceName.c_str(), height, bold, italic, underline, strikeout );
        return;
    }
       

    if ( PRREhwInfo::get().getVideo().isAcceleratorDetected() ) {

        // Create space for 255 characters.
        base = glGenLists(255);

        if ( base == 0 )
        {
            getConsole().EOLn(
                "PRREuiFontWin(%s, %d, %b, %b, %b, %b) FAILED to allocate display lists!",
                fontFaceName.c_str(), height, bold, italic, underline, strikeout );
            // DeleteObject(hFont); should i release the font object this way?
            return;
        }

        SelectObject(hDC, hFont);
        // rumor is that wglUseFontBitmaps() fails if there is a gl error before ... so calling glGetError() before this solves that problem.
        // display lists created here will contain 1 call only: glBitmap().
        // also pls check what are the return values of this function!
        if ( !wglUseFontBitmaps(hDC, 0, 255, base) )
        {
            getConsole().EOLn(
                "PRREuiFontWin(%s, %d, %b, %b, %b, %b) FAILED to generate bitmaps!",
                fontFaceName.c_str(), height, bold, italic, underline, strikeout );
            return;
        }

    }

    getConsole().SOLn(
        "PRREuiFontWin(%s, %d, %b, %b, %b, %b) created font successfully, handle: %d.",
        fontFaceName.c_str(), height, bold, italic, underline, strikeout, hFont );
}


PRREuiFontWin::PRREuiFontWin(const PRREuiFontWin& uiFont) :
    hDC(uiFont.hDC),
    hFont(uiFont.hFont),
    nHeight(uiFont.nHeight),
    nWeight(uiFont.nWeight),
    bItalic(uiFont.bItalic),
    bUnderline(uiFont.bUnderline),
    bStrikeOut(uiFont.bStrikeOut),
    sFontFaceName(uiFont.sFontFaceName),
    base(uiFont.base)
{
    getConsole().OLn(
            "PRREuiFontWin(%s, %d, %d, %b, %b, %b) copy, handle: %d.",
            uiFont.sFontFaceName.c_str(), uiFont.nHeight, uiFont.nWeight, uiFont.bItalic, uiFont.bUnderline, uiFont.bStrikeOut, uiFont.hFont );
}


PRREuiFontWin& PRREuiFontWin::operator=(const PRREuiFontWin& uiFont)
{
    hDC = uiFont.hDC;
    hFont = uiFont.hFont;
    nHeight = uiFont.nHeight;
    nWeight = uiFont.nWeight;
    bItalic = uiFont.bItalic;
    bUnderline = uiFont.bUnderline;
    bStrikeOut = uiFont.bStrikeOut;
    sFontFaceName = uiFont.sFontFaceName;
    base = uiFont.base;

    getConsole().OLn(
            "PRREuiFontWin(%s, %d, %d, %b, %b, %b) assignment, handle: %d.",
            sFontFaceName.c_str(), uiFont.nHeight, uiFont.nWeight, uiFont.bItalic, uiFont.bUnderline, uiFont.bStrikeOut, uiFont.hFont );

    return *this;
}


PRREuiFontWin::~PRREuiFontWin()
{
    // TODO: display lists should be deleted also, right?
    if ( hFont )
    {
        // should i release the font object this way?
        if ( DeleteObject(hFont) )
        {
            getConsole().SOLn(
                "~PRREuiFontWin(%s, %d, %b, %b, %b, %b) deleted font successfully, handle: %d.",
                sFontFaceName.c_str(), nHeight, nWeight, bItalic, bUnderline, bStrikeOut, hFont );
            hFont = NULL;
        }
        else
            getConsole().EOLn(
                "~PRREuiFontWin(%s, %d, %b, %b, %b, %b) FAILED to delete font, handle: %d!",
                sFontFaceName.c_str(), nHeight, nWeight, bItalic, bUnderline, bStrikeOut, hFont );
            
    }
    else
        getConsole().OLn(
            "~PRREuiFontWin(%s, %d, %b, %b, %b, %b) no handle.",
            sFontFaceName.c_str(), nHeight, nWeight, bItalic, bUnderline, bStrikeOut );
}


const std::string& PRREuiFontWin::getFontFaceName() const
{
    return sFontFaceName;
}


int PRREuiFontWin::getHeight() const
{
    return nHeight;
}


bool PRREuiFontWin::getBold() const
{
    return (nWeight == FW_BOLD);
}


bool PRREuiFontWin::getItalic() const
{
    return (bItalic == 0) ? false : true;
}


bool PRREuiFontWin::getUnderline() const
{
    return (bUnderline == 0) ? false : true;
}


bool PRREuiFontWin::getStrikeOut() const
{
    return (bStrikeOut == 0) ? false : true;
}


unsigned int PRREuiFontWin::getListBase() const
{
    return base;
}


long PRREuiFontWin::getTextWidth(const std::string& text) const
{
    SIZE sizeText;
    SelectObject(hDC, hFont);
    if ( GetTextExtentPoint32(hDC, text.c_str(), text.length(), &sizeText) )
        return sizeText.cx;
    else
        return 0;
}

long PRREuiFontWin::getTextHeight(const std::string& text) const
{
    SIZE sizeText;
    SelectObject(hDC, hFont);
    if ( GetTextExtentPoint32(hDC, text.c_str(), text.length(), &sizeText) )
        return sizeText.cy;
    else
        return 0;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################