#pragma once

/*
    ###################################################################################
    PureHwAudio.h
    This file is part of PURE.
    External header.
    Pure audio capabilities class.
    Made by PR00F88
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PureHwBase.h"


/**
    Pure audio capabilities class.
*/
class PureHwAudio :
    public PureHwBase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwAudio is included")
#endif

public:

    static PureHwAudio& get();        /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

};
