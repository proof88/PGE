/*
    ###################################################################################
    PGE.cpp
    This file is part of PGE.
    PR00F's Game Engine main class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

//#include "PURE/PureBaseIncludes.h"  // PCH
#include "PureBaseIncludes.h"  // PCH

#include <chrono>
#include <thread>

#include "PGE.h"
#include "PGEincludes.h"
#include "PGEpragmas.h"
// Subsystems
#include "PGESysGFX.h"
#include "PGESysSFX.h"
#include "Config/PGEcfgProfiles.h"
#include "Weapons/WeaponManager.h"

#include "PURE/include/external/Display/PureScreen.h"
#include "PURE/include/external/Display/PureWindow.h"

using namespace std;

static constexpr char* CVAR_GFX_WINDOWED = "gfx_windowed";

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

    PGEcfgProfiles& getConfigProfiles();
    PGEInputHandler& getInput() const;       
    PGEWorld& getWorld() const;             
    PR00FsUltimateRenderingEngine& getPure() const;
    pge_network::PgeNetwork& getNetwork() const;
    SoLoud::Soloud& getAudio();
    WeaponManager& getWeaponManager();
                    
    bool isGameRunning() const;               
    int  destroyGame();                        

protected:

private:

    static std::string** m_pLangTable;      /**< Language-dependent strings. */
    static int           m_nLangTable;      /**< Length of m_pLangTable. */

    // ---------------------------------------------------------------------------

    static PGE_ENUM_LANG getLangFromMSG_ID(PGE_MSG_ID msg_id); /**< Transforms a message ID into a lang ID. */
    static int showWindowsMessageDialogWin32(
        const char* msg, const char* cpt, UINT type);          /**< Shows an error dialog box using WinAPI. */
    static int showWindowsMessageDialogWin32(
        PGE_MSG_ID msg_id, PGE_MSG_ID cpt_id, UINT type);      /**< Shows an error dialog box using WinAPI. */

    // ---------------------------------------------------------------------------

    PGE*      m_pOwner;                  /**< The owner public object who creates this pimpl object. */

    PGEcfgProfiles m_cfgProfiles;
    PGEInputHandler& m_inputHandler;
    PGEWorld& m_world;
    PR00FsUltimateRenderingEngine& m_gfx;
    PGESysGFX m_sysGFX;
    pge_network::PgeNetwork& m_network;
    PGESysSFX  m_sysSFX;
    WeaponManager m_wpnMgr;

    bool        m_bIsGameRunning;         /**< Is the game running (true after successful init and before initiating shutdown)? */
    std::string m_sGameTitle;             /**< Simplified name of the game, used in paths too, so can't contain joker chars. */
    int         m_nInactiveSleep;         /**< Amount of sleep in millisecs when inactive, 0 means no sleep. */
    bool        m_bInactiveLikeActive;    /**< If true, runGame() will act the same way in inactive state as in active state. */

    unsigned int m_nTargetGameLoopFreq; /**< Frequency for the main game engine loop, 0 means no target frequency. */
    double m_minFrameTimeMicrosecs;

    // ---------------------------------------------------------------------------

    PGEimpl();

    PGEimpl(const PGEimpl&); 
    PGEimpl& operator=(const PGEimpl&);

    PGEimpl(const char* gametitle);

    void frameFrameLimit(
        std::chrono::time_point<std::chrono::steady_clock>& timeNow,
        std::chrono::time_point<std::chrono::steady_clock>& timeLastTime);

    friend class PGE;

}; 


// ############################### PUBLIC ################################


PGE::PGEimpl::~PGEimpl()
{
    m_pOwner = NULL;
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
    return m_sGameTitle;
} // getGameTitle()


void PGE::PGEimpl::SetGameTitle(const char* gameTitle)
{
    if ( !m_bIsGameRunning )
    {
        m_sGameTitle = gameTitle;
    }
} // SetGameTitle()


int  PGE::PGEimpl::getWaitingWhileInactive() const
{
    return m_nInactiveSleep;    
} // getWaitingWhileInactive()


void PGE::PGEimpl::SetWaitingWhileInactive(int msecs)   
{
    if ( msecs < -1 )
        return;
    m_nInactiveSleep = msecs;
} // setWaitingWhileInactive()


bool PGE::PGEimpl::isInactiveLikeActive() const
{
    return m_bInactiveLikeActive;
} // isInactiveLikeActive()


void PGE::PGEimpl::SetInactiveLikeActive(bool value)
{
    m_bInactiveLikeActive = value;
} // setInactiveLikeActive()


PGEcfgProfiles& PGE::PGEimpl::getConfigProfiles()
{
    return m_cfgProfiles;
}


PGEInputHandler& PGE::PGEimpl::getInput() const
{
    return m_inputHandler;
} // getInput()


PGEWorld& PGE::PGEimpl::getWorld() const
{
    return m_world;
} // getWorld()


PR00FsUltimateRenderingEngine& PGE::PGEimpl::getPure() const
{
    return m_gfx;
}

pge_network::PgeNetwork& PGE::PGEimpl::getNetwork() const
{
    return m_network;
}

SoLoud::Soloud& PGE::PGEimpl::getAudio()
{
    return m_sysSFX.getAudioCore();
}

WeaponManager& PGE::PGEimpl::getWeaponManager()
{
    return m_wpnMgr;
}


bool PGE::PGEimpl::isGameRunning() const
{
    return m_bIsGameRunning;
} // isGameRunning()


int PGE::PGEimpl::destroyGame()
{
    // make sure that everything is destructed in REVERSE order compared to initializeGame()
    // first things to shutdown are instances that are NOT even initialized by initializeGame(), such as m_wpnMgr
    m_wpnMgr.Clear();
    m_world.Shutdown();
    // m_inputHandler doesnt have any shutdown
    m_sysGFX.destroySysGFX();
    m_sysSFX.destroySysSFX();
    getNetwork().shutdown();
    // m_cfgProfiles doesnt have any shutdown

    getConsole().Deinitialize();

    return 0;
} // destroyGameEngine()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


string** PGE::PGEimpl::m_pLangTable = NULL;
int      PGE::PGEimpl::m_nLangTable = 0;

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
    return ( MessageBox(0, m_pLangTable[getLangFromMSG_ID(msg_id)]->c_str(), m_pLangTable[getLangFromMSG_ID(cpt_id)]->c_str(), type) );
} // showWindowsMessageDialogWindows()


PGE::PGEimpl::PGEimpl() :
    m_pOwner(NULL),  // currently not used
    m_cfgProfiles(""),
    m_inputHandler( PGEInputHandler::createAndGet(m_cfgProfiles) ),
    m_world( PGEWorld::createAndGet() ),
    m_gfx( PR00FsUltimateRenderingEngine::createAndGet(m_cfgProfiles, m_inputHandler) ),
    m_sysGFX(m_cfgProfiles, m_inputHandler),
    m_network( pge_network::PgeNetwork::createAndGet(m_cfgProfiles) ),
    m_sysSFX(m_cfgProfiles),
    m_wpnMgr(m_cfgProfiles, m_gfx),
    m_bIsGameRunning(false),
    m_nTargetGameLoopFreq(0),
    m_minFrameTimeMicrosecs(0.0)
{
    
}


PGE::PGEimpl::PGEimpl(const PGE::PGEimpl&) :
    m_pOwner(NULL),  // currently not used
    m_cfgProfiles(""),
    m_inputHandler( PGEInputHandler::createAndGet(m_cfgProfiles) ),
    m_world( PGEWorld::createAndGet() ),
    m_gfx( PR00FsUltimateRenderingEngine::createAndGet(m_cfgProfiles, m_inputHandler) ),
    m_sysGFX(m_cfgProfiles, m_inputHandler),
    m_network( pge_network::PgeNetwork::createAndGet(m_cfgProfiles) ),
    m_sysSFX(m_cfgProfiles),
    m_wpnMgr(m_cfgProfiles, m_gfx),
    m_bIsGameRunning(false),
    m_nTargetGameLoopFreq(0),
    m_minFrameTimeMicrosecs(0.0)
{

}  


PGE::PGEimpl& PGE::PGEimpl::operator=(const PGE::PGEimpl&)
{
    m_pOwner = NULL;  // currently not used
    return *this;
}


/** 
    This is the only usable ctor, this is used by the static createAndGet().
*/
PGE::PGEimpl::PGEimpl(const char* gameTitle) :
    m_pOwner(NULL),  // currently not used
    m_cfgProfiles(gameTitle),
    m_inputHandler( PGEInputHandler::createAndGet(m_cfgProfiles) ),
    m_world( PGEWorld::createAndGet() ),
    m_gfx( PR00FsUltimateRenderingEngine::createAndGet(m_cfgProfiles, m_inputHandler) ),
    m_sysGFX(m_cfgProfiles, m_inputHandler),
    m_network(pge_network::PgeNetwork::createAndGet(m_cfgProfiles)),
    m_sysSFX(m_cfgProfiles),
    m_wpnMgr(m_cfgProfiles, m_gfx),
    m_bIsGameRunning(false),
    m_sGameTitle(gameTitle),
    m_nInactiveSleep(PGE_INACTIVE_SLEEP),
    m_bInactiveLikeActive(PGE_INACTIVE_AS_ACTIVE),
    m_nTargetGameLoopFreq(0),
    m_minFrameTimeMicrosecs(0.0)
{
    
} // PGE(...)

static void busyWait(double microsecsToWait)
{
    if (microsecsToWait <= 0.0)
    {
        return;
    }

    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    long long microsecsWaited = 0;
    while (microsecsWaited < microsecsToWait)
    {
        microsecsWaited = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
    }
}

void PGE::PGEimpl::frameFrameLimit(
    std::chrono::time_point<std::chrono::steady_clock>& timeNow,
    std::chrono::time_point<std::chrono::steady_clock>& timeLastTime)
{
    if (m_minFrameTimeMicrosecs <= 0.0)
    {
        return;
    }

    timeNow = std::chrono::steady_clock::now();
    const long long durElapsedMicrosecs = (std::chrono::duration_cast<std::chrono::microseconds>(timeNow - timeLastTime)).count();
    if (durElapsedMicrosecs < m_minFrameTimeMicrosecs)
    {
        // not nice, but effective without using sleep
        busyWait(m_minFrameTimeMicrosecs - durElapsedMicrosecs);
    }
    timeLastTime = std::chrono::steady_clock::now();
}



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
    Returns the config handler object.

    @return Game engine config handler.
*/
PGEcfgProfiles& PGE::getConfigProfiles() const
{
    return p->getConfigProfiles();
}


/**
    Returns the input handler object.

    @return Game engine input handler.
*/
PGEInputHandler& PGE::getInput() const
{
    return p->getInput();
} // getInput()


/**
    Returns the m_world object.

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
PR00FsUltimateRenderingEngine& PGE::getPure() const
{
    return p->getPure();
}


/**
    Returns the network functionality interface.

    @return The network functionality interface.
*/
pge_network::PgeNetwork& PGE::getNetwork() const
{
    return p->getNetwork();
}


/**
    Returns audio lib interface.

    @return Audio lib interface.
*/
SoLoud::Soloud& PGE::getAudio()
{
    return p->getAudio();
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
int PGE::initializeGame(const char* szCmdLine)
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
    if ( p->m_bIsGameRunning )
    {
        getConsole().OLn("  already initialized!");
        return 0;
    }
    getConsole().OI();
    getConsole().OLn(PGE_NAME);
    getConsole().OLn(PGE_VERSION);

    if (!onGameInitializing())
    {
        getConsole().EOLnOO("onGameInitializing() failed!");
        getConsole().OLn("");
        return 1;
    }

    getConsole().OLn("Game Title: %s", p->m_sGameTitle.c_str());
    getConsole().OLn("Command Line Args: %s", szCmdLine);

    getConsole().OLn("Documents Folder: %s", p->m_cfgProfiles.getMyDocsFolder().c_str());
    p->m_nLangTable = p->m_cfgProfiles.readLanguageData( p->m_pLangTable );
    getConsole().OLn("Lang Table with %d rows from %s.", p->m_nLangTable, p->m_cfgProfiles.getLangFileName().c_str());
    if ( p->m_nLangTable == 0 )
    {
        getConsole().EOLnOO("ERROR: Failed to read language data, exiting!");
        return 99;
    }

    getConsole().OLn("Profiles stored in Documents: %b", p->m_cfgProfiles.areProfilesInMyDocs());
    getConsole().OLn("Profiles: %s", p->m_cfgProfiles.getPathToProfiles().c_str());
    getConsole().OIOLn("Count: %d", p->m_cfgProfiles.getProfilesCount());
    for (int i = 0; i < p->m_cfgProfiles.getProfilesCount(); i++)
    {
        getConsole().OLn("%s.cfg ~ %s", p->m_cfgProfiles.getProfilesList()[i]->c_str(), p->m_cfgProfiles.getProfilePlayersList()[i]->c_str());
    }
    p->m_cfgProfiles.ProcessCommandLine(szCmdLine);
    if (p->m_cfgProfiles.getProfilesCount() > 0)
    {
        p->m_cfgProfiles.SetProfile(0);
    }
    getConsole().OO();

    getConsole().L();
    getConsole().OLnOI("Initializing Networking ...");
    if (!(p->m_network.initialize()))
    {
        getConsole().EOLnOO("Failed!");
        getConsole().OLn("");
        return 1;
    }
    else
    {
        getConsole().SOLnOO("Done!");
        getConsole().OLn("");
    } 
    getConsole().L();
        
    getConsole().L();
    getConsole().OLnOI("Initializing Audio ...");
    if ( !(p->m_sysSFX.initSysSFX()) )
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
    bool bFullScreen;

    if (getConfigProfiles().getVars()[CVAR_GFX_WINDOWED].getAsString().empty())
    {
        bFullScreen = MessageBox(0, "Fullscreen?", ":)", MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND) == IDYES;
        getConsole().OLn("Full screen override: %b", bFullScreen);
    }
    else
    {
        bFullScreen = !getConfigProfiles().getVars()[CVAR_GFX_WINDOWED].getAsBool();
        getConsole().OLn("Full screen from config: %b", bFullScreen);
    }

    if ( bFullScreen )
        bGFXinit = p->m_sysGFX.initSysGFX(0, 0, PURE_FULLSCREEN, 0, 32, 24, 0, 0);
    else
        bGFXinit = p->m_sysGFX.initSysGFX(1024, 768, PURE_WINDOWED, 0, 32, 24, 0, 0);

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

    PureWindow& window = p->m_gfx.getWindow();
    window.SetAutoCleanupOnQuitOn(false);
    window.SetCaption( p->m_sGameTitle );
    window.ShowFull();
    window.WriteSettings();
    //window.SetCursorVisible(false);
    p->m_gfx.getScreen().SetVSyncEnabled(true);

    getConsole().L();
    getConsole().OLnOI("Initializing Input ...");
    if ( p->m_inputHandler.initialize( window.getWndHandle() ) )
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
    if ( p->m_world.initialize() )
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

    p->m_bIsGameRunning = true;

    if (!onGameInitialized())
    {
        getConsole().EOLnOO("onGameInitialized() failed!");
        getConsole().OLn("");
        return 1;
    }

    getConsole().OO();

    return 0;
} // initializeGame()

/**
    Runs the game.

    @return Always 0.
*/
int PGE::runGame()
{
    std::chrono::time_point<std::chrono::steady_clock> timeNow = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> timeLastTime = timeNow;

    PureWindow& window = p->m_gfx.getWindow();
    window.ProcessMessages();
    getInput().getMouse().getWheel();  // trigger zeroing out any possibly accumulated wheel rotation so onGameRunning() won't see any

    while ( isGameRunning() )
    {
        onGameFrameBegin();
        
        window.ProcessMessages();
        p->m_bIsGameRunning = !window.hasCloseRequest();

        getNetwork().Update();  // this may also inject packet(s) to SysNET.queuePackets
        while (getNetwork().getPacketQueueSize() > 0)
        {
            // as far as we check for packet queue size before pop, exception won't be thrown
            if (!onPacketReceived(getNetwork().popFrontPacket()))
            {
                getConsole().EOLn("ERROR: onPacketReceived() failed, closing window ...");
                window.Close();
                break;
            }
        }

        // TODO: on the long run, bullet movement and collision handling could be put here ...       
        if ( window.isActive() || p->m_bInactiveLikeActive )
        {
            p->m_inputHandler.getMouse().ApplyRelativeInput();
            onGameRunning();
            p->m_gfx.getRenderer()->RenderScene();
            p->frameFrameLimit(timeNow, timeLastTime);
        }
        //else
        //{
        //    // I think that if multiplayer is enabled, we should not sleep that big because we are processing
        //    // packets on this same thread, and if we are server, all clients rely on the response
        //    // time of this main thread ...
        //    // For now, I think it is enough if we do sleep only if we are clients ... server must not sleep.
        //    // But at the same time, even server should also stick to some update rate in the future ...
        //    // 
        //    // TODO: change this condition: in case of multiplayer, do not sleep.
        //    //if ( !isServer() && (p->m_nInactiveSleep > 0) )
        //    //{
        //    //    Sleep( p->m_nInactiveSleep );
        //    //}
        //    // in inactive state, even though RenderScene() doesn't get called from here,
        //    // the scene may be re-rendered from WndProc(), if wnd repaint is needed ...
        //}
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

/**
    Gets the frequency for the main game engine loop.
    This controls how many times onGameRunning() will be invoked in each second by the game engine.
    Basically this can be used to determine a target FPS value for your game.
    Default value is 0, meaning not limiting maximum FPS.

    Note that even if this value is 0, maximum FPS might be limited by the current V-Sync setting.
    V-Sync is controlled by PureScreen::SetVSyncEnabled().
*/
unsigned int PGE::getGameRunningFrequency() const
{
    return p->m_nTargetGameLoopFreq;
}

/**
    Sets the frequency for the main game engine loop.
    This controls how many times onGameRunning() will be invoked in each second by the game engine.
    Basically this can be used to determine a target FPS value for your game.
    Default value is 0, meaning not limiting maximum FPS.

    Note that even if this value is 0, maximum FPS might be limited by the current V-Sync setting.
    V-Sync is controlled by PureScreen::SetVSyncEnabled().
*/
void PGE::setGameRunningFrequency(unsigned int freq)
{
    p->m_nTargetGameLoopFreq = freq;
    p->m_minFrameTimeMicrosecs = freq > 0 ? (1000.0 * 1000.0 / freq) : 0;
}


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




