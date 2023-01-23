#pragma once

/*
    ###################################################################################
    PurehwSystemMemory.h
    This file is part of PURE.
    External header.
    Pure system memory info class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PurehwBase.h"


/**
    Pure system memory info class.
*/
class PurehwSystemMemory :
    public PurehwBase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PurehwSystemMemory is included")
#endif

public:
    static PurehwSystemMemory& get();        /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual TPureULong getFreeSystemMemory() = 0;         /**< Gets the amount of free physical memory. */
    virtual TPureULong getTotalSystemMemory() const = 0;  /**< Gets the total amount of physical memory. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

};
