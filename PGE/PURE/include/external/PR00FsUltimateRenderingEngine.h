#pragma once

/*
    ###################################################################################
    PR00FsUltimateRenderingEngine.h
    This file is part of Pure.
    External header.
    PR00F's Ultimate Rendering Engine
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../../../../Console/CConsole/src/CConsole.h"

#include "../../../Config/PGEcfgProfiles.h"
#include "../../../PGEInputHandler.h"

#include "PureAllHeaders.h"
#include "PureTypes.h"
#include "Display/PureScreen.h"
#include "Display/PureWindow.h"
#include "Hardware/PureHwInfo.h"
#include "Material/PureImageManager.h"
#include "Material/PureTextureManager.h"
#include "Material/PureMaterialManager.h"
#include "Object3D/PureObject3DManager.h"
#include "PureCamera.h"
#include "PureUiManager.h"
#include "Render/PureIRenderer.h"


/**
    Which renderer to be used.
*/
enum TPURE_RENDERER
{
    PURE_RENDERER_SW,      /**< Software renderer. */
    PURE_RENDERER_HW_FP    /**< Hardware renderer. */
}; // TPURE_RENDERER


/**
    The primary and main class of the graphics engine.
    Singleton, can be instantiated with static createAndGet().
    This initializes the engine.
*/

class PR00FsUltimateRenderingEngine
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PR00FsUltimateRenderingEngine is included")
#endif

public:
    static PR00FsUltimateRenderingEngine& createAndGet(
        PGEcfgProfiles& cfgProfiles,
        PGEInputHandler& inputHandler);                /**< Creates and gets the singleton implementation instance. */

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    virtual TPureUInt initialize(
        TPURE_RENDERER rndr,
        TPureUInt width, TPureUInt height,
        TPURE_DISPLAY_MODES dmode,
        TPureUInt freq, TPureInt cdepth,
        TPureInt zdepth, TPureInt stencil,
        TPureInt samples,
        HWND window = NULL) = 0;                            /**< Initializes the engine. */
    virtual TPureBool shutdown() = 0;                       /**< This stops the engine. */
    virtual TPureBool isInitialized() const = 0;            /**< Gets the state of the engine. */
    
    virtual PureScreen&          getScreen() const = 0;           /**< Get access to screen manipulation functions. */
    virtual PureWindow&          getWindow() const = 0;           /**< Get access to window manipulation functions. */
    virtual PureHwInfo&          getHardwareInfo() const = 0;     /**< Get access to hardware information. */
    virtual PureImageManager&    getImageManager() const = 0;     /**< Get access to image handler functions. */
    virtual PureTextureManager&  getTextureManager() const = 0;   /**< Get access to texture handler functions. */
    virtual PureMaterialManager& getMaterialManager() const = 0;  /**< Get access to material handler functions. */
    virtual PureMesh3DManager&   getMesh3DManager() const = 0;    /**< Get access to Mesh3D handler functions. */
    virtual PureObject3DManager& getObject3DManager() const = 0;  /**< Get access to Object3D handler functions. */
    virtual PureCamera&          getCamera() const = 0;           /**< Get access to camera handler functions. */
    virtual PureUiManager&       getUImanager() const = 0;        /**< Get access to user interface functions. */
    virtual PureIRenderer*       getRenderer() const = 0;         /**< Get access to the selected renderer. */
    virtual void                 ShowGuiDemo(TPureBool state) = 0;

    virtual void                 WriteList() const = 0;           /**< Invoke WriteList() of all children instances. */

    virtual TPureBool getAutoWriteStatsAtShutdown() const = 0;           /**< Checks if invoke of WriteStats() would happen at the beginning of shutdown(). */
    virtual void      SetAutoWriteStatsAtShutdown(TPureBool state) = 0;  /**< Sets if invoke of WriteStats() should happen at the beginning of shutdown(). */

    virtual void                 CopyScreenToTexture(PureTexture& tex) = 0;  /**< Copies screen content to texture. */
  
}; // class PR00FsUltimateRenderingEngine
