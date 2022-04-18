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
        AddSubTest("test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag);
        AddSubTest("test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag);
        AddSubTest("test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown);
        AddSubTest("test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control);
        AddSubTest("test_wpn_load_weapon_max_bullet_speed_incompatible_with_non_zero_bullet_drag", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_max_bullet_speed_incompatible_with_non_zero_bullet_drag);
        AddSubTest("test_wpn_load_weapon_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse);
        AddSubTest("test_wpn_load_weapon_good", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_good);
        
        AddSubTest("test_wpn_reload_when_no_more_bullets_does_not_reload", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_when_no_more_bullets_does_not_reload);
        AddSubTest("test_wpn_reload_when_full_does_not_reload", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_when_full_does_not_reload);
        AddSubTest("test_wpn_reload_when_not_reloadable_does_not_reload", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_when_not_reloadable_does_not_reload);
        AddSubTest("test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets",
            (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets);
        AddSubTest("test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets",
            (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets);
        AddSubTest("test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_with_reload_whole_mag_wastes_bullets",
            (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_with_reload_whole_mag_wastes_bullets);
        AddSubTest("test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_with_reload_whole_mag_wastes_bullets",
            (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_with_reload_whole_mag_wastes_bullets);
        AddSubTest("test_wpn_reload_per_bullet_is_one_by_one", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_per_bullet_is_one_by_one);
        AddSubTest("test_wpn_reload_doesnt_reload_when_already_reloading", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_doesnt_reload_when_already_reloading);

        AddSubTest("test_wpn_shoot_when_empty_does_not_shoot", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_when_empty_does_not_shoot);
        AddSubTest("test_wpn_shoot_during_reloading_per_mag_does_not_shoot", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_during_reloading_per_mag_does_not_shoot);
        AddSubTest("test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading);
        AddSubTest("test_wpn_shoot_doesnt_shoot_when_already_shooting", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_doesnt_shoot_when_already_shooting);
        
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

    bool test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_not_reloadable_incompatible_with_reload_per_mag.txt");
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_reload_whole_mag_incompatible_with_no_reload_per_mag.txt");
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_no_recoil_incompatible_with_non_zero_recoil_cooldown.txt");
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_no_recoil_incompatible_with_recoil_control.txt");
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_load_weapon_max_bullet_speed_incompatible_with_non_zero_bullet_drag()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_max_bullet_speed_incompatible_with_non_zero_bullet_drag.txt");
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_load_weapon_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/wpn_test_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse.txt");
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
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state") &
                assertEquals(30u, wpn.getMagBulletCount(), "mag") &
                assertEquals(0u,  wpn.getUnmagBulletCount(), "unmag") &
                assertEquals("Sample Weapon 1", wpn.getVars()["name"].getAsString(), "name") &
                assertEquals(999, wpn.getVars()["cap_max"].getAsInt(), "cap_max") &
                assertEquals(30,  wpn.getVars()["reloadable"].getAsInt(), "reloadable") &
                assertEquals(30,  wpn.getVars()["bullets_default"].getAsInt(), "bullets_default") &
                assertTrue(wpn.getVars()["reload_per_mag"].getAsBool(), "reload_per_mag") &
                assertTrue(wpn.getVars()["reload_whole_mag"].getAsBool(), "reload_whole_mag") &
                assertEquals(1500,    wpn.getVars()["reload_time"].getAsInt(), "reload_time") &
                assertEquals("semi",  wpn.getVars()["firing_mode_def"].getAsString(), "firing_mode_def") &
                assertEquals("burst", wpn.getVars()["firing_mode_max"].getAsString(), "firing_mode_max") &
                assertEquals(5.f,  wpn.getVars()["acc_angle"].getAsFloat(), "acc_angle") &
                assertEquals(1.2f, wpn.getVars()["acc_m_walk"].getAsFloat(), "acc_m_walk") &
                assertEquals(3.0f, wpn.getVars()["acc_m_run"].getAsFloat(), "acc_m_run") &
                assertEquals(0.6f, wpn.getVars()["acc_m_duck"].getAsFloat(), "acc_m_duck") &
                assertEquals(1.1f, wpn.getVars()["recoil_m"].getAsFloat(), "recoil_m") &
                assertEquals(500,  wpn.getVars()["recoil_cooldown"].getAsInt(), "recoil_cooldown") &
                assertEquals("high", wpn.getVars()["recoil_control"].getAsString(), "recoil_control") &
                assertEquals(1.f,  wpn.getVars()["bullet_size_x"].getAsFloat(), "bullet_size_x") &
                assertEquals(2.f,  wpn.getVars()["bullet_size_y"].getAsFloat(), "bullet_size_y") &
                assertEquals(3.f,  wpn.getVars()["bullet_size_z"].getAsFloat(), "bullet_size_z") &
                assertEquals(2.0f, wpn.getVars()["bullet_speed"].getAsFloat(), "bullet_speed") &
                assertEquals(0.f,  wpn.getVars()["bullet_gravity"].getAsFloat(), "bullet_gravity") &
                assertEquals(0.2f, wpn.getVars()["bullet_drag"].getAsFloat(), "bullet_drag") &
                assertTrue(wpn.getVars()["bullet_fragile"].getAsBool(), "bullet_fragile") &
                assertEquals(20,  wpn.getVars()["damage_hp"].getAsInt(), "damage_hp") &
                assertEquals(10,  wpn.getVars()["damage_ap"].getAsInt(), "damage_ap") &
                assertEquals(5.f, wpn.getVars()["damage_area_size"].getAsFloat(), "damage_area_size") &
                assertEquals("linear", wpn.getVars()["damage_area_effect"].getAsString(), "damage_area_effect") &
                assertEquals(20.f, wpn.getVars()["damage_area_pulse"].getAsFloat(), "damage_area_pulse");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_reload_when_no_more_bullets_does_not_reload()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            // by default no unmag bullets are available
            wpn.SetMagBulletCount(0);
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_reload_when_full_does_not_reload()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            // by default magazine is full == 30 bullets
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_reload_when_not_reloadable_does_not_reload()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            // by default magazine is full == 30 bullets
            wpn.getVars()["reloadable"].Set(0); // set weapon to not reloadable
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            
            PFL::timeval timeReloadStarted;
            PFL::gettimeofday(&timeReloadStarted, 0);
            int iWait = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                wpn.Update();
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPRREfloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(30u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(84u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertGequals(fMillisecsReloadTook, wpn.getVars()["reload_time"].getAsFloat(), "time");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.SetUnmagBulletCount(7); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");

            PFL::timeval timeReloadStarted;
            PFL::gettimeofday(&timeReloadStarted, 0);
            int iWait = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                wpn.Update();
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPRREfloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(21u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(0u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertGequals(fMillisecsReloadTook, wpn.getVars()["reload_time"].getAsFloat(), "time");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_with_reload_whole_mag_wastes_bullets()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            // "reload_whole_mag" is true in sample wpn file
            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");

            PFL::timeval timeReloadStarted;
            PFL::gettimeofday(&timeReloadStarted, 0);
            int iWait = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                wpn.Update();
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPRREfloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(30u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(70u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertGequals(fMillisecsReloadTook, wpn.getVars()["reload_time"].getAsFloat(), "time");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_with_reload_whole_mag_wastes_bullets()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            // "reload_whole_mag" is true in sample wpn file
            wpn.SetUnmagBulletCount(7); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");

            PFL::timeval timeReloadStarted;
            PFL::gettimeofday(&timeReloadStarted, 0);
            int iWait = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                wpn.Update();
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPRREfloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(7u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(0u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertGequals(fMillisecsReloadTook, wpn.getVars()["reload_time"].getAsFloat(), "time");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_reload_per_bullet_is_one_by_one()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(25); // full would be 30
            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.getVars()["reload_per_mag"].Set(false);  // per-bullet reload
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            // so the idea is that we frequently checking the mag and unmag
            // bullets count and we remove the observed values from the sets,
            // so after the loop we expect both sets to be empty, proving that
            // bullet counts were changing continuously by 1
            std::set<TPRREuint> expectedMagBulletCounts;
            // TODO: cpp11 initializer list!
            expectedMagBulletCounts.insert(25);
            expectedMagBulletCounts.insert(26);
            expectedMagBulletCounts.insert(27);
            expectedMagBulletCounts.insert(28);
            expectedMagBulletCounts.insert(29);
            expectedMagBulletCounts.insert(30);

            std::set<TPRREuint> expectedUnmagBulletCounts;
            // TODO: cpp11 initializer list!
            expectedUnmagBulletCounts.insert(100);
            expectedUnmagBulletCounts.insert(99);
            expectedUnmagBulletCounts.insert(98);
            expectedUnmagBulletCounts.insert(97);
            expectedUnmagBulletCounts.insert(96);
            expectedUnmagBulletCounts.insert(95);

            int iWait = 0;
            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 75) )
            {
                iWait++;
                wpn.Update();
                expectedMagBulletCounts.erase( wpn.getMagBulletCount() );
                expectedUnmagBulletCounts.erase( wpn.getUnmagBulletCount() );
                Sleep(20);
            }

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(30u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(95u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertTrue(expectedMagBulletCounts.empty(), "exp mag empty");
            b &= assertTrue(expectedUnmagBulletCounts.empty(), "exp unmag empty");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_reload_doesnt_reload_when_already_reloading()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(25); // full would be 30

            b &= assertTrue(wpn.reload(), "reload 1");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertFalse(wpn.reload(), "reload 2");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 2");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_shoot_when_empty_does_not_shoot()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;

            // by default magazine is full == 30 bullets, set it to 0
            wpn.SetMagBulletCount(0);
            b &= assertFalse(wpn.shoot(), "shoot");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_shoot_during_reloading_per_mag_does_not_shoot()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;
            b &= false; /* TODO: copy the per mag reload test case here and try shoot within the loop! */
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading()
    {
        bool b = false;
        try
        {
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt");
            b = true;
            b &= false; /* TODO */
        }
        catch (const std::exception) {}

        return b;
    }

    bool test_wpn_shoot_doesnt_shoot_when_already_shooting()
    {
        return false;
    }

    bool test_wm_initially_empty()
    {
        WeaponManager wm(*engine);
        return assertTrue(wm.getWeapons().empty());
    }

    bool test_wm_clear_weapons()
    {
        WeaponManager wm(*engine);
        bool b = assertTrue(wm.load("gamedata/weapons/sample_good_wpn.txt"), "load");
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
        bool b = assertTrue(wm.load("gamedata/weapons/sample_good_wpn.txt"), "load");
        b &= assertFalse(wm.getWeapons().empty(), "not empty");

        return b;
    }

}; 