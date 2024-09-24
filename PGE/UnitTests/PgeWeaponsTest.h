#pragma once

/*
    ###################################################################################
    PgeWeaponsTest.h
    Unit test for Weapon and WeaponManager.
    Made by PR00F88, West Whiskhyll Entertainment
    2022
    ###################################################################################
*/

#include "UnitTest.h"  // PCH

#include <cmath>
#include <thread>

#include "../../../PFL/PFL/PFL.h"
#include "../Weapons/WeaponManager.h"

class PgeWeaponsTest :
    public UnitTest
{
public:

    PgeWeaponsTest() :
        UnitTest( __FILE__ ),
        m_audio(cfgProfiles)
    {
        engine = NULL;
    }

    PgeWeaponsTest(const PgeWeaponsTest&) = delete;
    PgeWeaponsTest& operator=(const PgeWeaponsTest&) = delete;
    PgeWeaponsTest(PgeWeaponsTest&&) = delete;
    PgeWeaponsTest&& operator=(PgeWeaponsTest&&) = delete;

protected:

    virtual void Initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(WeaponManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(Weapon::getLoggerModuleName(), true);

        m_audio.initialize();

        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        
        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems

        /* weapon load negative tests: general config errors */

        AddSubTest("test_wpn_load_weapon_bad_assignment", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_bad_assignment);
        AddSubTest("test_wpn_load_weapon_unaccepted_var", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_unaccepted_var);
        AddSubTest("test_wpn_load_weapon_missing_var", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_missing_var);
        AddSubTest("test_wpn_load_weapon_double_defined_var", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_double_defined_var);
        
        /* weapon load negative tests: reload config errors */

        AddSubTest("test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag);
        AddSubTest("test_wpn_load_weapon_reloadable_cannot_be_greater_than_cap_max", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_reloadable_cannot_be_greater_than_cap_max);
        AddSubTest("test_wpn_load_weapon_bullets_default_cannot_be_greater_than_non_zero_reloadable", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_bullets_default_cannot_be_greater_than_non_zero_reloadable);
        AddSubTest("test_wpn_load_weapon_bullets_default_cannot_be_greater_than_cap_max", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_bullets_default_cannot_be_greater_than_cap_max);
        AddSubTest("test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag);
        AddSubTest("test_wpn_load_weapon_reload_end_snd_incompatible_with_no_reload_per_mag", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_reload_end_snd_incompatible_with_no_reload_per_mag);

        /* weapon load negative tests: accuracy config errors */

        AddSubTest("test_wpn_load_weapon_acc_angle_cannot_be_negative", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_acc_angle_cannot_be_negative);
        AddSubTest("test_wpn_load_weapon_acc_m_walk_cannot_be_negative", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_acc_m_walk_cannot_be_negative);
        AddSubTest("test_wpn_load_weapon_acc_m_run_cannot_be_negative", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_acc_m_run_cannot_be_negative);
        AddSubTest("test_wpn_load_weapon_acc_m_duck_cannot_be_negative", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_acc_m_duck_cannot_be_negative);
        
        /* weapon load negative tests: firing, recoil and cooldown config errors */

        AddSubTest("test_wpn_load_weapon_recoil_cannot_be_less_than_1", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_recoil_cannot_be_less_than_1);
        AddSubTest("test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown);
        AddSubTest("test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control);
        AddSubTest("test_wpn_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled);
        AddSubTest("test_wpn_firing_cooldown_must_be_positive", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_firing_cooldown_must_be_positive);
        AddSubTest("test_wpn_firing_mode_max_cannot_be_less_than_default", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_firing_mode_max_cannot_be_less_than_default);
        AddSubTest("test_wpn_firing_modes_default_and_max_cannot_be_burst_and_proj", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_firing_modes_default_and_max_cannot_be_burst_and_proj);
        
        /* weapon load negative tests: bullet config errors */

        AddSubTest("test_wpn_load_weapon_max_bullet_speed_incompatible_with_non_zero_bullet_drag", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_max_bullet_speed_incompatible_with_non_zero_bullet_drag);
        AddSubTest("test_wpn_load_weapon_damage_area_size_cannot_be_negative", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_damage_area_size_cannot_be_negative);
        AddSubTest("test_wpn_load_weapon_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse);
        AddSubTest("test_wpn_load_weapon_invalid_damage_area_effect", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_load_weapon_invalid_damage_area_effect);
        AddSubTest("test_wpn_damage_hp_must_be_positive", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_damage_hp_must_be_positive);
        AddSubTest("test_wpn_damage_ap_must_be_positive", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_damage_ap_must_be_positive);
        
        /* weapon load positive tests and other use cases */

        AddSubTest("test_wpn_state_to_string", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_state_to_string);

        AddSubTest("test_wpn_load_weapon_good", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_load_weapon_good);

        AddSubTest("test_wpn_set_available", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_set_available);
        AddSubTest("test_wpn_set_owner", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_set_owner);
        AddSubTest("test_wpn_set_mag_bullet_count", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_set_mag_bullet_count);
        AddSubTest("test_wpn_set_unmag_bullet_count", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_set_unmag_bullet_count);
        AddSubTest("test_wpn_inc_bullet_count", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_inc_bullet_count);

        AddSubTest("test_wpn_update_position_updates_weapon_object_position", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_update_position_updates_weapon_object_position);
        AddSubTest("test_wpn_update_positions_updates_weapon_object_position_and_angle_1", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_update_positions_updates_weapon_object_position_and_angle_1);
        AddSubTest("test_wpn_update_positions_updates_weapon_object_position_and_angle_2", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_update_positions_updates_weapon_object_position_and_angle_2);

        AddSubTest("test_wpn_get_damage_per_fire_rating", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_get_damage_per_fire_rating);
        AddSubTest("test_wpn_get_damage_per_second_rating", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_get_damage_per_second_rating);

        AddSubTest("test_wpn_client_receive_state_from_server", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_client_receive_state_from_server);
        
        /* operational tests: reload */

        AddSubTest("test_wpn_reload_when_no_more_bullets_does_not_reload", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_when_no_more_bullets_does_not_reload);
        AddSubTest("test_wpn_reload_when_full_does_not_reload", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_when_full_does_not_reload);
        AddSubTest("test_wpn_reload_when_not_reloadable_does_not_reload", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_when_not_reloadable_does_not_reload);
        AddSubTest("test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets",
            (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets);
        AddSubTest("test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets",
            (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_without_reload_whole_mag_does_not_waste_bullets);
        AddSubTest("test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_with_reload_whole_mag_wastes_bullets",
            (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_per_mag_when_we_have_atleast_full_spare_mag_with_reload_whole_mag_wastes_bullets);
        AddSubTest("test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_with_reload_whole_mag_wastes_bullets",
            (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_per_mag_when_we_do_not_have_full_spare_mag_with_reload_whole_mag_wastes_bullets);
        AddSubTest("test_wpn_reload_per_bullet_is_one_by_one", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_per_bullet_is_one_by_one);
        AddSubTest("test_wpn_reload_doesnt_reload_when_already_reloading", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_doesnt_reload_when_already_reloading);

        /* operational tests: firing */

        AddSubTest("test_wpn_accuracy_angle_by_pose", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_accuracy_angle_by_pose);
        AddSubTest("test_wpn_get_lowest_accuracy_by_pose", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_get_lowest_accuracy_by_pose);
        AddSubTest("test_wpn_get_lowest_accuracy_possible", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_get_lowest_accuracy_possible);
        AddSubTest("test_wpn_get_random_relative_bullet_angle", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_get_random_relative_bullet_angle);
        AddSubTest("test_wpn_shoot_creates_bullet_within_momentary_accuracy_range_of_weapon", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_shoot_creates_bullet_within_momentary_accuracy_range_of_weapon);
        AddSubTest("test_wpn_release_trigger_after_shoot", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_release_trigger_after_shoot);
        AddSubTest("test_wpn_shoot_when_empty_does_not_shoot", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_shoot_when_empty_does_not_shoot);
        AddSubTest("test_wpn_shoot_during_reloading_per_mag_does_not_shoot", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_shoot_during_reloading_per_mag_does_not_shoot);
        AddSubTest("test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_shoot_during_reloading_per_bullet_interrupts_reloading);
        AddSubTest("test_wpn_shoot_doesnt_shoot_when_already_shooting", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_shoot_doesnt_shoot_when_already_shooting);
        AddSubTest("test_wpn_automatic_shoot_continuously_in_loop_respects_cooldown_time", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_automatic_shoot_continuously_in_loop_respects_cooldown_time);
        AddSubTest("test_wpn_semi_shoot_has_to_release_and_pull_trigger_continuously_in_loop", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_semi_shoot_has_to_release_and_pull_trigger_continuously_in_loop);
        AddSubTest("test_wpn_reload_doesnt_reload_during_shooting", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wpn_reload_doesnt_reload_during_shooting);
        AddSubTest("test_wpn_reset_sets_defaults", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wpn_reset_sets_defaults);
        
        /* WeaponManager */

        AddSubTest("test_wm_initially_empty", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wm_initially_empty);
        AddSubTest("test_wm_clear_weapons", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wm_clear_weapons);
        AddSubTest("test_wm_set_default_available_weapon", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_set_default_available_weapon);
        AddSubTest("test_wm_load_weapon_bad_assignment", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wm_load_weapon_bad_assignment);
        AddSubTest("test_wm_load_weapon_unaccepted_var", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wm_load_weapon_unaccepted_var);
        AddSubTest("test_wm_load_weapon_missing_var", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wm_load_weapon_missing_var);
        AddSubTest("test_wm_load_weapon_double_defined_var", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wm_load_weapon_double_defined_var);
        AddSubTest("test_wm_load_weapon_good", (PFNUNITSUBTEST) &PgeWeaponsTest::test_wm_load_weapon_good);
        AddSubTest("test_wm_get_set_current_weapon", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_get_set_current_weapon);
        AddSubTest("test_wm_get_prev_avail_weapon", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_get_prev_avail_weapon);
        AddSubTest("test_wm_get_prev_avail_weapon_not_in_keytoweaponmap", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_get_prev_avail_weapon_not_in_keytoweaponmap);
        AddSubTest("test_wm_get_next_avail_weapon", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_get_next_avail_weapon);
        AddSubTest("test_wm_get_next_avail_weapon_not_in_keytoweaponmap", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_get_next_avail_weapon_not_in_keytoweaponmap);
        AddSubTest("test_wm_get_next_best_avail_weapon_with_any_ammo", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_get_next_best_avail_weapon_with_any_ammo);
        AddSubTest("test_wm_get_next_best_avail_weapon_with_any_ammo_not_in_keytoweaponmap", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_get_next_best_avail_weapon_with_any_ammo_not_in_keytoweaponmap);
        AddSubTest("test_wm_get_next_best_avail_weapon_with_loaded_ammo", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_get_next_best_avail_weapon_with_loaded_ammo);
        AddSubTest("test_wm_get_next_best_avail_weapon_with_loaded_ammo_not_in_keytoweaponmap", (PFNUNITSUBTEST)&PgeWeaponsTest::test_wm_get_next_best_avail_weapon_with_loaded_ammo_not_in_keytoweaponmap);
    }

    virtual bool setUp() override
    {
        return assertTrue(engine && engine->isInitialized());
    }

    virtual void TearDown() override
    {
    }

    virtual void Finalize() override
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }

        m_audio.shutdown();

        CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PGEcfgFile::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(WeaponManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(Weapon::getLoggerModuleName(), false);
    }

private:

    pge_audio::PgeAudio m_audio;
    PR00FsUltimateRenderingEngine* engine;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    bool test_helper_wpn_load_and_expect_exception(const std::string& sFilename, const std::string& sExpectedExceptionString)
    {
        if (!PFL::fileExists(sFilename.c_str()))
        {
            // these tests must throw exception due to invalid content of the file, not because the file is missing!
            return assertTrue(false, (std::string("file doesnt exist: ") + sFilename).c_str());
        }

        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn(sFilename.c_str(), bullets, m_audio, *engine, 0);
        }
        catch (const std::exception& e)
        {
            if (std::string(e.what()).find(sExpectedExceptionString) != std::string::npos)
            {
                b = true;
            }
        }

        return b;
    }

    bool test_helper_check_random_relative_bullet_angles(
        bool& b,
        Weapon& wpn,
        const bool& bMoving, const bool& bRun, const bool& bDuck)
    {
        if (!b)
        {
            return false;
        }

        constexpr float fEps = 0.001f;
        // since recoil is not changed by getRandomRelativeBulletAngle(), recoil factor stays 1.f the entire time
        const float fMaxAbsAccAngle = wpn.getMomentaryAccuracy(bMoving, bRun, bDuck);
        
        int i = 0;
        while (b && (i < 100))
        {
            const float fRandomRelativeAngle = wpn.getRandomRelativeBulletAngle(bMoving, bRun, bDuck);
            b &= assertGequals(fMaxAbsAccAngle, fRandomRelativeAngle, fEps);
            b &= assertLequals(-fMaxAbsAccAngle, fRandomRelativeAngle, fEps);
            i++;
        }

        assertTrue(
            b,
            (std::string(__func__) + "() failed with params: bMoving: " + std::to_string(bMoving) + ", bRun: " + std::to_string(bRun) + ", bDuck: " + std::to_string(bDuck) + "!").c_str());

        return b;
    }

    bool test_helper_shoot_all_bullets_from_magazine_and_check_bullets(
        bool& b,
        Weapon& wpn,
        std::list<Bullet>& bullets,
        const bool& bMoving, const bool& bRun, const bool& bDuck,
        const bool& bWithRecoilFactor)
    {
        if (!b)
        {
            return false;
        }

        constexpr float fEps = 0.001f;
        const int nMaxShots = wpn.getVars()["reloadable"].getAsInt();
        const float fMaxAbsAccAngle =
            bWithRecoilFactor ?
            wpn.getMomentaryAccuracy(bMoving, bRun, bDuck) + wpn.getVars()["recoil_m"].getAsFloat() :
            wpn.getMomentaryAccuracy(bMoving, bRun, bDuck);

        // empty the magazine
        int i = 0;
        while (b && (i < nMaxShots))
        {
            i++;
            b &= assertTrue(wpn.pullTrigger(bMoving, bRun, bDuck), "shoot");  // should transition READY -> SHOOTING
            b &= assertEquals(static_cast<size_t>(i), bullets.size(), "bullets size in loop");
            
            // if we dont want to see recoil factor affecting accuracy, we need to sleep at least recoil_cooldown millisecs!
            if (bWithRecoilFactor)
            {
                // with bWithRecoilFactor, we sleep just enough for a next shot, allowing recoil to accumulate, that is why we have
                // increased fMaxAbsAccAngle when bWithRecoilFactor is true
                std::this_thread::sleep_for(std::chrono::milliseconds(wpn.getVars()["firing_cooldown"].getAsInt()));
            }
            else
            {
                // recoil_cooldown >= firing_cooldown, so sleeping for recoil_cooldown implicitly means sleeping for firing_cooldown as well
                std::this_thread::sleep_for(std::chrono::milliseconds(wpn.getVars()["recoil_cooldown"].getAsInt()));
            }
            wpn.update(); // should transition SHOOTING -> READY

            // since this is an automatic weapon, no need for releasing the trigger between shots
        }
        
        // reset weapon to initial state
        wpn.releaseTrigger();
        wpn.SetMagBulletCount(static_cast<unsigned int>(nMaxShots));
        b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "wpn state after firing loop");

        // check bullet properties
        b &= assertEquals(static_cast<size_t>(nMaxShots), bullets.size(), "bullets final size");
        if (b)
        {
            bool bNotAllRelativeAnglesWereZero = false;
            i = 0;
            for (const auto& bullet : bullets)
            {
                b &= assertEquals(wpn.getVars()["bullet_visible"].getAsBool(), bullet.getObject3D().isRenderingAllowed(), ("visible " + std::to_string(i)).c_str());
                b &= assertEquals(wpn.getObject3D().getPosVec(), bullet.getObject3D().getPosVec(), ("pos " + std::to_string(i)).c_str());
                b &= assertEquals(wpn.getObject3D().getAngleVec().getY(), bullet.getObject3D().getAngleVec().getY(), ("angle Y " + std::to_string(i)).c_str());
                const float fAbsRelativeBulletAngleZ = std::abs(wpn.getObject3D().getAngleVec().getZ() - bullet.getObject3D().getAngleVec().getZ());
                if (fAbsRelativeBulletAngleZ != 0.f)
                {
                    bNotAllRelativeAnglesWereZero = true;
                }
                b &= assertGequals(fMaxAbsAccAngle, fAbsRelativeBulletAngleZ, fEps, ("angle Z " + std::to_string(i)).c_str());
                b &= assertEquals(wpn.getOwner(), bullet.getOwner(), ("bullet owner " + std::to_string(i)).c_str());
                b &= assertEquals(wpn.getVars()["damage_hp"].getAsInt(), bullet.getDamageHp(), ("damageHp " + std::to_string(i)).c_str());
                b &= assertEquals(wpn.getVars()["damage_ap"].getAsInt(), bullet.getDamageAp(), ("damageAp " + std::to_string(i)).c_str());

                i++;
            }
            b &= assertTrue(bNotAllRelativeAnglesWereZero, "all relative angles were 0");
        }

        assertTrue(b, (std::string(__func__) + "() failed with params: bMoving: " + std::to_string(bMoving) + ", bRun: " + std::to_string(bRun) + ", bDuck: " + std::to_string(bDuck) +
            ", bWithRecoilFactor: " + std::to_string(bWithRecoilFactor) + "!").c_str());

        bullets.clear();

        return b;
    }

    bool test_wpn_load_weapon_bad_assignment()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_bad_assignment.txt",
            "");
    }

    bool test_wpn_load_weapon_unaccepted_var()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_unaccepted_var.txt",
            "");
    }

    bool test_wpn_load_weapon_missing_var()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_missing_var.txt",
            "");
    }

    bool test_wpn_load_weapon_double_defined_var()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_double_defined_var.txt",
            "");
    }

    bool test_wpn_load_weapon_not_reloadable_incompatible_with_reload_per_mag()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_not_reloadable_incompatible_with_reload_per_mag.txt",
            "reloadable is 0 but reload_per_mag is true");
    }

    bool test_wpn_load_weapon_reloadable_cannot_be_greater_than_cap_max()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_reloadable_cannot_be_greater_than_cap_max.txt",
            "reloadable cannot be greater than cap_max");
    }

    bool test_wpn_load_weapon_bullets_default_cannot_be_greater_than_non_zero_reloadable()
    {     
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_bullets_default_cannot_be_greater_than_non_zero_reloadable.txt",
            "bullets_default cannot be greater than reloadable when the latter is non-zero");
    }

    bool test_wpn_load_weapon_bullets_default_cannot_be_greater_than_cap_max()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_bullets_default_cannot_be_greater_than_cap_max.txt",
            "bullets_default cannot be greater than cap_max");
    }

    bool test_wpn_load_weapon_reload_whole_mag_incompatible_with_no_reload_per_mag()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_reload_whole_mag_incompatible_with_no_reload_per_mag.txt",
            "reload_whole_mag is true but reload_per_mag is false");
    }

    bool test_wpn_load_weapon_reload_end_snd_incompatible_with_no_reload_per_mag()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_reload_end_snd_incompatible_with_no_reload_per_mag.txt",
            "reload_end_snd is set but reload_per_mag is false");
    }

    bool test_wpn_load_weapon_acc_angle_cannot_be_negative()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_acc_angle_cannot_be_negative.txt",
            "acc_angle cannot be negative");
    }

    bool test_wpn_load_weapon_acc_m_walk_cannot_be_negative()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_acc_m_walk_cannot_be_negative.txt",
            "acc_m_walk cannot be negative");
    }

    bool test_wpn_load_weapon_acc_m_run_cannot_be_negative()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_acc_m_run_cannot_be_negative.txt",
            "acc_m_run cannot be negative");
    }

    bool test_wpn_load_weapon_acc_m_duck_cannot_be_negative()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_acc_m_duck_cannot_be_negative.txt",
            "acc_m_duck cannot be negative");
    }

    bool test_wpn_load_weapon_recoil_cannot_be_less_than_1()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_recoil_cannot_be_less_than_1.txt",
            "recoil_m cannot be less than 1");
    }

    bool test_wpn_load_weapon_no_recoil_incompatible_with_non_zero_recoil_cooldown()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_no_recoil_incompatible_with_non_zero_recoil_cooldown.txt",
            "recoil_m is 1 but recoil_cooldown is non-zero");
    }

    bool test_wpn_load_weapon_no_recoil_incompatible_with_recoil_control()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_no_recoil_incompatible_with_recoil_control.txt",
            "recoil_m is 1 but recoil_control is not off");
    }

    bool test_wpn_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_recoil_cooldown_cannot_be_less_than_firing_cooldown_when_recoil_is_enabled.txt",
            "recoil enabled, but recoil_cooldown is less than firing_cooldown");
    }

    bool test_wpn_firing_cooldown_must_be_positive()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_firing_cooldown_must_be_positive.txt",
            "firing_cooldown must be a positive value");
    }

    bool test_wpn_load_weapon_max_bullet_speed_incompatible_with_non_zero_bullet_drag()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_max_bullet_speed_incompatible_with_non_zero_bullet_drag.txt",
            "bullet_speed is 1000 but bullet_drag is non-zero");
    }

    bool test_wpn_load_weapon_damage_area_size_cannot_be_negative()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_damage_area_size_cannot_be_negative.txt",
            "damage_area_size cannot be negative");
    }

    bool test_wpn_load_weapon_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse.txt",
            "damage_area_size is 0 but damage_area_pulse is non-zero");
    }

    bool test_wpn_load_weapon_invalid_damage_area_effect()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_damage_area_effect_invalid.txt",
            "invalid damage_area_effect");
    }

    bool test_wpn_firing_mode_max_cannot_be_less_than_default()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_firing_mode_max_cannot_be_less_than_default.txt",
            "wrong order of default and max firing modes: auto and semi");
    }

    bool test_wpn_firing_modes_default_and_max_cannot_be_burst_and_proj()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_firing_modes_default_and_max_cannot_be_burst_and_proj.txt",
            "incompatiable default and max firing modes: burst and proj");
    }

    bool test_wpn_damage_hp_must_be_positive()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_damage_hp_must_be_positive.txt",
            "damage_hp and damage_ap must be positive values");
    }

    bool test_wpn_damage_ap_must_be_positive()
    {
        return test_helper_wpn_load_and_expect_exception(
            "gamedata/weapons/wpn_test_damage_ap_must_be_positive.txt",
            "damage_hp and damage_ap must be positive values");
    }

    bool test_wpn_state_to_string()
    {
        bool b = true;
        b &= assertEquals("READY", Weapon::stateToString(Weapon::WPN_READY)) &
            assertEquals("RELOADING", Weapon::stateToString(Weapon::WPN_RELOADING)) &
            assertEquals("SHOOTING", Weapon::stateToString(Weapon::WPN_SHOOTING));

        return b;
    }

    bool test_wpn_load_weapon_good()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 10); // might throw
            b = true; // did not throw
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state") &
                assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old") &
                assertFalse(wpn.getState().isDirty(), "state dirty") &
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
                assertEquals("reload_start_dummy.wav", wpn.getVars()["reload_start_snd"].getAsString(), "reload_start_snd") &
                assertEquals("reload_end_dummy.wav", wpn.getVars()["reload_end_snd"].getAsString(), "reload_end_snd") &
                assertEquals("auto", wpn.getVars()["firing_mode_def"].getAsString(), "firing_mode_def") &
                assertEquals("auto", wpn.getVars()["firing_mode_max"].getAsString(), "firing_mode_max") &
                assertEquals(50, wpn.getVars()["firing_cooldown"].getAsInt(), "firing_cooldown") &
                assertEquals("automatic_dummy.wav", wpn.getVars()["firing_snd"].getAsString(), "firing_snd") &
                assertEquals("automatic_dry_dummy.wav", wpn.getVars()["firing_dry_snd"].getAsString(), "firing_dry_snd") &
                assertEquals(0.0, wpn.getFiringSound().getLength(), "firing snd len") &
                assertEquals(0.0, wpn.getDryFiringSound().getLength(), "firing dry snd len") &
                assertEquals(5.f, wpn.getVars()["acc_angle"].getAsFloat(), "acc_angle") &
                assertEquals(1.2f, wpn.getVars()["acc_m_walk"].getAsFloat(), "acc_m_walk") &
                assertEquals(3.0f, wpn.getVars()["acc_m_run"].getAsFloat(), "acc_m_run") &
                assertEquals(0.6f, wpn.getVars()["acc_m_duck"].getAsFloat(), "acc_m_duck") &
                assertEquals(1.1f, wpn.getVars()["recoil_m"].getAsFloat(), "recoil_m") &
                assertEquals(1.1f, wpn.getMaximumRecoilMultiplier(), "max recoil mplier") &
                assertEquals(100, wpn.getVars()["recoil_cooldown"].getAsInt(), "recoil_cooldown") &
                assertEquals("high", wpn.getVars()["recoil_control"].getAsString(), "recoil_control") &
                assertTrue(wpn.getVars()["bullet_visible"].getAsBool(), "bullet_visible") &
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 10);

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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 10);

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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 10);

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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 10);

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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 10);

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

    bool test_wpn_update_position_updates_weapon_object_position()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.UpdatePosition(PureVector(10.f, 20.f, 30.f), false);
            b &= assertEquals(PureVector(10.f, 20.f + Weapon::WpnYBiasToPlayerCenter, 30.f), wpn.getObject3D().getPosVec(), "pos 1");

            wpn.UpdatePosition(PureVector(10.f, 20.f, 30.f), true);
            b &= assertEquals(PureVector(10.f, 20.f, 30.f), wpn.getObject3D().getPosVec(), "pos 2");

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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            // this test requires proper camera direction, since getCamera().project3dTo2d() is invoked by Weapon::UpdatePositions(PureVector,PureVector)
            engine->getCamera().SetNearPlane(0.1f);
            engine->getCamera().SetFarPlane(100.0f);
            engine->getCamera().getPosVec().Set(0, 0, -5);
            engine->getCamera().getTargetVec().Set(0, 0, 1);

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f - Weapon::WpnYBiasToPlayerCenter, 0.f), PureVector(1.f, 1.f, 0.f));
            b &= assertEquals(PureVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 1");
            b &= assertEquals(PureVector(-30.f /* angleX is untouched */, 180.f, -45.f), wpn.getObject3D().getAngleVec(), "angle right up");

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f - Weapon::WpnYBiasToPlayerCenter, 0.f), PureVector(-1.f, 1.f, 0.f));
            b &= assertEquals(PureVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 2");
            b &= assertEquals(PureVector(-30.f /* angleX is untouched */, 0.f, -45.f), wpn.getObject3D().getAngleVec(), "angle left up");

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f - Weapon::WpnYBiasToPlayerCenter, 0.f), PureVector(-1.f, -1.f, 0.f));
            b &= assertEquals(PureVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos 3");
            b &= assertEquals(PureVector(-30.f /* angleX is untouched */, 0.f, 45.f), wpn.getObject3D().getAngleVec(), "angle left bottom");

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f - Weapon::WpnYBiasToPlayerCenter, 0.f), PureVector(-1.f, -1.f, 0.f));
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.getObject3D().getPosVec().Set(30.f, 30.f, 30.f);
            wpn.getObject3D().getAngleVec().Set(-30.f, -30.f, -30.f);

            wpn.UpdatePositions(PureVector(0.f, 0.f - Weapon::WpnYBiasToPlayerCenter, 0.f), 180.f, 60.f);
            b &= assertEquals(PureVector(0.f, 0.f, 0.f), wpn.getObject3D().getPosVec(), "pos");
            b &= assertEquals(PureVector(-30.f /* angleX is untouched */, 180.f, 60.f), wpn.getObject3D().getAngleVec(), "angle");

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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);

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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);

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

    bool test_wpn_client_receive_state_from_server()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);

            b = assertEquals(Weapon::State::WPN_READY, wpn.getState(), "new state 1");
            b &= assertEquals(Weapon::State::WPN_READY, wpn.getState().getOld(), "old state 1");

            wpn.clientReceiveStateFromServer(Weapon::State::WPN_RELOADING);
            b &= assertEquals(Weapon::State::WPN_RELOADING, wpn.getState(), "new state 2");
            b &= assertEquals(Weapon::State::WPN_READY, wpn.getState().getOld(), "old state 2");

            wpn.clientReceiveStateFromServer(Weapon::State::WPN_SHOOTING);
            b &= assertEquals(Weapon::State::WPN_SHOOTING, wpn.getState(), "new state 3");
            b &= assertEquals(Weapon::State::WPN_RELOADING, wpn.getState().getOld(), "old state 3");
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            // by default no unmag bullets are available, and we set mag to 0 as well
            wpn.SetMagBulletCount(0);
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertFalse(wpn.getState().isDirty(), "state dirty");
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            // by default magazine is full == 30 bullets
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertFalse(wpn.getState().isDirty(), "state dirty");
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            // by default magazine is full == 30 bullets
            wpn.getVars()["reloadable"].Set(0); // set weapon to not reloadable
            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertFalse(wpn.getState().isDirty(), "state dirty");
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");
            
            PFL::timeval timeReloadStarted;
            PFL::gettimeofday(&timeReloadStarted, 0);
            unsigned int iWait = 0;
            unsigned int nNumWpnUpdateChangedBulletCount = 0;
            unsigned int nNumWpnStateChangesCount = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                if (wpn.getState().isDirty())
                {
                    nNumWpnStateChangesCount++;
                    wpn.updateOldValues();
                }
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPureFloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(30u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(84u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(1u, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertEquals(2u, nNumWpnStateChangesCount, "state changes count"); /* READY -> RELOADING -> READY */
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.SetUnmagBulletCount(7); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");

            PFL::timeval timeReloadStarted;
            PFL::gettimeofday(&timeReloadStarted, 0);
            unsigned int iWait = 0;
            unsigned int nNumWpnUpdateChangedBulletCount = 0;
            unsigned int nNumWpnStateChangesCount = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                if (wpn.getState().isDirty())
                {
                    nNumWpnStateChangesCount++;
                    wpn.updateOldValues();
                }
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPureFloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(21u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(0u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(1u, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertEquals(2u, nNumWpnStateChangesCount, "state changes count"); /* READY -> RELOADING -> READY */
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            // "reload_whole_mag" is true in sample wpn file
            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");

            PFL::timeval timeReloadStarted;
            PFL::gettimeofday(&timeReloadStarted, 0);
            unsigned int iWait = 0;
            unsigned int nNumWpnUpdateChangedBulletCount = 0;
            unsigned int nNumWpnStateChangesCount = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                if (wpn.getState().isDirty())
                {
                    nNumWpnStateChangesCount++;
                    wpn.updateOldValues();
                }
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPureFloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(30u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(70u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(1u, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertEquals(2u, nNumWpnStateChangesCount, "state changes count"); /* READY -> RELOADING -> READY */
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            // "reload_whole_mag" is true in sample wpn file
            wpn.SetUnmagBulletCount(7); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30
            // reload time is 1500 msecs by sample wpn file
            wpn.getVars()["reload_time"].Set("200");

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");

            PFL::timeval timeReloadStarted;
            PFL::gettimeofday(&timeReloadStarted, 0);
            unsigned int iWait = 0;
            unsigned int nNumWpnUpdateChangedBulletCount = 0;
            unsigned int nNumWpnStateChangesCount = 0;
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 10) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                if (wpn.getState().isDirty())
                {
                    nNumWpnStateChangesCount++;
                    wpn.updateOldValues();
                }
                Sleep(50);
            }

            PFL::timeval timeReloadEnded;
            PFL::gettimeofday(&timeReloadEnded, 0);
            const TPureFloat fMillisecsReloadTook = PFL::getTimeDiffInUs(timeReloadEnded, timeReloadStarted) / 1000.f;

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(7u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(0u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(1u, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertEquals(2u, nNumWpnStateChangesCount, "state changes count"); /* READY -> RELOADING -> READY */
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
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

            unsigned int iWait = 0;
            unsigned int nNumWpnUpdateChangedBulletCount = 0;
            unsigned int nNumWpnStateChangesCount = 0;
            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");
            while ( (wpn.getState() == Weapon::WPN_RELOADING) && (iWait < 75) )
            {
                iWait++;
                if (wpn.update())
                {
                    nNumWpnUpdateChangedBulletCount++;
                }
                if (wpn.getState().isDirty())
                {
                    nNumWpnStateChangesCount++;
                    wpn.updateOldValues();
                }
                expectedMagBulletCounts.erase( wpn.getMagBulletCount() );
                expectedUnmagBulletCounts.erase( wpn.getUnmagBulletCount() );
                Sleep(20);
            }

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(30u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(95u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(5u, nNumWpnUpdateChangedBulletCount, "update true count");
            b &= assertEquals(2u, nNumWpnStateChangesCount, "state changes count"); /* READY -> RELOADING -> READY */
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(25); // full would be 30

            b &= assertTrue(wpn.reload(), "reload 1");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");

            b &= assertFalse(wpn.reload(), "reload 2");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 2");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 2");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 2");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_accuracy_angle_by_pose()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);

            constexpr float fEps = 0.001f;
            const float fAccAngle = wpn.getVars().at("acc_angle").getAsFloat();
            const float fAccMDuck = wpn.getVars().at("acc_m_duck").getAsFloat();
            const float fAccMRun = wpn.getVars().at("acc_m_run").getAsFloat();
            const float fAccMWalk = wpn.getVars().at("acc_m_walk").getAsFloat();

            b = true;

            // bMoving is false so run and walk are ignored
            b &= assertEquals(fAccAngle, wpn.getAccuracyByPose(false /* bMoving */, true /* bRun */, false /* bDuck */), fEps, "still, run, stand");
            b &= assertEquals(fAccAngle * fAccMDuck, wpn.getAccuracyByPose(false /* bMoving */, true /* bRun */, true /* bDuck */), fEps, "still, run, duck");
            b &= assertEquals(fAccAngle, wpn.getAccuracyByPose(false /* bMoving */, false /* bRun */, false /* bDuck */), fEps, "still, walk, stand");
            b &= assertEquals(fAccAngle * fAccMDuck, wpn.getAccuracyByPose(false /* bMoving */, false /* bRun */, true /* bDuck */), fEps, "still, walk, duck");

            // bMoving is true so run and walk are now considered
            b &= assertEquals(fAccAngle * fAccMRun, wpn.getAccuracyByPose(true /* bMoving */, true /* bRun */, false /* bDuck */), fEps, "moving, run, stand");
            b &= assertEquals(fAccAngle * fAccMRun * fAccMDuck, wpn.getAccuracyByPose(true /* bMoving */, true /* bRun */, true /* bDuck */), fEps, "moving, run, duck");
            b &= assertEquals(fAccAngle * fAccMWalk, wpn.getAccuracyByPose(true /* bMoving */, false /* bRun */, false /* bDuck */), fEps, "moving, walk, stand");
            b &= assertEquals(fAccAngle * fAccMWalk * fAccMDuck, wpn.getAccuracyByPose(true /* bMoving */, false /* bRun */, true /* bDuck */), fEps, "moving, walk, duck");
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_get_lowest_accuracy_by_pose()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);

            // recoil does not play in this test since we are interested in by pose only
            b = true;
            b &= assertEquals(wpn.getAccuracyByPose(true /* bMoving */, true /* bRun */, false /* bDuck */), wpn.getLowestAccuracyByPose());
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_get_lowest_accuracy_possible()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);

            // recoil is not changing in this test, we are interested in the max possible relative bullet angle, given by
            // the lowest accuracy by pose and highest possible recoil multiplier
            b = true;
            b &= assertEquals(wpn.getLowestAccuracyByPose() * wpn.getMaximumRecoilMultiplier(), wpn.getLowestAccuracyPossible());
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_get_random_relative_bullet_angle()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);

            b = true;
            // recoil does not play in this test, since it is not changed in this test
            b &= assertTrue(test_helper_check_random_relative_bullet_angles(b, wpn, false /* bMoving */, true /* bRun */, false /* bDuck */));
            b &= assertTrue(test_helper_check_random_relative_bullet_angles(b, wpn, false /* bMoving */, true /* bRun */, true /* bDuck */));
            b &= assertTrue(test_helper_check_random_relative_bullet_angles(b, wpn, false /* bMoving */, false /* bRun */, false /* bDuck */));
            b &= assertTrue(test_helper_check_random_relative_bullet_angles(b, wpn, false /* bMoving */, false /* bRun */, true /* bDuck */));
            b &= assertTrue(test_helper_check_random_relative_bullet_angles(b, wpn, true /* bMoving */, true /* bRun */, false /* bDuck */));
            b &= assertTrue(test_helper_check_random_relative_bullet_angles(b, wpn, true /* bMoving */, true /* bRun */, true /* bDuck */));
            b &= assertTrue(test_helper_check_random_relative_bullet_angles(b, wpn, true /* bMoving */, false /* bRun */, false /* bDuck */));
            b &= assertTrue(test_helper_check_random_relative_bullet_angles(b, wpn, true /* bMoving */, false /* bRun */, true /* bDuck */));
        }
        catch (const std::exception& e)
        {
            assertTrue(false, e.what());
        }

        return b;
    }

    bool test_wpn_shoot_creates_bullet_within_momentary_accuracy_range_of_weapon()
    {
        bool b = false;

        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 52u);
            b = true;

            wpn.getObject3D().getPosVec().Set(1.f, 2.f, 3.f);
            wpn.getObject3D().getAngleVec().Set(30.f, 40.f, 50.f);

            bool bWithRecoilFactor = false;
            for (int i = 0; i < 2; i++)
            {
                bWithRecoilFactor = !bWithRecoilFactor;
                b &= assertTrue(test_helper_shoot_all_bullets_from_magazine_and_check_bullets(b, wpn, bullets, false /* bMoving */, true /* bRun */, false /* bDuck */, bWithRecoilFactor));
                b &= assertTrue(test_helper_shoot_all_bullets_from_magazine_and_check_bullets(b, wpn, bullets, false /* bMoving */, true /* bRun */, true /* bDuck */, bWithRecoilFactor));
                b &= assertTrue(test_helper_shoot_all_bullets_from_magazine_and_check_bullets(b, wpn, bullets, false /* bMoving */, false /* bRun */, false /* bDuck */, bWithRecoilFactor));
                b &= assertTrue(test_helper_shoot_all_bullets_from_magazine_and_check_bullets(b, wpn, bullets, false /* bMoving */, false /* bRun */, true /* bDuck */, bWithRecoilFactor));
                b &= assertTrue(test_helper_shoot_all_bullets_from_magazine_and_check_bullets(b, wpn, bullets, true /* bMoving */, true /* bRun */, false /* bDuck */, bWithRecoilFactor));
                b &= assertTrue(test_helper_shoot_all_bullets_from_magazine_and_check_bullets(b, wpn, bullets, true /* bMoving */, true /* bRun */, true /* bDuck */, bWithRecoilFactor));
                b &= assertTrue(test_helper_shoot_all_bullets_from_magazine_and_check_bullets(b, wpn, bullets, true /* bMoving */, false /* bRun */, false /* bDuck */, bWithRecoilFactor));
                b &= assertTrue(test_helper_shoot_all_bullets_from_magazine_and_check_bullets(b, wpn, bullets, true /* bMoving */, false /* bRun */, true /* bDuck */, bWithRecoilFactor));
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, connHandle);
            b = true;

            b &= assertTrue(wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */), "shoot");
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

    bool test_wpn_shoot_when_empty_does_not_shoot()
    {
        bool b = false;
        try
        {
            std::list<Bullet> bullets;
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            // by default magazine is full == 30 bullets, set it to 0
            wpn.SetMagBulletCount(0);
            b &= assertFalse(wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */), "shoot");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertFalse(wpn.getState().isDirty(), "state dirty");
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.getVars()["reload_whole_mag"].Set(false); // reload does not waste bullets
            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(14); // full would be 30

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");

            b &= assertFalse(wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */), "shoot");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 2");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 2");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 2");
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_bazooka.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(3); // make sure we could reload
            wpn.SetMagBulletCount(1); // full would be 3

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            b &= assertTrue(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_RELOADING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");

            unsigned int iWait = 0;
            unsigned int nNumPullTriggerReturnedTrue = 0;
            unsigned int nNumWpnStateChangesCount = 0;
            bool bChangedToShooting = false;
            do
            {
                iWait++;

                if (wpn.getState() == Weapon::WPN_RELOADING)
                {
                    // shoot to cancel reloading
                    if (wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */))
                    {
                        nNumPullTriggerReturnedTrue++;
                        wpn.releaseTrigger();
                        bChangedToShooting = (wpn.getState() == Weapon::WPN_SHOOTING);
                    }
                }
                wpn.update();
                if (wpn.getState().isDirty())
                {
                    nNumWpnStateChangesCount++;
                    wpn.updateOldValues();
                }
                Sleep(20);
            } while ((wpn.getState() != Weapon::WPN_READY) && (iWait < 150));

            b &= assertTrue(bChangedToShooting, "changed to shooting");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state settled down");
            b &= assertEquals(bullets.size(), nNumPullTriggerReturnedTrue, "pullTrigger true count");
            b &= assertEquals(2u, nNumWpnStateChangesCount, "state changes count"); /* READY -> SHOOTING -> READY */
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            // first shot allowed
            b &= assertTrue(wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */), "shoot 1");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");
            b &= assertEquals(1u, bullets.size(), "created bullets size 1");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 1");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 1");

            // second shot now allowed
            b &= assertFalse(wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */), "shoot 2");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 2");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            wpn.SetUnmagBulletCount(100); // make sure we could reload
            wpn.SetMagBulletCount(7);     // full would be 30, just set a lower value to decrease test case time
            const auto nExpectedShotsToBeFired = wpn.getMagBulletCount();

            // so the idea is that we are frequently checking the mag
            // bullets count and we remove the observed values from the set,
            // so after the loop we expect the set to be empty, proving that
            // bullet counts were changing decreasing by 1
            std::set<TPureUInt> expectedMagBulletCounts = {0, 1, 2, 3, 4, 5, 6};

            unsigned int iWait = 0;
            unsigned int nNumPullTriggerReturnedTrue = 0;
            unsigned int nNumWpnUpdateChangedMagUnmagBulletCount = 0;
            unsigned int nNumWpnStateChangesCount = 0;
            // firing cooldown is 300 msecs by the file
            do
            {
                iWait++;
                // even if we call pullTrigger(), it must not shoot when conditions dont meet
                if (wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */))
                {
                    nNumPullTriggerReturnedTrue++;
                }
                if (wpn.update())
                {
                    nNumWpnUpdateChangedMagUnmagBulletCount++;
                }
                if (wpn.getState().isDirty())
                {
                    nNumWpnStateChangesCount++;
                    wpn.updateOldValues();
                }
                expectedMagBulletCounts.erase( wpn.getMagBulletCount() );
                Sleep(20);
            } while ( (wpn.getMagBulletCount() > 0) && (iWait < 150) );
            // let the cooldown after last shot elapse so state can also go back to READY
            Sleep( wpn.getVars()["firing_cooldown"].getAsInt() );
            wpn.update();

            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertEquals(nExpectedShotsToBeFired, bullets.size(), "created bullets size");
            b &= assertEquals(0u, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(100u, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(bullets.size(), nNumPullTriggerReturnedTrue, "pullTrigger true count");
            b &= assertEquals(0u, nNumWpnUpdateChangedMagUnmagBulletCount, "update true count");
            b &= assertEquals(
                nExpectedShotsToBeFired * 2 - 1,
                nNumWpnStateChangesCount,
                "state changes count"); /* 1 shot fired generates a (READY -> SHOOTING -> READY) state trans sequence, but the last one is not waited for by the loop above */
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", bullets, m_audio, *engine, 0);
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
            constexpr auto nExpectedShotsToBeFired_1 = 1u;
            const auto nOriginalMagCount_1 = wpn.getMagBulletCount();
            const auto nOriginalUnmagCount = wpn.getUnmagBulletCount();
            int iWait = 0;
            unsigned int nNumPullTriggerReturnedTrue = 0;
            unsigned int nNumWpnUpdateChangedMagUnmagBulletCount = 0; // wpn.update() would change mag/unmag count only during reload
            unsigned int nNumWpnStateChangesCount = 0;
            // firing cooldown is 300 msecs by the file
            do
            {
                iWait++;
                // even if we call pullTrigger(), it must not shoot when conditions dont meet
                if (wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */))
                {
                    nNumPullTriggerReturnedTrue++;
                }
                if (wpn.update())
                {
                    nNumWpnUpdateChangedMagUnmagBulletCount++;
                }
                if (wpn.getState().isDirty())
                {
                    nNumWpnStateChangesCount++;
                    wpn.updateOldValues();
                }
                expectedMagBulletCounts.erase(wpn.getMagBulletCount());
                Sleep(20);
            } while ((wpn.getMagBulletCount() > 0) && (iWait < 150));
            // let the cooldown after last shot elapse so state can also go back to READY
            Sleep(wpn.getVars()["firing_cooldown"].getAsInt());
            wpn.update();
    
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state");
            b &= assertEquals(nExpectedShotsToBeFired_1, bullets.size(), "created bullets size");
            b &= assertEquals(nOriginalMagCount_1 - nExpectedShotsToBeFired_1, wpn.getMagBulletCount(), "mag");
            b &= assertEquals(nOriginalUnmagCount, wpn.getUnmagBulletCount(), "unmag");
            b &= assertEquals(bullets.size(), nNumPullTriggerReturnedTrue, "pullTrigger true count");
            b &= assertEquals(0u, nNumWpnUpdateChangedMagUnmagBulletCount, "update true count");
            b &= assertEquals(nExpectedShotsToBeFired_1 * 2, nNumWpnStateChangesCount, "state changes count 1"); /* READY -> SHOOTING -> READY */
            b &= assertEquals(nOriginalMagCount_1 - nExpectedShotsToBeFired_1, expectedMagBulletCounts.size(), "exp mag count");
            b &= assertFalse(wpn.isTriggerReleased(), "trigger"); // since we didnt explicitly called releaseTrigger()
    
            // if above stuff failed, there is no use proceeding with this test case
            if (!b)
            {
                return b;
            }
    
            // In next loop below, we continuously pull AND release the trigger, we expect shooting until wpn becomes empty.
            wpn.releaseTrigger();
            const auto nExpectedShotsToBeFired_2 = wpn.getMagBulletCount();
            iWait = 0;
            nNumPullTriggerReturnedTrue = 0;
            nNumWpnUpdateChangedMagUnmagBulletCount = 0; // wpn.update() would change mag/unmag count only during reload
            nNumWpnStateChangesCount = 0;
            do
            {
                iWait++;
                // even if we call pullTrigger(), it must not shoot when conditions dont meet
                if (wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */))
                {
                    nNumPullTriggerReturnedTrue++;
                }
                wpn.releaseTrigger();
                if (wpn.update())
                {
                    nNumWpnUpdateChangedMagUnmagBulletCount++;
                }
                if (wpn.getState().isDirty())
                {
                    nNumWpnStateChangesCount++;
                    wpn.updateOldValues();
                }
                expectedMagBulletCounts.erase(wpn.getMagBulletCount());
                Sleep(20);
            } while ((wpn.getMagBulletCount() > 0) && (iWait < 150));
            // let the cooldown after last shot elapse so state can also go back to READY
            Sleep(wpn.getVars()["firing_cooldown"].getAsInt());
            wpn.update();
    
            b &= assertEquals(Weapon::WPN_READY, wpn.getState(), "state 2");
            b &= assertEquals(nOriginalMagCount_1, bullets.size(), "created bullets size 2");
            b &= assertEquals(0u, wpn.getMagBulletCount(), "mag 2");
            b &= assertEquals(nOriginalUnmagCount, wpn.getUnmagBulletCount(), "unmag 2");
            b &= assertEquals(nExpectedShotsToBeFired_2, nNumPullTriggerReturnedTrue, "pullTrigger true count 2");
            b &= assertEquals(0u, nNumWpnUpdateChangedMagUnmagBulletCount, "update true count 2");
            b &= assertEquals(
                nExpectedShotsToBeFired_2 * 2 - 1,
                nNumWpnStateChangesCount,
                "state changes count 2"); /* 1 shot fired generates a (READY -> SHOOTING -> READY) state trans sequence, but the last one is not waited for by the loop above */
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            b &= assertTrue(wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */), "shoot");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 1");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 1");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 1");
            b &= assertEquals(nOriginalMagBulletCount - 1, wpn.getMagBulletCount(), "mag bullet count 1");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count 1");

            b &= assertFalse(wpn.reload(), "reload");
            b &= assertEquals(Weapon::WPN_SHOOTING, wpn.getState(), "state 2");
            b &= assertEquals(Weapon::WPN_READY, wpn.getState().getOld(), "state old 2");
            b &= assertTrue(wpn.getState().isDirty(), "state dirty 2");
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
            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 0);
            b = true;

            const TPureUInt nOriginalMagBulletCount = wpn.getMagBulletCount();
            const TPureUInt nOriginalUnmagBulletCount = wpn.getUnmagBulletCount();

            wpn.pullTrigger(false /* bMoving */, true /* bRun */, false /* bDuck */); // default trigger released state is true, this changes to false
            wpn.SetUnmagBulletCount(100); // default would be 0
            wpn.SetMagBulletCount(14); // full would be 30
            wpn.SetAvailable(true); // default is false

            wpn.Reset();

            b &= assertEquals(nOriginalMagBulletCount, wpn.getMagBulletCount(), "mag bullet count");
            b &= assertEquals(nOriginalUnmagBulletCount, wpn.getUnmagBulletCount(), "unmag bullet count");
            b &= assertFalse(wpn.isAvailable(), "available");
            b &= assertTrue(wpn.isTriggerReleased(), "trigger");
            b &= assertEquals(Weapon::State::WPN_READY, wpn.getState(), "state");

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
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = true;
        b &= assertTrue(wm.getWeapons().empty(), "weapons") & assertTrue(wm.getBullets().empty(), "bullets") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon") &
            assertNull(wm.getCurrentWeapon(), "currentWeapon") &
            assertEquals(0, wm.getTimeLastWeaponSwitch().time_since_epoch().count(), "last wpn switch");

        return b;
    }

    bool test_wm_clear_weapons()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load");
        if (b)
        {
            b &= assertTrue(wm.setDefaultAvailableWeaponByFilename("sample_good_wpn_automatic.txt"), "setDefaultAvailable");
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[0], true, false), "setCurrentWeapon");
        }

        wm.Clear();

        b &= assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon") &
            assertNull(wm.getCurrentWeapon(), "currentWeapon") &
            assertEquals(0, wm.getTimeLastWeaponSwitch().time_since_epoch().count(), "last wpn switch");

        return b;
    }

    bool test_wm_set_default_available_weapon()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
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
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_bad_assignment.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_load_weapon_unaccepted_var()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_unaccepted_var.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_load_weapon_missing_var()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_missing_var.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_load_weapon_double_defined_var()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertFalse(wm.load("gamedata/weapons/wpn_test_double_defined_var.txt", 0), "load");
        b &= assertTrue(wm.getWeapons().empty(), "empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_load_weapon_good()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load");
        b &= assertFalse(wm.getWeapons().empty(), "not empty") &
            assertTrue(wm.getDefaultAvailableWeaponFilename().empty(), "defaultWeapon");

        return b;
    }

    bool test_wm_get_set_current_weapon()
    {
        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
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

            Weapon wpn("gamedata/weapons/sample_good_wpn_automatic.txt", bullets, m_audio, *engine, 10);
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
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
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
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
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
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
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
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
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

    bool test_wm_get_next_best_avail_weapon_with_any_ammo()
    {
        // with any ammo means: can be either mag or unmag ammo
        constexpr bool bMustHaveMagBullet = false;

        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "sample_good_wpn_automatic.txt"},
            {'4', "sample_good_wpn_railgun.txt"},
        };

        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_railgun.txt", 0), "load 3");

        constexpr int iMchgun = 0;
        constexpr int iPistol = 1;
        constexpr int iRailgun = 2;

        if (b)
        {
            b &= assertGreater(
                wm.getWeapons()[iMchgun]->getDamagePerSecondRating(),
                wm.getWeapons()[iPistol]->getDamagePerSecondRating(),
                "DPSR of mchgun should be greater than pistol");
            b &= assertGreater(
                wm.getWeapons()[iRailgun]->getDamagePerSecondRating(),
                wm.getWeapons()[iMchgun]->getDamagePerSecondRating(),
                "DPSR of railgun should be greater than mchgun");
        }

        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");

            // only pistol is available and becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to no other available");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to no other available");

            // machinegun becomes available
            wm.getWeapons()[iMchgun]->SetAvailable(true);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun");

            // machinegun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iMchgun], false, true /*server*/), "try switch to avail 2 server");

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol since current is mchgun");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol since current is mchgun");

            // railgun becomes available
            wm.getWeapons()[iRailgun]->SetAvailable(true);
            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('4', cWpnTarget, "cWpnTarget should change to railgun");
            b &= assertEquals(wm.getWeapons()[iRailgun], pWpnTarget, "pWpnTarget should change to railgun");

            // railgun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iRailgun], false, true /*server*/), "try switch to avail 3 server");
            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun since current is railgun");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun since current is railgun");

            // machinegun becomes empty but has unmag bullet
            wm.getWeapons()[iMchgun]->SetUnmagBulletCount(10);
            wm.getWeapons()[iMchgun]->SetMagBulletCount(0);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun due to mchgun has unmag");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun has unmag");

            // machinegun becomes empty and no unmag bullet either
            wm.getWeapons()[iMchgun]->SetUnmagBulletCount(0);
            wm.getWeapons()[iMchgun]->SetMagBulletCount(0);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol due to mchgun empty");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol due to mchgun empty");

            // pistol becomes empty
            wm.getWeapons()[iPistol]->SetUnmagBulletCount(0);
            wm.getWeapons()[iPistol]->SetMagBulletCount(0);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('4', cWpnTarget, "cWpnTarget should stay railgun becomes no better option is available");
            b &= assertEquals(wm.getWeapons()[iRailgun], pWpnTarget, "pWpnTarget should stay railgun becomes no better option is available");
        }

        return b;
    }

    bool test_wm_get_next_best_avail_weapon_with_any_ammo_not_in_keytoweaponmap()
    {
        // with any ammo means: can be either mag or unmag ammo
        constexpr bool bMustHaveMagBullet = false;

        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "asdasd.txt"}
        };

        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");

        constexpr int iMchgun = 0;
        constexpr int iPistol = 1;

        if (b)
        {
            b &= assertGreater(
                wm.getWeapons()[iMchgun]->getDamagePerSecondRating(),
                wm.getWeapons()[iPistol]->getDamagePerSecondRating(),
                "DPSR of mchgun should be greater than pistol");
        }

        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");

            // both loaded weapons become available and pistol becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            wm.getWeapons()[iMchgun]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to other weapon added to KeypressToWeaponMap is unknown weapon");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to other weapon added to KeypressToWeaponMap is unknown weapon");
        }

        return b;
    }

    bool test_wm_get_next_best_avail_weapon_with_loaded_ammo()
    {
        constexpr bool bMustHaveMagBullet = true;

        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "sample_good_wpn_automatic.txt"},
            {'4', "sample_good_wpn_railgun.txt"},
        };

        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_railgun.txt", 0), "load 3");

        constexpr int iMchgun = 0;
        constexpr int iPistol = 1;
        constexpr int iRailgun = 2;

        if (b)
        {
            b &= assertGreater(
                wm.getWeapons()[iMchgun]->getDamagePerSecondRating(),
                wm.getWeapons()[iPistol]->getDamagePerSecondRating(),
                "DPSR of mchgun should be greater than pistol");
            b &= assertGreater(
                wm.getWeapons()[iRailgun]->getDamagePerSecondRating(),
                wm.getWeapons()[iMchgun]->getDamagePerSecondRating(),
                "DPSR of railgun should be greater than mchgun");
        }

        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");

            // only pistol is available and becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to no other available");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to no other available");

            // machinegun becomes available
            wm.getWeapons()[iMchgun]->SetAvailable(true);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun");

            // machinegun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iMchgun], false, true /*server*/), "try switch to avail 2 server");

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol since current is mchgun");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol since current is mchgun");

            // railgun becomes available
            wm.getWeapons()[iRailgun]->SetAvailable(true);
            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('4', cWpnTarget, "cWpnTarget should change to railgun");
            b &= assertEquals(wm.getWeapons()[iRailgun], pWpnTarget, "pWpnTarget should change to railgun");

            // railgun becomes current
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iRailgun], false, true /*server*/), "try switch to avail 3 server");
            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('3', cWpnTarget, "cWpnTarget should change to mchgun since current is railgun");
            b &= assertEquals(wm.getWeapons()[iMchgun], pWpnTarget, "pWpnTarget should change to mchgun since current is railgun");

            // machinegun becomes empty
            wm.getWeapons()[iMchgun]->SetUnmagBulletCount(10);
            wm.getWeapons()[iMchgun]->SetMagBulletCount(0);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should change to pistol due to mchgun does not have mag ammo");
            b &= assertEquals(wm.getWeapons()[iPistol], pWpnTarget, "pWpnTarget should change to pistol due to mchgun does not have mag ammo");

            // pistol becomes empty
            wm.getWeapons()[iPistol]->SetUnmagBulletCount(10);
            wm.getWeapons()[iPistol]->SetMagBulletCount(0);

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('4', cWpnTarget, "cWpnTarget should stay railgun becomes no better option is available");
            b &= assertEquals(wm.getWeapons()[iRailgun], pWpnTarget, "pWpnTarget should stay railgun becomes no better option is available");
        }

        return b;
    }

    bool test_wm_get_next_best_avail_weapon_with_loaded_ammo_not_in_keytoweaponmap()
    {
        // with any ammo means: can be either mag or unmag ammo
        constexpr bool bMustHaveMagBullet = true;

        // Which key should switch to which weapon, this defines logical order
        WeaponManager::getKeypressToWeaponMap() = {
            {'2', "sample_good_wpn_semi_with_burst.txt"},
            {'3', "asdasd.txt"}
        };

        std::list<Bullet> bullets;
        WeaponManager wm(m_audio, cfgProfiles, *engine, bullets);
        bool b = assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_automatic.txt", 0), "load 1");
        b &= assertNotNull(wm.load("gamedata/weapons/sample_good_wpn_semi_with_burst.txt", 0), "load 2");

        constexpr int iMchgun = 0;
        constexpr int iPistol = 1;

        if (b)
        {
            b &= assertGreater(
                wm.getWeapons()[iMchgun]->getDamagePerSecondRating(),
                wm.getWeapons()[iPistol]->getDamagePerSecondRating(),
                "DPSR of mchgun should be greater than pistol");
        }

        if (b)
        {
            unsigned char cWpnTarget;
            Weapon* pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('\0', cWpnTarget, "cWpnTarget should stay null due to no current weapon");
            b &= assertNull(pWpnTarget, "pWpnTarget should be null due to no current weapon");

            // both loaded weapons become available and pistol becomes current
            wm.getWeapons()[iPistol]->SetAvailable(true);
            wm.getWeapons()[iMchgun]->SetAvailable(true);
            b &= assertTrue(wm.setCurrentWeapon(wm.getWeapons()[iPistol], false, true /*server*/), "try switch to avail 1 server");

            pWpnTarget = wm.getNextBestAvailableWeapon(cWpnTarget, bMustHaveMagBullet);
            b &= assertEquals('2', cWpnTarget, "cWpnTarget should stay as current due to other weapon added to KeypressToWeaponMap is unknown weapon");
            b &= assertEquals(wm.getCurrentWeapon(), pWpnTarget, "pWpnTarget should be current wpn due to other weapon added to KeypressToWeaponMap is unknown weapon");
        }

        return b;
    }

}; 
