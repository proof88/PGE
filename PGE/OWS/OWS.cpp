/*
    ###################################################################################
    OWS.cpp
    This file is part of PGE.
    PR00F's Game Engine Open Weapon Subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "OWS.h"

/*
   Weapon
   ###########################################################################
*/


// ############################### PUBLIC ################################


Weapon::Weapon()
{

}

Weapon::~Weapon()
{

}

CConsole& Weapon::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

const char* Weapon::getLoggerModuleName()
{
    return "Weapon";
}

std::map<std::string, PGEcfgVariable>& Weapon::getVars()
{
    return m_vars;
}

const std::map<std::string, PGEcfgVariable>& Weapon::getVars() const
{
    return m_vars;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/*
   OWS
   ###########################################################################
*/


// ############################### PUBLIC ################################


OWS::OWS(PR00FsReducedRenderingEngine& gfx) :
    m_gfx(gfx)
{

}

OWS::~OWS()
{

}

CConsole& OWS::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

const char* OWS::getLoggerModuleName()
{
    return "OWS";
}

bool OWS::load(const char* fname)
{
    getConsole().OLnOI("OWS::load(%s) ...", fname);
    std::ifstream f;

    f.open (fname, std::ifstream::in);
    if ( !f.good() )
    {
        getConsole().EOLnOO("ERROR: failed to open file!");
        return false;
    }

    bool bParseError = false;
    const std::streamsize nBuffSize = 1024;
    char cLine[nBuffSize];
    Weapon wpn;
    while ( !bParseError && !f.eof() )
    {
        f.getline(cLine, nBuffSize);
        // TODO: we should finally have a strClr() version for std::string or FINALLY UPGRADE TO NEWER CPP THAT MAYBE HAS THIS FUNCTIONALITY!!!
        PFL::strClr( cLine );
        const std::string sLine(cLine);
        std::string sVar, sValue;
        if ( lineShouldBeIgnored(sLine) )
        {
            continue;
        }
        if ( lineIsValueAssignment(sLine, sVar, sValue, bParseError) )
        {
            getConsole().OLn("Var \"%s\" = \"%s\"", sVar.c_str(), sValue.c_str());
            wpn.getVars()[sVar] = sValue.c_str();
        }
    };
    f.close();

    if ( bParseError )
    {
        getConsole().EOLnOO("ERROR: failed to parse file!");
        return false;
    }

    m_weapons.push_back(wpn);
    getConsole().SOLnOO("Weapon loaded!");
    return true;
}

const std::vector<Weapon>& OWS::getWeapons() const
{
    return m_weapons;
}

void OWS::Clear()
{
    m_weapons.clear();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


bool OWS::lineShouldBeIgnored(const std::string& sLine)
{
    return sLine.empty() || (sLine[0] == '#');
}

bool OWS::lineIsValueAssignment(const std::string& sLine, std::string& sVar, std::string& sValue, bool& bParseError)
{
    const std::string::size_type nAssignmentPos = sLine.find('=');
    if ( nAssignmentPos == std::string::npos )
    {
        return false;
    }

    if ( (nAssignmentPos == (sLine.length() - 1)) || (nAssignmentPos == 0 ) )
    {
        CConsole::getConsoleInstance("OWS").EOLn("ERROR: erroneous assignment: %s!", sLine.c_str());
        bParseError = true;
        return false;
    }

    // sLine is already trimmed: neither leading nor trailing spaces

    // get rid of trailing spaces from the variable name itself, standing before the '=' char
    // TODO: should rather use std::string compatible PFL::strClr()
    std::string::size_type nSpPos = sLine.find(' ');
    if ( nSpPos != std::string::npos )
    {
        if ( nSpPos < nAssignmentPos )
        {
            sVar = sLine.substr(0, nSpPos);
            if ( sVar.find(' ') != std::string::npos )
            {
                // we should not have more space before '=' char
                CConsole::getConsoleInstance("OWS").EOLn("ERROR: erroneous assignment, failed to parse variable in line: %s!", sLine.c_str());
                bParseError = true;
                return false;
            }
        }
        else
        {
            // should never reach this point based on above 2 conditions
            CConsole::getConsoleInstance("OWS").EOLn("ERROR: erroneous assignment: %s!", sLine.c_str());
            bParseError = true;
            return false;
        }
    }
    else
    {
        sVar = sLine.substr(0, nAssignmentPos);
    }

    // get rid of leading spaces from the value itself, standing after the '=' char
    std::string::size_type i = nAssignmentPos+1;
    while ( (i < sLine.length()) && sLine[i] == ' ' )
    {
        i++;
    }

    if ( i < sLine.length() )
    {
        sValue = sLine.substr(i);
    }
    else
    {
        CConsole::getConsoleInstance("OWS").EOLn("ERROR: erroneous assignment, failed to parse value in line: %s!", sLine.c_str());
        bParseError = true;
        return false;
    }

    return true;
}

