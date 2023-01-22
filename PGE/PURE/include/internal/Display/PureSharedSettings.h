#pragma once

/*
    ########################################################################################
    PureSharedSettings.h
    This file is part of PURE.
    Internal header.
    Pure shared settings class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ########################################################################################
*/

#include "../../../../../../CConsole/CConsole/src/CConsole.h"
#include "../../external/PureAllHeaders.h"

/**
    Shared settings enumerator.
*/
enum TPure_SHARED_SETTINGS
{
    Pure_SSET_MONITORPOWERSAVE_ALLOWED,
    Pure_SSET_SCREENSAVER_ALLOWED,
    Pure_SSET_STANDBY_ALLOWED,
    Pure_SSET_VSYNC_SUPPORTED
}; // TPure_SHARED_SETTINGS


/**
    Contains Pure shared settings.
    These settings are used by a few classes, but we store these in one place (here), to avoid
    redundancy and inconsistency. Other classes reach these settings through pointers. One class
    may be responsible for properly initializing a setting, while other class may be responsible
    for providing get/set access to the user.
    For a user, this class is not known. Users can manipulate shared settings through public
    classes.
    This class is singleton, of course.
    IMPORTANT: the main Pure object should initialize this class before any other class attempts
    to use it.
*/
class PureSharedSettings
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureSharedSettings is included")
#endif

public:

    /**
        Creates and gets the singleton instance.
    */
    static PureSharedSettings& createAndGet();

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    /**
        Gets the specified shared setting.
    */
    TPurebool get(TPure_SHARED_SETTINGS s) const;


    /**
        Sets the specified shared setting.
    */
    void Set(TPure_SHARED_SETTINGS s, TPurebool value);

protected:

    // ---------------------------------------------------------------------------

private:

    // ---------------------------------------------------------------------------

    TPurebool bMonitorPowerSave;      /**< Is monitor powersaving enabled? Used by Window, accessed through Screen from outside. */
    TPurebool bScreenSaver;           /**< Is screensaver enabled? Used by Window, accessed through Screen from outside. */
    TPurebool bStandby;               /**< Is computer standby allowed? Used by Window, accessed through Screen from outside. */
    TPurebool bVSyncSupported;        /**< Is VSync supported? Set by HWInfo, accessed through Screen from outside. */

    // ---------------------------------------------------------------------------

    PureSharedSettings();

    PureSharedSettings(const PureSharedSettings&);
    PureSharedSettings& operator=(const PureSharedSettings&);
    
    virtual ~PureSharedSettings();

}; // class PureSharedSettings
