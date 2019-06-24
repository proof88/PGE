#pragma once

/*
    ###################################################################################
    PRREColor.h
    This file is part of PRRE.
    PRRE color class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREColor.h"
#include <sstream>


// ############################### PUBLIC ################################

/**
    Creates a black color.
*/
PRREColor::PRREColor() :
    r(0), g(0), b(0), a(0)
{

}

PRREColor::PRREColor(const PRREColor& cm) :
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
PRREColor::PRREColor(
    const TPRREubyte& red,
    const TPRREubyte& green,
    const TPRREubyte& blue,
    const TPRREubyte& alpha) :
    r(red), g(green), b(blue), a(alpha)
{
    
}


/**
    Gets red component.

    @return Red component in range [0, 255].
*/
TPRREubyte PRREColor::getRed() const
{
    return r;
} // getRed()


/**
    Gets green component.

    @return Green component in range [0, 255].
*/
TPRREubyte PRREColor::getGreen() const
{
    return g;
} // getGreen()


/**
    Gets blue component.

    @return Blue component in range [0, 255].
*/
TPRREubyte PRREColor::getBlue() const
{
    return b;
} // getBlue()


/**
    Gets alpha component.

    @return Alpha component in range [0, 255].
*/
TPRREubyte PRREColor::getAlpha() const
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
void PRREColor::Set(TPRREubyte red, TPRREubyte green, TPRREubyte blue, TPRREubyte alpha)
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
void PRREColor::SetRed(TPRREubyte red)
{
    r = red;
} // SetRed()


/**
    Sets green component.

    @param green Green component in range [0, 255].
*/
void PRREColor::SetGreen(TPRREubyte green)
{
    g = green;
} // SetGreen()


/**
    Sets blue component.

    @param blue Blue component in range [0, 255].
*/
void PRREColor::SetBlue(TPRREubyte blue)
{
    b = blue;
} // SetBlue()


/**
    Sets alpha component.

    @param alpha Alpha component in range [0, 255].
*/
void PRREColor::SetAlpha(TPRREubyte alpha)
{
    a = alpha;
} // SetAlpha()


/**
    Gets red component.

    @return Red component in range [0.0f, 1.0f].
*/
TPRREfloat PRREColor::getRedAsFloat() const
{
    return r / 255.0f;
} // getRedAsFloat()


/**
    Gets green component.

    @return Green component in range [0.0f, 1.0f].
*/
TPRREfloat PRREColor::getGreenAsFloat() const
{
    return g / 255.0f;
} // getGreenAsFloat()


/**
    Gets blue component.

    @return Blue component in range [0.0f, 1.0f].
*/
TPRREfloat PRREColor::getBlueAsFloat() const
{
    return b / 255.0f;
} // getBlueAsFloat()


/**
    Gets alpha component.

    @return Alpha component in range [0.0f, 1.0f].
*/
TPRREfloat PRREColor::getAlphaAsFloat() const
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
void PRREColor::Setf(TPRREfloat red, TPRREfloat green, TPRREfloat blue, TPRREfloat alpha)
{
    SetRedf(red);
    SetGreenf(green);
    SetBluef(blue);
    SetAlphaf(alpha);
} // Set()


/**
    Sets red component.

    @param red Red component in range [0.0f, 1.0f].
*/
void PRREColor::SetRedf(TPRREfloat red)
{
    r = (TPRREubyte) PFL::roundi(PFL::constrain(red, 0.0f, 1.0f) * 255);
} // SetRed()


/**
    Sets green component.

    @param green Green component in range [0.0f, 1.0f].
*/
void PRREColor::SetGreenf(TPRREfloat green)
{
    g = (TPRREubyte) PFL::roundi(PFL::constrain(green, 0.0f, 1.0f) * 255);
} // SetGreen()


/**
    Sets blue component.

    @param blue Blue component in range [0.0f, 1.0f].
*/
void PRREColor::SetBluef(TPRREfloat blue)
{
    b = (TPRREubyte) PFL::roundi(PFL::constrain(blue, 0.0f, 1.0f) * 255);
} // SetBlue()


/**
    Sets alpha component.

    @param alpha Alpha component in range [0.0f, 1.0f].
*/
void PRREColor::SetAlphaf(TPRREfloat alpha)
{
    a = (TPRREubyte) PFL::roundi(PFL::constrain(alpha, 0.0f, 1.0f) * 255);
} // SetAlpha()


/**
    Equals to operator.
*/
TPRREbool PRREColor::operator==(const PRREColor& clr) const
{
    return clr.r == r && clr.g == g && clr.b == b && clr.a == a;
} // operator==()


/**
    Not equals to operator.
*/
TPRREbool PRREColor::operator!=(const PRREColor& clr) const
{
    return !( *this == clr );
} // operator!=()


/**
    Assignment operator.
*/
PRREColor& PRREColor::operator=(const PRREColor& vec)
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
std::ostream& operator<< (std::ostream &s, const PRREColor& clr)
{
    return ( s << "[" << (unsigned short)clr.getRed() << "," << (unsigned short)clr.getGreen() << "," << (unsigned short)clr.getBlue() << "," << (unsigned short)clr.getAlpha() << "]" );
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################




