#pragma once

/*
    ###################################################################################
    PurehwCentralProcessor.h
    This file is part of PURE.
    External header.
    Pure central processor capabilities class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PurehwBase.h"


/**
    Pure central processor capabilities class.
*/
class PurehwCentralProcessor :
    public PurehwBase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PurehwCentralProcessor is included")
#endif

public:
    static PurehwCentralProcessor& get();  /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual void WriteStats() = 0;         /**< Writes statistics to the console. */   

};
