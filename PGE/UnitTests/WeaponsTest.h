#pragma once

/*
    ###################################################################################
    WeaponsTest.h
    Unit test for Weapon and WeaponManager.
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
        //CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(WeaponManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(Weapon::getLoggerModuleName(), true);
        
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems

        AddSubTest("test_wpn_load_weapon_bad_assignment", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_bad_assignment);
        AddSubTest("test_wpn_load_weapon_unaccepted_var", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_unaccepted_var);
        AddSubTest("test_wpn_load_weapon_missing_var", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_missing_var);
        AddSubTest("test_wpn_load_weapon_double_defined_var", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_double_defined_var);
        AddSubTest("test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag);
        AddSubTest("test_wpn_load_weapon_bullets_default_cannot_be_greater_than_non_zero_reloadable", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_bullets_default_cannot_be_greater_than_non_zero_reloadable);
        AddSubTest("test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag);
        AddSubTest("test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown);
        AddSubTest("test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control);
        AddSubTest("wpn_test_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled", (PFNUNITSUBTEST) &WeaponsTest::wpn_test_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled);
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

        AddSubTest("test_wpn_shoot_creates_bullet_with_same_angle_and_pos_as_weapon", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_creates_bullet_with_same_angle_and_pos_as_weapon);
        AddSubTest("test_wpn_update_position_updates_weapon_object_position", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_update_position_updates_weapon_object_position);
        AddSubTest("test_wpn_update_positions_updates_weapon_object_position_and_angle_1", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_update_positions_updates_weapon_object_position_and_angle_1);
        AddSubTest("test_wpn_update_positions_updates_weapon_object_position_and_angle_2", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_update_positions_updates_weapon_object_position_and_angle_2);

        AddSubTest("test_wpn_shoot_when_empty_does_not_shoot", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_when_empty_does_not_shoot);
        AddSubTest("test_wpn_shoot_during_reloading_per_mag_does_not_shoot", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_during_reloading_per_mag_does_not_shoot);
        AddSubTest("test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading);
        AddSubTest("test_wpn_shoot_doesnt_shoot_when_already_shooting", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_doesnt_shoot_when_already_shooting);
        AddSubTest("test_wpn_shoot_continuously_in_loop_respects_cooldown_time", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_continuously_in_loop_respects_cooldown_time);
        AddSubTest("test_wpn_reload_doesnt_reload_during_shooting", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_doesnt_reload_during_shooting);
        AddSubTest("test_wpn_reset_sets_defaults", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_reset_sets_defaults);
        
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
        CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), false);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_bad_assignment.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_unaccepted_var.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_missing_var.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_double_defined_var.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_not_reloadable_incompatible_with_reload_per_mag.txt", bullets, *engine, 0);
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_load_weapon_bullets_default_cannot_be_greater_than_non_zero_reloadable()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_bullets_default_cannot_be_greater_than_non_zero_reloadable.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_reload_whole_mag_incompatible_with_no_reload_per_mag.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_no_recoil_incompatible_with_non_zero_recoil_cooldown.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_no_recoil_incompatible_with_recoil_control.txt", bullets, *engine, 0);
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool wpn_test_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_max_bullet_speed_incompatible_with_non_zero_bullet_drag.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse.txt", bullets, *engine, 0);
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
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 10);
            b = true;
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state") &
                assertEquals(10u, wpn.getOwner(), "owner") &
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
                assertEquals(300,    wpn.getVars()["firing_cooldown"].getAsInt(), "firing_cooldown") &
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
                assertEquals(20.f, wpn.getVars()["damage_area_pulse"].getAsFloat(), "damage_area_pulse") &
                assertTrue(bullets.empty(), "bullets");
        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_when_no_more_bullets_does_not_reload()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            // by default no unmag bullets are available, and we set mag to 0 as well
            wpn.SetMagBulletCount(0);
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_when_full_does_not_reload()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            // by default magazine is full == 30 bullets
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_when_not_reloadable_does_not_reload()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            // by default magazine is full == 30 bullets
            wpn.getVars()["reloadable"].Set(0); // set weapon to not reloadable
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
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
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
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
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_with_reload_whole_mag_wastes_bullets()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
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
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_with_reload_whole_mag_wastes_bullets()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
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
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_per_bullet_is_one_by_one()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(25); // full would be 30
            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.getVars()["reload_per_mag"].Set(false);  // per-bullet reload
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            // so the idea is that we are frequently checking the mag and unmag
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
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_doesnt_reload_when_already_reloading()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(25); // full would be 30

            b &= assertTrue(wpn.reload(), "reload 1");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertFalse(wpn.reload(), "reload 2");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 2");
        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_shoot_when_empty_does_not_shoot()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            // by default magazine is full == 30 bullets, set it to 0
            wpn.SetMagBulletCount(0);
            b &= assertFalse(wpn.shoot(), "shoot");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertTrue(bullets.empty(), "bullets");
        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_shoot_during_reloading_per_mag_does_not_shoot()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30

            const TPRREuint nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPRREuint nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");

            b &= assertFalse(wpn.shoot(), "shoot");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 2");
            b &= assertTrue(bullets.empty(), "bullets");
            b &= assertEquals(nOriginalMagBulletCount, wpn.getMagBulletCount(), "mag bullet count");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count");

        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.getVars()["reload_per_mag"].Set(false);   // reload per bullet
            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30

            const TPRREuint nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPRREuint nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");

            b &= assertTrue(wpn.shoot(), "shoot");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 2");
            b &= assertFalse(bullets.empty(), "bullets");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count");

        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_shoot_doesnt_shoot_when_already_shooting()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            const TPRREuint nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPRREuint nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            // first shot allowed
            b &= assertTrue(wpn.shoot(), "shoot 1");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 1");
            b &= assertEquals(1u, bullets.size(), "size 1");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 1");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 1");

            // second shot now allowed
            b &= assertFalse(wpn.shoot(), "shoot 2");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 2");
            b &= assertEquals(1u, bullets.size(), "size 2");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 2");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 2");
        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_shoot_continuously_in_loop_respects_cooldown_time()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(7);     // full would be 30

            // so the idea is that we are frequently checking the mag
            // bullets count and we remove the observed values from the set,
            // so after the loop we expect the set to be empty, proving that
            // bullet counts were changing decreasing by 1
            std::set<TPRREuint> expectedMagBulletCounts;
            // TODO: cpp11 initializer list!
            expectedMagBulletCounts.insert(6);
            expectedMagBulletCounts.insert(5);
            expectedMagBulletCounts.insert(4);
            expectedMagBulletCounts.insert(3);
            expectedMagBulletCounts.insert(2);
            expectedMagBulletCounts.insert(1);
            expectedMagBulletCounts.insert(0);

            int iWait = 0;
            // firing cooldown is 300 msecs by the file
            do
            {
                iWait++;
                wpn.shoot(); // even if we call shoot(), it must not shoot when conditions dont meet
                wpn.Update();
                expectedMagBulletCounts.erase( wpn.getMagBulletCount() );
                Sleep(20);
            } while ( (wpn.getMagBulletCount() > 0) && (iWait < 150) );
            // let the cooldown after last shot elapse so state can also go back to READY
            Sleep( wpn.getVars()["firing_cooldown"].getAsInt() );
            wpn.Update();

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(7u, bullets.size(), "size");
            b &= assertEquals(0u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(100u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertTrue(expectedMagBulletCounts.empty(), "exp mag empty");
        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reload_doesnt_reload_during_shooting()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            const TPRREuint nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPRREuint nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            b &= assertTrue(wpn.shoot(), "shoot");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 1");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 1");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 1");

            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 2");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 2");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 2");
        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_reset_sets_defaults()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            const TPRREuint nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPRREuint nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            wpn.SetUnmagBulletCount(100); // default would be 0
            wpn.SetMagBulletCount(14); // full would be 30

            wpn.Reset();

            b &= assertEquals(nOriginalMagBulletCount, wpn.getMagBulletCount(), "mag bullet count");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count");

        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_shoot_creates_bullet_with_same_angle_and_pos_as_weapon()
    {
        bool b = false;
        const pge_network::PgeNetworkConnectionHandle connHandle = 52;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, connHandle);
            b = true;

            wpn.getObject3D().getPosVec().Set(1.f, 2.f, 3.f);
            wpn.getObject3D().getAngleVec().Set(30.f, 40.f, 50.f);
            b &= assertTrue(wpn.shoot(), "shoot");
            b &= assertEquals(1u, bullets.size(), "size");
            
            if ( b )
            {
                const Bullet& bullet = *bullets.begin();
                b &= assertEquals(wpn.getObject3D().getPosVec(), bullet.getObject3D().getPosVec(), "pos");
                b &= assertEquals(wpn.getObject3D().getAngleVec(), bullet.getObject3D().getAngleVec(), "angle");
                b &= assertEquals(connHandle, bullet.getOwner(), "bullet owner");
                b &= assertEquals(wpn.getVars()["damage_hp"].getAsInt(), bullet.getDamageHp(), "damageHp");
            }

        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_update_position_updates_weapon_object_position()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.UpdatePosition(PRREVector(10.f, 20.f, 30.f));
            b &= assertEquals(PRREVector(10.f, 20.f, 30.f), wpn.getObject3D().getPosVec(), "pos");

        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_update_positions_updates_weapon_object_position_and_angle_1()
    {
        bool b = false;

        /*
          By default with AngleY 0° and AngleZ 0°, weapon looks to <- direction.
          
           When AngleY is 0, and:
            - AngleZ is 0, it means weapon looks to <- direction.
                                                      ^
            - AngleZ is -45, it means weapon looks to  \  direction.
                                                     
            - AngleZ is 45, it means weapon looks to  /  direction.
                                                     ¡
          
           When AngleY is 180, and:
            - AngleZ is 0, it means weapon looks to -> direction.
                                                       ^
            - AngleZ is -45, it means weapon looks to /  direction.
          
            - AngleZ is 45, it means weapon looks to \  direction.
                                                      ¡
          
           This means that when targetX is less than posX, then angleY should be 0°, otherwise it needs to be 180°, and angleX always needs to be between 90° and -90°.
          
           Y
           ^
           |
           |  (x)          (x) is the xhair, (w) is the weapon, and ß is angleZ of wpn we need to set.
           |   |\           Length of A and B are obviously known. Thus tanß can be calculated by B/A, so ß in radians will be arctan(B/A).
           |  B|  \
           |   |___ß\(w)
           |     A
          -|------------------------->X
           |
        */

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PRREVector(0.f, 0.f, 0.f), PRREVector(1.f, 1.f, 0.f));
            b &= assertEquals(PRREVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 1");
            b &= assertEquals(PRREVector(-30.f /* angleX is untouched */, 180.f, -45.f), wpn.getObject3D().getAngleVec(), "angle right up");

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PRREVector(0.f, 0.f, 0.f), PRREVector(-1.f, 1.f, 0.f));
            b &= assertEquals(PRREVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 2");
            b &= assertEquals(PRREVector(-30.f /* angleX is untouched */, 0.f, -45.f), wpn.getObject3D().getAngleVec(), "angle left up");

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PRREVector(0.f, 0.f, 0.f), PRREVector(-1.f, -1.f, 0.f));
            b &= assertEquals(PRREVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 3");
            b &= assertEquals(PRREVector(-30.f /* angleX is untouched */, 0.f, 45.f), wpn.getObject3D().getAngleVec(), "angle left bottom");

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PRREVector(0.f, 0.f, 0.f), PRREVector(-1.f, -1.f, 0.f));
            b &= assertEquals(PRREVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 4");
            b &= assertEquals(PRREVector(-30.f /* angleX is untouched */, 0.f, 45.f), wpn.getObject3D().getAngleVec(), "angle right bottom");

        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wpn_update_positions_updates_weapon_object_position_and_angle_2()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn.txt", bullets, *engine, 0);
            b = true;

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PRREVector(0.f, 0.f, 0.f), 180.f, 60.f);
            b &= assertEquals(PRREVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos");
            b &= assertEquals(PRREVector(-30.f /* angleX is untouched */, 180.f, 60.f), wpn.getObject3D().getAngleVec(), "angle");

        }
        catch (const std::exception&) {}

        return b;
    }

    bool test_wm_initially_empty()
    {
        WeaponManager wm(*engine);
        return assertTrue(wm.getWeapons().empty(), "weapons") & assertTrue(wm.getBullets().empty(), "bullets");
    }

    bool test_wm_clear_weapons()
    {
        WeaponManager wm(*engine);
        bool b = assertTrue(wm.load("gamedata/weapons/sample_good_wpn.txt", 0), "load");
        wm.Clear();

        return b & assertTrue(wm.getWeapons().empty(), "empty");
    }

    bool test_wm_load_weapon_bad_assignment()
    {
        WeaponManager wm(*engine);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_bad_assignment.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty");

        return b;
    }

    bool test_wm_load_weapon_unaccepted_var()
    {
        WeaponManager wm(*engine);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_unaccepted_var.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty");

        return b;
    }

    bool test_wm_load_weapon_missing_var()
    {
        WeaponManager wm(*engine);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_missing_var.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty");

        return b;
    }

    bool test_wm_load_weapon_double_defined_var()
    {
        WeaponManager wm(*engine);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_double_defined_var.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty");

        return b;
    }

    bool test_wm_load_weapon_good()
    {
        WeaponManager wm(*engine);
        bool b = assertTrue(wm.load("gamedata/weapons/sample_good_wpn.txt", 0), "load");
        b &= assertFalse(wm.getWeapons().empty(), "not empty");

        return b;
    }

}; 