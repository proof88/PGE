#pragma once

/*
    ###################################################################################
    PureSharedSettingsTest.h
    Unit test for PureSharedSettings.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/internal/Display/PureSharedSettings.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureSharedSettingsTest :
    public UnitTest
{
public:

    PureSharedSettingsTest() :
        UnitTest( __FILE__ ),
        ss( PureSharedSettings::createAndGet() )
    {
        screensvrOrig = false;
        monitorpwrOrig = false;
        standbyOrig = false;
        vsyncOrig = false;
        
    }

    virtual ~PureSharedSettingsTest()
    {
        
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureSharedSettings::getLoggerModuleName(), true);
        screensvrOrig = ss.get(Pure_SSET_SCREENSAVER_ALLOWED);
        monitorpwrOrig = ss.get(Pure_SSET_MONITORPOWERSAVE_ALLOWED);
        standbyOrig = ss.get(Pure_SSET_STANDBY_ALLOWED);
        vsyncOrig = ss.get(Pure_SSET_VSYNC_SUPPORTED);
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureSharedSettingsTest::testCtor);
        AddSubTest("testSet", (PFNUNITSUBTEST) &PureSharedSettingsTest::testSet);
    }

    virtual void Finalize()
    {
        ss.Set(Pure_SSET_MONITORPOWERSAVE_ALLOWED, monitorpwrOrig);
        ss.Set(Pure_SSET_SCREENSAVER_ALLOWED, screensvrOrig);
        ss.Set(Pure_SSET_STANDBY_ALLOWED, standbyOrig);
        ss.Set(Pure_SSET_VSYNC_SUPPORTED, vsyncOrig);
        CConsole::getConsoleInstance().SetLoggingState(PureSharedSettings::getLoggerModuleName(), false);
    }

private:
    PureSharedSettings& ss;
    bool screensvrOrig;
    bool monitorpwrOrig;
    bool standbyOrig;
    bool vsyncOrig;

    // ---------------------------------------------------------------------------

    PureSharedSettingsTest(const PureSharedSettingsTest&) :
        ss( PureSharedSettings::createAndGet() )
    {};         

    PureSharedSettingsTest& operator=(const PureSharedSettingsTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertTrue(ss.get(Pure_SSET_MONITORPOWERSAVE_ALLOWED), "monitorpwrsave") &  /* should be True as defined in Screen */
            assertFalse(ss.get(Pure_SSET_SCREENSAVER_ALLOWED), "screensvr") &           /* should be False as defined in Screen */
            assertFalse(ss.get(Pure_SSET_STANDBY_ALLOWED), "standby") &                 /* should be False as defined in Screen */
            assertFalse(ss.get(Pure_SSET_VSYNC_SUPPORTED), "vsync");                      /* should be False as defined in Screen */
    }

    bool testSet()
    {
        ss.Set(Pure_SSET_MONITORPOWERSAVE_ALLOWED, false);
        ss.Set(Pure_SSET_SCREENSAVER_ALLOWED, true);
        ss.Set(Pure_SSET_STANDBY_ALLOWED, true);
        ss.Set(Pure_SSET_VSYNC_SUPPORTED, true);

        return assertFalse(ss.get(Pure_SSET_MONITORPOWERSAVE_ALLOWED)) &
            assertTrue(ss.get(Pure_SSET_SCREENSAVER_ALLOWED)) &
            assertTrue(ss.get(Pure_SSET_STANDBY_ALLOWED)) &
            assertTrue(ss.get(Pure_SSET_VSYNC_SUPPORTED));
    }
    
}; // class PureSharedSettingsTest
