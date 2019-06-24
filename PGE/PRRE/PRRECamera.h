#pragma once

/*
    ###################################################################################
    PRRECamera.h
    This file is part of PRRE.
    External header.
    Camera class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREBaseClass.h"
#include "PRREVector.h"
#include "PRREColor.h"


/**
    What buffers to clear before rendering.
*/
enum TPRRE_CLEAR_MODE
{
    PRRE_CLEAR_ZBUFFER_COLORBUFFER,  /**< Clear both Z-Buffer and Color Buffer. */
    PRRE_CLEAR_ZBUFFER               /**< Clear Z-Buffer only. Faster. */
};


/**
    Aspect ratio mode.
*/
enum TPRRE_ASPECTRATIO_MODE
{
    PRRE_AM_FIX,                     /**< Aspect ratio will be the set value. */
    PRRE_AM_DYNAMIC                  /**< Aspect ratio will be set by the viewport geometry. */
};


/**
    Camera class.
    Camera is the eye from where the scene is rendered onto the screen.

    This class doesn't use any API directly.
*/

class PRRECamera : 
    public PRREBaseClass
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRRECamera is included")
#endif

public:

    PRRECamera();

    PRRECamera(const PRRECamera&);
    PRRECamera& operator=(const PRRECamera&);   /**< Assignment operator. */

    virtual ~PRRECamera();

    PRREVector& getPosVec();                    /**< Gets camera's Position vector. */
    const PRREVector& getPosVec() const;        /**< Gets camera's Position vector. */

    PRREVector& getTargetVec();                 /**< Gets camera's Target vector. */
    const PRREVector& getTargetVec() const;     /**< Gets camera's Target vector. */

    PRREVector& getUpVec();                     /**< Gets camera's Up vector. */
    const PRREVector& getUpVec() const;         /**< Gets camera's Up vector. */

    void Move(TPRREfloat amount);               /**< Moves camera forward or backward from viewers perspective by the specified amount. */
    void Strafe(TPRREfloat amount);             /**< Moves camera horizontally from viewers perspective by the specified amount. */
    void Elevate(TPRREfloat amount);            /**< Changes camera's Y-position by the specified amount. */
    void SetRotation(
        TPRREfloat x,
        TPRREfloat y,
        TPRREfloat z);                          /**< Sets camera angle values. */

    const TRECT& getViewport() const;           /**< Gets camera viewport. */
    void SetViewport(
        TPRREuint vx,
        TPRREuint vy,
        TPRREuint vsx,
        TPRREuint vsy);                         /**< Sets viewport position and size. */

    TPRREfloat getNearPlane() const;            /**< Gets camera near plane distance from eye position. */
    void SetNearPlane(TPRREfloat value);        /**< Sets camera near plane distance from eye position. */

    TPRREfloat getFarPlane() const;             /**< Gets camera far plane distance from eye position. */
    void SetFarPlane(TPRREfloat value);         /**< Sets camera far plane distance from eye position. */

    TPRREfloat getFieldOfView() const;          /**< Gets camera field of view angle on Y direction in degrees. */
    void SetFieldOfView(TPRREfloat value);      /**< Sets camera field of view angle on Y direction in degrees. */

    TPRREfloat getAspectRatio() const;          /**< Gets the camera's aspect ratio (usually viewport width / height). */
    void SetAspectRatio(TPRREfloat value);      /**< Sets the camera's aspect ratio (usually viewport width / height). */

    TPRRE_CLEAR_MODE getClearMode() const;      /**< Gets clear mode. */
    void SetClearMode(TPRRE_CLEAR_MODE mode);   /**< Sets clear mode. */

    TPRRE_ASPECTRATIO_MODE getAspectRatioMode() const;     /**< Gets aspect ratio mode. */
    void SetAspectRatioMode(TPRRE_ASPECTRATIO_MODE mode);  /**< Sets aspect ratio mode. */

    PRREColor& getBackgroundColor();               /**< Gets camera background color. */
    const PRREColor& getBackgroundColor() const;   /**< Gets camera background color. */

private:
    class PRRECameraImpl;
    PRRECameraImpl* p;

    // ---------------------------------------------------------------------------

};

