#pragma once

/*
    ###################################################################################
    WeaponManager.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine Weapon Manager
    Made by PR00F88
    ###################################################################################
*/

#include <chrono> // requires cpp11
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../PGEallHeaders.h"
#include "../Audio/PgeAudio.h"
#include "../Config/PGEcfgFile.h"
#include "../Config/PGEcfgProfiles.h"
#include "../Config/PgeOldNewValue.h"
#include "../Memory/PgeObjectPool.h"
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"
#include "../Network/PgePacket.h"

typedef PFL::StringHash WeaponId;

/**
    Bullet class for PR00F's Game Engine Weapon Manager
*/
class Bullet
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  Bullet is included")   
#endif

public:
    enum ParticleType
    {
        None,
        Smoke
    };

    enum DamageAreaEffect
    {
        Constant,
        Linear
    };


    // for now uint32_t will be fine for bullet id ... if we calculate with 10 bullets/second for a player, who is shooting
    // continuously in a 60 minutes gameplay, that is still only 36000 bullets for that player, and with 30 players that is
    // still 1 080 000 bullets, so uint32_t is far enough for us. Even if it overflows, it is not the problem, since
    // no such old bullets with low ids will even exists at that time ...
    typedef uint32_t BulletId;

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    static BulletId getGlobalBulletId();
    static void resetGlobalBulletId();
    static void destroyReferenceObject();

    // ---------------------------------------------------------------------------

    /** Ctor to be used by PGE server instance. */
    Bullet(
        const WeaponId& wpnId,
        PR00FsUltimateRenderingEngine& gfx,
        pge_network::PgeNetworkConnectionHandle connHandle,
        TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
        TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
        bool visible,
        TPureFloat sx, TPureFloat sy, TPureFloat sz,
        TPureFloat speed, TPureFloat gravity, TPureFloat drag, TPureBool fragile,
        TPureFloat fDistMax, TPureBool bDmgRelDist,
        TPureBool bCanBounce,
        int nTimerConfigSeconds,
        const ParticleType& particleType,
        int nDamageAp, int nDamageHp,
        TPureFloat fDamageAreaSize,
        const DamageAreaEffect& eDamageAreaEffect,
        TPureFloat fDamageAreaPulse);
    
    /** Ctor to be used by PGE client instance. */
    Bullet(
        const WeaponId& wpnId,
        PR00FsUltimateRenderingEngine& gfx,
        TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
        TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
        bool visible,
        TPureFloat sx, TPureFloat sy, TPureFloat sz,
        TPureFloat speed, TPureFloat gravity, TPureFloat drag, /* client does not receive nor use fragile */
        TPureFloat fDistMax, TPureBool bDmgRelDist,
        /* client does not receive nor use fDistMax */
        TPureBool bCanBounce,
        /* client does not receive nor use nTimerConfigSeconds */
        const ParticleType& particleType,
        /* client does not receive nor use nDamageAp */ int nDamageHp,
        const TPureFloat& fDamageAreaSize,
        const DamageAreaEffect& eDamageAreaEffect,
        const TPureFloat& fDamageAreaPulse);
    
    /** User is expected to explicitly call Bullet::destroyReferenceObject() after they freed up all Bullet instances.
        PGE also calls it when destroying the game, but the game might do it based on their use case. */
    virtual ~Bullet();

    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;
    Bullet(Bullet&&) = delete;
    Bullet& operator=(Bullet&&) = delete;

    //Bullet(const Bullet& other) : // TODO check if we really cannot live with just compiler generated copy ctor?
    //    m_id(other.m_id),
    //    m_wpnId(other.m_wpnId),
    //    m_gfx(other.m_gfx),
    //    m_connHandle(other.m_connHandle),
    //    m_put(other.m_put),
    //    m_speed(other.m_speed),
    //    m_gravityConfigured(other.m_gravityConfigured),
    //    m_gravityCurrent(other.m_gravityCurrent),
    //    m_drag(other.m_drag),
    //    m_fragile(other.m_fragile),
    //    m_fDistMax(other.m_fDistMax),
    //    m_fDistTravelled(other.m_fDistTravelled),
    //    m_nDamageAp(other.m_nDamageAp),
    //    m_nDamageHp(other.m_nDamageHp),
    //    m_fDamageAreaSize(other.m_fDamageAreaSize),
    //    m_eDamageAreaEffect(other.m_eDamageAreaEffect),
    //    m_fDamageAreaPulse(other.m_fDamageAreaPulse),
    //    m_bCreateSentToClients(other.m_bCreateSentToClients)
    //{
    //    m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY()); // TODO: throw if cnanot creaate
    //    m_obj->SetDoubleSided(true);
    //    m_obj->getPosVec() = other.m_obj->getPosVec();
    //    m_obj->getAngleVec() = other.m_obj->getAngleVec();
    //    m_obj->SetRenderingAllowed(other.m_obj->isRenderingAllowed());
    //}

    //Bullet& operator=(const Bullet& other) // TODO check if we really cannot live with just compiler generated operator=?
    //{
    //    m_id = other.m_id;
    //    m_wpnId = other.m_wpnId;
    //    m_gfx = other.m_gfx;
    //    m_connHandle = other.m_connHandle;
    //    m_put = other.m_put;
    //    m_speed = other.m_speed;
    //    m_gravityConfigured = other.m_gravityConfigured;
    //    m_gravityCurrent = other.m_gravityCurrent;
    //    m_drag = other.m_drag;
    //    m_fragile = other.m_fragile;
    //    m_fDistMax = other.m_fDistMax;
    //    m_fDistTravelled = other.m_fDistTravelled;
    //    m_nDamageAp = other.m_nDamageAp;
    //    m_nDamageHp = other.m_nDamageHp;
    //    m_fDamageAreaSize = other.m_fDamageAreaSize;
    //    m_eDamageAreaEffect = other.m_eDamageAreaEffect;
    //    m_fDamageAreaPulse = other.m_fDamageAreaPulse;
    //    m_bCreateSentToClients = other.m_bCreateSentToClients;
    //
    //    m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY());  // TODO: throw if cnanot creaate
    //    m_obj->SetDoubleSided(true);
    //    m_obj->getPosVec() = other.m_obj->getPosVec();
    //    m_obj->getAngleVec() = other.m_obj->getAngleVec();
    //    m_obj->SetRenderingAllowed(other.m_obj->isRenderingAllowed());
    //
    //    return *this;
    //}

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    BulletId getId() const;
    const WeaponId& getWeaponId() const;

    pge_network::PgeNetworkConnectionHandle getOwner() const;
    const PurePosUpTarget& getPut() const;

    TPureFloat getSpeed() const;
    TPureFloat getConfiguredGravity() const;
    TPureFloat getCurrentGravity() const;
    TPureFloat getDrag() const;
    TPureBool isFragile() const;
    const ParticleType& getParticleType() const;
    int& getParticleEmitPerNthPhysicsIterationCntr();
    const int& getParticleEmitPerNthPhysicsIterationCntr() const;
    int& getParticlesEmittedTotal();
    const int& getParticlesEmittedTotal() const;
    TPureFloat getTravelDistanceMax() const;
    TPureFloat getTravelledDistance() const;
    TPureBool canBounce() const;
    int getTimerConfigSeconds() const;
    bool expired() const;
    int getDamageAp() const;
    int getDamageHp() const;
    TPureBool isDamageRelativeToDistance() const;
    TPureFloat getAreaDamageSize() const;
    const DamageAreaEffect& getAreaDamageEffect() const;
    TPureFloat getAreaDamagePulse() const;
    bool& isCreateSentToClients();

    /** Init to be used by PGE server instance.
        This also gets invoked by PgeObjectPool::create() when PooledBullet instances are stored there. */
    void init(
        const WeaponId& wpnId,
        PR00FsUltimateRenderingEngine& gfx,
        pge_network::PgeNetworkConnectionHandle connHandle,
        TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
        TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
        bool visible,
        TPureFloat sx, TPureFloat sy, TPureFloat sz,
        TPureFloat speed, TPureFloat gravity, TPureFloat drag, TPureBool fragile,
        TPureFloat fDistMax, TPureBool bDmgRelDist,
        TPureBool bCanBounce,
        int nTimerConfigSeconds,
        const ParticleType& particleType,
        int nDamageAp, int nDamageHp,
        TPureFloat fDamageAreaSize,
        const DamageAreaEffect& eDamageAreaEffect,
        TPureFloat fDamageAreaPulse);

    /** Init to be used by PGE client instance.
        This also gets invoked by PgeObjectPool::create() when PooledBullet instances are stored there. */
    void init(
        const BulletId& id,
        const WeaponId& wpnId,
        PR00FsUltimateRenderingEngine& gfx,
        TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
        TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
        bool visible,
        TPureFloat sx, TPureFloat sy, TPureFloat sz,
        TPureFloat speed, TPureFloat gravity, TPureFloat drag, /* client does not receive nor use fragile */
        TPureFloat fDistMax, TPureBool bDmgRelDist,
        TPureBool bCanBounce,
        const ParticleType& particleType,
        /* client does not receive nor use nDamageAp */ int nDamageHp,
        TPureFloat fDamageAreaSize,
        const DamageAreaEffect& eDamageAreaEffect,
        TPureFloat fDamageAreaPulse);

    void update(
        const unsigned int& nFactor,
        const float& fGravityChangePerTick,
        const float& fGravityMin);

    void handleVerticalCollision(
        const PureObject3D& pObjectHit,
        const TPureFloat& oldBulletY,
        const unsigned int& nFactor /* physics rate from physics engine */,
        const float& fGravityMin);
    void handleHorizontalCollision(
        const PureObject3D& pObjectHit,
        const TPureFloat& oldBulletX);

    PureObject3D& getObject3D();
    const PureObject3D& getObject3D() const;

protected:

private:

    static BulletId m_globalBulletId;                      /**< Next unique bullet id for identifying. Used by PGE server instance only. */
    static PureObject3D* m_pObjRef;                        /**< Reference object for cloned bullet objects. */

    BulletId m_id;                                         /**< Unique bullet id for identifying. Used by both PGE client and server instances. */
    WeaponId m_wpnId;                                      /**< Unique weapon id. Used by both PGE client and server instances. */
    PR00FsUltimateRenderingEngine& m_gfx;
    pge_network::PgeNetworkConnectionHandle m_connHandle;  /**< Owner (shooter) of this bullet. Used by PGE server instance only. */
    PurePosUpTarget m_put;                                 /**< PUT to calculate next position. Used by both PGE client and server instances. */
    TPureFloat m_speed;                                    /**< Speed as defined by weapon file. Used by both PGE client and server instances. */
    TPureFloat m_gravityConfigured;                        /**< Gravity as defined by weapon file. Used by both PGE client and server instances. */
    TPureFloat m_gravityCurrent;                           /**< Gravity as defined by weapon file. Used by PGE server instance only. */
    TPureFloat m_drag;                                     /**< Drag as defined by weapon file. Used by both PGE client and server instances. */
    TPureBool m_fragile;                                   /**< Fragile flag as defined by weapon file. Used by PGE server instance only. */
    TPureFloat m_fDistMax;                                 /**< Max distance as defined by weapon file. Used by both PGE client and server instances. */
    TPureBool m_bDmgRelDist;                               /**< Damage is relative to distance travelled by bullet, as defined by weapon file. Used by both PGE client and server instances. */
    TPureFloat m_fDistTravelled;                           /**< Distance travelled so far. Used by both PGE client and server instances. */
    TPureBool m_bCanBounce;                                /**< Can bounce off walls, as defined by weapon file. Used by both PGE client and server instances. */
    int m_nTimerConfigSeconds;                             /**< Configured timer, as defined by weapon file. Used by PGE server instance only. */
    std::chrono::time_point<std::chrono::steady_clock>
        m_timeFired;                                       /**< Timestamp of firing the bullet. Used by PGE server instance only. */
    ParticleType m_particleType;                           /**< Emitted particle type. Used by both PGE client and server instances. */
    int m_nParticleEmitPerNthPhysicsIterCntr;              /**< Counter to be used by particle emitter logic. Used by both PGE client and server instances. */
    int m_nParticlesEmittedCurrent;                        /**< Counter to be used by particle emitter logic. Used by both PGE client and server instances. */
    int m_nDamageAp;                                       /**< Damage to AP as defined by weapon file. Used by PGE server instance only. */
    int m_nDamageHp;                                       /**< Damage to HP as defined by weapon file. Used by both PGE client and server instances. */
    TPureFloat m_fDamageAreaSize;                          /**< Area damage size as defined by weapon file. Used by both PGE client and server instances. */
    DamageAreaEffect m_eDamageAreaEffect;                  /**< Area damage effect as defined by weapon file. Used by both PGE client and server instances. */
    TPureFloat m_fDamageAreaPulse;                         /**< Area damage pulse to HP as defined by weapon file. Used by both PGE client and server instances. */

    PureObject3D* m_obj;                                   /**< Associated Pure object to be rendered. Used by PGE server and client instances.
                                                                TODO: shared ptr would be better though, so deleting the obj earlier than bullet
                                                                instance wouldn't be a problem. */
    bool m_bCreateSentToClients;                           /**< Server should send update to clients about creation of new bullets. By default false, client ignores. */

    // ---------------------------------------------------------------------------

    void build3dObject();

}; // class Bullet

/**
    Pooled Bullet class to be used with PgeObjectPool.
*/
class PooledBullet : public Bullet, public PgePooledObject
{
public:
    /** Most of the parameters required by Bullet ctor are NOT available when we create the pool,
        for those we are passing some default values. Later when a pooled bullet is actually
        "created" by firing a weapon, those values should be properly specified. */
    PooledBullet(
        PgeObjectPoolBase& parentPool,
        PR00FsUltimateRenderingEngine& gfx) :
        PgePooledObject(parentPool),
        /* Bullet server ctor is invoked where the Bullet id is assigned internally.
           This is invoked also on client, thus client also assigns Bullet ids on its own.
           It is not a problem because server and client assign Bullet ids using the same logic,
           so they will assign exactly the same Bullet ids.
           Still the MsgBulletUpdateFromServer will contain Bullet id that must be set for a new
           Bullet on client side. */
        Bullet(
            static_cast<WeaponId>(0),
            gfx,
            static_cast<pge_network::PgeNetworkConnectionHandle>(0),
            0.f, 0.f, 0.f, /* wpn pos */
            0.f, 0.f, 0.f, /* wpn angles */
            false /* visible */,
            1.f, 1.f, 0.f /* size */,
            0.f /* speed */, 0.f /* gravity */, 0.f /* drag */, false /* fragile */,
            0.f /* distMax */, false /* dmgRelDist */,
            false /* can bounce */,
            0 /* timer */,
            Bullet::ParticleType::None,
            0 /* AP */, 0 /* HP */,
            0.f /* dmg area size */, Bullet::DamageAreaEffect::Constant, 0.f /* dmg area pulse */)
    {}

    ~PooledBullet() = default;

    PooledBullet(const PooledBullet&) = delete;
    PooledBullet& operator=(const PooledBullet&) = delete;
    PooledBullet(PooledBullet&&) = delete;
    PooledBullet& operator=(PooledBullet&&) = delete;

    virtual void onSetUsed() override
    {
        if (!used())
        {
            getObject3D().SetRenderingAllowed(false);
        }
    }
}; // class PooledBullet

/**
    Weapon class for PR00F's Game Engine Weapon Manager
*/
class Weapon : public PGEcfgFile
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  Weapon is included")   
#endif

public:
    enum State
    {
        WPN_READY,
        WPN_RELOADING,
        WPN_SHOOTING
    }; /**< Weapon states are independent of isTriggerReleased(). */

    enum FiringMode
    {
        WPN_FM_SEMI,  /**< Semi-automatic: trigger needs to be pulled-and-released as many times as many bullets we want to fire. */
        WPN_FM_BURST, /**< Similar to semi, but 1 trigger pull shots some (limited amount) bullets after each other. */
        WPN_FM_AUTO   /**< Trigger needs to be pulled only once to fire continuously until we have bullet. */
    };

    enum Type
    {
        Ranged,
        Melee,
        Throwable
    };

    static constexpr TPureFloat WpnYBiasToPlayerCenter{ 0.15f };  // TODO: I guess this supposed to be get/set through functions later based on something ...

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */
    static std::string stateToString(const State& eState);

    // ---------------------------------------------------------------------------

    Weapon(
        const char* fname,
        PgeObjectPool<PooledBullet>& bullets,
        pge_audio::PgeAudio& audio,
        PR00FsUltimateRenderingEngine& gfx,
        pge_network::PgeNetworkConnectionHandle connHandle);
    virtual ~Weapon();

    CConsole&   getConsole() const;                     /**< Returns access to console preset with logger module name as this class. */

    const WeaponId& getUniqueId() const;
    const Type& getType() const;

    PureObject3D& getObject3D();                        /**< Returns the graphical object entity associated to this weapon object. */
    const PureObject3D& getObject3D() const;            /**< Returns the graphical object entity associated to this weapon object. */
    void UpdatePosition(
        const PureVector& playerPos, bool bStickToCenter);   /**< Updates the graphical object entity associated to this weapon object. */
    void UpdatePositions(
        const PureVector& playerPos, TPureFloat fAngleY, TPureFloat fAngleZ);  /**< Updates the graphical object entity associated to this weapon object. */
    void UpdatePositions(
        const PureVector& playerPos, const PureVector& targetPos2D);           /**< Updates the graphical object entity associated to this weapon object. */

    const FiringMode& getCurrentFiringMode() const;  /**< Returns the current firing mode of the weapon. */

    TPureUInt getUnmagBulletCount() const;      /**< Returns the "unmag" bullet count of this weapon. */
    void SetUnmagBulletCount(TPureUInt count);  /**< Sets the "unmag" bullet count of this weapon. */

    TPureUInt getMagBulletCount() const;        /**< Returns the "mag" bullet count of this weapon. */
    void SetMagBulletCount(TPureUInt count);    /**< Sets the "mag" bullet count of this weapon. */

    bool canIncBulletCount() const;             /**< Returns if we can add more bullets for this weapon. */
    void IncBulletCount(TPureUInt count);       /**< Adds more bullets for this weapon. */

    const pge_network::PgeNetworkConnectionHandle& getOwner() const;      /**< Returns the player associated with this weapon. */
    void SetOwner(const pge_network::PgeNetworkConnectionHandle& owner);  /**< Sets the player associated with this weapon. */

    bool update();                     /**< Updates the weapon based on the time elapsed since last call to this function.*/
    const PgeOldNewValue<State>& getState() const;  /**< Returns the current state of the weapon. */
    void clientReceiveStateFromServer(const State& state);
    void updateOldValues();
    TPureBool reload();                /**< Reloads the weapon i.e. moves some "unmag" bullets into "mag" bullet count. */
    TPureBool pullTrigger(
        bool bMoving,
        bool bRun,
        bool bDuck);                   /**< Pulls the trigger of the weapon, which might result in shooting the weapon. */
    void releaseTrigger();             /**< Releases the trigger of the weapon. */
    bool isTriggerReleased() const;    /**< Returns the state of the trigger. */

    void Reset();                      /**< Resets properties of the weapon to its initial values like when it was just loaded from file. */

    bool isAvailable() const;          /**< Returns if this weapon is available for the player. */
    void SetAvailable(bool bAvail);    /**< Changes the availability of this weapon for the player. */

    float getFiringRate() const;             /**< Returns a calculated firing rate per 1 second. */
    float getBulletRate() const;             /**< Returns a calculated bullet firing rate per 1 second. */
    float getDamagePerFireRating() const;    /**< Returns a calculated rating of damage caused by a single shot fired. */
    float getDamagePerSecondRating() const;  /**< Returns a calculated rating of total damage per 1 second. */

    float getAccuracyByPose(bool bMoving, bool bRun, bool bDuck) const;              /**< Returns a calculated accuracy based on player's pose. */
    float getLowestAccuracyByPose() const;                                           /**< Returns the lowest possible accuracy of this weapon, based only on player's pose. */
    float getMomentaryRecoilMultiplier() const;                                      /**< Returns the momentary recoil multiplier. */
    float getMaximumRecoilMultiplier() const;                                        /**< Returns the weapon's maximum recoil multiplier. */
    float getMomentaryAccuracy(bool bMoving, bool bRun, bool bDuck) const;           /**< Returns the calculated momentary accuracy based on all factors. */
    float getLowestAccuracyPossible() const;                                         /**< Returns a positive relative bullet angle representing the lowest possible accuracy with this weapon. */
    float getRandomRelativeBulletAngle(bool bMoving, bool bRun, bool bDuck) const;   /**< Returns a random relative angle for a bullet. */

    SoLoud::Wav& getFiringSound();
    SoLoud::Wav& getDryFiringSound();
    SoLoud::Wav& getReloadStartSound();
    SoLoud::Wav& getReloadEndSound();
    SoLoud::Wav& getPlayerHitSound();
    SoLoud::Wav& getWallHitSound();
    SoLoud::Wav& getBulletBounceSound();

    Weapon(const Weapon& other) : // TODO check if we really cannot live with just compiler generated copy ctor?
        PGEcfgFile(other),
        m_bullets(other.m_bullets),
        m_audio(other.m_audio),
        m_gfx(other.m_gfx),
        m_connHandle(other.m_connHandle),
        m_id(other.m_id),
        m_type(other.m_type),
        m_state(other.m_state),
        m_firingMode(other.m_firingMode),
        m_nUnmagBulletCount(other.m_nUnmagBulletCount),
        m_nMagBulletCount(other.m_nMagBulletCount),
        m_nBulletsToReload(other.m_nBulletsToReload),
        m_timeReloadStarted(other.m_timeReloadStarted),
        m_timeLastShot(other.m_timeLastShot),
        m_bAvailable(false),
        m_bTriggerReleased(true)
    {
        // TODO: this is same as in regular ctor and operator=
        m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY());
        m_obj->SetDoubleSided(true);
        m_obj->Hide();

        PureTexture* const wpntex = m_gfx.getTextureManager().createFromFile(
            (std::string("gamedata\\textures\\weapons\\") + PFL::changeExtension(this->getFilename().c_str(), "bmp")).c_str());
        if (wpntex)
        {
            // set blending only when texture is available, otherwise object might not be visible at all
            m_obj->getMaterial().setTexture(wpntex);
            m_obj->getMaterial(false).setBlendFuncs(PURE_SRC_ALPHA, PURE_ONE);
        }
    }

    Weapon& operator=(const Weapon& other) // TODO check if we really cannot live with just compiler generated operator_=?
    {
        //m_bullets = other.m_bullets;
        //m_audio = other.m_audio; // deleted assignment operator
        m_gfx = other.m_gfx;
        m_connHandle = other.m_connHandle;
        m_id = other.m_id;
        m_type = other.m_type;
        m_state = other.m_state;
        m_firingMode = other.m_firingMode;
        m_nUnmagBulletCount = other.m_nUnmagBulletCount;
        m_nMagBulletCount = other.m_nMagBulletCount;
        m_nBulletsToReload = other.m_nBulletsToReload;
        m_timeReloadStarted = other.m_timeReloadStarted;
        m_timeLastShot = other.m_timeLastShot;
        m_bAvailable = other.m_bAvailable;
        m_bTriggerReleased = other.m_bTriggerReleased;

        // TODO: this is same as in regular ctor and copy ctor
        m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY());
        m_obj->SetDoubleSided(true);
        m_obj->Hide();

        PureTexture* const wpntex = m_gfx.getTextureManager().createFromFile(
            (std::string("gamedata\\textures\\weapons\\") + PFL::changeExtension(this->getFilename().c_str(), "bmp")).c_str());
        if (wpntex)
        {
            // set blending only when texture is available, otherwise object might not be visible at all
            m_obj->getMaterial().setTexture(wpntex);
            m_obj->getMaterial(false).setBlendFuncs(PURE_SRC_ALPHA, PURE_ONE);
        }

        return *this;
    }

protected:

private:

    typedef std::pair<FiringMode, std::string> FiringModeEnumToStringPair;

    static const std::vector<FiringModeEnumToStringPair> m_vecOrderOfFiringModes;

    static std::set<std::string> m_WpnAcceptedVars;

    PgeObjectPool<PooledBullet>& m_bullets;
    pge_audio::PgeAudio& m_audio;
    PR00FsUltimateRenderingEngine& m_gfx;
    pge_network::PgeNetworkConnectionHandle m_connHandle;  /**< Owner (shooter) of this weapon. Should be used by PGE server instance only. */
    PureObject3D* m_obj;
    WeaponId m_id{};                                   /**< Unique ID, filled by ctor. */
    Type m_type{};                                     /**< Type of weapon, filled by ctor. */
    PgeOldNewValue<State> m_state;                     /**< State as calculated and updated by PGE server instance. */
    FiringMode m_firingMode;                           /**< Current firing mode, something between getVars("firing_mode_def") and getVars("firing_mode_max"). */
    TPureUInt m_nUnmagBulletCount;                     /**< Spare bullets not loaded into weapon. Should be managed by PGE server instance. */
    TPureUInt m_nMagBulletCount;                       /**< Bullets loaded into weapon. Even if weapon is not reloadable. Should be managed by PGE server instance. */
    TPureUInt m_nBulletsToReload;                      /**< Only updated during reload() / Update(). Should be managed by PGE server instance. */
    PFL::timeval m_timeReloadStarted;                  /**< Only updated during reload() / Update(). Should be managed by PGE server instance. */
    PFL::timeval m_timeLastShot;                       /**< Only updated during pullTrigger() / Update(). Should be managed by PGE server instance. */
    bool m_bAvailable;                                 /**< Flag for the game, e.g. if true then the player has this weapon. */
    bool m_bTriggerReleased;                           /**< True if trigger is released, false when being pulled. True by default. */
    SoLoud::Wav m_sndShoot;
    SoLoud::Wav m_sndShootDry;
    SoLoud::Wav m_sndReloadStart;
    SoLoud::Wav m_sndReloadEnd;
    SoLoud::Wav m_sndDamageWall;
    SoLoud::Wav m_sndDamagePlayer;
    SoLoud::Wav m_sndBulletBounce;

    // ---------------------------------------------------------------------------

    Weapon();

    void UpdateGraphics();

}; // class Weapon


/**
    PR00F's Game Engine Weapon Manager.
    The idea is that Weapon instances are created and stored by this manager.
    Each player has a different instance of Weapon Manager. Thus, for each player, Weapon instances also need to be created.
    This way, each player can utilize convenient manager functions, such as:
     - setCurrentWeapon()
     - getNextBestAvailableWeapon()
     - etc.
*/
class WeaponManager
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  Weapon Manager is included")   
#endif

public:
    typedef std::map<unsigned char, std::string> KeypressToWeaponMap;

    static const char* getLoggerModuleName();              /**< Returns the logger module name of this class. */

    static KeypressToWeaponMap& getKeypressToWeaponMap();  /**< Returns the only instance of KeypressToWeaponMap. */

    // ---------------------------------------------------------------------------

    WeaponManager(
        pge_audio::PgeAudio& audio,
        PGEcfgProfiles& cfgProfiles,
        PR00FsUltimateRenderingEngine& gfx,
        PgeObjectPool<PooledBullet>& bullets);
    virtual ~WeaponManager();

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    Weapon* load(const char* fname, pge_network::PgeNetworkConnectionHandle connHandle);
    const std::vector<Weapon*>& getWeapons() const;

    Weapon* getWeaponById(const WeaponId& id);
    const Weapon* getWeaponById(const WeaponId& id) const;
    
    Weapon* getWeaponByFilename(const std::string& wpnName);
    const Weapon* getWeaponByFilename(const std::string& wpnName) const;

    const std::string& getDefaultAvailableWeaponFilename() const;
    bool setDefaultAvailableWeaponByFilename(const std::string& sFilename);

    const Weapon* getCurrentWeapon() const;
    Weapon* getCurrentWeapon();
    bool setCurrentWeapon(Weapon* wpn, bool bRecordSwitchTime, bool bServer);

    Weapon* getPrevAvailableWeapon(unsigned char& cTargetWeapon);
    const Weapon* getPrevAvailableWeapon(unsigned char& cTargetWeapon) const;
    
    Weapon* getNextAvailableWeapon(unsigned char& cTargetWeapon);
    const Weapon* getNextAvailableWeapon(unsigned char& cTargetWeapon) const;

    Weapon* getNextBestAvailableWeapon(unsigned char& cTargetWeapon, bool bMustHaveMagBullet);
    const Weapon* getNextBestAvailableWeapon(unsigned char& cTargetWeapon, bool bMustHaveMagBullet) const;

    const std::chrono::time_point<std::chrono::steady_clock>& getTimeLastWeaponSwitch() const;

    void Clear();
    PgeObjectPool<PooledBullet>& getBullets();

protected:

    WeaponManager(const WeaponManager&) :
        m_audio(m_audio),
        m_cfgProfiles(m_cfgProfiles),
        m_gfx(m_gfx),
        m_pCurrentWpn(nullptr),
        m_bullets(m_bullets)
    {}

    WeaponManager& operator=(const WeaponManager&)
    {
        return *this;
    }

private:
    static KeypressToWeaponMap m_mapKeypressToWeapon;

    pge_audio::PgeAudio& m_audio;
    PGEcfgProfiles& m_cfgProfiles;
    PR00FsUltimateRenderingEngine& m_gfx;
    std::vector<Weapon*> m_weapons;  // due to lot of legacy code, I'm keeping this as vector, but it should be std::map<Weapon::WeaponId, Weapon*>
    Weapon* m_pCurrentWpn;
    std::chrono::time_point<std::chrono::steady_clock> m_timeLastWeaponSwitch;
    PgeObjectPool<PooledBullet>& m_bullets;
    std::string m_sDefaultAvailableWeapon;

    // ---------------------------------------------------------------------------

    WeaponManager();

}; // class WeaponManager
