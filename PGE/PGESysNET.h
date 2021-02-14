#pragma once

/*
    ###################################################################################
    PGESysNET.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"

/**
    PR00F's Game Engine networking subsystem.
*/
class PGESysNET
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGESysNET is included")   
#endif

public:
    PGESysNET();
    virtual ~PGESysNET();

    void* initSysNET(void);
    bool  destroySysNET(void);

private:

    // ---------------------------------------------------------------------------

    PGESysNET(const PGESysNET&); 
    PGESysNET& operator=(const PGESysNET&);

}; // class PGESysNET