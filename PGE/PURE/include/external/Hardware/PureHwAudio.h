#pragma once

/*
    ###################################################################################
    PurehwAudio.h
    This file is part of PURE.
    External header.
    Pure audio capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PurehwBase.h"


/**
    Pure audio capabilities class.
*/
class PurehwAudio :
    public PurehwBase
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PurehwAudio is included")
#endif

public:

    static PurehwAudio& get();        /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

};
