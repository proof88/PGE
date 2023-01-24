#pragma once

/*
    ###################################################################################
    PureHwCentralProcessor.h
    This file is part of PURE.
    External header.
    Pure central processor capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PureHwBase.h"


/**
    Pure central processor capabilities class.
*/
class PureHwCentralProcessor :
    public PureHwBase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwCentralProcessor is included")
#endif

public:
    static PureHwCentralProcessor& get();  /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual void WriteStats() = 0;         /**< Writes statistics to the console. */   

};
