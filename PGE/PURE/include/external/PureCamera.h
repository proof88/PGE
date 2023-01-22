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
#include "../../../../../CConsole/CConsole/src/CConsole.h"
#include "Math/PureVector.h"
#include "Math/PurePosUpTarget.h"
#include "Material/PureColor.h"


/**
    What buffers to clear before rendering.
*/
enum TPure_CLEAR_MODE
{
    Pure_CLEAR_ZBUFFER_COLORBUFFER,  /**< Clear both Z-Buffer and Color Buffer. */
    Pure_CLEAR_ZBUFFER               /**< Clear Z-Buffer only. Faster. */
};


/**
    Aspect ratio mode.
*/
enum TPure_ASPECTRATIO_MODE
{
    Pure_AM_FIX,                     /**< Aspect ratio will be the set value. */
    Pure_AM_DYNAMIC                  /**< Aspect ratio will be set by the viewport geometry. */
};


/**
    Camera class.
    Camera is the eye from where the scene is rendered onto the screen.
*/

class PureCamera : 
    public PurePosUpTarget
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
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
        TPureuint vx,
        TPureuint vy,
        TPureuint vsx,
        TPureuint vsy);                         /**< Sets viewport position and size. */

    TPurefloat getNearPlane() const;            /**< Gets camera near plane distance from eye position. */
    void SetNearPlane(TPurefloat value);        /**< Sets camera near plane distance from eye position. */

    TPurefloat getFarPlane() const;             /**< Gets camera far plane distance from eye position. */
    void SetFarPlane(TPurefloat value);         /**< Sets camera far plane distance from eye position. */

    TPurefloat getFieldOfView() const;          /**< Gets camera field of view angle on Y direction in degrees. */
    void SetFieldOfView(TPurefloat value);      /**< Sets camera field of view angle on Y direction in degrees. */

    TPurefloat getAspectRatio() const;          /**< Gets the camera's aspect ratio (usually viewport width / height). */
    void SetAspectRatio(TPurefloat value);      /**< Sets the camera's aspect ratio (usually viewport width / height). */

    TPure_CLEAR_MODE getClearMode() const;      /**< Gets clear mode. */
    void SetClearMode(TPure_CLEAR_MODE mode);   /**< Sets clear mode. */

    TPure_ASPECTRATIO_MODE getAspectRatioMode() const;     /**< Gets aspect ratio mode. */
    void SetAspectRatioMode(TPure_ASPECTRATIO_MODE mode);  /**< Sets aspect ratio mode. */

    PureColor& getBackgroundColor();               /**< Gets camera background color. */
    const PureColor& getBackgroundColor() const;   /**< Gets camera background color. */

private:
    class PureCameraImpl;
    PureCameraImpl* pImpl;

    // ---------------------------------------------------------------------------

};

