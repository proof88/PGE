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

#include <cmath>

class WeaponsTest :
    public UnitTest
{
public:

    WeaponsTest() :
        UnitTest( __FILE__ ),
        cfgProfiles("")
    {
        engine = NULL;
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(WeaponManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(Weapon::getLoggerModuleName(), true);

        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        
        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems

        AddSubTest("test_wpn_load_weapon_bad_assignment", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_bad_assignment);
        AddSubTest("test_wpn_load_weapon_unaccepted_var", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_unaccepted_var);
        AddSubTest("test_wpn_load_weapon_missing_var", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_missing_var);
        AddSubTest("test_wpn_load_weapon_double_defined_var", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_double_defined_var);
        AddSubTest("test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag);
        AddSubTest("test_wpn_load_weapon_reloadable_cannot_be_greater_than_cap_max", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_load_weapon_reloadable_cannot_be_greater_than_cap_max);
        AddSubTest("test_wpn_load_weapon_bullets_default_cannot_be_greater_than_non_zero_reloadable", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_bullets_default_cannot_be_greater_than_non_zero_reloadable);
        AddSubTest("test_wpn_load_weapon_bullets_default_cannot_be_greater_than_cap_max", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_load_weapon_bullets_default_cannot_be_greater_than_cap_max);
        AddSubTest("test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag);
        AddSubTest("test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown);
        AddSubTest("test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control);
        AddSubTest("test_wpn_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled);
        AddSubTest("test_wpn_firing_cooldown_must_be_positive", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_firing_cooldown_must_be_positive);
        AddSubTest("test_wpn_load_weapon_max_bullet_speed_incompatible_with_non_zero_bullet_drag", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_max_bullet_speed_incompatible_with_non_zero_bullet_drag);
        AddSubTest("test_wpn_load_weapon_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse);
        AddSubTest("test_wpn_firing_mode_max_cannot_be_less_than_default", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_firing_mode_max_cannot_be_less_than_default);
        AddSubTest("test_wpn_firing_modes_default_and_max_cannot_be_burst_and_proj", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_firing_modes_default_and_max_cannot_be_burst_and_proj);
        
        AddSubTest("test_wpn_load_weapon_good", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_load_weapon_good);

        AddSubTest("test_wpn_set_available", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_set_available);
        AddSubTest("test_wpn_set_owner", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_set_owner);
        AddSubTest("test_wpn_set_mag_bullet_count", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_set_mag_bullet_count);
        AddSubTest("test_wpn_set_unmag_bullet_count", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_set_unmag_bullet_count);
        AddSubTest("test_wpn_inc_bullet_count", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_inc_bullet_count);
        
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
        AddSubTest("test_wpn_release_trigger_after_shoot", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_release_trigger_after_shoot);
        AddSubTest("test_wpn_update_position_updates_weapon_object_position", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_update_position_updates_weapon_object_position);
        AddSubTest("test_wpn_update_positions_updates_weapon_object_position_and_angle_1", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_update_positions_updates_weapon_object_position_and_angle_1);
        AddSubTest("test_wpn_update_positions_updates_weapon_object_position_and_angle_2", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_update_positions_updates_weapon_object_position_and_angle_2);

        AddSubTest("test_wpn_shoot_when_empty_does_not_shoot", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_when_empty_does_not_shoot);
        AddSubTest("test_wpn_shoot_during_reloading_per_mag_does_not_shoot", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_during_reloading_per_mag_does_not_shoot);
        AddSubTest("test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading);
        AddSubTest("test_wpn_shoot_doesnt_shoot_when_already_shooting", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_shoot_doesnt_shoot_when_already_shooting);
        AddSubTest("test_wpn_automatic_shoot_continuously_in_loop_respects_cooldown_time", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_automatic_shoot_continuously_in_loop_respects_cooldown_time);
        AddSubTest("test_wpn_semi_shoot_has_to_release_and_pull_trigger_continuously_in_loop", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_semi_shoot_has_to_release_and_pull_trigger_continuously_in_loop);
        AddSubTest("test_wpn_reload_doesnt_reload_during_shooting", (PFNUNITSUBTEST) &WeaponsTest::test_wpn_reload_doesnt_reload_during_shooting);
        AddSubTest("test_wpn_reset_sets_defaults", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_reset_sets_defaults);

        AddSubTest("test_wpn_get_damage_per_fire_rating", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_get_damage_per_fire_rating);
        AddSubTest("test_wpn_get_damage_per_second_rating", (PFNUNITSUBTEST)&WeaponsTest::test_wpn_get_damage_per_second_rating);
        
        AddSubTest("test_wm_initially_empty", (PFNUNITSUBTEST) &WeaponsTest::test_wm_initially_empty);
        AddSubTest("test_wm_clear_weapons", (PFNUNITSUBTEST) &WeaponsTest::test_wm_clear_weapons);
        AddSubTest("test_wm_set_default_available_weapon", (PFNUNITSUBTEST)&WeaponsTest::test_wm_set_default_available_weapon);
        AddSubTest("test_wm_load_weapon_bad_assignment", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_bad_assignment);
        AddSubTest("test_wm_load_weapon_unaccepted_var", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_unaccepted_var);
        AddSubTest("test_wm_load_weapon_missing_var", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_missing_var);
        AddSubTest("test_wm_load_weapon_double_defined_var", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_double_defined_var);
        AddSubTest("test_wm_load_weapon_good", (PFNUNITSUBTEST) &WeaponsTest::test_wm_load_weapon_good);
        AddSubTest("test_wm_get_set_current_weapon", (PFNUNITSUBTEST)&WeaponsTest::test_wm_get_set_current_weapon);
        AddSubTest("test_wm_get_prev_avail_weapon", (PFNUNITSUBTEST)&WeaponsTest::test_wm_get_prev_avail_weapon);
        AddSubTest("test_wm_get_prev_avail_weapon_not_in_keytoweaponmap", (PFNUNITSUBTEST)&WeaponsTest::test_wm_get_prev_avail_weapon_not_in_keytoweaponmap);
        AddSubTest("test_wm_get_next_avail_weapon", (PFNUNITSUBTEST)&WeaponsTest::test_wm_get_next_avail_weapon);
        AddSubTest("test_wm_get_next_avail_weapon_not_in_keytoweaponmap", (PFNUNITSUBTEST)&WeaponsTest::test_wm_get_next_avail_weapon_not_in_keytoweaponmap);
        AddSubTest("test_wm_get_next_best_avail_weapon", (PFNUNITSUBTEST)&WeaponsTest::test_wm_get_next_best_avail_weapon);
        AddSubTest("test_wm_get_next_best_avail_weapon_not_in_keytoweaponmap", (PFNUNITSUBTEST)&WeaponsTest::test_wm_get_next_best_avail_weapon_not_in_keytoweaponmap);
    }

    virtual bool setUp() override
    {
        return assertTrue(engine && engine->isInitialized());
    }

    virtual void TearDown() override
    {
    }

    virtual void Finalize()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }

        CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(WeaponManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(Weapon::getLoggerModuleName(), false);
    }

private:

    PR00FsUltimateRenderingEngine* engine;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    WeaponsTest(const WeaponsTest&) : cfgProfiles("")
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

    bool test_wpn_load_weapon_reloadable_cannot_be_greater_than_cap_max()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_reloadable_cannot_be_greater_than_cap_max.txt", bullets, *engine, 0);
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

    bool test_wpn_load_weapon_bullets_default_cannot_be_greater_than_cap_max()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_bullets_default_cannot_be_greater_than_cap_max.txt", bullets, *engine, 0);
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

    bool test_wpn_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled()
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

    bool test_wpn_firing_cooldown_must_be_positive()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_firing_cooldown_must_be_positive.txt", bullets, *engine, 0);
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

    bool test_wpn_firing_mode_max_cannot_be_less_than_default()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_firing_mode_max_cannot_be_less_than_default.txt", bullets, *engine, 0);
        }
        catch (const std::exception)
        {
            b = true;
        }

        return b;
    }

    bool test_wpn_firing_modes_default_and_max_cannot_be_burst_and_proj()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/wpn_test_firing_modes_default_and_max_cannot_be_burst_and_proj.txt", bullets, *engine, 0);
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 10);
            b = true;
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state") &
                assertFalse(wpn.isAvailable(), "available") &
                assertEquals(10u, wpn.getOwner(), "owner") &
                assertEquals(Weapon::FiringMode::WPN_FM_AUTO, wpn.getCurrentFiringMode(), "firing mode") &
                assertEquals(30u, wpn.getMagBulletCount(), "mag") &
                assertEquals(0u, wpn.getUnmagBulletCount(), "unmag") &
                assertTrue(wpn.isTriggerReleased(), "trigger") &
                assertEquals("Sample Weapon 1", wpn.getVars()["name"].getAsString(), "name") &
                assertEquals(999, wpn.getVars()["cap_max"].getAsInt(), "cap_max") &
                assertEquals(30, wpn.getVars()["reloadable"].getAsInt(), "reloadable") &
                assertEquals(30, wpn.getVars()["bullets_default"].getAsInt(), "bullets_default") &
                assertTrue(wpn.getVars()["reload_per_mag"].getAsBool(), "reload_per_mag") &
                assertTrue(wpn.getVars()["reload_whole_mag"].getAsBool(), "reload_whole_mag") &
                assertEquals(1500, wpn.getVars()["reload_time"].getAsInt(), "reload_time") &
                assertEquals("auto", wpn.getVars()["firing_mode_def"].getAsString(), "firing_mode_def") &
                assertEquals("auto", wpn.getVars()["firing_mode_max"].getAsString(), "firing_mode_max") &
                assertEquals(150, wpn.getVars()["firing_cooldown"].getAsInt(), "firing_cooldown") &
                assertEquals(5.f, wpn.getVars()["acc_angle"].getAsFloat(), "acc_angle") &
                assertEquals(1.2f, wpn.getVars()["acc_m_walk"].getAsFloat(), "acc_m_walk") &
                assertEquals(3.0f, wpn.getVars()["acc_m_run"].getAsFloat(), "acc_m_run") &
                assertEquals(0.6f, wpn.getVars()["acc_m_duck"].getAsFloat(), "acc_m_duck") &
                assertEquals(1.1f, wpn.getVars()["recoil_m"].getAsFloat(), "recoil_m") &
                assertEquals(500, wpn.getVars()["recoil_cooldown"].getAsInt(), "recoil_cooldown") &
                assertEquals("high", wpn.getVars()["recoil_control"].getAsString(), "recoil_control") &
                assertEquals(1.f, wpn.getVars()["bullet_size_x"].getAsFloat(), "bullet_size_x") &
                assertEquals(2.f, wpn.getVars()["bullet_size_y"].getAsFloat(), "bullet_size_y") &
                assertEquals(3.f, wpn.getVars()["bullet_size_z"].getAsFloat(), "bullet_size_z") &
                assertEquals(2.0f, wpn.getVars()["bullet_speed"].getAsFloat(), "bullet_speed") &
                assertEquals(0.f, wpn.getVars()["bullet_gravity"].getAsFloat(), "bullet_gravity") &
                assertEquals(0.2f, wpn.getVars()["bullet_drag"].getAsFloat(), "bullet_drag") &
                assertTrue(wpn.getVars()["bullet_fragile"].getAsBool(), "bullet_fragile") &
                assertEquals(20, wpn.getVars()["damage_hp"].getAsInt(), "damage_hp") &
                assertEquals(10, wpn.getVars()["damage_ap"].getAsInt(), "damage_ap") &
                assertEquals(5.f, wpn.getVars()["damage_area_size"].getAsFloat(), "damage_area_size") &
                assertEquals("linear", wpn.getVars()["damage_area_effect"].getAsString(), "damage_area_effect") &
                assertEquals(20.f, wpn.getVars()["damage_area_pulse"].getAsFloat(), "damage_area_pulse") &
                assertTrue(bullets.empty(), "bullets") &
                assertNotNull(&(wpn.getObject3D()), "object3d") &
                assertFalse(wpn.getObject3D().isRenderingAllowed(), "rendering allowed") &
                assertNotNull(wpn.getObject3D().getMaterial().getTexture(), "texture") &
                assertEquals("gamedata\\textures\\weapons\\sample_good_wpn_automatic.bmp", wpn.getObject3D().getMaterial().getTexture()->getFilename(), "texture filename");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_set_available()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 10);

            wpn.SetAvailable(true);
            b = assertTrue(wpn.isAvailable(), "true");

            wpn.SetAvailable(false);
            b &= assertFalse(wpn.isAvailable(), "false");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_set_owner()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 10);

            wpn.SetOwner(5678);
            b = assertEquals(5678u, wpn.getOwner(), "owner");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_set_mag_bullet_count()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 10);

            // sample good wpn has reloadable as 30, so its mag bullet count is that value, unmag is 0 by default,
            // and cap_max is 999
            wpn.SetMagBulletCount(25);
            b = assertEquals(25u, wpn.getMagBulletCount(), "1");

            wpn.SetMagBulletCount(31);
            b &= assertEquals(25u, wpn.getMagBulletCount(), "2");

            // now change weapon to non-reloadable
            wpn.getVars()["reloadable"].Set(0);

            wpn.SetMagBulletCount(31);
            b &= assertEquals(31u, wpn.getMagBulletCount(), "3");

            // cap_max should limit it
            wpn.SetMagBulletCount(1000);
            b &= assertEquals(31u, wpn.getMagBulletCount(), "4");

            wpn.SetMagBulletCount(999);
            b &= assertEquals(999u, wpn.getMagBulletCount(), "4");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_set_unmag_bullet_count()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 10);

            // sample good wpn has reloadable as 30, so its mag bullet count is that value, unmag is 0 by default,
            // and cap_max is 999
            wpn.SetUnmagBulletCount(25);
            b = assertEquals(25u, wpn.getUnmagBulletCount(), "1");

            wpn.SetUnmagBulletCount(31);
            b &= assertEquals(31u, wpn.getUnmagBulletCount(), "2");

            // cap_max should limit it
            wpn.SetUnmagBulletCount(1000);
            b &= assertEquals(31u, wpn.getUnmagBulletCount(), "3");

            wpn.SetUnmagBulletCount(999);
            b &= assertEquals(999u, wpn.getUnmagBulletCount(), "4");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_inc_bullet_count()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 10);

            // sample good wpn has reloadable as 30, so its mag bullet count is that value, unmag is 0 by default,
            // and cap_max is 999
            
            // for reloadable weapon, "inc bullet count" increments the unmag bullet count until cap_max
            b = assertTrue(wpn.canIncBulletCount(), "can 1");
            wpn.IncBulletCount(5);
            b &= assertEquals(5u, wpn.getUnmagBulletCount(), "1");

            b &= assertTrue(wpn.canIncBulletCount(), "can 2");
            wpn.IncBulletCount(0);
            b &= assertEquals(5u, wpn.getUnmagBulletCount(), "2");
            
            b &= assertTrue(wpn.canIncBulletCount(), "can 3");
            wpn.IncBulletCount(5);
            b &= assertEquals(10u, wpn.getUnmagBulletCount(), "3");

            b &= assertTrue(wpn.canIncBulletCount(), "can 4");
            wpn.IncBulletCount(1000);
            b &= assertEquals(999u, wpn.getUnmagBulletCount(), "4");

            b &= assertFalse(wpn.canIncBulletCount(), "can xxx");

            // now change weapon to non-reloadable
            wpn.getVars()["reloadable"].Set(0);

            // for non-reloadable weapon, "inc bullet count" increments the mag bullet count until cap_max
            b &= assertTrue(wpn.canIncBulletCount(), "can 5");
            wpn.IncBulletCount(5);
            b &= assertEquals(35u, wpn.getMagBulletCount(), "5");

            b &= assertTrue(wpn.canIncBulletCount(), "can 6");
            wpn.IncBulletCount(0);
            b &= assertEquals(35u, wpn.getMagBulletCount(), "6");

            b &= assertTrue(wpn.canIncBulletCount(), "can 7");
            wpn.IncBulletCount(1000);
            b &= assertEquals(999u, wpn.getMagBulletCount(), "7");

            b &= assertFalse(wpn.canIncBulletCount(), "can yyy");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reload_when_no_more_bullets_does_not_reload()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            // by default no unmag bullets are available, and we set mag to 0 as well
            wpn.SetMagBulletCount(0);
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reload_when_full_does_not_reload()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            // by default magazine is full == 30 bullets
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reload_when_not_reloadable_does_not_reload()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            // by default magazine is full == 30 bullets
            wpn.getVars()["reloadable"].Set(0); // set weapon to not reloadable
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
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
            int nNumWpnUpdateChangedBulletCount = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPureFloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(30u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(84u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(1, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertGequals(fMillisecsReloadTook, wpn.getVars()["reload_time"].getAsFloat(), "time");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
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
            int nNumWpnUpdateChangedBulletCount = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPureFloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(21u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(0u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(1, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertGequals(fMillisecsReloadTook, wpn.getVars()["reload_time"].getAsFloat(), "time");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_with_reload_whole_mag_wastes_bullets()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
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
            int nNumWpnUpdateChangedBulletCount = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPureFloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(30u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(70u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(1, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertGequals(fMillisecsReloadTook, wpn.getVars()["reload_time"].getAsFloat(), "time");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_with_reload_whole_mag_wastes_bullets()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
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
            int nNumWpnUpdateChangedBulletCount = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPureFloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(7u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(0u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(1, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertGequals(fMillisecsReloadTook, wpn.getVars()["reload_time"].getAsFloat(), "time");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reload_per_bullet_is_one_by_one()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
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
            std::set<TPureUInt> expectedMagBulletCounts = {25, 26, 27, 28, 29, 30};
            std::set<TPureUInt> expectedUnmagBulletCounts = {100, 99, 98, 97, 96, 95};

            int iWait = 0;
            int nNumWpnUpdateChangedBulletCount = 0;
            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 75) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                expectedMagBulletCounts.erase( wpn.getMagBulletCount() );
                expectedUnmagBulletCounts.erase( wpn.getUnmagBulletCount() );
                Sleep(20);
            }

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(30u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(95u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(5, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertTrue(expectedMagBulletCounts.empty(), "exp mag empty");
            b &= assertTrue(expectedUnmagBulletCounts.empty(), "exp unmag empty");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reload_doesnt_reload_when_already_reloading()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(25); // full would be 30

            b &= assertTrue(wpn.reload(), "reload 1");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertFalse(wpn.reload(), "reload 2");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 2");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_shoot_when_empty_does_not_shoot()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            // by default magazine is full == 30 bullets, set it to 0
            wpn.SetMagBulletCount(0);
            b &= assertFalse(wpn.pullTrigger(), "shoot");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertFalse(wpn.isTriggerReleased(), "trigger"); // since we didnt explicitly called releaseTrigger()
            b &= assertTrue(bullets.empty(), "created bullets empty");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_shoot_during_reloading_per_mag_does_not_shoot()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");

            b &= assertFalse(wpn.pullTrigger(), "shoot");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 2");
            b &= assertTrue(bullets.empty(), "created bullets empty");
            b &= assertEquals(nOriginalMagBulletCount, wpn.getMagBulletCount(), "mag bullet count");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count");

        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            wpn.getVars()["reload_per_mag"].Set(false);   // reload per bullet
            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");

            b &= assertTrue(wpn.pullTrigger(), "shoot");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 2");
            b &= assertFalse(bullets.empty(), "created bullets empty");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count");

        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_shoot_doesnt_shoot_when_already_shooting()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            // first shot allowed
            b &= assertTrue(wpn.pullTrigger(), "shoot 1");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 1");
            b &= assertEquals(1u, bullets.size(), "created bullets size 1");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 1");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 1");

            // second shot now allowed
            b &= assertFalse(wpn.pullTrigger(), "shoot 2");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 2");
            b &= assertEquals(1u, bullets.size(), "created bullets size 2");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 2");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 2");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_automatic_shoot_continuously_in_loop_respects_cooldown_time()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(7);     // full would be 30, just set a lower value to decrease test case time

            // so the idea is that we are frequently checking the mag
            // bullets count and we remove the observed values from the set,
            // so after the loop we expect the set to be empty, proving that
            // bullet counts were changing decreasing by 1
            std::set<TPureUInt> expectedMagBulletCounts = {0, 1, 2, 3, 4, 5, 6};

            int iWait = 0;
            unsigned int nNumPullTriggerReturnedTrue = 0;
            int nNumWpnUpdateChangedMagUnmagBulletCount = 0;
            // firing cooldown is 300 msecs by the file
            do
            {
                iWait++;
                // even if we call pullTrigger(), it must not shoot when conditions dont meet
                if (wpn.pullTrigger())
                {
                    nNumPullTriggerReturnedTrue++;
                }
                if (wpn.update())
                {
                    nNumWpnUpdateChangedMagUnmagBulletCount++;
                }
                expectedMagBulletCounts.erase( wpn.getMagBulletCount() );
                Sleep(20);
            } while ( (wpn.getMagBulletCount() > 0) && (iWait < 150) );
            // let the cooldown after last shot elapse so state can also go back to READY
            Sleep( wpn.getVars()["firing_cooldown"].getAsInt() );
            wpn.update();

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertEquals(7u, bullets.size(), "created bullets size");
            b &= assertEquals(0u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(100u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(bullets.size(), nNumPullTriggerReturnedTrue, "pullTrigger true count");
            b &= assertEquals(0, nNumWpnUpdateChangedMagUnmagBulletCount, "update true count");
            b &= assertTrue(expectedMagBulletCounts.empty(), "exp mag empty");
            b &= assertFalse(wpn.isTriggerReleased(), "trigger"); // since we didnt explicitly called releaseTrigger()
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_semi_shoot_has_to_release_and_pull_trigger_continuously_in_loop()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", bullets, *engine, 0);
            b = true;
    
            b &= assertEquals("semi", wpn.getVars()["firing_mode_def"].getAsString(), "firing_mode_def");
            b &= assertEquals("burst", wpn.getVars()["firing_mode_max"].getAsString(), "firing_mode_max");
            b &= assertEquals(Weapon::FiringMode::WPN_FM_SEMI, wpn.getCurrentFiringMode(), "firing mode");
    
            wpn.SetUnmagBulletCount(12); // make sure we could reload
            wpn.SetMagBulletCount(7);    // full would be 12, just set a lower value to decrease test case time
    
            // so the idea is that we are frequently checking the mag
            // bullets count and we remove the observed values from the set,
            // so after the loop we expect the set to be empty, proving that
            // bullet counts were changing decreasing by 1
            std::set<TPureUInt> expectedMagBulletCounts = { 0, 1, 2, 3, 4, 5, 6 };
    
            // another idea is that in contrast to the automatic-type test above, here we have to release the
            // trigger before any subsequent pull in order to shoot, otherwise it never shoots again.
    
            // So, in the first loop below, we just continuously pull the trigger, never release, we expect only
            // 1 shot this way.
            int iWait = 0;
            unsigned int nNumPullTriggerReturnedTrue = 0;
            int nNumWpnUpdateChangedMagUnmagBulletCount = 0; // wpn.update() would change mag/unmag count only during reload
            // firing cooldown is 300 msecs by the file
            do
            {
                iWait++;
                // even if we call pullTrigger(), it must not shoot when conditions dont meet
                if (wpn.pullTrigger())
                {
                    nNumPullTriggerReturnedTrue++;
                }
                if (wpn.update())
                {
                    nNumWpnUpdateChangedMagUnmagBulletCount++;
                }
                expectedMagBulletCounts.erase(wpn.getMagBulletCount());
                Sleep(20);
            } while ((wpn.getMagBulletCount() > 0) && (iWait < 150));
            // let the cooldown after last shot elapse so state can also go back to READY
            Sleep(wpn.getVars()["firing_cooldown"].getAsInt());
            wpn.update();
    
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertEquals(1u, bullets.size(), "created bullets size");
            b &= assertEquals(6u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(12u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(bullets.size(), nNumPullTriggerReturnedTrue, "pullTrigger true count");
            b &= assertEquals(0, nNumWpnUpdateChangedMagUnmagBulletCount, "update true count");
            b &= assertEquals(6u, expectedMagBulletCounts.size(), "exp mag count");
            b &= assertFalse(wpn.isTriggerReleased(), "trigger"); // since we didnt explicitly called releaseTrigger()
    
            // if above stuff failed, there is no use proceeding with this test case
            if (!b)
            {
                return b;
            }
    
            // In next loop below, we continuously pull AND release the trigger, we expect shooting until wpn becomes empty.
            wpn.releaseTrigger();
            iWait = 0;
            nNumPullTriggerReturnedTrue = 0;
            nNumWpnUpdateChangedMagUnmagBulletCount = 0; // wpn.update() would change mag/unmag count only during reload
            do
            {
                iWait++;
                // even if we call pullTrigger(), it must not shoot when conditions dont meet
                if (wpn.pullTrigger())
                {
                    nNumPullTriggerReturnedTrue++;
                }
                wpn.releaseTrigger();
                if (wpn.update())
                {
                    nNumWpnUpdateChangedMagUnmagBulletCount++;
                }
                expectedMagBulletCounts.erase(wpn.getMagBulletCount());
                Sleep(20);
            } while ((wpn.getMagBulletCount() > 0) && (iWait < 150));
            // let the cooldown after last shot elapse so state can also go back to READY
            Sleep(wpn.getVars()["firing_cooldown"].getAsInt());
            wpn.update();
    
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(7u, bullets.size(), "created bullets size 2");
            b &= assertEquals(0u, wpn.getMagBulletCount(), "mag 2");
            b &= assertEquals(12u, wpn.getUnmagBulletCount(), "unmag 2");
            b &= assertEquals(6u, nNumPullTriggerReturnedTrue, "pullTrigger true count 2");
            b &= assertEquals(0, nNumWpnUpdateChangedMagUnmagBulletCount, "update true count 2");
            b &= assertTrue(expectedMagBulletCounts.empty(), "exp mag empty");
            b &= assertTrue(wpn.isTriggerReleased(), "trigger 2");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }
    
        return b;
    }

    bool test_wpn_reload_doesnt_reload_during_shooting()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            b &= assertTrue(wpn.pullTrigger(), "shoot");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 1");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 1");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 1");

            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 2");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 2");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 2");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_reset_sets_defaults()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            wpn.pullTrigger(); // default trigger released state is true, this changes to false
            wpn.SetUnmagBulletCount(100); // default would be 0
            wpn.SetMagBulletCount(14); // full would be 30
            wpn.SetAvailable(true); // default is false

            wpn.Reset();

            b &= assertEquals(nOriginalMagBulletCount, wpn.getMagBulletCount(), "mag bullet count");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count");
            b &= assertFalse(wpn.isAvailable(), "available");
            b &= assertTrue(wpn.isTriggerReleased(), "trigger");

        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_get_damage_per_fire_rating()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);

            b = assertEquals((wpn.getVars().at("damage_hp").getAsInt() * wpn.getVars().at("damage_ap").getAsInt()) / 100.f,
                wpn.getDamagePerFireRating(), 0.001f, "1");
            b &= assertBetween(0.01f, 100.f, wpn.getDamagePerFireRating(), 0.001f, "2");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_get_damage_per_second_rating()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);

            b = assertEquals(std::powf(1000.f / wpn.getVars().at("firing_cooldown").getAsInt() * wpn.getDamagePerFireRating(), 2.f),
                wpn.getDamagePerSecondRating(), 0.001f, "1");
            b &= assertGreater(wpn.getDamagePerSecondRating(), 0.f, "2");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_shoot_creates_bullet_with_same_angle_and_pos_as_weapon()
    {
        bool b = false;
        const pge_network::PgeNetworkConnectionHandle connHandle = 52;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, connHandle);
            b = true;

            wpn.getObject3D().getPosVec().Set(1.f, 2.f, 3.f);
            wpn.getObject3D().getAngleVec().Set(30.f, 40.f, 50.f);
            b &= assertTrue(wpn.pullTrigger(), "shoot");
            b &= assertFalse(wpn.isTriggerReleased(), "trigger");
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
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_release_trigger_after_shoot()
    {
        bool b = false;
        const pge_network::PgeNetworkConnectionHandle connHandle = 52;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, connHandle);
            b = true;

            b &= assertTrue(wpn.pullTrigger(), "shoot");
            b &= assertFalse(wpn.isTriggerReleased(), "trigger 1");

            // wpn state cares about cooldown, but trigger state is controlled by input
            wpn.releaseTrigger();
            b &= assertTrue(wpn.isTriggerReleased(), "trigger 2");

        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_update_position_updates_weapon_object_position()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            wpn.UpdatePosition(PureVector(10.f, 20.f, 30.f));
            b &= assertEquals(PureVector(10.f, 20.f, 30.f), wpn.getObject3D().getPosVec(), "pos");

        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f, 0.f), PureVector(1.f, 1.f, 0.f));
            b &= assertEquals(PureVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 1");
            b &= assertEquals(PureVector(-30.f /* angleX is untouched */, 180.f, -45.f), wpn.getObject3D().getAngleVec(), "angle right up");

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f, 0.f), PureVector(-1.f, 1.f, 0.f));
            b &= assertEquals(PureVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 2");
            b &= assertEquals(PureVector(-30.f /* angleX is untouched */, 0.f, -45.f), wpn.getObject3D().getAngleVec(), "angle left up");

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f, 0.f), PureVector(-1.f, -1.f, 0.f));
            b &= assertEquals(PureVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 3");
            b &= assertEquals(PureVector(-30.f /* angleX is untouched */, 0.f, 45.f), wpn.getObject3D().getAngleVec(), "angle left bottom");

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f, 0.f), PureVector(-1.f, -1.f, 0.f));
            b &= assertEquals(PureVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 4");
            b &= assertEquals(PureVector(-30.f /* angleX is untouched */, 0.f, 45.f), wpn.getObject3D().getAngleVec(), "angle right bottom");

        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_update_positions_updates_weapon_object_position_and_angle_2()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 0);
            b = true;

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f, 0.f), 180.f, 60.f);
            b &= assertEquals(PureVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos");
            b &= assertEquals(PureVector(-30.f /* angleX is untouched */, 180.f, 60.f), wpn.getObject3D().getAngleVec(), "angle");

        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wm_initially_empty()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        return assertTrue(wm.getWeapons().empty(), "weapons") & assertTrue(wm.getBullets().empty(), "bullets") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon") &
            assertNull(wm.getCurrentWeapon(), "currentWeapon") &
            assertEquals(0, wm.getTimeLastWeaponSwitch().time_since_epoch().count(), "last wpn switch");
    }

    bool test_wm_clear_weapons()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load");
        b &= assertTrue(wm.setDefaultAvailableWeaponByFilename("sample_good_wpn_automatic.txt"), "setDefaultAvailable");
        b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[0], true, false), "setCurrentWeapon");

        wm.Clear();

        return b & assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon") &
            assertNull(wm.getCurrentWeapon(), "currentWeapon") &
            assertEquals(0, wm.getTimeLastWeaponSwitch().time_since_epoch().count(), "last wpn switch");
    }

    bool test_wm_set_default_available_weapon()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load");
        
        b &= assertFalse(wm.setDefaultAvailableWeaponByFilename("xxx"), "setDefaultAvailable 1");
        b &= assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon 1");

        b &= assertTrue(wm.setDefaultAvailableWeaponByFilename("sample_good_wpn_automatic.txt"), "setDefaultAvailable 2");
        b &= assertEquals("sample_good_wpn_automatic.txt", wm.getDefaultAvailableWeaponFilename(), "defaultWeapon 2");

        return b;
    }

    bool test_wm_load_weapon_bad_assignment()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_bad_assignment.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_load_weapon_unaccepted_var()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_unaccepted_var.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_load_weapon_missing_var()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_missing_var.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_load_weapon_double_defined_var()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_double_defined_var.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_load_weapon_good()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load");
        b &= assertFalse(wm.getWeapons().empty(), "not empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_get_set_current_weapon()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");

        if (b)
        {
            b &= assertNull(wm.getCurrentWeapon(), "current 1");
            b &= assertFalse(wm.setCurrentWeapon(wm.getWeapons()[0], true, true /*server*/), "try switch to non-avail 1 server should not work");
            b &= assertNull(wm.getCurrentWeapon(), "current 2");
            b &= assertEquals(0, wm.getTimeLastWeaponSwitch().time_since_epoch().count(), "last switch time should not change 1");

            b &= assertFalse(wm.setCurrentWeapon(wm.getWeapons()[0], true, true /*server*/), "try switch to non-avail 2 server should not work");
            b &= assertNull(wm.getCurrentWeapon(), "current 3");
            b &= assertEquals(0, wm.getTimeLastWeaponSwitch().time_since_epoch().count(), "last switch time should not change 2");

            wm.getWeapons()[0]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[0], true, true /*server*/), "try switch to avail 1 server");
            b &= assertEquals(wm.getWeapons()[0], wm.getCurrentWeapon(), "current 4");
            b &= assertEquals(0, wm.getTimeLastWeaponSwitch().time_since_epoch().count(), "last switch time should not change when this is our first used weapon");

            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[1], true, false /*client*/), "try switch to non-avail 2 client should work");
            b &= assertEquals(wm.getWeapons()[1], wm.getCurrentWeapon(), "current 4");
            const auto nLastSwitchTimeSinceEpoch = wm.getTimeLastWeaponSwitch().time_since_epoch().count();
            b &= assertNotEquals(0, nLastSwitchTimeSinceEpoch, "last switch time should change 2");

            b &= assertFalse(wm.setCurrentWeapon(nullptr, true, false), "switch to null should not work");
            b &= assertEquals(nLastSwitchTimeSinceEpoch, wm.getTimeLastWeaponSwitch().time_since_epoch().count(), "last switch time should not change 4");

            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, *engine, 10);
            b &= assertFalse(wm.setCurrentWeapon(&wpn, true, false), "switch to not owned wpn should not work");
            b &= assertEquals(nLastSwitchTimeSinceEpoch, wm.getTimeLastWeaponSwitch().time_since_epoch().count(), "last switch time should not change 5");
        }

        return b;
    }

    bool test_wm_get_prev_avail_weapon()
    {
        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "sample_good_wpn_automatic.txt"},
            {'4', "sample_good_wpn_railgun.txt"},
        };

        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_railgun.txt", 0), "load 3");

        constexpr int iMchgun = 0;
        constexpr int iPistol = 1;
        constexpr int iRailgun = 2;

        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getPrevAvailableWeapon(cWpnTarget);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");

            // only pistol is available and becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");
            
            pWpnTarget = wm.getPrevAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to no other available");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to no other available");
            
            // machinegun becomes available
            wm.getWeapons()[iMchgun]->SetAvailable(true);
            
            pWpnTarget = wm.getPrevAvailableWeapon(cWpnTarget);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun");
            
            // machinegun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iMchgun], false, true /*server*/), "try switch to avail 2 server");
            
            pWpnTarget = wm.getPrevAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol");

            // rail gun becomes available
            wm.getWeapons()[iRailgun]->SetAvailable(true);
            pWpnTarget = wm.getPrevAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol 2");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol 2");

            // pistol becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 3 server");
            pWpnTarget = wm.getPrevAvailableWeapon(cWpnTarget);
            b &= assertEquals('4', cWpnTarget, "cWpnTarget should change to railgun");
            b &= assertEquals(wm.getWeapons()[iRailgun], pWpnTarget, "pWpnTarget should change to railgun");

            // railgun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iRailgun], false, true /*server*/), "try switch to avail 4 server");
            pWpnTarget = wm.getPrevAvailableWeapon(cWpnTarget);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun 2");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun 2");
        }

        return b;
    }

    bool test_wm_get_prev_avail_weapon_not_in_keytoweaponmap()
    {
        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "sample_good_wpn_asdasd.txt"},
        };

        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");

        constexpr int iPistol = 1;

        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getPrevAvailableWeapon(cWpnTarget);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");

            // both loaded weapons become available and pistol becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");

            pWpnTarget = wm.getPrevAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to other weapon added to KeypressToWeaponMap is unknown weapon");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to other weapon added to KeypressToWeaponMap is unknown weapon");
        }

        return b;
    }

    bool test_wm_get_next_avail_weapon()
    {
        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "sample_good_wpn_automatic.txt"},
            {'4', "sample_good_wpn_railgun.txt"},
        };
        
        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_railgun.txt", 0), "load 3");
         
        constexpr int iMchgun = 0;
        constexpr int iPistol = 1;
        constexpr int iRailgun = 2;
        
        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getNextAvailableWeapon(cWpnTarget);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");
            
            // only pistol is available and becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");
        
            pWpnTarget = wm.getNextAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to no other available");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to no other available");
        
            // machinegun becomes available
            wm.getWeapons()[iMchgun]->SetAvailable(true);
        
            pWpnTarget = wm.getNextAvailableWeapon(cWpnTarget);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun");
        
            // machinegun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iMchgun], false, true /*server*/), "try switch to avail 2 server");
        
            pWpnTarget = wm.getNextAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol");

            // railgun becomes available
            wm.getWeapons()[iRailgun]->SetAvailable(true);
            pWpnTarget = wm.getNextAvailableWeapon(cWpnTarget);
            b &= assertEquals('4', cWpnTarget, "cWpnTarget should change to railgun");
            b &= assertEquals(wm.getWeapons()[iRailgun], pWpnTarget, "pWpnTarget should change to railgun");

            // railgun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iRailgun], false, true /*server*/), "try switch to avail 3 server");
            pWpnTarget = wm.getNextAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol 2");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol 2");
        }
        
        return b;
    }

    bool test_wm_get_next_avail_weapon_not_in_keytoweaponmap()
    {
        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "sample_good_wpn_asdasd.txt"},
        };

        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");

        constexpr int iMchgun = 0;
        constexpr int iPistol = 1;

        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getNextAvailableWeapon(cWpnTarget);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");

            // both loaded weapons become available and pistol becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            wm.getWeapons()[iMchgun]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");

            pWpnTarget = wm.getNextAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to other weapon added to KeypressToWeaponMap is unknown weapon");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to other weapon added to KeypressToWeaponMap is unknown weapon");
        }

        return b;
    }

    bool test_wm_get_next_best_avail_weapon()
    {
        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "sample_good_wpn_automatic.txt"},
            {'4', "sample_good_wpn_railgun.txt"},
        };

        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_railgun.txt", 0), "load 3");

        constexpr int iMchgun = 0;
        constexpr int iPistol = 1;
        constexpr int iRailgun = 2;

        b &= assertGreater(
            wm.getWeapons()[iMchgun]->getDamagePerSecondRating(),
            wm.getWeapons()[iPistol]->getDamagePerSecondRating(),
            "DPSR of mchgun should be greater than pistol");
        b &= assertGreater(
            wm.getWeapons()[iRailgun]->getDamagePerSecondRating(),
            wm.getWeapons()[iMchgun]->getDamagePerSecondRating(),
            "DPSR of railgun should be greater than mchgun");

        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");

            // only pistol is available and becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to no other available");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to no other available");

            // machinegun becomes available
            wm.getWeapons()[iMchgun]->SetAvailable(true);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun");

            // machinegun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iMchgun], false, true /*server*/), "try switch to avail 2 server");

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol since current is mchgun");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol since current is mchgun");

            // railgun becomes available
            wm.getWeapons()[iRailgun]->SetAvailable(true);
            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('4', cWpnTarget, "cWpnTarget should change to railgun");
            b &= assertEquals(wm.getWeapons()[iRailgun], pWpnTarget, "pWpnTarget should change to railgun");

            // railgun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iRailgun], false, true /*server*/), "try switch to avail 3 server");
            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun since current is railgun");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun since current is railgun");

            // machinegun becomes empty
            wm.getWeapons()[iMchgun]->SetUnmagBulletCount(0);
            wm.getWeapons()[iMchgun]->SetMagBulletCount(0);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol due to mchgun empty");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol due to mchgun empty");

            // pistol becomes empty
            wm.getWeapons()[iPistol]->SetUnmagBulletCount(0);
            wm.getWeapons()[iPistol]->SetMagBulletCount(0);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('4', cWpnTarget, "cWpnTarget should stay railgun becomes no better option is available");
            b &= assertEquals(wm.getWeapons()[iRailgun], pWpnTarget, "pWpnTarget should stay railgun becomes no better option is available");
        }

        return b;
    }

    bool test_wm_get_next_best_avail_weapon_not_in_keytoweaponmap()
    {
        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "asdasd.txt"}
        };

        std::list<Bullet> bullets;
        WeaponManager wm(cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");

        constexpr int iMchgun = 0;
        constexpr int iPistol = 1;

        b &= assertGreater(
            wm.getWeapons()[iMchgun]->getDamagePerSecondRating(),
            wm.getWeapons()[iPistol]->getDamagePerSecondRating(),
            "DPSR of mchgun should be greater than pistol");

        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");

            // both loaded weapons become available and pistol becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            wm.getWeapons()[iMchgun]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to other weapon added to KeypressToWeaponMap is unknown weapon");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to other weapon added to KeypressToWeaponMap is unknown weapon");
        }

        return b;
    }

}; 
