#pragma once

/*
    ###################################################################################
    PGE.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine main class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"
#include <string>
#include "../../CConsole/CConsole/src/CConsole.h"
#include "PRRE/include/external/PR00FsReducedRenderingEngine.h"
#include "PGEInputHandler.h"
#include "PGEWorld.h"
#include "Network/PgeNetwork.h"
#include "Weapons/WeaponManager.h"


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

    static PGE* createAndGet(const char* gameTitle);  /**< Creates and gets the only singleton instance. */

    static int  showErrorDialog(const char* msg);     /**< Shows an error dialog box. */
    static int  showErrorDialog(PGE_MSG_ID msg_id);   /**< Shows an error dialog box. */
    static int  showAlertDialog(const char* msg);     /**< Shows an alert dialog box. */
    static int  showAlertDialog(PGE_MSG_ID msg_id);   /**< Shows an alert dialog box. */
    static int  showInfoDialog(const char* msg);      /**< Shows an info dialog box. */
    static int  showInfoDialog(PGE_MSG_ID msg_id);    /**< Shows an info dialog box. */

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    const std::string& getGameTitle() const;                 /**< Gets the game title. */
    void               SetGameTitle(const char* gameTitle);  /**< Sets the game title. */

    int  getWaitingWhileInactive() const;      /**< Gets the time runGame() waits between each refresh while the app is inactive. */
    void SetWaitingWhileInactive(int msecs);   /**< Sets the time runGame() waits between each refresh while the app is inactive. */
    bool isInactiveLikeActive() const;         /**< Gets whether runGame() acts the same way in inactive state as in active state. */
    void SetInactiveLikeActive(bool value);    /**< Sets how runGame() should act in inactive state. */

    PGEInputHandler& getInput() const;         /**< Returns the input handler object. */
    PGEWorld& getWorld() const;                /**< Returns the world object. */
    PR00FsReducedRenderingEngine& getPRRE() const;   /**< Returns the graphics engine. */
    PgeNetwork& getNetwork() const;            /**< Returns the network functionality interface. */
    
    WeaponManager& getWeaponManager();         /**< Returns the weapon manager object. */

    int  initializeGame();                     /**< Initializes the game engine. */
    int  runGame();                            /**< Runs the game. */
    bool isGameRunning() const;                /**< Gets the running state of the game. */
    int  destroyGame();                        /**< Destroys the game engine. */

protected:
    
    // ---------------------------------------------------------------------------
    
    PGE();

    PGE(const PGE&); 
    PGE& operator=(const PGE&);

    PGE(const char* gametitle);     /**< This is the only usable ctor, this is used by the static createAndGet(). */
    virtual ~PGE();

    // Event handlers to be overridden.
    virtual void onGameInitializing() {}  /**< Called before initializing the engine. */
    virtual void onGameInitialized() {}   /**< Called after initializing the engine. */
    virtual void onGameRunning() {}       /**< Called while running the engine. */
    virtual void onPacketReceived(
        uint32_t,
        const PgePkt::PgePacket&) {}      /**< Called when a new network packet is received. */
    virtual void onGameDestroying() {}    /**< Called before stopping the engine. */
    virtual void onGameDestroyed() {}     /**< Called after stopping the engine. */

private:

    class PGEimpl;
    PGEimpl* p;

}; // class PGE

