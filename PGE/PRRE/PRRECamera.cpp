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
#include "PRREBaseClass.h"
#include "PRRECamera.h"
#include "PRREMatrix.h"






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

    PRREVector& getPosVec();
    const PRREVector& getPosVec() const;

    PRREVector& getTargetVec();
    const PRREVector& getTargetVec() const;

    PRREVector& getUpVec();
    const PRREVector& getUpVec() const;

    void Move(TPRREfloat amount);
    void Strafe(TPRREfloat amount);
    void Elevate(TPRREfloat amount);
    void SetRotation(TPRREfloat x, TPRREfloat y, TPRREfloat z);

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
    PRREVector vPos, vTarget, vUp;     /**< Position (eye), target (focus or view), up vectors. */
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


// ############################### PUBLIC ################################


PRREVector& PRRECamera::PRRECameraImpl::getPosVec()
{
    return vPos;
}

const PRREVector& PRRECamera::PRRECameraImpl::getPosVec() const
{
    return vPos;
}

PRREVector& PRRECamera::PRRECameraImpl::getTargetVec()
{
    return vTarget;
}

const PRREVector& PRRECamera::PRRECameraImpl::getTargetVec() const
{
    return vTarget;
}

PRREVector& PRRECamera::PRRECameraImpl::getUpVec()
{
    return vUp;
}

const PRREVector& PRRECamera::PRRECameraImpl::getUpVec() const
{
    return vUp;
}


void PRRECamera::PRRECameraImpl::Move(TPRREfloat amount)
{
    PRREVector tmpView(vTarget - vPos);
    tmpView.Normalize();
    vPos += amount * tmpView;
    vTarget += amount * tmpView;
}

void PRRECamera::PRRECameraImpl::Strafe(TPRREfloat amount)
{
    PRREVector tmpView(vTarget - vPos);
    tmpView.Normalize();
    PRREVector tmpCross(vUp ^ tmpView);
    tmpCross.Normalize();
    vPos += amount * tmpCross;
    vTarget += amount * tmpCross;
    // todo: xz values only, not altering Y ?
}

void PRRECamera::PRRECameraImpl::Elevate(TPRREfloat amount)
{
    vPos.SetY( vPos.getY() + amount );
    vTarget.SetY( vTarget.getY() + amount );
}

void PRRECamera::PRRECameraImpl::SetRotation(TPRREfloat x, TPRREfloat y, TPRREfloat z)
{
    const PRREVector vDefTarget(0, 0, 1);
    const PRREVector vDefPos(0, 0, 0);
    // Note: we set absolute rotation here so our initial Target vec is the default in +Z direction and Pos vec is zero.
    // To implement Rotate() instead of SetRotation(), use the current vTarget and vPos vectors to calculate tmpView.
    PRREVector tmpView(vDefTarget - vDefPos);  
    tmpView.Normalize();

    PRREMatrix matRotX, matRotY, matRotZ;
    matRotX.SetRotationX(x);
    matRotY.SetRotationY(y);
    matRotZ.SetRotationZ(z);
    // todo: 3 matrices, are u kidding me?

    tmpView *= matRotX * matRotY * matRotZ;
    tmpView.Normalize();
    vTarget = vPos + tmpView;
}


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


PRRECamera::PRRECamera()
{
    p = new PRRECameraImpl();
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
    p = new PRRECameraImpl();
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
    delete p;
    p = NULL;
}


/**
    Gets camera's Position vector.
*/
PRREVector& PRRECamera::getPosVec()
{
    return p->getPosVec();
}


/**
    Gets camera's Position vector.
*/
const PRREVector& PRRECamera::getPosVec() const
{
    return p->getPosVec();
}


/**
    Gets camera's Target vector.
*/
PRREVector& PRRECamera::getTargetVec()
{
    return p->getTargetVec();
}


/**
    Gets camera's Target vector.
*/
const PRREVector& PRRECamera::getTargetVec() const
{
    return p->getTargetVec();
}

/**
    Gets camera's Up vector.
*/
PRREVector& PRRECamera::getUpVec()
{
    return p->getUpVec();
}


/**
    Gets camera's Up vector.
*/
const PRREVector& PRRECamera::getUpVec() const
{
    return p->getUpVec();
}


/**
    Moves camera forward or backward from viewers perspective by the specified amount.
*/
void PRRECamera::Move(TPRREfloat amount)
{
    p->Move(amount);
}

/**
    Moves camera horizontally from viewers perspective by the specified amount.
*/
void PRRECamera::Strafe(TPRREfloat amount)
{
    p->Strafe(amount);
}


/**
    Changes camera's Y-position by the specified amount.
*/
void PRRECamera::Elevate(TPRREfloat amount)
{
    p->Elevate(amount);
}


/**
    Sets camera angle values.
*/
void PRRECamera::SetRotation(TPRREfloat x, TPRREfloat y, TPRREfloat z)
{
    p->SetRotation(x, y, z);
}


/**
    Gets camera viewport.
*/
const TRECT& PRRECamera::getViewport() const
{
    return p->getViewport();
}
    
/**
    Sets viewport position and size.
    Position values cannot be negative.
    Size values must be positive.
    No effect if invalid value is specified.
*/
void PRRECamera::SetViewport(TPRREuint vx, TPRREuint vy, TPRREuint vsx, TPRREuint vsy)
{
    p->SetViewport(vx, vy, vsx, vsy);
}


/**
    Gets camera near plane distance from eye position.
*/
TPRREfloat PRRECamera::getNearPlane() const
{
    return p->getNearPlane();
}

/**
    Sets camera near plane distance from eye position.
    Only positive value is accepted. Cannot be greater than or equal to the current far plane.
    No effect if invalid value is specified.
*/
void PRRECamera::SetNearPlane(TPRREfloat value)
{
    p->SetNearPlane(value);
}


/**
    Gets camera far plane distance from eye position.
*/
TPRREfloat PRRECamera::getFarPlane() const
{
    return p->getFarPlane();
}


/**
    Sets camera far plane distance from eye position.
    Only positive value is accepted. Cannot be less than or equal to the current near plane.
    No effect if invalid value is specified.
*/
void PRRECamera::SetFarPlane(TPRREfloat value)
{
    p->SetFarPlane(value);
}


/**
    Gets camera field of view angle on Y direction in degrees.
    Default value is 80°.
*/
TPRREfloat PRRECamera::getFieldOfView() const
{
    return p->getFieldOfView();
}

/**
    Sets camera field of view angle on Y direction in degrees.
    Must be positive, cannot be greater than 180.
    No effect if invalid value is specified.
    Default value is 80°.
*/
void PRRECamera::SetFieldOfView(TPRREfloat value)
{
    p->SetFieldOfView(value);
}


/**
    Gets the camera's aspect ratio (usually viewport width / height).
*/
TPRREfloat PRRECamera::getAspectRatio() const
{
    return p->getAspectRatio();
}

/**
    Sets the camera's aspect ratio (usually viewport width / height).
    This is similar to setting the field of view angle on X direction in degrees.
    Must be positive.
    No effect if invalid value is specified.
*/
void PRRECamera::SetAspectRatio(TPRREfloat value)
{
    p->SetAspectRatio(value);
}


/**
    Gets clear mode.
    Clear mode defines which buffers to clear when rendering starts.
    Default is PRRE_CLEAR_ZBUFFER_COLORBUFFER.
*/
TPRRE_CLEAR_MODE PRRECamera::getClearMode() const
{
    return p->getClearMode();
}

/**
    Sets clear mode.
    Clear mode defines which buffers to clear when rendering starts.
    Default is PRRE_CLEAR_ZBUFFER_COLORBUFFER.
*/
void PRRECamera::SetClearMode(TPRRE_CLEAR_MODE mode)
{
    p->SetClearMode(mode);
}


/**
    Gets aspect ratio mode.
    Aspect ratio mode defines what aspect ratio to be used when viewport size changes.
    Default is PRRE_AM_FIX.
*/
TPRRE_ASPECTRATIO_MODE PRRECamera::getAspectRatioMode() const
{
    return p->getAspectRatioMode();
}

/**
    Sets aspect ratio mode.
    Aspect ratio mode defines what aspect ratio to be used when viewport size changes.
    Default is PRRE_AM_FIX.
*/
void PRRECamera::SetAspectRatioMode(TPRRE_ASPECTRATIO_MODE mode)
{
    p->SetAspectRatioMode(mode);
}


/**
    Gets camera background color.
    Camera background color defines with what color to clear the color buffer at the beginning of rendering.
    Note that color buffer is cleared with background color only if clear mode is PRRE_CLEAR_ZBUFFER_COLORBUFFER.
*/
PRREColor& PRRECamera::getBackgroundColor()
{
    return p->getBackgroundColor();
}


/**
    Gets camera background color.
    Camera background color defines with what color to clear the color buffer at the beginning of rendering.
    Note that color buffer is cleared with background color only if clear mode is PRRE_CLEAR_ZBUFFER_COLORBUFFER.
*/
const PRREColor& PRRECamera::getBackgroundColor() const
{
    return p->getBackgroundColor();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRRECamera::PRRECameraImpl::PRRECameraImpl() :
    vUp(0.0f, 1.0f, 0.0f),
    vTarget(0.0f, 0.0f, 1.0f),
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

