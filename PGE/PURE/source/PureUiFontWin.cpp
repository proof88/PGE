#pragma once

/*
    ###################################################################################
    PureUiFontWin.cpp
    This file is part of Pure.
    Pure User Interface Windows Font class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../include/internal/PureUiFontWin.h"
#include "../include/internal/Purepragmas.h"
#include "../include/external/Hardware/PureHwInfo.h"


// ############################### PUBLIC ################################


PureUiFontWin::PureUiFontWin() :
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


PureUiFontWin::PureUiFontWin(
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
            "PureUiFontWin(%s, %d, %b, %b, %b, %b) FAILED to create font!",
            fontFaceName.c_str(), height, bold, italic, underline, strikeout );
        // exception should be thrown but ui system will be replaced anyway ...
        return;
    }
       

    if ( PureHwInfo::get().getVideo().isAcceleratorDetected() ) {

        // Create space for 255 characters.
        base = glGenLists(255);

        if ( base == 0 )
        {
            getConsole().EOLn(
                "PureUiFontWin(%s, %d, %b, %b, %b, %b) FAILED to allocate display lists!",
                fontFaceName.c_str(), height, bold, italic, underline, strikeout );
            // DeleteObject(hFont); should i release the font object this way?
            // exception should be thrown but ui system will be replaced anyway ...
            return;
        }

        if ( NULL == SelectObject(hDC, hFont) )
        {
            getConsole().EOLn(
                "PureUiFontWin(%s, %d, %b, %b, %b, %b) ERROR: SelectObject() FAILED!",
                fontFaceName.c_str(), height, bold, italic, underline, strikeout );
            // DeleteObject(hFont); should i release the font object this way?
            // exception should be thrown but ui system will be replaced anyway ...
            return;
        }

        // rumor is that wglUseFontBitmaps() fails if there is a gl error before ... so calling glGetError() before this solves that problem.
        // display lists created here will contain 1 call only: glBitmap().
        // also pls check what are the return values of this function!
        if ( !wglUseFontBitmaps(hDC, 0, 255, base) )
        {
            getConsole().EOLn(
                "PureUiFontWin(%s, %d, %b, %b, %b, %b) FAILED to generate bitmaps!",
                fontFaceName.c_str(), height, bold, italic, underline, strikeout );
            return;
        }

    }

    getConsole().SOLn(
        "PureUiFontWin(%s, %d, %b, %b, %b, %b) created font successfully, handle: %d.",
        fontFaceName.c_str(), height, bold, italic, underline, strikeout, hFont );
}


PureUiFontWin::PureUiFontWin(const PureUiFontWin& uiFont) :
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
            "PureUiFontWin(%s, %d, %d, %b, %b, %b) copy, handle: %d.",
            uiFont.sFontFaceName.c_str(), uiFont.nHeight, uiFont.nWeight, uiFont.bItalic, uiFont.bUnderline, uiFont.bStrikeOut, uiFont.hFont );
}


PureUiFontWin& PureUiFontWin::operator=(const PureUiFontWin& uiFont)
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
            "PureUiFontWin(%s, %d, %d, %b, %b, %b) assignment, handle: %d.",
            sFontFaceName.c_str(), uiFont.nHeight, uiFont.nWeight, uiFont.bItalic, uiFont.bUnderline, uiFont.bStrikeOut, uiFont.hFont );

    return *this;
}


PureUiFontWin::~PureUiFontWin()
{
    if ( hFont )
    {
        // should i release the font object this way?
        if ( DeleteObject(hFont) )
        {
            getConsole().SOLn(
                "~PureUiFontWin(%s, %d, %b, %b, %b, %b) deleted font successfully, handle: %d.",
                sFontFaceName.c_str(), nHeight, nWeight, bItalic, bUnderline, bStrikeOut, hFont );
            hFont = NULL;
        }
        else
            getConsole().EOLn(
                "~PureUiFontWin(%s, %d, %b, %b, %b, %b) FAILED to delete font, handle: %d!",
                sFontFaceName.c_str(), nHeight, nWeight, bItalic, bUnderline, bStrikeOut, hFont );

        if ( base != 0 )
        {
            glDeleteLists(base, 255);
            base = 0;
        }
            
    }
    else
        getConsole().OLn(
            "~PureUiFontWin(%s, %d, %b, %b, %b, %b) no handle.",
            sFontFaceName.c_str(), nHeight, nWeight, bItalic, bUnderline, bStrikeOut );
}


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureUiFontWin::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureUiFontWin::getLoggerModuleName()
{
    return "PureUiFontWin";
} // getLoggerModuleName()


const std::string& PureUiFontWin::getFontFaceName() const
{
    return sFontFaceName;
}


int PureUiFontWin::getHeight() const
{
    return nHeight;
}


bool PureUiFontWin::getBold() const
{
    return (nWeight == FW_BOLD);
}


bool PureUiFontWin::getItalic() const
{
    return (bItalic == 0) ? false : true;
}


bool PureUiFontWin::getUnderline() const
{
    return (bUnderline == 0) ? false : true;
}


bool PureUiFontWin::getStrikeOut() const
{
    return (bStrikeOut == 0) ? false : true;
}


unsigned int PureUiFontWin::getListBase() const
{
    return base;
}


long PureUiFontWin::getTextWidth(const std::string& text) const
{
    SIZE sizeText;
    SelectObject(hDC, hFont);
    if ( GetTextExtentPoint32(hDC, text.c_str(), text.length(), &sizeText) )
        return sizeText.cx;
    else
        return 0;
}

long PureUiFontWin::getTextHeight(const std::string& text) const
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