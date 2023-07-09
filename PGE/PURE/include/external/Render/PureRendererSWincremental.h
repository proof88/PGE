#pragma once

/*
    ###################################################################################
    PureRendererSWincremental.h
    This file is part of PURE.
    External header.
    Incremental SW Renderer class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../../../../../Console/CConsole/src/CConsole.h"
#include "../../../../Config/PGEcfgProfiles.h"
#include "PureIRenderer.h"
#include "../Display/PureScreen.h"
#include "../Display/PureWindow.h"
#include "../Hardware/PureHwInfo.h"


/**
    Incremental software renderer path.
    Only deviations from the original PureIRenderer documentation are mentioned here.
*/
class PureRendererSWincremental :
    public PureIRenderer
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureRendererSWincremental is included")
#endif

public:
    /** Default render hints for PureRendererSWincremental. */
    static const TPURE_RENDER_HINT DefaultHints = 0;

    static PureRendererSWincremental& createAndGet(
        PGEcfgProfiles& cfgProfiles,
        PureWindow& _wnd,
        PureScreen& _scr,
        PureHwInfo& _hwinfo );    /**< Creates and gets the singleton implementation instance. */

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    /**
        Initializes the software renderer.
 
        @param width   Same as described in PureIRenderer::initialize().
        @param height  Same as described in PureIRenderer::initialize().
        @param dmode   Same as described in PureIRenderer::initialize().
        @param freq    Same as described in PureIRenderer::initialize().
        @param cdepth  Same as described in PureIRenderer::initialize().
        @param zdepth  The Z-buffer-depth to be set. Currently Z-buffering is not implemented by this renderer.
        @param stencil The stencil buffer depth to be set. Currently Z-buffering is not implemented by this renderer.
        @param samples The level of antialiasing. Currently Z-buffering is not implemented by this renderer.
        @param window  Same as described in PureIRenderer::initialize().

        @return The result of the initialization. 0 on success, 1 otherwise.
    */
    virtual TPureUInt initialize(
        TPureUInt width, TPureUInt height,
        TPURE_DISPLAY_MODES dmode,
        TPureUInt freq, TPureInt cdepth,
        TPureInt zdepth, TPureInt stencil,
        TPureInt samples,
        HWND window = NULL) = 0;

    /**
        Not implemented.
        Use RenderScene() to render all objects.
    */
    virtual void RenderObject(PureObject3D& object) = 0;

    virtual void ShowGuiDemo(TPureBool state) = 0;


}; // PureRendererHWfixedPipe