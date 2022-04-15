/*
    ###################################################################################
    PGEcfgFile.cpp
    This file is part of PGE.
    PR00F's Game Engine configuration file handler base class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "PGEcfgFile.h"

/*
   PGEcfgFile
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @param bCaseSensitiveVars If true, all variables will be treated case sensitive, e.g. "Name =" will be different to "name =" or "NAME =".
    Note: it is your responsibility to define all accepted variables in their lowercafe form if you set case sensitivity to false!
*/
PGEcfgFile::PGEcfgFile(
    bool bRequireAllAcceptedVarsDefineRequirement,
    bool bCaseSensitiveVars)
{
    m_bRequireAllAcceptedVarsDefineRequirement = bRequireAllAcceptedVarsDefineRequirement;
    m_bCaseSensitiveVars = bCaseSensitiveVars;
}

PGEcfgFile::~PGEcfgFile()
{
}

CConsole& PGEcfgFile::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

const char* PGEcfgFile::getLoggerModuleName()
{
    return "PGEcfgFile";
}

std::map<std::string, PGEcfgVariable>& PGEcfgFile::getVars()
{
    return m_vars;
}

const std::map<std::string, PGEcfgVariable>& PGEcfgFile::getVars() const
{
    return m_vars;
}

bool PGEcfgFile::load(const char* fname)
{
    getConsole().OLnOI("PGEcfgFile::load(%s) ...", fname);

    if ( !m_vars.empty() )
    {
        getConsole().EOLnOO("ERROR: variables already present, not loading anything from %s!", fname);
        return false;
    }

    std::ifstream f;

    f.open (fname, std::ifstream::in);
    if ( !f.good() )
    {
        getConsole().EOLnOO("ERROR: failed to open file: %s!", fname);
        return false;
    }

    std::set<std::string> m_missingVars = m_acceptedVars;
    bool bParseError = false;
    const std::streamsize nBuffSize = 1024;
    char cLine[nBuffSize];
    while ( !bParseError && !f.eof() )
    {
        f.getline(cLine, nBuffSize);
        // TODO: we should finally have a strClr() version for std::string or FINALLY UPGRADE TO NEWER CPP THAT MAYBE HAS THIS FUNCTIONALITY!!!
        PFL::strClrLeads( cLine );
        const std::string sTrimmedLine(cLine);
        if ( lineShouldBeIgnored(sTrimmedLine) )
        {
            continue;
        }
        std::string sVar, sValue;
        if ( lineIsValueAssignment(sTrimmedLine, m_bCaseSensitiveVars, sVar, sValue, bParseError) )
        {
            lineHandleAssignment(sVar, sValue, fname, m_missingVars, bParseError);
        }
    };
    f.close();

    if ( bParseError )
    {
        getConsole().EOLnOO("ERROR: failed to parse file: %s!", fname);
        m_vars.clear();
        return false;
    }

    if ( m_bRequireAllAcceptedVarsDefineRequirement && !m_missingVars.empty() )
    {
        std::string sMissingVars;
        while ( !m_missingVars.empty() )
        {
            sMissingVars += *(m_missingVars.begin());
            m_missingVars.erase( m_missingVars.begin() );
            if ( !m_missingVars.empty() )
            {
                sMissingVars += ", ";
            }
        }
        getConsole().EOLnOO("ERROR: failed to parse file: %s, variable(s) missing: %s!", fname, sMissingVars.c_str());
        m_vars.clear();
        return false;
    }

    getConsole().SOLnOO("PGEcfgFile loaded!");
    return true;
}

bool PGEcfgFile::getAllAcceptedVarsDefineRequirement() const
{
    return m_bRequireAllAcceptedVarsDefineRequirement;
}

bool PGEcfgFile::getCaseSensitiveVars() const
{
    return m_bCaseSensitiveVars;
}

const std::set<std::string>& PGEcfgFile::getAcceptedVars() const
{
    return m_acceptedVars;
}

bool PGEcfgFile::setAcceptedVars(const std::set<std::string>& newAcceptedVars)
{
    if ( !m_acceptedVars.empty() || !m_vars.empty() )
    {
        getConsole().EOLn("setAcceptedVars() ERROR: accepted vars or vars are already defined!");
        return false;
    }

    m_acceptedVars = newAcceptedVars;
    return true;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


bool PGEcfgFile::lineShouldBeIgnored(const std::string& sTrimmedLine)
{
    return sTrimmedLine.empty() || (sTrimmedLine[0] == '#');
}

bool PGEcfgFile::lineIsValueAssignment(const std::string& sTrimmedLine, bool bCaseSensitiveVars, std::string& sVar, std::string& sValue, bool& bParseError)
{
    const std::string::size_type nAssignmentPos = sTrimmedLine.find('=');
    if ( nAssignmentPos == std::string::npos )
    {
        return false;
    }

    if ( nAssignmentPos == 0 )
    {
        CConsole::getConsoleInstance("PGEcfgFile").EOLn("ERROR: erroneous assignment: %s!", sTrimmedLine.c_str());
        bParseError = true;
        return false;
    }

    // sLine is already trimmed: neither leading nor trailing spaces

    // get rid of trailing spaces from the variable name itself, standing before the '=' char
    // TODO: should rather use std::string compatible PFL::strClr()
    std::string::size_type nSpPos = sTrimmedLine.find(' ');
    if ( nSpPos < nAssignmentPos )
    {
        sVar = sTrimmedLine.substr(0, nSpPos);
        if ( sVar.find(' ') != std::string::npos )
        {
            // we should not have more space before '=' char
            CConsole::getConsoleInstance("PGEcfgFile").EOLn("ERROR: erroneous assignment, failed to parse variable in line: %s!", sTrimmedLine.c_str());
            bParseError = true;
            return false;
        }
    }
    else
    {
        sVar = sTrimmedLine.substr(0, nAssignmentPos);
    }

    if ( !bCaseSensitiveVars )
    {
        std::transform(sVar.begin(), sVar.end(), sVar.begin(), ::tolower);
    }

    // get rid of leading spaces from the value itself, standing after the '=' char
    std::string::size_type i = nAssignmentPos+1;
    while ( (i < sTrimmedLine.length()) && sTrimmedLine[i] == ' ' )
    {
        i++;
    }

    if ( i < sTrimmedLine.length() )
    {
        sValue = sTrimmedLine.substr(i);
    }
    else
    {
        // we reached EOL while trying to trim the value, this means that:
        // - either value is full of spaces, which is a valid case,
        // - or value is empty, which is also a valid case.
        // We can simply get substr from (pos of '=')+1, since that SHOULD NOT throw exception:
        // exception is thrown if pos > size(), but our case is: pos == size().
        sValue = sTrimmedLine.substr(nAssignmentPos+1);
    }

    return true;
}

void PGEcfgFile::lineHandleAssignment(const std::string& sVar, const std::string& sValue, const char* fname, std::set<std::string>& m_missingVars, bool& bParseError)
{
    getConsole().OLn("Var \"%s\" = \"%s\"", sVar.c_str(), sValue.c_str());
    
    if ( !m_acceptedVars.empty() && (m_acceptedVars.end() == m_acceptedVars.find(sVar)) )
    {
        getConsole().EOLnOO("ERROR: setting unknown/unaccepted variable %s in file %s!", sVar.c_str(), fname);
        bParseError = true;
        return;
    }

    const auto it_vars = getVars().find(sVar);
    if ( it_vars != getVars().end() )
    {
        getConsole().EOLnOO("ERROR: variable %s in file %s has been already set previously (defined multiple times)!", sVar.c_str(), fname);
        bParseError = true;
        return;
    } 

    const auto it_missing = m_missingVars.find(sVar);
    if ( it_missing != m_missingVars.end() )
    {
        m_missingVars.erase(it_missing);
    }
  
    getVars()[sVar] = sValue.c_str();
}

