/*
    ###################################################################################
    PgeClient.cpp
    This file is part of PGE.
    PGE Network Main class
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgeClient.h"
#include "PgeGnsClient.h"

/*
   PgeClientImpl
   ###########################################################################
*/

class PgeClientImpl final :
    public pge_network::PgeClient
{
public:
    virtual ~PgeClientImpl();       /**< Calls shutdown(). */

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

    /* implement stuff from PgeClient start */

    bool connectToServer(
        const std::string& sServerAddress,
        const std::string& sAppVersion = "") override; /* temporal */
    const pge_network::PgeNetworkConnectionHandle& getConnectionHandle() const override;
    const pge_network::PgeNetworkConnectionHandle& getConnectionHandleServerSide() const override;
    const char* getServerAddress() const override; 

    int getPing(bool bForceUpdate) override;
    float getQualityLocal(bool bForceUpdate) override;
    float getQualityRemote(bool bForceUpdate) override;
    float getRxByteRate(bool bForceUpdate) override;
    float getTxByteRate(bool bForceUpdate) override;
    int64_t getPendingUnreliableBytes(bool bForceUpdate) override;
    int64_t getPendingReliableBytes(bool bForceUpdate) override;
    int64_t getSentButUnAckedReliableBytes(bool bForceUpdate) override;
    int64_t getInternalQueueTimeUSecs(bool bForceUpdate) override;
    std::string getDetailedConnectionStatus() const override;

    /* implement stuff from PgeClient end */

private:

    // ---------------------------------------------------------------------------

    PGEcfgProfiles& m_cfgProfiles;
    PgeGnsClient& m_gnsClient;

    explicit PgeClientImpl(PGEcfgProfiles& cfgProfiles);
    PgeClientImpl(const PgeClientImpl&);
    PgeClientImpl& operator=(const PgeClientImpl&);

    friend class pge_network::PgeClient;
};


// ############################### PUBLIC ################################


/**
    Calls shutdown().
*/
PgeClientImpl::~PgeClientImpl()
{
    shutdown();
} // ~PgeClient()

/**
    Initialize the client subsystem.

    @return The result of the initialization. True on success, false otherwise.
*/
bool PgeClientImpl::initialize()
{
    return m_gnsClient.init();
} // initialize()

/**
    This stops the client subsystem.
    No effect before initialization.
    After shutdown, initialize() and connectToServer() can be called again.

    @return True on successful shutdown, false otherwise.
*/
bool PgeClientImpl::shutdown()
{
    return m_gnsClient.destroy();
} // shutdown()

/**
    Gets the state of the client subsystem.
    @return True if the client subsystem is successfully connected to a server, false otherwise.
*/
bool PgeClientImpl::isInitialized() const
{
    return m_gnsClient.isInitialized();
} // isInitialized()

/**
    Terminates the active connection of the client instance.

    @param sExtraDebugText An optional text that will be sent to the other side and will be logged to help debugging.
*/
void PgeClientImpl::disconnect(const std::string& sExtraDebugText)
{
    m_gnsClient.disconnectClient(sExtraDebugText);
}

void PgeClientImpl::Update()
{
    if (m_gnsClient.isConnected())
    {
        m_gnsClient.pollIncomingMessages();
    }
    m_gnsClient.pollConnectionStateChanges();  // this may also add packet(s) to m_gnsClient.queuePackets
}

bool PgeClientImpl::pollIncomingMessages()
{
    if (m_gnsClient.isConnected())
    {
        return m_gnsClient.pollIncomingMessages();
    }
    return false;
}

void PgeClientImpl::pollConnectionStateChanges()
{
    return m_gnsClient.pollConnectionStateChanges();
}

std::size_t PgeClientImpl::getPacketQueueSize() const
{
    return m_gnsClient.getPacketQueueSize();
}

pge_network::PgePacket PgeClientImpl::popFrontPacket() noexcept(false)
{
    return m_gnsClient.popFrontPacket();
}

std::set<pge_network::PgePktId>& PgeClientImpl::getAllowListedPgeMessages()
{
    return m_gnsClient.getAllowListedPgeMessages();
}

std::set<pge_network::MsgApp::TMsgId>& PgeClientImpl::getAllowListedAppMessages()
{
    return m_gnsClient.getAllowListedAppMessages();
}

void PgeClientImpl::send(const pge_network::PgePacket& pkt, const pge_network::PgeNetworkConnectionHandle& connHandle)
{
    if (connHandle != pge_network::ServerConnHandle)
    {
        getConsole().EOLn("%s: CLIENT target connHandle must be %u!", __func__, pge_network::ServerConnHandle);
        return;
    }
    m_gnsClient.sendToServer(pkt);
}

uint32_t PgeClientImpl::getRxPacketCount() const
{
    return m_gnsClient.getRxPacketCount();
}

uint32_t PgeClientImpl::getTxPacketCount() const
{
    return m_gnsClient.getTxPacketCount();
}

uint32_t PgeClientImpl::getInjectPacketCount() const
{
    return m_gnsClient.getInjectPacketCount();
}

uint32_t PgeClientImpl::getRxPacketPerSecondCount() const
{
    return m_gnsClient.getRxPacketPerSecondCount();
}

uint32_t PgeClientImpl::getTxPacketPerSecondCount() const
{
    return m_gnsClient.getTxPacketPerSecondCount();
}

uint32_t PgeClientImpl::getInjectPacketPerSecondCount() const
{
    return m_gnsClient.getInjectPacketPerSecondCount();
}

const std::map<pge_network::MsgApp::TMsgId, uint32_t>& PgeClientImpl::getRxMsgCount() const
{
    return m_gnsClient.getRxMsgCount();
}

const std::map<pge_network::MsgApp::TMsgId, uint32_t>& PgeClientImpl::getTxMsgCount() const
{
    return m_gnsClient.getTxMsgCount();
}

const std::map<pge_network::MsgApp::TMsgId, uint32_t>& PgeClientImpl::getInjectMsgCount() const
{
    return m_gnsClient.getInjectMsgCount();
}

std::map<pge_network::MsgApp::TMsgId, std::string>& PgeClientImpl::getMsgAppId2StringMap()
{
    return m_gnsClient.getMsgAppId2StringMap();
}

uint32_t PgeClientImpl::getRxByteCount() const
{
    return m_gnsClient.getRxByteCount();
}

uint32_t PgeClientImpl::getTxByteCount() const
{
    return m_gnsClient.getTxByteCount();
}

uint32_t PgeClientImpl::getInjectByteCount() const
{
    return m_gnsClient.getInjectByteCount();
}

void PgeClientImpl::WriteList() const
{
    getConsole().OLnOI("PgeClient::WriteList() start");
    if (isInitialized())
    {
        getConsole().OLn("Role: Client");
        if (getConnectionHandle() == pge_network::ServerConnHandle)
        {
            getConsole().EOLn("Connection Handle: Invalid");
        }
        else
        {
            getConsole().OLn("Connection Handle: %u", static_cast<uint32_t>(getConnectionHandle()));
            getConsole().OLn("Server Address: %s", getServerAddress());
        }
    }
    else
    {
        getConsole().OLn("PgeClient is NOT initialized!");
    }

    getConsole().OOOLn("PgeClient::WriteList() end");
} // WriteList()

const pge_network::PgeNetworkConnectionHandle& PgeClientImpl::getConnectionHandle() const
{
    return static_cast<pge_network::PgeNetworkConnectionHandle>(m_gnsClient.getConnectionHandle());
}

const pge_network::PgeNetworkConnectionHandle& PgeClientImpl::getConnectionHandleServerSide() const
{
    return static_cast<pge_network::PgeNetworkConnectionHandle>(m_gnsClient.getConnectionHandleServerSide());
}

const char* PgeClientImpl::getServerAddress() const
{
    return m_gnsClient.getServerAddress();
}

bool PgeClientImpl::connectToServer(const std::string& sServerAddress, const std::string& sAppVersion)
{
    return m_gnsClient.connectToServer(sServerAddress, sAppVersion);
}

int PgeClientImpl::getPing(bool bForceUpdate)
{
    return m_gnsClient.getRealTimeStatus(bForceUpdate).m_nPing;
}

float PgeClientImpl::getQualityLocal(bool bForceUpdate)
{
    return m_gnsClient.getRealTimeStatus(bForceUpdate).m_flConnectionQualityLocal;
}

float PgeClientImpl::getQualityRemote(bool bForceUpdate)
{
    return m_gnsClient.getRealTimeStatus(bForceUpdate).m_flConnectionQualityRemote;
}

float PgeClientImpl::getRxByteRate(bool bForceUpdate)
{
    return m_gnsClient.getRealTimeStatus(bForceUpdate).m_flInBytesPerSec;
}

float PgeClientImpl::getTxByteRate(bool bForceUpdate)
{
    return m_gnsClient.getRealTimeStatus(bForceUpdate).m_flOutBytesPerSec;
}

int64_t PgeClientImpl::getPendingUnreliableBytes(bool bForceUpdate)
{
    return m_gnsClient.getRealTimeStatus(bForceUpdate).m_cbPendingUnreliable;
}

int64_t PgeClientImpl::getPendingReliableBytes(bool bForceUpdate)
{
    return m_gnsClient.getRealTimeStatus(bForceUpdate).m_cbPendingReliable;
}

int64_t PgeClientImpl::getSentButUnAckedReliableBytes(bool bForceUpdate)
{
    return m_gnsClient.getRealTimeStatus(bForceUpdate).m_cbSentUnackedReliable;
}

int64_t PgeClientImpl::getInternalQueueTimeUSecs(bool bForceUpdate)
{
    return static_cast<int64_t>(m_gnsClient.getRealTimeStatus(bForceUpdate).m_usecQueueTime);
}

std::string PgeClientImpl::getDetailedConnectionStatus() const
{
    return m_gnsClient.getDetailedConnectionStatus();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PgeClientImpl::PgeClientImpl(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    m_gnsClient(PgeGnsClient::createAndGet(cfgProfiles))
{
    m_gnsClient.getAllowListedPgeMessages().insert(pge_network::MsgUserDisconnectedFromServer::id);
    m_gnsClient.getAllowListedPgeMessages().insert(pge_network::MsgApp::id);
} // PgeClientImpl(...)

PgeClientImpl::PgeClientImpl(const PgeClientImpl& other) :
    m_cfgProfiles(other.m_cfgProfiles),
    m_gnsClient(PgeGnsClient::createAndGet(other.m_cfgProfiles))
{
}

PgeClientImpl& PgeClientImpl::operator=(const PgeClientImpl&)
{
    return *this;
}


/*
   PgeClient
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
pge_network::PgeIClient& pge_network::PgeClient::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PgeClientImpl clientInst(cfgProfiles);
    return clientInst;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& pge_network::PgeClient::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################

