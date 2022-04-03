#pragma once

/*
    ###################################################################################
    OWSTest.h
    Unit test for OWS.
    Made by PR00F88, West Whiskhyll Entertainment
    2022
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../OWS/OWS.h"

class OWSTest :
    public UnitTest
{
public:

    OWSTest() :
        UnitTest( __FILE__ )
    {
        engine = NULL;
    } // PGEcfgVariableTest()

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PRRETexture::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRRETextureManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(OWS::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(Weapon::getLoggerModuleName(), true);
        
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems

        AddSubTest("test_wpn_load_weapon_bad_assignment", (PFNUNITSUBTEST) &OWSTest::test_wpn_load_weapon_bad_assignment);
        AddSubTest("test_wpn_load_weapon_unaccepted_var", (PFNUNITSUBTEST) &OWSTest::test_wpn_load_weapon_unaccepted_var);
        AddSubTest("test_wpn_load_weapon_missing_var", (PFNUNITSUBTEST) &OWSTest::test_wpn_load_weapon_missing_var);
        AddSubTest("test_wpn_load_weapon_good", (PFNUNITSUBTEST) &OWSTest::test_wpn_load_weapon_good);
        AddSubTest("test_ows_initially_empty", (PFNUNITSUBTEST) &OWSTest::test_ows_initially_empty);
        AddSubTest("test_ows_clear_weapons", (PFNUNITSUBTEST) &OWSTest::test_ows_clear_weapons);
        AddSubTest("test_ows_load_weapon_bad_assignment", (PFNUNITSUBTEST) &OWSTest::test_ows_load_weapon_bad_assignment);
        AddSubTest("test_ows_load_weapon_unaccepted_var", (PFNUNITSUBTEST) &OWSTest::test_ows_load_weapon_unaccepted_var);
        AddSubTest("test_ows_load_weapon_missing_var", (PFNUNITSUBTEST) &OWSTest::test_ows_load_weapon_missing_var);
        AddSubTest("test_ows_load_weapon_good", (PFNUNITSUBTEST) &OWSTest::test_ows_load_weapon_good);
    }

    virtual bool setUp()
    {
        return assertTrue(engine && engine->isInitialized());
    }

    virtual void TearDown()
    {
    }

    virtual void Finalize()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }

        CConsole::getConsoleInstance().SetLoggingState(PRRETexture::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRRETextureManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(OWS::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(Weapon::getLoggerModuleName(), false);
    }

private:

    PR00FsReducedRenderingEngine* engine;

    // ---------------------------------------------------------------------------

    OWSTest(const OWSTest&)
    {};         

    OWSTest& operator=(const OWSTest&)
    {
        return *this;
    };

    bool test_wpn_load_weapon_bad_assignment()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_bad_assignment.txt");
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_load_weapon_unaccepted_var()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_wpn_load_weapon_unaccepted_var.txt");
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_load_weapon_missing_var()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_wpn_load_weapon_missing_var.txt");
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_load_weapon_good()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_wpn.txt");
            b = true;
        }
        catch (const std::exception)
        {
        }

        return b;
    }

    bool test_ows_initially_empty()
    {
        OWS ows(*engine);
        return assertTrue(ows.getWeapons().empty());
    }

    bool test_ows_clear_weapons()
    {
        OWS ows(*engine);
        bool b = assertTrue(ows.load("gamedata/weapons/sample_wpn.txt"), "load");
        ows.Clear();

        return b & assertTrue(ows.getWeapons().empty(), "empty");
    }

    bool test_ows_load_weapon_bad_assignment()
    {
        OWS ows(*engine);
        bool b = assertFalse(ows.load("gamedata/weapons/wpn_test_bad_assignment.txt"), "load");
        b &= assertTrue(ows.getWeapons().empty(), "empty");

        return b;
    }

    bool test_ows_load_weapon_unaccepted_var()
    {
        OWS ows(*engine);
        bool b = assertFalse(ows.load("gamedata/weapons/wpn_test_wpn_load_weapon_unaccepted_var.txt"), "load");
        b &= assertTrue(ows.getWeapons().empty(), "empty");

        return b;
    }

    bool test_ows_load_weapon_missing_var()
    {
        OWS ows(*engine);
        bool b = assertFalse(ows.load("gamedata/weapons/wpn_test_wpn_load_weapon_missing_var.txt"), "load");
        b &= assertTrue(ows.getWeapons().empty(), "empty");

        return b;
    }

    bool test_ows_load_weapon_good()
    {
        OWS ows(*engine);
        bool b = assertTrue(ows.load("gamedata/weapons/sample_wpn.txt"), "load");
        b &= assertFalse(ows.getWeapons().empty(), "not empty");

        return b;
    }

}; 