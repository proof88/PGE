#pragma once

/*
    ###################################################################################
    PureHwSystemMemory.h
    This file is part of PURE.
    External header.
    Pure system memory info class.
    Made by PR00F88
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PureHwBase.h"


/**
    Pure system memory info class.
*/
class PureHwSystemMemory :
    public PureHwBase
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwSystemMemory is included")
#endif

public:
    static PureHwSystemMemory& get();        /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual TPureULong getFreeSystemMemory() = 0;         /**< Gets the amount of free physical memory. */
    virtual TPureULong getTotalSystemMemory() const = 0;  /**< Gets the total amount of physical memory. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

};
