/*
    ###################################################################################
    PGESysSFX.cpp
    This file is part of PGE.
    PR00F's Game Engine sound subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PGESysSFX.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"


/*
   PGESysSFX
   ###########################################################################
*/


 // ############################### PUBLIC ################################


PGESysSFX::PGESysSFX()
{

} // PGESysSFX()


PGESysSFX::~PGESysSFX()
{

} // ~PGESysSFX()


void* PGESysSFX::initSysSFX(void)
{
    return PGENULL;
} // InitSysSFX()


bool PGESysSFX::destroySysSFX(void)
{
    return true;
} // DestroySysSFX()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGESysSFX::PGESysSFX(const PGESysSFX&)
{

}


PGESysSFX& PGESysSFX::operator=(const PGESysSFX&)
{
    return *this;
}

