#include "WeaponManager.h"
#include "WeaponManager.h"
#include "WeaponManager.h"
/*
    ###################################################################################
    WeaponManager.cpp
    This file is part of PGE.
    PR00F's Game Engine Weapon Manager
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "WeaponManager.h"


/*
   Bullet
   ###########################################################################
*/


// ############################### PUBLIC ################################

const char* Bullet::getLoggerModuleName()
{
    return "Bullet";
}

Bullet::BulletId Bullet::getGlobalBulletId()
{
    return m_globalBulletId;
}

Bullet::Bullet(
    PR00FsReducedRenderingEngine& gfx,
    TPRREfloat wpn_px, TPRREfloat wpn_py, TPRREfloat wpn_pz,
    TPRREfloat wpn_ax, TPRREfloat wpn_ay, TPRREfloat wpn_az,
    TPRREfloat sx, TPRREfloat sy, TPRREfloat /*sz*/,
    TPRREfloat speed, TPRREfloat gravity, TPRREfloat drag, TPRREbool fragile) :
    m_id(m_globalBulletId++),
    m_gfx(gfx),
    m_speed(speed),
    m_gravity(gravity),
    m_drag(drag),
    m_fragile(fragile),
    m_obj(NULL)
{
    if ( (m_speed == 1000.f) && (m_drag > 0.f))
    {
        getConsole().EOLnOO("Bullet ctor: m_speed is 1000 but m_drag is non-zero!");
        throw std::runtime_error("Bullet ctor: m_speed is 1000 but m_drag is non-zero");
    }

    m_put.getPosVec().Set(wpn_px, wpn_py, wpn_pz);
    m_put.SetRotation(wpn_ax, (wpn_ay > 0.0f) ? 90.f : -90.f, (wpn_ay > 0.0f) ? wpn_az : -wpn_az);

    m_obj = m_gfx.getObject3DManager().createPlane(sx, sy);
    // TODO throw exception if cant create!
    m_obj->SetDoubleSided(true);
    m_obj->getPosVec().Set(wpn_px, wpn_py, wpn_pz);
    m_obj->getAngleVec().Set(wpn_ax, wpn_ay, wpn_az);
}

Bullet::Bullet(
    PR00FsReducedRenderingEngine& gfx,
    Bullet::BulletId id,
    TPRREfloat wpn_px, TPRREfloat wpn_py, TPRREfloat wpn_pz,
    TPRREfloat wpn_ax, TPRREfloat wpn_ay, TPRREfloat wpn_az,
    TPRREfloat sx, TPRREfloat sy, TPRREfloat /*sz*/) :
    m_id(id),
    m_gfx(gfx),
    m_speed(0.f),
    m_gravity(0.f),
    m_drag(0.f),
    m_fragile(0.f),
    m_obj(NULL)
{
    m_put.getPosVec().Set(wpn_px, wpn_py, wpn_pz);
    m_put.SetRotation(wpn_ax, (wpn_ay > 0.0f) ? 90.f : -90.f, (wpn_ay > 0.0f) ? wpn_az : -wpn_az);

    m_obj = m_gfx.getObject3DManager().createPlane(sx, sy);
    // TODO throw exception if cant create!
    m_obj->SetDoubleSided(true);
    m_obj->getPosVec().Set(wpn_px, wpn_py, wpn_pz);
    m_obj->getAngleVec().Set(wpn_ax, wpn_ay, wpn_az);
}

Bullet::~Bullet()
{
    if ( m_obj )
    {
        m_gfx.getObject3DManager().DeleteAttachedInstance(*m_obj);
    }
}

CConsole& Bullet::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

Bullet::BulletId Bullet::getId() const
{
    return m_id;
}

TPRREfloat Bullet::getSpeed() const
{
    return m_speed;
}

TPRREfloat Bullet::getGravity() const
{
    return m_gravity;
}

TPRREfloat Bullet::getDrag() const
{
    return m_drag;
}

TPRREbool Bullet::isFragile() const
{
    return m_fragile;
}

void Bullet::Update()
{
    m_put.Move(m_speed);
    m_obj->getPosVec() = m_put.getPosVec();
}


PRREObject3D& Bullet::getObject3D()
{
    return *m_obj;
}

const PRREObject3D& Bullet::getObject3D() const
{
    return *m_obj;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


uint32_t Bullet::m_globalBulletId = 0;


/*
   Weapon
   ###########################################################################
*/


// ############################### PUBLIC ################################


Weapon::Weapon(const char* fname, std::list<Bullet>& bullets, PR00FsReducedRenderingEngine& gfx) :
    PGEcfgFile(true, false),
    m_bullets(bullets),
    m_gfx(gfx),
    m_obj(NULL),
    m_state(WPN_READY),
    m_nUnmagBulletCount(0),
    m_nMagBulletCount(0),
    m_nBulletsToReload(0)
{
    getConsole().OLnOI("Weapon::Weapon(%s) ...", fname);

    if ( m_WpnAcceptedVars.empty() )
    {
        m_WpnAcceptedVars.insert("name");
        m_WpnAcceptedVars.insert("cap_max");
        m_WpnAcceptedVars.insert("reloadable");
        m_WpnAcceptedVars.insert("bullets_default");
        m_WpnAcceptedVars.insert("reload_per_mag");
        m_WpnAcceptedVars.insert("reload_whole_mag");
        m_WpnAcceptedVars.insert("reload_time");
        m_WpnAcceptedVars.insert("firing_mode_def");
        m_WpnAcceptedVars.insert("firing_mode_max");
        m_WpnAcceptedVars.insert("firing_cooldown");
        m_WpnAcceptedVars.insert("acc_angle");
        m_WpnAcceptedVars.insert("acc_m_walk");
        m_WpnAcceptedVars.insert("acc_m_run");
        m_WpnAcceptedVars.insert("acc_m_duck");
        m_WpnAcceptedVars.insert("recoil_m");
        m_WpnAcceptedVars.insert("recoil_cooldown");
        m_WpnAcceptedVars.insert("recoil_control");
        m_WpnAcceptedVars.insert("bullet_size_x");
        m_WpnAcceptedVars.insert("bullet_size_y");
        m_WpnAcceptedVars.insert("bullet_size_z");
        m_WpnAcceptedVars.insert("bullet_speed");
        m_WpnAcceptedVars.insert("bullet_gravity");
        m_WpnAcceptedVars.insert("bullet_drag");
        m_WpnAcceptedVars.insert("bullet_fragile");
        m_WpnAcceptedVars.insert("damage_hp");
        m_WpnAcceptedVars.insert("damage_ap");
        m_WpnAcceptedVars.insert("damage_area_size");
        m_WpnAcceptedVars.insert("damage_area_effect");
        m_WpnAcceptedVars.insert("damage_area_pulse");
    }

    // Need to think about better design ...
    // The problem in this design is that the derived part always makes a copy of the set,
    // so if we have 100 weapons in a game, then the set will have 100 copies, with exactly
    // the same content. A solution would be if derived part would just update a reference
    // of the static set defined here in the derived part, however that might have some issues,
    // e.g. when the set is freed up by anyhow, then all weapons reference illegal memory area ...
    // So let's re-think this after we have Cpp11 with shared ptrs or something ...
    setAcceptedVars(m_WpnAcceptedVars);

    if ( !load(fname) )
    {
        getConsole().EOLnOO("failed to load file: %s! ", fname);
        throw std::runtime_error("failed to load file: " + std::string(fname));
    }

    if ( (getVars()["reloadable"].getAsInt() == 0) && getVars()["reload_per_mag"].getAsBool() )
    {
        getConsole().EOLnOO("reloadable is 0 but reload_per_mag is true in %s! ", fname);
        throw std::runtime_error("reloadable is 0 but reload_per_mag is true in " + std::string(fname));
    }

    if ( (getVars()["reloadable"].getAsInt() > 0) && (getVars()["bullets_default"].getAsInt() > getVars()["reloadable"].getAsInt()) )
    {
        getConsole().EOLnOO("bullets_default cannot be greater than reloadable when the latter is non-zero %s! ", fname);
        throw std::runtime_error("bullets_default cannot be greater than reloadable when the latter is non-zero in " + std::string(fname));
    }

    if ( getVars()["reload_whole_mag"].getAsBool() && !getVars()["reload_per_mag"].getAsBool() )
    {
        getConsole().EOLnOO("reload_whole_mag is true but reload_per_mag is false in %s! ", fname);
        throw std::runtime_error("reload_whole_mag is true but reload_per_mag is false in " + std::string(fname));
    }

    if ( getVars()["recoil_m"].getAsFloat() > 1.f )
    {
        if ( getVars()["recoil_cooldown"].getAsInt() < getVars()["firing_cooldown"].getAsInt() )
        {
            getConsole().EOLnOO("recoil enabled, but recoil_cooldown is less than firing_cooldown in %s! ", fname);
            throw std::runtime_error("recoil enabled, but recoil_cooldown is less than firing_cooldown in " + std::string(fname));
        }
    }

    if ( (getVars()["recoil_m"].getAsFloat() == 1.f) && (getVars()["recoil_cooldown"].getAsInt() > 0) )
    {
        getConsole().EOLnOO("recoil_m is 1 but recoil_cooldown is non-zero in %s! ", fname);
        throw std::runtime_error("recoil_m is 1 but recoil_cooldown is non-zero in " + std::string(fname));
    }

    if ( (getVars()["recoil_m"].getAsFloat() == 1.f) && (getVars()["recoil_control"].getAsString() != "off") )
    {
        getConsole().EOLnOO("recoil_m is 1 but recoil_control is not off in %s! ", fname);
        throw std::runtime_error("recoil_m is 1 but recoil_control is not off in " + std::string(fname));
    }

    if ( (getVars()["bullet_speed"].getAsFloat() == 1000.f) && (getVars()["bullet_drag"].getAsFloat() > 0.f) )
    {
        getConsole().EOLnOO("bullet_speed is 1000 but bullet_drag is non-zero in %s! ", fname);
        throw std::runtime_error("bullet_speed is 1000 but bullet_drag is non-zero in " + std::string(fname));
    }

    if ( (getVars()["damage_area_size"].getAsFloat() == 0.f) && (getVars()["damage_area_pulse"].getAsFloat() > 0.f) )
    {
        getConsole().EOLnOO("damage_area_size is 0 but damage_area_pulse is non-zero in %s! ", fname);
        throw std::runtime_error("damage_area_size is 0 but damage_area_pulse is non-zero in " + std::string(fname));
    }

    // it doesnt matter if weapon is reloadable or not, the loaded bullet count is in nMagBulletCount
    m_nMagBulletCount = getVars()["bullets_default"].getAsInt();

    m_obj = m_gfx.getObject3DManager().createPlane(1.f, 0.5f); // TODO: grab sizes from wpn file
    if ( !m_obj )
    {
        getConsole().EOLnOO("m_obj is null for %s! ", fname);
        throw std::runtime_error("m_obj is null for " + std::string(fname));
    }

    m_obj->SetDoubleSided(true);
    PRRETexture* wpntex = m_gfx.getTextureManager().createFromFile( "gamedata\\textures\\hud_wpn_mchgun_b_nolabel.bmp" ); // TODO: grab texture from wpn file
    m_obj->getMaterial().setTexture( wpntex );
    m_obj->getMaterial(false).setBlendFuncs(PRRE_SRC_ALPHA, PRRE_ONE);

    getConsole().SOLnOO("Weapon loaded!");
}

Weapon::~Weapon()
{
    if ( m_obj )
    {
        m_gfx.getObject3DManager().DeleteAttachedInstance(*m_obj);
    }
}

CConsole& Weapon::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

const char* Weapon::getLoggerModuleName()
{
    return "Weapon";
}

PRREObject3D& Weapon::getObject3D()
{
    return *m_obj;
}

const PRREObject3D& Weapon::getObject3D() const
{
    return *m_obj;
}

void Weapon::UpdatePosition(const PRREVector& playerPos)
{
    getObject3D().getPosVec().Set(playerPos.getX(), playerPos.getY(), playerPos.getZ());
}

void Weapon::UpdatePositions(const PRREVector& playerPos, TPRREfloat fAngleY, TPRREfloat fAngleZ)
{
    getObject3D().getPosVec().Set(playerPos.getX(), playerPos.getY(), playerPos.getZ());
    getObject3D().getAngleVec().SetY(fAngleY);
    getObject3D().getAngleVec().SetZ(fAngleZ);
}

void Weapon::UpdatePositions(const PRREVector& playerPos, const PRREVector& targetPos2D)
{
    getObject3D().getPosVec().Set( playerPos.getX(), playerPos.getY(), playerPos.getZ() );

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

    const float distYXratio = (targetPos2D.getX() == 0.f) ? targetPos2D.getY() : targetPos2D.getY()/targetPos2D.getX();
    if ( targetPos2D.getX() < 0.f )
    {
        getObject3D().getAngleVec().SetY( 0.f );
        getObject3D().getAngleVec().SetZ( atan( distYXratio )*180.f / PFL::PI );
    }
    else
    {
        getObject3D().getAngleVec().SetY( 180.f );
        getObject3D().getAngleVec().SetZ( -atan( distYXratio )*180.f / PFL::PI );
    }
}

TPRREuint Weapon::getUnmagBulletCount() const
{
    return m_nUnmagBulletCount;
}

void Weapon::SetUnmagBulletCount(TPRREuint count)
{
    m_nUnmagBulletCount = count;
}

TPRREuint Weapon::getMagBulletCount() const
{
    return m_nMagBulletCount;
}

void Weapon::SetMagBulletCount(TPRREuint count)
{
    m_nMagBulletCount = count;
}

void Weapon::Update()
{
    UpdateGraphics();

    if ( m_state == WPN_READY )
    {
        return;
    }

    PFL::timeval timeNow;
    PFL::gettimeofday(&timeNow, 0);

    if ( m_state == WPN_SHOOTING )
    {      
        const TPRREfloat fMillisecsSinceLastShot = PFL::getTimeDiffInUs(timeNow, m_timeLastShot) / 1000.f;
        if ( getVars()["firing_cooldown"].getAsInt() <= fMillisecsSinceLastShot )
        {
            m_state = WPN_READY;
        }
        return;
    }

    // WPN_RELOADING
    if ( getVars()["reload_per_mag"].getAsBool() )
    {
        const TPRREfloat fMillisecsSinceReloadStarted = PFL::getTimeDiffInUs(timeNow, m_timeReloadStarted) / 1000.f;
        if ( getVars()["reload_time"].getAsInt() <= fMillisecsSinceReloadStarted )
        {
            if ( getVars()["reload_whole_mag"].getAsBool() )
            {
                m_nMagBulletCount = m_nBulletsToReload;
            }
            else
            {
                m_nMagBulletCount += m_nBulletsToReload;
            }
            m_nUnmagBulletCount -= m_nBulletsToReload;
            m_state = WPN_READY;
        }
    }
    else
    {
        const TPRREfloat fMillisecsSinceLastBulletReload = PFL::getTimeDiffInUs(timeNow, m_timeReloadStarted) / 1000.f;
        if ( getVars()["reload_time"].getAsInt() <= fMillisecsSinceLastBulletReload )
        {
            m_nMagBulletCount++;
            m_nUnmagBulletCount--;
            m_nBulletsToReload--;
            if ( m_nBulletsToReload == 0 )
            {
                m_state = WPN_READY;
            }
            else
            {
                m_timeReloadStarted = timeNow;
            }
        }
    }
}

Weapon::State Weapon::getState() const
{
    return m_state;
}

TPRREbool Weapon::reload()
{
    if ( m_state != WPN_READY )
    {
        return false;
    }

    const TPRREuint nCapMagazine = getVars()["reloadable"].getAsInt();
    if ( nCapMagazine == 0 )
    {
        // not reloadable
        return false;
    }

    if ( m_nMagBulletCount == nCapMagazine )
    {
        // magazine is already full
        return false;
    }

    if ( m_nUnmagBulletCount == 0 )
    {
        // we do not have spare bullets
        return false;
    }

    m_state = WPN_RELOADING;
    if ( getVars()["reload_whole_mag"].getAsBool() )
    {
        m_nBulletsToReload = min(nCapMagazine, m_nUnmagBulletCount);
    }
    else
    {
        m_nBulletsToReload = min(nCapMagazine - m_nMagBulletCount, m_nUnmagBulletCount);
    }
    
    PFL::gettimeofday(&m_timeReloadStarted, 0);

    return true;
}

TPRREbool Weapon::shoot()
{
    if ( (m_state != WPN_READY) && /* reloading can be stopped if it is per-bullet */
         !( (m_state == WPN_RELOADING) && (!getVars()["reload_per_mag"].getAsBool()) ) )
    {
        return false;
    }

    if ( m_nMagBulletCount == 0 )
    {
        return false;
    }

    m_state = WPN_SHOOTING;
    m_nMagBulletCount--;
    
    m_bullets.push_back(
        Bullet(
            m_gfx,
            m_obj->getPosVec().getX(), m_obj->getPosVec().getY(), m_obj->getPosVec().getZ(),
            m_obj->getAngleVec().getX(), m_obj->getAngleVec().getY(), m_obj->getAngleVec().getZ(),
            getVars()["bullet_size_x"].getAsFloat(),
            getVars()["bullet_size_y"].getAsFloat(),
            getVars()["bullet_size_z"].getAsFloat(),
            getVars()["bullet_speed"].getAsFloat(),
            getVars()["bullet_gravity"].getAsFloat(),
            getVars()["bullet_drag"].getAsFloat(),
            getVars()["bullet_fragile"].getAsBool())
    );

    PFL::gettimeofday(&m_timeLastShot, 0);

    return true;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


std::set<std::string> Weapon::m_WpnAcceptedVars;


Weapon::Weapon() :
    PGEcfgFile(true, false),
    m_bullets(m_bullets),
    m_gfx(m_gfx),
    m_obj(NULL),
    m_state(WPN_READY),
    m_nUnmagBulletCount(0),
    m_nMagBulletCount(0),
    m_nBulletsToReload(0)
{}

void Weapon::UpdateGraphics()
{
}


/*
   WeaponManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


WeaponManager::WeaponManager(PR00FsReducedRenderingEngine& gfx) :
    m_gfx(gfx)
{

}

WeaponManager::~WeaponManager()
{
    Clear();
}

CConsole& WeaponManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

const char* WeaponManager::getLoggerModuleName()
{
    return "WeaponManager";
}

bool WeaponManager::load(const char* fname)
{
    try
    {
        Weapon* const wpn = new Weapon(fname, m_bullets, m_gfx);
        if (!wpn)
        {
            return false;
        }

        m_weapons.push_back(wpn);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

std::vector<Weapon*>& WeaponManager::getWeapons()
{
    return m_weapons;
}

void WeaponManager::Clear()
{
    for (auto& pWpn : m_weapons)
    {
        delete pWpn;
    }
    m_weapons.clear();
    m_bullets.clear();
}

std::list<Bullet>& WeaponManager::getBullets()
{
    return m_bullets;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


