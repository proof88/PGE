#pragma once

/*
    ###################################################################################
    PRREVector.h
    This file is part of PRRE.
    External header.
    PRRE 3D-vector class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREtypes.h"
#include <sstream>

/**
    PRRE 3D-vector class.
    PRRE uses the left-handed Cartesian coordinate system.
    This means the XZ plane is horizontal, the XY and and YZ planes are vertical,
    a positive X value means right, a positive Y value means up, and a positive Z value means forward.

    This class doesn't use any API directly.
*/
class PRREVector
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREVector is included")
#endif

public:
    PRREVector();                      /** Creates a nullvector. */

    PRREVector(const PRREVector& vec);

    PRREVector(
        const TPRREfloat& x,
        const TPRREfloat& y,
        const TPRREfloat& z );   /**< Creates a vector containing the specified values. */

    virtual ~PRREVector();

    TPRREfloat getX() const;            /**< Gets the X-coordinate. */
    TPRREfloat getY() const;            /**< Gets the Y-coordinate. */
    TPRREfloat getZ() const;            /**< Gets the Z-coordinate. */
    const TPRRE_XYZW& getXYZW() const;  /**< Gets all the coordinates. */
    TPRREfloat get(TPRREbyte index) const;    /**< Gets the value at the given index. */
    void       SetX(TPRREfloat x);            /**< Sets the X-coordinate. */
    void       SetY(TPRREfloat y);            /**< Sets the Y-coordinate. */
    void       SetZ(TPRREfloat z);            /**< Sets the Z-coordinate. */
    void       Set(TPRREbyte index, TPRREfloat value);          /**< Sets the value at the given index. */
    void       Set(TPRREfloat x, TPRREfloat y, TPRREfloat z);   /**< Sets the given values. */
    
    TPRREbool  isZero() const;       /**< Gets if the vector is nullvector. */
    void       SetZero();            /**< Sets the vector to be a nullvector. */
    TPRREfloat getLength() const;    /**< Gets the length. */
    void       Normalize();          /**< Normalizes the coordinates.*/

    TPRREfloat getDotProduct(const PRREVector& vec) const;       /**< Dot product. */
    PRREVector getCrossProduct(const PRREVector& vec) const;     /**< Cross product. */
    
    TPRREbool   operator==(const PRREVector& vec) const;         /**< Equals to operator. */
    TPRREbool   operator!=(const PRREVector& vec) const;         /**< Not equals to operator. */
    PRREVector& operator= (const PRREVector& vec);               /**< Assignment operator. */
    PRREVector  operator+ (const PRREVector& vec) const;         /**< Addition operator. */
    PRREVector& operator+=(const PRREVector& vec);               /**< Addition assignment operator. */
    PRREVector  operator- (const PRREVector& vec) const;         /**< Subtraction operator. */
    PRREVector& operator-=(const PRREVector& vec);               /**< Subtraction assignment operator. */
    PRREVector  operator* (const TPRREfloat& scalar) const;      /**< Multiplication by scalar operator. */
    PRREVector& operator*=(const TPRREfloat& scalar);            /**< Multiplication assignment operator. */
    TPRREfloat  operator* (const PRREVector& vec) const;         /**< Dot product operator. */
    PRREVector  operator^ (const PRREVector& vec) const;         /**< Cross product operator. */
    PRREVector& operator^=(const PRREVector& vec);               /**< Cross product assignment operator. */
    TPRREfloat&       operator()(const TPRREbyte& index);        /**< Access element operator. */
    const TPRREfloat& operator()(const TPRREbyte& index) const;  /**< Access element operator. */
    TPRREfloat&       operator[](const TPRREbyte& index);        /**< Access element operator. */
    const TPRREfloat& operator[](const TPRREbyte& index) const;  /**< Access element operator. */

protected:

    // ---------------------------------------------------------------------------

private:
    static bool correctIndex(int index);

    // ---------------------------------------------------------------------------
    
    /*
        Add a fake "w" coordinate to make a 4D vector that looks like
        (x, y, z, w), and in every case, just make the w coordinate be 1.
        So the point (3, 5, 7) will be represented by (3, 5, 7, 1).
    */
    TPRRE_XYZW pos;        /**< Coordinates. */

}; // class PRREVector

PRREVector operator- (const PRREVector& vec);                            /**< Negative operator. */
PRREVector operator* (const TPRREfloat& scalar, const PRREVector& vec);  /**< Multiplication by scalar. */

std::ostream& operator<< (std::ostream &s, const PRREVector& vec);  /**< Write to stream. */