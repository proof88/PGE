#pragma once

/*
    ###################################################################################
    PRREhwSystemMemory.h
    This file is part of PRRE.
    External header.
    PRRE system memory info class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "PRREhwBase.h"


/**
    PRRE system memory info class.

    This class directly uses WinAPI.
*/
class PRREhwSystemMemory :
    public PRREhwBase
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwSystemMemory is included")
#endif

public:
    static PRREhwSystemMemory& get();        /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    virtual TPRREulong getFreeSystemMemory() = 0;         /**< Gets the amount of free physical memory. */
    virtual TPRREulong getTotalSystemMemory() const = 0;  /**< Gets the total amount of physical memory. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

};
