#pragma once

/*
    ###################################################################################
    PRREColor.h
    This file is part of PRRE.
    External header.
    PRRE color class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../PRREallHeaders.h"
#include "../PRREtypes.h"
#include <sstream>


/**
    PRRE color class.

    This class doesn't use any API directly.
*/
class PRREColor
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREColor is included")
#endif

public:

    // ---------------------------------------------------------------------------

    PRREColor();                         /**< Creates a black color. */

    PRREColor(const PRREColor& cm);

    PRREColor(
        const TPRREubyte& red,
        const TPRREubyte& green,
        const TPRREubyte& blue,
        const TPRREubyte& alpha = 0);    /**< Constructs a color. */

    TPRREubyte getRed() const;           /**< Gets red component. */
    TPRREubyte getGreen() const;         /**< Gets green component. */
    TPRREubyte getBlue() const;          /**< Gets blue component. */
    TPRREubyte getAlpha() const;         /**< Gets alpha component. */

    void Set(
        TPRREubyte red,
        TPRREubyte green,
        TPRREubyte blue,
        TPRREubyte alpha = 0);           /**< Sets all color components. */

    void SetRed(TPRREubyte red);         /**< Sets red component. */
    void SetGreen(TPRREubyte green);     /**< Sets green component. */
    void SetBlue(TPRREubyte blue);       /**< Sets blue component. */
    void SetAlpha(TPRREubyte alpha);     /**< Sets alpha component. */

    TPRREfloat getRedAsFloat() const;    /**< Gets red component. */
    TPRREfloat getGreenAsFloat() const;  /**< Gets green component. */
    TPRREfloat getBlueAsFloat() const;   /**< Gets blue component. */
    TPRREfloat getAlphaAsFloat() const;  /**< Gets alpha component. */

    void SetAsFloats(
        TPRREfloat red,
        TPRREfloat green,
        TPRREfloat blue,
        TPRREfloat alpha = 0.0f);        /**< Sets all color components. */

    void SetRedAsFloat(TPRREfloat red);        /**< Sets red component. */
    void SetGreenAsFloat(TPRREfloat green);    /**< Sets green component. */
    void SetBlueAsFloat(TPRREfloat blue);      /**< Sets blue component. */
    void SetAlphaAsFloat(TPRREfloat alpha);    /**< Sets alpha component. */

    TPRREbool operator==(const PRREColor& clr) const;  /**< Equals to operator. */
    TPRREbool operator!=(const PRREColor& clr) const;  /**< Not equals to operator. */
    PRREColor& operator=(const PRREColor& vec);        /**< Assignment operator. */

private:

    // ---------------------------------------------------------------------------

    TPRREubyte r, g, b, a;

}; // class PRREColor

std::ostream& operator<< (std::ostream &s, const PRREColor& clr);  /**< Write to stream operator. */
