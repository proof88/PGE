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
enum TPure_NOTABLE_MATRICES
{
    Pure_MATRIX_ZERO,         /**< Zero matrix. */
    Pure_MATRIX_IDENTITY      /**< Identity matrix. */
}; // TPure_NOTABLE_MATRICES


/**
    Pure basic 4x4 matrix class.
    For 3D transformations, descendant PureTransformMatrix class is recommended.
*/
class PureMatrix
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMatrix is included")
#endif

public:
    static const PureMatrix& getZero();       /**< Gets a constant zero matrix. */
    static const PureMatrix& getIdentity();   /**< Gets a constant identity matrix. */

    // ---------------------------------------------------------------------------

    PureMatrix();                                           /**< Creates an identity matrix. */
    PureMatrix(const PureMatrix& cm);
    explicit PureMatrix(const TPure_NOTABLE_MATRICES& nm);  /**< Creates the requested matrix. */
    virtual ~PureMatrix();

    TPurefloat get(TPurebyte row, TPurebyte col) const;              /**< Gets the element at the given row and column. */
    void       Set(TPurebyte row, TPurebyte col, TPurefloat value);  /**< Sets the element at the given row and column. */

    const TPurefloat* get() const;  /**< Returns the matrix array as is. */
    
    TPurebool isZero() const;      /**< Is it a zero matrix? */
    void      SetZero();           /**< Sets the matrix to zero matrix. */
    TPurebool isIdentity() const;  /**< Is it an identity matrix? */
    void      SetIdentity();       /**< Sets the matrix to identity matrix. */

    void              Transpose();       /**< Transposes the matrix. */
    const TPurefloat* getTransposed();   /**< Returns the matrix array in its transposed form without actually modifying the matrix. */

    TPurefloat getDeterminant() const;   /**< Returns the determinant of the matrix. */

    TPurebool isInvertible() const;      /**< Returns if the matrix is invertible. */

    TPurebool         invert();          /**< Inverts the matrix. */
    const TPurefloat* getInverse();      /**< Returns the matrix array in its inverted form without actually modifying the matrix. */
   
    TPurebool   operator==(const PureMatrix& cm) const;      /**< Equals to operator. */
    TPurebool   operator!=(const PureMatrix& cm) const;      /**< Not equals to operator. */
    PureMatrix& operator= (const PureMatrix& cm);            /**< Assignment operator. */
    PureMatrix  operator+ (const PureMatrix& cm) const;      /**< Addition operator. */
    PureMatrix& operator+=(const PureMatrix& cm);            /**< Addition assignment operator. */
    PureMatrix  operator- (const PureMatrix& cm) const;      /**< Subtraction operator. */
    PureMatrix& operator-=(const PureMatrix& cm);            /**< Subtraction assignment operator. */
    PureMatrix  operator* (const PureMatrix& cm) const;      /**< Multiplication by matrix operator. */
    PureMatrix& operator*=(const PureMatrix& cm);            /**< Multiplication assignment operator. */
    PureMatrix  operator* (const TPurefloat& scalar) const;  /**< Multiplication by scalar operator. */
    PureMatrix& operator*=(const TPurefloat& scalar);        /**< Multiplication assignment operator. */
    PureVector  operator* (const PureVector& vec) const;     /**< Multiplication by vector operator. */
    TPurefloat&       operator()(const TPurebyte& row, const TPurebyte& col);        /**< Access element operator operator. */
    const TPurefloat& operator()(const TPurebyte& row, const TPurebyte& col) const;  /**< Access element operator operator. */

protected:

    // ---------------------------------------------------------------------------

    TPurefloat mat[4][4];              /**< Values. */

private:
    static PureMatrix matIdentity;   /**< Static zero matrix. */
    static PureMatrix matZero;       /**< Static identity matrix. */

    // ---------------------------------------------------------------------------

    static TPurebool correctIndices(TPurebyte row, TPurebyte col);                     /**< Are the given row and column indices correct? */
    static TPurebyte ind(TPurebyte row, TPurebyte col);                                /**< Returns index of cell in given row and column. */

    static void      TransposeArray(TPurefloat* arr, TPurebyte rows, TPurebyte cols);  /**< Treats the given array as a matrix and transposes it. */
    static TPurebool invertArray(TPurefloat arr[], TPurebyte rows, TPurebyte cols);    /**< Treats the given array as a matrix and inverts it. */

    // ---------------------------------------------------------------------------

    TPurefloat tanspmat[4][4];         /**< Values when matrix is requested as transposed. */

}; // class PureMatrix

PureMatrix  operator- (const PureMatrix& vec);                            /**< Negative operator. */
PureMatrix  operator* (const TPurefloat& scalar, const PureMatrix& mat);  /**< Multiplication by scalar operator. */
PureVector  operator* (const PureVector& vec, const PureMatrix& mat);     /**< Multiplication by vector operator. */
PureVector& operator*=(PureVector& vec, const PureMatrix& mat);           /**< Multiplication assignment operator. */
