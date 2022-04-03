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


Weapon::Weapon(const char* fname)
{
    if ( m_acceptedVars.empty() )
    {
        m_acceptedVars.insert("name");
        m_acceptedVars.insert("cap_max");
        m_acceptedVars.insert("cap_reload");
        m_acceptedVars.insert("reload_per_mag");
        m_acceptedVars.insert("reload_whole_mag");
        m_acceptedVars.insert("reload_time");
        m_acceptedVars.insert("firing_mode_def");
        m_acceptedVars.insert("firing_mode_max");
        m_acceptedVars.insert("acc_angle");
        m_acceptedVars.insert("acc_m_walk");
        m_acceptedVars.insert("acc_m_run");
        m_acceptedVars.insert("acc_m_duck");
        m_acceptedVars.insert("recoil_m");
        m_acceptedVars.insert("recoil_cooldown");
        m_acceptedVars.insert("recoil_control");
        m_acceptedVars.insert("bullet_size_x");
        m_acceptedVars.insert("bullet_size_y");
        m_acceptedVars.insert("bullet_size_z");
        m_acceptedVars.insert("bullet_speed");
        m_acceptedVars.insert("bullet_gravity");
        m_acceptedVars.insert("bullet_drag");
        m_acceptedVars.insert("bullet_fragile");
        m_acceptedVars.insert("damage_hp");
        m_acceptedVars.insert("damage_ap");
        m_acceptedVars.insert("damage_area_size");
        m_acceptedVars.insert("damage_area_effect");
        m_acceptedVars.insert("damage_area_pulse");
    }

    getConsole().OLnOI("Weapon::load(%s) ...", fname);
    std::ifstream f;

    f.open (fname, std::ifstream::in);
    if ( !f.good() )
    {
        getConsole().EOLnOO("ERROR: failed to open file: %s!", fname);
        throw std::runtime_error("failed to open file: " + std::string(fname));
    }

    std::set<std::string> m_missingVars = m_acceptedVars;
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
            lineHandleAssignment(sVar, sValue, fname, m_missingVars, bParseError);
        }
    };
    f.close();

    if ( bParseError )
    {
        getConsole().EOLnOO("ERROR: failed to parse file: %s!", fname);
        throw std::runtime_error("failed to parse file: " + std::string(fname));
    }

    if ( !m_missingVars.empty() )
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
        throw std::runtime_error("failed to parse file: " + std::string(fname) + "variable(s) missing: " + sMissingVars);
    }

    getConsole().SOLnOO("Weapon loaded!");
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


std::set<std::string> Weapon::m_acceptedVars;

bool Weapon::lineShouldBeIgnored(const std::string& sLine)
{
    return sLine.empty() || (sLine[0] == '#');
}

bool Weapon::lineIsValueAssignment(const std::string& sLine, std::string& sVar, std::string& sValue, bool& bParseError)
{
    const std::string::size_type nAssignmentPos = sLine.find('=');
    if ( nAssignmentPos == std::string::npos )
    {
        return false;
    }

    if ( (nAssignmentPos == (sLine.length() - 1)) || (nAssignmentPos == 0 ) )
    {
        CConsole::getConsoleInstance("Weapon").EOLn("ERROR: erroneous assignment: %s!", sLine.c_str());
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
                CConsole::getConsoleInstance("Weapon").EOLn("ERROR: erroneous assignment, failed to parse variable in line: %s!", sLine.c_str());
                bParseError = true;
                return false;
            }
        }
        else
        {
            // should never reach this point based on above 2 conditions
            CConsole::getConsoleInstance("Weapon").EOLn("ERROR: erroneous assignment: %s!", sLine.c_str());
            bParseError = true;
            return false;
        }
    }
    else
    {
        sVar = sLine.substr(0, nAssignmentPos);
    }

    std::transform(sVar.begin(), sVar.end(), sVar.begin(), ::tolower);

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
        CConsole::getConsoleInstance("Weapon").EOLn("ERROR: erroneous assignment, failed to parse value in line: %s!", sLine.c_str());
        bParseError = true;
        return false;
    }

    return true;
}

Weapon::Weapon()
{
}

void Weapon::lineHandleAssignment(std::string& sVar, std::string& sValue, const char* fname, std::set<std::string>& m_missingVars, bool& bParseError)
{
    getConsole().OLn("Var \"%s\" = \"%s\"", sVar.c_str(), sValue.c_str());
    
    if ( m_acceptedVars.end() == m_acceptedVars.find(sVar) )
    {
        getConsole().EOLnOO("ERROR: setting unknown variable %s in file %s!", sVar.c_str(), fname);
        bParseError = true;
        return;
    }

    const auto it_missing = m_missingVars.find(sVar);
    if ( it_missing == m_missingVars.end() )
    {
        getConsole().EOLnOO("ERROR: variable %s in file %s has been already set previously!", sVar.c_str(), fname);
        bParseError = true;
        return;
    }
    m_missingVars.erase(it_missing);

    getVars()[sVar] = sValue.c_str();
}


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
    try
    {
        Weapon wpn(fname);
        m_weapons.push_back(wpn);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
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


