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
#include "../PGE/PRRE/PRRESharedSettings.h"

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
        screensvrOrig = ss.get(PRRE_SSET_SCREENSAVER);
        monitorpwrOrig = ss.get(PRRE_SSET_MONITORPOWERSAVE);
        standbyOrig = ss.get(PRRE_SSET_STANDBY);
        vsyncOrig = ss.get(PRRE_SSET_VSYNC);
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRRESharedSettingsTest::testCtor);
        AddSubTest("testSet", (PFNUNITSUBTEST) &PRRESharedSettingsTest::testSet);
    }

    virtual void Finalize()
    {
        ss.Set(PRRE_SSET_MONITORPOWERSAVE, monitorpwrOrig);
        ss.Set(PRRE_SSET_SCREENSAVER, screensvrOrig);
        ss.Set(PRRE_SSET_STANDBY, standbyOrig);
        ss.Set(PRRE_SSET_VSYNC, vsyncOrig);
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
        return assertTrue(ss.get(PRRE_SSET_MONITORPOWERSAVE), "monitorpwrsave") &  /* should be True as defined in Screen */
            assertFalse(ss.get(PRRE_SSET_SCREENSAVER), "screensvr") &           /* should be False as defined in Screen */
            assertFalse(ss.get(PRRE_SSET_STANDBY), "standby") &                 /* should be False as defined in Screen */
            assertFalse(ss.get(PRRE_SSET_VSYNC), "vsync");                      /* should be False as defined in Screen */
    }

    bool testSet()
    {
        ss.Set(PRRE_SSET_MONITORPOWERSAVE, false);
        ss.Set(PRRE_SSET_SCREENSAVER, true);
        ss.Set(PRRE_SSET_STANDBY, true);
        ss.Set(PRRE_SSET_VSYNC, true);

        return assertFalse(ss.get(PRRE_SSET_MONITORPOWERSAVE)) &
            assertTrue(ss.get(PRRE_SSET_SCREENSAVER)) &
            assertTrue(ss.get(PRRE_SSET_STANDBY)) &
            assertTrue(ss.get(PRRE_SSET_VSYNC));
    }
    
}; // class PRRESharedSettingsTest
