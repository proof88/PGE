#pragma once

/*
    ###################################################################################
    PGE.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine main class
    Made by PR00F88
    ###################################################################################
*/

#include "PGEallHeaders.h"

#include <list>
#include <string>

#include "../../Console/CConsole/src/CConsole.h"

// TODO: add "Audio/soloud-RELEASE_20200207/include/" to project include dirs, do not use that path here!
#include "Audio/soloud-RELEASE_20200207/include/soloud.h"
#include "Audio/soloud-RELEASE_20200207/include/soloud_wav.h"
#include "Audio/PgeAudio.h"

#include "Network/PgeNetwork.h"

#include "PURE/include/external/PR00FsUltimateRenderingEngine.h"

#include "Weapons/WeaponManager.h"

#include "PGEInputHandler.h"
#include "PGEWorld.h"


/**
    Message IDs for multilingual support.
*/
enum PGE_MSG_ID
{
    PGE_MSG_ERR_INIT_GFX,
    PGE_MSG_ERR_INIT_SFX,
    PGE_MSG_ERR_INIT_NET,
    PGE_MSG_ERR_EXIT_GFX,
    PGE_MSG_ERR_EXIT_SFX,
    PGE_MSG_ERR_EXIT_NET,
    PGE_MSG_TITLE_ERR,
    PGE_MSG_TITLE_WARN,
    PGE_MSG_TITLE_INFO
}; // enum PGE_MSG_ID


/**
    PR00F's Game Engine main class.
*/
class PGE
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGE is included")   
#endif

public:

    static constexpr char* CVAR_GFX_WINDOWED = "gfx_windowed";  // TODO: Move this into PURE

    static const char* getVersionString();

    static PGE* createAndGet(const char* gameTitle);  /**< Creates and gets the only singleton instance. */

    static int  showErrorDialog(const char* msg);     /**< Shows an error dialog box. */
    static int  showErrorDialog(PGE_MSG_ID msg_id);   /**< Shows an error dialog box. */
    static int  showAlertDialog(const char* msg);     /**< Shows an alert dialog box. */
    static int  showAlertDialog(PGE_MSG_ID msg_id);   /**< Shows an alert dialog box. */
    static int  showInfoDialog(const char* msg);      /**< Shows an info dialog box. */
    static int  showInfoDialog(PGE_MSG_ID msg_id);    /**< Shows an info dialog box. */

    static const char* getLoggerModuleName();         /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    const std::string& getGameTitle() const;                 /**< Gets the game title. */
    void               SetGameTitle(const char* gameTitle);  /**< Sets the game title. */

    int  getWaitingWhileInactive() const;      /**< Gets the time runGame() waits between each refresh while the app is inactive. */
    void SetWaitingWhileInactive(int msecs);   /**< Sets the time runGame() waits between each refresh while the app is inactive. */
    bool isInactiveLikeActive() const;         /**< Gets whether runGame() acts the same way in inactive state as in active state. */
    void SetInactiveLikeActive(bool value);    /**< Sets how runGame() should act in inactive state. */

    void setCookie(int cookie);                /**< Sets a special purpose value for arbitrary use. */
    int getCookie() const;                     /**< Gets a previously set special purpose value for arbitrary use. */

    pge_audio::PgeAudio& getAudio();                 /**< Returns audio lib interface. */
    PGEcfgProfiles& getConfigProfiles() const;       /**< Returns the config handler object. */
    PGEInputHandler& getInput() const;               /**< Returns the input handler object. */
    pge_network::PgeINetwork& getNetwork() const;    /**< Returns the network functionality interface. */
    PR00FsUltimateRenderingEngine& getPure() const;  /**< Returns the graphics engine. */
    PGEWorld& getWorld() const;                      /**< Returns the world object. */
    
    PgeObjectPool<PooledBullet>& getBullets();       /**< Returns the bullets simulated by the engine. */

    int  initializeGame(const char* szCmdLine);  /**< Initializes the game engine. */
    int  runGame();                              /**< Runs the game. */
    bool isGameRunning() const;                  /**< Gets the running state of the game. */
    int  destroyGame();                          /**< Destroys the game engine. */

    unsigned int getGameRunningFrequency() const;      /**< Gets the frequency for the main game engine loop. */
    void setGameRunningFrequency(unsigned int freq);   /**< Sets the frequency for the main game engine loop. */

    unsigned int getRenderExtraDelayMillisecs() const;
    void setRenderExtraDelayMillisecs(unsigned int nMillisecs);

protected:
    
    // ---------------------------------------------------------------------------

    PGE();                          /**< Kept for easier virtual inheritance by application, but not actually used. */
    PGE(const char* gametitle);     /**< This is the only usable ctor, this is used by the static createAndGet(). */
    virtual ~PGE();

    PGE(const PGE&) = delete;
    PGE& operator=(const PGE&) = delete;
    PGE(PGE&&) = delete;
    PGE& operator=(PGE&&) = delete;

    // Event handlers to be overridden.
    virtual bool onGameInitializing() { return true; }  /**< Called before initializing the engine. */
    virtual bool onGameInitialized() { return true; }   /**< Called after initializing the engine. */
    virtual void onGameFrameBegin() {}    /**< Called at the beginning of a new frame. */
    virtual void onGameRunning() {}       /**< Called while running the engine. */
    virtual bool onPacketReceived(
        const pge_network::PgePacket&) {
        return true; 
    }                                     /**< Called when a new network packet is received. 
                                               @return True on success, false on serious error that should result in terminating the application. */
    virtual void onGameDestroying() {}    /**< Called before stopping the engine. */
    virtual void onGameDestroyed() {}     /**< Called after stopping the engine. */

private:

    class PGEimpl;
    PGEimpl* p;

}; // class PGE

