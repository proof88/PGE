#pragma once

/*
    ###################################################################################
    WeaponManager.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine Weapon Manager
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <chrono> // requires cpp11
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../PGEallHeaders.h"
#include "../Config/PGEcfgFile.h"
#include "../Config/PGEcfgProfiles.h"
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"
#include "../Network/PgePacket.h"

/**
    Bullet class for PR00F's Game Engine Weapon Manager
*/
class Bullet
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  Bullet is included")   
#endif

public:
    // for now uint32_t will be fine for bullet id ... if we calculate with 10 bullets/second for a player, who is shooting
    // continuously in a 60 minutes gameplay, that is still only 36000 bullets for that player, and with 30 players that is
    // still 1 080 000 bullets, so uint32_t is far enough for us. Even if it overflows, it is not the problem, since
    // no such old bullets with low ids will even exists at that time ...
    typedef uint32_t BulletId;

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    static BulletId getGlobalBulletId();
    static void ResetGlobalBulletId();

    // ---------------------------------------------------------------------------

    /** Ctor to be used by PGE server instance: bullet id will be assigned within the ctor. */
    Bullet(
        PR00FsUltimateRenderingEngine& gfx,
        pge_network::PgeNetworkConnectionHandle connHandle,
        TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
        TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
        TPureFloat sx, TPureFloat sy, TPureFloat sz,
        TPureFloat speed, TPureFloat gravity, TPureFloat drag, TPureBool fragile, int nDamageHp);
    
    /** Ctor to be used by PGE client instance: bullet id as received from server. */
    Bullet(
        PR00FsUltimateRenderingEngine& gfx,
        Bullet::BulletId id,
        TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
        TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
        TPureFloat sx, TPureFloat sy, TPureFloat sz);
    
    virtual ~Bullet();

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    BulletId getId() const;

    pge_network::PgeNetworkConnectionHandle getOwner() const;

    TPureFloat getSpeed() const;
    TPureFloat getGravity() const;
    TPureFloat getDrag() const;
    TPureBool isFragile() const;
    int getDamageHp() const;

    void Update();

    PureObject3D& getObject3D();
    const PureObject3D& getObject3D() const;

    Bullet(const Bullet& other) : // TODO check if we really cannot live with just compiler generated copy ctor?
        m_id(other.m_id),
        m_gfx(other.m_gfx),
        m_connHandle(other.m_connHandle),
        m_put(other.m_put),
        m_speed(other.m_speed),
        m_gravity(other.m_gravity),
        m_drag(other.m_drag),
        m_fragile(other.m_fragile),
        m_nDamageHp(other.m_nDamageHp)
    {
        m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY()); // TODO: throw if cnanot creaate
        m_obj->SetDoubleSided(true);
        m_obj->getPosVec() = other.m_obj->getPosVec();
        m_obj->getAngleVec() = other.m_obj->getAngleVec();
    }

    Bullet& operator=(const Bullet& other) // TODO check if we really cannot live with just compiler generated operator=?
    {
        m_id = other.m_id;
        m_gfx = other.m_gfx;
        m_connHandle = other.m_connHandle;
        m_put = other.m_put;
        m_speed = other.m_speed;
        m_gravity = other.m_gravity;
        m_drag = other.m_drag;
        m_fragile = other.m_fragile;
        m_nDamageHp = other.m_nDamageHp;

        m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY());  // TODO: throw if cnanot creaate
        m_obj->SetDoubleSided(true);
        m_obj->getPosVec() = other.m_obj->getPosVec();
        m_obj->getAngleVec() = other.m_obj->getAngleVec();

        return *this;
    }

protected:

private:

    static BulletId m_globalBulletId;                      /**< Next unique bullet id for identifying. Used by PGE server instance only. */

    BulletId m_id;                                         /**< Unique bullet id for identifying. Used by PGE server and client instances. */
    PR00FsUltimateRenderingEngine& m_gfx;
    pge_network::PgeNetworkConnectionHandle m_connHandle;  /**< Owner (shooter) of this bullet. Used by PGE server instance only. */
    PurePosUpTarget m_put;                                 /**< PUT to calculate next position. Used by PGE server instance only. */
    TPureFloat m_speed;                                    /**< Speed as defined by weapon file. Used by PGE server instance only. */
    TPureFloat m_gravity;                                  /**< Gravity as defined by weapon file. Used by PGE server instance only. */
    TPureFloat m_drag;                                     /**< Drag as defined by weapon file. Used by PGE server instance only. */
    TPureBool m_fragile;                                   /**< Fragile flag as defined by weapon file. Used by PGE server instance only. */
    int m_nDamageHp;                                       /**< Damage to HP as defined by weapon file. Used by PGE server instance only. */

    PureObject3D* m_obj;                                   /**< Associated Pure object to be rendered. Used by PGE server and client instances. */

    // ---------------------------------------------------------------------------

}; // class Bullet

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
        WPN_FM_PROJ,  /**< Similar to burst, but 1 trigger pull shots some (limited amount) of projectiles AT ONCE, e.g. shotguns. */
        WPN_FM_AUTO   /**< Trigger needs to be pulled only once to fire continuously until we have bullet. */
    };

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    Weapon(
        const char* fname, std::list<Bullet>& bullets,
        PR00FsUltimateRenderingEngine& gfx,
        pge_network::PgeNetworkConnectionHandle connHandle);
    virtual ~Weapon();

    CConsole&   getConsole() const;                     /**< Returns access to console preset with logger module name as this class. */

    PureObject3D& getObject3D();                        /**< Returns the graphical object entity associated to this weapon object. */
    const PureObject3D& getObject3D() const;            /**< Returns the graphical object entity associated to this weapon object. */
    void UpdatePosition(const PureVector& playerPos);   /**< Updates the graphical object entity associated to this weapon object. */
    void UpdatePositions(const PureVector& playerPos, TPureFloat fAngleY, TPureFloat fAngleZ);  /**< Updates the graphical object entity associated to this weapon object. */
    void UpdatePositions(const PureVector& playerPos, const PureVector& targetPos2D);           /**< Updates the graphical object entity associated to this weapon object. */

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
    State getState() const;            /**< Returns the current state of the weapon. */
    TPureBool reload();                /**< Reloads the weapon i.e. moves some "unmag" bullets into "mag" bullet count. */
    TPureBool pullTrigger();           /**< Pulls the trigger of the weapon, which might result in shooting the weapon. */
    void releaseTrigger();             /**< Releases the trigger of the weapon. */
    bool isTriggerReleased() const;    /**< Returns the state of the trigger. */

    void Reset();                      /**< Resets properties of the weapon to its initial values like when it was just loaded from file. */

    bool isAvailable() const;          /**< Returns if this weapon is available for the player. */
    void SetAvailable(bool bAvail);    /**< Changes the availability of this weapon for the player. */

    const float getDamagePerFireRating() const;    /**< Returns a calculated rating of damage caused by a single shot fired. */
    const float getDamagePerSecondRating() const;  /**< Returns a calculated rating of total damage per 1 second. */

    Weapon(const Weapon& other) : // TODO check if we really cannot live with just compiler generated copy ctor?
        PGEcfgFile(other),
        m_bullets(other.m_bullets),
        m_gfx(other.m_gfx),
        m_connHandle(other.m_connHandle),
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
        m_bullets = other.m_bullets;
        m_gfx = other.m_gfx;
        m_connHandle = other.m_connHandle;
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

    std::list<Bullet>& m_bullets;
    PR00FsUltimateRenderingEngine& m_gfx;
    pge_network::PgeNetworkConnectionHandle m_connHandle;  /**< Owner (shooter) of this weapon. Should be used by PGE server instance only. */
    PureObject3D* m_obj;
    State m_state;                                     /**< State as calculated and updated by PGE server instance. */
    FiringMode m_firingMode;                           /**< Current firing mode, something between getVars("firing_mode_def") and getVars("firing_mode_max"). */
    TPureUInt m_nUnmagBulletCount;                     /**< Spare bullets not loaded into weapon. Should be managed by PGE server instance. */
    TPureUInt m_nMagBulletCount;                       /**< Bullets loaded into weapon. Even if weapon is not reloadable. Should be managed by PGE server instance. */
    TPureUInt m_nBulletsToReload;                      /**< Only updated during reload() / Update(). Should be managed by PGE server instance. */
    PFL::timeval m_timeReloadStarted;                  /**< Only updated during reload() / Update(). Should be managed by PGE server instance. */
    PFL::timeval m_timeLastShot;                       /**< Only updated during pullTrigger() / Update(). Should be managed by PGE server instance. */
    bool m_bAvailable;                                 /**< Flag for the game, e.g. if true then the player has this weapon. */
    bool m_bTriggerReleased;                           /**< True if trigger is released, false when being pulled. True by default. */

    // ---------------------------------------------------------------------------

    Weapon();

    void UpdateGraphics();

}; // class Weapon


/**
    PR00F's Game Engine Weapon Manager
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

    WeaponManager(PGEcfgProfiles& cfgProfiles, PR00FsUltimateRenderingEngine& gfx, std::list<Bullet>& bullets);
    virtual ~WeaponManager();

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    Weapon* load(const char* fname, pge_network::PgeNetworkConnectionHandle connHandle);
    const std::vector<Weapon*>& getWeapons() const;

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

    const std::chrono::time_point<std::chrono::steady_clock>& getTimeLastWeaponSwitch() const;

    void Clear();
    std::list<Bullet>& getBullets();

protected:

    WeaponManager(const WeaponManager&) :
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

    PGEcfgProfiles& m_cfgProfiles;
    PR00FsUltimateRenderingEngine& m_gfx;
    std::vector<Weapon*> m_weapons;
    Weapon* m_pCurrentWpn;
    std::chrono::time_point<std::chrono::steady_clock> m_timeLastWeaponSwitch;
    std::list<Bullet>& m_bullets;
    std::string m_sDefaultAvailableWeapon;

    // ---------------------------------------------------------------------------

    WeaponManager();

}; // class WeaponManager
