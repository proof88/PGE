/*
    ###################################################################################
    PRRETransformMatrix.cpp
    This file is part of PRRE.
    PRRE 3D-transformation matrix class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRRETransformMatrix.h"


// ############################### PUBLIC ################################


/**
    Gets a constant zero matrix.
*/
const PRRETransformMatrix& PRRETransformMatrix::getZero()
{
    return matZero;
} // getZero() static


/**
    Gets a constant identity matrix.
*/
const PRRETransformMatrix& PRRETransformMatrix::getIdentity()
{
    return matIdentity;
} // getIdentity() static


/**
    Creates an identity matrix.
*/
PRRETransformMatrix::PRRETransformMatrix()
{
} // PRRETransformMatrix()


PRRETransformMatrix::PRRETransformMatrix(const PRRETransformMatrix& cm) :
    PRREMatrix(cm)
{
} // PRRETransformMatrix(...)


/**
    Creates the requested matrix.
*/
PRRETransformMatrix::PRRETransformMatrix(const TPRRE_NOTABLE_MATRICES& nm) : 
    PRREMatrix(nm)
{
} // PRRETransformMatrix(...)


PRRETransformMatrix::~PRRETransformMatrix()
{

} // ~PRRETransformMatrix()


/**
    Sets the matrix to a rotation matrix around X-axis.
*/
void PRRETransformMatrix::SetRotationX(TPRREfloat anglex)
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
void PRRETransformMatrix::SetRotationY(TPRREfloat angley)
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
void PRRETransformMatrix::SetRotationZ(TPRREfloat anglez)
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
void PRRETransformMatrix::SetTranslation(TPRREfloat x, TPRREfloat y, TPRREfloat z)
{
    SetIdentity();
    mat[0][3] = x;
    mat[1][3] = y;
    mat[2][3] = z;
} // SetTranslation()


/**
    Gets the scaling on the X-axis.
*/
TPRREfloat PRRETransformMatrix::getScaleX() const
{
    return mat[0][0];
} // getScaleX()


/**
    Gets the scaling on the Y-axis.
*/
TPRREfloat PRRETransformMatrix::getScaleY() const
{
    return mat[1][1];
} // getScaleY()


/**
    Gets the scaling on the Z-axis.
*/
TPRREfloat PRRETransformMatrix::getScaleZ() const
{
    return mat[2][2];
} // getScaleZ()


/**
    Sets the scalings on the 3 axes.
*/
void PRRETransformMatrix::SetScale(TPRREfloat x, TPRREfloat y, TPRREfloat z)
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
void PRRETransformMatrix::SetScaleX(TPRREfloat x)
{
    SetScale(x, 1.0f, 1.0f);
} // SetScaleX()


/**
    Sets the matrix to a scaling matrix only on the Y-axis.
*/
void PRRETransformMatrix::SetScaleY(TPRREfloat y) 
{
    SetScale(1.0f, y, 1.0f);
} // SetScaleY()


/**
    Sets the matrix to a scaling matrix only on the Z-axis.
*/
void PRRETransformMatrix::SetScaleZ(TPRREfloat z)
{
    SetScale(1.0f, 1.0f, z);
} // SetScaleZ()


/**
    Sets the matrix to a mirroring matrix on the XY planes.
*/
void PRRETransformMatrix::SetMirrorXY()
{
    SetIdentity();
    mat[2][2] = -1.0f;
} // SetMirrorXY()


/**
    Sets the matrix to a mirroring matrix on the XZ planes.
*/
void PRRETransformMatrix::SetMirrorXZ()
{
    SetIdentity();
    mat[1][1] = -1.0f;
} // SetMirrorXZ()


/**
    Sets the matrix to a mirroring matrix on the YZ planes.
*/
void PRRETransformMatrix::SetMirrorYZ()
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
void PRRETransformMatrix::SetFrustum(float left, float right, float bottom, float top, float n, float f)
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
void PRRETransformMatrix::SetFrustumByFovX(float fovx, float aspect, float zNear, float zFar)
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


/**
    Sets the matrix to represent a viewing frustum in world coordinate system.
    It can be used to produce perspective projection, typically from eye space to clip space.

    Depth buffer precision is affected by the values specified for zNear and zFar. The greater the ratio of zFar to zNear is, the less effective the depth buffer will be at distinguishing between surfaces that are near each other.

    @param fovy   The field of vew angle in Y direction. Must be greater than 0.
    @param aspect Specifies the aspect ratio that determines the field of view in the X direction. Typically the aspect ratio is the ratio of viewport width to viewport height. Must be greater than 0.
    @param zNear  Near clipping plane distance to the viewer in eye space on Z direction. Must be greater than 0.
    @param zFar   Far clipping plane distance to the viewer in eye space on Z direction. Must be greater than zNear.
*/
void PRRETransformMatrix::SetFrustumByFovY(float fovy, float aspect, float zNear, float zFar)
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
void PRRETransformMatrix::SetLookAt(const PRREVector& vPos,
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


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRRETransformMatrix PRRETransformMatrix::matZero(PRRE_MATRIX_ZERO);            // Our static zero matrix.
PRRETransformMatrix PRRETransformMatrix::matIdentity(PRRE_MATRIX_IDENTITY);    // Our static identity matrix.