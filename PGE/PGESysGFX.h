#pragma once

/*
    ###################################################################################
    PGESysGFX.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine graphics subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"
#include "PRRE/PR00FsReducedRenderingEngine.h"
// #include "PURE/PURE.h"

/**
    This class doesn't use any API directly.
*/
class PGESysGFX
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGESysGFX is included")   
#endif

public:
    PGESysGFX();
    virtual ~PGESysGFX();

    bool initSysGFX(
        int width, int height, TPRRE_DISPLAY_MODES dmode, int freq,
        int cdepth, int zdepth, int stencil, int samples );

    bool destroySysGFX(void);

private:
    PR00FsReducedRenderingEngine& engine;

    // ---------------------------------------------------------------------------

    PGESysGFX(const PGESysGFX&); 
    PGESysGFX& operator=(const PGESysGFX&);

}; // class PGESysGFX
