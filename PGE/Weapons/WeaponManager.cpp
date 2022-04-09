/*
    ###################################################################################
    WeaponManager.cpp
    This file is part of PGE.
    PR00F's Game Engine Weapon Manager
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "WeaponManager.h"

/*
   Weapon
   ###########################################################################
*/


// ############################### PUBLIC ################################


Weapon::Weapon(const char* fname)
{
    getConsole().OLnOI("Weapon::Weapon(%s) ...", fname);

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

    setAcceptedVars(m_acceptedVars);
    SetAllAcceptedVarsDefineRequirement(true);

    if ( !load(fname) )
    {
        getConsole().EOLnOO("failed to load file: %s! ", fname);
        throw std::runtime_error("failed to load file: " + std::string(fname));
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


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


Weapon::Weapon()
{
}


/*
   WeaponManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


WeaponManager::WeaponManager(PR00FsReducedRenderingEngine& gfx) :
    m_gfx(gfx)
{

}

WeaponManager::~WeaponManager()
{

}

CConsole& WeaponManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

const char* WeaponManager::getLoggerModuleName()
{
    return "WeaponManager";
}

bool WeaponManager::load(const char* fname)
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

const std::vector<Weapon>& WeaponManager::getWeapons() const
{
    return m_weapons;
}

void WeaponManager::Clear()
{
    m_weapons.clear();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


