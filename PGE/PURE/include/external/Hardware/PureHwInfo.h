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
#include "PurehwCentralProcessor.h"
#include "PurehwSystemMemory.h"
#include "PurehwAudio.h"
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

    virtual PurehwCentralProcessor& getCentralProcessor() const = 0;  /**< Gives access to PurehwCentralProcessor instance. */
    virtual PurehwSystemMemory&     getMemory() const = 0;            /**< Gives access to PurehwSystemMemory instance. */
    virtual PureHwVideo&            getVideo() const = 0;             /**< Gives access to PureHwVideo instance. */
    virtual PurehwAudio&            getAudio() const = 0;             /**< Gives access to PurehwAudio instance. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

}; // class PureHwInfo
