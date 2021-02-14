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

    void* initSysSFX(void);
    bool  destroySysSFX(void);

private:

    // ---------------------------------------------------------------------------

    PGESysSFX(const PGESysSFX&); 
    PGESysSFX& operator=(const PGESysSFX&);

}; // class PGESysSFX
