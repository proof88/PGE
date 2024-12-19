#pragma once

/*
    ###################################################################################
    PureConsts.h
    This file is part of PURE.
    Internal header.
    Pure constants, macros.
    Made by PR00F88
    ###################################################################################
*/

#include "../external/PureAllHeaders.h"


/*
    This file MUSTN'T be included in EXTERNAL header files!
*/


/**
    If PGE_CCONSOLE_IS_ENABLED macro is defined, there will be console window created and printouts will be visible, otherwise not.
*/
#ifndef PGE_CCONSOLE_IS_ENABLED
#define PGE_CCONSOLE_IS_ENABLED
#endif

#define PURE_NAME    "PR00F's Ultimate Rendering Engine (PURE)"
#define PURE_VERSION "0.5 private alpha built on "__DATE__" @ "__TIME__
