#include "WeaponManager.h"
/*
    ###################################################################################
    WeaponManager.cpp
    This file is part of PGE.
    PR00F's Game Engine Weapon Manager
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "WeaponManager.h"

#include <cmath>


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

void Bullet::resetGlobalBulletId()
{
    m_globalBulletId = 0;
}

Bullet::Bullet(
    PR00FsUltimateRenderingEngine& gfx,
    pge_network::PgeNetworkConnectionHandle connHandle,
    TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
    TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
    TPureFloat sx, TPureFloat sy, TPureFloat /*sz*/,
    TPureFloat speed, TPureFloat gravity, TPureFloat drag, TPureBool fragile,
    int nDamageAp, int nDamageHp,
    TPureFloat fDamageAreaSize, TPureFloat fDamageAreaPulse) :
    m_id(m_globalBulletId++),
    m_gfx(gfx),
    m_connHandle(connHandle),
    m_speed(speed),
    m_gravity(gravity),
    m_drag(drag),
    m_fragile(fragile),
    m_nDamageAp(nDamageAp),
    m_nDamageHp(nDamageHp),
    m_fDamageAreaSize(fDamageAreaSize),
    m_fDamageAreaPulse(fDamageAreaPulse),
    m_obj(NULL),
    m_bCreateSentToClients(false)
{
    // TODO: actually these copy-pasted validations related to bullets should be defined in a public static validate function,
    // which should be invoked by WeaponManager when a new Weapon is constructed, to remove validation redundancy!
    // Ticket: https://github.com/proof88/PGE/issues/14
    if ( (m_speed == 1000.f) && (m_drag > 0.f))
    {
        getConsole().EOLnOO("Bullet ctor: m_speed is 1000 but m_drag is non-zero!");
        throw std::runtime_error("Bullet ctor: m_speed is 1000 but m_drag is non-zero");
    }

    if (m_fDamageAreaSize < 0.f)
    {
        getConsole().EOLnOO("Bullet ctor: m_fDamageAreaSize cannot be negative!");
        throw std::runtime_error("Bullet ctor: m_fDamageAreaSize cannot be negative!");
    }

    if ((m_fDamageAreaSize == 0.f) && (m_fDamageAreaPulse > 0.f))
    {
        getConsole().EOLnOO("Bullet ctor: m_fDamageAreaSize is 0 but m_fDamageAreaPulse is non-zero!");
        throw std::runtime_error("Bullet ctor: m_fDamageAreaSize is 0 but m_fDamageAreaPulse is non-zero!");
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
    PR00FsUltimateRenderingEngine& gfx,
    Bullet::BulletId id,
    TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
    TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
    TPureFloat sx, TPureFloat sy, TPureFloat /*sz*/,
    TPureFloat speed, TPureFloat gravity, TPureFloat drag, int nDamageHp,
    const TPureFloat& fDamageAreaSize,
    const TPureFloat& fDamageAreaPulse) :
    m_id(id),
    m_gfx(gfx),
    m_connHandle(0),
    m_speed(speed),
    m_gravity(gravity),
    m_drag(drag),
    m_fragile(0.f) /* irrelevant for this client-side ctor */,
    m_nDamageAp(0) /* irrelevant for this client-side ctor */,
    m_nDamageHp(nDamageHp),
    m_fDamageAreaSize(fDamageAreaSize),
    m_fDamageAreaPulse(fDamageAreaPulse),
    m_obj(NULL),
    m_bCreateSentToClients(true) /* irrelevant for this client-side ctor but we are client so yes it is sent :) */
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

pge_network::PgeNetworkConnectionHandle Bullet::getOwner() const
{
    return m_connHandle;
}

TPureFloat Bullet::getSpeed() const
{
    return m_speed;
}

TPureFloat Bullet::getGravity() const
{
    return m_gravity;
}

TPureFloat Bullet::getDrag() const
{
    return m_drag;
}

TPureBool Bullet::isFragile() const
{
    return m_fragile;
}

/**
* Maximum damage to Player AP this bullet can cause.
*/
int Bullet::getDamageAp() const
{
    return m_nDamageAp;
}

/**
* Maximum damage to Player HP this bullet can cause.
* 
* It is the game's responsibility to calculate actual damage to player.
* A recommendation I was also doing in PRooFPS v0.2.7:
*  - Player has AP and HP;
*  - explosive bullets make radius damage where damage amount is linear decreasing from radius center until edge;
*  - non-explosive bullets make damage upon hitting target;
*  - Player has takeDamage() function which is responsible for calculating the damage taken by AP and HP, based on
*    input Bullet::getDamageAp() and Bullet::getDamageHp();
*  - if AP is non-0, then it decreases the damage caused to HP.
*/
int Bullet::getDamageHp() const
{
    return m_nDamageHp;
}

TPureFloat Bullet::getAreaDamageSize() const
{
    return m_fDamageAreaSize;
}

TPureFloat Bullet::getAreaDamagePulse() const
{
    return m_fDamageAreaPulse;
}

bool& Bullet::isCreateSentToClients()
{
    return m_bCreateSentToClients;
}

void Bullet::Update(const unsigned int& nFactor)
{
    /*
    * In the PR00FPS Promo flash game I did this:
    * 1.) when firing the bullet, the xMove and yMove properties of the new bullet were initialized like this:
    *     eval("_root.mc_map.mc_bullet"+_root.bulletCount).xMove = Math.cos((this.mc_player_arm1._rotation+aimShit)*Math.PI/180.0) * neg;
    *     eval("_root.mc_map.mc_bullet"+_root.bulletCount).yMove = Math.sin((this.mc_player_arm1._rotation+aimShit)*Math.PI/180.0) * neg;
    *    
    *     And initial positions were set based on the above like this:
    *     eval("_root.mc_map.mc_bullet"+_root.bulletCount)._x = this.newX + eval("_root.mc_map.mc_bullet"+_root.bulletCount).xMove*this.mc_player_arm1._width;
    *     eval("_root.mc_map.mc_bullet"+_root.bulletCount)._y = this.newY+150 + eval("_root.mc_map.mc_bullet"+_root.bulletCount).yMove*this.mc_player_arm1._width;
    * 
    * 2.) And bulletProc updated the position of the bullets with a simple multiplication:
    *     this._x += this.xMove * _root.BULLETSPEED;
    *     this._y += this.yMove * _root.BULLETSPEED;
    * 
    * Maybe this approach would be faster than using PUT.Move() like below.
    * However, collision check is the most expensive thing now anyway, so I don't think about this now.
    */
    m_put.Move(m_speed / nFactor);
    m_obj->getPosVec() = m_put.getPosVec();
}


PureObject3D& Bullet::getObject3D()
{
    return *m_obj;
}

const PureObject3D& Bullet::getObject3D() const
{
    return *m_obj;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


Bullet::BulletId Bullet::m_globalBulletId = 0;


/*
   Weapon
   ###########################################################################
*/


// ############################### PUBLIC ################################


Weapon::Weapon(
    const char* fname,
    std::list<Bullet>& bullets,
    pge_audio::PgeAudio& audio,
    PR00FsUltimateRenderingEngine& gfx,
    pge_network::PgeNetworkConnectionHandle connHandle) :
    PGEcfgFile(true, false),
    m_bullets(bullets),
    m_audio(audio),
    m_gfx(gfx),
    m_connHandle(connHandle),
    m_obj(NULL),
    m_state(WPN_READY),
    m_firingMode(WPN_FM_SEMI),
    m_nUnmagBulletCount(0),
    m_nMagBulletCount(0),
    m_nBulletsToReload(0),
    m_bAvailable(false),
    m_bTriggerReleased(true)
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
        m_WpnAcceptedVars.insert("reload_start_snd");
        m_WpnAcceptedVars.insert("reload_end_snd");
        m_WpnAcceptedVars.insert("firing_mode_def");
        m_WpnAcceptedVars.insert("firing_mode_max");
        m_WpnAcceptedVars.insert("firing_cooldown");
        m_WpnAcceptedVars.insert("firing_snd");
        m_WpnAcceptedVars.insert("firing_dry_snd");
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

    // TODO: too many manual CVAR validations here, update after implementing https://github.com/proof88/PRooFPS-dd/issues/251 !
    if ( (getVars()["reloadable"].getAsInt() == 0) && getVars()["reload_per_mag"].getAsBool() )
    {
        getConsole().EOLnOO("reloadable is 0 but reload_per_mag is true in %s! ", fname);
        throw std::runtime_error("reloadable is 0 but reload_per_mag is true in " + std::string(fname));
    }

    if ( getVars()["reloadable"].getAsInt() > getVars()["cap_max"].getAsInt() )
    {
        getConsole().EOLnOO("reloadable cannot be greater than cap_max in %s! ", fname);
        throw std::runtime_error("reloadable cannot be greater than cap_max in " + std::string(fname));
    }

    if ( (getVars()["reloadable"].getAsInt() > 0) && (getVars()["bullets_default"].getAsInt() > getVars()["reloadable"].getAsInt()) )
    {
        getConsole().EOLnOO("bullets_default cannot be greater than reloadable when the latter is non-zero in %s! ", fname);
        throw std::runtime_error("bullets_default cannot be greater than reloadable when the latter is non-zero in " + std::string(fname));
    }

    // since we call PGEcfgFile ctor at the beginning with "require all accepted values to be present", we can be sure that
    // neither of the below getVars() calls return 'end' iterator
    const auto itDefFiringModePos = std::find_if(
        m_vecOrderOfFiringModes.begin(),
        m_vecOrderOfFiringModes.end(),
        [this](const FiringModeEnumToStringPair& fm) { return fm.second == getVars()["firing_mode_def"].getAsString(); }
    );

    const auto itMaxFiringModePos = std::find_if(
        m_vecOrderOfFiringModes.begin(),
        m_vecOrderOfFiringModes.end(),
        [this](const FiringModeEnumToStringPair& fm) { return fm.second == getVars()["firing_mode_max"].getAsString(); }
    );

    if ((itDefFiringModePos == m_vecOrderOfFiringModes.end()) || (itMaxFiringModePos == m_vecOrderOfFiringModes.end()))
    {
        getConsole().EOLnOO("either default or max firing mode is unhandled: %s or %s in %s! ",
            getVars()["firing_mode_def"].getAsString().c_str(),
            getVars()["firing_mode_max"].getAsString().c_str(),
            fname);
        throw std::runtime_error("either default or max firing mode is unhandled: " + getVars()["firing_mode_def"].getAsString() +
            " or " + getVars()["firing_mode_max"].getAsString() + " in " + std::string(fname));
    }

    if (std::distance(itDefFiringModePos, itMaxFiringModePos) < 0)
    {
        getConsole().EOLnOO("wrong order of default and max firing modes: %s and %s in %s! ",
            getVars()["firing_mode_def"].getAsString().c_str(),
            getVars()["firing_mode_max"].getAsString().c_str(),
            fname);
        throw std::runtime_error("wrong order of default and max firing modes: " + getVars()["firing_mode_def"].getAsString() +
            " and " + getVars()["firing_mode_max"].getAsString() + " in " + std::string(fname));
    }

    if (((getVars()["firing_mode_def"].getAsString() == "burst") && (getVars()["firing_mode_max"].getAsString() == "proj"))
        ||
        ((getVars()["firing_mode_def"].getAsString() == "proj") && (getVars()["firing_mode_max"].getAsString() == "burst")))
    {
        getConsole().EOLnOO("incompatiable default and max firing modes: %s and %s in %s! ",
            getVars()["firing_mode_def"].getAsString().c_str(),
            getVars()["firing_mode_max"].getAsString().c_str(),
            fname);
        throw std::runtime_error("incompatiable default and max firing modes: " + getVars()["firing_mode_def"].getAsString() +
            " and " + getVars()["firing_mode_max"].getAsString() + " in " + std::string(fname));
    }

    if (getVars()["bullets_default"].getAsInt() > getVars()["cap_max"].getAsInt())
    {
        getConsole().EOLnOO("bullets_default cannot be greater than cap_max in %s! ", fname);
        throw std::runtime_error("bullets_default cannot be greater than cap_max in " + std::string(fname));
    }

    if ( getVars()["reload_whole_mag"].getAsBool() && !getVars()["reload_per_mag"].getAsBool() )
    {
        getConsole().EOLnOO("reload_whole_mag is true but reload_per_mag is false in %s! ", fname);
        throw std::runtime_error("reload_whole_mag is true but reload_per_mag is false in " + std::string(fname));
    }

    if (!getVars()["reload_end_snd"].getAsString().empty() && !getVars()["reload_per_mag"].getAsBool())
    {
        getConsole().EOLnOO("reload_end_snd is set but reload_per_mag is false in %s! ", fname);
        throw std::runtime_error("reload_end_snd is set but reload_per_mag is false in " + std::string(fname));
    }

    if (getVars()["firing_cooldown"].getAsInt() < 1)
    {
        getConsole().EOLnOO("firing_cooldown must be a positive value in %s! ", fname);
        throw std::runtime_error("firing_cooldown must be a positive value in " + std::string(fname));
    }

    if (getVars()["acc_angle"].getAsFloat() < 0.f)
    {
        getConsole().EOLnOO("acc_angle cannot be negative in %s! ", fname);
        throw std::runtime_error("acc_angle cannot be negative in " + std::string(fname));
    }

    if (getVars()["acc_m_walk"].getAsFloat() < 0.f)
    {
        getConsole().EOLnOO("acc_m_walk cannot be negative in %s! ", fname);
        throw std::runtime_error("acc_m_walk cannot be negative in " + std::string(fname));
    }

    if (getVars()["acc_m_run"].getAsFloat() < 0.f)
    {
        getConsole().EOLnOO("acc_m_run cannot be negative in %s! ", fname);
        throw std::runtime_error("acc_m_run cannot be negative in " + std::string(fname));
    }

    if (getVars()["acc_m_duck"].getAsFloat() < 0.f)
    {
        getConsole().EOLnOO("acc_m_duck cannot be negative in %s! ", fname);
        throw std::runtime_error("acc_m_duck cannot be negative in " + std::string(fname));
    }

    if (getVars()["recoil_m"].getAsFloat() < 1.f)
    {
        getConsole().EOLnOO("recoil_m cannot be less than 1 in %s! ", fname);
        throw std::runtime_error("recoil_m cannot be less than 1 in " + std::string(fname));
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

    if (getVars()["damage_area_size"].getAsFloat() < 0.f)
    {
        getConsole().EOLnOO("damage_area_size cannot be negative in %s! ", fname);
        throw std::runtime_error("damage_area_size cannot be negative in " + std::string(fname));
    }

    if ( (getVars()["damage_area_size"].getAsFloat() == 0.f) && (getVars()["damage_area_pulse"].getAsFloat() > 0.f) )
    {
        getConsole().EOLnOO("damage_area_size is 0 but damage_area_pulse is non-zero in %s! ", fname);
        throw std::runtime_error("damage_area_size is 0 but damage_area_pulse is non-zero in " + std::string(fname));
    }

    if ((getVars()["damage_hp"].getAsInt() < 1) || (getVars()["damage_ap"].getAsInt() < 1))
    {
        getConsole().EOLnOO("damage_hp and damage_ap must be positive values in %s! ", fname);
        throw std::runtime_error("damage_hp and damage_ap must be positive values in " + std::string(fname));
    }

    Reset();

    // TODO: this is same as in copy ctor and operator=
    m_obj = m_gfx.getObject3DManager().createPlane(1.f, 0.5f); // TODO: grab sizes from wpn file
    if ( !m_obj )
    {
        getConsole().EOLnOO("m_obj is null for %s! ", fname);
        throw std::runtime_error("m_obj is null for " + std::string(fname));
    }

    m_obj->SetDoubleSided(true);
    m_obj->Hide();

    // TODO: hardcoded directory should be coming from somewhere instead!
    PureTexture* const wpntex = m_gfx.getTextureManager().createFromFile(
        (std::string("gamedata\\textures\\weapons\\") + PFL::changeExtension(this->getFilename().c_str(), "bmp")).c_str());
    if (wpntex)
    {
        // set blending only when texture is available, otherwise object might not be visible at all
        m_obj->getMaterial().setTexture(wpntex);
        m_obj->getMaterial(false).setBlendFuncs(PURE_SRC_ALPHA, PURE_ONE);
    }
    else
    {
        getConsole().EOLnOO("texture file was not found for %s! ", fname);
        throw std::runtime_error("texture file was not found for " + std::string(fname));
    }

    // finally we load sounds, failing to load is NOT fatal error, weapons will stay simply silent in such case, SoLoud handles that!
    // TODO: hardcoded directory should be coming from somewhere instead!
    m_audio.loadSound(m_sndShoot, std::string("gamedata\\audio\\weapons\\") + getVars()["firing_snd"].getAsString());
    m_audio.loadSound(m_sndShootDry, std::string("gamedata\\audio\\weapons\\") + getVars()["firing_dry_snd"].getAsString());
    m_audio.loadSound(m_sndReloadStart, std::string("gamedata\\audio\\weapons\\") + getVars()["reload_start_snd"].getAsString());
    m_audio.loadSound(m_sndReloadEnd, std::string("gamedata\\audio\\weapons\\") + getVars()["reload_end_snd"].getAsString());

    getConsole().SOLnOO("Weapon loaded!");
}

Weapon::~Weapon()
{
    if ( m_obj )
    {
        m_gfx.getObject3DManager().DeleteAttachedInstance(*m_obj);
    }
}

/**
    Returns access to console preset with logger module name as this class.
*/
CConsole& Weapon::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
}

const char* Weapon::getLoggerModuleName()
{
    return "Weapon";
}

std::string Weapon::stateToString(const State& eState)
{
    switch (eState)
    {
    case Weapon::State::WPN_RELOADING:
        return "RELOADING";
    case Weapon::State::WPN_SHOOTING:
        return "SHOOTING";
    case Weapon::State::WPN_READY:
        return "READY";
    default:
        return "UNKNOWN";
    }
}

/**
 * Returns the graphical object entity associated to this weapon object.
 */
PureObject3D& Weapon::getObject3D()
{
    return *m_obj;
}

/**
 * Returns the graphical object entity associated to this weapon object.
 */
const PureObject3D& Weapon::getObject3D() const
{
    return *m_obj;
}

/**
 * Updates the graphical object entity associated to this weapon object.
 * Only the position is updated.
 */
void Weapon::UpdatePosition(const PureVector& playerPos, bool bStickToCenter)
{
    getObject3D().getPosVec().Set(
        playerPos.getX(),
        bStickToCenter ? playerPos.getY() : playerPos.getY() + WpnYBiasToPlayerCenter,
        playerPos.getZ());
}

/**
 * Updates the graphical object entity associated to this weapon object.
 * The position and angles are updated to the specified values.
 * This function can be used to update the weapon of other clients on our side.
 */
void Weapon::UpdatePositions(const PureVector& playerPos, TPureFloat fAngleY, TPureFloat fAngleZ)
{
    getObject3D().getPosVec().Set(playerPos.getX(), playerPos.getY() + WpnYBiasToPlayerCenter, playerPos.getZ());
    getObject3D().getAngleVec().SetY(fAngleY);
    getObject3D().getAngleVec().SetZ(fAngleZ);
}

/**
 * Updates the graphical object entity associated to this weapon object.
 * The position and angles are updated. The angles are updated to point to the specified targetPos2D value.
 * This function can be used to update our weapon on our side.
 */
void Weapon::UpdatePositions(const PureVector& playerPos, const PureVector& targetPos2D)
{
    getObject3D().getPosVec().Set( playerPos.getX(), playerPos.getY() + WpnYBiasToPlayerCenter, playerPos.getZ() );

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

    PureVector vecWpnPosProjected2D;
    if (!m_gfx.getCamera().project3dTo2d(
        getObject3D().getPosVec().getX(),
        getObject3D().getPosVec().getY(),
        getObject3D().getPosVec().getZ(),
        vecWpnPosProjected2D))
    {
        // Failure is expected when the weapon is out of frustum, so this is not a real error, ignore it silently
        //getConsole().EOLn("PRooFPSddPGE::%s(): project3dTo2d() failed!", __func__);
        return;
    }
    else
    {
        vecWpnPosProjected2D.SetX(vecWpnPosProjected2D.getX() - m_gfx.getCamera().getViewport().size.width / 2.f);
        vecWpnPosProjected2D.SetY(vecWpnPosProjected2D.getY() - m_gfx.getCamera().getViewport().size.height / 2.f);
        //    getConsole().EOLn("PRooFPSddPGE::%s(): wpn2d: (%f,%f,%f), target2d: (%f,%f,%f)",
        //        __func__,
        //        vecWpnPosProjected2D.getX(), vecWpnPosProjected2D.getY(), vecWpnPosProjected2D.getZ(),
        //        targetPos2D.getX(), targetPos2D.getY(), targetPos2D.getZ());
    }

    const PureVector vecNewTargetPos2D = targetPos2D - vecWpnPosProjected2D;
    const float distYXratio = (vecNewTargetPos2D.getX() == 0.f) ? vecNewTargetPos2D.getY() : vecNewTargetPos2D.getY()/ vecNewTargetPos2D.getX();
    if (vecNewTargetPos2D.getX() < 0.f )
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

/**
 * Returns the current firing mode of the weapon.
 * This is something between getVars("firing_mode_def") and getVars("firing_mode_max").
 * 
 * @return Current firing mode of the weapon.
 */
const Weapon::FiringMode& Weapon::getCurrentFiringMode() const
{
    return m_firingMode;
}

/**
 * Returns the "unmag" bullet count of this weapon.
 * This is the number of bullets with the player not loaded into the weapon.
 * To fire these bullets, they first need to be loaded into the weapon using reload().
 * Only reloadable weapons can have positive number of "unmag" bullets.
 * 
 * @return "Unmag" bullet count i.e. number of bullets with the player not loaded into the weapon.
 */
TPureUInt Weapon::getUnmagBulletCount() const
{
    return m_nUnmagBulletCount;
}

/**
 * Sets the "unmag" bullet count of this weapon.
 * See getUnmagBulletCount() for better understanding "unmag" bullet count.
 * 
 * @param count The "unmag" bullet count to be set. This cannot be greater than getVars()["cap_max"].
 */
void Weapon::SetUnmagBulletCount(TPureUInt count)
{
    if (count <= static_cast<TPureUInt>(getVars()["cap_max"].getAsInt()))
    {
        m_nUnmagBulletCount = count;
    }
}

/**
 * Returns the "mag" bullet count of this weapon.
 * This is the number of bullets already loaded into the weapon.
 * A weapon can be fired only if it has positive "mag" bullet count.
 * "Mag" is short for "magazine", even though not all weapons have "magazines", we always refer to "mag" bullet count
 * to know how many bullets are ready to be fired from the weapon without the need of reload.
 * This also means that not reloadable weapons have all their bullets counted as "mag" bullet count.
 *
 * @return "Mag" bullet count i.e. number of bullets already loaded into the weapon.
 */
TPureUInt Weapon::getMagBulletCount() const
{
    return m_nMagBulletCount;
}

/**
 * Sets the "mag" bullet count of this weapon.
 * See getMagBulletCount() for better understanding "mag" bullet count.
 * 
 * @param count The "mag" bullet count to be set.
 *              For reloadable weapons, this cannot be greater than getVars()["reloadable"].
 *              For non-reloadable weapons, this cannot be greater than getVars()["cap_max"].
 */
void Weapon::SetMagBulletCount(TPureUInt count)
{
    if (getVars()["reloadable"].getAsInt() > 0)
    {
        if (count <= static_cast<TPureUInt>(getVars()["reloadable"].getAsInt()))
        {
            m_nMagBulletCount = count;
        }
    }
    else if (count <= static_cast<TPureUInt>(getVars()["cap_max"].getAsInt()))
    {
        m_nMagBulletCount = count;
    }
}

/**
 * Returns if we can add more bullets for this weapon, i.e. if we can pick up more bullets for this weapon.
 * Useful if we are touching bullet pack for this weapon and want to check if we can pick that up, or we are actually full.
 * Basically this depends on the "unmag" or "mag" bullet count compared to getVars()["cap_max"], depending on if the weapon is reloadable or not.
 * 
 * @return True if we can still add more bullets for this weapon, false otherwise.
 */
bool Weapon::canIncBulletCount() const
{
    if (getVars().at("reloadable").getAsInt() > 0)
    {
        return m_nUnmagBulletCount < static_cast<TPureUInt>(getVars().at("cap_max").getAsInt());
    }
    else
    {
        return m_nMagBulletCount < static_cast<TPureUInt>(getVars().at("cap_max").getAsInt());
    }
}

/**
 * Adds more bullets for this weapon.
 * Useful when we are picking up bullet pack for this weapon or same kind of weapon.
 * 
 * @param count How many bullets we want to add. The actual increase value might be less. See more details at canIncBulletCount(). 
 */
void Weapon::IncBulletCount(TPureUInt count)
{
    if (getVars()["reloadable"].getAsInt() > 0)
    {
        m_nUnmagBulletCount = std::min(static_cast<TPureUInt>(getVars()["cap_max"].getAsInt()), (m_nUnmagBulletCount + count));
    }
    else
    {
        // not reloadable has always zero m_nUnmagBulletCount, e.g. rail gun
        m_nMagBulletCount = std::min(static_cast<TPureUInt>(getVars()["cap_max"].getAsInt()), (m_nMagBulletCount + count));
    }
}

/**
 * Returns the player associated with this weapon.
 * This is the player specified originally in the constructor of this Weapon object, or another player set using SetOwner().
 * Players are identified by their connection handle.
 * 
 * @return Connection handle of player associated with this Weapon object.
 */
const pge_network::PgeNetworkConnectionHandle& Weapon::getOwner() const
{
    return m_connHandle;
}

/**
 * Sets the player associated with this weapon.
 * Players are identified by their connection handle.
 */
void Weapon::SetOwner(const pge_network::PgeNetworkConnectionHandle& owner)
{
    m_connHandle = owner;
}

/**
 * Updates the weapon based on the time elapsed since last call to this function.
 * This function is recommended to be invoked by the game as often as possible, since it is time-dependent.
 * Calling this function takes care of the state transition and other property updates of the weapon, initiated by
 * a trigger pull or reload.
 * 
 * @return True if the mag- or unmag bullet count changed, false otherwise. 
 */
bool Weapon::update()
{
    UpdateGraphics();

    if ( m_state == WPN_READY )
    {
        return false;
    }

    PFL::timeval timeNow;
    PFL::gettimeofday(&timeNow, 0);

    if ( m_state == WPN_SHOOTING )
    {      
        const TPureFloat fMillisecsSinceLastShot = PFL::getTimeDiffInUs(timeNow, m_timeLastShot) / 1000.f;
        if ( getVars()["firing_cooldown"].getAsInt() <= fMillisecsSinceLastShot )
        {
            m_state = WPN_READY;
        }
        return false;
    }

    // WPN_RELOADING
    if ( getVars()["reload_per_mag"].getAsBool() )
    {
        const TPureFloat fMillisecsSinceReloadStarted = PFL::getTimeDiffInUs(timeNow, m_timeReloadStarted) / 1000.f;
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
            return true;
        }
    }
    else
    {
        const TPureFloat fMillisecsSinceLastBulletReload = PFL::getTimeDiffInUs(timeNow, m_timeReloadStarted) / 1000.f;
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
            return true;
        }
    }
    return false;
}

/**
 * Returns the current state of the weapon.
 * Weapon state can be updated by functions like pullTrigger(), reload(), update(), etc.
 * Weapon state is totally independent of the state of trigger, see: isTriggerReleased().
 * 
 * @return Current state of the weapon.
 */
const PgeOldNewValue<Weapon::State>& Weapon::getState() const
{
    return m_state;
}

void Weapon::clientReceiveStateFromServer(const State& state)
{
    updateOldValues();
    m_state = state;
}

void Weapon::updateOldValues()
{
    // TODO: should look like as proofps_dd::Player::updateOldValues()

    m_state.commit();
}

/**
 * Reloads the weapon i.e. moves some "unmag" bullets into "mag" bullet count.
 * See the return value of the function if you are interested in if the reloading is actually triggered by this call.
 * It can happen that the reloading process is not initiated by this call, e.g. reloading has been already initiated previously,
 * or the magazine is already full, etc.
 * 
 * @return True if reload has been actually initiated by this call, false otherwise.
 */
TPureBool Weapon::reload()
{
    if ( (m_state != WPN_READY) || !m_bTriggerReleased )
    {
        return false;
    }

    const TPureUInt nCapMagazine = getVars()["reloadable"].getAsInt();
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
        m_nBulletsToReload = std::min(nCapMagazine, m_nUnmagBulletCount);
    }
    else
    {
        m_nBulletsToReload = std::min(nCapMagazine - m_nMagBulletCount, m_nUnmagBulletCount);
    }
    
    PFL::gettimeofday(&m_timeReloadStarted, 0);

    return true;
}

/**
 * Pulls the trigger of the weapon, which might result in shooting the weapon.
 * Firing an actual shot depends on different things like current state of the weapon, firing mode, etc.
 * If a shot was actually fired, at least one new bullet is placed in 'bullets' specified in the constructor of this weapon.
 * 
 * @param bMoving Same as for getAccuracyByPose().
 * @param bRun    Same as for getAccuracyByPose().
 * @param bDuck   Same as for getAccuracyByPose().
 * 
 * @return True if a shot was actually triggered, false otherwise.
 */
TPureBool Weapon::pullTrigger(bool bMoving, bool bRun, bool bDuck)
{
    const bool bPrevTriggerReleased = m_bTriggerReleased;
    m_bTriggerReleased = false;
    
    if ( (m_state != WPN_READY) && /* reloading can be stopped if it is per-bullet */
         !( (m_state == WPN_RELOADING) && (!getVars()["reload_per_mag"].getAsBool()) ) )
    {
        return false;
    }

    if (!bPrevTriggerReleased && (m_firingMode != WPN_FM_AUTO))
    {
        // in non-automatic modes, to trigger a next shot, we require the trigger to be released before being pulled
        return false;
    }

    if ( m_nMagBulletCount == 0 )
    {
        return false;
    }

    m_state = WPN_SHOOTING;
    m_nMagBulletCount--;



    const float fRelativeBulletAngleZ = getRandomRelativeBulletAngle(bMoving, bRun, bDuck);
    //getConsole().EOLn("bMoving: %b, bRun: %b, bDuck: %b, fRelativeBulletAngleZ: %f! ", bMoving, bRun, bDuck, fRelativeBulletAngleZ);
    
    m_bullets.push_back(
        Bullet(
            m_gfx,
            m_connHandle,
            m_obj->getPosVec().getX(), m_obj->getPosVec().getY(), m_obj->getPosVec().getZ(),
            m_obj->getAngleVec().getX(), m_obj->getAngleVec().getY(), m_obj->getAngleVec().getZ() + fRelativeBulletAngleZ,
            getVars()["bullet_size_x"].getAsFloat(),
            getVars()["bullet_size_y"].getAsFloat(),
            getVars()["bullet_size_z"].getAsFloat(),
            getVars()["bullet_speed"].getAsFloat(),
            getVars()["bullet_gravity"].getAsFloat(),
            getVars()["bullet_drag"].getAsFloat(),
            getVars()["bullet_fragile"].getAsBool(),
            getVars()["damage_ap"].getAsInt(),
            getVars()["damage_hp"].getAsInt(),
            getVars()["damage_area_size"].getAsFloat(),
            getVars()["damage_area_pulse"].getAsFloat())
    );

    PFL::gettimeofday(&m_timeLastShot, 0);

    return true;
}

/**
 * Releases the trigger of the weapon.
 * It is important to invoke this function whenever the player's input imply it.
 * Some firing modes require repeated pull-and-release of the trigger, see FiringMode for details.
 */
void Weapon::releaseTrigger()
{
    m_bTriggerReleased = true;
}

/**
    Returns the state of the trigger.
    Note that the trigger state is independent of the state of the weapon that can be queried by getState().
    For example:
     - a pulled trigger may or may not transition the state to WPN_SHOOTING;
     - releasing the trigger while the weapon has WPN_SHOOTING state doesn't change the state to anything else.

    @return True if the trigger is in released position, false if it is pulled.
*/
bool Weapon::isTriggerReleased() const
{
    return m_bTriggerReleased;
}

/**
 * Resets properties of the weapon to its initial values like when it was just loaded from file. 
 * Useful when the player respawns and we need to reset all of their weapons.
 * The availability for the player is also being reset to false.
 */
void Weapon::Reset()
{
    const auto itDefFiringModePos = std::find_if(
        m_vecOrderOfFiringModes.begin(),
        m_vecOrderOfFiringModes.end(),
        [this](const FiringModeEnumToStringPair& fm) { return fm.second == getVars()["firing_mode_def"].getAsString(); }
    );

    if (itDefFiringModePos == m_vecOrderOfFiringModes.end())
    {
        // As I wrote in the ctor ... since we call PGEcfgFile ctor at the beginning with "require all accepted values to be present", we
        // can be sure that getVars() doesn't return 'end' iterator there ... however, since we are in a public function of an already
        // constructed object, I can imagine some funny guys accidentally or intentionally screwing up content of getVars(), in such case
        // obviously we might get 'end' iterator here. In that case I throw exception here and expect the program to crash.
        throw std::runtime_error("Weapon::Reset(): itDefFiringModePos is at the end!");
    }

    m_firingMode = itDefFiringModePos->first;
    m_state = Weapon::State::WPN_READY;
    m_bAvailable = false;
    m_bTriggerReleased = true;
    // it doesnt matter if weapon is reloadable or not, the loaded bullet count is in nMagBulletCount
    m_nMagBulletCount = getVars()["bullets_default"].getAsInt();
    m_nUnmagBulletCount = 0;
    m_nBulletsToReload = 0;
}

/**
 * Returns if this weapon is available for the player.
 * The player is that client whose connection handle was specified in the constructor of this Weapon object.
 * 
 * @return True if this weapon is available for the player, false otherwise.
 */
bool Weapon::isAvailable() const
{
    return m_bAvailable;
}

/**
 * Changes the availability of this weapon for the player.
 */
void Weapon::SetAvailable(bool bAvail)
{
    m_bAvailable = bAvail;
}

/**
* Returns a calculated rating of damage caused by a single shot fired: DPFR.
* This is a positive number usually in range [0.01, 100], rarely even bigger.
* 
* @return DPFR (Damage per Fire Rating) caused by a single shot fired, calculated as: damage_hp * damage_ap / 100.f.
*/
float Weapon::getDamagePerFireRating() const
{
    /*
      damage_hp       -> greater is better
      damage_ap       -> greater is better
      damage_area_size-> greater is better
      bullet distance -> greater is better
      firing_mode_def -> greater is better
    */
    // later we can also add damage_area_size and bullet distance and firing_mode_def to this calculation
    return (getVars().at("damage_hp").getAsInt() * getVars().at("damage_ap").getAsInt()) / 100.f;
}

/**
* Returns a calculated rating of total damage per 1 second: DPSR.
* This is used by WeaponManager to put Weapons into order based on their power.
* Always positive.
* 
* @return DPSR (Damage per Second Rating), calculated as: (1000.f/firing_cooldown * DPFR)^2.
*/
float Weapon::getDamagePerSecondRating() const
{
    /*
      reloadable      -> greater is better
      reload_time     -> smaller is better
      firing_cooldown -> smaller is better
    */
    return std::powf(1000.f / getVars().at("firing_cooldown").getAsInt() * getDamagePerFireRating(), 2.f);
}

/**
* Returns a calculated accuracy (aim) based on player's pose.
* This is basically the weapon's base accuracy (CVAR: acc_angle) multiplied by CVARS: acc_m_duck, acc_m_run, acc_m_walk. 
* 
* @param bMoving Set to true of player is moving (walk or run), or false if is still at the moment.
* @param bRun    Used only if bMoving is true.
*                If bRun is true, CVAR acc_m_run will be used, otherwise CVAR acc_m_walk will be used for calculation.
* @param bDuck   If true, accuracy will be multiplied by CVAR acc_m_duck.
* 
* @return Calculated accuracy based on player's pose.
*/
float Weapon::getAccuracyByPose(bool bMoving, bool bRun, bool bDuck) const
{
    float fAccuracy = getVars().at("acc_angle").getAsFloat() * (bDuck ? getVars().at("acc_m_duck").getAsFloat() : 1.f);

    if (bMoving)
    {
        fAccuracy *= bRun ? getVars().at("acc_m_run").getAsFloat() : getVars().at("acc_m_walk").getAsFloat();
    }

    return fAccuracy;
}

/**
* Returns the lowest possible accuracy (aim) of this weapon, based only on player's pose.
* Note: I use the term "weapon accuracy" interchangeably with "aim".
* 
* @return The lowest possible accuracy (aim) of this weapon, based only on player's pose
*/
float Weapon::getLowestAccuracyByPose() const
{
    return getAccuracyByPose(true /* bMoving */, true /* bRun */, false /* bDuck */);
}

/**
* Returns the momentary recoil multiplier.
* The momentary recoil multiplier is always between 1.0 and CVAR recoil_m.
* When a shot is fired, the momentary recoil multiplier is set to CVAR recoil_m, and then is it linear decreased to 1.0
* over the duration of CVAR recoil_cooldown.
* This causes rapid firing less accurate than moderate firing with the same weapon.
* Note that CVAR recoil_m set to 1.0 means no recoil i.e. weapon accuracy (aim) is not affected by recoil.
* 
* @return The momentary recoil multiplier.
*/
float Weapon::getMomentaryRecoilMultiplier() const
{
    const float fRecoilMax = getMaximumRecoilMultiplier();
    if (fRecoilMax <= 1.f)
    {
        // fRecoilMax is minimized at 1.f by ctor but it is better to to have the condition that way
        return 1.f;
    }
    
    PFL::timeval timeNow;
    PFL::gettimeofday(&timeNow, 0);
    const TPureFloat fMillisecsSinceLastShot = PFL::getTimeDiffInUs(timeNow, m_timeLastShot) / 1000.f;
    const float fRecoilCooldownMillisecs = getVars().at("recoil_cooldown").getAsFloat();

    // ctor makes sure that recoil_cooldown is positive (bigger than firing_cooldown) if recoil_m is > 1.f, so
    // this assertion is implied from ctor behavior, thus we cannot divide by zero below!
    assert(fRecoilCooldownMillisecs > 0.f);

    if (fRecoilCooldownMillisecs <= fMillisecsSinceLastShot)
    {
        return 1.f;
    }

    return PFL::lerp(1.f, fRecoilMax, 1.f - (fMillisecsSinceLastShot / fRecoilCooldownMillisecs));
}

/**
* Returns the weapon's maximum recoil multiplier.
* This is basically value of CVAR recoil_m.
* The minimum recoil multiplier is always 1.f for any weapon.
* 
* @return The maximum recoil multiplier for this weapon.
*/
float Weapon::getMaximumRecoilMultiplier() const
{
    return getVars().at("recoil_m").getAsFloat();
}

/**
* Returns the calculated momentary accuracy (aim) based on all factors.
* The momentary accuracy depends on multiple factors:
*  - by-pose accuracy (aim), as returned by getAccuracyByPose();
*  - recoil multiplier, as returned by getMomentaryRecoilMultiplier().
*
* Note: I use the term "weapon accuracy" interchangeably with "aim".
* 
* @param bMoving Same as for getAccuracyByPose().
* @param bRun    Same as for getAccuracyByPose().
* @param bDuck   Same as for getAccuracyByPose().
*
* @return The calculated momentary accuracy (aim) based on all factors.
*/
float Weapon::getMomentaryAccuracy(bool bMoving, bool bRun, bool bDuck) const
{
    return getAccuracyByPose(bMoving, bRun, bDuck) * getMomentaryRecoilMultiplier();
}

/**
* Returns a positive relative bullet angle representing the lowest possible accuracy (aim) with this weapon.
* The higher is the absolute value of the relative bullet angle, the lower the accuracy (aim) is.
* 
* Note: I use the term "weapon accuracy" interchangeably with "aim".
* 
* @return A positive relative bullet angle representing the lowest possible accuracy (aim) with this weapon.
*/
float Weapon::getLowestAccuracyPossible() const
{
    return getLowestAccuracyByPose() * getMaximumRecoilMultiplier();
}

/**
* Returns a random relative Z angle for a newborn bullet.
* This relative Z angle is the difference of the Z angles of the weapon and the newborn bullet.
* The relative Z angle can be positive or negative but its absolute maximum value is the momentary accuracy (aim) (getMomentaryAccuracy()).
* 
* Note: I use the term "weapon accuracy" interchangeably with "aim".
* 
* @param bMoving Same as for getAccuracyByPose().
* @param bRun    Same as for getAccuracyByPose().
* @param bDuck   Same as for getAccuracyByPose().
*
* @return A random relative Z angle for a newborn bullet, in the [-getMomentaryAccuracy(), getMomentaryAccuracy()] range.
*/
float Weapon::getRandomRelativeBulletAngle(bool bMoving, bool bRun, bool bDuck) const
{
    const float fMomentaryAccuracy = getMomentaryAccuracy(bMoving, bRun, bDuck);

    return PFL::random(
        static_cast<int>(std::lroundf(-fMomentaryAccuracy * 100)),
        static_cast<int>(std::lroundf(fMomentaryAccuracy * 100))) / 100.f;
}

SoLoud::Wav& Weapon::getFiringSound()
{
    return m_sndShoot;
}

SoLoud::Wav& Weapon::getDryFiringSound()
{
    return m_sndShootDry;
}

SoLoud::Wav& Weapon::getReloadStartSound()
{
    return m_sndReloadStart;
}

SoLoud::Wav& Weapon::getReloadEndSound()
{
    return m_sndReloadEnd;
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


const std::vector<Weapon::FiringModeEnumToStringPair> Weapon::m_vecOrderOfFiringModes =
{
    {WPN_FM_SEMI, "semi"},
    {WPN_FM_BURST, "burst"},
    {WPN_FM_PROJ, "proj"},
    {WPN_FM_AUTO, "auto"}
};

std::set<std::string> Weapon::m_WpnAcceptedVars;


Weapon::Weapon() :
    PGEcfgFile(true, false),
    m_bullets(m_bullets),
    m_audio(m_audio),
    m_gfx(m_gfx),
    m_connHandle(0),
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


WeaponManager::WeaponManager(
    pge_audio::PgeAudio& audio,
    PGEcfgProfiles& cfgProfiles,
    PR00FsUltimateRenderingEngine& gfx,
    std::list<Bullet>& bullets) :
    m_audio(audio),
    m_cfgProfiles(cfgProfiles),
    m_gfx(gfx),
    m_pCurrentWpn(nullptr),
    m_bullets(bullets)
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

/**
    Returns the only instance of KeypressToWeaponMap.
    This map allows the user to define keyboard key mapping to weapons.
    Currently it is still the user's responsibility to handle these keypresses in game code, however
    this map is also used by WeaponManager's getPrev/NextWeapon() functions since this map
    defines the desired logical order of the weapons.
    Note that this logical order is independent from the power order which is based on the DPFR value of weapons.
*/
WeaponManager::KeypressToWeaponMap& WeaponManager::getKeypressToWeaponMap()
{
    return m_mapKeypressToWeapon;
}

/**
* Loads the given weapon file and returns the created Weapon instance.
* 
* @param fname                Weapon file name.
* @param connHandleServerSide The server-side connection handle of the owner of the loaded weapon.
*                             Basically this connects the weapon to the user of the weapon.
* @return Nullptr on error loading the file, otherwise pointer to the created Weapon instance.
*/
Weapon* WeaponManager::load(const char* fname, pge_network::PgeNetworkConnectionHandle connHandleServerSide)
{
    try
    {
        Weapon* const wpn = new Weapon(fname, m_bullets, m_audio, m_gfx, connHandleServerSide);
        if (!wpn)
        {
            return nullptr;
        }

        m_weapons.push_back(wpn);
        wpn->getObject3D().SetName(wpn->getObject3D().getName() + " (WeaponManager-loaded " + fname + ")");
        return wpn;
    }
    catch (const std::exception& e)
    {
        getConsole().EOLn("WeaponManager::load(%s) failed due to exception: %s", fname, e.what());
        return nullptr;
    }
}

const std::vector<Weapon*>& WeaponManager::getWeapons() const
{
    return m_weapons;
}

Weapon* WeaponManager::getWeaponByFilename(const std::string& wpnName)
{    
    return const_cast<Weapon*>(const_cast<const WeaponManager*>(this)->getWeaponByFilename(wpnName));
}

const Weapon* WeaponManager::getWeaponByFilename(const std::string& wpnName) const
{
    const auto it = std::find_if(
        m_weapons.begin(),
        m_weapons.end(),
        [&wpnName](const Weapon* pwpn) { return pwpn && (pwpn->getFilename() == wpnName); }
    );

    return it == m_weapons.end() ? nullptr : *it;
}

/**
* @return Default available weapon that was previously set by setDefaultAvailableWeaponByFilename().
*/
const std::string& WeaponManager::getDefaultAvailableWeaponFilename() const
{
    return m_sDefaultAvailableWeapon;
}

/**
* Sets default available weapon that should be by default available for any new/respawning player.
* Note that WeaponManager doesn't know about any player entity, you need to connect this logic with your player entity.
*/
bool WeaponManager::setDefaultAvailableWeaponByFilename(const std::string& sFilename)
{
    Weapon* wpn = getWeaponByFilename(sFilename);
    if (wpn)
    {
        m_sDefaultAvailableWeapon = sFilename;
        return true;
    }

    return false;
}

/**
* Gets the current weapon.
* Useful for a player object for tracking player's current weapon.
* Note that WeaponManager doesn't know about any player entity, you need to connect this logic with your player entity.
* 
* @return Current weapon. Can be null if setCurrentWeapon() was not yet called.
*/
const Weapon* WeaponManager::getCurrentWeapon() const
{
    return m_pCurrentWpn;
}

/**
* Gets the current weapon.
* Useful for a player object for tracking player's current weapon.
* Note that WeaponManager doesn't know about any player entity, you need to connect this logic with your player entity.
* 
* @return Current weapon. Can be null if setCurrentWeapon() was not yet called.
*/
Weapon* WeaponManager::getCurrentWeapon()
{
    return m_pCurrentWpn;
}

/**
* Sets the current weapon. 
* Useful for a player object for tracking player's current weapon.
* Note that WeaponManager doesn't know about any player entity, you need to connect this logic with your player entity.
* 
* @param wpn               The weapon you want to set to be current. Cannot be null.
* @param bRecordSwitchTime Set it to true if you want the timestamp of this change to be saved for later query by getTimeLastWeaponSwitch().
*                          Ignored if this is the very first time you invoke this function, since that won't be a real switch at all.
* @param bServer           Set it to true if we are server instance, false if we are client.
*                          Only server does availability check because clients might not be up-to-date about weapon availability.
* 
* @return True if current weapon has been set to the given weapon, false otherwise.
*/
bool WeaponManager::setCurrentWeapon(Weapon* wpn, bool bRecordSwitchTime, bool bServer)
{
    if (!wpn)
    {
        getConsole().EOLn("Player::%s(): CANNOT set nullptr!", __func__);
        return false;
    }

    if (bServer /* client should not do availability check since it is not aware of wpn availability of other players, only its own weapons! */ &&
        !wpn->isAvailable())
    {
        //getConsole().EOLn(
        //    "Player::%s(): wpn %s is NOT available!", __func__, wpn->getFilename().c_str());
        return false;
    }

    if (m_pCurrentWpn && (m_pCurrentWpn != wpn))
    {
        if (m_weapons.end() == std::find(m_weapons.begin(), m_weapons.end(), wpn))
        {
            getConsole().EOLn("WeaponManager::%s(): CANNOT HAPPEN: wpn %s is NOT owned by us!", __func__, wpn->getFilename().c_str());
            return false;
        }

        // we already have a current different weapon, so this will be a weapon switch
        m_pCurrentWpn->getObject3D().Hide();
        wpn->getObject3D().getAngleVec() = m_pCurrentWpn->getObject3D().getAngleVec();

        if (bRecordSwitchTime)
        {
            m_timeLastWeaponSwitch = std::chrono::steady_clock::now();
        }
    }
    wpn->getObject3D().Show();
    m_pCurrentWpn = wpn;

    return true;
}

/**
* Gets the previous available weapon.
* Typically you need this function when the user is browsing their weapons backwards e.g. scrolling mousewheel backward.
* 
* @param cTargetWeapon Output argument, the key associated to the previous available weapon in KeypressToWeaponMap will be set in this output argument.
*                      Will be set to '\0' (null-char) if no current weapon is set or the file name of the current weapon is not present in KeypressToWeaponMap.
*                      Will be set to the key of the current weapon if we could not find another available weapon.
* 
* @return The previous available weapon based on logical order defined by KeypressToWeaponMap.
*         Nullptr if there is no current weapon set or the file name of the current weapon is not present in KeypressToWeaponMap.
*         Same weapon as current weapon if we could not find another available weapon.
*         Note that owner of weapon is not taken into consideration, because the idea is that
*         each player entity has their own WeaponManager, storing only their owned weapons.
*/
Weapon* WeaponManager::getPrevAvailableWeapon(unsigned char& cTargetWeapon)
{
    return const_cast<Weapon*>(const_cast<const WeaponManager*>(this)->getPrevAvailableWeapon(cTargetWeapon));
}

/**
* Gets the previous available weapon.
* Typically you need this function when the user is browsing their weapons backwards e.g. scrolling mousewheel backward.
* 
* @param cTargetWeapon Output argument, the key associated to the previous available weapon in KeypressToWeaponMap will be set in this output argument.
*                      Will be set to '\0' (null-char) if no current weapon is set or the file name of the current weapon is not present in KeypressToWeaponMap.
*                      Will be set to the key of the current weapon if we could not find another available weapon.
* 
* @return The previous available weapon based on logical order defined by KeypressToWeaponMap.
*         Nullptr if there is no current weapon set or the file name of the current weapon is not present in KeypressToWeaponMap.
*         Same weapon as current weapon if we could not find another available weapon.
*         Note that owner of weapon is not taken into consideration, because the idea is that
*         each player entity has their own WeaponManager, storing only their owned weapons.
*/
const Weapon* WeaponManager::getPrevAvailableWeapon(unsigned char& cTargetWeapon) const
{
    const Weapon* pRetWeapon = getCurrentWeapon();
    cTargetWeapon = '\0';
    if (!pRetWeapon)
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): CurrentWeapon null!", __func__);
        return nullptr;
    }

    const auto itKeypressToWeaponMapStart = std::find_if(
        getKeypressToWeaponMap().begin(),
        getKeypressToWeaponMap().end(),
        [this](const auto& keyWpnPair) { return keyWpnPair.second == this->getCurrentWeapon()->getFilename(); });

    if (itKeypressToWeaponMapStart == getKeypressToWeaponMap().end())
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): could not set cCurrentWeaponKeyChar based on %s!",
            __func__, getCurrentWeapon()->getFilename().c_str());
        return nullptr;
    }

    const unsigned char cCurrentWeaponKeyChar = itKeypressToWeaponMapStart->first;
    cTargetWeapon = cCurrentWeaponKeyChar;
    auto itKeypressToWeaponMap = itKeypressToWeaponMapStart;
    // Now we have to decrement 'itKeypressToWeaponMap' until:
    // - we find an available weapon;
    // - we reach back to the current weapon.
    // This is very ugly as I have to iterate in a map back ... on the long run I will need
    // a double linked list or similar, so it is easier to go in the list until we end up at the starting element or find an available wpn.
    do
    {
        if (itKeypressToWeaponMap == getKeypressToWeaponMap().begin())
        {                           
            itKeypressToWeaponMap = --getKeypressToWeaponMap().end();
        }
        else
        {
            --itKeypressToWeaponMap;
        }
        const Weapon* const pTargetWeapon = getWeaponByFilename(itKeypressToWeaponMap->second);
        if (pTargetWeapon)
        {
            if (pTargetWeapon->isAvailable())
            {
                // we dont care about if bullets are loaded, if available then let this one be the target!
                cTargetWeapon = itKeypressToWeaponMap->first;
                pRetWeapon = pTargetWeapon;
                break;
            }
        }
        else
        {
            getConsole().EOLn("PRooFPSddPGE::%s(): SHOULD NOT HAPPEN: found a not loaded weapon file name in KeypressToWeaponMap: %s!",
                __func__, itKeypressToWeaponMap->second.c_str());
        }
    } while (itKeypressToWeaponMap != itKeypressToWeaponMapStart);

    return pRetWeapon;
}

/**
* Gets the next available weapon.
* Typically you need this function when the user is browsing their weapons forward e.g. scrolling mousewheel forward.
* 
* @param cTargetWeapon Output argument, the key associated to the next available weapon in KeypressToWeaponMap will be set in this output argument.
*                      Will be set to '\0' (null-char) if no current weapon is set or the file name of the current weapon is not present in KeypressToWeaponMap.
*                      Will be set to the key of the current weapon if we could not find another available weapon.
* 
* @return The next available weapon based on logical order defined by KeypressToWeaponMap.
*         Nullptr if there is no current weapon set or the file name of the current weapon is not present in KeypressToWeaponMap.
*         Same weapon as current weapon if we could not find another available weapon.
*         Note that owner of weapon is not taken into consideration, because the idea is that
*         each player entity has their own WeaponManager, storing only their owned weapons.
*/
Weapon* WeaponManager::getNextAvailableWeapon(unsigned char& cTargetWeapon)
{
    return const_cast<Weapon*>(const_cast<const WeaponManager*>(this)->getNextAvailableWeapon(cTargetWeapon));
}

/**
* Gets the next available weapon.
* Typically you need this function when the user is browsing their weapons forward e.g. scrolling mousewheel forward.
* 
* @param cTargetWeapon Output argument, the key associated to the next available weapon in KeypressToWeaponMap will be set in this output argument.
*                      Will be set to '\0' (null-char) if no current weapon is set or the file name of the current weapon is not present in KeypressToWeaponMap.
*                      Will be set to the key of the current weapon if we could not find another available weapon.
* 
* @return The next available weapon based on logical order defined by KeypressToWeaponMap.
*         Nullptr if there is no current weapon set or the file name of the current weapon is not present in KeypressToWeaponMap.
*         Same weapon as current weapon if we could not find another available weapon.
*         Note that owner of weapon is not taken into consideration, because the idea is that
*         each player entity has their own WeaponManager, storing only their owned weapons.
*/
const Weapon* WeaponManager::getNextAvailableWeapon(unsigned char& cTargetWeapon) const
{
    const Weapon* pRetWeapon = getCurrentWeapon();
    cTargetWeapon = '\0';
    if (!pRetWeapon)
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): CurrentWeapon null!", __func__);
        return nullptr;
    }

    const auto itKeypressToWeaponMapStart = std::find_if(
        getKeypressToWeaponMap().begin(),
        getKeypressToWeaponMap().end(),
        [this](const auto& keyWpnPair) { return keyWpnPair.second == this->getCurrentWeapon()->getFilename(); });

    if (itKeypressToWeaponMapStart == getKeypressToWeaponMap().end())
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): could not set cCurrentWeaponKeyChar based on %s!",
            __func__, getCurrentWeapon()->getFilename().c_str());
        return nullptr;
    }

    const unsigned char cCurrentWeaponKeyChar = itKeypressToWeaponMapStart->first;
    cTargetWeapon = cCurrentWeaponKeyChar;
    auto itKeypressToWeaponMap = itKeypressToWeaponMapStart;
    // Now we have to increment 'itKeypressToWeaponMap' until:
    // - we find an available weapon;
    // - we reach back to the current weapon.
    // This is very ugly as I have to iterate in a map forth ... on the long run I will need
    // a double linked list or similar, so it is easier to go in the list until we end up at the starting element or find an available wpn.
    do
    {
        ++itKeypressToWeaponMap;
        if (itKeypressToWeaponMap == getKeypressToWeaponMap().end())
        {
            itKeypressToWeaponMap = getKeypressToWeaponMap().begin();
        }
        const Weapon* const pTargetWeapon = getWeaponByFilename(itKeypressToWeaponMap->second);
        if (pTargetWeapon)
        {
            if (pTargetWeapon->isAvailable())
            {
                // we dont care about if bullets are loaded, if available then let this one be the target!
                cTargetWeapon = itKeypressToWeaponMap->first;
                pRetWeapon = pTargetWeapon;
                break;
            }
        }
        else
        {
            getConsole().EOLn("PRooFPSddPGE::%s(): SHOULD NOT HAPPEN: found a not loaded weapon file name in KeypressToWeaponMap: %s!",
                __func__, itKeypressToWeaponMap->second.c_str());
        }
    } while (itKeypressToWeaponMap != itKeypressToWeaponMapStart);

    return pRetWeapon;
}

/**
* Gets the next best available weapon.
* Typically you need this when you run out of ammo of your current weapon and want to switch to another most useful weapon.
* Unlike the getPrev/NextAvailableWeapon() functions, here we follow the power order of available weapons defined by their DPSR value, and
* we don't consider weapons for which we don't have any kind (mag and unmag) ammo.
* 
* @param cTargetWeapon      Output-only argument, the key associated to the next best available weapon.
*                           Set to '\0' in case of error.
*                           Set to key associated to current weapon if we could not find a best next available weapon.
*                           Set to key associated to the found next best available weapon if we found one.
* @param bMustHaveMagBullet If true, only weapons with non-zero mag bullet count will be considered.
*
* @return The next best available weapon based on power order defined by DPSR value of weapons.
*         Nullptr if there is no current weapon set or the file name of the next best available weapon is not present in KeypressToWeaponMap.
*         Same weapon as current weapon if we could not find a best next available weapon.
*         Note that owner of weapon is not taken into consideration, because the idea is that
*         each player entity has their own WeaponManager, storing only their owned weapons.
*/
Weapon* WeaponManager::getNextBestAvailableWeapon(unsigned char& cTargetWeapon, bool bMustHaveMagBullet)
{
    return const_cast<Weapon*>(const_cast<const WeaponManager*>(this)->getNextBestAvailableWeapon(cTargetWeapon, bMustHaveMagBullet));
}

/**
* Gets the next best available weapon.
* Typically you need this when you run out of ammo of your current weapon and want to switch to another most useful weapon.
* Unlike the getPrev/NextAvailableWeapon() functions, here we follow the power order of available weapons defined by their DPSR value, and
* we don't consider weapons for which we don't have any kind (mag and unmag) ammo.
*
* @param cTargetWeapon      Output-only argument, the key associated to the next best available weapon.
*                           Set to '\0' in case of error.
*                           Set to key associated to current weapon if we could not find a best next available weapon.
*                           Set to key associated to the found next best available weapon if we found one.
* @param bMustHaveMagBullet If true, only weapons with non-zero mag bullet count will be considered.
*
* @return The next best available weapon based on power order defined by DPSR value of weapons.
*         Nullptr if there is no current weapon set or the file name of the next best available weapon is not present in KeypressToWeaponMap.
*         Same weapon as current weapon if we could not find a best next available weapon.
*         Note that owner of weapon is not taken into consideration, because the idea is that
*         each player entity has their own WeaponManager, storing only their owned weapons.
*/
const Weapon* WeaponManager::getNextBestAvailableWeapon(unsigned char& cTargetWeapon, bool bMustHaveMagBullet) const
{
    const Weapon* pRetWeapon = getCurrentWeapon();
    cTargetWeapon = '\0';
    if (!pRetWeapon)
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): CurrentWeapon null!", __func__);
        return nullptr;
    }

    const auto itKeypressToWeaponMapCurrentWeapon = std::find_if(
        getKeypressToWeaponMap().begin(),
        getKeypressToWeaponMap().end(),
        [this](const auto& keyWpnPair) { return keyWpnPair.second == this->getCurrentWeapon()->getFilename(); });

    if (itKeypressToWeaponMapCurrentWeapon == getKeypressToWeaponMap().end())
    {
        getConsole().EOLn("PRooFPSddPGE::%s(): could not set cCurrentWeaponKeyChar based on %s!",
            __func__, getCurrentWeapon()->getFilename().c_str());
        return nullptr;
    }

    cTargetWeapon = itKeypressToWeaponMapCurrentWeapon->first;

    float fMaxDpsr = 0.f;
    for (const auto& keyWpnPair : getKeypressToWeaponMap())
    {
        const Weapon* const pTargetWeapon = getWeaponByFilename(keyWpnPair.second);
        if (pTargetWeapon)
        {
            if ((pTargetWeapon != getCurrentWeapon()) &&
                 pTargetWeapon->isAvailable() &&
                (pTargetWeapon->getDamagePerSecondRating() > fMaxDpsr) &&
                ((pTargetWeapon->getMagBulletCount() > 0) || (pTargetWeapon->getUnmagBulletCount() > 0)))
            {
                if (bMustHaveMagBullet && (pTargetWeapon->getMagBulletCount() == 0))
                {
                    continue;
                }

                cTargetWeapon = keyWpnPair.first;
                pRetWeapon = pTargetWeapon;
                fMaxDpsr = pRetWeapon->getDamagePerSecondRating();
            }
        }
        else
        {
            getConsole().EOLn("PRooFPSddPGE::%s(): SHOULD NOT HAPPEN: found a not loaded weapon file name in KeypressToWeaponMap: %s!",
                __func__, keyWpnPair.second.c_str());
        }
    }

    return pRetWeapon;
}

/**
* @return Timestamp of last successful setCurrentWeapon() with bRecordSwitchTime as true.
*/
const std::chrono::time_point<std::chrono::steady_clock>& WeaponManager::getTimeLastWeaponSwitch() const
{
    return m_timeLastWeaponSwitch;
}

/**
* Deletes all loaded weapons, sets current weapon to null and clears default available weapon and last weapon switch timestamp.
*/
void WeaponManager::Clear()
{
    for (auto& pWpn : m_weapons)
    {
        delete pWpn;
    }
    m_weapons.clear();
    m_sDefaultAvailableWeapon.clear();
    m_pCurrentWpn = nullptr;
    m_timeLastWeaponSwitch = {};
}

/**
* @return Reference to the list storing bullets created by firing any weapon managed by this WeaponManager instance.
*/
std::list<Bullet>& WeaponManager::getBullets()
{
    return m_bullets;
}



// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


WeaponManager::KeypressToWeaponMap WeaponManager::m_mapKeypressToWeapon;
