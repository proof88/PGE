/*
    ###################################################################################
    PGE.cpp
    This file is part of PGE.
    PR00F's Game Engine main class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

//#include "PRRE/PRREbaseIncludes.h"  // PCH
#include "PRREbaseIncludes.h"
#include "PGE.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"
// Subsystems
#include "PGESysGFX.h"
#include "PGESysSFX.h"
#include "PGESysNET.h"
#include "PGESysCFG.h"
#include "Weapons/WeaponManager.h"

#include "PRRE/include/external/Display/PRREScreen.h"
#include "PRRE/include/external/Display/PRREWindow.h"

using namespace std;

/*
   PGE::PGEimpl
   ###########################################################################
*/

class PGE::PGEimpl
{

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PGEimpl();

    CConsole& getConsole() const;    

    const std::string& getGameTitle() const;               
    void               SetGameTitle(const char* gameTitle); 

    int  getWaitingWhileInactive() const;     
    void SetWaitingWhileInactive(int msecs); 
    bool isInactiveLikeActive() const;       
    void SetInactiveLikeActive(bool value);  

    PGEInputHandler& getInput() const;       
    PGEWorld& getWorld() const;             
    PR00FsReducedRenderingEngine& getPRRE() const;
    //TODO later some other class needs to be returned here PGESysNET& getNetwork() const;
    WeaponManager& getWeaponManager();
                    
    bool isGameRunning() const;               
    int  destroyGame();                        

protected:

private:

    static std::string** pLangTable;      /**< Language-dependent strings. */
    static int           nLangTable;      /**< Length of pLangTable. */

    // ---------------------------------------------------------------------------

    static PGE_ENUM_LANG getLangFromMSG_ID(PGE_MSG_ID msg_id); /**< Transforms a message ID into a lang ID. */
    static int showWindowsMessageDialogWin32(
        const char* msg, const char* cpt, UINT type);          /**< Shows an error dialog box using WinAPI. */
    static int showWindowsMessageDialogWin32(
        PGE_MSG_ID msg_id, PGE_MSG_ID cpt_id, UINT type);      /**< Shows an error dialog box using WinAPI. */

    // ---------------------------------------------------------------------------

    PGE*      _pOwner;                 /**< The owner public object who creates this pimpl object. */

    PGESysCFG  SysCFG;
    PGESysGFX  SysGFX;
    PGESysNET  SysNET;
    PGESysSFX  SysSFX;
    
    PR00FsReducedRenderingEngine& GFX; 
    void* pSFX;
    PGEInputHandler& inputHandler;
    PGEWorld& world;
    WeaponManager wpnMgr;

    std::map<std::string, Player_t> m_mapPlayers;  // used by both server and clients

    bool        bIsGameRunning;        /**< Is the game running (true after successful init and before initiating shutdown)? */
    std::string sGameTitle;            /**< Simplified name of the game, used in paths too, so can't contain joker chars. */
    int         nInactiveSleep;        /**< Amount of sleep in millisecs when inactive, 0 means no sleep. */
    bool        bInactiveLikeActive;   /**< If true, runGame() will act the same way in inactive state as in active state. */

    // ---------------------------------------------------------------------------

    PGEimpl();

    PGEimpl(const PGEimpl&); 
    PGEimpl& operator=(const PGEimpl&);

    PGEimpl(const char* gametitle);


    friend class PGE;

}; 


// ############################### PUBLIC ################################


PGE::PGEimpl::~PGEimpl()
{
    _pOwner = NULL;
} // ~PGE()


CConsole& PGE::PGEimpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


const char* PGE::PGEimpl::getLoggerModuleName()
{
    return "PGE";
} // getLoggerModuleName()


const string& PGE::PGEimpl::getGameTitle() const
{
    return sGameTitle;
} // getGameTitle()


void PGE::PGEimpl::SetGameTitle(const char* gameTitle)
{
    if ( !bIsGameRunning )
    {
        sGameTitle = gameTitle;
    }
} // SetGameTitle()


int  PGE::PGEimpl::getWaitingWhileInactive() const
{
    return nInactiveSleep;    
} // getWaitingWhileInactive()


void PGE::PGEimpl::SetWaitingWhileInactive(int msecs)   
{
    if ( msecs < -1 )
        return;
    nInactiveSleep = msecs;
} // setWaitingWhileInactive()


bool PGE::PGEimpl::isInactiveLikeActive() const
{
    return bInactiveLikeActive;
} // isInactiveLikeActive()


void PGE::PGEimpl::SetInactiveLikeActive(bool value)
{
    bInactiveLikeActive = value;
} // setInactiveLikeActive()


PGEInputHandler& PGE::PGEimpl::getInput() const
{
    return inputHandler;
} // getInput()


PGEWorld& PGE::PGEimpl::getWorld() const
{
    return world;
} // getWorld()


PR00FsReducedRenderingEngine& PGE::PGEimpl::getPRRE() const
{
    return GFX;
}


//PGESysNET& PGE::PGEimpl::getNetwork()
//{
//    return SysNET;
//}


WeaponManager& PGE::PGEimpl::getWeaponManager()
{
    return wpnMgr;
}


bool PGE::PGEimpl::isGameRunning() const
{
    return bIsGameRunning;
} // isGameRunning()


int PGE::PGEimpl::destroyGame()
{
    // make sure that everything is destructed in REVERSE order compared to initializeGame()
    // first things to shutdown are instances that are NOT even initialized by initializeGame(), such as wpnMgr
    wpnMgr.Clear();
    world.Shutdown();
    // inputHandler doesnt have any shutdown
    SysGFX.destroySysGFX();
    SysSFX.destroySysSFX();
    SysNET.destroySysNET();
    // SysCFG doesnt have any shutdown

    getConsole().Deinitialize();

    return 0;
} // destroyGameEngine()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


string** PGE::PGEimpl::pLangTable = NULL;
int      PGE::PGEimpl::nLangTable = 0;

/**
    Transforms a message ID into a lang ID.
*/
PGE_ENUM_LANG PGE::PGEimpl::getLangFromMSG_ID(PGE_MSG_ID msg_id)
{
    switch (msg_id)
    {
    case PGE_MSG_ERR_INIT_GFX: return PGE_LANG_E001;
    case PGE_MSG_ERR_INIT_SFX: return PGE_LANG_E002;
    case PGE_MSG_ERR_INIT_NET: return PGE_LANG_E003;
    case PGE_MSG_ERR_EXIT_GFX: return PGE_LANG_E004;
    case PGE_MSG_ERR_EXIT_SFX: return PGE_LANG_E005;
    case PGE_MSG_ERR_EXIT_NET: return PGE_LANG_E006;
    case PGE_MSG_TITLE_ERR   : return PGE_LANG_TITLE_ERR;
    case PGE_MSG_TITLE_WARN  : return PGE_LANG_TITLE_WARN;
    case PGE_MSG_TITLE_INFO  : return PGE_LANG_TITLE_INFO;
    default                  : return PGE_LANG_E000;
    }
} // getLangFromMSG_ID()


/**
    Shows an error dialog box using WinAPI.
*/
int PGE::PGEimpl::showWindowsMessageDialogWin32(const char* msg, const char* cpt, UINT type)
{
    return ( MessageBox(0, msg, cpt, type) );
} // showWindowsMessageDialogWindows()


/**
    Shows an error dialog box using WinAPI.
*/
int PGE::PGEimpl::showWindowsMessageDialogWin32(PGE_MSG_ID msg_id, PGE_MSG_ID cpt_id, UINT type) 
{
    return ( MessageBox(0, pLangTable[getLangFromMSG_ID(msg_id)]->c_str(), pLangTable[getLangFromMSG_ID(cpt_id)]->c_str(), type) );
} // showWindowsMessageDialogWindows()


PGE::PGEimpl::PGEimpl() :
    inputHandler( PGEInputHandler::createAndGet() ),
    world( PGEWorld::createAndGet() ),
    GFX( PR00FsReducedRenderingEngine::createAndGet() ),
    SysCFG("") ,
    wpnMgr(GFX)
{
    _pOwner = NULL;  // currently not used
}


PGE::PGEimpl::PGEimpl(const PGE::PGEimpl&) :
    inputHandler( PGEInputHandler::createAndGet() ),
    world( PGEWorld::createAndGet() ),
    GFX( PR00FsReducedRenderingEngine::createAndGet() ),
    SysCFG(""),
    wpnMgr(GFX)
{
    _pOwner = NULL;  // currently not used
}  


PGE::PGEimpl& PGE::PGEimpl::operator=(const PGE::PGEimpl&)
{
    _pOwner = NULL;  // currently not used
    return *this;
}


/** 
    This is the only usable ctor, this is used by the static createAndGet().
*/
PGE::PGEimpl::PGEimpl(const char* gameTitle) :
    inputHandler( PGEInputHandler::createAndGet() ),
    world( PGEWorld::createAndGet() ),
    GFX( PR00FsReducedRenderingEngine::createAndGet() ),
    SysCFG(gameTitle),
    wpnMgr(GFX)
{
    _pOwner = NULL;  // currently not used
    sGameTitle = gameTitle;
    bIsGameRunning = false;
    nInactiveSleep = PGE_INACTIVE_SLEEP;
    bInactiveLikeActive = PGE_INACTIVE_AS_ACTIVE;
} // PGE(...)



/*
   PGE
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the only singleton instance.

    @return Singleton game engine instance.
*/
PGE* PGE::createAndGet(const char* gameTitle)
{
    static PGE pgeInstance(gameTitle);
    return &pgeInstance;
} // createAndGet()


/** 
    Shows an error dialog box.

    @param  msg The text to be showed.
    @return The return code of the OS-dependent dialog box display function. In case of Windows, this is MessageBox().
*/
int PGE::showErrorDialog(const char* msg)
{
    return ( PGEimpl::showWindowsMessageDialogWin32(msg, PGE_ERRORMSG_BASETITLE, MB_ICONERROR) );
} // showErrorDialog()


/** 
    Shows an error dialog box.

    @param  msg_id The message ID of the message to be displayed.
    @return The return code of the OS-dependent dialog box display function. In case of Windows, this is MessageBox().
*/
int PGE::showErrorDialog(PGE_MSG_ID msg_id)
{
    return ( PGEimpl::showWindowsMessageDialogWin32(msg_id, PGE_MSG_TITLE_ERR, MB_ICONERROR) );
} // showErrorDialog()


/**
    Shows an alert dialog box.

    @param  msg The text to be showed.
    @return The return code of the OS-dependent dialog box display function. In case of Windows, this is MessageBox().
*/
int PGE::showAlertDialog(const char* msg)
{
    return ( PGEimpl::showWindowsMessageDialogWin32(msg, PGE_WARNINGMSG_BASETITLE, MB_ICONEXCLAMATION) );
} // showAlertDialog()


/**
    Shows an alert dialog box.

    @param  msg_id The message ID of the message to be displayed.
    @return The return code of the OS-dependent dialog box display function. In case of Windows, this is MessageBox().
*/
int PGE::showAlertDialog(PGE_MSG_ID msg_id)
{
    return ( PGEimpl::showWindowsMessageDialogWin32(msg_id, PGE_MSG_TITLE_WARN, MB_ICONEXCLAMATION) );
} // showAlertDialog()


/**
    Shows an info dialog box.

    @param  msg The text to be showed.
    @return The return code of the OS-dependent dialog box display function. In case of Windows, this is MessageBox().
*/
int PGE::showInfoDialog(const char* msg)
{
    return ( PGEimpl::showWindowsMessageDialogWin32(msg, PGE_INFOMSG_BASETITLE, MB_ICONINFORMATION) );
} // showInfoDialog()


/**
    Shows an info dialog box.

    @param  msg_id The message ID of the message to be displayed.
    @return The return code of the OS-dependent dialog box display function. In case of Windows, this is MessageBox().
*/
int PGE::showInfoDialog(PGE_MSG_ID msg_id)
{
    return ( PGEimpl::showWindowsMessageDialogWin32(msg_id, PGE_MSG_TITLE_INFO, MB_ICONINFORMATION) );
} // showInfoDialog()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by the game engine.
*/
CConsole& PGE::getConsole() const
{
    return p->getConsole();
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PGE::getLoggerModuleName()
{
    return PGE::PGEimpl::getLoggerModuleName();
} // getLoggerModuleName()


/**
    Gets the game title.

    @return Game title.
*/
const string& PGE::getGameTitle() const
{
    return p->getGameTitle();
} // getGameTitle()


/**
    Sets the game title.

    @param gameTitle The desired game title.
*/
void PGE::SetGameTitle(const char* gameTitle)
{
    p->SetGameTitle(gameTitle);
} // SetGameTitle()


/**
    Gets the time runGame() waits between each refresh while the app is inactive.

    @return Amount of sleep time in milliseconds between each refresh while the main window is inactive.
*/
int  PGE::getWaitingWhileInactive() const
{
    return p->getWaitingWhileInactive();    
} // getWaitingWhileInactive()


/**
    Sets the time runGame() waits between each refresh while the app is inactive.

    @param msecs Amount of sleep time in milliseconds between each refresh while the main window is inactive. Negative value is ignored.
*/
void PGE::SetWaitingWhileInactive(int msecs)   
{
    p->SetWaitingWhileInactive(msecs);
} // SetWaitingWhileInactive()


/**
    Gets whether runGame() acts the same way in inactive state as in active state.

    @return True if the same sleep time is used between each refresh when the window is inactive as when it is active, false otherwise.
*/
bool PGE::isInactiveLikeActive() const
{
    return p->isInactiveLikeActive();
} // isInactiveLikeActive()


/**
    Sets how runGame() should act in inactive state.

    @param value Specify true if you wish the same sleep time between each refresh when the window is inactive.
                 Specify false if you wish to have sleeps specified by SetWaitingWhileInactive() between each refresh.
*/
void PGE::SetInactiveLikeActive(bool value)
{
    p->SetInactiveLikeActive(value);
} // SetInactiveLikeActive()


/**
    Returns the input handler object.

    @return Game engine input handler.
*/
PGEInputHandler& PGE::getInput() const
{
    return p->getInput();
} // getInput()


/**
    Returns the world object.

    @return World simulated by the game engine.
*/
PGEWorld& PGE::getWorld() const
{
    return p->getWorld();
} // getWorld()


/**
    Returns the graphics engine.

    @return Graphics engine.
*/
PR00FsReducedRenderingEngine& PGE::getPRRE() const
{
    return p->getPRRE();
}

bool PGE::isServer() const
{
    return p->SysNET.isServer();
}


//PGESysNET& PGE::getNetwork() const
//{
//    return p->getNetwork();
//}


bool PGE::ConnectClient()
{
    return p->SysNET.ConnectClient();
}

void PGE::SendStringToClient(const char* str)
{
    p->SysNET.SendStringToClient(p->SysNET.m_hConnection, str);
}

void PGE::SendPacketToClient(const PgePacket& pkt)
{
    p->SysNET.SendPacketToClient(p->SysNET.m_hConnection, pkt);
}

void PGE::SendStringToAllClients(const char* str)
{
    p->SysNET.SendStringToAllClients(str);
}

void PGE::SendPacketToAllClients(const PgePacket& pkt)
{
    p->SysNET.SendPacketToAllClients(pkt);
}

std::deque<PgePacket>& PGE::getPacketQueue()
{
    return p->SysNET.queuePackets;
}

std::map<std::string, Player_t>& PGE::getPlayers()
{
    return p->m_mapPlayers;
}

/**
    Returns the weapon manager object.
*/
WeaponManager& PGE::getWeaponManager()
{
    return p->getWeaponManager();
}


/**
    Initializes the game engine.

    @return 0 on success, 1 on failure.
*/
int PGE::initializeGame()
{

#ifdef PGE_CCONSOLE_IS_ENABLED
    getConsole().Initialize("PGE log", true);
    getConsole().SetLoggingState(getLoggerModuleName(), true);
    getConsole().SetFGColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, "999999" );
    getConsole().SetIntsColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "FFFF00" );
    getConsole().SetStringsColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, "FFFFFF" );
    getConsole().SetFloatsColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "FFFF00" );
    getConsole().SetBoolsColor( FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "00FFFF" );
#endif

    getConsole().OLn("PGE::initializeGame()");
    if ( p->bIsGameRunning )
    {
        getConsole().OLn("  already initialized!");
        return 0;
    }
    getConsole().OI();
    getConsole().OLn(PGE_NAME);
    getConsole().OLn(PGE_VERSION);
    onGameInitializing();
    getConsole().OLn("Game Title: %s", p->sGameTitle.c_str());
    getConsole().OLn("Documents Folder: %s", p->SysCFG.getMyDocsFolder().c_str());
    p->nLangTable = p->SysCFG.readLanguageData( p->pLangTable );
    getConsole().OLn("Lang Table with %d rows from %s.", p->nLangTable, p->SysCFG.getLangFileName().c_str());
    if ( p->nLangTable == 0 )
    {
        getConsole().EOLnOO("ERROR: Failed to read language data, exiting!");
        return 99;
    }
    getConsole().OLn("Profiles stored in Documents: %b", p->SysCFG.areProfilesInMyDocs());
    getConsole().OLn("Profiles: %s", p->SysCFG.getPathToProfiles().c_str());
    getConsole().OIOLn("Count: %d", p->SysCFG.getProfilesCount());
    for (int i = 0; i < p->SysCFG.getProfilesCount(); i++)
    {
        getConsole().OLn("%s.cfg ~ %s", p->SysCFG.getProfilesList()[i]->c_str(), p->SysCFG.getProfilePlayersList()[i]->c_str());
    }
    getConsole().OO();

    getConsole().L();
    getConsole().OLnOI("Initializing Networking ...");
    if (!(p->SysNET.initSysNET()))
    {
        getConsole().EOLnOO("Failed!");
        getConsole().OLn("");
        /*return 1;*/
    }
    else
    {
        getConsole().SOLnOO("Done!");
        getConsole().OLn("");
    } 
    getConsole().L();
        
    getConsole().L();
    getConsole().OLnOI("Initializing Audio ...");
    if ( !(p->SysSFX.initSysSFX()) )
    {
        getConsole().EOLnOO("Failed!");
        getConsole().OLn("");
        /*return 1;*/
    }
    else
    {
        getConsole().SOLnOO("Done!");
        getConsole().OLn("");
    }
    getConsole().L();

    getConsole().L();
    getConsole().OLnOI("Initializing Graphics ...");
    bool bGFXinit;
    /*if ( MessageBox(0, "Teljes képernyõ?", ":)", MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND) == IDYES )
        bGFXinit = p->SysGFX.initSysGFX(0, 0, PRRE_FULLSCREEN, 0, 32, 24, 0, 0);
    else*/
        bGFXinit = p->SysGFX.initSysGFX(800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);

    if ( !bGFXinit )
    {
        getConsole().EOLnOO("Failed!");
        getConsole().OLnOO("");
        return 1;
    }
    else
    {
        getConsole().SOLnOO("Done!");
        getConsole().OLn("");
    }
    getConsole().L();

    PRREWindow& window = p->GFX.getWindow();
    window.SetAutoCleanupOnQuitOn(false);
    window.SetCaption( p->sGameTitle );
    window.ShowFull();
    window.WriteSettings();
    //window.SetCursorVisible(false);
    p->GFX.getScreen().SetVSyncEnabled(true);

    getConsole().L();
    getConsole().OLnOI("Initializing Input ...");
    if ( p->inputHandler.initialize( window.getWndHandle() ) )
    {
        getConsole().SOLnOO("Done!");
        getConsole().OLn("");
    }
    else
    {
        getConsole().EOLnOO("Failed!");
        getConsole().OLn("");
    }
    getConsole().L();

    getConsole().L();
    getConsole().OLnOI("Initializing World ...");
    if ( p->world.initialize() )
    {
        getConsole().SOLnOO("Done!");
        getConsole().OLn("");
    }
    else
    {
        getConsole().EOLnOO("Failed!");
        getConsole().OLn("");
    }
    getConsole().L();

    p->bIsGameRunning = true;

    onGameInitialized();
    getConsole().OO();

    return 0;
} // initializeGame()


/**
    Runs the game.

    @return Always 0.
*/
int PGE::runGame()
{
    while ( isGameRunning() )
    {
        PRREWindow& window = p->GFX.getWindow();
        window.ProcessMessages();
        p->bIsGameRunning = !window.hasCloseRequest();

        PgePacket pkt;
        if (p->SysNET.PollIncomingMessages(pkt))
        {
            // TODO: PollIncomingMessages() could simply put this pkt into the queue by itself ... 
            p->SysNET.queuePackets.push_back(pkt);
        };
        p->SysNET.PollConnectionStateChanges();  // this may also add packet(s) to SysNET.queuePackets
        while (getPacketQueue().size() > 0)
        {
            pkt = getPacketQueue().front();
            getPacketQueue().pop_front();
            onPacketReceived(pkt);
        }
        
        if ( window.isActive() || p->bInactiveLikeActive )
        {
            PGEInputHandler::createAndGet().getMouse().ApplyRelativeInput();

            // TODO: on the long run, bullet movement and collision handling could be put here ...
            onGameRunning();
            p->GFX.getRenderer()->RenderScene();
        }
        else
        {
            if ( p->nInactiveSleep > 0 )
            {
                Sleep( p->nInactiveSleep );
            }
            // in inactive state, even though RenderScene() doesn't get called from here,
            // the scene may be re-rendered from WndProc(), if wnd repaint is needed ...
        }
    }

    return 0;
} // runGame()


/**
    Gets the running state of the game.

    @return True if the game has been successfully initialized, false otherwise.
*/
bool PGE::isGameRunning() const
{
    return p->isGameRunning();
} // isGameRunning()


/**
    Destroys the game engine.
    The game engine can be initialized again after this call.

    @return Always 0.
*/
int PGE::destroyGame()
{
    onGameDestroying();

    p->destroyGame();

    onGameDestroyed();
    return 0;
} // destroyGameEngine()


// ############################## PROTECTED ##############################


PGE::PGE()
{
    p = NULL;
}


PGE::PGE(const PGE&)
{
    p = NULL;
}  


PGE& PGE::operator=(const PGE&)
{
    p = NULL;
    return *this;
}


/** 
    This is the only usable ctor, this is used by the static createAndGet().
*/
PGE::PGE(const char* gameTitle)
{
    p = new PGEimpl(gameTitle);
} // PGE(...)


PGE::~PGE()
{
    delete p;
    p = NULL;
} // ~PGE()


// ############################### PRIVATE ###############################




