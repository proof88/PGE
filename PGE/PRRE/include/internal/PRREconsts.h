#pragma once

/*
    ###################################################################################
    PRREconsts.h
    This file is part of PRRE.
    Internal header.
    PRRE constants, macros.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../external/PRREallHeaders.h"


/*
    This file MUSTN'T be included in EXTERNAL header files!
*/


/**
    If PGE_CCONSOLE_IS_ENABLED macro is defined, there will be console window created and printouts will be visible, otherwise not.
*/
#ifndef PGE_CCONSOLE_IS_ENABLED
#define PGE_CCONSOLE_IS_ENABLED
#endif

#define PRRE_NAME    "PR00F's Reduced Rendering Engine (PRRE)"           /**< Name of the engine. */
#define PRRE_VERSION "0.3 private alpha built on "__DATE__" @ "__TIME__  /**< Version of the engine. */


