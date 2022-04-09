#pragma once

/*
    ###################################################################################
    PGEcfgFile.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine configuration file handler base class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <map>
#include <set>
#include <string>
#include <vector>

#include "PGEallHeaders.h"
#include "PGEcfgVariable.h"

/**
    PR00F's Game Engine configuration file handler base class.
    This class can load configuration variable data from simple ascii text files and save to file as well.
    Further logic is added by derived classes.
*/
class PGEcfgFile
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGEcfgFile is included")   
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PGEcfgFile(
        bool bRequireAllAcceptedVarsDefineRequirement,
        bool bCaseSensitiveVars);
    virtual ~PGEcfgFile();

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    std::map<std::string, PGEcfgVariable>& getVars();
    const std::map<std::string, PGEcfgVariable>& getVars() const;

    bool load(const char* fname);

    bool getAllAcceptedVarsDefineRequirement() const;
    bool getCaseSensitiveVars() const;

    std::set<std::string>& getAcceptedVars();
    const std::set<std::string>& getAcceptedVars() const;
    void setAcceptedVars(const std::set<std::string>& newAcceptedVars);

protected:

    std::set<std::string> m_acceptedVars;
    std::map<std::string, PGEcfgVariable> m_vars;

private:

    bool m_bRequireAllAcceptedVarsDefineRequirement;
    bool m_bCaseSensitiveVars;

    // ---------------------------------------------------------------------------

    static bool lineShouldBeIgnored(const std::string& sTrimmedLine);
    static bool lineIsValueAssignment(const std::string& sTrimmedLine, bool bCaseSensitiveVars, std::string& sVar, std::string& sValue, bool& bParseError);

    void lineHandleAssignment(const std::string& sVar, const std::string& sValue, const char* fname, std::set<std::string>& m_missingVars, bool& bParseError);

}; // class PGEcfgFile
