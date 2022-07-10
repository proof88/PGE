#pragma once

/*
    ###################################################################################
    PGESysNET.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"

// this idea of building include paths is coming from:
// https://stackoverflow.com/questions/32066204/construct-path-for-include-directive-with-macro
#define IDENT(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
#define PATH2(x,y) STR(IDENT(x)IDENT(y))
#define PATH3(x,y,z) STR(IDENT(x)IDENT(y)IDENT(z))

#define GAMENETWORKINGSOCKETS_VER 1.4.0
#define GAMENETWORKINGSOCKETS_VER_STR STR(GAMENETWORKINGSOCKETS_VER)

// TODO: add "Network/GameNetworkingSockets-1.4.0/include" to project include dirs, do not use that path here!
// TODO: to log lib version, extract version from %AdditionalIncludeDirectories
#include PATH3(Network/GameNetworkingSockets-,GAMENETWORKINGSOCKETS_VER,/include/steam/steamnetworkingsockets.h)
#include PATH3(Network/GameNetworkingSockets-,GAMENETWORKINGSOCKETS_VER,/include/steam/isteamnetworkingutils.h)

/**
    PR00F's Game Engine networking subsystem.
*/
class PGESysNET
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGESysNET is included")   
#endif

public:
    PGESysNET();
    virtual ~PGESysNET();

    bool initSysNET(void);
    bool destroySysNET(void);

private:

    // ---------------------------------------------------------------------------

    PGESysNET(const PGESysNET&); 
    PGESysNET& operator=(const PGESysNET&);

}; // class PGESysNET