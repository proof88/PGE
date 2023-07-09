#pragma once

/*
    ###################################################################################
    PureCamera.h
    This file is part of PURE.
    External header.
    Camera class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureAllHeaders.h"
#include "../../../../../Console/CConsole/src/CConsole.h"
#include "Math/PureVector.h"
#include "Math/PurePosUpTarget.h"
#include "Material/PureColor.h"


/**
    What buffers to clear before rendering.
*/
enum TPURE_CLEAR_MODE
{
    PURE_CLEAR_ZBUFFER_COLORBUFFER,  /**< Clear both Z-Buffer and Color Buffer. */
    PURE_CLEAR_ZBUFFER               /**< Clear Z-Buffer only. Faster. */
};


/**
    Aspect ratio mode.
*/
enum TPURE_ASPECTRATIO_MODE
{
    PURE_AM_FIX,                     /**< Aspect ratio will be the set value. */
    PURE_AM_DYNAMIC                  /**< Aspect ratio will be set by the viewport geometry. */
};


/**
    Camera class.
    Camera is the eye from where the scene is rendered onto the screen.
*/

class PureCamera : 
    public PurePosUpTarget
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureCamera is included")
#endif

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureCamera();                               /**< Creates camera with default settings. */ /* TODO: mark this as noexcept(false) when using newer compiler! */

    PureCamera(const PureCamera&);
    PureCamera& operator=(const PureCamera&);   /**< Assignment operator. */

    virtual ~PureCamera();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    const TRECT& getViewport() const;           /**< Gets camera viewport. */
    void SetViewport(
        TPureUInt vx,
        TPureUInt vy,
        TPureUInt vsx,
        TPureUInt vsy);                         /**< Sets viewport position and size. */

    TPureFloat getNearPlane() const;            /**< Gets camera near plane distance from eye position. */
    void SetNearPlane(TPureFloat value);        /**< Sets camera near plane distance from eye position. */

    TPureFloat getFarPlane() const;             /**< Gets camera far plane distance from eye position. */
    void SetFarPlane(TPureFloat value);         /**< Sets camera far plane distance from eye position. */

    TPureFloat getFieldOfView() const;          /**< Gets camera field of view angle on Y direction in degrees. */
    void SetFieldOfView(TPureFloat value);      /**< Sets camera field of view angle on Y direction in degrees. */

    TPureFloat getAspectRatio() const;          /**< Gets the camera's aspect ratio (usually viewport width / height). */
    void SetAspectRatio(TPureFloat value);      /**< Sets the camera's aspect ratio (usually viewport width / height). */

    TPURE_CLEAR_MODE getClearMode() const;      /**< Gets clear mode. */
    void SetClearMode(TPURE_CLEAR_MODE mode);   /**< Sets clear mode. */

    TPURE_ASPECTRATIO_MODE getAspectRatioMode() const;     /**< Gets aspect ratio mode. */
    void SetAspectRatioMode(TPURE_ASPECTRATIO_MODE mode);  /**< Sets aspect ratio mode. */

    PureColor& getBackgroundColor();               /**< Gets camera background color. */
    const PureColor& getBackgroundColor() const;   /**< Gets camera background color. */

private:
    class PureCameraImpl;
    PureCameraImpl* pImpl;

    // ---------------------------------------------------------------------------

};

