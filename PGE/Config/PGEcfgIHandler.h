#pragma once

/*
    ###################################################################################
    PGEcfgIHandler.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine config handler interface
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PGEallHeaders.h"

/**
    PR00F's Game Engine profile config handler class.
*/
class PGEcfgIHandler
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEcfgIHandler is included")   
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    explicit PGEcfgIHandler(const char* gameTitle);
    virtual ~PGEcfgIHandler();

    CConsole& getConsole() const;                   /**< Returns access to console preset with logger module name as this class. */

    virtual std::string getConfigLines() = 0;
    virtual bool processConfigLine(std::string sLine) = 0;
    virtual const container& getHandledConfigVars() = 0;

protected:

private:

    // ---------------------------------------------------------------------------

}; // class PGEcfgIHandler
