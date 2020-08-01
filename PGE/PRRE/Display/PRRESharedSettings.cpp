#pragma once

/*
    ########################################################################################
    PRRESharedSettings.cpp
    This file is part of PRRE.
    PRRE shared settings class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ########################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../PRREpragmas.h"
#include "PRRESharedSettings.h"


#define PRRE_DEF_MONITORPWRSAVE_ON true     /**< Default state of monitor power save enabled. */
#define PRRE_DEF_SCRNSAVE_ON       false    /**< Default state of screensaver enabled. */
#define PRRE_DEF_STANDBY_ON        false    /**< Default state of computer standby enabled. */


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PRRESharedSettings& PRRESharedSettings::createAndGet()
{
    static PRRESharedSettings sharedSettingsInstance;  // define it here instead of .cpp file because we dont have .cpp file
    return sharedSettingsInstance;
} // createAndGet()


/**
    Gets the specified shared setting.

    @param s Any enum element defined in TPRRE_SHARED_SETTINGS. 

    @return Current setting of the given shared setting.
*/
TPRREbool PRRESharedSettings::get(TPRRE_SHARED_SETTINGS s) const
{
    switch (s)
    {
    case PRRE_SSET_MONITORPOWERSAVE: return bMonitorPowerSave;
    case PRRE_SSET_SCREENSAVER     : return bScreenSaver;
    case PRRE_SSET_STANDBY         : return bStandby;
    case PRRE_SSET_VSYNC           : return bVSyncSupported;
    default                        : getConsole().EOLn("PRRESharedSettings::get(%d) ERROR: default switch case!", s);
                                        return false;
    }
} // get()


/**
    Sets the specified shared setting.

    @param s     Any enum element defined in TPRRE_SHARED_SETTINGS. 
    @param value The given shared setting will be set to this value.
*/
void PRRESharedSettings::Set(TPRRE_SHARED_SETTINGS s, TPRREbool value)
{
    switch (s)
    {
    case PRRE_SSET_MONITORPOWERSAVE: bMonitorPowerSave = value; break;
    case PRRE_SSET_SCREENSAVER     : bScreenSaver = value; break;
    case PRRE_SSET_STANDBY         : bStandby = value; break;
    case PRRE_SSET_VSYNC           : bVSyncSupported = value; break;
    default                        : getConsole().EOLn("PRRESharedSettings::Set(%d, %b) ERROR: default switch case!", s, value);
                                        return;
    }
} // Set()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PRRESharedSettings::PRRESharedSettings()
{
    bMonitorPowerSave = PRRE_DEF_MONITORPWRSAVE_ON;
    bScreenSaver      = PRRE_DEF_SCRNSAVE_ON;
    bStandby          = PRRE_DEF_STANDBY_ON;
    bVSyncSupported   = false;
} // PRRESharedSettings()

    
PRRESharedSettings::PRRESharedSettings(const PRRESharedSettings&)
{

}    

PRRESharedSettings& PRRESharedSettings::operator=(const PRRESharedSettings&)
{
    return *this;
}
    
    
PRRESharedSettings::~PRRESharedSettings()
{

} // ~PRRESharedSettings()

