/*
    ###################################################################################
    WeaponManager.cpp
    This file is part of PGE.
    PR00F's Game Engine Weapon Manager
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
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

void Bullet::ResetGlobalBulletId()
{
    m_globalBulletId = 0;
}

Bullet::Bullet(
    PR00FsUltimateRenderingEngine& gfx,
    pge_network::PgeNetworkConnectionHandle connHandle,
    TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
    TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
    TPureFloat sx, TPureFloat sy, TPureFloat /*sz*/,
    TPureFloat speed, TPureFloat gravity, TPureFloat drag, TPureBool fragile, int nDamageHp) :
    m_id(m_globalBulletId++),
    m_gfx(gfx),
    m_connHandle(connHandle),
    m_speed(speed),
    m_gravity(gravity),
    m_drag(drag),
    m_fragile(fragile),
    m_nDamageHp(nDamageHp),
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
    PR00FsUltimateRenderingEngine& gfx,
    Bullet::BulletId id,
    TPureFloat wpn_px, TPureFloat wpn_py, TPureFloat wpn_pz,
    TPureFloat wpn_ax, TPureFloat wpn_ay, TPureFloat wpn_az,
    TPureFloat sx, TPureFloat sy, TPureFloat /*sz*/) :
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

int Bullet::getDamageHp() const
{
    return m_nDamageHp;
}

void Bullet::Update()
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
    m_put.Move(m_speed);
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


Weapon::Weapon(const char* fname, std::list<Bullet>& bullets, PR00FsUltimateRenderingEngine& gfx, pge_network::PgeNetworkConnectionHandle connHandle) :
    PGEcfgFile(true, false),
    m_bullets(bullets),
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
void Weapon::UpdatePosition(const PureVector& playerPos)
{
    getObject3D().getPosVec().Set(playerPos.getX(), playerPos.getY(), playerPos.getZ());
}

/**
 * Updates the graphical object entity associated to this weapon object.
 * The position and angles are updated to the specified values.
 * This function can be used to update the weapon of other clients on our side.
 */
void Weapon::UpdatePositions(const PureVector& playerPos, TPureFloat fAngleY, TPureFloat fAngleZ)
{
    getObject3D().getPosVec().Set(playerPos.getX(), playerPos.getY(), playerPos.getZ());
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
 * @param How many bullets we want to add. The actual increase value might be less. See more details at canIncBulletCount(). 
 */
void Weapon::IncBulletCount(TPureUInt count)
{
    if (getVars()["reloadable"].getAsInt() > 0)
    {
        m_nUnmagBulletCount = min(static_cast<TPureUInt>(getVars()["cap_max"].getAsInt()), (m_nUnmagBulletCount + count));
    }
    else
    {
        // not reloadable has always zero m_nUnmagBulletCount, e.g. rail gun
        m_nMagBulletCount = min(static_cast<TPureUInt>(getVars()["cap_max"].getAsInt()), (m_nMagBulletCount + count));
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
Weapon::State Weapon::getState() const
{
    return m_state;
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
        m_nBulletsToReload = min(nCapMagazine, m_nUnmagBulletCount);
    }
    else
    {
        m_nBulletsToReload = min(nCapMagazine - m_nMagBulletCount, m_nUnmagBulletCount);
    }
    
    PFL::gettimeofday(&m_timeReloadStarted, 0);

    return true;
}

/**
 * Pulls the trigger of the weapon, which might result in shooting the weapon.
 * Firing an actual shot depends on different things like current state of the weapon, firing mode, etc.
 * If a shot was actually fired, at least one new bullet is placed in 'bullets' specified in the constructor of this weapon.
 * 
 * @return True if a shot was actually triggered, false otherwise.
 */
TPureBool Weapon::pullTrigger()
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
    
    m_bullets.push_back(
        Bullet(
            m_gfx,
            m_connHandle,
            m_obj->getPosVec().getX(), m_obj->getPosVec().getY(), m_obj->getPosVec().getZ(),
            m_obj->getAngleVec().getX(), m_obj->getAngleVec().getY(), m_obj->getAngleVec().getZ(),
            getVars()["bullet_size_x"].getAsFloat(),
            getVars()["bullet_size_y"].getAsFloat(),
            getVars()["bullet_size_z"].getAsFloat(),
            getVars()["bullet_speed"].getAsFloat(),
            getVars()["bullet_gravity"].getAsFloat(),
            getVars()["bullet_drag"].getAsFloat(),
            getVars()["bullet_fragile"].getAsBool(),
            getVars()["damage_hp"].getAsInt())
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

    // it doesnt matter if weapon is reloadable or not, the loaded bullet count is in nMagBulletCount
    m_nMagBulletCount = getVars()["bullets_default"].getAsInt();
    m_nUnmagBulletCount = 0;
    m_bAvailable = false;
    m_bTriggerReleased = true;
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


WeaponManager::WeaponManager(PR00FsUltimateRenderingEngine& gfx) :
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

bool WeaponManager::load(const char* fname, pge_network::PgeNetworkConnectionHandle connHandleServerSide)
{
    try
    {
        Weapon* const wpn = new Weapon(fname, m_bullets, m_gfx, connHandleServerSide);
        if (!wpn)
        {
            return false;
        }

        m_weapons.push_back(wpn);
        return true;
    }
    catch (const std::exception& e)
    {
        getConsole().EOLn("WeaponManager::load(%s) failed due to exception: %s", fname, e.what());
        return false;
    }
}

std::vector<Weapon*>& WeaponManager::getWeapons()
{
    return m_weapons;
}

Weapon* WeaponManager::getWeaponByFilename(const std::string& wpnName)
{    
    for (const auto pWpn : m_weapons)
    {
        if (pWpn)
        {
            if (pWpn->getFilename() == wpnName)
            {
                return pWpn;
            }
        }
    }

    return nullptr;
}

const std::string& WeaponManager::getDefaultAvailableWeaponFilename() const
{
    return m_sDefaultAvailableWeapon;
}

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

void WeaponManager::Clear()
{
    for (auto& pWpn : m_weapons)
    {
        delete pWpn;
    }
    m_weapons.clear();
    m_bullets.clear();
    m_sDefaultAvailableWeapon.clear();
}

std::list<Bullet>& WeaponManager::getBullets()
{
    return m_bullets;
}



// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


