#pragma once

/*
    ###################################################################################
    PureMatrix.h
    This file is part of PURE.
    External header.
    Pure 3D-matrix class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include "PureVector.h"


/**
    Notable matrices.
*/
enum TPURE_NOTABLE_MATRICES
{
    PURE_MATRIX_ZERO,         /**< Zero matrix. */
    PURE_MATRIX_IDENTITY      /**< Identity matrix. */
}; // TPURE_NOTABLE_MATRICES


/**
    Pure basic 4x4 matrix class.
    For 3D transformations, descendant PureTransformMatrix class is recommended.
*/
class PureMatrix
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMatrix is included")
#endif

public:
    static const PureMatrix& getZero();       /**< Gets a constant zero matrix. */
    static const PureMatrix& getIdentity();   /**< Gets a constant identity matrix. */

    // ---------------------------------------------------------------------------

    PureMatrix();                                           /**< Creates an identity matrix. */
    PureMatrix(const PureMatrix& cm);
    explicit PureMatrix(const TPURE_NOTABLE_MATRICES& nm);  /**< Creates the requested matrix. */
    virtual ~PureMatrix();

    TPureFloat get(TPureByte row, TPureByte col) const;              /**< Gets the element at the given row and column. */
    void       Set(TPureByte row, TPureByte col, TPureFloat value);  /**< Sets the element at the given row and column. */

    const TPureFloat* get() const;  /**< Returns the matrix array as is. */
    
    TPureBool isZero() const;      /**< Is it a zero matrix? */
    void      SetZero();           /**< Sets the matrix to zero matrix. */
    TPureBool isIdentity() const;  /**< Is it an identity matrix? */
    void      SetIdentity();       /**< Sets the matrix to identity matrix. */

    void              Transpose();       /**< Transposes the matrix. */
    const TPureFloat* getTransposed();   /**< Returns the matrix array in its transposed form without actually modifying the matrix. */

    TPureFloat getDeterminant() const;   /**< Returns the determinant of the matrix. */

    TPureBool isInvertible() const;      /**< Returns if the matrix is invertible. */

    TPureBool         invert();          /**< Inverts the matrix. */
    const TPureFloat* getInverse();      /**< Returns the matrix array in its inverted form without actually modifying the matrix. */
   
    TPureBool   operator==(const PureMatrix& cm) const;      /**< Equals to operator. */
    TPureBool   operator!=(const PureMatrix& cm) const;      /**< Not equals to operator. */
    PureMatrix& operator= (const PureMatrix& cm);            /**< Assignment operator. */
    PureMatrix  operator+ (const PureMatrix& cm) const;      /**< Addition operator. */
    PureMatrix& operator+=(const PureMatrix& cm);            /**< Addition assignment operator. */
    PureMatrix  operator- (const PureMatrix& cm) const;      /**< Subtraction operator. */
    PureMatrix& operator-=(const PureMatrix& cm);            /**< Subtraction assignment operator. */
    PureMatrix  operator* (const PureMatrix& cm) const;      /**< Multiplication by matrix operator. */
    PureMatrix& operator*=(const PureMatrix& cm);            /**< Multiplication assignment operator. */
    PureMatrix  operator* (const TPureFloat& scalar) const;  /**< Multiplication by scalar operator. */
    PureMatrix& operator*=(const TPureFloat& scalar);        /**< Multiplication assignment operator. */
    PureVector  operator* (const PureVector& vec) const;     /**< Multiplication by vector operator. */
    TPureFloat&       operator()(const TPureByte& row, const TPureByte& col);        /**< Access element operator operator. */
    const TPureFloat& operator()(const TPureByte& row, const TPureByte& col) const;  /**< Access element operator operator. */

protected:

    // ---------------------------------------------------------------------------

    TPureFloat mat[4][4];              /**< Values. */

private:
    static PureMatrix matIdentity;   /**< Static zero matrix. */
    static PureMatrix matZero;       /**< Static identity matrix. */

    // ---------------------------------------------------------------------------

    static TPureBool correctIndices(TPureByte row, TPureByte col);                     /**< Are the given row and column indices correct? */
    static TPureByte ind(TPureByte row, TPureByte col);                                /**< Returns index of cell in given row and column. */

    static void      TransposeArray(TPureFloat* arr, TPureByte rows, TPureByte cols);  /**< Treats the given array as a matrix and transposes it. */
    static TPureBool invertArray(TPureFloat arr[], TPureByte rows, TPureByte cols);    /**< Treats the given array as a matrix and inverts it. */

    // ---------------------------------------------------------------------------

    TPureFloat tanspmat[4][4];         /**< Values when matrix is requested as transposed. */

}; // class PureMatrix

PureMatrix  operator- (const PureMatrix& vec);                            /**< Negative operator. */
PureMatrix  operator* (const TPureFloat& scalar, const PureMatrix& mat);  /**< Multiplication by scalar operator. */
PureVector  operator* (const PureVector& vec, const PureMatrix& mat);     /**< Multiplication by vector operator. */
PureVector& operator*=(PureVector& vec, const PureMatrix& mat);           /**< Multiplication assignment operator. */
