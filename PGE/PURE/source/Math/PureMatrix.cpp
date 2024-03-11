/*
    ###################################################################################
    PureMatrix.cpp
    This file is part of PURE.
    Pure 3D-matrix class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Math/PureMatrix.h"
#include "../../include/internal/PurePragmas.h"


// ############################### PUBLIC ################################


/**
    Gets a constant zero matrix.
*/
const PureMatrix& PureMatrix::getZero()
{
    return matZero;
} // getZero() static


/**
    Gets a constant identity matrix.
*/
const PureMatrix& PureMatrix::getIdentity()
{
    return matIdentity;
} // getIdentity() static


/**
    Creates an identity matrix.
*/
PureMatrix::PureMatrix()
{
    SetIdentity();
} // PureMatrix()


PureMatrix::PureMatrix(const PureMatrix& cm)
{
    ::memcpy(&mat, &(cm.mat), 16*sizeof(TPureFloat));
} // PureMatrix(...)


/**
    Creates the requested matrix.
*/
PureMatrix::PureMatrix(const TPURE_NOTABLE_MATRICES& nm)
{
    switch (nm)
    {
    case PURE_MATRIX_ZERO     : SetZero(); break;
    case PURE_MATRIX_IDENTITY : SetIdentity(); break;
    }
} // PureMatrix(...)


PureMatrix::~PureMatrix()
{

} // ~PureMatrix()


/**
    Gets the element at the given row and column.
*/
TPureFloat PureMatrix::get(TPureByte row, TPureByte col) const
{
    return correctIndices(row, col) ? mat[row][col] : 0;
} // get()


/**
    Sets the element at the given row and column.
*/
void PureMatrix::Set(TPureByte row, TPureByte col, TPureFloat value)
{
    if ( correctIndices(row, col) )
    {
        mat[row][col] = value;
    }
} // Set()


/**
    Returns the matrix array as is.
*/
const TPureFloat* PureMatrix::get() const
{
    return (TPureFloat*) mat;
}


/**
    Is it a zero matrix?
*/
TPureBool PureMatrix::isZero() const
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if ( (abs(mat[i][j]) >= PFL::E) && (i != 3) && (j != 3) ) return false;
    return true;
} // isZero()


/**
    Sets the matrix to zero matrix.
*/
void PureMatrix::SetZero()
{
    memset(&mat, 0, 16*sizeof(TPureFloat));
    mat[3][3] = 1.0f;
} // SetZero()


/**
    Is it an identity matrix?
*/
TPureBool PureMatrix::isIdentity() const
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
void PureMatrix::SetIdentity()
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
void PureMatrix::Transpose()
{
    TransposeArray((TPureFloat*)mat, 4, 4);
}


/**
    Returns the matrix in its transposed form without actually modifying the matrix.
*/
const TPureFloat* PureMatrix::getTransposed()
{
    ::memcpy(tanspmat, mat, 16*sizeof(TPureFloat));
    TransposeArray((TPureFloat*)tanspmat, 4, 4);
    return (TPureFloat*) tanspmat;
}


/**
    Returns the determinant of the matrix.
    Determinant of the identity matrix is 1: det(I) = 1.
    Determinant of a matrix with any row or column having all elements equal to 0 is 0.
    Determinant of a transposed matrix equals to the determinant of the original matrix: det(transp(A)) = det(A).
    Determinant of an inverted matrix equals to the determinant of the original matrix: det(inverse(A)) = 1 / det(A).
*/
TPureFloat PureMatrix::getDeterminant() const
{
    /*
        https://hu.wikipedia.org/wiki/Determin%C3%A1ns#4%C3%974-es
        https://en.wikipedia.org/wiki/Determinant
        https://en.wikipedia.org/wiki/Laplace_expansion
        https://hu.wikipedia.org/wiki/Kifejt%C3%A9si_t%C3%A9tel
        http://www.mathwords.com/c/cofactor_matrix.htm
    */

    const TPureFloat det2233 = mat[2][2]*mat[3][3] - mat[2][3]*mat[3][2];
    const TPureFloat det2133 = mat[2][1]*mat[3][3] - mat[2][3]*mat[3][1];
    const TPureFloat det2033 = mat[2][0]*mat[3][3] - mat[2][3]*mat[3][0];
    const TPureFloat det2031 = mat[2][0]*mat[3][1] - mat[2][1]*mat[3][0];
    const TPureFloat det2132 = mat[2][1]*mat[3][2] - mat[2][2]*mat[3][1];
    const TPureFloat det2032 = mat[2][0]*mat[3][2] - mat[2][2]*mat[3][0];

    const TPureFloat det00 = mat[1][1]*det2233 - mat[1][2]*det2133 + mat[1][3]*det2132;
    const TPureFloat det01 = mat[1][0]*det2233 - mat[1][2]*det2033 + mat[1][3]*det2032;
    const TPureFloat det02 = mat[1][0]*det2133 - mat[1][1]*det2033 + mat[1][3]*det2031;
    const TPureFloat det03 = mat[1][0]*det2132 - mat[1][1]*det2032 + mat[1][2]*det2031;

    return mat[0][0]*det00 - mat[0][1]*det01 + mat[0][2]*det02 - mat[0][3]*det03;
}


/**
    Returns if the matrix is invertible.
    The matrix is invertible only if its determinant is non-zero.
    If determinant is zero, the matrix is not invertible, and then we call the matrix singular or degenerate.

    @return True if the matrix is invertible, false otherwise.
*/
TPureBool PureMatrix::isInvertible() const
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
TPureBool PureMatrix::invert()
{
    return invertArray((TPureFloat*)mat, 4, 4);    
}

/**
    Returns the matrix array in its inverted form without actually modifying the matrix.
    See more about matrix invert at invert().

    @return Pointer to matrix array in its inverted form if inversion is successful, NULL otherwise.
*/
const TPureFloat* PureMatrix::getInverse()
{
    ::memcpy(tanspmat, mat, 16*sizeof(TPureFloat));
    return ( invertArray((TPureFloat*)tanspmat, 4, 4) ) ? (TPureFloat*) tanspmat : NULL;
}


/**
    Equals to operator.
*/
TPureBool PureMatrix::operator==(const PureMatrix& cm) const 
{
    return ( memcmp(&mat, &(cm.mat), 16*sizeof(TPureFloat)) == 0 );
} // operator==()


/**
    Not equals to operator.
*/
TPureBool PureMatrix::operator!=(const PureMatrix& cm) const 
{
    return !( *this == cm );
} // operator!=()


/**
    Assignment operator.
*/
PureMatrix& PureMatrix::operator=(const PureMatrix& cm)
{
    ::memcpy(&mat, &(cm.mat), 16*sizeof(TPureFloat));
    return *this;
} // operator=()


/**
    Addition operator.
*/
PureMatrix PureMatrix::operator+(const PureMatrix& cm) const
{
    PureMatrix newmat;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            newmat.mat[i][j] = mat[i][j] + cm.mat[i][j];
    return newmat;
} // operator+()


/**
    Addition assignment operator.
*/
PureMatrix& PureMatrix::operator+=(const PureMatrix& cm)
{                                                                                                                                          
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] += cm.mat[i][j];
    return *this;
} // operator+=()


/**
    Subtraction operator.
*/
PureMatrix PureMatrix::operator-(const PureMatrix& cm) const
{
    PureMatrix newmat;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            newmat.mat[i][j] = mat[i][j] - cm.mat[i][j];
    return newmat;
} // operator-()


/**
    Subtraction assignment operator.
*/
PureMatrix& PureMatrix::operator-=(const PureMatrix& cm)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] -= cm.mat[i][j];
    return *this;
} // operator-=()


/**
    Multiplication by matrix operator.
*/
PureMatrix PureMatrix::operator*(const PureMatrix& cm) const
{
    PureMatrix newmat;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            TPureFloat s = 0.0f;
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
PureMatrix& PureMatrix::operator*=(const PureMatrix& cm)
{
    TPureFloat calculated[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            TPureFloat s = 0.0f;
            for (int k = 0; k < 4; k++)
                s += mat[i][k]*cm.mat[k][j];
            calculated[i][j] = s;
        }
    }
    ::memcpy(&mat, &calculated, 16*sizeof(TPureFloat));
    return *this;
} // operator*=()


/**
    Multiplication by scalar operator.
*/
PureMatrix PureMatrix::operator*(const TPureFloat& scalar) const
{
    PureMatrix newmat;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            newmat.mat[i][j] = mat[i][j] * scalar;
    return newmat;
} // operator*()


/**
    Multiplication assignment operator.
*/
PureMatrix& PureMatrix::operator*=(const TPureFloat& scalar)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mat[i][j] *= scalar;
    return *this;
} // operator*=()


/**
    Multiplication by vector operator.
*/
PureVector PureMatrix::operator*(const PureVector& vec) const
{
    PureVector newvec;
    for (TPureByte i = 0; i < 4; i++)
    {
        TPureFloat s = 0.0f;
        for (TPureByte k = 0; k < 4; k++)
            s += mat[i][k] * vec.get(k);
        newvec.Set(i, s);
    }
    return newvec;
} // operator*()


/**
    Access element operator operator.
*/
TPureFloat& PureMatrix::operator()(const TPureByte& row, const TPureByte& col)
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
const TPureFloat& PureMatrix::operator()(const TPureByte& row, const TPureByte& col) const
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
PureMatrix operator-(const PureMatrix& mat)
{
    return PureMatrix(mat) * -1;
} // operator-()


/**
    Multiplication by scalar operator.
*/
PureMatrix operator*(const TPureFloat& scalar, const PureMatrix& mat)
{
    return mat * scalar;
}


/**
    Multiplication by vector operator.
*/
PureVector operator*(const PureVector& vec, const PureMatrix& mat)
{
    PureVector newvec;
    for (TPureByte i = 0; i < 4; i++)
    {
        TPureFloat s = 0.0f;
        for (TPureByte k = 0; k < 4; k++)
            s += mat.get(k,i) * vec.get(k);
        newvec.Set(i, s);
    }
    return newvec;
} // operator*()


/**
    Multiplication assignment operator.
*/
PureVector& operator*=(PureVector& vec, const PureMatrix& mat)
{
    TPureFloat calculated[4];
    for (TPureByte i = 0; i < 4; i++)
    {
        TPureFloat s = 0.0f;
        for (TPureByte k = 0; k < 4; ++k)
            s += mat.get(k,i) * vec.get(k);
        calculated[i] = s;
    }
    for (TPureByte i = 0; i < 4; i++)
        vec.Set(i, calculated[i]);
    return vec;
} // operator*=()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PureMatrix PureMatrix::matZero(PURE_MATRIX_ZERO);            // Our static zero matrix.
PureMatrix PureMatrix::matIdentity(PURE_MATRIX_IDENTITY);    // Our static identity matrix.


/**
    Are the given row and column indices correct?

    @return True if the given indices are both correct.
*/
TPureBool PureMatrix::correctIndices(TPureByte row, TPureByte col)
{
    return (row >= 0 && row < 4 && col >= 0 && col < 4);
} // correctIndices()


/**
    Returns index of cell in given row and column.
*/
TPureByte PureMatrix::ind(TPureByte row, TPureByte col)
{
    return row*4 + col;
}


 /**
    Treats the given array as a matrix and transposes it.
 */
void PureMatrix::TransposeArray(TPureFloat* arr, TPureByte rows, TPureByte cols)
{
    // although we are in 4x4 matrix class, this is a generalized transpose function for matrix with arbitrary rows and cols
    const TPureByte maxdim = std::max(rows, cols);
    const TPureByte mindim = std::min(rows, cols);

    // no matter if rows or cols is bigger, we always let cols alias j run until max dimension since j is always bigger than i
    // j is always bigger than i to avoid extra swaps those silly transpose functions found on the internet are doing ...
    for (int i = 0; i < mindim; i++)
    {
        for (int j = i+1; j < maxdim; j++)
        {
            TPureFloat tmp = arr[i*rows+j];
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
TPureBool PureMatrix::invertArray(TPureFloat* arr, TPureByte rows, TPureByte cols)
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

    const TPureFloat det2233 = arr[ind(2,2)]*arr[ind(3,3)] - arr[ind(2,3)]*arr[ind(3,2)];
    const TPureFloat det2133 = arr[ind(2,1)]*arr[ind(3,3)] - arr[ind(2,3)]*arr[ind(3,1)];
    const TPureFloat det2033 = arr[ind(2,0)]*arr[ind(3,3)] - arr[ind(2,3)]*arr[ind(3,0)];
    const TPureFloat det2031 = arr[ind(2,0)]*arr[ind(3,1)] - arr[ind(2,1)]*arr[ind(3,0)];
    const TPureFloat det2132 = arr[ind(2,1)]*arr[ind(3,2)] - arr[ind(2,2)]*arr[ind(3,1)];
    const TPureFloat det2032 = arr[ind(2,0)]*arr[ind(3,2)] - arr[ind(2,2)]*arr[ind(3,0)];

    // minors for row 0
    const TPureFloat det00 = arr[ind(1,1)]*det2233 - arr[ind(1,2)]*det2133 + arr[ind(1,3)]*det2132;
    const TPureFloat det01 = arr[ind(1,0)]*det2233 - arr[ind(1,2)]*det2033 + arr[ind(1,3)]*det2032;
    const TPureFloat det02 = arr[ind(1,0)]*det2133 - arr[ind(1,1)]*det2033 + arr[ind(1,3)]*det2031;
    const TPureFloat det03 = arr[ind(1,0)]*det2132 - arr[ind(1,1)]*det2032 + arr[ind(1,2)]*det2031;

    // above lines are from getDeterminant()
    const TPureFloat det = arr[ind(0,0)]*det00 - arr[ind(0,1)]*det01 + arr[ind(0,2)]*det02 - arr[ind(0,3)]*det03;

    // check if matrix is singular
    if ( abs(det) < PFL::E )
        return false;  // cannot divive by det

    // following lines continue calculating minors for next rows exactly same way we calculated above for row0 ...

    // minors for row 1
    const TPureFloat det10 = arr[ind(0,1)]*det2233 - arr[ind(0,2)]*det2133 + arr[ind(0,3)]*det2132;
    const TPureFloat det11 = arr[ind(0,0)]*det2233 - arr[ind(0,2)]*det2033 + arr[ind(0,3)]*det2032;
    const TPureFloat det12 = arr[ind(0,0)]*det2133 - arr[ind(0,1)]*det2033 + arr[ind(0,3)]*det2031;
    const TPureFloat det13 = arr[ind(0,0)]*det2132 - arr[ind(0,1)]*det2032 + arr[ind(0,2)]*det2031;

    const TPureFloat det1233 = arr[ind(1,2)]*arr[ind(3,3)] - arr[ind(1,3)]*arr[ind(3,2)];
    const TPureFloat det1133 = arr[ind(1,1)]*arr[ind(3,3)] - arr[ind(1,3)]*arr[ind(3,1)];
    const TPureFloat det1033 = arr[ind(1,0)]*arr[ind(3,3)] - arr[ind(1,3)]*arr[ind(3,0)];
    const TPureFloat det1132 = arr[ind(1,1)]*arr[ind(3,2)] - arr[ind(1,2)]*arr[ind(3,1)];
    const TPureFloat det1032 = arr[ind(1,0)]*arr[ind(3,2)] - arr[ind(1,2)]*arr[ind(3,0)];
    const TPureFloat det1031 = arr[ind(1,0)]*arr[ind(3,1)] - arr[ind(1,1)]*arr[ind(3,0)];

    // minors for row 2
    const TPureFloat det20 = arr[ind(0,1)]*det1233 - arr[ind(0,2)]*det1133 + arr[ind(0,3)]*det1132;
    const TPureFloat det21 = arr[ind(0,0)]*det1233 - arr[ind(0,2)]*det1033 + arr[ind(0,3)]*det1032;
    const TPureFloat det22 = arr[ind(0,0)]*det1133 - arr[ind(0,1)]*det1033 + arr[ind(0,3)]*det1031;
    const TPureFloat det23 = arr[ind(0,0)]*det1132 - arr[ind(0,1)]*det1032 + arr[ind(0,2)]*det1031;

    const TPureFloat det1223 = arr[ind(1,2)]*arr[ind(2,3)] - arr[ind(1,3)]*arr[ind(2,2)];
    const TPureFloat det1123 = arr[ind(1,1)]*arr[ind(2,3)] - arr[ind(1,3)]*arr[ind(2,1)];
    const TPureFloat det1122 = arr[ind(1,1)]*arr[ind(2,2)] - arr[ind(1,2)]*arr[ind(2,1)];
    const TPureFloat det1023 = arr[ind(1,0)]*arr[ind(2,3)] - arr[ind(1,3)]*arr[ind(2,0)];
    const TPureFloat det1022 = arr[ind(1,0)]*arr[ind(2,2)] - arr[ind(1,2)]*arr[ind(2,0)];
    const TPureFloat det1021 = arr[ind(1,0)]*arr[ind(2,1)] - arr[ind(1,1)]*arr[ind(2,0)];

    // minors for row 3
    const TPureFloat det30 = arr[ind(0,1)]*det1223 - arr[ind(0,2)]*det1123 + arr[ind(0,3)]*det1122;
    const TPureFloat det31 = arr[ind(0,0)]*det1223 - arr[ind(0,2)]*det1023 + arr[ind(0,3)]*det1022;
    const TPureFloat det32 = arr[ind(0,0)]*det1123 - arr[ind(0,1)]*det1023 + arr[ind(0,3)]*det1021;
    const TPureFloat det33 = arr[ind(0,0)]*det1122 - arr[ind(0,1)]*det1022 + arr[ind(0,2)]*det1021;

    // now we know all minors, we need to build the adjugate matrix and simply divide by det;
    // adjugate matrix is the transposed form of the cofactor matrix;
    // cofactor matrix consists of signed minors;
    // we build it now right into the original array

    arr[ind(0,0)] =  det00 / det;  arr[ind(0,1)] = -det10 / det;  arr[ind(0,2)] =  det20 / det;  arr[ind(0,3)] = -det30 / det;
    arr[ind(1,0)] = -det01 / det;  arr[ind(1,1)] =  det11 / det;  arr[ind(1,2)] = -det21 / det;  arr[ind(1,3)] =  det31 / det;
    arr[ind(2,0)] =  det02 / det;  arr[ind(2,1)] = -det12 / det;  arr[ind(2,2)] =  det22 / det;  arr[ind(2,3)] = -det32 / det;
    arr[ind(3,0)] = -det03 / det;  arr[ind(3,1)] =  det13 / det;  arr[ind(3,2)] = -det23 / det;  arr[ind(3,3)] =  det33 / det;

    return true;
}

