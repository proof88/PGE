#pragma once

/*
    ###################################################################################
    PureColor.h
    This file is part of PURE.
    PureColor class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Material/PureColor.h"
#include <sstream>
#include "../../include/internal/PurePragmas.h"


// ############################### PUBLIC ################################

/**
    Creates a black color.
*/
PureColor::PureColor() :
    r(0), g(0), b(0), a(0)
{

}

PureColor::PureColor(const PureColor& cm) :
    r(cm.r), g(cm.g), b(cm.b), a(cm.a)
{

}


/**
    Constructs a color.

    @param red   Red component in range [0, 255].
    @param green Green component in range [0, 255].
    @param blue  Blue component in range [0, 255].
    @param alpha Alpha component in range [0, 255].
*/
PureColor::PureColor(
    const TPureUByte& red,
    const TPureUByte& green,
    const TPureUByte& blue,
    const TPureUByte& alpha) :
    r(red), g(green), b(blue), a(alpha)
{
    
}


/**
    Gets red component.

    @return Red component in range [0, 255].
*/
TPureUByte PureColor::getRed() const
{
    return r;
} // getRed()


/**
    Gets green component.

    @return Green component in range [0, 255].
*/
TPureUByte PureColor::getGreen() const
{
    return g;
} // getGreen()


/**
    Gets blue component.

    @return Blue component in range [0, 255].
*/
TPureUByte PureColor::getBlue() const
{
    return b;
} // getBlue()


/**
    Gets alpha component.

    @return Alpha component in range [0, 255].
*/
TPureUByte PureColor::getAlpha() const
{
    return a;
} // getAlpha()


/**
    Sets all color components.

    @param red   Red component in range [0, 255].
    @param green Green component in range [0, 255].
    @param blue  Blue component in range [0, 255].
    @param alpha Alpha component in range [0, 255].
*/
void PureColor::Set(TPureUByte red, TPureUByte green, TPureUByte blue, TPureUByte alpha)
{
    SetRed(red);
    SetGreen(green);
    SetBlue(blue);
    SetAlpha(alpha);
} // Set()


/**
    Sets red component.

    @param red Red component in range [0, 255].
*/
void PureColor::SetRed(TPureUByte red)
{
    r = red;
} // SetRed()


/**
    Sets green component.

    @param green Green component in range [0, 255].
*/
void PureColor::SetGreen(TPureUByte green)
{
    g = green;
} // SetGreen()


/**
    Sets blue component.

    @param blue Blue component in range [0, 255].
*/
void PureColor::SetBlue(TPureUByte blue)
{
    b = blue;
} // SetBlue()


/**
    Sets alpha component.

    @param alpha Alpha component in range [0, 255].
*/
void PureColor::SetAlpha(TPureUByte alpha)
{
    a = alpha;
} // SetAlpha()


/**
    Gets red component.

    @return Red component in range [0.0f, 1.0f].
*/
TPureFloat PureColor::getRedAsFloat() const
{
    return r / 255.0f;
} // getRedAsFloat()


/**
    Gets green component.

    @return Green component in range [0.0f, 1.0f].
*/
TPureFloat PureColor::getGreenAsFloat() const
{
    return g / 255.0f;
} // getGreenAsFloat()


/**
    Gets blue component.

    @return Blue component in range [0.0f, 1.0f].
*/
TPureFloat PureColor::getBlueAsFloat() const
{
    return b / 255.0f;
} // getBlueAsFloat()


/**
    Gets alpha component.

    @return Alpha component in range [0.0f, 1.0f].
*/
TPureFloat PureColor::getAlphaAsFloat() const
{
    return a / 255.0f;
} // getAlphaAsFloat()


/**
    Sets all color components.

    @param red   Red component in range [0.0f, 1.0f].
    @param green Green component in range [0.0f, 1.0f].
    @param blue  Blue component in range [0.0f, 1.0f].
    @param alpha Alpha component in range [0.0f, 1.0f].
*/
void PureColor::SetAsFloats(TPureFloat red, TPureFloat green, TPureFloat blue, TPureFloat alpha)
{
    SetRedAsFloat(red);
    SetGreenAsFloat(green);
    SetBlueAsFloat(blue);
    SetAlphaAsFloat(alpha);
} // Set()


/**
    Sets red component.

    @param red Red component in range [0.0f, 1.0f].
*/
void PureColor::SetRedAsFloat(TPureFloat red)
{
    r = (TPureUByte) PFL::roundi(PFL::constrain(red, 0.0f, 1.0f) * 255);
} // SetRed()


/**
    Sets green component.

    @param green Green component in range [0.0f, 1.0f].
*/
void PureColor::SetGreenAsFloat(TPureFloat green)
{
    g = (TPureUByte) PFL::roundi(PFL::constrain(green, 0.0f, 1.0f) * 255);
} // SetGreen()


/**
    Sets blue component.

    @param blue Blue component in range [0.0f, 1.0f].
*/
void PureColor::SetBlueAsFloat(TPureFloat blue)
{
    b = (TPureUByte) PFL::roundi(PFL::constrain(blue, 0.0f, 1.0f) * 255);
} // SetBlue()


/**
    Sets alpha component.

    @param alpha Alpha component in range [0.0f, 1.0f].
*/
void PureColor::SetAlphaAsFloat(TPureFloat alpha)
{
    a = (TPureUByte) PFL::roundi(PFL::constrain(alpha, 0.0f, 1.0f) * 255);
} // SetAlpha()


/**
    Equals to operator.
*/
TPureBool PureColor::operator==(const PureColor& clr) const
{
    return clr.r == r && clr.g == g && clr.b == b && clr.a == a;
} // operator==()


/**
    Not equals to operator.
*/
TPureBool PureColor::operator!=(const PureColor& clr) const
{
    return !( *this == clr );
} // operator!=()


/**
    Assignment operator.
*/
PureColor& PureColor::operator=(const PureColor& vec)
{
    r = vec.r;
    g = vec.g;
    b = vec.b;
    a = vec.a;
    return *this;
} // operator=()


/**
    Write to stream operator.
*/
std::ostream& operator<< (std::ostream &s, const PureColor& clr)
{
    return ( s << "[" << (unsigned short)clr.getRed() << "," << (unsigned short)clr.getGreen() << "," << (unsigned short)clr.getBlue() << "," << (unsigned short)clr.getAlpha() << "]" );
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################




