/*
    ###################################################################################
    PRREMatrix.cpp
    This file is part of PRRE.
    PRRE 3D-matrix class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRREMatrix.h"


// ############################### PUBLIC ################################


/**
    Gets a constant zero matrix.
*/
const PRREMatrix& PRREMatrix::getZero()
{
    return matZero;
} // getZero() static


/**
    Gets a constant identity matrix.
*/
const PRREMatrix& PRREMatrix::getIdentity()
{
    return matIdentity;
} // getIdentity() static


/**
    Creates an identity matrix.
*/
PRREMatrix::PRREMatrix()
{
    SetIdentity();
} // PRREMatrix()


PRREMatrix::PRREMatrix(const PRREMatrix& cm)
{
    ::memcpy(&mat, &(cm.mat), 16*sizeof(TPRREfloat));
} // PRREMatrix(...)


/**
    Creates the requested matrix.
*/
PRREMatrix::PRREMatrix(const TPRRE_NOTABLE_MATRICES& nm)
{
    switch (nm)
    {
    case PRRE_MATRIX_ZERO     : SetZero(); break;
    case PRRE_MATRIX_IDENTITY : SetIdentity(); break;
    }
} // PRREMatrix(...)


PRREMatrix::~PRREMatrix()
{

} // ~PRREMatrix()


/**
    Gets the element at the given row and column.
*/
TPRREfloat PRREMatrix::get(TPRREbyte row, TPRREbyte col) const
{
    return correctIndices(row, col) ? mat[row][col] : 0;
} // get()


/**
    Sets the element at the given row and column.
*/
void PRREMatrix::Set(TPRREbyte row, TPRREbyte col, TPRREfloat value)
{
    if ( correctIndices(row, col) )
    {
        mat[row][col] = value;
    }
} // Set()


/**
    Returns the matrix array as is.
*/
const TPRREfloat* PRREMatrix::get() const
{
    return (TPRREfloat*) mat;
}


/**
    Is it a zero matrix?
*/
TPRREbool PRREMatrix::isZero() const
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if ( (abs(mat[i][j]) >= PFL::E) && (i != 3) && (j != 3) ) return false;
    return true;
} // isZero()


/**
    Sets the matrix to zero matrix.
*/
void PRREMatrix::SetZero()
{
    memset(&mat, 0, 16*sizeof(TPRREfloat));
    mat[3][3] = 1.0f;
} // SetZero()


/**
    Is it an identity matrix?
*/
TPRREbool PRREMatrix::isIdentity() const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ( i == j )
            {
                if ( (mat[i][i] <= (1.0f-PFL::E)) || (mat[i][i] >= (1.0f+PFL::E)) )
                    return false;
            }
            else
            {
                if ( abs(mat[i][j]) >= PFL::E )
                    return false;
            }
        }
    }
    return true;
} // isIdentity()


/**
    Sets the matrix to identity matrix.
*/
void PRREMatrix::SetIdentity()
{
    SetZero();
    mat[0][0] = 1;
    mat[1][1] = 1;
    mat[2][2] = 1;
    mat[3][3] = 1;
} // SetIdentity()


/**
    Transposes the matrix.
*/
void PRREMatrix::Transpose()
{
    TransposeArray((TPRREfloat*)mat, 4, 4);
}


/**
    Returns the matrix in its transposed form without actually modifying the matrix.
*/
const TPRREfloat* PRREMatrix::getTransposed()
{
    ::memcpy(tanspmat, mat, 16*sizeof(TPRREfloat));
    TransposeArray((TPRREfloat*)tanspmat, 4, 4);
    return (TPRREfloat*) tanspmat;
}


/**
    Returns the determinant of the matrix.
    Determinant of the identity matrix is 1: det(I) = 1.
    Determinant of a matrix with any row or column having all elements equal to 0 is 0.
    Determinant of a transposed matrix equals to the determinant of the original matrix: det(transp(A)) = det(A).
    Determinant of an inverted matrix equals to the determinant of the original matrix: det(inverse(A)) = 1 / det(A).
*/
TPRREfloat PRREMatrix::getDeterminant() const
{
    /*
        https://hu.wikipedia.org/wiki/Determin%C3%A1ns#4%C3%974-es
        https://en.wikipedia.org/wiki/Determinant
        https://en.wikipedia.org/wiki/Laplace_expansion
        https://hu.wikipedia.org/wiki/Kifejt%C3%A9si_t%C3%A9tel
        http://www.mathwords.com/c/cofactor_matrix.htm
    */

    const TPRREfloat det2233 = mat[2][2]*mat[3][3] - mat[2][3]*mat[3][2];
    const TPRREfloat det2133 = mat[2][1]*mat[3][3] - mat[2][3]*mat[3][1];
    const TPRREfloat det2033 = mat[2][0]*mat[3][3] - mat[2][3]*mat[3][0];
    const TPRREfloat det2031 = mat[2][0]*mat[3][1] - mat[2][1]*mat[3][0];
    const TPRREfloat det2132 = mat[2][1]*mat[3][2] - mat[2][2]*mat[3][1];
    const TPRREfloat det2032 = mat[2][0]*mat[3][2] - mat[2][2]*mat[3][0];

    const TPRREfloat det00 = mat[1][1]*det2233 - mat[1][2]*det2133 + mat[1][3]*det2132;
    const TPRREfloat det01 = mat[1][0]*det2233 - mat[1][2]*det2033 + mat[1][3]*det2032;
    const TPRREfloat det02 = mat[1][0]*det2133 - mat[1][1]*det2033 + mat[1][3]*det2031;
    const TPRREfloat det03 = mat[1][0]*det2132 - mat[1][1]*det2032 + mat[1][2]*det2031;

    return mat[0][0]*det00 - mat[0][1]*det01 + mat[0][2]*det02 - mat[0][3]*det03;
}


/**
    Returns if the matrix is invertible.
    The matrix is invertible only if its determinant is non-zero.
    If determinant is zero, the matrix is not invertible, and then we call the matrix singular or degenerate.

    @return True if the matrix is invertible, false otherwise.
*/
TPRREbool PRREMatrix::isInvertible() const
{
    return (abs(getDeterminant()) >= PFL::E);
}


/**
    Inverts the matrix.
    Has no effect if the matrix in not invertible (when it is singular alias degenerate).
    The inverse of a translation by (tx, ty, tz) is a translation by (-tx, -ty, -tz).
    The inverse of a rotation by n degrees is a rotation by -n degrees.
    The inverse of a scale by (sx, sy, sz) in the direction of the X-Y-Z axes is a scale by (1/sx, 1/sy, 1/sz).

    @return True if inversion is successful, false otherwise.
*/
TPRREbool PRREMatrix::invert()
{
    return invertArray((TPRREfloat*)mat, 4, 4);    
}

/**
    Returns the matrix array in its inverted form without actually modifying the matrix.
    See more about matrix invert at invert().

    @return Pointer to matrix array in its inverted form if inversion is successful, NULL otherwise.
*/
const TPRREfloat* PRREMatrix::getInverse()
{
    ::memcpy(tanspmat, mat, 16*sizeof(TPRREfloat));
    return ( invertArray((TPRREfloat*)tanspmat, 4, 4) ) ? (TPRREfloat*) tanspmat : NULL;
}


/**
    Equals to operator.
*/
TPRREbool PRREMatrix::operator==(const PRREMatrix& cm) const 
{
    return ( memcmp(&mat, &(cm.mat), 16*sizeof(TPRREfloat)) == 0 );
} // operator==()


/**
    Not equals to operator.
*/
TPRREbool PRREMatrix::operator!=(const PRREMatrix& cm) const 
{
    return !( *this == cm );
} // operator!=()


/**
    Assignment operator.
*/
PRREMatrix& PRREMatrix::operator=(const PRREMatrix& cm)
{
    ::memcpy(&mat, &(cm.mat), 16*sizeof(TPRREfloat));
    return *this;
} // operator=()


/**
    Addition operator.
*/
PRREMatrix PRREMatrix::operator+(const PRREMatrix& cm) const
{
    PRREMatrix newmat;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            newmat.mat[i][j] = mat[i][j] + cm.mat[i][j];
    return newmat;
} // operator+()


/**
    Addition assignment operator.
*/
PRREMatrix& PRREMatrix::operator+=(const PRREMatrix& cm)
{                                                                                                                                          
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] += cm.mat[i][j];
    return *this;
} // operator+=()


/**
    Subtraction operator.
*/
PRREMatrix PRREMatrix::operator-(const PRREMatrix& cm) const
{
    PRREMatrix newmat;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            newmat.mat[i][j] = mat[i][j] - cm.mat[i][j];
    return newmat;
} // operator-()


/**
    Subtraction assignment operator.
*/
PRREMatrix& PRREMatrix::operator-=(const PRREMatrix& cm)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] -= cm.mat[i][j];
    return *this;
} // operator-=()


/**
    Multiplication by matrix operator.
*/
PRREMatrix PRREMatrix::operator*(const PRREMatrix& cm) const
{
    PRREMatrix newmat;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            TPRREfloat s = 0.0f;
            for (int k = 0; k < 4; k++)
                s += mat[i][k]*cm.mat[k][j];
            newmat.mat[i][j] = s;
        }
    }
    return newmat;
} // operator*()


/**
    Multiplication assignment operator.
*/
PRREMatrix& PRREMatrix::operator*=(const PRREMatrix& cm)
{
    TPRREfloat calculated[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            TPRREfloat s = 0.0f;
            for (int k = 0; k < 4; k++)
                s += mat[i][k]*cm.mat[k][j];
            calculated[i][j] = s;
        }
    }
    ::memcpy(&mat, &calculated, 16*sizeof(TPRREfloat));
    return *this;
} // operator*=()


/**
    Multiplication by scalar operator.
*/
PRREMatrix PRREMatrix::operator*(const TPRREfloat& scalar) const
{
    PRREMatrix newmat;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            newmat.mat[i][j] = mat[i][j] * scalar;
    return newmat;
} // operator*()


/**
    Multiplication assignment operator.
*/
PRREMatrix& PRREMatrix::operator*=(const TPRREfloat& scalar)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] *= scalar;
    return *this;
} // operator*=()


/**
    Multiplication by vector operator.
*/
PRREVector PRREMatrix::operator*(const PRREVector& vec) const
{
    PRREVector newvec;
    for (TPRREbyte i = 0; i < 4; i++)
    {
        TPRREfloat s = 0.0f;
        for (TPRREbyte k = 0; k < 4; k++)
            s += mat[i][k] * vec.get(k);
        newvec.Set(i, s);
    }
    return newvec;
} // operator*()


/**
    Access element operator operator.
*/
TPRREfloat& PRREMatrix::operator()(const TPRREbyte& row, const TPRREbyte& col)
{
    if ( correctIndices(row, col) )
    {
        return mat[row][col];
    }
    return mat[0][0];
} // operator()()


/**
    Access element operator operator.
*/
const TPRREfloat& PRREMatrix::operator()(const TPRREbyte& row, const TPRREbyte& col) const
{
    if ( correctIndices(row, col) )
    {
        return mat[row][col];
    }
    return mat[0][0];
} // operator()()


/**
    Negative operator operator.
*/
PRREMatrix operator-(const PRREMatrix& mat)
{
    return PRREMatrix(mat) * -1;
} // operator-()


/**
    Multiplication by scalar operator.
*/
PRREMatrix operator*(const TPRREfloat& scalar, const PRREMatrix& mat)
{
    return mat * scalar;
}


/**
    Multiplication by vector operator.
*/
PRREVector operator*(const PRREVector& vec, const PRREMatrix& mat)
{
    PRREVector newvec;
    for (TPRREbyte i = 0; i < 4; i++)
    {
        TPRREfloat s = 0.0f;
        for (TPRREbyte k = 0; k < 4; k++)
            s += mat.get(k,i) * vec.get(k);
        newvec.Set(i, s);
    }
    return newvec;
} // operator*()


/**
    Multiplication assignment operator.
*/
PRREVector& operator*=(PRREVector& vec, const PRREMatrix& mat)
{
    TPRREfloat calculated[4];
    for (TPRREbyte i = 0; i < 4; i++)
    {
        TPRREfloat s = 0.0f;
        for (TPRREbyte k = 0; k < 4; ++k)
            s += mat.get(k,i) * vec.get(k);
        calculated[i] = s;
    }
    for (TPRREbyte i = 0; i < 4; i++)
        vec.Set(i, calculated[i]);
    return vec;
} // operator*=()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRREMatrix PRREMatrix::matZero(PRRE_MATRIX_ZERO);            // Our static zero matrix.
PRREMatrix PRREMatrix::matIdentity(PRRE_MATRIX_IDENTITY);    // Our static identity matrix.


/**
    Are the given row and column indices correct?

    @return True if the given indices are both correct.
*/
TPRREbool PRREMatrix::correctIndices(TPRREbyte row, TPRREbyte col)
{
    return (row >= 0 && row < 4 && col >= 0 && col < 4);
} // correctIndices()


/**
    Returns index of cell in given row and column.
*/
TPRREbyte PRREMatrix::ind(TPRREbyte row, TPRREbyte col)
{
    return row*4 + col;
}


 /**
    Treats the given array as a matrix and transposes it.
 */
void PRREMatrix::TransposeArray(TPRREfloat* arr, TPRREbyte rows, TPRREbyte cols)
{
    // although we are in 4x4 matrix class, this is a generalized transpose function for matrix with arbitrary rows and cols
    const TPRREbyte maxdim = max(rows, cols);
    const TPRREbyte mindim = min(rows, cols);

    // no matter if rows or cols is bigger, we always let cols alias j run until max dimension since j is always bigger than i
    // j is always bigger than i to avoid extra swaps those silly transpose functions found on the internet are doing ...
    for (int i = 0; i < mindim; i++)
    {
        for (int j = i+1; j < maxdim; j++)
        {
            TPRREfloat tmp = arr[i*rows+j];
            arr[i*rows+j] = arr[j*rows+i];
            arr[j*rows+i] = tmp;
        }
    }

    // obviously rows and cols values are needed to be swapped after transpose, but we don't need to do it in this 4x4 case
}


/**
    Treats the given array as a matrix and inverts it.
    No effect on input array if matrix is not invertible.

    @return True if inversion is successful, false otherwise.
*/
TPRREbool PRREMatrix::invertArray(TPRREfloat* arr, TPRREbyte rows, TPRREbyte cols)
{
    // this invert implementation is just for rows = cols = 4
    if ( (rows != cols) || (rows != 4) )
        return false;

    /*
        https://en.wikipedia.org/wiki/Adjugate_matrix
        https://hu.wikipedia.org/wiki/Adjung%C3%A1lt_(m%C3%A1trixinvert%C3%A1l%C3%A1s)
        https://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/index.htm
        https://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/threeD/index.htm
        https://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
    */

    const TPRREfloat det2233 = arr[ind(2,2)]*arr[ind(3,3)] - arr[ind(2,3)]*arr[ind(3,2)];
    const TPRREfloat det2133 = arr[ind(2,1)]*arr[ind(3,3)] - arr[ind(2,3)]*arr[ind(3,1)];
    const TPRREfloat det2033 = arr[ind(2,0)]*arr[ind(3,3)] - arr[ind(2,3)]*arr[ind(3,0)];
    const TPRREfloat det2031 = arr[ind(2,0)]*arr[ind(3,1)] - arr[ind(2,1)]*arr[ind(3,0)];
    const TPRREfloat det2132 = arr[ind(2,1)]*arr[ind(3,2)] - arr[ind(2,2)]*arr[ind(3,1)];
    const TPRREfloat det2032 = arr[ind(2,0)]*arr[ind(3,2)] - arr[ind(2,2)]*arr[ind(3,0)];

    // minors for row 0
    const TPRREfloat det00 = arr[ind(1,1)]*det2233 - arr[ind(1,2)]*det2133 + arr[ind(1,3)]*det2132;
    const TPRREfloat det01 = arr[ind(1,0)]*det2233 - arr[ind(1,2)]*det2033 + arr[ind(1,3)]*det2032;
    const TPRREfloat det02 = arr[ind(1,0)]*det2133 - arr[ind(1,1)]*det2033 + arr[ind(1,3)]*det2031;
    const TPRREfloat det03 = arr[ind(1,0)]*det2132 - arr[ind(1,1)]*det2032 + arr[ind(1,2)]*det2031;

    // above lines are from getDeterminant()
    const TPRREfloat det = arr[ind(0,0)]*det00 - arr[ind(0,1)]*det01 + arr[ind(0,2)]*det02 - arr[ind(0,3)]*det03;

    // check if matrix is singular
    if ( abs(det) < PFL::E )
        return false;  // cannot divive by det

    // following lines continue calculating minors for next rows exactly same way we calculated above for row0 ...

    // minors for row 1
    const TPRREfloat det10 = arr[ind(0,1)]*det2233 - arr[ind(0,2)]*det2133 + arr[ind(0,3)]*det2132;
    const TPRREfloat det11 = arr[ind(0,0)]*det2233 - arr[ind(0,2)]*det2033 + arr[ind(0,3)]*det2032;
    const TPRREfloat det12 = arr[ind(0,0)]*det2133 - arr[ind(0,1)]*det2033 + arr[ind(0,3)]*det2031;
    const TPRREfloat det13 = arr[ind(0,0)]*det2132 - arr[ind(0,1)]*det2032 + arr[ind(0,2)]*det2031;

    const TPRREfloat det1233 = arr[ind(1,2)]*arr[ind(3,3)] - arr[ind(1,3)]*arr[ind(3,2)];
    const TPRREfloat det1133 = arr[ind(1,1)]*arr[ind(3,3)] - arr[ind(1,3)]*arr[ind(3,1)];
    const TPRREfloat det1033 = arr[ind(1,0)]*arr[ind(3,3)] - arr[ind(1,3)]*arr[ind(3,0)];
    const TPRREfloat det1132 = arr[ind(1,1)]*arr[ind(3,2)] - arr[ind(1,2)]*arr[ind(3,1)];
    const TPRREfloat det1032 = arr[ind(1,0)]*arr[ind(3,2)] - arr[ind(1,2)]*arr[ind(3,0)];
    const TPRREfloat det1031 = arr[ind(1,0)]*arr[ind(3,1)] - arr[ind(1,1)]*arr[ind(3,0)];

    // minors for row 2
    const TPRREfloat det20 = arr[ind(0,1)]*det1233 - arr[ind(0,2)]*det1133 + arr[ind(0,3)]*det1132;
    const TPRREfloat det21 = arr[ind(0,0)]*det1233 - arr[ind(0,2)]*det1033 + arr[ind(0,3)]*det1032;
    const TPRREfloat det22 = arr[ind(0,0)]*det1133 - arr[ind(0,1)]*det1033 + arr[ind(0,3)]*det1031;
    const TPRREfloat det23 = arr[ind(0,0)]*det1132 - arr[ind(0,1)]*det1032 + arr[ind(0,2)]*det1031;

    const TPRREfloat det1223 = arr[ind(1,2)]*arr[ind(2,3)] - arr[ind(1,3)]*arr[ind(2,2)];
    const TPRREfloat det1123 = arr[ind(1,1)]*arr[ind(2,3)] - arr[ind(1,3)]*arr[ind(2,1)];
    const TPRREfloat det1122 = arr[ind(1,1)]*arr[ind(2,2)] - arr[ind(1,2)]*arr[ind(2,1)];
    const TPRREfloat det1023 = arr[ind(1,0)]*arr[ind(2,3)] - arr[ind(1,3)]*arr[ind(2,0)];
    const TPRREfloat det1022 = arr[ind(1,0)]*arr[ind(2,2)] - arr[ind(1,2)]*arr[ind(2,0)];
    const TPRREfloat det1021 = arr[ind(1,0)]*arr[ind(2,1)] - arr[ind(1,1)]*arr[ind(2,0)];

    // minors for row 3
    const TPRREfloat det30 = arr[ind(0,1)]*det1223 - arr[ind(0,2)]*det1123 + arr[ind(0,3)]*det1122;
    const TPRREfloat det31 = arr[ind(0,0)]*det1223 - arr[ind(0,2)]*det1023 + arr[ind(0,3)]*det1022;
    const TPRREfloat det32 = arr[ind(0,0)]*det1123 - arr[ind(0,1)]*det1023 + arr[ind(0,3)]*det1021;
    const TPRREfloat det33 = arr[ind(0,0)]*det1122 - arr[ind(0,1)]*det1022 + arr[ind(0,2)]*det1021;

    // now we know all minors, we need to build the adjugate matrix and simply divide by det
    // adjugate matrix is the transposed form of the cofactor matrix
    // cofactor matrix consists of signed minors
    // we build it now right into the original array

    arr[ind(0,0)] =  det00 / det;  arr[ind(0,1)] = -det10 / det;  arr[ind(0,2)] =  det20 / det;  arr[ind(0,3)] = -det30 / det;
    arr[ind(1,0)] = -det01 / det;  arr[ind(1,1)] =  det11 / det;  arr[ind(1,2)] = -det21 / det;  arr[ind(1,3)] =  det31 / det;
    arr[ind(2,0)] =  det02 / det;  arr[ind(2,1)] = -det12 / det;  arr[ind(2,2)] =  det22 / det;  arr[ind(2,3)] = -det32 / det;
    arr[ind(3,0)] = -det03 / det;  arr[ind(3,1)] =  det13 / det;  arr[ind(3,2)] = -det23 / det;  arr[ind(3,3)] =  det33 / det;

    return true;
}

