/*
    ###################################################################################
    PGESysGFX.cpp
    This file is part of PGE.
    PR00F's Game Engine graphics subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PGESysGFX.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"


/*
   PGESysGFX
   ###########################################################################
*/


 // ############################### PUBLIC ################################


PGESysGFX::PGESysGFX() :
    engine( PR00FsReducedRenderingEngine::createAndGet() )
{

} // PGESysGFX()


PGESysGFX::~PGESysGFX()
{
    engine.shutdown();
} // ~PGESysGFX()


bool PGESysGFX::initSysGFX(
    int width, int height, TPRRE_DISPLAY_MODES dmode, int freq,
    int cdepth, int zdepth, int stencil, int samples )
{
    return ( engine.initialize( /*PRRE_RENDERER_SW*/ PRRE_RENDERER_HW_FP, width, height, dmode, freq, cdepth, zdepth, stencil, samples) == 0 );
} // initSysGFX()


bool PGESysGFX::destroySysGFX(void)
{
    this->~PGESysGFX();
    return true;
} // destroySysGFX()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGESysGFX::PGESysGFX(const PGESysGFX&) :
    engine( PR00FsReducedRenderingEngine::createAndGet() )
{

}


PGESysGFX& PGESysGFX::operator=(const PGESysGFX&)
{
    return *this;
}

