#pragma once

/*
    ###################################################################################
    PRREbaseIncludes.h
    This file is part of PRRE.
    Internal header.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

/*
    This file MUSTN'T be included in header files!
    This file should be included ONLY in cpp files.
    This file contains includes and macros for convenience.
*/

// standard headers, never changed
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <algorithm>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <typeinfo>

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

// Own headers, rarely changed
#include "PRREallHeaders.h"
#include "PRREconsts.h"
#include "PRREtypes.h"
#include "../../PFL/PFL.h"
#include "../../CConsole/src/CConsole.h"
