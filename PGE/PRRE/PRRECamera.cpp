#pragma once

/*
    ###################################################################################
    PRRECamera.cpp
    This file is part of PRRE.
    Camera class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PRRECamera.h"
#include "Math/PRREMatrix.h"






/*
http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/
http://nehe.gamedev.net/article/camera_class_tutorial/18010/

http://unspecified.wordpress.com/2012/06/21/calculating-the-gluperspective-matrix-and-other-opengl-matrix-maths/
*/


/*
   PRRECamera::PRRECameraImpl
   ###########################################################################
*/


class PRRECamera::PRRECameraImpl
{

public:

    virtual ~PRRECameraImpl();

    const TRECT& getViewport() const;
    void SetViewport(TPRREuint vx, TPRREuint vy, TPRREuint vsx, TPRREuint vsy);

    TPRREfloat getNearPlane() const;
    void SetNearPlane(TPRREfloat value);

    TPRREfloat getFarPlane() const;
    void SetFarPlane(TPRREfloat value);

    TPRREfloat getFieldOfView() const;
    void SetFieldOfView(TPRREfloat value);

    TPRREfloat getAspectRatio() const;
    void SetAspectRatio(TPRREfloat value);

    TPRRE_CLEAR_MODE getClearMode() const;
    void SetClearMode(TPRRE_CLEAR_MODE mode);

    TPRRE_ASPECTRATIO_MODE getAspectRatioMode() const;
    void SetAspectRatioMode(TPRRE_ASPECTRATIO_MODE mode);

    PRREColor& getBackgroundColor();
    const PRREColor& getBackgroundColor() const;

private:
    
    TRECT      rectViewport;           /**< Viewport geometry. */
    TPRREfloat fPlaneNear, fPlaneFar;  /**< Near plane, far plane. */
    TPRREfloat fFOV, fAspectRatio;     /**< Field of view, aspect ratio. */
    TPRRE_ASPECTRATIO_MODE aspectMode; /**< Aspect ratio mode. */
    TPRRE_CLEAR_MODE clearMode;        /**< What buffers to clear before rendering. */
    PRREColor        clrBg;            /**< Background color used when clearing buffer. */

    // ---------------------------------------------------------------------------

    PRRECameraImpl();

    PRRECameraImpl(const PRRECameraImpl&);
    PRRECameraImpl& operator=(const PRRECameraImpl&);

    friend class PRRECamera;

};

const TRECT& PRRECamera::PRRECameraImpl::getViewport() const
{
    return rectViewport;
}
    
void PRRECamera::PRRECameraImpl::SetViewport(TPRREuint vx, TPRREuint vy, TPRREuint vsx, TPRREuint vsy)
{
    if ( (vx < 0) || (vy < 0) )
        return;

    if ( (vsx <= 0) || (vsy <= 0) )
        return;

    rectViewport.pos.x = (TPRREfloat) vx;
    rectViewport.pos.y = (TPRREfloat) vy;
    rectViewport.size.width = (TPRREfloat) vsx;
    rectViewport.size.height = (TPRREfloat) vsy;
}


TPRREfloat PRRECamera::PRRECameraImpl::getNearPlane() const
{
    return fPlaneNear;
}

void PRRECamera::PRRECameraImpl::SetNearPlane(TPRREfloat value)
{
    if ( (value >= fPlaneFar) || (value <= 0.f) ) 
        return;

    fPlaneNear = value;
}


TPRREfloat PRRECamera::PRRECameraImpl::getFarPlane() const
{
    return fPlaneFar;
}

void PRRECamera::PRRECameraImpl::SetFarPlane(TPRREfloat value)
{
    if ( (value <= fPlaneNear) || (value <= 0.f) ) 
        return;

    fPlaneFar = value;
}


TPRREfloat PRRECamera::PRRECameraImpl::getFieldOfView() const
{
    return fFOV;
}

void PRRECamera::PRRECameraImpl::SetFieldOfView(TPRREfloat value)
{
    if ( (value <= 0.f) || (value > 180.f) )
        return;

    fFOV = value;
}


TPRREfloat PRRECamera::PRRECameraImpl::getAspectRatio() const
{
    return fAspectRatio;
}

void PRRECamera::PRRECameraImpl::SetAspectRatio(TPRREfloat value)
{
    if ( value <= 0.f )
        return;

    fAspectRatio = value;
}


TPRRE_CLEAR_MODE PRRECamera::PRRECameraImpl::getClearMode() const
{
    return clearMode;
}

void PRRECamera::PRRECameraImpl::SetClearMode(TPRRE_CLEAR_MODE mode)
{
    clearMode = mode;
}


TPRRE_ASPECTRATIO_MODE PRRECamera::PRRECameraImpl::getAspectRatioMode() const
{
    return aspectMode;
}

void PRRECamera::PRRECameraImpl::SetAspectRatioMode(TPRRE_ASPECTRATIO_MODE mode)
{
    aspectMode = mode;
}


PRREColor& PRRECamera::PRRECameraImpl::getBackgroundColor()
{
    return clrBg;
}

const PRREColor& PRRECamera::PRRECameraImpl::getBackgroundColor() const
{
    return clrBg;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################




/*
   PRRECamera
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates camera with default settings.
    Default settings:
     - clear mode: PRRE_CLEAR_ZBUFFER_COLORBUFFER;
     - near plane: 1.0;
     - far plane: 10.0;
     - viewport X,Y: 0,0;
     - viewport W,H: 800,600;
     - field of view: 80.0;
     - aspect mode: PRRE_AM_FIX;
     - aspect ratio: 800 / 600.0;
     - position, up and target vectors as initialized by PRREPosUpTarget ctor.
*/
PRRECamera::PRRECamera()
{
    pImpl = new PRRECameraImpl();
}

PRRECamera::PRRECamera(const PRRECamera& other)
{
    *this = other;
}


/**
    Assignment operator.
*/
PRRECamera& PRRECamera::operator=(const PRRECamera& other)
{
    pImpl = new PRRECameraImpl();
    getPosVec()    = other.getPosVec();
    getUpVec()     = other.getUpVec();
    getTargetVec() = other.getTargetVec();

    SetViewport(
        (TPRREuint) other.getViewport().pos.x,
        (TPRREuint) other.getViewport().pos.y,
        (TPRREuint) other.getViewport().size.width,
        (TPRREuint) other.getViewport().size.height );

    SetFieldOfView( other.getFieldOfView() );
    SetFarPlane( other.getFarPlane() );
    SetNearPlane( other.getNearPlane() );
    SetAspectRatioMode( other.getAspectRatioMode() );
    SetAspectRatio( other.getAspectRatio() );
    this->SetClearMode( other.getClearMode() );
    this->getBackgroundColor() = other.getBackgroundColor();

    return *this;
}

PRRECamera::~PRRECamera()
{
    delete pImpl;
    pImpl = NULL;
}

/**
    Gets camera viewport.
*/
const TRECT& PRRECamera::getViewport() const
{
    return pImpl->getViewport();
}
    
/**
    Sets viewport position and size.
    Position values cannot be negative.
    Size values must be positive.
    No effect if invalid value is specified.
*/
void PRRECamera::SetViewport(TPRREuint vx, TPRREuint vy, TPRREuint vsx, TPRREuint vsy)
{
    pImpl->SetViewport(vx, vy, vsx, vsy);
}


/**
    Gets camera near plane distance from eye position.
*/
TPRREfloat PRRECamera::getNearPlane() const
{
    return pImpl->getNearPlane();
}

/**
    Sets camera near plane distance from eye position.
    Only positive value is accepted. Cannot be greater than or equal to the current far plane.
    No effect if invalid value is specified.
*/
void PRRECamera::SetNearPlane(TPRREfloat value)
{
    pImpl->SetNearPlane(value);
}


/**
    Gets camera far plane distance from eye position.
*/
TPRREfloat PRRECamera::getFarPlane() const
{
    return pImpl->getFarPlane();
}


/**
    Sets camera far plane distance from eye position.
    Only positive value is accepted. Cannot be less than or equal to the current near plane.
    No effect if invalid value is specified.
*/
void PRRECamera::SetFarPlane(TPRREfloat value)
{
    pImpl->SetFarPlane(value);
}


/**
    Gets camera field of view angle on Y direction in degrees.
    Default value is 80°.
*/
TPRREfloat PRRECamera::getFieldOfView() const
{
    return pImpl->getFieldOfView();
}

/**
    Sets camera field of view angle on Y direction in degrees.
    Must be positive, cannot be greater than 180.
    No effect if invalid value is specified.
    Default value is 80°.
*/
void PRRECamera::SetFieldOfView(TPRREfloat value)
{
    pImpl->SetFieldOfView(value);
}


/**
    Gets the camera's aspect ratio (usually viewport width / height).
*/
TPRREfloat PRRECamera::getAspectRatio() const
{
    return pImpl->getAspectRatio();
}

/**
    Sets the camera's aspect ratio (usually viewport width / height).
    This is similar to setting the field of view angle on X direction in degrees.
    Must be positive.
    No effect if invalid value is specified.
*/
void PRRECamera::SetAspectRatio(TPRREfloat value)
{
    pImpl->SetAspectRatio(value);
}


/**
    Gets clear mode.
    Clear mode defines which buffers to clear when rendering starts.
    Default is PRRE_CLEAR_ZBUFFER_COLORBUFFER.
*/
TPRRE_CLEAR_MODE PRRECamera::getClearMode() const
{
    return pImpl->getClearMode();
}

/**
    Sets clear mode.
    Clear mode defines which buffers to clear when rendering starts.
    Default is PRRE_CLEAR_ZBUFFER_COLORBUFFER.
*/
void PRRECamera::SetClearMode(TPRRE_CLEAR_MODE mode)
{
    pImpl->SetClearMode(mode);
}


/**
    Gets aspect ratio mode.
    Aspect ratio mode defines what aspect ratio to be used when viewport size changes.
    Default is PRRE_AM_FIX.
*/
TPRRE_ASPECTRATIO_MODE PRRECamera::getAspectRatioMode() const
{
    return pImpl->getAspectRatioMode();
}

/**
    Sets aspect ratio mode.
    Aspect ratio mode defines what aspect ratio to be used when viewport size changes.
    Default is PRRE_AM_FIX.
*/
void PRRECamera::SetAspectRatioMode(TPRRE_ASPECTRATIO_MODE mode)
{
    pImpl->SetAspectRatioMode(mode);
}


/**
    Gets camera background color.
    Camera background color defines with what color to clear the color buffer at the beginning of rendering.
    Note that color buffer is cleared with background color only if clear mode is PRRE_CLEAR_ZBUFFER_COLORBUFFER.
*/
PRREColor& PRRECamera::getBackgroundColor()
{
    return pImpl->getBackgroundColor();
}


/**
    Gets camera background color.
    Camera background color defines with what color to clear the color buffer at the beginning of rendering.
    Note that color buffer is cleared with background color only if clear mode is PRRE_CLEAR_ZBUFFER_COLORBUFFER.
*/
const PRREColor& PRRECamera::getBackgroundColor() const
{
    return pImpl->getBackgroundColor();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRRECamera::PRRECameraImpl::PRRECameraImpl() :
    aspectMode(PRRE_AM_FIX),
    clearMode(PRRE_CLEAR_ZBUFFER_COLORBUFFER),
    fPlaneNear(1.f),
    fPlaneFar(10.f)
{
    SetViewport(0, 0, 800, 600);
    SetFieldOfView(80.0f);
    SetAspectRatio(rectViewport.size.width / rectViewport.size.height);
}

PRRECamera::PRRECameraImpl::PRRECameraImpl(const PRRECamera::PRRECameraImpl&)
{

}

PRRECamera::PRRECameraImpl& PRRECamera::PRRECameraImpl::operator=(const PRRECamera::PRRECameraImpl&)
{
    return *this;
}

PRRECamera::PRRECameraImpl::~PRRECameraImpl()
{
}

