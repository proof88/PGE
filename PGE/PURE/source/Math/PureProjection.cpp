/*
    ###################################################################################
    PureProjection.cpp
    This file is part of PURE.
    PURE 2D <-> 3D projection class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Math/PureProjection.h"

#include <cmath>

#include "../../include/internal/PurePragmas.h"


// ############################### PUBLIC ################################


/**
    Makes a projection from world-space to window/screen-space.
    Note that for the projected coordinates, vecProjected = (0,0,z) represents the lower left corner of the window/screen, so if you want
    to have this projected coordinate vecProjected to be in the same coordinate system as the sticked objects, you need to
    subtract the half of the camera viewport's width from vecProjected.getX() and the half of the camera viewport's height from vecProjected.getY()
    because sticked objects have (0,0) as the center of the window/screen as the origin.

    @param fWorldX,
           fWorldY,
           fWorldZ      The world-space position coordinates to be projected to window/screen-space.
    @param vecProjected The projected 2D window/screen space coordinates.
                        Valid only if the function returned true.

    @return True if projection is successful, false otherwise.
*/
bool PureProjection::project3dTo2d(
    TPureFloat fWorldX, TPureFloat fWorldY, TPureFloat fWorldZ,
    TPureFloat fCamPosX, TPureFloat fCamPosY, TPureFloat fCamPosZ,
    TPureFloat fCamTargetX, TPureFloat fCamTargetY, TPureFloat fCamTargetZ,
    TPureFloat fCamUpX, TPureFloat fCamUpY, TPureFloat fCamUpZ,
    TPureFloat fCamFovY, TPureFloat fCamAspect,
    TPureFloat fNearPlane, TPureFloat fFarPlane,
    TPureInt nViewportX, TPureInt nViewportY,
    TPureUInt nViewportW, TPureUInt nViewportH,
    PureVector& vecProjected)
{
    // TODO: basically this is copy-paste of PureRendererSWincrementalImpl::VertexProcessing() and
    // PureRendererSWincrementalImpl::PrimitiveAssembly(), so it would be better if those functions
    // would use this. For that this function needs to be split into 2.

    /*
        Projection: to convert 3D world-space coordinates into 2D window-space coordinates, 4 steps are needed:

        1. View Transform:
           World Space -> Eye Space
        2. Projection Transform:
           Eye Space -> Clip Space
        3. Perspective Divide:
           Clip Space -> Normalized Device Space
        4. Viewport Transformation:
           Normalized Device Space -> Window/Screen Space
    
        This is basically what my software renderer is doing in PureRendererSWincrementalImpl::GeometricStage().
        These steps are shown with example coordinates in:
        https://github.com/proof88/PGE/blob/master/PGE/PURE/docpages/internal/PR00FPSvsPRRE-Transformations.xlsx .
        And also explained in https://proof88.github.io/pure-doc/architecture.html .
        Obviously we don't do this with PureRendererHWincrementalImpl since in that case these transformations are happening in the GPU.
    */

    if ((fCamFovY <= 0.f) || (fCamAspect <= 0.f) ||
        (fNearPlane <= 0.f) || (fNearPlane >= fFarPlane) ||
        (nViewportW == 0) || (nViewportH == 0))
    {
        return false;
    }

    vecProjected.Set(fWorldX, fWorldY, fWorldZ);
    vecProjected.Set(3, 1.f);  // W, just in case someone somehow messed W up

    /* 1. World -> View/Eye Space */
    PureTransformMatrix mViewTr;
    mViewTr.SetLookAt(
        PureVector(fCamPosX, fCamPosY, fCamPosZ),
        PureVector(fCamTargetX, fCamTargetY, fCamTargetZ),
        PureVector(fCamUpX, fCamUpY, fCamUpZ));
    vecProjected = mViewTr * vecProjected;

    /* 2. View/Eye -> Clip Space */
    /* The pyramidal frustum in eye space becomes a cube, therefore, the clipping plane equations in clip space are: X + Wc = 0, Y + Wc = 0, Z + Wc = 0, etc. */
    PureTransformMatrix mProjTr;
    mProjTr.SetFrustumByFovY(fCamFovY, fCamAspect, fNearPlane, fFarPlane);
    vecProjected = mProjTr * vecProjected;

    /* We can now easly decide if the given 3D coordinate is within the view frustum or not, since we are in clip space, where the frustum is actually a cube. */
    // Note that I'm unsure if we should return false here or continue projecting, since I also think if the user gives X-Y coords that are
    // too far horizontally or vertically from the camera, we can still project them to 2D even though they fall outside of the viewport/screen area.
    // Maybe we should add a flag argument also that controls this.
    if ((vecProjected.getX() < -vecProjected.getW()) || (vecProjected.getX() > vecProjected.getW())
        ||
        (vecProjected.getY() < -vecProjected.getW()) || (vecProjected.getY() > vecProjected.getW())
        ||
        (vecProjected.getZ() < -vecProjected.getW()) || (vecProjected.getZ() > vecProjected.getW()))

    {
        // out of frustum
        return false;
    }

    /* 3. Clip -> Normalized Device Space */
    if (vecProjected.getW() == 0.f)
    {
        return false;
    }
    vecProjected /= vecProjected.getW();

    /* 4. Normalized Device -> Window/Screen Space */
    const float DEPTH_RANGE_MIN = 0.0f;
    const float DEPTH_RANGE_MAX = 1.0f;

    vecProjected.SetX( std::roundf(nViewportW / 2.f * vecProjected.getX() + nViewportX + nViewportW / 2.f) );
    vecProjected.SetY( std::roundf(nViewportH / 2.f * vecProjected.getY() + nViewportY + nViewportH / 2.f) );
    vecProjected.SetZ( (DEPTH_RANGE_MAX - DEPTH_RANGE_MIN) / 2.f * vecProjected.getZ() + (DEPTH_RANGE_MAX + DEPTH_RANGE_MIN) / 2.f );

    return true;
}  // project3dTo2d()


/**
    Makes an unprojection from window/screen-space to world-space.

    @param nScreenX,
           nScreenY
           nScreenDepth   The window/screen-space position coordinates to be unprojected to world-space.
    @param vecUnprojected The unprojected 3D world-space coordinates.
                          Valid only if the function returned true.

    @return True if unprojection is successful, false otherwise.
*/
bool PureProjection::project2dTo3d(
    TPureUInt nScreenX, TPureUInt nScreenY, TPureFloat nScreenDepth,
    TPureFloat fCamPosX, TPureFloat fCamPosY, TPureFloat fCamPosZ,
    TPureFloat fCamTargetX, TPureFloat fCamTargetY, TPureFloat fCamTargetZ,
    TPureFloat fCamUpX, TPureFloat fCamUpY, TPureFloat fCamUpZ,
    TPureFloat fCamFovY, TPureFloat fCamAspect,
    TPureFloat fNearPlane, TPureFloat fFarPlane,
    TPureInt nViewportX, TPureInt nViewportY,
    TPureUInt nViewportW, TPureUInt nViewportH,
    PureVector& vecUnprojected)
{
    // In this function basically I do everything in reverse order compared to project3dTo2d(),
    // however if there is any problem, check the formula of gluUnProject():
    // https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/gluUnProject4.xml
    //
    // Can also check: https://gamedev.stackexchange.com/questions/45445/what-method-replaces-gl-select-for-box-selection

    /*
        Unprojection: to convert 2D window-space coordinates to 3D world-space coordinates, the reverse order steps are needed:

        1. Viewport Transformation:
           Window/Screen Space -> Normalized Device Space
        2. Perspective Divide:
           Normalized Device Space -> Clip Space
        3. Projection Transform:
           Clip Space -> Eye Space
        4. View Transform:
           Eye Space -> World Space
     */
    
    const float DEPTH_RANGE_MIN = 0.0f;
    const float DEPTH_RANGE_MAX = 1.0f;

    if ((nScreenDepth < DEPTH_RANGE_MIN) ||
        (nScreenDepth > DEPTH_RANGE_MAX) ||
        (fCamFovY <= 0.f) || (fCamAspect <= 0.f) ||
        (fNearPlane <= 0.f) || (fNearPlane >= fFarPlane) ||
        (nViewportW == 0) || (nViewportH == 0))
    {
        return false;
    }

    /* 1. Window/Screen -> Normalized Device Space */
    vecUnprojected.Set(
        (nScreenX - nViewportX - nViewportW / 2.f) / (nViewportW / 2.f),
        (nScreenY - nViewportY - nViewportH / 2.f) / (nViewportH / 2.f),
        (nScreenDepth - (DEPTH_RANGE_MAX + DEPTH_RANGE_MIN) / 2.f) / ((DEPTH_RANGE_MAX - DEPTH_RANGE_MIN) / 2.f));
    vecUnprojected.Set(3, 1.f);  // W, just in case someone somehow messed W up

    /* 2. Normalized Device -> Clip Space */
    // TODO: are we already in Clip space now?

    /* We can now easly decide if the given 2D coordinate is within the view frustum or not, since we are in clip space, where the frustum is actually a cube. */
    // Note that I'm unsure if we should return false here or continue unprojecting, since I also think if the user gives X-Y coords that are
    // off-screen coords, we can still unproject them to 3D even though they fall outside of the frustum.
    // Maybe we should add a flag argument also that controls this.
    if ((vecUnprojected.getX() < -1.f) || (vecUnprojected.getX() > 1.f)
        ||
        (vecUnprojected.getY() < -1.f) || (vecUnprojected.getY() > 1.f)
        ||
        (vecUnprojected.getZ() < -1.f) || (vecUnprojected.getZ() > 1.f))

    {
        // out of frustum
        return false;
    }

    /* 3-4. Clip -> View/Eye -> World Space */
    // I'm doing these 2 steps together so I need to do only 1 matrix invert() instead of 2
    PureTransformMatrix mProjTr;
    mProjTr.SetFrustumByFovY(fCamFovY, fCamAspect, fNearPlane, fFarPlane);
    PureTransformMatrix mViewTr;
    mViewTr.SetLookAt(
        PureVector(fCamPosX, fCamPosY, fCamPosZ),
        PureVector(fCamTargetX, fCamTargetY, fCamTargetZ),
        PureVector(fCamUpX, fCamUpY, fCamUpZ));
    mProjTr *= mViewTr; // here we combine Projection with View Matrix

    mProjTr.invert();
    vecUnprojected = mProjTr * vecUnprojected;

    vecUnprojected = vecUnprojected / vecUnprojected.getW();

    return true;
}  // project2dTo3d()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################

