#pragma once

/*
    ###################################################################################
    PureVector.h
    This file is part of PURE.
    External header.
    Pure 3D-vector class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include <sstream>

/**
    Pure 3D-vector class.
    Pure uses the left-handed Cartesian coordinate system.
    This means the XZ plane is horizontal, the XY and and YZ planes are vertical,
    a positive X value means right, a positive Y value means up, and a positive Z value means forward.
*/
class PureVector
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureVector is included")
#endif

public:
    PureVector();                      /** Creates a nullvector. */

    PureVector(const PureVector& vec);

    PureVector(
        const TPurefloat& x,
        const TPurefloat& y,
        const TPurefloat& z );   /**< Creates a vector containing the specified values. */

    virtual ~PureVector();

    TPurefloat getX() const;            /**< Gets the X-coordinate. */
    TPurefloat getY() const;            /**< Gets the Y-coordinate. */
    TPurefloat getZ() const;            /**< Gets the Z-coordinate. */
    const TPure_XYZW& getXYZW() const;  /**< Gets all the coordinates. */
    TPurefloat get(TPurebyte index) const;    /**< Gets the value at the given index. */
    void       SetX(TPurefloat x);            /**< Sets the X-coordinate. */
    void       SetY(TPurefloat y);            /**< Sets the Y-coordinate. */
    void       SetZ(TPurefloat z);            /**< Sets the Z-coordinate. */
    void       Set(TPurebyte index, TPurefloat value);          /**< Sets the value at the given index. */
    void       Set(TPurefloat x, TPurefloat y, TPurefloat z);   /**< Sets the given values. */
    
    TPurebool  isZero() const;       /**< Gets if the vector is nullvector. */
    void       SetZero();            /**< Sets the vector to be a nullvector. */
    TPurefloat getLength() const;    /**< Gets the length. */
    void       Normalize();          /**< Normalizes the coordinates.*/

    TPurefloat getDotProduct(const PureVector& vec) const;       /**< Dot product. */
    PureVector getCrossProduct(const PureVector& vec) const;     /**< Cross product. */
    
    TPurebool   operator==(const PureVector& vec) const;         /**< Equals to operator. */
    TPurebool   operator!=(const PureVector& vec) const;         /**< Not equals to operator. */
    PureVector& operator= (const PureVector& vec);               /**< Assignment operator. */
    PureVector  operator+ (const PureVector& vec) const;         /**< Addition operator. */
    PureVector& operator+=(const PureVector& vec);               /**< Addition assignment operator. */
    PureVector  operator- (const PureVector& vec) const;         /**< Subtraction operator. */
    PureVector& operator-=(const PureVector& vec);               /**< Subtraction assignment operator. */
    PureVector  operator* (const TPurefloat& scalar) const;      /**< Multiplication by scalar operator. */
    PureVector& operator*=(const TPurefloat& scalar);            /**< Multiplication assignment operator. */
    TPurefloat  operator* (const PureVector& vec) const;         /**< Dot product operator. */
    PureVector  operator^ (const PureVector& vec) const;         /**< Cross product operator. */
    PureVector& operator^=(const PureVector& vec);               /**< Cross product assignment operator. */
    TPurefloat&       operator()(const TPurebyte& index);        /**< Access element operator. */
    const TPurefloat& operator()(const TPurebyte& index) const;  /**< Access element operator. */
    TPurefloat&       operator[](const TPurebyte& index);        /**< Access element operator. */
    const TPurefloat& operator[](const TPurebyte& index) const;  /**< Access element operator. */
    TPurebool operator<(const PureVector& r) const;              /**< Comparison operator "less than". */
    TPurebool operator>(const PureVector& r) const;              /**< Comparison operator "greater than". */
    TPurebool operator<=(const PureVector& r) const;             /**< Comparison operator "less than or equal". */
    TPurebool operator>=(const PureVector& r) const;             /**< Comparison operator "greater than or equal". */

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
    TPure_XYZW pos;        /**< Coordinates. */

}; // class PureVector

PureVector operator- (const PureVector& vec);                            /**< Negative operator. */
PureVector operator* (const TPurefloat& scalar, const PureVector& vec);  /**< Multiplication by scalar. */

std::ostream& operator<< (std::ostream &s, const PureVector& vec);  /**< Write to stream. */