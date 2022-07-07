/*
    ###################################################################################
    PGESysNET.cpp
    This file is part of PGE.
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PGESysNET.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"


/*
   PGESysNET
   ###########################################################################
*/


 // ############################### PUBLIC ################################


PGESysNET::PGESysNET()
{

} // PGESysNET()


PGESysNET::~PGESysNET()
{

} // ~PGESysNET()


bool PGESysNET::initSysNET(void)
{
    SteamDatagramErrMsg errMsg;
    const bool bRet = GameNetworkingSockets_Init(nullptr, errMsg);
    if (bRet)
    {
        CConsole::getConsoleInstance("PGESysNET").SOLn("Initialized GameNetworkingSockets %s!", GAMENETWORKINGSOCKETS_VER_STR);
    }
    else
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("Failed to initialize GameNetworkingSockets %s: %s!", GAMENETWORKINGSOCKETS_VER_STR, errMsg);
    }

    return bRet;
} // initSysNET()


bool PGESysNET::destroySysNET(void)
{
    GameNetworkingSockets_Kill();
    return true;
} // destroySysNET()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGESysNET::PGESysNET(const PGESysNET&)
{

}


PGESysNET& PGESysNET::operator=(const PGESysNET&)
{
    return *this;
}

