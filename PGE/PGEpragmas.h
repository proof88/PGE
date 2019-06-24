#pragma once

/*
    ###################################################################################
    PGEpragmas.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine pragmas
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

/*
    This file MUSTN'T be included in header files!
    This file should be included ONLY in cpp files.
    We use maximum compiler warning level, but we don't want to see all warnings hence we suppress some of them.
    This file contains hints to the compiler about what warnings should be suppressed.
*/

#include "PGEallHeaders.h"


/**
    Hides "may be unsafe" and "deprecated" warnings.
*/
#pragma warning(disable:4996)

/**
    Hides "conversion from 'size_t' to '...', possible loss of data".
*/
#pragma warning(disable:4267)

/**
    Hides "pointer truncation".
*/
#pragma warning(disable:4311)

