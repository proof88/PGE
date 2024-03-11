#pragma once

/*
    ###################################################################################
    PureIRenderer.h
    This file is part of PURE.
    External header.
    Renderer interface.
    Made by PR00F88
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "../PureTypes.h"
#include "../PureCamera.h"
#include "../PureUiManager.h"

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
    Render hints are basically debug settings for the renderer used in rare circumstances.
    The available render hints are documented at each renderer implementation.    
*/
typedef TPureUInt TPURE_RENDER_HINT;

class PureObject3D;
class PureObject3DManager;

/**
    Renderer interface.
    Renderer classes implementing this interface must fully conform to the method specifications written in the comments.

    Basically a renderer class implementing this interface defines a rendering path.
*/
class PureIRenderer
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureIRenderer is included")
#endif

public:

    /**
        Initializes the renderer.
        Tries to set the given display mode.
        Render hints are not affected.
 
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
    virtual TPureUInt initialize(
        TPureUInt width, TPureUInt height,
        TPURE_DISPLAY_MODES dmode,
        TPureUInt freq, TPureInt cdepth,
        TPureInt zdepth, TPureInt stencil,
        TPureInt samples,
        HWND window = NULL) = 0;

    /**
        This stops the renderer.
        Deletes everything created by the renderer.
        No effect before initialization.
        After shutdown, initialize() can be called again.
        Render hints are not affected.

        @return True on successful shutdown, false otherwise.
    */
    virtual TPureBool shutdown() = 0;

    /**
        Gets the state of the renderer.
        @return True if the renderer is successfully initialized, false before initialization or after shutdown.
    */
    virtual TPureBool isInitialized() const = 0;

    /**
        This should be called after initialization, prior to first call to RenderScene().
        This is because these managers may depend on the initialization status of the renderer.
    */
    virtual void SetManagers(
        PureObject3DManager* _objmgr,
        PureUiManager* _uimgr,
        PureCamera* _cam ) = 0;

    /**
        Renders the given object.
        No effect if the engine is not initialized.
    */
    virtual void RenderObject(PureObject3D& object) = 0;

    /**
        Renders the scene.
        No effect if the engine is not initialized.
        Note that the first call to this function might not result in actually producing picture of the scene, because the
        implementing renderer might use "feedback mode" which means that the geometry is transformed into feedback buffer
        for debugging purposes, and rasterization is not happening. This is true only for newly created objects though,
        which are rendered for the first time.
    */
    virtual void RenderScene() = 0;

    /**
        Get current render hints.
        Render hints are basically debug settings for the renderer used in rare circumstances.
        The available render hints are documented at each renderer implementation.
    */
    virtual const TPURE_RENDER_HINT& getRenderHints() = 0;

    /**
        Set current render hints.
        Render hints are basically debug settings for the renderer used in rare circumstances.
        The available render hints are documented at each renderer implementation.
    */
    virtual void SetRenderHints(const TPURE_RENDER_HINT& hints) = 0;


    /**
        Resets "Current Statistics".
        The renderer might collect statistical data during its lifetime or for a shorter period, that might be shown
        in debug log by WriteStats().
        In any circumstances you want to reset this data i.e. restart measurements, this function does the job.
        If logging is enabled, it also logs statistics.
    */
    virtual void ResetStatistics() = 0;

    /**
        Writes "Current Statistics" and "Last Frame Statistics" to the console window.
        These are explained at \ref debugging.
    */
    virtual void WriteStats() const = 0;

};

