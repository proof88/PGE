#pragma once

/*
    ###################################################################################
    PureHwInfo.h
    This file is part of PURE.
    External header.
    Pure hardware information class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PureAllHeaders.h"
#include "PureHwCentralProcessor.h"
#include "PureHwSystemMemory.h"
#include "PureHwAudio.h"
#include "PureHwVideo.h"


/**
    Pure hardware information class.
*/
class PureHwInfo
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureHwInfo is included")
#endif

public:
    static PureHwInfo& get();  /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    /** Sets members to real values within the singleton instance. */
    virtual void      Initialize(
        HGLRC rc, HDC wnd_dc,
        TPureUInt nResX, TPureUInt nResY,
        TPureInt nColorBits, TPureInt nDepthBits,
        TPureInt nStencilBits, TPureInt nFSAAlevel ) = 0;
    virtual void      Deinitialize() = 0;         /**< Deinitializes the singleton instance. */
    virtual TPureBool isInitialized() const = 0;  /**< Returns whether the singleton instance is successfully initialized. */

    virtual PureHwCentralProcessor& getCentralProcessor() const = 0;  /**< Gives access to PureHwCentralProcessor instance. */
    virtual PureHwSystemMemory&     getMemory() const = 0;            /**< Gives access to PureHwSystemMemory instance. */
    virtual PureHwVideo&            getVideo() const = 0;             /**< Gives access to PureHwVideo instance. */
    virtual PureHwAudio&            getAudio() const = 0;             /**< Gives access to PureHwAudio instance. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

}; // class PureHwInfo
