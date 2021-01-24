#pragma once

/*
    ###################################################################################
    PRREbaseIncludes.h
    This file is part of PRRE.
    Internal PRECOMPILED HEADER.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

/*
    This file MUSTN'T be included in header files!
    This file should be included ONLY in cpp files.
    This file contains includes and macros for convenience.
    Used as PCH (Precompiled header).
*/

// standard C headers, never changed
#include <conio.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// standard C++ headers, never changed
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

// Windows headers, never changed
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
#include <WinUser.h>
#include <windowsx.h>

// GL headers, never changed
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glext.h"
#include "gl/wglext.h"

// Own headers, but rarely changed, so we can keep them here in PCH
#include "../external/PRREallHeaders.h"
#include "PRREconsts.h"
#include "../external/PRREtypes.h"
#include "../../../../PFL/PFL/PFL.h"
#include "../../../../CConsole/CConsole/src/CConsole.h"
