#pragma once

/*
    ###################################################################################
    PureUiText.cpp
    This file is part of PURE.
    Pure User Interface Text class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../include/internal/PureUiText.h"
#include "../include/internal/PurePragmas.h"
#include "../include/external/Hardware/PureHwInfo.h"


// ############################### PUBLIC ################################


unsigned long PureUiText::getHash(const std::string& text, int x, int y, int height)
{
    return calcHash(text, x, y, height);
}

// ---------------------------------------------------------------------------

 
PureUiText::PureUiText() :
    _x(0),
    _y(0),
    bDelete(false),
    bPermanent(false),
    bDropShadow(false),
    uiFont(uiFont)
{}

PureUiText::PureUiText(std::string text, int x, int y, PureUiFontWin& font)
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

PureUiText::PureUiText(const PureUiText& uiText) :
    sText(uiText.sText),
    _x(uiText._x),
    _y(uiText._y),
    bDelete(uiText.bDelete),
    bPermanent(uiText.bPermanent),
    bDropShadow(uiText.bDropShadow),
    uiFont(uiText.uiFont)
{}

PureUiText::~PureUiText()
{}

const std::string& PureUiText::getText() const
{
    return sText;
}

int PureUiText::getX() const
{
    return _x;
}

void PureUiText::SetX(int newx)
{
    _x = newx;
}

int PureUiText::getY() const
{
    return _y;
}

void PureUiText::SetY(int newy)
{
    _y = newy;
}

long PureUiText::getWidth() const
{
    return uiFont->getTextWidth(sText);
}

long PureUiText::getHeight() const
{
    return uiFont->getTextHeight(sText);
}

bool PureUiText::getDelete() const
{
    return bDelete;
}

void PureUiText::SetDelete(bool value)
{
    bDelete = value;
}

bool PureUiText::getPermanent() const
{
    return bPermanent;
}

void PureUiText::SetPermanent(bool value)
{
    bPermanent = value;
}

const PureUiFontWin* PureUiText::getFont() const
{
    return uiFont;
}

bool PureUiText::getDropShadow() const
{
    return bDropShadow;
}

void PureUiText::SetDropShadow(bool value)
{
    bDropShadow = value;
}

/**
    Returns reference to text color.
    The text color is black by default.
*/
const PureColor& PureUiText::getColor() const
{
    return clr;
}

/**
    Returns reference to text color.
    The text color is black by default.
*/
PureColor& PureUiText::getColor()
{
    return clr;
}

void PureUiText::PrintText() const
{
    /* SW rendering currently not supported here */
    if ( !PureHwInfo::get().getVideo().isAcceleratorDetected() )
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

PureUiText& PureUiText::operator=(const PureUiText& uiText)
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


unsigned long PureUiText::calcHash(const std::string& text, int x, int y, int height)
{
    unsigned long hash = 5381;

    for (std::string::size_type i = 0; i < text.length(); i++)
        hash = ((hash << 5) + hash) + (int)(text[i]) + x*3000 + y + height; /* hash * 33 + c + x + y + height*/

    return hash;
}


