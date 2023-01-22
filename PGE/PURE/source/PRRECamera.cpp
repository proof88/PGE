#pragma once

/*
    ###################################################################################
    PureCamera.cpp
    This file is part of Pure.
    Camera class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../include/external/PureCamera.h"
#include "../include/external/Math/PureMatrix.h"
#include "../include/internal/Purepragmas.h"

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
    void SetViewport(TPureuint vx, TPureuint vy, TPureuint vsx, TPureuint vsy);

    TPurefloat getNearPlane() const;
    void SetNearPlane(TPurefloat value);

    TPurefloat getFarPlane() const;
    void SetFarPlane(TPurefloat value);

    TPurefloat getFieldOfView() const;
    void SetFieldOfView(TPurefloat value);

    TPurefloat getAspectRatio() const;
    void SetAspectRatio(TPurefloat value);

    TPure_CLEAR_MODE getClearMode() const;
    void SetClearMode(TPure_CLEAR_MODE mode);

    TPure_ASPECTRATIO_MODE getAspectRatioMode() const;
    void SetAspectRatioMode(TPure_ASPECTRATIO_MODE mode);

    PureColor& getBackgroundColor();
    const PureColor& getBackgroundColor() const;

private:
    
    TRECT      rectViewport;           /**< Viewport geometry. */
    TPurefloat fPlaneNear, fPlaneFar;  /**< Near plane, far plane. */
    TPurefloat fFOV, fAspectRatio;     /**< Field of view, aspect ratio. */
    TPure_ASPECTRATIO_MODE aspectMode; /**< Aspect ratio mode. */
    TPure_CLEAR_MODE clearMode;        /**< What buffers to clear before rendering. */
    PureColor        clrBg;            /**< Background color used when clearing buffer. */

    // ---------------------------------------------------------------------------

    PureCameraImpl();

    friend class PureCamera;

};

const TRECT& PureCamera::PureCameraImpl::getViewport() const
{
    return rectViewport;
}
    
void PureCamera::PureCameraImpl::SetViewport(TPureuint vx, TPureuint vy, TPureuint vsx, TPureuint vsy)
{
    if ( (vsx == 0) || (vsy == 0) )
        return;

    rectViewport.pos.x = (TPurefloat) vx;
    rectViewport.pos.y = (TPurefloat) vy;
    rectViewport.size.width = (TPurefloat) vsx;
    rectViewport.size.height = (TPurefloat) vsy;
}


TPurefloat PureCamera::PureCameraImpl::getNearPlane() const
{
    return fPlaneNear;
}

void PureCamera::PureCameraImpl::SetNearPlane(TPurefloat value)
{
    if ( (value >= fPlaneFar) || (value <= 0.f) ) 
        return;

    fPlaneNear = value;
}


TPurefloat PureCamera::PureCameraImpl::getFarPlane() const
{
    return fPlaneFar;
}

void PureCamera::PureCameraImpl::SetFarPlane(TPurefloat value)
{
    if ( (value <= fPlaneNear) || (value <= 0.f) ) 
        return;

    fPlaneFar = value;
}


TPurefloat PureCamera::PureCameraImpl::getFieldOfView() const
{
    return fFOV;
}

void PureCamera::PureCameraImpl::SetFieldOfView(TPurefloat value)
{
    if ( (value <= 0.f) || (value > 180.f) )
        return;

    fFOV = value;
}


TPurefloat PureCamera::PureCameraImpl::getAspectRatio() const
{
    return fAspectRatio;
}

void PureCamera::PureCameraImpl::SetAspectRatio(TPurefloat value)
{
    if ( value <= 0.f )
        return;

    fAspectRatio = value;
}


TPure_CLEAR_MODE PureCamera::PureCameraImpl::getClearMode() const
{
    return clearMode;
}

void PureCamera::PureCameraImpl::SetClearMode(TPure_CLEAR_MODE mode)
{
    clearMode = mode;
}


TPure_ASPECTRATIO_MODE PureCamera::PureCameraImpl::getAspectRatioMode() const
{
    return aspectMode;
}

void PureCamera::PureCameraImpl::SetAspectRatioMode(TPure_ASPECTRATIO_MODE mode)
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
     - clear mode: Pure_CLEAR_ZBUFFER_COLORBUFFER;
     - near plane: 1.0;
     - far plane: 10.0;
     - viewport X,Y: 0,0;
     - viewport W,H: 800,600;
     - field of view: 80.0;
     - aspect mode: Pure_AM_FIX;
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
        (TPureuint) other.getViewport().pos.x,
        (TPureuint) other.getViewport().pos.y,
        (TPureuint) other.getViewport().size.width,
        (TPureuint) other.getViewport().size.height );

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
void PureCamera::SetViewport(TPureuint vx, TPureuint vy, TPureuint vsx, TPureuint vsy)
{
    pImpl->SetViewport(vx, vy, vsx, vsy);
}


/**
    Gets camera near plane distance from eye position.
*/
TPurefloat PureCamera::getNearPlane() const
{
    return pImpl->getNearPlane();
}

/**
    Sets camera near plane distance from eye position.
    Only positive value is accepted. Cannot be greater than or equal to the current far plane.
    No effect if invalid value is specified.
*/
void PureCamera::SetNearPlane(TPurefloat value)
{
    pImpl->SetNearPlane(value);
}


/**
    Gets camera far plane distance from eye position.
*/
TPurefloat PureCamera::getFarPlane() const
{
    return pImpl->getFarPlane();
}


/**
    Sets camera far plane distance from eye position.
    Only positive value is accepted. Cannot be less than or equal to the current near plane.
    No effect if invalid value is specified.
*/
void PureCamera::SetFarPlane(TPurefloat value)
{
    pImpl->SetFarPlane(value);
}


/**
    Gets camera field of view angle on Y direction in degrees.
    Default value is 80°.
*/
TPurefloat PureCamera::getFieldOfView() const
{
    return pImpl->getFieldOfView();
}

/**
    Sets camera field of view angle on Y direction in degrees.
    Must be positive, cannot be greater than 180.
    No effect if invalid value is specified.
    Default value is 80°.
*/
void PureCamera::SetFieldOfView(TPurefloat value)
{
    pImpl->SetFieldOfView(value);
}


/**
    Gets the camera's aspect ratio (usually viewport width / height).
*/
TPurefloat PureCamera::getAspectRatio() const
{
    return pImpl->getAspectRatio();
}

/**
    Sets the camera's aspect ratio (usually viewport width / height).
    This is similar to setting the field of view angle on X direction in degrees.
    Must be positive.
    No effect if invalid value is specified.
*/
void PureCamera::SetAspectRatio(TPurefloat value)
{
    pImpl->SetAspectRatio(value);
}


/**
    Gets clear mode.
    Clear mode defines which buffers to clear when rendering starts.
    Default is Pure_CLEAR_ZBUFFER_COLORBUFFER.
*/
TPure_CLEAR_MODE PureCamera::getClearMode() const
{
    return pImpl->getClearMode();
}

/**
    Sets clear mode.
    Clear mode defines which buffers to clear when rendering starts.
    Default is Pure_CLEAR_ZBUFFER_COLORBUFFER.
*/
void PureCamera::SetClearMode(TPure_CLEAR_MODE mode)
{
    pImpl->SetClearMode(mode);
}


/**
    Gets aspect ratio mode.
    Aspect ratio mode defines what aspect ratio to be used when viewport size changes.
    Default is Pure_AM_FIX.
*/
TPure_ASPECTRATIO_MODE PureCamera::getAspectRatioMode() const
{
    return pImpl->getAspectRatioMode();
}

/**
    Sets aspect ratio mode.
    Aspect ratio mode defines what aspect ratio to be used when viewport size changes.
    Default is Pure_AM_FIX.
*/
void PureCamera::SetAspectRatioMode(TPure_ASPECTRATIO_MODE mode)
{
    pImpl->SetAspectRatioMode(mode);
}


/**
    Gets camera background color.
    Camera background color defines with what color to clear the color buffer at the beginning of rendering.
    Note that color buffer is cleared with background color only if clear mode is Pure_CLEAR_ZBUFFER_COLORBUFFER.
*/
PureColor& PureCamera::getBackgroundColor()
{
    return pImpl->getBackgroundColor();
}


/**
    Gets camera background color.
    Camera background color defines with what color to clear the color buffer at the beginning of rendering.
    Note that color buffer is cleared with background color only if clear mode is Pure_CLEAR_ZBUFFER_COLORBUFFER.
*/
const PureColor& PureCamera::getBackgroundColor() const
{
    return pImpl->getBackgroundColor();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PureCamera::PureCameraImpl::PureCameraImpl() :
    aspectMode(Pure_AM_FIX),
    clearMode(Pure_CLEAR_ZBUFFER_COLORBUFFER),
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

