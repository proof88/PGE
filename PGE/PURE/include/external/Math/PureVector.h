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
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureVector is included")
#endif

public:
    PureVector();                      /** Creates a nullvector. */

    PureVector(const PureVector& vec);

    PureVector(
        const TPureFloat& x,
        const TPureFloat& y,
        const TPureFloat& z );   /**< Creates a vector containing the specified values. */

    virtual ~PureVector();

    TPureFloat getX() const;            /**< Gets the X-coordinate. */
    TPureFloat getY() const;            /**< Gets the Y-coordinate. */
    TPureFloat getZ() const;            /**< Gets the Z-coordinate. */
    TPureFloat getW() const;            /**< Gets the W-coordinate. */
    const TPURE_XYZW& getXYZW() const;  /**< Gets all the coordinates. */
    TPureFloat get(TPureByte index) const;    /**< Gets the value at the given index. */
    void       SetX(TPureFloat x);            /**< Sets the X-coordinate. */
    void       SetY(TPureFloat y);            /**< Sets the Y-coordinate. */
    void       SetZ(TPureFloat z);            /**< Sets the Z-coordinate. */
    void       Set(TPureByte index, TPureFloat value);          /**< Sets the value at the given index. */
    void       Set(TPureFloat x, TPureFloat y, TPureFloat z);   /**< Sets the given values. */
    
    TPureBool  isZero() const;       /**< Gets if the vector is nullvector. */
    void       SetZero();            /**< Sets the vector to be a nullvector. */
    TPureFloat getLength() const;    /**< Gets the length. */
    void       Normalize();          /**< Normalizes the coordinates.*/

    TPureFloat getDotProduct(const PureVector& vec) const;       /**< Dot product. */
    PureVector getCrossProduct(const PureVector& vec) const;     /**< Cross product. */
    
    TPureBool   operator==(const PureVector& vec) const;         /**< Equals to operator. */
    TPureBool   operator!=(const PureVector& vec) const;         /**< Not equals to operator. */
    PureVector& operator= (const PureVector& vec);               /**< Assignment operator. */
    PureVector  operator+ (const PureVector& vec) const;         /**< Addition operator. */
    PureVector& operator+=(const PureVector& vec);               /**< Addition assignment operator. */
    PureVector  operator- (const PureVector& vec) const;         /**< Subtraction operator. */
    PureVector& operator-=(const PureVector& vec);               /**< Subtraction assignment operator. */
    PureVector  operator* (const TPureFloat& scalar) const;      /**< Multiplication by scalar operator. */
    PureVector& operator*=(const TPureFloat& scalar);            /**< Multiplication assignment operator. */
    PureVector  operator/ (const TPureFloat& scalar) const;      /**< Division by scalar operator. */
    PureVector& operator/=(const TPureFloat& scalar);            /**< Division assignment operator. */
    TPureFloat  operator* (const PureVector& vec) const;         /**< Dot product operator. */
    PureVector  operator^ (const PureVector& vec) const;         /**< Cross product operator. */
    PureVector& operator^=(const PureVector& vec);               /**< Cross product assignment operator. */
    TPureFloat&       operator()(const TPureByte& index);        /**< Access element operator. */
    const TPureFloat& operator()(const TPureByte& index) const;  /**< Access element operator. */
    TPureFloat&       operator[](const TPureByte& index);        /**< Access element operator. */
    const TPureFloat& operator[](const TPureByte& index) const;  /**< Access element operator. */
    TPureBool operator<(const PureVector& r) const;              /**< Comparison operator "less than". */
    TPureBool operator>(const PureVector& r) const;              /**< Comparison operator "greater than". */
    TPureBool operator<=(const PureVector& r) const;             /**< Comparison operator "less than or equal". */
    TPureBool operator>=(const PureVector& r) const;             /**< Comparison operator "greater than or equal". */

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
    TPURE_XYZW pos;        /**< Coordinates. */

}; // class PureVector

PureVector operator- (const PureVector& vec);                            /**< Negative operator. */
PureVector operator* (const TPureFloat& scalar, const PureVector& vec);  /**< Multiplication by scalar. */

std::ostream& operator<< (std::ostream &s, const PureVector& vec);  /**< Write to stream. */