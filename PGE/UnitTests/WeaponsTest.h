#pragma once

/*
    ###################################################################################
    WeaponsTest.h
    Unit test for WeaponManager.
    Made by PR00F88, West Whiskhyll Entertainment
    2022
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Weapons/WeaponManager.h"

class WeaponsTest :
    public UnitTest
{
public:

    WeaponsTest() :
        UnitTest( __FILE__ )
    {
        engine = NULL;
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PRRETexture::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRRETextureManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(WeaponManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(Weapon::getLoggerModuleName(), true);
        
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems

        AddSubTest("test_wpn_load_weapon_bad_assignment", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_bad_assignment);
        AddSubTest("test_wpn_load_weapon_unaccepted_var", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_unaccepted_var);
        AddSubTest("test_wpn_load_weapon_missing_var", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_missing_var);
        AddSubTest("test_wpn_load_weapon_double_defined_var", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_double_defined_var);
        AddSubTest("test_wpn_load_weapon_good", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_good);
        AddSubTest("test_wm_initially_empty", (PFNUNITSUBTEST) &WeaponsTest::test_wm_initially_empty);
        AddSubTest("test_wm_clear_weapons", (PFNUNITSUBTEST) &WeaponsTest::test_wm_clear_weapons);
        AddSubTest("test_wm_load_weapon_bad_assignment", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_bad_assignment);
        AddSubTest("test_wm_load_weapon_unaccepted_var", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_unaccepted_var);
        AddSubTest("test_wm_load_weapon_missing_var", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_missing_var);
        AddSubTest("test_wm_load_weapon_double_defined_var", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_double_defined_var);
        AddSubTest("test_wm_load_weapon_good", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_good);
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
        CConsole::getConsoleInstance().SetLoggingState(WeaponManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(Weapon::getLoggerModuleName(), false);
    }

private:

    PR00FsReducedRenderingEngine* engine;

    // ---------------------------------------------------------------------------

    WeaponsTest(const WeaponsTest&)
    {};         

    WeaponsTest& operator=(const WeaponsTest&)
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

    bool test_wpn_load_weapon_double_defined_var()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_wpn_load_weapon_double_defined_var.txt");
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

    bool test_wm_initially_empty()
    {
        WeaponManager wm(*engine);
        return assertTrue(wm.getWeapons().empty());
    }

    bool test_wm_clear_weapons()
    {
        WeaponManager wm(*engine);
        bool b = assertTrue(wm.load("gamedata/weapons/sample_wpn.txt"), "load");
        wm.Clear();

        return b & assertTrue(wm.getWeapons().empty(), "empty");
    }

    bool test_wm_load_weapon_bad_assignment()
    {
        WeaponManager wm(*engine);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_bad_assignment.txt"), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty");

        return b;
    }

    bool test_wm_load_weapon_unaccepted_var()
    {
        WeaponManager wm(*engine);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_wpn_load_weapon_unaccepted_var.txt"), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty");

        return b;
    }

    bool test_wm_load_weapon_missing_var()
    {
        WeaponManager wm(*engine);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_wpn_load_weapon_missing_var.txt"), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty");

        return b;
    }

    bool test_wm_load_weapon_double_defined_var()
    {
        WeaponManager wm(*engine);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_wpn_load_weapon_double_defined_var.txt"), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty");

        return b;
    }

    bool test_wm_load_weapon_good()
    {
        WeaponManager wm(*engine);
        bool b = assertTrue(wm.load("gamedata/weapons/sample_wpn.txt"), "load");
        b &= assertFalse(wm.getWeapons().empty(), "not empty");

        return b;
    }

}; 