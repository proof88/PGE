#pragma once

/*
    ###################################################################################
    PRREMatrix.h
    This file is part of PRRE.
    External header.
    PRRE 3D-matrix class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREtypes.h"
#include "PRREVector.h"


/**
    Notable matrices.
*/
enum TPRRE_NOTABLE_MATRICES
{
    PRRE_MATRIX_ZERO,         /**< Zero matrix. */
    PRRE_MATRIX_IDENTITY      /**< Identity matrix. */
}; // TPRRE_NOTABLE_MATRICES


/**
    PRRE 3D-matrix class.
    PRRE uses the left-handed Cartesian coordinate system.
    This means the XZ plane is horizontal, the XY and and YZ planes are vertical,
    a positive X value means right, a positive Y value means up, and a positive Z value means forward.

    This class doesn't use any API directly.
*/
class PRREMatrix
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREMatrix is included")
#endif

public:
    static const PRREMatrix& getZero();       /**< Gets a constant zero matrix. */
    static const PRREMatrix& getIdentity();   /**< Gets a constant identity matrix. */

    // ---------------------------------------------------------------------------

    PRREMatrix();                                           /**< Creates an identity matrix. */
    PRREMatrix(const PRREMatrix& cm);
    explicit PRREMatrix(const TPRRE_NOTABLE_MATRICES& nm);  /**< Creates the requested matrix. */
    virtual ~PRREMatrix();

    TPRREfloat get(TPRREbyte row, TPRREbyte col) const;              /**< Gets the element at the given row and column. */
    void       Set(TPRREbyte row, TPRREbyte col, TPRREfloat value);  /**< Sets the element at the given row and column. */

    const TPRREfloat* get() const;  /**< Returns the matrix array as is. */
    
    TPRREbool isZero() const;      /**< Is it a zero matrix? */
    void      SetZero();           /**< Sets the matrix to zero matrix. */
    TPRREbool isIdentity() const;  /**< Is it an identity matrix? */
    void      SetIdentity();       /**< Sets the matrix to identity matrix. */

    void              Transpose();       /**< Transposes the matrix. */
    const TPRREfloat* getTransposed();   /**< Returns the matrix array in its transposed form without actually modifying the matrix. */

    TPRREfloat getDeterminant() const;   /**< Returns the determinant of the matrix. */

    TPRREbool isInvertible() const;      /**< Returns if the matrix is invertible. */

    TPRREbool         invert();          /**< Inverts the matrix. */
    const TPRREfloat* getInverse();      /**< Returns the matrix array in its inverted form without actually modifying the matrix. */

    void SetRotationX(TPRREfloat anglex);  /**< Sets the matrix to a rotation matrix around X-axis. */
    void SetRotationY(TPRREfloat angley);  /**< Sets the matrix to a rotation matrix around Y-axis. */
    void SetRotationZ(TPRREfloat anglez);  /**< Sets the matrix to a rotation matrix around Z-axis. */
    void SetTranslation(
        TPRREfloat x,
        TPRREfloat y,
        TPRREfloat z );                     /**< Sets the matrix to a translation matrix. */

    TPRREfloat getScaleX() const;           /**< Gets the scaling on the X-axis. */
    TPRREfloat getScaleY() const;           /**< Gets the scaling on the Y-axis. */
    TPRREfloat getScaleZ() const;           /**< Gets the scaling on the Z-axis. */
    void       SetScale(
        TPRREfloat x,
        TPRREfloat y,
        TPRREfloat z );                     /**< Sets the scalings on the 3 axes. */
    void       SetScaleX( TPRREfloat x );   /**< Sets the matrix to a scaling matrix only on the X-axis. */
    void       SetScaleY( TPRREfloat y );   /**< Sets the matrix to a scaling matrix only on the Y-axis. */
    void       SetScaleZ( TPRREfloat z );   /**< Sets the matrix to a scaling matrix only on the Z-axis. */

    void SetMirrorXY();   /**< Sets the matrix to a mirroring matrix on the XY planes. */
    void SetMirrorXZ();   /**< Sets the matrix to a mirroring matrix on the XZ planes. */
    void SetMirrorYZ();   /**< Sets the matrix to a mirroring matrix on the YZ planes. */

    void SetFrustum(float left, float right, float bottom, float top, float n, float f);   /**< Sets the matrix to represent a viewing frustum in world coordinate system. */
    void SetFrustumByFovX(float fovx, float aspect, float zNear, float zFar);              /**< Sets the matrix to represent a viewing frustum in world coordinate system. */
    void SetFrustumByFovY(float fovy, float aspect, float zNear, float zFar);              /**< Sets the matrix to represent a viewing frustum in world coordinate system. */

    void SetLookAt(const PRREVector& pos, const PRREVector& target, const PRREVector& up); /**< Sets the matrix to represent a view matrix in world coordinate system. */
    
    TPRREbool   operator==(const PRREMatrix& cm) const;      /**< Equals to operator. */
    TPRREbool   operator!=(const PRREMatrix& cm) const;      /**< Not equals to operator. */
    PRREMatrix& operator= (const PRREMatrix& cm);            /**< Assignment operator. */
    PRREMatrix  operator+ (const PRREMatrix& cm) const;      /**< Addition operator. */
    PRREMatrix& operator+=(const PRREMatrix& cm);            /**< Addition assignment operator. */
    PRREMatrix  operator- (const PRREMatrix& cm) const;      /**< Subtraction operator. */
    PRREMatrix& operator-=(const PRREMatrix& cm);            /**< Subtraction assignment operator. */
    PRREMatrix  operator* (const PRREMatrix& cm) const;      /**< Multiplication by matrix operator. */
    PRREMatrix& operator*=(const PRREMatrix& cm);            /**< Multiplication assignment operator. */
    PRREMatrix  operator* (const TPRREfloat& scalar) const;  /**< Multiplication by scalar operator. */
    PRREMatrix& operator*=(const TPRREfloat& scalar);        /**< Multiplication assignment operator. */
    PRREVector  operator* (const PRREVector& vec) const;     /**< Multiplication by vector operator. */
    TPRREfloat&       operator()(const TPRREbyte& row, const TPRREbyte& col);        /**< Access element operator operator. */
    const TPRREfloat& operator()(const TPRREbyte& row, const TPRREbyte& col) const;  /**< Access element operator operator. */

protected:

    // ---------------------------------------------------------------------------

private:
    static PRREMatrix matIdentity;   /**< Static zero matrix. */
    static PRREMatrix matZero;       /**< Static identity matrix. */

    // ---------------------------------------------------------------------------

    static TPRREbool correctIndices(TPRREbyte row, TPRREbyte col);                     /**< Are the given row and column indices correct? */
    static TPRREbyte ind(TPRREbyte row, TPRREbyte col);                                /**< Returns index of cell in given row and column. */

    static void      TransposeArray(TPRREfloat* arr, TPRREbyte rows, TPRREbyte cols);  /**< Treats the given array as a matrix and transposes it. */
    static TPRREbool invertArray(TPRREfloat arr[], TPRREbyte rows, TPRREbyte cols);    /**< Treats the given array as a matrix and inverts it. */

    // ---------------------------------------------------------------------------

    TPRREfloat mat[4][4];              /**< Values. */
    TPRREfloat tanspmat[4][4];         /**< Values when matrix is requested as transposed. */

}; // class PRREMatrix

PRREMatrix  operator- (const PRREMatrix& vec);                            /**< Negative operator. */
PRREMatrix  operator* (const TPRREfloat& scalar, const PRREMatrix& mat);  /**< Multiplication by scalar operator. */
PRREVector  operator* (const PRREVector& vec, const PRREMatrix& mat);     /**< Multiplication by vector operator. */
PRREVector& operator*=(PRREVector& vec, const PRREMatrix& mat);           /**< Multiplication assignment operator. */
