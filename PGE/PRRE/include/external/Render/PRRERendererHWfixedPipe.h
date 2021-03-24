#pragma once

/*
    ###################################################################################
    PRRERendererHWfixedPipe.h
    This file is part of PRRE.
    External header.
    Fixed-Pipeline HW Renderer class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../../../../../CConsole/CConsole/src/CConsole.h"
#include "PRREIRenderer.h"
#include "../Display/PRREScreen.h"
#include "../Display/PRREWindow.h"
#include "../Hardware/PRREhwInfo.h"

/**
    Incremental fixed-function hardware renderer path.
    Only deviations from the original PRREIRenderer documentation are mentioned here.
*/
class PRRERendererHWfixedPipe :
    public PRREIRenderer
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRRERendererHWfixedPipe is included")
#endif

public:
    static PRRERendererHWfixedPipe& createAndGet(
        PRREWindow& _wnd,
        PRREScreen& _scr,
        PRREhwInfo& _hwinfo );    /**< Creates and gets the singleton implementation instance. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */
    const char* getLoggerModuleName() const;          /**< Returns the logger module name of this class. */

    /**
        Not implemented.
        Use RenderScene() to render all objects.
    */
    virtual void RenderObject(PRREObject3D& object) = 0;


}; // PRRERendererHWfixedPipe

