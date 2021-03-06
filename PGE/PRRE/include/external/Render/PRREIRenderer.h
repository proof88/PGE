#pragma once

/*
    ###################################################################################
    PRREIRenderer.h
    This file is part of PRRE.
    External header.
    Renderer interface.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "../PRREtypes.h"
#include "../PRRECamera.h"
#include "../PRREuiManager.h"

/*
    We possibly need the following rendering paths:

    - SOFTWARE (CPU):
	    - incremental;
	    - raycasting;
	    - raytracing.

    - HARDWARE (GPU):
	    - forward rendering:
		    - fixed function: TNT2, Geforce, Radeon 7xyz, coming from proofps engine;
		    - shaders: 1.x for Geforce 3/4 and Radeon 8xxx, 2.0 for Geforce FX and Radeon 9xxx
                - possibly different paths for 1.x and 2.0 ?
	    - deferred rendering: G-buffer (no fixed function)
		    - normal (2 passes)
                - MSAA wont work, instead: MLAA, FXAA, SRAA, DLAA;
			    - from DX10 MSAA is good maybe;
		    - deferred lighting (3 passes)
                - MSAA works with DX9 HW too.
*/

/**
    Renderer interface.
    Renderer classes implementing this interface must fully conform to the method specifications written in the comments.

    Basically a renderer class implementing this interface defines a rendering path.
*/


class PRREObject3D;
class PRREObject3DManager;

class PRREIRenderer
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREIRenderer is included")
#endif

public:

    /**
        Initializes the renderer.
        Tries to set the given display mode.
 
        @param width   Client width of the window being created.
                       If 0, the target width will be the current horizontal screen resolution.
                       If the current horizontal screen resolution can't be queried, the target width will be 800.
        @param height  Height of the window being created.
                       If 0, the target height will be the current vertical screen resolution.
                       If the current vertical screen resolution can't be queried, the target height will be 600.
        @param dmode   Display mode to be set.
        @param freq    The refresh rate to be set. If 0, the highest possible will be set at the given resolution. Ignored in windowed mode.
        @param cdepth  The color depth to be set. Ignored in windowed mode.
        @param zdepth  The Z-buffer-depth to be set. Can be 32, 24 or 16. If 32 doesn't work, it tries 24, finally 16, before stopping with error.
        @param stencil The stencil buffer depth to be set. 0 means 0, any other value means 8.
        @param samples The level of antialiasing, < 2 means no AA thus no sample buffers, 2 means 2x AA, 4 means 4x AA, and so on ...
        @param window  If a target window is already created for rendering purpose, it can be specified here. To be used for legacy reasons only.
        @return The result of the initialization. 0 on success, positive value otherwise.
    */
    virtual TPRREuint initialize(
        TPRREuint width, TPRREuint height,
        TPRRE_DISPLAY_MODES dmode,
        TPRREuint freq, TPRREint cdepth,
        TPRREint zdepth, TPRREint stencil,
        TPRREint samples,
        HWND window = NULL) = 0;

    /**
        This stops the renderer.
        Deletes everything created by the renderer.
        No effect before initialization.
        After shutdown, initialize() can be called again.

        @return True on successful shutdown, false otherwise.
    */
    virtual TPRREbool shutdown() = 0;

    /**
        Gets the state of the renderer.
        @return True if the renderer is successfully initialized, false before initialization or after shutdown.
    */
    virtual TPRREbool isInitialized() const = 0;

    /**
        This should be called after initialization, prior to first call to RenderScene().
        This is because these managers may depend on the initialization status of the renderer.
    */
    virtual void SetManagers(
        PRREObject3DManager* _objmgr,
        PRREuiManager* _uimgr,
        PRRECamera* _cam ) = 0;

    /**
        Renders the given object.
        No effect if the engine is not initialized.
    */
    virtual void RenderObject(PRREObject3D& object) = 0;

    /**
        Renders the scene.
        No effect if the engine is not initialized.
    */
    virtual void RenderScene() = 0;

    /**
        Writes statistics to the console window.
    */
    virtual void WriteStats() const = 0;

};

