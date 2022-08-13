#pragma once

/*
    ###################################################################################
    PGEincludes.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine usually used header files
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

// standard headers
// ---------------------------------------------------------------------------
#include <conio.h>
#include <direct.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>


// Windows header
// ---------------------------------------------------------------------------
#ifndef WINVER
//#define WINVER 0x0501
#define WINVER 0x0600
#endif
#ifndef _WIN32_WINNT                  
#define _WIN32_WINNT 0x0501
#endif                        
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif
#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>


// own headers
// ---------------------------------------------------------------------------
#include "PGEconsts.h"
#include "PGEallHeaders.h"
#include "../../PFL/PFL/PFL.h"
#include "../../CConsole/CConsole/src/CConsole.h"

