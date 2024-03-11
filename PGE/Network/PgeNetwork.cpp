/*
    ###################################################################################
    PgeNetwork.cpp
    This file is part of PGE.
    PGE Network Main class
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgeNetwork.h"
#include "PgeGnsWrapper.h"

/*
   PgeNetworkImpl
   ###########################################################################
*/

class PgeNetworkImpl :
    public pge_network::PgeNetwork
{
public:
    virtual ~PgeNetworkImpl();       /**< Calls shutdown(). */

    bool initialize() override;
    bool shutdown() override;
    bool isInitialized() const override;
    bool reinitialize() override;
    void disconnect(const std::string& sExtraDebugText = "") override;

    bool isServer() const override;
    void Update() override;

    pge_network::PgeIServerClient* getServerClientInstance() override;
    pge_network::PgeClient& getClient() override;
    pge_network::PgeServer& getServer() override;

    void WriteList() const override;

private:

    // ---------------------------------------------------------------------------

    PGEcfgProfiles& m_cfgProfiles;
    pge_network::PgeIServerClient* m_pServerClient;
    pge_network::PgeClient& m_pClient;
    pge_network::PgeServer& m_pServer;
    bool m_bServer;

    explicit PgeNetworkImpl(PGEcfgProfiles& cfgProfiles);   /**< NULLs members only. */
    PgeNetworkImpl(const PgeNetworkImpl&);
    PgeNetworkImpl& operator=(const PgeNetworkImpl&);

    friend class pge_network::PgeNetwork;
};


// ############################### PUBLIC ################################


/**
    Calls shutdown().
*/
PgeNetworkImpl::~PgeNetworkImpl()
{
    shutdown();
} // ~PgeNetwork()

/**
    Initialize the networking subsystem.
    Either the server or a client instance will be initialized.

    @return The result of the initialization. True on success, false otherwise.
*/
bool PgeNetworkImpl::initialize()
{
    if (m_cfgProfiles.getVars()[CVAR_NET_SERVER].getAsString().empty())
    {
        m_bServer = (IDYES == MessageBox(0, "Want to be a Server?", ":)", MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND));
    }
    else
    {
        m_bServer = m_cfgProfiles.getVars()[CVAR_NET_SERVER].getAsBool();
        CConsole::getConsoleInstance("PgeGnsWrapper").OLn("s_bServer from config: %b", m_bServer);
    }

    if (m_bServer)
    {
        m_pServerClient = &m_pServer;
    }
    else
    {
        m_pServerClient = &m_pClient;
    }

    if (!m_pServerClient->initialize())
    {
        getConsole().EOLn("ServerClient has FAILED to initialize!");
        return false;
    }

    return true;
} // initialize()


/**
    Stops the networking subsystem.
    No effect before initialization.
    After shutdown, initialize() can be called again.

    @return True on successful shutdown, false otherwise.
*/
bool PgeNetworkImpl::shutdown()
{
    if (m_pServerClient)
    {
        const bool bRet = m_pServerClient->shutdown();
        m_pServerClient = nullptr;
        return bRet;
    }
    return true;
} // shutdown()


/**
    Stop and then initialize the networking subsystem.
    Useful when we change the value of CVAR_NET_SERVER and want this change to take effect.

    @return True on successful reinitialization, false otherwise.
*/
bool PgeNetworkImpl::reinitialize()
{
    getConsole().OLn("PgeNetwork::reinitialize() ...");
    if (shutdown())
    {
        return initialize();
    }
    getConsole().EOLn("ERROR: PgeNetwork::reinitialize() failed!");
    return false;
}

/**
    Gets the state of the networking subsystem.
    @return True if the networking subsystem is successfully initialized, false before initialization or after shutdown.
*/
bool PgeNetworkImpl::isInitialized() const
{
    return m_pServerClient != nullptr;
} // isInitialized()


/**
    Disconnects the initialized networking subsystem.
    After a disconnect(), the initialized networking subsystem can be connected again:
     - in case of server: getServer()->startListening();
     - in case of client: getClient()->connectToServer() .

    @param sExtraDebugText An optional text that will be sent to the other side and will be logged to help debugging.
*/
void PgeNetworkImpl::disconnect(const std::string& sExtraDebugText)
{
    if (m_pServerClient)
    {
        m_pServerClient->disconnect(sExtraDebugText);
    }
}

/**
*   Returns whether the initialized network subsystem should be the server instance.
*   Note: this has nothing to do with the actual initialization state of the network subsystem, this is just the desired configuration.
* 
*   @return True if initialized network subsystem should be the server instance, false if it should be the client instance.
*/
bool PgeNetworkImpl::isServer() const
{
    return m_bServer;
}

/**
*   Handles connection state changes and forwards messages to the application.
*   Application can process these messages by overriding PGE::onPacketReceived().
*/
void PgeNetworkImpl::Update()
{
    if (!isInitialized())
    {
        return;
    }

    while (m_pServerClient->pollIncomingMessages())
    {}
    m_pServerClient->pollConnectionStateChanges();  // this may also add packet(s) to m_pServerClient.queuePackets
}

/**
*   Returns access to the initialized networking subsystem.
*   This is either the server or the client instance, or none of them.
* 
*   @return Either the server or the client instance, depending on which is initialized.
*           Nullptr if not yet initialized (when isInitialized() is false).
*/
pge_network::PgeIServerClient* PgeNetworkImpl::getServerClientInstance()
{
    return m_pServerClient;
}

/**
*   Returns the client instance.
*   This is always a valid reference, even if getServerClientInstance() returns nullptr.
*   This is because both client and server instances are always created, however neither of them might be initialized.
*   This function is for convenience, since it is similar to:
*     dynamic_cast<pge_network::PgeClient*>(getServerClientInstance())
* 
*   @return The client instance which might or might not be initialized.
*/
pge_network::PgeClient& PgeNetworkImpl::getClient()
{
    return m_pClient;
}

/**
*   Returns the server instance.
*   This is always a valid reference, even if getServerClientInstance() returns nullptr.
*   This is because both client and server instances are always created, however neither of them might be initialized.
*   This function is for convenience, since it is similar to:
*     dynamic_cast<pge_network::PgeServer*>(getServerClientInstance())
*
*   @return The server instance which might or might not be initialized.
*/
pge_network::PgeServer& PgeNetworkImpl::getServer()
{
    return m_pServer;
}

/**
    Writes statistics to console.
*/
void PgeNetworkImpl::WriteList() const
{
    getConsole().OLnOI("PgeNetwork::WriteList() start");
    if (isInitialized())
    {
        m_pServerClient->WriteList();
    }
    else
    {
        getConsole().OLn("PgeNetwork is NOT initialized!");
    }

    getConsole().OOOLn("PgeNetwork::WriteList() end");
    getConsole().OLn("");
} // WriteList()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    NULLs members only.
*/
PgeNetworkImpl::PgeNetworkImpl(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    m_pServerClient(nullptr),
    m_pClient(pge_network::PgeClient::createAndGet(m_cfgProfiles)),
    m_pServer(pge_network::PgeServer::createAndGet(m_cfgProfiles)),
    m_bServer(false)
{

} // PgeNetworkImpl(...)


PgeNetworkImpl::PgeNetworkImpl(const PgeNetworkImpl& other) :
    m_cfgProfiles(other.m_cfgProfiles),
    m_pServerClient(nullptr),
    m_pClient(pge_network::PgeClient::createAndGet(m_cfgProfiles)),
    m_pServer(pge_network::PgeServer::createAndGet(m_cfgProfiles)),
    m_bServer(false)
{
}


PgeNetworkImpl& PgeNetworkImpl::operator=(const PgeNetworkImpl&)
{
    return *this;
}


/*
   PgeNetwork
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
pge_network::PgeNetwork& pge_network::PgeNetwork::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PgeNetworkImpl inst(cfgProfiles);
    return inst;
} // createAndGet()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* pge_network::PgeNetwork::getLoggerModuleName()
{
    return "PgeNetwork";
} // getLoggerModuleName()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& pge_network::PgeNetwork::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


