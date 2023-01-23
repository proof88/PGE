#pragma once

/*
    ###################################################################################
    PureColor.h
    This file is part of PURE.
    External header.
    Pure color class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include <sstream>


/**
    Pure color class.
*/
class PureColor
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureColor is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PureColor();                         /**< Creates a black color. */

    PureColor(const PureColor& cm);

    PureColor(
        const TPureUByte& red,
        const TPureUByte& green,
        const TPureUByte& blue,
        const TPureUByte& alpha = 0);    /**< Constructs a color. */

    TPureUByte getRed() const;           /**< Gets red component. */
    TPureUByte getGreen() const;         /**< Gets green component. */
    TPureUByte getBlue() const;          /**< Gets blue component. */
    TPureUByte getAlpha() const;         /**< Gets alpha component. */

    void Set(
        TPureUByte red,
        TPureUByte green,
        TPureUByte blue,
        TPureUByte alpha = 0);           /**< Sets all color components. */

    void SetRed(TPureUByte red);         /**< Sets red component. */
    void SetGreen(TPureUByte green);     /**< Sets green component. */
    void SetBlue(TPureUByte blue);       /**< Sets blue component. */
    void SetAlpha(TPureUByte alpha);     /**< Sets alpha component. */

    TPureFloat getRedAsFloat() const;    /**< Gets red component. */
    TPureFloat getGreenAsFloat() const;  /**< Gets green component. */
    TPureFloat getBlueAsFloat() const;   /**< Gets blue component. */
    TPureFloat getAlphaAsFloat() const;  /**< Gets alpha component. */

    void SetAsFloats(
        TPureFloat red,
        TPureFloat green,
        TPureFloat blue,
        TPureFloat alpha = 0.0f);        /**< Sets all color components. */

    void SetRedAsFloat(TPureFloat red);        /**< Sets red component. */
    void SetGreenAsFloat(TPureFloat green);    /**< Sets green component. */
    void SetBlueAsFloat(TPureFloat blue);      /**< Sets blue component. */
    void SetAlphaAsFloat(TPureFloat alpha);    /**< Sets alpha component. */

    TPureBool operator==(const PureColor& clr) const;  /**< Equals to operator. */
    TPureBool operator!=(const PureColor& clr) const;  /**< Not equals to operator. */
    PureColor& operator=(const PureColor& vec);        /**< Assignment operator. */

private:

    // ---------------------------------------------------------------------------

    TPureUByte r, g, b, a;

}; // class PureColor

std::ostream& operator<< (std::ostream &s, const PureColor& clr);  /**< Write to stream operator. */
