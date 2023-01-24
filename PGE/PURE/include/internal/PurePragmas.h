#pragma once

/*
    ###################################################################################
    include/internal/PurePragmas.h
    This file is part of PURE.
    Internal header.
    Pure pragmas.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

/*
    This file MUSTN'T be included in header files!
    This file should be included ONLY in cpp files.
    This file contains hints to the compiler about what warnings should be suppressed.
*/

#include "../external/PureAllHeaders.h"

/**
    Hides "unreferenced formal parameter" warnings.
*/
#pragma warning(disable:4100)

/**
    Hides "assignment within conditional expression".
    We intentionally do this a lot. Doesn't make code readability really worse. Or if so, it is still cool.
*/
#pragma warning(disable:4706)

/**
    Hides "may be unsafe" and "deprecated" warnings.
    There are some safer forms of some standard functions such as sscanf (vs sscanf_s), however
    these are mostly supported by MS compiler and may not be properly supported by other compilers like GNU C/CPP.
    Let's review this once we upgrade the dev environment in 2020.
*/
#pragma warning(disable:4996)
