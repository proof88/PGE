/*
    ###################################################################################
    PgeServer.cpp
    This file is part of PGE.
    PGE Network Main class
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgeServer.h"
#include "PgeGnsServer.h"

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
    void disconnect(const std::string& sExtraDebugText = "") override;

    void Update() override;

    bool pollIncomingMessages() override;
    void pollConnectionStateChanges() override;

    std::size_t getPacketQueueSize() const override;
    pge_network::PgePacket popFrontPacket() noexcept(false) override;

    std::set<pge_network::PgePktId>& getAllowListedPgeMessages() override;
    std::set<pge_network::MsgApp::TMsgId>& getAllowListedAppMessages() override;

    void send(const pge_network::PgePacket& pkt, const pge_network::PgeNetworkConnectionHandle& connHandle = pge_network::ServerConnHandle) override;

    uint32_t getRxPacketCount() const override;
    uint32_t getTxPacketCount() const override;
    uint32_t getInjectPacketCount() const override;

    uint32_t getRxPacketPerSecondCount() const override;
    uint32_t getTxPacketPerSecondCount() const override;
    uint32_t getInjectPacketPerSecondCount() const override;

    const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getRxMsgCount() const override;
    const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getTxMsgCount() const override;
    const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getInjectMsgCount() const override;

    std::map<pge_network::MsgApp::TMsgId, std::string>& getMsgAppId2StringMap() override;

    uint32_t getRxByteCount() const override;
    uint32_t getTxByteCount() const override;
    uint32_t getInjectByteCount() const override;

    void WriteList() const override;

    /* implement stuff from PgeIServerClient end */

    /* implement stuff from PgeServer start */

    bool startListening(const std::string& sAppVersion = "") override;
    void sendToAllClientsExcept(const pge_network::PgePacket& pkt, const pge_network::PgeNetworkConnectionHandle& exceptConnHandle = 0) override;
    void sendToAll(const pge_network::PgePacket& pkt) override;

    /* Debug functions. */

    void setDebugNickname(const pge_network::PgeNetworkConnectionHandle& connHandle, const std::string& sNickname) override;

    int getPing(
        const pge_network::PgeNetworkConnectionHandle& connHandle,
        bool bForceUpdate) override;
    float getQualityLocal(
        const pge_network::PgeNetworkConnectionHandle& connHandle,
        bool bForceUpdate) override;
    float getQualityRemote(
        const pge_network::PgeNetworkConnectionHandle& connHandle,
        bool bForceUpdate) override;
    float getRxByteRate(
        const pge_network::PgeNetworkConnectionHandle& connHandle,
        bool bForceUpdate) override;
    float getTxByteRate(
        const pge_network::PgeNetworkConnectionHandle& connHandle,
        bool bForceUpdate) override;
    int64_t getPendingUnreliableBytes(
        const pge_network::PgeNetworkConnectionHandle& connHandle,
        bool bForceUpdate) override;
    int64_t getPendingReliableBytes(
        const pge_network::PgeNetworkConnectionHandle& connHandle,
        bool bForceUpdate) override;
    int64_t getSentButUnAckedReliableBytes(
        const pge_network::PgeNetworkConnectionHandle& connHandle,
        bool bForceUpdate) override;
    int64_t getInternalQueueTimeUSecs(
        const pge_network::PgeNetworkConnectionHandle& connHandle,
        bool bForceUpdate) override;
    std::string getDetailedConnectionStatus(
        const pge_network::PgeNetworkConnectionHandle& connHandle) const override;

    /* implement stuff from PgeServer end */

private:

    // ---------------------------------------------------------------------------

    PGEcfgProfiles& m_cfgProfiles;
    PgeGnsServer& m_gnsServer;

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
    return m_gnsServer.init();
} // initialize()


/**
    This stops the server subsystem.
    No effect before initialization.
    After shutdown, initialize() and startListening() can be called again.

    @return True on successful shutdown, false otherwise.
*/
bool PgeServerImpl::shutdown()
{
    return m_gnsServer.destroy();
} // shutdown()

/**
    Gets the state of the server subsystem.
    @return True if the server subsystem is successfully initialized and listening, false otherwise.
*/
bool PgeServerImpl::isInitialized() const
{
    return m_gnsServer.isInitialized();
} // isInitialized()

/**
    Terminates the active connections of the server instance and stops listening.

    @param sExtraDebugText An optional text that will be sent to the other side and will be logged to help debugging.
*/
void PgeServerImpl::disconnect(const std::string& sExtraDebugText)
{
    m_gnsServer.stopListening(sExtraDebugText);
}

void PgeServerImpl::Update()
{
    if (m_gnsServer.isListening())
    {
        m_gnsServer.pollIncomingMessages();
    }
    m_gnsServer.pollConnectionStateChanges();  // this may also add packet(s) to SysNET.queuePackets
}

bool PgeServerImpl::pollIncomingMessages()
{
    if (m_gnsServer.isListening())
    {
        return m_gnsServer.pollIncomingMessages();
    }
    return false;
}

void PgeServerImpl::pollConnectionStateChanges()
{
    return m_gnsServer.pollConnectionStateChanges();
}

std::size_t PgeServerImpl::getPacketQueueSize() const
{
    return m_gnsServer.getPacketQueueSize();
}

pge_network::PgePacket PgeServerImpl::popFrontPacket() noexcept(false)
{
    return m_gnsServer.popFrontPacket();
}

std::set<pge_network::PgePktId>& PgeServerImpl::getAllowListedPgeMessages()
{
    return m_gnsServer.getAllowListedPgeMessages();
}

std::set<pge_network::MsgApp::TMsgId>& PgeServerImpl::getAllowListedAppMessages()
{
    return m_gnsServer.getAllowListedAppMessages();
}

void PgeServerImpl::send(const pge_network::PgePacket& pkt, const pge_network::PgeNetworkConnectionHandle& connHandle)
{
    if (connHandle == pge_network::ServerConnHandle)
    {
        m_gnsServer.inject(pkt);
    }
    else
    {
        m_gnsServer.sendToClient(static_cast<const HSteamNetConnection&>(connHandle), pkt);
    }
}

uint32_t PgeServerImpl::getRxPacketCount() const
{
    return m_gnsServer.getRxPacketCount();
}

uint32_t PgeServerImpl::getTxPacketCount() const
{
    return m_gnsServer.getTxPacketCount();
}

uint32_t PgeServerImpl::getInjectPacketCount() const
{
    return m_gnsServer.getInjectPacketCount();
}

uint32_t PgeServerImpl::getRxPacketPerSecondCount() const
{
    return m_gnsServer.getRxPacketPerSecondCount();
}

uint32_t PgeServerImpl::getTxPacketPerSecondCount() const
{
    return m_gnsServer.getTxPacketPerSecondCount();
}

uint32_t PgeServerImpl::getInjectPacketPerSecondCount() const
{
    return m_gnsServer.getInjectPacketPerSecondCount();
}

const std::map<pge_network::MsgApp::TMsgId, uint32_t>& PgeServerImpl::getRxMsgCount() const
{
    return m_gnsServer.getRxMsgCount();
}

const std::map<pge_network::MsgApp::TMsgId, uint32_t>& PgeServerImpl::getTxMsgCount() const
{
    return m_gnsServer.getTxMsgCount();
}

const std::map<pge_network::MsgApp::TMsgId, uint32_t>& PgeServerImpl::getInjectMsgCount() const
{
    return m_gnsServer.getInjectMsgCount();
}

std::map<pge_network::MsgApp::TMsgId, std::string>& PgeServerImpl::getMsgAppId2StringMap()
{
    return m_gnsServer.getMsgAppId2StringMap();
}

uint32_t PgeServerImpl::getRxByteCount() const
{
    return m_gnsServer.getRxByteCount();
}

uint32_t PgeServerImpl::getTxByteCount() const
{
    return m_gnsServer.getTxByteCount();
}

uint32_t PgeServerImpl::getInjectByteCount() const
{
    return m_gnsServer.getInjectByteCount();
}

void PgeServerImpl::WriteList() const
{
    getConsole().OLnOI("PgeServer::WriteList() start");
    if (isInitialized())
    {
        getConsole().OLn("Role: Server");
        // TODO: PgeGnsWrapper will obviously use PgeGnsWrapper as module name when writing to console, so it is recommended now
        // to always turn on PgeGnsWrapper logging as well together with PgeServer
        m_gnsServer.WriteServerClientList();
    }
    else
    {
        getConsole().OLn("PgeServer is NOT initialized!");
    }

    getConsole().OOOLn("PgeServer::WriteList() end");
} // WriteList()

bool PgeServerImpl::startListening(const std::string& sAppVersion)
{
    return m_gnsServer.startListening(sAppVersion);
}

void PgeServerImpl::sendToAllClientsExcept(const pge_network::PgePacket& pkt, const pge_network::PgeNetworkConnectionHandle& exceptConnHandle)
{
    m_gnsServer.sendToAllClientsExcept(pkt, exceptConnHandle);
}

void PgeServerImpl::sendToAll(const pge_network::PgePacket& pkt)
{
    send(pkt);
    sendToAllClientsExcept(pkt);
}

void PgeServerImpl::setDebugNickname(const pge_network::PgeNetworkConnectionHandle& connHandle, const std::string& sNickname)
{
    m_gnsServer.setClientDebugName(connHandle, sNickname.c_str());
}

int PgeServerImpl::getPing(const pge_network::PgeNetworkConnectionHandle& connHandle, bool bForceUpdate)
{
    return m_gnsServer.getRealTimeStatus(connHandle, bForceUpdate).m_nPing;
}

float PgeServerImpl::getQualityLocal(const pge_network::PgeNetworkConnectionHandle& connHandle, bool bForceUpdate)
{
    return m_gnsServer.getRealTimeStatus(connHandle, bForceUpdate).m_flConnectionQualityLocal;
}

float PgeServerImpl::getQualityRemote(const pge_network::PgeNetworkConnectionHandle& connHandle, bool bForceUpdate)
{
    return m_gnsServer.getRealTimeStatus(connHandle, bForceUpdate).m_flConnectionQualityRemote;
}

float PgeServerImpl::getRxByteRate(const pge_network::PgeNetworkConnectionHandle& connHandle, bool bForceUpdate)
{
    return m_gnsServer.getRealTimeStatus(connHandle, bForceUpdate).m_flInBytesPerSec;
}

float PgeServerImpl::getTxByteRate(const pge_network::PgeNetworkConnectionHandle& connHandle, bool bForceUpdate)
{
    return m_gnsServer.getRealTimeStatus(connHandle, bForceUpdate).m_flOutBytesPerSec;
}

int64_t PgeServerImpl::getPendingUnreliableBytes(const pge_network::PgeNetworkConnectionHandle& connHandle, bool bForceUpdate)
{
    return m_gnsServer.getRealTimeStatus(connHandle, bForceUpdate).m_cbPendingUnreliable;
}

int64_t PgeServerImpl::getPendingReliableBytes(const pge_network::PgeNetworkConnectionHandle& connHandle, bool bForceUpdate)
{
    return m_gnsServer.getRealTimeStatus(connHandle, bForceUpdate).m_cbPendingReliable;
}

int64_t PgeServerImpl::getSentButUnAckedReliableBytes(const pge_network::PgeNetworkConnectionHandle& connHandle, bool bForceUpdate)
{
    return m_gnsServer.getRealTimeStatus(connHandle, bForceUpdate).m_cbSentUnackedReliable;
}

int64_t PgeServerImpl::getInternalQueueTimeUSecs(const pge_network::PgeNetworkConnectionHandle& connHandle, bool bForceUpdate)
{
    return static_cast<int64_t>(m_gnsServer.getRealTimeStatus(connHandle, bForceUpdate).m_usecQueueTime);
}

std::string PgeServerImpl::getDetailedConnectionStatus(const pge_network::PgeNetworkConnectionHandle& connHandle) const
{
    return m_gnsServer.getDetailedConnectionStatus(connHandle);
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PgeServerImpl::PgeServerImpl(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    m_gnsServer(PgeGnsServer::createAndGet(cfgProfiles))
{
    m_gnsServer.getAllowListedPgeMessages().insert(pge_network::MsgClientAppVersionFromClient::id);
    m_gnsServer.getAllowListedPgeMessages().insert(pge_network::MsgApp::id);
} // PgeServerImpl(...)

PgeServerImpl::PgeServerImpl(const PgeServerImpl& other) :
    m_cfgProfiles(other.m_cfgProfiles),
    m_gnsServer(PgeGnsServer::createAndGet(other.m_cfgProfiles))
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
pge_network::PgeIServer& pge_network::PgeServer::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PgeServerImpl serverInst(cfgProfiles);
    return serverInst;
} // createAndGet()


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


