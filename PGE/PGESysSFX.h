#pragma once

/*
    ###################################################################################
    PGESysSFX.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine sound subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"

// backends for SoLoud: https://sol.gfxile.net/soloud/backends.html
// in case of problem with MiniAudio, try PortAudio!
#define WITH_MINIAUDIO
// TODO: add "Audio/soloud-RELEASE_20200207/include/" to project include dirs, do not use that path here!
#include "Audio/soloud-RELEASE_20200207/include/soloud.h"

/**
    PR00F's Game Engine sound subsystem.
*/
class PGESysSFX
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGESysSFX is included")   
#endif

public:
    PGESysSFX();
    virtual ~PGESysSFX();

    bool initSysSFX(void);
    bool destroySysSFX(void);

private:
    SoLoud::Soloud gSoloud;

    // ---------------------------------------------------------------------------

    PGESysSFX(const PGESysSFX&); 
    PGESysSFX& operator=(const PGESysSFX&);

}; // class PGESysSFX
