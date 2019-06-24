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
    Sets the matrix to a rotation matrix around X-axis.
*/
void PRREMatrix::SetRotationX(TPRREfloat anglex)
{
    SetZero();
    mat[0][0] = 1;
    mat[1][1] = cos( PFL::degToRad(anglex) );
    mat[2][2] = mat[1][1];
    mat[2][1] = sin( PFL::degToRad(anglex) );
    mat[1][2] = -(mat[2][1]);
    mat[3][3] = 1;
} // SetRotationX()


/**
    Sets the matrix to a rotation matrix around Y-axis.
*/
void PRREMatrix::SetRotationY(TPRREfloat angley)
{
    SetZero();
    mat[0][0] = cos( PFL::degToRad(angley) );
    mat[2][0] = sin( PFL::degToRad(angley) );
    mat[0][2] = -(mat[2][0]);
    mat[2][2] = mat[0][0];
    mat[1][1] = 1;
    mat[3][3] = 1;
} // SetRotationY()


/**
    Sets the matrix to a rotation matrix around Z-axis.
*/
void PRREMatrix::SetRotationZ(TPRREfloat anglez)
{
    SetZero();
    mat[0][0] = cos( PFL::degToRad(anglez) );
    mat[1][0] = sin( PFL::degToRad(anglez) );
    mat[0][1] = -(mat[1][0]);
    mat[1][1] = mat[0][0];
    mat[2][2] = 1;
    mat[3][3] = 1;
} // SetRotationZ()


/**
    Sets the matrix to a translation matrix.
*/
void PRREMatrix::SetTranslation(TPRREfloat x, TPRREfloat y, TPRREfloat z)
{
    SetIdentity();
    mat[0][3] = x;
    mat[1][3] = y;
    mat[2][3] = z;
} // SetTranslation()


/**
    Gets the scaling on the X-axis.
*/
TPRREfloat PRREMatrix::getScaleX() const
{
    return mat[0][0];
} // getScaleX()


/**
    Gets the scaling on the Y-axis.
*/
TPRREfloat PRREMatrix::getScaleY() const
{
    return mat[1][1];
} // getScaleY()


/**
    Gets the scaling on the Z-axis.
*/
TPRREfloat PRREMatrix::getScaleZ() const
{
    return mat[2][2];
} // getScaleZ()


/**
    Sets the scalings on the 3 axes.
*/
void PRREMatrix::SetScale(TPRREfloat x, TPRREfloat y, TPRREfloat z)
{
    SetZero();
    mat[0][0] = x;
    mat[1][1] = y;
    mat[2][2] = z;
    mat[3][3] = 1.0f;
} // SetScale()


/**
    Sets the matrix to a scaling matrix only on the X-axis.
*/
void PRREMatrix::SetScaleX(TPRREfloat x)
{
    SetScale(x, 1.0f, 1.0f);
} // SetScaleX()


/**
    Sets the matrix to a scaling matrix only on the Y-axis.
*/
void PRREMatrix::SetScaleY(TPRREfloat y) 
{
    SetScale(1.0f, y, 1.0f);
} // SetScaleY()


/**
    Sets the matrix to a scaling matrix only on the Z-axis.
*/
void PRREMatrix::SetScaleZ(TPRREfloat z)
{
    SetScale(1.0f, 1.0f, z);
} // SetScaleZ()


/**
    Sets the matrix to a mirroring matrix on the XY planes.
*/
void PRREMatrix::SetMirrorXY()
{
    SetIdentity();
    mat[2][2] = -1.0f;
} // SetMirrorXY()


/**
    Sets the matrix to a mirroring matrix on the XZ planes.
*/
void PRREMatrix::SetMirrorXZ()
{
    SetIdentity();
    mat[1][1] = -1.0f;
} // SetMirrorXZ()


/**
    Sets the matrix to a mirroring matrix on the YZ planes.
*/
void PRREMatrix::SetMirrorYZ()
{
    SetIdentity();
    mat[0][0] = -1.0f;
} // SetMirrorYZ()


/**
    Sets the matrix to represent a viewing frustum in world coordinate system.
    It can be used to produce perspective projection, typically from eye space to clip space.
    Assuming that the eye is located at (0, 0, 0), following statements we can make:
     - point specified by (left, bottom, n) on the near clipping plane is mapped to the lower left corner of the window;
     - point specified by (right, top, n) on the near clipping plane is mapped to the upper right corner of the window.

    Depth buffer precision is affected by the values specified for n and f. The greater the ratio of f to n is, the less effective the depth buffer will be at distinguishing between surfaces that are near each other.

    @param left   Left clipping plane distance to the viewer in eye space on X direction.
    @param right  Right clipping plane distance to the viewer in eye space on X direction. Must be greater than left.
    @param bottom Bottom clipping plane distance to the viewer in eye space on Y direction.
    @param top    Top clipping plane distance to the viewer in eye space on Y direction. Must be greater than bottom.
    @param n      Near clipping plane distance to the viewer in eye space on Z direction. Must be greater than 0.
    @param f      Far clipping plane distance to the viewer in eye space on Z direction. Must be greater than n.
*/
void PRREMatrix::SetFrustum(float left, float right, float bottom, float top, float n, float f)
{
    if ( (n <= 0.0f) || (n >= f) )
        return;

    if ( (right <= left) || (top <= bottom) )
        return;

    SetZero();

    /* 1st row */
    mat[0][0] = 2*n / (right-left);
    mat[0][2] = (right+left) / (right-left);

    /* 2nd row */
    mat[1][1] = 2*n / (top-bottom);
    mat[1][2] = (top+bottom) / (top-bottom);

    /* 3rd row */
    mat[2][2] = (f+n) / (f-n);
    mat[2][3] = -2*f*n / (f-n);

    /* 4th row */
    mat[3][2] = 1.0f;
    mat[3][3] = 0.0f; /* because SetZero had set it as 1 */
}


/**
    Sets the matrix to represent a viewing frustum in world coordinate system.
    It can be used to produce perspective projection, typically from eye space to clip space.

    Depth buffer precision is affected by the values specified for zNear and zFar. The greater the ratio of zFar to zNear is, the less effective the depth buffer will be at distinguishing between surfaces that are near each other.

    @param fovx   The field of vew angle in X direction. Must be greater than 0.
    @param aspect Specifies the aspect ratio that determines the field of view in the X direction. Typically the aspect ratio is the ratio of viewport width to viewport height. Must be greater than 0.
    @param zNear  Near clipping plane distance to the viewer in eye space on Z direction. Must be greater than 0.
    @param zFar   Far clipping plane distance to the viewer in eye space on Z direction. Must be greater than zNear.
*/
void PRREMatrix::SetFrustumByFovX(float fovx, float aspect, float zNear, float zFar)
{
    if ( (fovx <= 0.0f) || (aspect <= 0.0f) || (zNear <= 0.0f) || (zNear >= zFar) )
        return;

    fovx = fovx * PFL::PI/180;

    const float left  = -(zNear * tan(fovx / 2));  /* left needs to be negative for sure */
    const float right = -left;

    const float top    = right / aspect;
    const float bottom = -top;

    SetFrustum(left, right, bottom, top, zNear, zFar);
}


// TODO: implement this in Transformations class
/**
    Sets the matrix to represent a viewing frustum in world coordinate system.
    It can be used to produce perspective projection, typically from eye space to clip space.

    Depth buffer precision is affected by the values specified for zNear and zFar. The greater the ratio of zFar to zNear is, the less effective the depth buffer will be at distinguishing between surfaces that are near each other.

    @param fovy   The field of vew angle in Y direction. Must be greater than 0.
    @param aspect Specifies the aspect ratio that determines the field of view in the X direction. Typically the aspect ratio is the ratio of viewport width to viewport height. Must be greater than 0.
    @param zNear  Near clipping plane distance to the viewer in eye space on Z direction. Must be greater than 0.
    @param zFar   Far clipping plane distance to the viewer in eye space on Z direction. Must be greater than zNear.
*/
void PRREMatrix::SetFrustumByFovY(float fovy, float aspect, float zNear, float zFar)
{
    if ( (fovy <= 0.0f) || (aspect <= 0.0f) || (zNear <= 0.0f) || (zNear >= zFar) )
        return;

    fovy = fovy * PFL::PI/180;

    const float top    = zNear * tan(fovy / 2);
    const float bottom = -top;

    const float right = top * aspect;
    const float left  = -right;

    SetFrustum(left, right, bottom, top, zNear, zFar);
}


/**
    Sets the matrix to represent a view matrix in world coordinate system.
    Transforms everything based on camera position and direction.
    
    @param vPos    Eye position.
    @param vTarget Where we are looking at.
    @param vUp     An Up vector representing positive direction on Y-angle, usually the camera's Up vector.
*/
void PRREMatrix::SetLookAt(const PRREVector& vPos,
    const PRREVector& vTarget,
    const PRREVector& vUp)
{
    /*

        My implementation doesn't equal to any of these:
        https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function
        https://www.khronos.org/opengl/wiki/GluLookAt_code
        https://github.com/g-truc/glm/blob/master/glm/gtc/matrix_transform.inl#L756

        And above implementations differ from each other a bit. But still good material to read and learn.
    */

    PRREVector vForwardFromTargetAndPos = vTarget - vPos;
    vForwardFromTargetAndPos.Normalize();

    PRREVector vAltUp = vUp;
   
    // this is to detect when our calculated forward vector is parallel with Y-axis and that would make cross product zero right vector ...
    // todo: switch to quaternions
    if ( (abs(vForwardFromTargetAndPos.getX()) < PFL::E) && (abs(vForwardFromTargetAndPos.getZ()) < PFL::E) )
    {
        if ( vForwardFromTargetAndPos.getY() >= 0.0f )
            vAltUp.Set(0.0f, 0.0f, 1.0f);
        else
            vAltUp.Set(0.0f, 0.0f, -1.0f);
    }

    PRREVector vRightFromForwardAndUp = vAltUp ^ vForwardFromTargetAndPos;
    vRightFromForwardAndUp.Normalize();

    PRREVector vUpFromRightAndForward = vForwardFromTargetAndPos ^ vRightFromForwardAndUp;
    vUpFromRightAndForward.Normalize();

    mat[0][0] = vRightFromForwardAndUp.getX();
    mat[1][0] = vRightFromForwardAndUp.getY();
    mat[2][0] = vRightFromForwardAndUp.getZ();
    mat[3][0] = 0.0f;

    mat[0][1] = vUpFromRightAndForward.getX();
    mat[1][1] = vUpFromRightAndForward.getY();
    mat[2][1] = vUpFromRightAndForward.getZ();
    mat[3][1] = 0.0f;

    mat[0][2] = vForwardFromTargetAndPos.getX();
    mat[1][2] = vForwardFromTargetAndPos.getY();
    mat[2][2] = vForwardFromTargetAndPos.getZ();
    mat[3][2] = 0.0f;

    /*
        Normally we should use negative vPos because this is camera's coords so we should translate in reverse direction,
        but we call invert() at the end anyway so let's stick to positive vPos now and let invert make the translation to reverse direction.
    */
    mat[0][3] = vPos.getX();
    mat[1][3] = vPos.getY();
    mat[2][3] = vPos.getZ();
    mat[3][3] = 1.0f;

    /*
        First I didn't understand why inverting the matrix was needed here. Originally I didn't even invert.
        But it didn't work well ... position of camera was fine as -vPos but the angles given by Right, Forward and Up vectors calculated above were bad!
        Then I realized during debugging they were giving inverse angles! When target was right to the camera, the camera was rotating to left side, etc.
        So what I had to do was to invert the matrix. This gave good angles, I have verified matrix content during debugging.
        But the position of the camera was bad this time... because I had set the last column of the matrix to -vPos because I knew that I needed
        to transform the camera in negative direction. But since invert makes translation to inverse direction, no need to specify negative vPos anymore!
    */
    invert();
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

