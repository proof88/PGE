#pragma once

/*
    ###################################################################################
    PR00FsReducedRenderingEngine.h
    This file is part of PRRE.
    External header.
    PR00F's Reduced Rendering Engine
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREtypes.h"
#include "PRREBaseClass.h"
#include "Display/PRREScreen.h"
#include "Display/PRREWindow.h"
#include "Hardware/PRREhwInfo.h"
#include "Material/PRREImageManager.h"
#include "Material/PRRETextureManager.h"
#include "Material/PRREMaterialManager.h"
#include "Object3D/PRREObject3DManager.h"
#include "PRRECamera.h"
#include "PRREuiManager.h"
#include "Render/PRREIRenderer.h"


/**
    Which renderer to be used.
*/
enum TPRRE_RENDERER
{
    PRRE_RENDERER_SW,      /**< Software renderer. */
    PRRE_RENDERER_HW_FP    /**< Hardware renderer. */
}; // TPRRE_RENDERER


/**
    The primary and main class of the graphics engine.
    Singleton, can be instantiated with static createAndGet().
    This initializes the engine.
*/

class PR00FsReducedRenderingEngine :
    public PRREBaseClass
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PR00FsReducedRenderingEngine is included")
#endif

public:
    static PR00FsReducedRenderingEngine& createAndGet();    /**< Creates and gets the singleton implementation instance. */

    // ---------------------------------------------------------------------------

    virtual TPRREuint initialize(
        TPRRE_RENDERER rndr,
        TPRREuint width, TPRREuint height,
        TPRRE_DISPLAY_MODES dmode,
        TPRREuint freq, TPRREint cdepth,
        TPRREint zdepth, TPRREint stencil,
        TPRREint samples,
        HWND window = NULL) = 0;                            /**< Initializes the engine. */
    virtual TPRREbool shutdown() = 0;                       /**< This stops the engine. */
    virtual TPRREbool isInitialized() const = 0;            /**< Gets the state of the engine. */
    
    virtual PRREScreen&          getScreen() const = 0;           /**< Get access to screen manipulation functions. */
    virtual PRREWindow&          getWindow() const = 0;           /**< Get access to window manipulation functions. */
    virtual PRREhwInfo&          getHardwareInfo() const = 0;     /**< Get access to hardware information. */
    virtual PRREImageManager&    getImageManager() const = 0;     /**< Get access to image handler functions. */
    virtual PRRETextureManager&  getTextureManager() const = 0;   /**< Get access to texture handler functions. */
    virtual PRREMaterialManager& getMaterialManager() const = 0;  /**< Get access to material handler functions. */
    virtual PRREMesh3DManager&   getMesh3DManager() const = 0;    /**< Get access to Mesh3D handler functions. */
    virtual PRREObject3DManager& getObject3DManager() const = 0;  /**< Get access to Object3D handler functions. */
    virtual PRRECamera&          getCamera() const = 0;           /**< Get access to camera handler functions. */
    virtual PRREuiManager&       getUImanager() const = 0;        /**< Get access to user interface functions. */
    virtual PRREIRenderer*       getRenderer() const = 0;         /**< Get access to the selected renderer. */

    virtual void                 WriteList() const = 0;           /**< Invoke WriteList() of all children instances. */

    virtual void                 CopyScreenToTexture(PRRETexture& tex) = 0;  /**< Copies screen content to texture. */
  
}; // class PR00FsReducedRenderingEngine
