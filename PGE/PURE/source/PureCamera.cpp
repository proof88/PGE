#include "PureCamera.h"
#include "PureCamera.h"
#pragma once

/*
    ###################################################################################
    PureCamera.cpp
    This file is part of PURE.
    Camera class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../include/external/PureCamera.h"
#include "../include/external/Math/PureMatrix.h"
#include "../include/external/Math/PureProjection.h"
#include "../include/internal/PurePragmas.h"

/*
    http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/
    http://nehe.gamedev.net/article/camera_class_tutorial/18010/
    
    http://unspecified.wordpress.com/2012/06/21/calculating-the-gluperspective-matrix-and-other-opengl-matrix-maths/
*/


/*
   PureCamera::PureCameraImpl
   ###########################################################################
*/


class PureCamera::PureCameraImpl
{

public:

    virtual ~PureCameraImpl();

    const TRECT& getViewport() const;
    void SetViewport(TPureUInt vx, TPureUInt vy, TPureUInt vsx, TPureUInt vsy);

    TPureFloat getNearPlane() const;
    void SetNearPlane(TPureFloat value);

    TPureFloat getFarPlane() const;
    void SetFarPlane(TPureFloat value);

    TPureFloat getFieldOfView() const;
    void SetFieldOfView(TPureFloat value);

    TPureFloat getAspectRatio() const;
    void SetAspectRatio(TPureFloat value);

    TPURE_CLEAR_MODE getClearMode() const;
    void SetClearMode(TPURE_CLEAR_MODE mode);

    TPURE_ASPECTRATIO_MODE getAspectRatioMode() const;
    void SetAspectRatioMode(TPURE_ASPECTRATIO_MODE mode);

    PureColor& getBackgroundColor();
    const PureColor& getBackgroundColor() const;

    bool project3dTo2d(
        TPureFloat fWorldX,
        TPureFloat xWorldY,
        TPureFloat fWorldZ,
        const PureVector& vecPos,
        const PureVector& vecTarget,
        const PureVector& vecUp,
        PureVector& vecProjected) const;

    bool project2dTo3d(
        TPureUInt nScreenX,
        TPureUInt nScreenY,
        TPureFloat nScreenDepth,
        const PureVector& vecPos,
        const PureVector& vecTarget,
        const PureVector& vecUp,
        PureVector& vecUnprojected) const;

private:
    
    TRECT      rectViewport;           /**< Viewport geometry. */
    TPureFloat fPlaneNear, fPlaneFar;  /**< Near plane, far plane. */
    TPureFloat fFOV, fAspectRatio;     /**< Field of view, aspect ratio. */
    TPURE_ASPECTRATIO_MODE aspectMode; /**< Aspect ratio mode. */
    TPURE_CLEAR_MODE clearMode;        /**< What buffers to clear before rendering. */
    PureColor        clrBg;            /**< Background color used when clearing buffer. */

    // ---------------------------------------------------------------------------

    PureCameraImpl();

    friend class PureCamera;

};

const TRECT& PureCamera::PureCameraImpl::getViewport() const
{
    return rectViewport;
}
    
void PureCamera::PureCameraImpl::SetViewport(TPureUInt vx, TPureUInt vy, TPureUInt vsx, TPureUInt vsy)
{
    if ( (vsx == 0) || (vsy == 0) )
        return;

    rectViewport.pos.x = (TPureFloat) vx;
    rectViewport.pos.y = (TPureFloat) vy;
    rectViewport.size.width = (TPureFloat) vsx;
    rectViewport.size.height = (TPureFloat) vsy;
}


TPureFloat PureCamera::PureCameraImpl::getNearPlane() const
{
    return fPlaneNear;
}

void PureCamera::PureCameraImpl::SetNearPlane(TPureFloat value)
{
    if ( (value >= fPlaneFar) || (value <= 0.f) ) 
        return;

    fPlaneNear = value;
}


TPureFloat PureCamera::PureCameraImpl::getFarPlane() const
{
    return fPlaneFar;
}

void PureCamera::PureCameraImpl::SetFarPlane(TPureFloat value)
{
    if ( (value <= fPlaneNear) || (value <= 0.f) ) 
        return;

    fPlaneFar = value;
}


TPureFloat PureCamera::PureCameraImpl::getFieldOfView() const
{
    return fFOV;
}

void PureCamera::PureCameraImpl::SetFieldOfView(TPureFloat value)
{
    if ( (value <= 0.f) || (value > 180.f) )
        return;

    fFOV = value;
}


TPureFloat PureCamera::PureCameraImpl::getAspectRatio() const
{
    return fAspectRatio;
}

void PureCamera::PureCameraImpl::SetAspectRatio(TPureFloat value)
{
    if ( value <= 0.f )
        return;

    fAspectRatio = value;
}


TPURE_CLEAR_MODE PureCamera::PureCameraImpl::getClearMode() const
{
    return clearMode;
}

void PureCamera::PureCameraImpl::SetClearMode(TPURE_CLEAR_MODE mode)
{
    clearMode = mode;
}


TPURE_ASPECTRATIO_MODE PureCamera::PureCameraImpl::getAspectRatioMode() const
{
    return aspectMode;
}

void PureCamera::PureCameraImpl::SetAspectRatioMode(TPURE_ASPECTRATIO_MODE mode)
{
    aspectMode = mode;
}


PureColor& PureCamera::PureCameraImpl::getBackgroundColor()
{
    return clrBg;
}

const PureColor& PureCamera::PureCameraImpl::getBackgroundColor() const
{
    return clrBg;
}

bool PureCamera::PureCameraImpl::project3dTo2d(
    TPureFloat fWorldX, TPureFloat fWorldY, TPureFloat fWorldZ,
    const PureVector& vecPos, const PureVector& vecTarget, const PureVector& vecUp,
    PureVector& vecProjected) const
{
    return PureProjection::project3dTo2d(
        fWorldX, fWorldY, fWorldZ,
        vecPos.getX(), vecPos.getY(), vecPos.getZ(),
        vecTarget.getX(), vecTarget.getY(), vecTarget.getZ(),
        vecUp.getX(), vecUp.getY(), vecUp.getZ(),
        fFOV, fAspectRatio,
        fPlaneNear, fPlaneFar,
        static_cast<TPureUInt>(rectViewport.pos.x),
        static_cast<TPureUInt>(rectViewport.pos.y),
        static_cast<TPureUInt>(rectViewport.size.width),
        static_cast<TPureUInt>(rectViewport.size.height),
        vecProjected);
}

bool PureCamera::PureCameraImpl::project2dTo3d(
    TPureUInt nScreenX, TPureUInt nScreenY, TPureFloat nScreenDepth,
    const PureVector& vecPos, const PureVector& vecTarget, const PureVector& vecUp,
    PureVector& vecUnprojected) const
{
    return PureProjection::project2dTo3d(
        nScreenX, nScreenY, nScreenDepth,
        vecPos.getX(), vecPos.getY(), vecPos.getZ(),
        vecTarget.getX(), vecTarget.getY(), vecTarget.getZ(),
        vecUp.getX(), vecUp.getY(), vecUp.getZ(),
        fFOV, fAspectRatio,
        fPlaneNear, fPlaneFar,
        static_cast<TPureUInt>(rectViewport.pos.x),
        static_cast<TPureUInt>(rectViewport.pos.y),
        static_cast<TPureUInt>(rectViewport.size.width),
        static_cast<TPureUInt>(rectViewport.size.height),
        vecUnprojected);
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################




/*
   PureCamera
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates camera with default settings.
    Default settings:
     - clear mode: PURE_CLEAR_ZBUFFER_COLORBUFFER;
     - near plane: 1.0;
     - far plane: 10.0;
     - viewport X,Y: 0,0;
     - viewport W,H: 800,600;
     - field of view: 80.0;
     - aspect mode: PURE_AM_FIX;
     - aspect ratio: 800 / 600.0;
     - position, up and target vectors as initialized by PurePosUpTarget ctor.

     @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureCamera::PureCamera()
{
    pImpl = new PureCameraImpl();
}

/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureCamera::PureCamera(const PureCamera& other) :
    PurePosUpTarget(other)
{
    pImpl = new PureCameraImpl();
    *pImpl = *(other.pImpl);
}


/**
    Assignment operator.
*/
PureCamera& PureCamera::operator=(const PureCamera& other)
{
    getPosVec()    = other.getPosVec();
    getUpVec()     = other.getUpVec();
    getTargetVec() = other.getTargetVec();

    SetViewport(
        (TPureUInt) other.getViewport().pos.x,
        (TPureUInt) other.getViewport().pos.y,
        (TPureUInt) other.getViewport().size.width,
        (TPureUInt) other.getViewport().size.height );

    SetFieldOfView( other.getFieldOfView() );
    SetFarPlane( other.getFarPlane() );
    SetNearPlane( other.getNearPlane() );
    SetAspectRatioMode( other.getAspectRatioMode() );
    SetAspectRatio( other.getAspectRatio() );
    this->SetClearMode( other.getClearMode() );
    this->getBackgroundColor() = other.getBackgroundColor();

    return *this;
}

PureCamera::~PureCamera()
{
    delete pImpl;
    pImpl = NULL;
}

/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureCamera::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureCamera::getLoggerModuleName()
{
    return "PureCamera";
} // getLoggerModuleName()

/**
    Gets camera viewport.
*/
const TRECT& PureCamera::getViewport() const
{
    return pImpl->getViewport();
}
    
/**
    Sets viewport position and size.
    Position values cannot be negative.
    Size values must be positive.
    No effect if invalid value is specified.
*/
void PureCamera::SetViewport(TPureUInt vx, TPureUInt vy, TPureUInt vsx, TPureUInt vsy)
{
    pImpl->SetViewport(vx, vy, vsx, vsy);
}


/**
    Gets camera near plane distance from eye position.
*/
TPureFloat PureCamera::getNearPlane() const
{
    return pImpl->getNearPlane();
}

/**
    Sets camera near plane distance from eye position.
    Only positive value is accepted. Cannot be greater than or equal to the current far plane.
    No effect if invalid value is specified.
*/
void PureCamera::SetNearPlane(TPureFloat value)
{
    pImpl->SetNearPlane(value);
}


/**
    Gets camera far plane distance from eye position.
*/
TPureFloat PureCamera::getFarPlane() const
{
    return pImpl->getFarPlane();
}


/**
    Sets camera far plane distance from eye position.
    Only positive value is accepted. Cannot be less than or equal to the current near plane.
    No effect if invalid value is specified.
*/
void PureCamera::SetFarPlane(TPureFloat value)
{
    pImpl->SetFarPlane(value);
}


/**
    Gets camera field of view angle on Y direction in degrees.
    Default value is 80�.
*/
TPureFloat PureCamera::getFieldOfView() const
{
    return pImpl->getFieldOfView();
}

/**
    Sets camera field of view angle on Y direction in degrees.
    Must be positive, cannot be greater than 180.
    No effect if invalid value is specified.
    Default value is 80�.
*/
void PureCamera::SetFieldOfView(TPureFloat value)
{
    pImpl->SetFieldOfView(value);
}


/**
    Gets the camera's aspect ratio (usually viewport width / height).
*/
TPureFloat PureCamera::getAspectRatio() const
{
    return pImpl->getAspectRatio();
}

/**
    Sets the camera's aspect ratio (usually viewport width / height).
    This is similar to setting the field of view angle on X direction in degrees.
    Must be positive.
    No effect if invalid value is specified.
*/
void PureCamera::SetAspectRatio(TPureFloat value)
{
    pImpl->SetAspectRatio(value);
}


/**
    Gets clear mode.
    Clear mode defines which buffers to clear when rendering starts.
    Default is PURE_CLEAR_ZBUFFER_COLORBUFFER.
*/
TPURE_CLEAR_MODE PureCamera::getClearMode() const
{
    return pImpl->getClearMode();
}

/**
    Sets clear mode.
    Clear mode defines which buffers to clear when rendering starts.
    Default is PURE_CLEAR_ZBUFFER_COLORBUFFER.
*/
void PureCamera::SetClearMode(TPURE_CLEAR_MODE mode)
{
    pImpl->SetClearMode(mode);
}


/**
    Gets aspect ratio mode.
    Aspect ratio mode defines what aspect ratio to be used when viewport size changes.
    Default is PURE_AM_FIX.
*/
TPURE_ASPECTRATIO_MODE PureCamera::getAspectRatioMode() const
{
    return pImpl->getAspectRatioMode();
}

/**
    Sets aspect ratio mode.
    Aspect ratio mode defines what aspect ratio to be used when viewport size changes.
    Default is PURE_AM_FIX.
*/
void PureCamera::SetAspectRatioMode(TPURE_ASPECTRATIO_MODE mode)
{
    pImpl->SetAspectRatioMode(mode);
}


/**
    Gets camera background color.
    Camera background color defines with what color to clear the color buffer at the beginning of rendering.
    Note that color buffer is cleared with background color only if clear mode is PURE_CLEAR_ZBUFFER_COLORBUFFER.
*/
PureColor& PureCamera::getBackgroundColor()
{
    return pImpl->getBackgroundColor();
}


/**
    Gets camera background color.
    Camera background color defines with what color to clear the color buffer at the beginning of rendering.
    Note that color buffer is cleared with background color only if clear mode is PURE_CLEAR_ZBUFFER_COLORBUFFER.
*/
const PureColor& PureCamera::getBackgroundColor() const
{
    return pImpl->getBackgroundColor();
}


/**
    Makes a projection from world-space to window/screen-space.
    Basically it calls PureProjection::project3dTo2d() will this camera's properties.
    Note that for the projected coordinates, vecProjected = (0,0,z) represents the lower left corner of the window/screen, so if you want
    to have this projected coordinate vecProjected to be in the same coordinate system as the sticked objects, you need to
    subtract the half of the viewport's width from vecProjected.getX() and the half of the viewport's height from vecProjected.getY()
    because sticked objects have (0,0) as the center of the window/screen as the origin.

    @param fWorldX,
           fWorldY,
           fWorldZ      The world-space position coordinates to be projected to window/screen-space.
    @param vecProjected The projected 2D window/screen space coordinates.
                        Valid only if the function returned true.

    @return True if projection is successful, false otherwise.
*/
bool PureCamera::project3dTo2d(TPureFloat fWorldX, TPureFloat fWorldY, TPureFloat fWorldZ, PureVector& vecProjected) const
{
    return pImpl->project3dTo2d(
        fWorldX, fWorldY, fWorldZ,
        getPosVec(), getTargetVec(), getUpVec(),
        vecProjected);
}


/**
    Makes an unprojection from window/screen-space to world-space.
    Basically it calls PureProjection::project2dTo3d() will this camera's properties.

    @param nScreenX,
           nScreenY
           nScreenDepth   The window/screen-space position coordinates to be unprojected to world-space.
    @param vecUnprojected The unprojected 3D world-space coordinates.
                          Valid only if the function returned true.

    @return True if unprojection is successful, false otherwise.
*/
bool PureCamera::project2dTo3d(TPureUInt nScreenX, TPureUInt nScreenY, TPureFloat nScreenDepth, PureVector& vecUnprojected) const
{
    return pImpl->project2dTo3d(
        nScreenX, nScreenY, nScreenDepth,
        getPosVec(), getTargetVec(), getUpVec(),
        vecUnprojected);
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PureCamera::PureCameraImpl::PureCameraImpl() :
    aspectMode(PURE_AM_FIX),
    clearMode(PURE_CLEAR_ZBUFFER_COLORBUFFER),
    fPlaneNear(1.f),
    fPlaneFar(10.f)
{
    SetViewport(0, 0, 800, 600);
    SetFieldOfView(80.0f);
    SetAspectRatio(rectViewport.size.width / rectViewport.size.height);
}


PureCamera::PureCameraImpl::~PureCameraImpl()
{
}

