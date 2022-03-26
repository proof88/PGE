/*
    ###################################################################################
    OWS.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine Open Weapon Subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "stdafx.h"  // PCH
#include "OWS.h"

// ############################### PUBLIC ################################


OWS::OWS(PR00FsReducedRenderingEngine& gfx) :
    m_gfx(gfx)
{

}

OWS::~OWS()
{

}

CConsole& OWS::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

const char* OWS::getLoggerModuleName()
{
    return "OWS";
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


