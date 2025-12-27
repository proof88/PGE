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

#include <thread>

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

    virtual void initialize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(Bullet::getLoggerModuleName(), true);

        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);

        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems

        Bullet::resetGlobalBulletId();

        addSubTest("test_bullet_ctor_server_good", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_server_good);
        addSubTest("test_bullet_ctor_client_good", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_client_good);
        addSubTest("test_bullet_init_server", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_init_server);
        addSubTest("test_bullet_init_client", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_init_client);
        addSubTest("test_reset_global_bullet_id", (PFNUNITSUBTEST)&PGEBulletTest::test_reset_global_bullet_id);
        addSubTest("test_bullet_ctor_client_does_not_accept_0_weapon_id", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_client_does_not_accept_0_weapon_id);
        addSubTest("test_bullet_ctor_max_bullet_speed_incompatible_with_non_zero_bullet_drag",
            (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_max_bullet_speed_incompatible_with_non_zero_bullet_drag);
        addSubTest("test_bullet_ctor_damage_area_size_cannot_be_negative",
            (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_damage_area_size_cannot_be_negative);
        addSubTest("test_bullet_ctor_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse",
            (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_ctor_zero_damage_area_size_incompatible_with_non_zero_damage_area_pulse);
        addSubTest("test_bullet_update_updates_position_without_gravity", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_update_updates_position_without_gravity);
        addSubTest("test_bullet_update_updates_position_with_gravity", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_update_updates_position_with_gravity);
        addSubTest("test_bullet_update_updates_alpha_and_damages_when_damage_rel_distance_is_true", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_update_updates_alpha_and_damages_when_damage_rel_distance_is_true);
        addSubTest("test_bullet_expired", (PFNUNITSUBTEST)&PGEBulletTest::test_bullet_expired);
    }

    virtual bool setUp() override
    {
        return assertTrue(engine && engine->isInitialized());
    }

    virtual void tearDown() override
    {
    }

    virtual void finalize() override
    {
        Bullet::destroyReferenceObject();
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
        constexpr WeaponId iWpnId = static_cast<WeaponId>(123u);
        const PureVector posVec(1.f, 2.f, 3.f);
        const PureVector angleVec(20.f, 40.f, 60.f);
        const PureVector sizeVec(4.f, 5.f, 0.f /* size-Z will be 0.f anyway */);
        constexpr float fSpeed = 60.f;
        constexpr float fGravity = 15.f;
        constexpr float fDrag = 25.f;
        constexpr bool bFragile = true;
        constexpr float fDistMax = 10.f;
        constexpr bool bDmgRelDist = true;
        constexpr bool bBouncing = false;
        constexpr bool bHitsPlayers = true;
        constexpr int nTimer = 3;
        constexpr Bullet::ParticleType particleType = Bullet::ParticleType::None;
        constexpr int nDamageAp = 20;
        constexpr int nDamageHp = 30;
        constexpr float fDamageAreaSize = 5.f;
        constexpr Bullet::DamageAreaEffect eDamageAreaEffect = Bullet::DamageAreaEffect::Constant;
        constexpr float fDamageAreaPulse = 2.f;
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 52;

        Bullet bullet(
            iWpnId,
            *engine,
            connHandle,
            posVec.getX(), posVec.getY(), posVec.getZ(),
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            sizeVec.getX(), sizeVec.getY(), sizeVec.getZ(),
            fSpeed, fGravity, fDrag, bFragile,
            fDistMax, bDmgRelDist,
            bBouncing,
            bHitsPlayers,
            nTimer,
            particleType,
            nDamageAp, nDamageHp,
            fDamageAreaSize, eDamageAreaEffect, fDamageAreaPulse);

        bool b = assertFalse(bullet.isMarkedForDeletion(), "marked for deletion");
        b &= assertEquals(iLastBulletId, bullet.getId(), "bullet id");
        b &= assertEquals(iWpnId, bullet.getWeaponId(), "weapon id");
        b &= assertEquals(iLastBulletId + 1, Bullet::getGlobalBulletId(), "global bullet id");
        b &= assertEquals(connHandle, bullet.getOwner(), "owner");
        b &= assertEquals(posVec, bullet.getObject3D().getPosVec(), "obj pos");
        b &= assertEquals(angleVec, bullet.getObject3D().getAngleVec(), "obj angle");
        b &= assertEquals(posVec, bullet.getPut().getPosVec(), "put pos");
        b &= assertEquals(PureVector(0,1,0), bullet.getPut().getUpVec(), "put up");
        b &= assertFalse(bullet.getObject3D().isRenderingAllowed(), "visible");
        b &= assertEquals(sizeVec, bullet.getObject3D().getScaledSizeVec(), "size");
        b &= assertEquals(fSpeed, bullet.getSpeed(), "speed");
        b &= assertEquals(fGravity, bullet.getConfiguredGravity(), "configured gravity");
        b &= assertEquals(0.f, bullet.getCurrentGravity(), "current gravity");
        b &= assertEquals(fDrag, bullet.getDrag(), "drag");
        b &= assertEquals(bFragile, bullet.isFragile(), "fragile");
        b &= assertEquals(fDistMax, bullet.getTravelDistanceMax(), "fDistMax");
        b &= assertEquals(0.f, bullet.getTravelledDistance(), "travelled dist");
        b &= assertEquals(bBouncing, bullet.canBounce(), "bBouncing");
        b &= assertEquals(bHitsPlayers, bullet.hitsPlayers(), "bHitsPlayers");
        b &= assertEquals(nTimer, bullet.getTimerConfigSeconds(), "nTimer");
        b &= assertFalse(bullet.expired(), "expired");
        b &= assertEquals(particleType, bullet.getParticleType(), "particleType");
        b &= assertEquals(0, bullet.getParticleEmitPerNthPhysicsIterationCntr(), "particle emit per iter cntr");
        //b &= assertEquals(0, bullet.getParticlesEmittedTotal(), "particles emitted total cntr");
        b &= assertEquals(nDamageAp, bullet.getDamageAp(), "damageAp");
        b &= assertEquals(nDamageHp, bullet.getDamageHp(), "damageHp");
        b &= assertTrue(bullet.isDamageRelativeToDistance(), "dmgRelDist");
        b &= assertEquals(fDamageAreaSize, bullet.getAreaDamageSize(), "damage area size");
        b &= assertEquals(eDamageAreaEffect, bullet.getAreaDamageEffect(), "damage area effect");
        b &= assertEquals(fDamageAreaPulse, bullet.getAreaDamagePulse(), "damage area pulse");
        b &= assertFalse(bullet.isCreateSentToClients(), "isCreateSentToClients");

        return b;
    }

    bool test_bullet_ctor_client_good()
    {
        const Bullet::BulletId iLastBulletId = Bullet::getGlobalBulletId();
        constexpr WeaponId iWpnId = static_cast<WeaponId>(123u);
        const PureVector posVec(1.f, 2.f, 3.f);
        const PureVector angleVec(20.f, 40.f, 60.f);
        const PureVector sizeVec(4.f, 5.f, 0.f /* size-Z will be 0.f anyway */);
        constexpr float fSpeed = 60.f;
        constexpr float fGravity = 15.f;
        constexpr float fDrag = 25.f;
        constexpr float fDistMax = 10.f;
        constexpr bool bDmgRelDist = true;
        constexpr bool bBouncing = false;
        constexpr Bullet::ParticleType particleType = Bullet::ParticleType::None;
        constexpr int nDamageHp = 30;
        constexpr float fDamageAreaSize = 5.f;
        constexpr Bullet::DamageAreaEffect eDamageAreaEffect = Bullet::DamageAreaEffect::Constant;
        constexpr float fDamageAreaPulse = 2.f;

        Bullet bullet(
            iWpnId,
            *engine,
            posVec.getX(), posVec.getY(), posVec.getZ(),
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            sizeVec.getX(), sizeVec.getY(), sizeVec.getZ(),
            fSpeed, fGravity, fDrag,
            fDistMax, bDmgRelDist,
            bBouncing,
            particleType,
            nDamageHp,
            fDamageAreaSize, eDamageAreaEffect, fDamageAreaPulse);

        bool b = assertFalse(bullet.isMarkedForDeletion(), "marked for deletion");
        b &= assertEquals(iLastBulletId, bullet.getId(), "bullet id");
        b &= assertEquals(bullet.getWeaponId(), iWpnId, "weapon id");
        b &= assertEquals(Bullet::getGlobalBulletId(), iLastBulletId + 1, "global bullet id");
        b &= assertEquals(posVec, bullet.getObject3D().getPosVec(), "obj pos");
        b &= assertEquals(angleVec, bullet.getObject3D().getAngleVec(), "obj angle");
        b &= assertEquals(posVec, bullet.getPut().getPosVec(), "put pos");
        b &= assertEquals(PureVector(0, 1, 0), bullet.getPut().getUpVec(), "put up");
        b &= assertFalse(bullet.getObject3D().isRenderingAllowed(), "visible");
        b &= assertEquals(sizeVec, bullet.getObject3D().getScaledSizeVec(), "size");
        b &= assertEquals(fSpeed, bullet.getSpeed(), "speed");
        b &= assertEquals(fGravity, bullet.getConfiguredGravity(), "configured gravity");
        b &= assertEquals(0.f, bullet.getCurrentGravity(), "current gravity");
        b &= assertEquals(fDrag, bullet.getDrag(), "drag");
        b &= assertEquals(false, bullet.isFragile(), "fragile");
        b &= assertEquals(fDistMax, bullet.getTravelDistanceMax(), "fDistMax");
        b &= assertEquals(0.f, bullet.getTravelledDistance(), "travelled dist");
        b &= assertEquals(bBouncing, bullet.canBounce(), "bBouncing");
        b &= assertEquals(true, bullet.hitsPlayers(), "bHitsPlayers");
        b &= assertEquals(0, bullet.getTimerConfigSeconds(), "nTimer"); /* client is not aware about bullet timer */
        b &= assertFalse(bullet.expired(), "expired");
        b &= assertEquals(0, bullet.getParticleEmitPerNthPhysicsIterationCntr(), "particle emit cntr");
        //b &= assertEquals(0, bullet.getParticlesEmittedTotal(), "particles emitted total cntr");
        b &= assertEquals(0, bullet.getDamageAp(), "damageAp");
        b &= assertEquals(nDamageHp, bullet.getDamageHp(), "damageHp");
        b &= assertTrue(bullet.isDamageRelativeToDistance(), "dmgRelDist");
        b &= assertEquals(fDamageAreaSize, bullet.getAreaDamageSize(), "damage area size");
        b &= assertEquals(eDamageAreaEffect, bullet.getAreaDamageEffect(), "damage area effect");
        b &= assertEquals(fDamageAreaPulse, bullet.getAreaDamagePulse(), "damage area pulse");
        b &= assertFalse(bullet.isCreateSentToClients(), "isCreateSentToClients"); /* ignoredby client */

        return b;
    }

    bool test_bullet_init_server()
    {
        const Bullet::BulletId iLastBulletId = Bullet::getGlobalBulletId();
        constexpr WeaponId iWpnId = static_cast<WeaponId>(123u);
        const PureVector posVec(1.f, 2.f, 3.f);
        const PureVector angleVec(20.f, 40.f, 60.f);
        const PureVector sizeVec(4.f, 5.f, 0.f /* size-Z will be 0.f anyway */);
        constexpr float fSpeed = 60.f;
        constexpr float fGravity = 15.f;
        constexpr float fDrag = 25.f;
        constexpr bool bFragile = true;
        constexpr float fDistMax = 10.f;
        constexpr bool bDmgRelDist = true;
        constexpr bool bBouncing = false;
        constexpr bool bHitsPlayers = true;
        constexpr int nTimer = 3;
        constexpr Bullet::ParticleType particleType = Bullet::ParticleType::None;
        constexpr int nDamageAp = 20;
        constexpr int nDamageHp = 30;
        constexpr float fDamageAreaSize = 5.f;
        constexpr Bullet::DamageAreaEffect eDamageAreaEffect = Bullet::DamageAreaEffect::Constant;
        constexpr float fDamageAreaPulse = 2.f;
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 52;

        // intentionally initialize with different values
        Bullet bullet(
            iWpnId,
            *engine,
            connHandle,
            0.f, 0.f, 0.f, /* pos xyz */
            0.f, 0.f, 0.f, /* angle xyz */
            true /* visible */,
            1.f, 1.f, 1.f /* size xyz */,
            fSpeed + 5.f, fGravity + 5.f, fDrag + 5.f, !bFragile,
            fDistMax + 5.f, !bDmgRelDist,
            !bBouncing,
            !bHitsPlayers,
            nTimer + 6,
            Bullet::ParticleType::Smoke,
            nDamageAp + 6, nDamageHp + 6,
            fDamageAreaSize + 5.f, Bullet::DamageAreaEffect::Linear, fDamageAreaPulse + 5.f);

        // try to alter as many values as we can
        bullet.update(30u, 1.f, -5.f);
        bullet.getParticleEmitPerNthPhysicsIterationCntr() = 5;
        bullet.isCreateSentToClients() = true;
        bullet.markForDeletion();

        bullet.init(
            iWpnId,
            *engine,
            connHandle,
            posVec.getX(), posVec.getY(), posVec.getZ(),
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            sizeVec.getX(), sizeVec.getY(), sizeVec.getZ(),
            fSpeed, fGravity, fDrag, bFragile,
            fDistMax, bDmgRelDist,
            bBouncing,
            bHitsPlayers,
            nTimer,
            particleType,
            nDamageAp, nDamageHp,
            fDamageAreaSize, eDamageAreaEffect, fDamageAreaPulse);

        bool b = assertFalse(bullet.isMarkedForDeletion(), "marked for deletion");
        b &= assertEquals(iLastBulletId, bullet.getId(), "bullet id");
        b &= assertEquals(iWpnId, bullet.getWeaponId(), "weapon id");
        b &= assertEquals(iLastBulletId + 1, Bullet::getGlobalBulletId(), "global bullet id");
        b &= assertEquals(connHandle, bullet.getOwner(), "owner");
        b &= assertEquals(posVec, bullet.getObject3D().getPosVec(), "obj pos");
        b &= assertEquals(angleVec, bullet.getObject3D().getAngleVec(), "obj angle");
        b &= assertEquals(posVec, bullet.getPut().getPosVec(), "put pos");
        b &= assertEquals(PureVector(0, 1, 0), bullet.getPut().getUpVec(), "put up");
        b &= assertFalse(bullet.getObject3D().isRenderingAllowed(), "visible");
        b &= assertEquals(sizeVec, bullet.getObject3D().getScaledSizeVec(), "size");
        b &= assertEquals(fSpeed, bullet.getSpeed(), "speed");
        b &= assertEquals(fGravity, bullet.getConfiguredGravity(), "configured gravity");
        b &= assertEquals(0.f, bullet.getCurrentGravity(), "current gravity");
        b &= assertEquals(fDrag, bullet.getDrag(), "drag");
        b &= assertEquals(bFragile, bullet.isFragile(), "fragile");
        b &= assertEquals(fDistMax, bullet.getTravelDistanceMax(), "fDistMax");
        b &= assertEquals(0.f, bullet.getTravelledDistance(), "travelled dist");
        b &= assertEquals(bBouncing, bullet.canBounce(), "bBouncing");
        b &= assertEquals(bHitsPlayers, bullet.hitsPlayers(), "bHitsPlayers");
        b &= assertEquals(nTimer, bullet.getTimerConfigSeconds(), "nTimer");
        b &= assertFalse(bullet.expired(), "expired");
        b &= assertEquals(particleType, bullet.getParticleType(), "particleType");
        b &= assertEquals(0, bullet.getParticleEmitPerNthPhysicsIterationCntr(), "particle emit per iter cntr");
        //b &= assertEquals(0, bullet.getParticlesEmittedTotal(), "particles emitted total cntr");
        b &= assertEquals(nDamageAp, bullet.getDamageAp(), "damageAp");
        b &= assertEquals(nDamageHp, bullet.getDamageHp(), "damageHp");
        b &= assertTrue(bullet.isDamageRelativeToDistance(), "dmgRelDist");
        b &= assertEquals(fDamageAreaSize, bullet.getAreaDamageSize(), "damage area size");
        b &= assertEquals(eDamageAreaEffect, bullet.getAreaDamageEffect(), "damage area effect");
        b &= assertEquals(fDamageAreaPulse, bullet.getAreaDamagePulse(), "damage area pulse");
        b &= assertFalse(bullet.isCreateSentToClients(), "isCreateSentToClients");

        return b;
    }

    bool test_bullet_init_client()
    {
        const Bullet::BulletId iLastBulletId = Bullet::getGlobalBulletId();
        constexpr Bullet::BulletId iBulletId = static_cast<WeaponId>(256u);
        constexpr WeaponId iWpnId = static_cast<WeaponId>(123u);
        const PureVector posVec(1.f, 2.f, 3.f);
        const PureVector angleVec(20.f, 40.f, 60.f);
        const PureVector sizeVec(4.f, 5.f, 0.f /* size-Z will be 0.f anyway */);
        constexpr float fSpeed = 60.f;
        constexpr float fGravity = 15.f;
        constexpr float fDrag = 25.f;
        constexpr bool bFragile = true;
        constexpr float fDistMax = 10.f;
        constexpr bool bDmgRelDist = true;
        constexpr bool bBouncing = false;
        constexpr bool bHitsPlayers = true;
        constexpr int nTimer = 3;
        constexpr Bullet::ParticleType particleType = Bullet::ParticleType::None;
        constexpr int nDamageAp = 20;
        constexpr int nDamageHp = 30;
        constexpr float fDamageAreaSize = 5.f;
        constexpr Bullet::DamageAreaEffect eDamageAreaEffect = Bullet::DamageAreaEffect::Constant;
        constexpr float fDamageAreaPulse = 2.f;
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 52;

        // intentionally initialize with different values
        Bullet bullet(
            iWpnId,
            *engine,
            connHandle,
            0.f, 0.f, 0.f, /* pos xyz */
            0.f, 0.f, 0.f, /* angle xyz */
            true /* visible */,
            1.f, 1.f, 1.f /* size xyz */,
            fSpeed + 5.f, fGravity + 5.f, fDrag + 5.f, !bFragile,
            fDistMax + 5.f, !bDmgRelDist,
            !bBouncing,
            !bHitsPlayers,
            nTimer + 6,
            Bullet::ParticleType::Smoke,
            nDamageAp + 6, nDamageHp + 6,
            fDamageAreaSize + 5.f, Bullet::DamageAreaEffect::Linear, fDamageAreaPulse + 5.f);

        // try to alter as many values as we can
        bullet.update(30u, 1.f, -5.f);
        bullet.getParticleEmitPerNthPhysicsIterationCntr() = 5;
        bullet.isCreateSentToClients() = true;
        bullet.markForDeletion();

        bullet.init(
            iBulletId,
            iWpnId,
            *engine,
            posVec.getX(), posVec.getY(), posVec.getZ(),
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            sizeVec.getX(), sizeVec.getY(), sizeVec.getZ(),
            fSpeed, fGravity, fDrag,
            fDistMax, bDmgRelDist,
            bBouncing,
            particleType,
            nDamageHp,
            fDamageAreaSize, eDamageAreaEffect, fDamageAreaPulse);

        bool b = assertFalse(bullet.isMarkedForDeletion(), "marked for deletion");
        b &= assertEquals(iBulletId /* client-side init receives this from server */, bullet.getId(), "bullet id");
        b &= assertEquals(iWpnId, bullet.getWeaponId(), "weapon id");
        b &= assertEquals(iLastBulletId + 1, Bullet::getGlobalBulletId(), "global bullet id");
        b &= assertEquals(0u /* client doesnt care */, bullet.getOwner(), "owner");
        b &= assertEquals(posVec, bullet.getObject3D().getPosVec(), "obj pos");
        b &= assertEquals(angleVec, bullet.getObject3D().getAngleVec(), "obj angle");
        b &= assertEquals(posVec, bullet.getPut().getPosVec(), "put pos");
        b &= assertEquals(PureVector(0, 1, 0), bullet.getPut().getUpVec(), "put up");
        b &= assertFalse(bullet.getObject3D().isRenderingAllowed(), "visible");
        b &= assertEquals(sizeVec, bullet.getObject3D().getScaledSizeVec(), "size");
        b &= assertEquals(fSpeed, bullet.getSpeed(), "speed");
        b &= assertEquals(fGravity, bullet.getConfiguredGravity(), "configured gravity");
        b &= assertEquals(0.f, bullet.getCurrentGravity(), "current gravity");
        b &= assertEquals(fDrag, bullet.getDrag(), "drag");
        b &= assertEquals(false /* client doesnt care */, bullet.isFragile(), "fragile");
        b &= assertEquals(fDistMax, bullet.getTravelDistanceMax(), "fDistMax");
        b &= assertEquals(0.f, bullet.getTravelledDistance(), "travelled dist");
        b &= assertEquals(bBouncing, bullet.canBounce(), "bBouncing");
        b &= assertEquals(true /* client doesnt care */, bullet.hitsPlayers(), "bHitsPlayers");
        b &= assertEquals(0 /* client doesnt care */, bullet.getTimerConfigSeconds(), "nTimer");
        b &= assertFalse(bullet.expired(), "expired");
        b &= assertEquals(particleType, bullet.getParticleType(), "particleType");
        b &= assertEquals(0, bullet.getParticleEmitPerNthPhysicsIterationCntr(), "particle emit per iter cntr");
        //b &= assertEquals(0, bullet.getParticlesEmittedTotal(), "particles emitted total cntr");
        b &= assertEquals(0 /* client doesnt care */, bullet.getDamageAp(), "damageAp");
        b &= assertEquals(nDamageHp, bullet.getDamageHp(), "damageHp");
        b &= assertTrue(bullet.isDamageRelativeToDistance(), "dmgRelDist");
        b &= assertEquals(fDamageAreaSize, bullet.getAreaDamageSize(), "damage area size");
        b &= assertEquals(eDamageAreaEffect, bullet.getAreaDamageEffect(), "damage area effect");
        b &= assertEquals(fDamageAreaPulse, bullet.getAreaDamagePulse(), "damage area pulse");
        b &= assertFalse(bullet.isCreateSentToClients(), "isCreateSentToClients");

        return b;
    }

    bool test_reset_global_bullet_id()
    {
        constexpr WeaponId iWpnId = static_cast<WeaponId>(123u);
        const PureVector posVec(1.f, 2.f, 3.f);
        const PureVector angleVec(20.f, 40.f, 60.f);
        const PureVector sizeVec(4.f, 5.f, 0.f /* size-Z will be 0.f anyway */);
        constexpr float fSpeed = 60.f;
        constexpr float fGravity = 15.f;
        constexpr float fDrag = 25.f;
        constexpr Bullet::ParticleType particleType = Bullet::ParticleType::None;
        constexpr bool bFragile = true;
        constexpr float fDistMax = 10.f;
        constexpr bool bDmgRelDist = false;
        constexpr bool bBouncing = false;
        constexpr bool bHitsPlayers = true;
        constexpr int nTimer = 0;
        constexpr int nDamageAp = 20;
        constexpr int nDamageHp = 30;
        constexpr float fDamageAreaSize = 5.f;
        constexpr Bullet::DamageAreaEffect eDamageAreaEffect = Bullet::DamageAreaEffect::Constant;
        constexpr float fDamageAreaPulse = 2.f;
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 52;

        Bullet bullet(
            iWpnId,
            *engine,
            connHandle,
            posVec.getX(), posVec.getY(), posVec.getZ(),
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            sizeVec.getX(), sizeVec.getY(), sizeVec.getZ(),
            fSpeed, fGravity, fDrag, bFragile,
            fDistMax, bDmgRelDist,
            bBouncing,
            bHitsPlayers,
            nTimer,
            particleType,
            nDamageAp, nDamageHp,
            fDamageAreaSize, eDamageAreaEffect, fDamageAreaPulse);

        Bullet::resetGlobalBulletId();

        return assertEquals(static_cast<Bullet::BulletId>(0), Bullet::getGlobalBulletId());
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
                0.f /* fDistMax */, false /* bDmgRelDist */,
                false /* bouncing */,
                Bullet::ParticleType::None,
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
                1000.f /* speed */, 15.f /* gravity */, 25.f /* drag */, true /* fragile */,
                0.f /* fDistMax */, false /* bDmgRelDist */,
                false /* bouncing */,
                true /* hitsPlayer */,
                0 /* timer */,
                Bullet::ParticleType::None,
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
                60.f /* speed */, 15.f /* gravity */, 25.f /* drag */, true /* fragile */,
                0.f /* fDistMax */, false /* bDmgRelDist */,
                false /* bouncing */,
                true /* hitsPlayer */,
                0 /* timer */,
                Bullet::ParticleType::None,
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
                60.f /* speed */, 15.f /* gravity */, 25.f /* drag */, true /* fragile */,
                0.f /* fDistMax */, false /* bDmgRelDist */,
                false /* bouncing */,
                true /* hitsPlayer */,
                0 /* timer */,
                Bullet::ParticleType::None,
                5 /* AP */, 10 /* HP */,
                0.f, Bullet::DamageAreaEffect::Constant, 2.f);
        }
        catch (const std::exception&)
        {
            b = true;
        }

        return b;
    }

    bool test_bullet_update_updates_position_without_gravity()
    {
        const PureVector angleVec(0.f, 90.f, 45.f);
        constexpr float fSpeed = 60.f;
        constexpr int nFactor = 1;
        constexpr float fConfiguredBulletGravity = 0.f;
        constexpr int nDamageAp = 20;
        constexpr int nDamageHp = 30;

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
            fSpeed, fConfiguredBulletGravity, 25.f, true,
            100.f /* fDistMax */, false /* bDmgRelDist */,
            false /* bouncing */,
            true /* hitsPlayer */,
            0 /* timer */,
            Bullet::ParticleType::None,
            nDamageAp, nDamageHp,
            5.f, Bullet::DamageAreaEffect::Constant, 2.f);
        bullet.update(nFactor, 0.f, 0.f);

        bool b = assertEquals(put.getPosVec(), bullet.getObject3D().getPosVec(), "pos");
        b &= assertEquals(fSpeed / static_cast<float>(nFactor), bullet.getTravelledDistance(), "travelled distance");
        
        // isDamageRelativeToDistance() is false, so these shall stay unchanged even though there is configured maximum travel distance
        b &= assertEquals(1.f, bullet.getObject3D().getMaterial(false).getTextureEnvColor().getAlphaAsFloat(), "alpha no change");
        b &= assertEquals(nDamageAp, bullet.getDamageAp(), "damageAp no change");
        b &= assertEquals(nDamageHp, bullet.getDamageHp(), "damageHp no change");

        return b;
    }

    bool test_bullet_update_updates_position_with_gravity()
    {
        const PureVector angleVec(0.f, 90.f, 45.f);
        constexpr float fSpeed = 60.f;
        constexpr int nFactor = 1;
        constexpr int nDamageAp = 20;
        constexpr int nDamageHp = 30;
        constexpr float fConfiguredBulletGravity = 0.7f;
        constexpr float fGravityChangePerTick = -6.f;
        constexpr float fGravityMin = -8.f;
        float fGravityCurrent = 0.f;

        PurePosUpTarget put;
        put.SetRotation(angleVec.getX(), angleVec.getY(), angleVec.getZ());
        put.Move(fSpeed / nFactor);
        fGravityCurrent = std::max(fGravityCurrent + fGravityChangePerTick * fConfiguredBulletGravity, fGravityMin);
        put.Elevate(fGravityCurrent);

        Bullet bullet(
            static_cast<WeaponId>(123u),
            *engine,
            0, 0.f, 0.f, 0.f,
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            1.f, 1.f, 1.f,
            fSpeed, fConfiguredBulletGravity, 25.f, true,
            0.f /* fDistMax */, false /* bDmgRelDist */,
            false /* bouncing */,
            true /* hitsPlayer */,
            0 /* timer */,
            Bullet::ParticleType::None,
            nDamageAp, nDamageHp,
            5.f, Bullet::DamageAreaEffect::Constant, 2.f);

        bullet.update(nFactor, fGravityChangePerTick, fGravityMin);
        bool b = assertEquals(put.getPosVec(), bullet.getObject3D().getPosVec(), "pos 1");
        b &= assertEquals(fSpeed / static_cast<float>(nFactor), bullet.getTravelledDistance(), "travelled distance 1");
        b &= assertEquals(fGravityCurrent, bullet.getCurrentGravity(), "gravity 1");

        // isDamageRelativeToDistance() is false, so these shall stay unchanged even though there is configured maximum travel distance
        b &= assertEquals(1.f, bullet.getObject3D().getMaterial(false).getTextureEnvColor().getAlphaAsFloat(), "alpha no change");
        b &= assertEquals(nDamageAp, bullet.getDamageAp(), "damageAp no change");
        b &= assertEquals(nDamageHp, bullet.getDamageHp(), "damageHp no change");

        put.Move(fSpeed / nFactor);
        fGravityCurrent = std::max(fGravityCurrent + fGravityChangePerTick * fConfiguredBulletGravity, fGravityMin);
        put.Elevate(fGravityCurrent);
        bullet.update(nFactor, fGravityChangePerTick, fGravityMin);
        b &= assertEquals(put.getPosVec(), bullet.getObject3D().getPosVec(), "pos 2");
        b &= assertEquals(fGravityCurrent, bullet.getCurrentGravity(), "gravity 2");

        // isDamageRelativeToDistance() is false, so these shall stay unchanged even though there is configured maximum travel distance
        b &= assertEquals(1.f, bullet.getObject3D().getMaterial(false).getTextureEnvColor().getAlphaAsFloat(), "alpha 2 no change");
        b &= assertEquals(nDamageAp, bullet.getDamageAp(), "damageAp 2 no change");
        b &= assertEquals(nDamageHp, bullet.getDamageHp(), "damageHp 2 no change");

        return b;
    }

    bool test_bullet_update_updates_alpha_and_damages_when_damage_rel_distance_is_true()
    {
        const PureVector angleVec(0.f, 90.f, 45.f);
        constexpr float fSpeed = 60.f;
        constexpr int nFactor = 1;
        constexpr int nDamageAp = 20;
        constexpr int nDamageHp = 30;
        constexpr float fConfiguredBulletGravity = 0.f;
        constexpr float fMaxDistance = 100.f;

        Bullet bullet(
            static_cast<WeaponId>(123u),
            *engine,
            0, 0.f, 0.f, 0.f,
            angleVec.getX(), angleVec.getY(), angleVec.getZ(),
            false /* visible */,
            1.f, 1.f, 1.f,
            fSpeed, fConfiguredBulletGravity, 25.f, true,
            fMaxDistance, true /* bDmgRelDist */,
            false /* bouncing */,
            true /* hitsPlayer */,
            0 /* timer */,
            Bullet::ParticleType::None,
            nDamageAp, nDamageHp,
            5.f, Bullet::DamageAreaEffect::Constant, 2.f);
        bullet.update(nFactor, 0.f, 0.f);

        const float fTravelDistanceFactor = 1.f - bullet.getTravelledDistance() / fMaxDistance;

        bool b = true;
        b &= assertNotEquals(1.f, bullet.getObject3D().getMaterial(false).getTextureEnvColor().getAlphaAsFloat(), "alpha a 1");
        b &= assertEquals(
            fTravelDistanceFactor,
            bullet.getObject3D().getMaterial(false).getTextureEnvColor().getAlphaAsFloat(), 0.01f, "alpha b 1");

        b &= assertEquals(nDamageAp * fTravelDistanceFactor, static_cast<float>(bullet.getDamageAp()), 0.01f, "damageAp 1");
        b &= assertEquals(nDamageHp * fTravelDistanceFactor, static_cast<float>(bullet.getDamageHp()), 0.01f, "damageHp 1");

        // with another update() bullet reaches max travel distance
        bullet.update(nFactor, 0.f, 0.f);

        b &= assertEquals(bullet.getTravelDistanceMax(), bullet.getTravelledDistance(), 0.01f, "travel dist");
        b &= assertEquals(
            0.f,
            bullet.getObject3D().getMaterial(false).getTextureEnvColor().getAlphaAsFloat(), 0.01f, "alpha b 1");

        b &= assertEquals(0.f, static_cast<float>(bullet.getDamageAp()), 0.01f, "damageAp 1");
        b &= assertEquals(0.f, static_cast<float>(bullet.getDamageHp()), 0.01f, "damageHp 1");

        return b;
    }

    bool test_bullet_expired()
    {
        constexpr int nTimerSeconds = 2;
        Bullet bullet(
            static_cast<WeaponId>(123u),
            *engine,
            0 /* connHandle */,
            1.f, 2.f, 3.f,
            20.f, 40.f, 60.f,
            false /* visible */,
            4.f, 5.f, 0.f,
            1000.f /* speed */, 15.f /* gravity */, 0.f /* drag */, true /* fragile */,
            0.f /* fDistMax */, false /* bDmgRelDist */,
            false /* bouncing */,
            true /* hitsPlayer */,
            nTimerSeconds,
            Bullet::ParticleType::None,
            5 /* AP */, 10 /* HP */,
            5.f, Bullet::DamageAreaEffect::Constant, 2.f);

        bool b = true;

        b &= assertFalse(bullet.expired(), "expired 1");
        std::this_thread::sleep_for(std::chrono::seconds(nTimerSeconds));
        b &= assertTrue(bullet.expired(), "expired 2");

        // server version of init() is supposed to update the timestamp of the Bullet
        bullet.init(
            static_cast<WeaponId>(123u),
            *engine,
            0 /* connHandle */,
            1.f, 2.f, 3.f,
            20.f, 40.f, 60.f,
            false /* visible */,
            4.f, 5.f, 0.f,
            1000.f /* speed */, 15.f /* gravity */, 0.f /* drag */, true /* fragile */,
            0.f /* fDistMax */, false /* bDmgRelDist */,
            false /* bouncing */,
            true /* hitsPlayer */,
            2 * nTimerSeconds,
            Bullet::ParticleType::None,
            5 /* AP */, 10 /* HP */,
            5.f, Bullet::DamageAreaEffect::Constant, 2.f);

        b &= assertFalse(bullet.expired(), "expired 3");
        std::this_thread::sleep_for(std::chrono::seconds(nTimerSeconds));
        b &= assertFalse(bullet.expired(), "expired 4");
        std::this_thread::sleep_for(std::chrono::seconds(nTimerSeconds));
        b &= assertTrue(bullet.expired(), "expired 5");

        // disable timer
        bullet.init(
            static_cast<WeaponId>(123u),
            *engine,
            0 /* connHandle */,
            1.f, 2.f, 3.f,
            20.f, 40.f, 60.f,
            false /* visible */,
            4.f, 5.f, 0.f,
            1000.f /* speed */, 15.f /* gravity */, 0.f /* drag */, true /* fragile */,
            0.f /* fDistMax */, false /* bDmgRelDist */,
            false /* bouncing */,
            true /* hitsPlayer */,
            0,
            Bullet::ParticleType::None,
            5 /* AP */, 10 /* HP */,
            5.f, Bullet::DamageAreaEffect::Constant, 2.f);
        b &= assertFalse(bullet.expired(), "expired 6");

        return b;
    }
};