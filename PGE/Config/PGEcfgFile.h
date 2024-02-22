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

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../PGEallHeaders.h"
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

    bool load(const char* fname);                      /**< Loads variables from the given config file. */
    bool save(const char* fname = "") const;           /**< Saves variables to the given config file. */

    const std::string& getFilename() const;
    const std::string& getPathToFile() const;

    bool getAllAcceptedVarsDefineRequirement() const;
    bool getCaseSensitiveVars() const;

    const std::set<std::string>& getAcceptedVars() const;
    bool setAcceptedVars(const std::set<std::string>& newAcceptedVars);

    std::vector<std::string>& getTemplate();               /**< Returns the lines of the template generated from the loaded config file. */
    const std::vector<std::string>& getTemplate() const;   /**< Returns the lines of the template generated from the loaded config file. */

    PGEcfgFile(const PGEcfgFile& other) : // TODO check if we really cannot live with just compiler generated copy ctor?
        m_acceptedVars(other.m_acceptedVars),
        m_vars(other.m_vars),
        m_bRequireAllAcceptedVarsDefineRequirement(other.m_bRequireAllAcceptedVarsDefineRequirement),
        m_bCaseSensitiveVars(other.m_bCaseSensitiveVars),
        m_sFilename(other.m_sFilename)
    {}

    PGEcfgFile& operator=(const PGEcfgFile& other) // TODO check if we really cannot live with just compiler generated operator=?
    {
        m_acceptedVars = other.m_acceptedVars;
        m_vars = other.m_vars;
        m_bRequireAllAcceptedVarsDefineRequirement = other.m_bRequireAllAcceptedVarsDefineRequirement;
        m_bCaseSensitiveVars = other.m_bCaseSensitiveVars;
        m_sFilename = other.m_sFilename;
        return *this;
    }

protected:

    std::set<std::string> m_acceptedVars;
    std::map<std::string, PGEcfgVariable> m_vars;

    // this is not private so PGEcfgProfiles::getPlayerNameFromFile() can access it, otherwise this should be private
    static bool lineIsValueAssignment(const std::string& sTrimmedLine, bool bCaseSensitiveVars, std::string& sVar, std::string& sValue, bool& bParseError);

    virtual bool validateOnLoad(std::ifstream&) const; /**< Validate the file being processed by load(). */
    virtual bool validateOnSave(std::ofstream&) const; /**< Validate the file being processed by save(). */

private:

    bool m_bRequireAllAcceptedVarsDefineRequirement;
    bool m_bCaseSensitiveVars;

    std::string m_sFilename;
    std::string m_sPathToFile;
    std::vector<std::string> m_vTemplateLines;

    // ---------------------------------------------------------------------------

    static bool lineIsComment(const std::string& sTrimmedLine);
    static bool lineShouldBeIgnored(const std::string& sTrimmedLine);

    void lineHandleAssignment(const std::string& sVar, const std::string& sValue, const char* fname, std::set<std::string>& m_missingVars, bool& bParseError);

}; // class PGEcfgFile
