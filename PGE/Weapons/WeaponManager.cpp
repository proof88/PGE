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


Bullet::Bullet(PR00FsReducedRenderingEngine& gfx,
        TPRREfloat px, TPRREfloat py, TPRREfloat pz,
        TPRREfloat ax, TPRREfloat ay, TPRREfloat az,
        TPRREfloat sx, TPRREfloat sy, TPRREfloat /*sz*/,
        TPRREfloat speed, TPRREfloat gravity, TPRREfloat drag, TPRREbool fragile) :
    m_gfx(gfx),
    m_speed(speed),
    m_gravity(gravity),
    m_drag(drag),
    m_fragile(fragile),
    m_obj(NULL)
{
    m_put.getPosVec().Set(px, py, pz);
    m_put.SetRotation(ax, ay, az);
    m_obj = m_gfx.getObject3DManager().createPlane(sx, sy);
    // TODO throw exception if cant create!
    m_obj->SetDoubleSided(true);
    m_obj->getPosVec().Set(px, py, pz);
    m_obj->getAngleVec().Set(0.f, 0.f, az); // since this is just a plane, we should not rotate it on neither X and Y axes so it always looks 2D
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

const char* Bullet::getLoggerModuleName()
{
    return "Bullet";
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


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


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

void Weapon::UpdatePositions(const PRREVector& playerPos, const PRREVector& targetPos2D)
{
    getObject3D().getPosVec().Set( playerPos.getX(), playerPos.getY(), playerPos.getZ() );

    //distY = _root.mc_mouseTarget._y - (_y + 30);
    const float distYXratio = (targetPos2D.getX() == 0.f) ? 3.0f : targetPos2D.getY()/targetPos2D.getX();
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
    TPRREfloat angley = m_obj->getAngleVec().getY() > 0.0f ? 90.f : -90.f;
    TPRREfloat anglez;
    if ( angley == 90.f )
    {
        anglez = m_obj->getAngleVec().getZ();
    }
    else
    {
        anglez = -m_obj->getAngleVec().getZ();
    }
    m_bullets.push_back(
        Bullet(
            m_gfx,
            m_obj->getPosVec().getX(), m_obj->getPosVec().getY(), m_obj->getPosVec().getZ(),
            m_obj->getAngleVec().getX(), angley, anglez,
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
        Weapon wpn(fname, m_bullets, m_gfx);
        m_weapons.push_back(wpn);
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

std::vector<Weapon>& WeaponManager::getWeapons()
{
    return m_weapons;
}

void WeaponManager::Clear()
{
    m_weapons.clear();
}

std::list<Bullet>& WeaponManager::getBullets()
{
    return m_bullets;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


