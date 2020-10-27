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


void* PGESysNET::initSysNET(void)
{
    return PGENULL;
} // initSysNET()


bool PGESysNET::destroySysNET(void)
{
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

