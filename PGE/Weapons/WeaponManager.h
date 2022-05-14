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

/**
    Bullet class for PR00F's Game Engine Weapon Manager
*/
class Bullet
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  Bullet is included")   
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    Bullet(
        PR00FsReducedRenderingEngine& gfx,
        TPRREfloat px, TPRREfloat py, TPRREfloat pz,
        TPRREfloat ax, TPRREfloat ay, TPRREfloat az,
        TPRREfloat sx, TPRREfloat sy, TPRREfloat sz,
        TPRREfloat speed, TPRREfloat gravity, TPRREfloat drag, TPRREbool fragile);
    virtual ~Bullet();

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    void Update();

    PRREObject3D& getObject3D();

    Bullet(const Bullet& other) : // TODO check if we really cannot live with just compiler generated copy ctor?
        m_gfx(other.m_gfx),
        m_put(other.m_put),
        m_speed(other.m_speed),
        m_gravity(other.m_gravity),
        m_drag(other.m_drag),
        m_fragile(other.m_fragile)
    {
        m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY()); // TODO: throw if cnanot creaate
        m_obj->SetDoubleSided(true);
        m_obj->getPosVec() = other.m_obj->getPosVec();
        m_obj->getAngleVec() = other.m_obj->getAngleVec();
    }

    Bullet& operator=(const Bullet& other) // TODO check if we really cannot live with just compiler generated operator=?
    {
        m_gfx = other.m_gfx;
        m_put = other.m_put;
        m_speed = other.m_speed;
        m_gravity = other.m_gravity;
        m_drag = other.m_drag;
        m_fragile = other.m_fragile;

        m_obj = m_gfx.getObject3DManager().createPlane(other.m_obj->getSizeVec().getX(), other.m_obj->getSizeVec().getY());  // TODO: throw if cnanot creaate
        m_obj->SetDoubleSided(true);
        m_obj->getPosVec() = other.m_obj->getPosVec();
        m_obj->getAngleVec() = other.m_obj->getAngleVec();

        return *this;
    }

protected:

private:

    PR00FsReducedRenderingEngine& m_gfx;
    PRREPosUpTarget m_put;
    TPRREfloat m_speed;
    TPRREfloat m_gravity;
    TPRREfloat m_drag;
    TPRREbool m_fragile;

    PRREObject3D* m_obj;

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

    Weapon(const char* fname, std::list<Bullet>& bullets, PR00FsReducedRenderingEngine& gfx);
    virtual ~Weapon();

    CConsole&   getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    PRREObject3D& getObject3D();
    void UpdatePositions(const PRREVector& playerPos, const PRREVector& targetPos2D);

    TPRREuint getUnmagBulletCount() const;
    void SetUnmagBulletCount(TPRREuint count);

    TPRREuint getMagBulletCount() const;
    void SetMagBulletCount(TPRREuint count);

    void Update();
    State getState() const;
    TPRREbool reload();
    TPRREbool shoot();

    Weapon(const Weapon& other) : // TODO check if we really cannot live with just compiler generated copy ctor?
        PGEcfgFile(other),
        m_bullets(other.m_bullets),
        m_gfx(other.m_gfx),
        m_state(other.m_state),
        m_nUnmagBulletCount(other.m_nUnmagBulletCount),
        m_nMagBulletCount(other.m_nMagBulletCount),
        m_nBulletsToReload(other.m_nBulletsToReload),
        m_timeReloadStarted(other.m_timeReloadStarted),
        m_timeLastShot(other.m_timeLastShot)
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
        m_state = other.m_state;
        m_nUnmagBulletCount = other.m_nUnmagBulletCount;
        m_nMagBulletCount = other.m_nMagBulletCount;
        m_nBulletsToReload = other.m_nBulletsToReload;
        m_timeReloadStarted = other.m_timeReloadStarted;
        m_timeLastShot = other.m_timeLastShot;

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
    PRREObject3D* m_obj;
    State m_state;
    TPRREuint m_nUnmagBulletCount;                     /**< Spare bullets not loaded into weapon. */
    TPRREuint m_nMagBulletCount;                       /**< Bullets loaded into weapon. Even if weapon is not reloadable. */
    TPRREuint m_nBulletsToReload;                      /**< Only updated during reload() / Update(). */
    PFL::timeval m_timeReloadStarted;                  /**< Only updated during reload() / Update(). */
    PFL::timeval m_timeLastShot;                       /**< Only updated during shoot() / Update(). */

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

    bool load(const char* fname);
    std::vector<Weapon>& getWeapons();
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
    std::vector<Weapon> m_weapons;
    std::list<Bullet> m_bullets;

    // ---------------------------------------------------------------------------

    WeaponManager();

}; // class WeaponManager
