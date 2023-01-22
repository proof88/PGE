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
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureColor is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PureColor();                         /**< Creates a black color. */

    PureColor(const PureColor& cm);

    PureColor(
        const TPureubyte& red,
        const TPureubyte& green,
        const TPureubyte& blue,
        const TPureubyte& alpha = 0);    /**< Constructs a color. */

    TPureubyte getRed() const;           /**< Gets red component. */
    TPureubyte getGreen() const;         /**< Gets green component. */
    TPureubyte getBlue() const;          /**< Gets blue component. */
    TPureubyte getAlpha() const;         /**< Gets alpha component. */

    void Set(
        TPureubyte red,
        TPureubyte green,
        TPureubyte blue,
        TPureubyte alpha = 0);           /**< Sets all color components. */

    void SetRed(TPureubyte red);         /**< Sets red component. */
    void SetGreen(TPureubyte green);     /**< Sets green component. */
    void SetBlue(TPureubyte blue);       /**< Sets blue component. */
    void SetAlpha(TPureubyte alpha);     /**< Sets alpha component. */

    TPurefloat getRedAsFloat() const;    /**< Gets red component. */
    TPurefloat getGreenAsFloat() const;  /**< Gets green component. */
    TPurefloat getBlueAsFloat() const;   /**< Gets blue component. */
    TPurefloat getAlphaAsFloat() const;  /**< Gets alpha component. */

    void SetAsFloats(
        TPurefloat red,
        TPurefloat green,
        TPurefloat blue,
        TPurefloat alpha = 0.0f);        /**< Sets all color components. */

    void SetRedAsFloat(TPurefloat red);        /**< Sets red component. */
    void SetGreenAsFloat(TPurefloat green);    /**< Sets green component. */
    void SetBlueAsFloat(TPurefloat blue);      /**< Sets blue component. */
    void SetAlphaAsFloat(TPurefloat alpha);    /**< Sets alpha component. */

    TPurebool operator==(const PureColor& clr) const;  /**< Equals to operator. */
    TPurebool operator!=(const PureColor& clr) const;  /**< Not equals to operator. */
    PureColor& operator=(const PureColor& vec);        /**< Assignment operator. */

private:

    // ---------------------------------------------------------------------------

    TPureubyte r, g, b, a;

}; // class PureColor

std::ostream& operator<< (std::ostream &s, const PureColor& clr);  /**< Write to stream operator. */
