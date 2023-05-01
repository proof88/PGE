/*
    ###################################################################################
    PgeServer.cpp
    This file is part of PGE.
    PGE Network Main class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgeServer.h"
#include "PgeGsnServer.h"

/*
   PgeServerImpl
   ###########################################################################
*/

class PgeServerImpl final :
    public pge_network::PgeServer
{
public:
    virtual ~PgeServerImpl();       /**< Calls shutdown(). */

    /* implement stuff from PgeIServerClient start */

    bool initialize() override;
    bool shutdown() override;
    bool isInitialized() const override;

    void Update() override;

    bool PollIncomingMessages() override;
    void PollConnectionStateChanges() override;

    std::size_t getPacketQueueSize() const override;
    pge_network::PgePacket popFrontPacket() noexcept(false) override;

    std::set<pge_network::PgePktId>& getAllowListedPgeMessages() override;
    std::set<pge_network::TPgeMsgAppMsgId>& getAllowListedAppMessages() override;

    uint32_t getRxPacketCount() const override;
    uint32_t getTxPacketCount() const override;
    uint32_t getInjectPacketCount() const override;

    uint32_t getRxPacketPerSecondCount() const override;
    uint32_t getTxPacketPerSecondCount() const override;
    uint32_t getInjectPacketPerSecondCount() const override;

    void WriteList() const override;

    /* implement stuff from PgeIServerClient end */

    /* implement stuff from PgeServer start */

    bool startListening() override;
    void SendPacketToClient(pge_network::PgeNetworkConnectionHandle connHandle, const pge_network::PgePacket& pkt) override;
    void SendPacketToAllClients(const pge_network::PgePacket& pkt, pge_network::PgeNetworkConnectionHandle exceptConnHandle = 0) override;
    void InjectPacket(const pge_network::PgePacket& pkt) override;

    /* implement stuff from PgeServer end */

private:

    // ---------------------------------------------------------------------------

    PGEcfgProfiles& m_cfgProfiles;
    PgeGsnServer& m_gsnServer;

    explicit PgeServerImpl(PGEcfgProfiles& cfgProfiles);
    PgeServerImpl(const PgeServerImpl&);
    PgeServerImpl& operator=(const PgeServerImpl&);

    friend class pge_network::PgeServer;
};


// ############################### PUBLIC ################################


/**
    Calls shutdown().
*/
PgeServerImpl::~PgeServerImpl()
{
    shutdown();
} // ~PgeServer()

/**
    Initialize the server subsystem.

    @return The result of the initialization. True on success, false otherwise.
*/
bool PgeServerImpl::initialize()
{
    return m_gsnServer.init();
} // initialize()


/**
    This stops the server subsystem.
    No effect before initialization.
    After shutdown, initialize() and startListening() can be called again.

    @return True on successful shutdown, false otherwise.
*/
bool PgeServerImpl::shutdown()
{
    // TODO: this is not symmetric, it should call destroy() because initialize() calls init().
    return m_gsnServer.stopListening();
} // shutdown()

/**
    Gets the state of the server subsystem.
    @return True if the server subsystem is successfully initialized and listening, false otherwise.
*/
bool PgeServerImpl::isInitialized() const
{
    return m_gsnServer.isInitialized();
} // isInitialized()

void PgeServerImpl::Update()
{
    m_gsnServer.PollIncomingMessages();
    m_gsnServer.PollConnectionStateChanges();  // this may also add packet(s) to SysNET.queuePackets
}

bool PgeServerImpl::PollIncomingMessages()
{
    return m_gsnServer.PollIncomingMessages();
}

void PgeServerImpl::PollConnectionStateChanges()
{
    return m_gsnServer.PollConnectionStateChanges();
}

std::size_t PgeServerImpl::getPacketQueueSize() const
{
    return m_gsnServer.getPacketQueueSize();
}

pge_network::PgePacket PgeServerImpl::popFrontPacket() noexcept(false)
{
    return m_gsnServer.popFrontPacket();
}

std::set<pge_network::PgePktId>& PgeServerImpl::getAllowListedPgeMessages()
{
    return m_gsnServer.getAllowListedPgeMessages();
}

std::set<pge_network::TPgeMsgAppMsgId>& PgeServerImpl::getAllowListedAppMessages()
{
    return m_gsnServer.getAllowListedAppMessages();
}

uint32_t PgeServerImpl::getRxPacketCount() const
{
    return m_gsnServer.getRxPacketCount();
}

uint32_t PgeServerImpl::getTxPacketCount() const
{
    return m_gsnServer.getTxPacketCount();
}

uint32_t PgeServerImpl::getInjectPacketCount() const
{
    return m_gsnServer.getInjectPacketCount();
}

uint32_t PgeServerImpl::getRxPacketPerSecondCount() const
{
    return m_gsnServer.getRxPacketPerSecondCount();
}

uint32_t PgeServerImpl::getTxPacketPerSecondCount() const
{
    return m_gsnServer.getTxPacketPerSecondCount();
}

uint32_t PgeServerImpl::getInjectPacketPerSecondCount() const
{
    return m_gsnServer.getInjectPacketPerSecondCount();
}

void PgeServerImpl::WriteList() const
{
    getConsole().OLnOI("PgeServer::WriteList() start");
    if (isInitialized())
    {
        getConsole().OLn("Role: Server");
        // TODO: PgeGsnWrapper will obviously use PgeGsnWrapper as module name when writing to console, so it is recommended now
        // to always turn on PgeGsnWrapper logging as well together with PgeServer
        m_gsnServer.WriteServerClientList();
    }
    else
    {
        getConsole().OLn("PgeServer is NOT initialized!");
    }

    getConsole().OOOLn("PgeServer::WriteList() end");
} // WriteList()

bool PgeServerImpl::startListening()
{
    return m_gsnServer.startListening();
}

void PgeServerImpl::SendPacketToClient(pge_network::PgeNetworkConnectionHandle connHandle, const pge_network::PgePacket& pkt)
{
    // TODO add check: connHandle cannot be 0!
    m_gsnServer.SendPacketToClient(static_cast<HSteamNetConnection>(connHandle), pkt);
}

void PgeServerImpl::SendPacketToAllClients(const pge_network::PgePacket& pkt, pge_network::PgeNetworkConnectionHandle exceptConnHandle)
{
    m_gsnServer.SendPacketToAllClients(pkt, exceptConnHandle);
}

void PgeServerImpl::InjectPacket(const pge_network::PgePacket& pkt)
{
    m_gsnServer.InjectPacket(pkt);
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PgeServerImpl::PgeServerImpl(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    m_gsnServer(PgeGsnServer::createAndGet(cfgProfiles))
{
    m_gsnServer.getAllowListedPgeMessages().insert(pge_network::MsgApp::id);
} // PgeServerImpl(...)

PgeServerImpl::PgeServerImpl(const PgeServerImpl& other) :
    m_cfgProfiles(other.m_cfgProfiles),
    m_gsnServer(PgeGsnServer::createAndGet(other.m_cfgProfiles))
{
}

PgeServerImpl& PgeServerImpl::operator=(const PgeServerImpl&)
{
    return *this;
}


/*
   PgeServer
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
pge_network::PgeServer& pge_network::PgeServer::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PgeServerImpl serverInst(cfgProfiles);
    return serverInst;
} // createAndGet()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* pge_network::PgeServer::getLoggerModuleName()
{
    return "PgeServer";
} // getLoggerModuleName()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& pge_network::PgeServer::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


