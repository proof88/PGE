#pragma once

/*
    ###################################################################################
    PureBaseIncludes.h
    This file is part of PURE.
    Internal PRECOMPILED HEADER.
    Made by PR00F88
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
#include <fstream>
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

#ifndef WINPROOF88_ALLOW_CONTROLS_AND_DIALOGS
#define WINPROOF88_ALLOW_CONTROLS_AND_DIALOGS
#endif
#ifndef WINPROOF88_ALLOW_GDI
#define WINPROOF88_ALLOW_GDI
#endif
#ifndef WINPROOF88_ALLOW_MESSAGEBOX
#define WINPROOF88_ALLOW_MESSAGEBOX
#endif
#ifndef WINPROOF88_ALLOW_MSG_USER_WINMESSAGES
#define WINPROOF88_ALLOW_MSG_USER_WINMESSAGES
#endif
#ifndef WINPROOF88_ALLOW_RASTERPOS
#define WINPROOF88_ALLOW_RASTERPOS
#endif
#ifndef WINPROOF88_ALLOW_SHOWWINDOW
#define WINPROOF88_ALLOW_SHOWWINDOW
#endif
#ifndef WINPROOF88_ALLOW_SOUND
#define WINPROOF88_ALLOW_SOUND
#endif
#ifndef WINPROOF88_ALLOW_SYSCOMMANDS_E_G_SCREENSAVER
#define WINPROOF88_ALLOW_SYSCOMMANDS_E_G_SCREENSAVER
#endif
#ifndef WINPROOF88_ALLOW_SYSMETRICS
#define WINPROOF88_ALLOW_SYSMETRICS
#endif
#ifndef WINPROOF88_ALLOW_WINOFFSETS
#define WINPROOF88_ALLOW_WINOFFSETS
#endif
#ifndef WINPROOF88_ALLOW_WINSTYLES
#define WINPROOF88_ALLOW_WINSTYLES
#endif
#include "../../../../../../../PFL/PFL/winproof88.h"

// GL headers, never changed
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glext.h"
#include "gl/wglext.h"

// Own headers, but rarely changed, so we can keep them here in PCH
#include "../external/PureAllHeaders.h"
#include "PureConsts.h"
#include "../external/PureTypes.h"
#include "../../../../../PFL/PFL/PFL.h"
#include "../../../../../Console/CConsole/src/CConsole.h"
