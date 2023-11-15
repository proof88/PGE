#pragma once

/*
    ###################################################################################
    PureProjection.h
    This file is part of PURE.
    External header.
    PURE 2D <-> 3D projection class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include "PureTransformMatrix.h"

/**
    PURE 2D <-> 3D projections class.
*/
class PureProjection
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureProjection is included")
#endif

public:

    static bool project3dTo2d(
        TPureFloat fWorldX, TPureFloat fWorldY, TPureFloat fWorldZ,
        TPureFloat fCamPosX, TPureFloat fCamPosY, TPureFloat fCamPosZ,
        TPureFloat fCamTargetX, TPureFloat fCamTargetY, TPureFloat fCamTargetZ,
        TPureFloat fCamUpX, TPureFloat fCamUpY, TPureFloat fCamUpZ,
        TPureFloat fCamFovY, TPureFloat fCamAspect,
        TPureFloat fNearPlane, TPureFloat fFarPlane,
        TPureInt nViewportX, TPureInt nViewportY,
        TPureUInt nViewportW, TPureUInt nViewportH,
        PureVector& vecProjected);       /**< Makes a projection from world-space to window/screen-space. */

    static bool project2dTo3d(
        TPureUInt nScreenX, TPureUInt nScreenY, TPureFloat nScreenDepth,
        TPureFloat fCamPosX, TPureFloat fCamPosY, TPureFloat fCamPosZ,
        TPureFloat fCamTargetX, TPureFloat fCamTargetY, TPureFloat fCamTargetZ,
        TPureFloat fCamUpX, TPureFloat fCamUpY, TPureFloat fCamUpZ,
        TPureFloat fCamFovY, TPureFloat fCamAspect,
        TPureFloat fNearPlane, TPureFloat fFarPlane,
        TPureInt nViewportX, TPureInt nViewportY,
        TPureUInt nViewportW, TPureUInt nViewportH,
        PureVector& vecUnprojected);     /**< Makes a projection from window/screen-space to world-space. */

    // ---------------------------------------------------------------------------


}; // class PureTransformMatrix