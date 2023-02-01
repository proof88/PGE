/*
    ###################################################################################
    PGESysGFX.cpp
    This file is part of PGE.
    PR00F's Game Engine graphics subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "PGESysGFX.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"


/*
   PGESysGFX
   ###########################################################################
*/


 // ############################### PUBLIC ################################


PGESysGFX::PGESysGFX(
    PGEcfgProfiles& cfgProfiles,
    PGEInputHandler& inputHandler) :
    m_cfgProfiles(cfgProfiles),
    m_inputHandler(inputHandler),
    engine( PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler) )
{

} // PGESysGFX()


PGESysGFX::~PGESysGFX()
{
    engine.shutdown();
} // ~PGESysGFX()


bool PGESysGFX::initSysGFX(
    unsigned int width, unsigned int height, TPURE_DISPLAY_MODES dmode, unsigned int freq,
    int cdepth, int zdepth, int stencil, int samples )
{
    return ( engine.initialize( /*PURE_RENDERER_SW*/ PURE_RENDERER_HW_FP, width, height, dmode, freq, cdepth, zdepth, stencil, samples) == 0 );
} // initSysGFX()


bool PGESysGFX::destroySysGFX(void)
{
    this->~PGESysGFX();
    return true;
} // destroySysGFX()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGESysGFX::PGESysGFX(const PGESysGFX& other) :
    m_cfgProfiles(other.m_cfgProfiles),
    m_inputHandler(other.m_inputHandler),
    engine( PR00FsUltimateRenderingEngine::createAndGet(other.m_cfgProfiles, other.m_inputHandler) )
{

}


PGESysGFX& PGESysGFX::operator=(const PGESysGFX&)
{
    return *this;
}

