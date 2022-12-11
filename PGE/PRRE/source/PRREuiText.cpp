#pragma once

/*
    ###################################################################################
    PRREuiText.cpp
    This file is part of PRRE.
    PRRE User Interface Text class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../include/internal/PRREuiText.h"
#include "../include/internal/PRREpragmas.h"
#include "../include/external/Hardware/PRREhwInfo.h"


// ############################### PUBLIC ################################


unsigned long PRREuiText::getHash(const std::string& text, int x, int y, int height)
{
    return calcHash(text, x, y, height);
}

// ---------------------------------------------------------------------------

 
PRREuiText::PRREuiText() :
    _x(0),
    _y(0),
    bDelete(false),
    bPermanent(false),
    bDropShadow(false),
    uiFont(uiFont)
{}

PRREuiText::PRREuiText(std::string text, int x, int y, PRREuiFontWin& font)
{
    sText = text;
    _x = x;
    _y = y;
    bDelete = false;
    bPermanent = true;
    bDropShadow = false;
    uiFont = &font;
    // calcHash();
}

PRREuiText::PRREuiText(const PRREuiText& uiText) :
    sText(uiText.sText),
    _x(uiText._x),
    _y(uiText._y),
    bDelete(uiText.bDelete),
    bPermanent(uiText.bPermanent),
    bDropShadow(uiText.bDropShadow),
    uiFont(uiText.uiFont)
{}

PRREuiText::~PRREuiText()
{}

const std::string& PRREuiText::getText() const
{
    return sText;
}

int PRREuiText::getX() const
{
    return _x;
}

void PRREuiText::SetX(int newx)
{
    _x = newx;
}

int PRREuiText::getY() const
{
    return _y;
}

void PRREuiText::SetY(int newy)
{
    _y = newy;
}

long PRREuiText::getWidth() const
{
    return uiFont->getTextWidth(sText);
}

long PRREuiText::getHeight() const
{
    return uiFont->getTextHeight(sText);
}

bool PRREuiText::getDelete() const
{
    return bDelete;
}

void PRREuiText::SetDelete(bool value)
{
    bDelete = value;
}

bool PRREuiText::getPermanent() const
{
    return bPermanent;
}

void PRREuiText::SetPermanent(bool value)
{
    bPermanent = value;
}

const PRREuiFontWin* PRREuiText::getFont() const
{
    return uiFont;
}

bool PRREuiText::getDropShadow() const
{
    return bDropShadow;
}

void PRREuiText::SetDropShadow(bool value)
{
    bDropShadow = value;
}

/**
    Returns reference to text color.
    The text color is black by default.
*/
const PRREColor& PRREuiText::getColor() const
{
    return clr;
}

/**
    Returns reference to text color.
    The text color is black by default.
*/
PRREColor& PRREuiText::getColor()
{
    return clr;
}

void PRREuiText::PrintText() const
{
    /* SW rendering currently not supported here */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( uiFont->getListBase() == 0 )
        return;

    if (bDropShadow)
    {
        glColor4f(0.f, 0.f, 0.f, clr.getAlphaAsFloat());
        glRasterPos2f((GLfloat)(_x+1), (GLfloat)(_y-1));
        glPushAttrib(GL_LIST_BIT);

            glListBase(uiFont->getListBase());
            glCallLists(sText.length(), GL_UNSIGNED_BYTE, sText.c_str());

        glPopAttrib();
    }

    glColor4f(clr.getRedAsFloat(), clr.getGreenAsFloat(), clr.getBlueAsFloat(), clr.getAlphaAsFloat());
    glRasterPos2f((GLfloat)_x, (GLfloat)_y);

    glPushAttrib(GL_LIST_BIT);
      
        glListBase(uiFont->getListBase());
        glCallLists(sText.length(), GL_UNSIGNED_BYTE, sText.c_str());
   
    glPopAttrib();
}

PRREuiText& PRREuiText::operator=(const PRREuiText& uiText)
{
    sText = uiText.sText;
    _x = uiText._x;
    _y = uiText._y;
    bDelete = uiText.bDelete;
    bPermanent = uiText.bPermanent;
    uiFont = uiText.uiFont;
    return *this;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


unsigned long PRREuiText::calcHash(const std::string& text, int x, int y, int height)
{
    unsigned long hash = 5381;

    for (std::string::size_type i = 0; i < text.length(); i++)
        hash = ((hash << 5) + hash) + (int)(text[i]) + x*3000 + y + height; /* hash * 33 + c + x + y + height*/

    return hash;
}


