#pragma once

/*
    ###################################################################################
    PureTransformMatrix.h
    This file is part of PURE.
    External header.
    Pure 3D-transformation matrix class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include "PureMatrix.h"

/**
    Pure 3D-transformation matrix class.
    Pure uses the left-handed Cartesian coordinate system.
    This means the XZ plane is horizontal, the XY and and YZ planes are vertical,
    a positive X value means right, a positive Y value means up, and a positive Z value means forward.
*/
class PureTransformMatrix : public PureMatrix
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureTransformMatrix is included")
#endif

public:

    static const PureTransformMatrix& getZero();       /**< Gets a constant zero matrix. */
    static const PureTransformMatrix& getIdentity();   /**< Gets a constant identity matrix. */

    // ---------------------------------------------------------------------------

    PureTransformMatrix();                                           /**< Creates an identity matrix. */
    PureTransformMatrix(const PureTransformMatrix& cm);
    explicit PureTransformMatrix(const TPURE_NOTABLE_MATRICES& nm);  /**< Creates the requested matrix. */
    virtual ~PureTransformMatrix();

    void SetRotationX(TPureFloat anglex);  /**< Sets the matrix to a rotation matrix around X-axis. */
    void SetRotationY(TPureFloat angley);  /**< Sets the matrix to a rotation matrix around Y-axis. */
    void SetRotationZ(TPureFloat anglez);  /**< Sets the matrix to a rotation matrix around Z-axis. */
    void SetTranslation(
        TPureFloat x,
        TPureFloat y,
        TPureFloat z );                     /**< Sets the matrix to a translation matrix. */

    TPureFloat getScaleX() const;           /**< Gets the scaling on the X-axis. */
    TPureFloat getScaleY() const;           /**< Gets the scaling on the Y-axis. */
    TPureFloat getScaleZ() const;           /**< Gets the scaling on the Z-axis. */
    void       SetScale(
        TPureFloat x,
        TPureFloat y,
        TPureFloat z );                     /**< Sets the scalings on the 3 axes. */
    void       SetScaleX( TPureFloat x );   /**< Sets the matrix to a scaling matrix only on the X-axis. */
    void       SetScaleY( TPureFloat y );   /**< Sets the matrix to a scaling matrix only on the Y-axis. */
    void       SetScaleZ( TPureFloat z );   /**< Sets the matrix to a scaling matrix only on the Z-axis. */

    void SetMirrorXY();   /**< Sets the matrix to a mirroring matrix on the XY planes. */
    void SetMirrorXZ();   /**< Sets the matrix to a mirroring matrix on the XZ planes. */
    void SetMirrorYZ();   /**< Sets the matrix to a mirroring matrix on the YZ planes. */

    void SetFrustum(float left, float right, float bottom, float top, float n, float f);   /**< Sets the matrix to represent a viewing frustum in world coordinate system. */
    void SetFrustumByFovX(float fovx, float aspect, float zNear, float zFar);              /**< Sets the matrix to represent a viewing frustum in world coordinate system. */
    void SetFrustumByFovY(float fovy, float aspect, float zNear, float zFar);              /**< Sets the matrix to represent a viewing frustum in world coordinate system. */

    void SetLookAt(const PureVector& pos, const PureVector& target, const PureVector& up); /**< Sets the matrix to represent a view matrix in world coordinate system. */
    
protected:

 private:

    static PureTransformMatrix matIdentity;   /**< Static zero matrix. */
    static PureTransformMatrix matZero;       /**< Static identity matrix. */

}; // class PureTransformMatrix