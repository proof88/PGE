#pragma once

/*
    ###################################################################################
    PRREhwInfo.h
    This file is part of PRRE.
    External header.
    PRRE hardware information class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "../PRREBaseClass.h"
#include "PRREhwCentralProcessor.h"
#include "PRREhwSystemMemory.h"
#include "PRREhwAudio.h"
#include "PRREhwVideo.h"


/**
    PRRE hardware information class.
*/
class PRREhwInfo :
    public PRREBaseClass
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREhwInfo is included")
#endif

public:
    static PRREhwInfo& get();  /**< Gets the singleton instance. */

    // ---------------------------------------------------------------------------

    /** Sets members to real values within the singleton instance. */
    virtual void      Initialize(
        HGLRC rc, HDC wnd_dc,
        TPRREuint nResX, TPRREuint nResY,
        TPRREint nColorBits, TPRREint nDepthBits,
        TPRREint nStencilBits, TPRREint nFSAAlevel ) = 0;
    virtual void      Deinitialize() = 0;         /**< Deinitializes the singleton instance. */
    virtual TPRREbool isInitialized() const = 0;  /**< Returns whether the singleton instance is successfully initialized. */

    virtual PRREhwCentralProcessor& getCentralProcessor() const = 0;  /**< Gives access to PRREhwCentralProcessor instance. */
    virtual PRREhwSystemMemory&     getMemory() const = 0;            /**< Gives access to PRREhwSystemMemory instance. */
    virtual PRREhwVideo&            getVideo() const = 0;             /**< Gives access to PRREhwVideo instance. */
    virtual PRREhwAudio&            getAudio() const = 0;             /**< Gives access to PRREhwAudio instance. */

    virtual void WriteStats() = 0;    /**< Writes statistics to the console. */

}; // class PRREhwInfo
