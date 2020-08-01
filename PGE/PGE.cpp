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
#include "PGEpragmas.h"
#include "PGE.h"
#include "PRRE/Display/PRREWindow.h"
#include "PRRE/Display/PRREScreen.h"
#include "PGEincludes.h"
// Subsystems
#include "PGESysGFX.h"
#include "PGESysSFX.h"
#include "PGESysNET.h"
#include "PGESysCFG.h"

using namespace std;



/*
   PGE::PGEimpl
   ###########################################################################
*/


class PGE::PGEimpl
{

public:

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

    CConsole&  con;
    PGESysCFG* SysCFG;
    PGESysGFX  SysGFX;
    PGESysNET  SysNET;
    PGESysSFX  SysSFX;
    
    PR00FsReducedRenderingEngine& GFX; 
    void* pSFX;
    void* pNET;
    PGEInputHandler& inputHandler;
    PGEWorld& world;

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
    delete SysCFG;
} // ~PGE()


CConsole& PGE::PGEimpl::getConsole() const
{
    return con;
} // getConsole()


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


bool PGE::PGEimpl::isGameRunning() const
{
    return bIsGameRunning;
} // isGameRunning()


int PGE::PGEimpl::destroyGame()
{
    world.Shutdown();
    SysGFX.destroySysGFX();
    SysSFX.destroySysSFX();
    SysNET.destroySysNET();

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
    con( CConsole::getConsoleInstance() ),
    inputHandler( PGEInputHandler::createAndGet() ),
    world( PGEWorld::createAndGet() ),
    GFX( PR00FsReducedRenderingEngine::createAndGet() )
{
    _pOwner = NULL;  // currently not used
}


PGE::PGEimpl::PGEimpl(const PGE::PGEimpl&) :
    con( CConsole::getConsoleInstance() ),
    inputHandler( PGEInputHandler::createAndGet() ),
    world( PGEWorld::createAndGet() ),
    GFX( PR00FsReducedRenderingEngine::createAndGet() )
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
    con( CConsole::getConsoleInstance() ),
    inputHandler( PGEInputHandler::createAndGet() ),
    world( PGEWorld::createAndGet() ),
    GFX( PR00FsReducedRenderingEngine::createAndGet() )
{
    _pOwner = NULL;  // currently not used
    sGameTitle = gameTitle;
    SysCFG = new PGESysCFG(sGameTitle.c_str());
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
    Gets the associated console object.

    @return Console instance used by the game engine.
*/
CConsole& PGE::getConsole() const
{
    return p->getConsole();
} // getConsole()


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


/**
    Initializes the game engine.

    @return 0 on success, 1 on failure.
*/
int PGE::initializeGame()
{

#ifdef PGE_CCONSOLE_IS_ENABLED
    p->con.Initialize("PGE log", true);
    p->con.SetFGColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, "999999" );
    p->con.SetIntsColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "FFFF00" );
    p->con.SetStringsColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, "FFFFFF" );
    p->con.SetFloatsColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "FFFF00" );
    p->con.SetBoolsColor( FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY, "00FFFF" );
#endif

    p->con.OLn("PGE::initializeGame()");
    if ( p->bIsGameRunning )
    {
        p->con.OLn("  already initialized!");
        return 0;
    }
    p->con.OI();
    onGameInitializing();
    p->con.OLn("pGameTitle: %s", p->sGameTitle.c_str());
    p->con.OLn("MyDocs: %s", p->SysCFG->getMyDocsFolder().c_str());
    p->nLangTable = p->SysCFG->readLanguageData( p->pLangTable );
    p->con.OLn("Profiles stored in MyDocs: %b", p->SysCFG->areProfilesInMyDocs());
    p->con.OLn("Lang Table with %d rows from %s.", p->nLangTable, p->SysCFG->getLangFileName().c_str());
    p->con.OLn("Profiles: %s", p->SysCFG->getPathToProfiles().c_str());
    p->con.OIOLn("Count: %d", p->SysCFG->getProfilesCount());
    for (int i = 0; i < p->SysCFG->getProfilesCount(); i++)
    {
        p->con.OLn("%s.cfg ~ %s", p->SysCFG->getProfilesList()[i]->c_str(), p->SysCFG->getProfilePlayersList()[i]->c_str());
    }
    p->con.OO();

    p->con.L();
    p->con.OLnOI("Initializing SysNET ...");
    if ( (p->pNET = p->SysNET.initSysNET()) == PGENULL )
    {
        p->con.EOLnOO("Failed!");
        p->con.OLn("");
        /*return 1;*/
    }
    else
    {
        p->con.SOLnOO("Done!");
        p->con.OLn("");
    } 
    p->con.L();
        
    p->con.L();
    p->con.OLnOI("Initializing SysSFX ...");
    if ( (p->pSFX = p->SysSFX.initSysSFX()) == PGENULL )
    {
        p->con.EOLnOO("Failed!");
        p->con.OLn("");
        /*return 1;*/
    }
    else
    {
        p->con.SOLnOO("Done!");
        p->con.OLn("");
    }
    p->con.L();

    p->con.L();
    p->con.OLnOI("Initializing SysGFX ...");
    bool bGFXinit;
    if ( MessageBox(0, "Teljes képernyõ?", ":)", MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND) == IDYES )
        bGFXinit = p->SysGFX.initSysGFX(-1, -1, PRRE_FULLSCREEN, 0, 32, 24, 0, 0);
    else
        bGFXinit = p->SysGFX.initSysGFX(800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);

    if ( !bGFXinit )
    {
        p->con.EOLnOO("Failed!");
        p->con.OLnOO("");
        return 1;
    }
    else
    {
        p->con.SOLnOO("Done!");
        p->con.OLn("");
    }
    p->con.L();

    PRREWindow& window = p->GFX.getWindow();
    window.SetAutoCleanupOnQuitOn(false);
    window.SetCaption( p->sGameTitle );
    window.ShowFull();
    window.WriteSettings();
    //window.SetCursorVisible(false);
    p->GFX.getScreen().SetVSyncEnabled(true);

    p->con.L();
    p->con.OLnOI("Initializing InputHandler ...");
    if ( p->inputHandler.initialize( window.getWndHandle() ) )
    {
        p->con.SOLnOO("Done!");
        p->con.OLn("");
    }
    else
    {
        p->con.EOLnOO("Failed!");
        p->con.OLn("");
    }
    p->con.L();

    p->con.L();
    p->con.OLnOI("Initializing World ...");
    if ( p->world.initialize() )
    {
        p->con.SOLnOO("Done!");
        p->con.OLn("");
    }
    else
    {
        p->con.EOLnOO("Failed!");
        p->con.OLn("");
    }
    p->con.L();

    p->bIsGameRunning = true;

    onGameInitialized();
    p->con.OO();

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
        
        if ( window.isActive() || p->bInactiveLikeActive )
        {
            PGEInputHandler::createAndGet().getMouse().ApplyRelativeInput();

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




