#pragma once

/*
    ###################################################################################
    PRREhwAudio.h
    This file is part of PRRE.
    External header.
    PRRE audio capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREallHeaders.h"
#include "PRREhwBase.h"


/**
    PRRE audio capabilities class.

    This class doesn't use any API directly.
*/
class PRREhwAudio :
    public PRREhwBase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwAudio is included")
#endif

public:

    static PRREhwAudio& get();        /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

};
