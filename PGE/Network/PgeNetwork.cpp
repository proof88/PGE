/*
    ###################################################################################
    PgeNetwork.cpp
    This file is part of PGE.
    PGE Network Main class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH

#include "PgeNetwork.h"
#include "PGESysNET.h"

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

    bool isServer() const override;
    void Update() override;

    pge_network::PgeClient& getClient() override;
    pge_network::PgeServer& getServer() override;

    void WriteList() const override;

private:

    // ---------------------------------------------------------------------------

    PGESysNET& m_PgeSysNET;
    pge_network::PgeClient& m_client;
    pge_network::PgeServer& m_server;

    PgeNetworkImpl();                /**< NULLs members only. */
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

    @return The result of the initialization. True on success, false otherwise.
*/
bool PgeNetworkImpl::initialize()
{
    bool b = m_PgeSysNET.initSysNET();
    if (!b)
    {
        getConsole().EOLn("PgeSysNET has FAILED to initialize!");
        return false;
    }

    if (m_PgeSysNET.isServer())
    {
        b = m_server.initialize();
        if (!b)
        {
            getConsole().EOLn("PgeServer has FAILED to initialize!");
        }
    }
    else
    {
        b = m_client.initialize();
        if (!b)
        {
            getConsole().EOLn("PgeClient has FAILED to initialize!");
        }
    }

    return b;
} // initialize()


/**
    This stops the networking subsystem.
    No effect before initialization.
    After shutdown, initialize() can be called again.

    @return True on successful shutdown, false otherwise.
*/
bool PgeNetworkImpl::shutdown()
{
    return m_PgeSysNET.destroySysNET();
} // shutdown()

/**
    Gets the state of the networking subsystem.
    @return True if the networking subsystem is successfully initialized, false before initialization or after shutdown.
*/
bool PgeNetworkImpl::isInitialized() const
{
    return m_PgeSysNET.isInitialized();
} // isInitialized()

bool PgeNetworkImpl::isServer() const
{
    return m_PgeSysNET.isServer();
}

void PgeNetworkImpl::Update()
{
    if (!isInitialized())
    {
        return;
    }

    m_PgeSysNET.PollIncomingMessages();
    m_PgeSysNET.PollConnectionStateChanges();  // this may also add packet(s) to SysNET.queuePackets
}

pge_network::PgeClient& PgeNetworkImpl::getClient()
{
    return m_client;
}

pge_network::PgeServer& PgeNetworkImpl::getServer()
{
    return m_server;
}

/**
    Writes statistics to console.
*/
void PgeNetworkImpl::WriteList() const
{
    getConsole().OLnOI("PgeNetwork::WriteList() start");
    if (isInitialized())
    {
        // m_server and m_client always exist but only one of them is initialized at a time
        if (isServer())
        {
            m_server.WriteList();
        }
        else
        {
            m_client.WriteList();
        }
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
PgeNetworkImpl::PgeNetworkImpl() :
    m_PgeSysNET(PGESysNET::createAndGet()),
    m_client(pge_network::PgeClient::createAndGet()),
    m_server(pge_network::PgeServer::createAndGet())
{

} // PgeNetworkImpl(...)


PgeNetworkImpl::PgeNetworkImpl(const PgeNetworkImpl&) :
    m_PgeSysNET(PGESysNET::createAndGet()),
    m_client(pge_network::PgeClient::createAndGet()),
    m_server(pge_network::PgeServer::createAndGet())
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
pge_network::PgeNetwork& pge_network::PgeNetwork::createAndGet()
{
    static PgeNetworkImpl inst;
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


