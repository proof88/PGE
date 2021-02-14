#pragma once

/*
    ###################################################################################
    PRREhwCentralProcessor.h
    This file is part of PRRE.
    External header.
    PRRE central processor capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "PRREhwBase.h"


/**
    PRRE central processor capabilities class.
*/
class PRREhwCentralProcessor :
    public PRREhwBase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwCentralProcessor is included")
#endif

public:
    static PRREhwCentralProcessor& get();  /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual void WriteStats() = 0;         /**< Writes statistics to the console. */   

};
