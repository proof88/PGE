#pragma once

/*
    ###################################################################################
    PRRETransformMatrix.h
    This file is part of PRRE.
    External header.
    PRRE 3D-transformation matrix class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "../PRREtypes.h"
#include "PRREMatrix.h"

/**
    PRRE 3D-transformation matrix class.
    PRRE uses the left-handed Cartesian coordinate system.
    This means the XZ plane is horizontal, the XY and and YZ planes are vertical,
    a positive X value means right, a positive Y value means up, and a positive Z value means forward.
*/
class PRRETransformMatrix : public PRREMatrix
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRRETransformMatrix is included")
#endif

public:

    static const PRRETransformMatrix& getZero();       /**< Gets a constant zero matrix. */
    static const PRRETransformMatrix& getIdentity();   /**< Gets a constant identity matrix. */

    // ---------------------------------------------------------------------------

    PRRETransformMatrix();                                           /**< Creates an identity matrix. */
    PRRETransformMatrix(const PRRETransformMatrix& cm);
    explicit PRRETransformMatrix(const TPRRE_NOTABLE_MATRICES& nm);  /**< Creates the requested matrix. */
    virtual ~PRRETransformMatrix();

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
    
protected:

 private:

    static PRRETransformMatrix matIdentity;   /**< Static zero matrix. */
    static PRRETransformMatrix matZero;       /**< Static identity matrix. */

}; // class PRRETransformMatrix