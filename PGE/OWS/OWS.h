#pragma once

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

#include "../PGEallHeaders.h"
#include "../PGEcfgVariable.h"
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"


/**
    PR00F's Game Engine Open Weapon Subsystem
*/
class OWS
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  Open Weapon Subsystem is included")   
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    OWS(PR00FsReducedRenderingEngine& gfx);
    virtual ~OWS();

    CConsole&   getConsole() const;                   /**< Returns access to console preset with logger module name as this class. */

protected:

    OWS(const OWS&) :
        m_gfx(m_gfx)
    {}

    OWS& operator=(const OWS&)
    {
        return *this;
    }

private:

    PR00FsReducedRenderingEngine& m_gfx;

    // ---------------------------------------------------------------------------

    OWS();

}; // class OWS
