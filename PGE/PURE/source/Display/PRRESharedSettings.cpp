#pragma once

/*
    ########################################################################################
    PureSharedSettings.cpp
    This file is part of Pure.
    Pure shared settings class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ########################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../../include/internal/Display/PureSharedSettings.h"
#include "../../include/internal/Purepragmas.h"


#define Pure_DEF_MONITORPWRSAVE_ON true     /**< Default state of monitor power save enabled. */
#define Pure_DEF_SCRNSAVE_ON       false    /**< Default state of screensaver enabled. */
#define Pure_DEF_STANDBY_ON        false    /**< Default state of computer standby enabled. */


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PureSharedSettings& PureSharedSettings::createAndGet()
{
    static PureSharedSettings sharedSettingsInstance;  // define it here instead of .cpp file because we dont have .cpp file
    return sharedSettingsInstance;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureSharedSettings::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureSharedSettings::getLoggerModuleName()
{
    return "PureSharedSettings";
} // getLoggerModuleName()


/**
    Gets the specified shared setting.

    @param s Any enum element defined in TPure_SHARED_SETTINGS. 

    @return Current setting of the given shared setting.
*/
TPurebool PureSharedSettings::get(TPure_SHARED_SETTINGS s) const
{
    switch (s)
    {
    case Pure_SSET_MONITORPOWERSAVE_ALLOWED: return bMonitorPowerSave;
    case Pure_SSET_SCREENSAVER_ALLOWED     : return bScreenSaver;
    case Pure_SSET_STANDBY_ALLOWED         : return bStandby;
    case Pure_SSET_VSYNC_SUPPORTED           : return bVSyncSupported;
    default                        : getConsole().EOLn("PureSharedSettings::get(%d) ERROR: default switch case!", s);
                                        return false;
    }
} // get()


/**
    Sets the specified shared setting.

    @param s     Any enum element defined in TPure_SHARED_SETTINGS. 
    @param value The given shared setting will be set to this value.
*/
void PureSharedSettings::Set(TPure_SHARED_SETTINGS s, TPurebool value)
{
    switch (s)
    {
    case Pure_SSET_MONITORPOWERSAVE_ALLOWED: bMonitorPowerSave = value; break;
    case Pure_SSET_SCREENSAVER_ALLOWED     : bScreenSaver = value; break;
    case Pure_SSET_STANDBY_ALLOWED         : bStandby = value; break;
    case Pure_SSET_VSYNC_SUPPORTED           : bVSyncSupported = value; break;
    default                        : getConsole().EOLn("PureSharedSettings::Set(%d, %b) ERROR: default switch case!", s, value);
                                        return;
    }
} // Set()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PureSharedSettings::PureSharedSettings()
{
    bMonitorPowerSave = Pure_DEF_MONITORPWRSAVE_ON;
    bScreenSaver      = Pure_DEF_SCRNSAVE_ON;
    bStandby          = Pure_DEF_STANDBY_ON;
    bVSyncSupported   = false;
} // PureSharedSettings()

    
PureSharedSettings::PureSharedSettings(const PureSharedSettings&)
{

}    

PureSharedSettings& PureSharedSettings::operator=(const PureSharedSettings&)
{
    return *this;
}
    
    
PureSharedSettings::~PureSharedSettings()
{

} // ~PureSharedSettings()

