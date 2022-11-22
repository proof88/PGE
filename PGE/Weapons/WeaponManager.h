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

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../PGEallHeaders.h"
#include "../PGEcfgFile.h"
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"
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
        PR00FsReducedRenderingEngine& gfx,
        pge_network::PgeNetworkConnectionHandle connHandle,
        TPRREfloat wpn_px, TPRREfloat wpn_py, TPRREfloat wpn_pz,
        TPRREfloat wpn_ax, TPRREfloat wpn_ay, TPRREfloat wpn_az,
        TPRREfloat sx, TPRREfloat sy, TPRREfloat sz,
        TPRREfloat speed, TPRREfloat gravity, TPRREfloat drag, TPRREbool fragile, int nDamageHp);
    
    /** Ctor to be used by PGE client instance: bullet id as received from server. */
    Bullet(
        PR00FsReducedRenderingEngine& gfx,
        Bullet::BulletId id,
        TPRREfloat wpn_px, TPRREfloat wpn_py, TPRREfloat wpn_pz,
        TPRREfloat wpn_ax, TPRREfloat wpn_ay, TPRREfloat wpn_az,
        TPRREfloat sx, TPRREfloat sy, TPRREfloat sz);
    
    virtual ~Bullet();

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    BulletId getId() const;

    pge_network::PgeNetworkConnectionHandle getOwner() const;

    TPRREfloat getSpeed() const;
    TPRREfloat getGravity() const;
    TPRREfloat getDrag() const;
    TPRREbool isFragile() const;
    int getDamageHp() const;

    void Update();

    PRREObject3D& getObject3D();
    const PRREObject3D& getObject3D() const;

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
    PR00FsReducedRenderingEngine& m_gfx;
    pge_network::PgeNetworkConnectionHandle m_connHandle;  /**< Owner (shooter) of this bullet. Used by PGE server instance only. */
    PRREPosUpTarget m_put;                                 /**< PUT to calculate next position. Used by PGE server instance only. */
    TPRREfloat m_speed;                                    /**< Speed as defined by weapon file. Used by PGE server instance only. */
    TPRREfloat m_gravity;                                  /**< Gravity as defined by weapon file. Used by PGE server instance only. */
    TPRREfloat m_drag;                                     /**< Drag as defined by weapon file. Used by PGE server instance only. */
    TPRREbool m_fragile;                                   /**< Fragile flag as defined by weapon file. Used by PGE server instance only. */
    int m_nDamageHp;                                       /**< Damage to HP as defined by weapon file. Used by PGE server instance only. */

    PRREObject3D* m_obj;                                   /**< Associated PRRE object to be rendered. Used by PGE server and client instances. */

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
    };
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    Weapon(const char* fname, std::list<Bullet>& bullets, PR00FsReducedRenderingEngine& gfx, pge_network::PgeNetworkConnectionHandle connHandle);
    virtual ~Weapon();

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    PRREObject3D& getObject3D();
    const PRREObject3D& getObject3D() const;
    void UpdatePosition(const PRREVector& playerPos);
    void UpdatePositions(const PRREVector& playerPos, TPRREfloat fAngleY, TPRREfloat fAngleZ);
    void UpdatePositions(const PRREVector& playerPos, const PRREVector& targetPos2D);

    TPRREuint getUnmagBulletCount() const;
    void SetUnmagBulletCount(TPRREuint count);

    TPRREuint getMagBulletCount() const;
    void SetMagBulletCount(TPRREuint count);

    pge_network::PgeNetworkConnectionHandle getOwner() const;

    void Update();
    State getState() const;
    TPRREbool reload();
    TPRREbool shoot();

    void Reset();

    bool isAvailable() const;
    void SetAvailable(bool bAvail);

    Weapon(const Weapon& other) : // TODO check if we really cannot live with just compiler generated copy ctor?
        PGEcfgFile(other),
        m_bullets(other.m_bullets),
        m_gfx(other.m_gfx),
        m_connHandle(other.m_connHandle),
        m_state(other.m_state),
        m_nUnmagBulletCount(other.m_nUnmagBulletCount),
        m_nMagBulletCount(other.m_nMagBulletCount),
        m_nBulletsToReload(other.m_nBulletsToReload),
        m_timeReloadStarted(other.m_timeReloadStarted),
        m_timeLastShot(other.m_timeLastShot),
        m_bAvailable(false)
    {
        m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY());
        m_obj->SetDoubleSided(true);
        PRRETexture* wpntex = m_gfx.getTextureManager().createFromFile( "gamedata\\textures\\hud_wpn_mchgun_b_nolabel.bmp" ); // TODO: grab texture from wpn file
        m_obj->getMaterial().setTexture( wpntex );
        m_obj->getMaterial(false).setBlendFuncs(PRRE_SRC_ALPHA, PRRE_ONE);
    }

    Weapon& operator=(const Weapon& other) // TODO check if we really cannot live with just compiler generated operator_=?
    {
        m_bullets = other.m_bullets;
        m_gfx = other.m_gfx;
        m_connHandle = other.m_connHandle;
        m_state = other.m_state;
        m_nUnmagBulletCount = other.m_nUnmagBulletCount;
        m_nMagBulletCount = other.m_nMagBulletCount;
        m_nBulletsToReload = other.m_nBulletsToReload;
        m_timeReloadStarted = other.m_timeReloadStarted;
        m_timeLastShot = other.m_timeLastShot;
        m_bAvailable = other.m_bAvailable;

        m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY());
        m_obj->SetDoubleSided(true);
        PRRETexture* wpntex = m_gfx.getTextureManager().createFromFile( "gamedata\\textures\\hud_wpn_mchgun_b_nolabel.bmp" ); // TODO: grab texture from wpn file
        m_obj->getMaterial().setTexture( wpntex );
        m_obj->getMaterial(false).setBlendFuncs(PRRE_SRC_ALPHA, PRRE_ONE);

        return *this;
    }

protected:

private:

    static std::set<std::string> m_WpnAcceptedVars;

    std::list<Bullet>& m_bullets;
    PR00FsReducedRenderingEngine& m_gfx;
    pge_network::PgeNetworkConnectionHandle m_connHandle;  /**< Owner (shooter) of this weapon. Should be used by PGE server instance only. */
    PRREObject3D* m_obj;
    State m_state;                                     /**< State as calculated and updated by PGE server instance. */
    TPRREuint m_nUnmagBulletCount;                     /**< Spare bullets not loaded into weapon. Should be managed by PGE server instance. */
    TPRREuint m_nMagBulletCount;                       /**< Bullets loaded into weapon. Even if weapon is not reloadable. Should be managed by PGE server instance. */
    TPRREuint m_nBulletsToReload;                      /**< Only updated during reload() / Update(). Should be managed by PGE server instance. */
    PFL::timeval m_timeReloadStarted;                  /**< Only updated during reload() / Update(). Should be managed by PGE server instance. */
    PFL::timeval m_timeLastShot;                       /**< Only updated during shoot() / Update(). Should be managed by PGE server instance. */
    bool m_bAvailable;                                 /**< Flag for the game, e.g. if true then the player has this weapon. */

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
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    WeaponManager(PR00FsReducedRenderingEngine& gfx);
    virtual ~WeaponManager();

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    bool load(const char* fname, pge_network::PgeNetworkConnectionHandle connHandle);
    std::vector<Weapon*>& getWeapons();
    void Clear();
    std::list<Bullet>& getBullets();

protected:

    WeaponManager(const WeaponManager&) :
        m_gfx(m_gfx)
    {}

    WeaponManager& operator=(const WeaponManager&)
    {
        return *this;
    }

private:

    PR00FsReducedRenderingEngine& m_gfx;
    std::vector<Weapon*> m_weapons;
    std::list<Bullet> m_bullets;

    // ---------------------------------------------------------------------------

    WeaponManager();

}; // class WeaponManager
