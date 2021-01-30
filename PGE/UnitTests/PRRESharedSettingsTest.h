#pragma once

/*
    ###################################################################################
    PRRESharedSettingsTest.h
    Unit test for PRRESharedSettings.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/internal/Display/PRRESharedSettings.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRRESharedSettingsTest :
    public UnitTest
{
public:

    PRRESharedSettingsTest() :
        UnitTest( __FILE__ ),
        ss( PRRESharedSettings::createAndGet() )
    {
        screensvrOrig = false;
        monitorpwrOrig = false;
        standbyOrig = false;
        vsyncOrig = false;
    }

protected:

    virtual void Initialize()
    {
        screensvrOrig = ss.get(PRRE_SSET_SCREENSAVER_ALLOWED);
        monitorpwrOrig = ss.get(PRRE_SSET_MONITORPOWERSAVE_ALLOWED);
        standbyOrig = ss.get(PRRE_SSET_STANDBY_ALLOWED);
        vsyncOrig = ss.get(PRRE_SSET_VSYNC_SUPPORTED);
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRRESharedSettingsTest::testCtor);
        AddSubTest("testSet", (PFNUNITSUBTEST) &PRRESharedSettingsTest::testSet);
    }

    virtual void Finalize()
    {
        ss.Set(PRRE_SSET_MONITORPOWERSAVE_ALLOWED, monitorpwrOrig);
        ss.Set(PRRE_SSET_SCREENSAVER_ALLOWED, screensvrOrig);
        ss.Set(PRRE_SSET_STANDBY_ALLOWED, standbyOrig);
        ss.Set(PRRE_SSET_VSYNC_SUPPORTED, vsyncOrig);
    }

private:
    PRRESharedSettings& ss;
    bool screensvrOrig;
    bool monitorpwrOrig;
    bool standbyOrig;
    bool vsyncOrig;

    // ---------------------------------------------------------------------------

    PRRESharedSettingsTest(const PRRESharedSettingsTest&) :
        ss( PRRESharedSettings::createAndGet() )
    {};         

    PRRESharedSettingsTest& operator=(const PRRESharedSettingsTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertTrue(ss.get(PRRE_SSET_MONITORPOWERSAVE_ALLOWED), "monitorpwrsave") &  /* should be True as defined in Screen */
            assertFalse(ss.get(PRRE_SSET_SCREENSAVER_ALLOWED), "screensvr") &           /* should be False as defined in Screen */
            assertFalse(ss.get(PRRE_SSET_STANDBY_ALLOWED), "standby") &                 /* should be False as defined in Screen */
            assertFalse(ss.get(PRRE_SSET_VSYNC_SUPPORTED), "vsync");                      /* should be False as defined in Screen */
    }

    bool testSet()
    {
        ss.Set(PRRE_SSET_MONITORPOWERSAVE_ALLOWED, false);
        ss.Set(PRRE_SSET_SCREENSAVER_ALLOWED, true);
        ss.Set(PRRE_SSET_STANDBY_ALLOWED, true);
        ss.Set(PRRE_SSET_VSYNC_SUPPORTED, true);

        return assertFalse(ss.get(PRRE_SSET_MONITORPOWERSAVE_ALLOWED)) &
            assertTrue(ss.get(PRRE_SSET_SCREENSAVER_ALLOWED)) &
            assertTrue(ss.get(PRRE_SSET_STANDBY_ALLOWED)) &
            assertTrue(ss.get(PRRE_SSET_VSYNC_SUPPORTED));
    }
    
}; // class PRRESharedSettingsTest
