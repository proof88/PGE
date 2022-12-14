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


bool PGESysSFX::initSysSFX(void)
{
    const SoLoud::result res = gSoloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::MINIAUDIO);
    const bool bRet = (SoLoud::SOLOUD_ERRORS::SO_NO_ERROR == res);
    if (bRet)
    {
        CConsole::getConsoleInstance("PGESysSFX").SOLn("Initialized SoLoud version %d!", SOLOUD_VERSION);
        CConsole::getConsoleInstance("PGESysSFX").OLn(
            "Backend ID: %d, Name: %s",
            gSoloud.getBackendId(),
            gSoloud.getBackendString());
        CConsole::getConsoleInstance("PGESysSFX").OLn(
            "Channels: %d, Sample Rate: %d Hz, Buffer Size: %d",
            gSoloud.getBackendChannels(),
            gSoloud.getBackendSamplerate(),
            gSoloud.getBackendBufferSize());
    }
    else
    {
        CConsole::getConsoleInstance("PGESysSFX").EOLn("Failed to initialize SoLoud version %d, code: %d!", SOLOUD_VERSION, res);
    }

    return bRet;
} // InitSysSFX()


bool PGESysSFX::destroySysSFX(void)
{
    gSoloud.deinit();
    return true;
} // DestroySysSFX()

SoLoud::Soloud& PGESysSFX::getAudioCore()
{
    return gSoloud;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGESysSFX::PGESysSFX(const PGESysSFX&)
{

}


PGESysSFX& PGESysSFX::operator=(const PGESysSFX&)
{
    return *this;
}

