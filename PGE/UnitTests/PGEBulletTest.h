#pragma once

/*
    ###################################################################################
    PGEBulletTest.h
    Unit test for PGEBullet.
    Made by PR00F88, West Whiskhyll Entertainment
    2022
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Weapons/WeaponManager.h"

class PGEBulletTest :
    public UnitTest
{
public:

    PGEBulletTest() :
        UnitTest(__FILE__)
    {
        engine = NULL;
    }

protected:

    virtual void Initialize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(Bullet::getLoggerModuleName(), true);

        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);

        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems

        AddSubTest("test_bullet_ctor_server_good", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_server_good);
        AddSubTest("test_bullet_ctor_client_good", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_client_good);
        // TODO: add test for copy ctor and assignment operator
        AddSubTest("test_reset_global_bullet_id", (PFNUNITSUBTEST)&PGEBulletTest::test_reset_global_bullet_id);
        AddSubTest("test_bullet_ctor_server_does_not_accept_0_weapon_id", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_server_does_not_accept_0_weapon_id);
        AddSubTest("test_bullet_ctor_client_does_not_accept_0_weapon_id", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_client_does_not_accept_0_weapon_id);
        AddSubTest("test_bullet_ctor_max_bullet_speed_incompatible_with_non_zero_bullet_drag",
            (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_max_bullet_speed_incompatible_with_non_zero_bullet_drag);
        AddSubTest("test_bullet_ctor_damage_area_size_cannot_be_negative",
            (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_damage_area_size_cannot_be_negative);
        AddSubTest("test_bullet_ctor_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse",
            (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse);
        AddSubTest("test_bullet_update_updates_position", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_update_updates_position);
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
        if (engine)
        {
            engine->shutdown();
            engine = NULL;
        }

        CConsole::getConsoleInstance().SetLoggingState(Bullet::getLoggerModuleName(), false);
    }

private:

    PR00FsUltimateRenderingEngine* engine;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PGEBulletTest(const PGEBulletTest&)
    {};

    PGEBulletTest& operator=(const PGEBulletTest&)
    {
        return *this;
    };

    bool test_bullet_ctor_server_good()
    {
        const Bullet::BulletId iLastBulletId = Bullet::getGlobalBulletId();
        const WeaponId iWpnId = static_cast<WeaponId>(123u);
        const PureVector posVec(1.f, 2.f, 3.f);
        const PureVector angleVec(20.f, 40.f, 60.f);
        const PureVector sizeVec(4.f, 5.f, 0.f /* size-Z will be 0.f anyway */);
        const float fSpeed = 60.f;
        const float fGravity = 15.f;
        const float fDrag = 25.f;
        const bool bFragile = true;
        const float fDistMax = 10.f;
        const int nDamageAp = 20;
        const int nDamageHp = 30;
        const float fDamageAreaSize = 5.f;
        const Bullet::DamageAreaEffect eDamageAreaEffect = Bullet::DamageAreaEffect::Constant;
        const float fDamageAreaPulse = 2.f;
        const pge_network::PgeNetworkConnectionHandle connHandle = 52;

        Bullet bullet(
            iWpnId,
            *engine,
            connHandle,
            posVec.getX(), posVec.getY(), posVec.getZ(),
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            sizeVec.getX(), sizeVec.getY(), sizeVec.getZ(),
            fSpeed, fGravity, fDrag, bFragile,
            fDistMax,
            nDamageAp, nDamageHp,
            fDamageAreaSize, eDamageAreaEffect, fDamageAreaPulse);
        
        bool b = assertEquals(bullet.getId(), iLastBulletId, "bullet id");
        b &= assertEquals(bullet.getWeaponId(), iWpnId, "weapon id");
        b &= assertEquals(Bullet::getGlobalBulletId(), iLastBulletId + 1, "global bullet id");
        b &= assertEquals(connHandle, bullet.getOwner(), "owner");
        b &= assertEquals(posVec, bullet.getObject3D().getPosVec(), "pos");
        b &= assertEquals(angleVec, bullet.getObject3D().getAngleVec(), "angle");
        b &= assertFalse(bullet.getObject3D().isRenderingAllowed(), "visible");
        b &= assertEquals(sizeVec, bullet.getObject3D().getScaledSizeVec(), "size");
        b &= assertEquals(fSpeed, bullet.getSpeed(), "speed");
        b &= assertEquals(fGravity, bullet.getGravity(), "gravity");
        b &= assertEquals(fDrag, bullet.getDrag(), "drag");
        b &= assertEquals(bFragile, bullet.isFragile(), "fragile");
        b &= assertEquals(fDistMax, bullet.getTravelDistanceMax(), "fDistMax");
        b &= assertEquals(nDamageAp, bullet.getDamageAp(), "damageAp");
        b &= assertEquals(nDamageHp, bullet.getDamageHp(), "damageHp");
        b &= assertEquals(fDamageAreaSize, bullet.getAreaDamageSize(), "damage area size");
        b &= assertEquals(eDamageAreaEffect, bullet.getAreaDamageEffect(), "damage area effect");
        b &= assertEquals(fDamageAreaPulse, bullet.getAreaDamagePulse(), "damage area pulse");
        b &= assertFalse(bullet.isCreateSentToClients(), "isCreateSentToClients");

        return b;
    }

    bool test_bullet_ctor_client_good()
    {
        const Bullet::BulletId iLastBulletId = Bullet::getGlobalBulletId();
        const WeaponId iWpnId = static_cast<WeaponId>(123u);
        const PureVector posVec(1.f, 2.f, 3.f);
        const PureVector angleVec(20.f, 40.f, 60.f);
        const PureVector sizeVec(4.f, 5.f, 0.f /* size-Z will be 0.f anyway */);
        const float fSpeed = 60.f;
        const float fGravity = 15.f;
        const float fDrag = 25.f;
        const int nDamageHp = 30;
        const float fDamageAreaSize = 5.f;
        const Bullet::DamageAreaEffect eDamageAreaEffect = Bullet::DamageAreaEffect::Constant;
        const float fDamageAreaPulse = 2.f;

        Bullet bullet(
            iWpnId,
            *engine,
            posVec.getX(), posVec.getY(), posVec.getZ(),
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            sizeVec.getX(), sizeVec.getY(), sizeVec.getZ(),
            fSpeed, fGravity, fDrag, nDamageHp,
            fDamageAreaSize, eDamageAreaEffect, fDamageAreaPulse);

        bool b = assertEquals(bullet.getId(), iLastBulletId, "bullet id");
        b &= assertEquals(bullet.getWeaponId(), iWpnId, "weapon id");
        b &= assertEquals(Bullet::getGlobalBulletId(), iLastBulletId + 1, "global bullet id");
        b &= assertEquals(posVec, bullet.getObject3D().getPosVec(), "pos");
        b &= assertEquals(angleVec, bullet.getObject3D().getAngleVec(), "angle");
        b &= assertFalse(bullet.getObject3D().isRenderingAllowed(), "visible");
        b &= assertEquals(sizeVec, bullet.getObject3D().getScaledSizeVec(), "size");
        b &= assertEquals(fSpeed, bullet.getSpeed(), "speed");
        b &= assertEquals(fGravity, bullet.getGravity(), "gravity");
        b &= assertEquals(fDrag, bullet.getDrag(), "drag");
        b &= assertEquals(false, bullet.isFragile(), "fragile");
        b &= assertEquals(0.f, bullet.getTravelDistanceMax(), "fDistMax");
        b &= assertEquals(0, bullet.getDamageAp(), "damageAp");
        b &= assertEquals(nDamageHp, bullet.getDamageHp(), "damageHp");
        b &= assertEquals(fDamageAreaSize, bullet.getAreaDamageSize(), "damage area size");
        b &= assertEquals(eDamageAreaEffect, bullet.getAreaDamageEffect(), "damage area effect");
        b &= assertEquals(fDamageAreaPulse, bullet.getAreaDamagePulse(), "damage area pulse");
        b &= assertFalse(bullet.isCreateSentToClients(), "isCreateSentToClients"); /* ignoredby client */

        return b;
    }

    bool test_reset_global_bullet_id()
    {
        const WeaponId iWpnId = static_cast<WeaponId>(123u);
        const PureVector posVec(1.f, 2.f, 3.f);
        const PureVector angleVec(20.f, 40.f, 60.f);
        const PureVector sizeVec(4.f, 5.f, 0.f /* size-Z will be 0.f anyway */);
        const float fSpeed = 60.f;
        const float fGravity = 15.f;
        const float fDrag = 25.f;
        const bool bFragile = true;
        const float fDistMax = 10.f;
        const int nDamageAp = 20;
        const int nDamageHp = 30;
        const float fDamageAreaSize = 5.f;
        const Bullet::DamageAreaEffect eDamageAreaEffect = Bullet::DamageAreaEffect::Constant;
        const float fDamageAreaPulse = 2.f;
        const pge_network::PgeNetworkConnectionHandle connHandle = 52;

        Bullet bullet(
            iWpnId,
            *engine,
            connHandle,
            posVec.getX(), posVec.getY(), posVec.getZ(),
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            sizeVec.getX(), sizeVec.getY(), sizeVec.getZ(),
            fSpeed, fGravity, fDrag, bFragile,
            fDistMax,
            nDamageAp, nDamageHp,
            fDamageAreaSize, eDamageAreaEffect, fDamageAreaPulse);

        Bullet::resetGlobalBulletId();

        return assertEquals(static_cast<Bullet::BulletId>(0), Bullet::getGlobalBulletId());
    }

    bool test_bullet_ctor_server_does_not_accept_0_weapon_id()
    {
        bool b = false;
        try
        {
            Bullet bullet(
                static_cast<WeaponId>(0u),
                *engine,
                0 /* connHandle */,
                1.f, 2.f, 3.f,
                20.f, 40.f, 60.f,
                false /* visible */,
                4.f, 5.f, 0.f,
                1000.f, 15.f, 25.f, true,
                0.f /* fDistMax */,
                5 /* AP */, 10 /* HP */,
                5.f, Bullet::DamageAreaEffect::Constant, 2.f);
        }
        catch (const std::exception&)
        {
            b = true;
        }

        return b;
    }

    bool test_bullet_ctor_client_does_not_accept_0_weapon_id()
    {
        bool b = false;
        try
        {
            Bullet bullet(
                static_cast<WeaponId>(0u),
                *engine,
                1.f, 2.f, 3.f,
                20.f, 40.f, 60.f,
                false /* visible */,
                4.f, 5.f, 0.f,
                1000.f, 15.f, 25.f, /* client does not receive nor use fragile */
                /* client does not receive nor use fDistMax */
                /* client does not receive nor use nDamageAp */
                10 /* HP */,
                5.f, Bullet::DamageAreaEffect::Constant, 2.f);
        }
        catch (const std::exception&)
        {
            b = true;
        }

        return b;
    }

    bool test_bullet_ctor_max_bullet_speed_incompatible_with_non_zero_bullet_drag()
    {
        bool b = false;
        try
        {
            Bullet bullet(
                static_cast<WeaponId>(123u),
                *engine,
                0,
                1.f, 2.f, 3.f,
                20.f, 40.f, 60.f,
                false /* visible */,
                4.f, 5.f, 0.f,
                1000.f, 15.f, 25.f, true,
                0.f /* fDistMax */,
                5 /* AP */, 10 /* HP */,
                5.f, Bullet::DamageAreaEffect::Constant, 2.f);
        }
        catch (const std::exception&)
        {
            b = true;
        }

        return b;
    }

    bool test_bullet_ctor_damage_area_size_cannot_be_negative()
    {
        bool b = false;
        try
        {
            Bullet bullet(
                static_cast<WeaponId>(123u),
                *engine,
                0,
                1.f, 2.f, 3.f,
                20.f, 40.f, 60.f,
                false /* visible */,
                4.f, 5.f, 0.f,
                60.f, 15.f, 25.f, true,
                0.f /* fDistMax */,
                5 /* AP */, 10 /* HP */,
                -5.f, Bullet::DamageAreaEffect::Constant, 2.f);
        }
        catch (const std::exception&)
        {
            b = true;
        }

        return b;
    }

    bool test_bullet_ctor_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse()
    {
        bool b = false;
        try
        {
            Bullet bullet(
                static_cast<WeaponId>(123u),
                *engine,
                0,
                1.f, 2.f, 3.f,
                20.f, 40.f, 60.f,
                false /* visible */,
                4.f, 5.f, 0.f,
                60.f, 15.f, 25.f, true,
                0.f /* fDistMax */,
                5 /* AP */, 10 /* HP */,
                0.f, Bullet::DamageAreaEffect::Constant, 2.f);
        }
        catch (const std::exception&)
        {
            b = true;
        }

        return b;
    }

    bool test_bullet_update_updates_position()
    {
        const PureVector angleVec(0.f, 90.f, 45.f);
        constexpr float fSpeed = 60.f;
        constexpr int nFactor = 1;

        PurePosUpTarget put;
        put.SetRotation(angleVec.getX(), angleVec.getY(), angleVec.getZ());
        put.Move(fSpeed / static_cast<float>(nFactor));

        Bullet bullet(
            static_cast<WeaponId>(123u),
            *engine,
            0, 0.f, 0.f, 0.f,
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            1.f, 1.f, 1.f,
            fSpeed, 15.f, 25.f, true,
            0.f /* fDistMax */,
            5 /* AP */, 10 /* HP */,
            5.f, Bullet::DamageAreaEffect::Constant, 2.f);
        bullet.Update(nFactor);

        bool b = assertEquals(put.getPosVec(), bullet.getObject3D().getPosVec(), "pos");
        b &= assertEquals(fSpeed / static_cast<float>(nFactor), bullet.getTravelledDistance(), "travelled distance");

        return b;
    }
};