/*
    ###################################################################################
    PgeClient.cpp
    This file is part of PGE.
    PGE Network Main class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgeClient.h"
#include "PgeGsnClient.h"

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

    void Update() override;

    bool pollIncomingMessages() override;
    void pollConnectionStateChanges() override;

    std::size_t getPacketQueueSize() const override;
    pge_network::PgePacket popFrontPacket() noexcept(false) override;

    std::set<pge_network::PgePktId>& getAllowListedPgeMessages() override;
    std::set<pge_network::TPgeMsgAppMsgId>& getAllowListedAppMessages() override;

    void send(const pge_network::PgePacket& pkt, const pge_network::PgeNetworkConnectionHandle& connHandle = pge_network::ServerConnHandle) override;

    uint32_t getRxPacketCount() const override;
    uint32_t getTxPacketCount() const override;
    uint32_t getInjectPacketCount() const override;

    uint32_t getRxPacketPerSecondCount() const override;
    uint32_t getTxPacketPerSecondCount() const override;
    uint32_t getInjectPacketPerSecondCount() const override;

    void WriteList() const override;

    /* implement stuff from PgeIServerClient end */

    /* implement stuff from PgeClient start */

    bool connectToServer(const std::string& sServerAddress) override; /* temporal */
    const pge_network::PgeNetworkConnectionHandle& getConnectionHandle() const override;
    const pge_network::PgeNetworkConnectionHandle& getConnectionHandleServerSide() const override;
    const char* getServerAddress() const override;  
    int getPing(bool bForceUpdate) override;
    float getQualityLocal(bool bForceUpdate) override;
    float getQualityRemote(bool bForceUpdate) override;
    float getRxByteRate(bool bForceUpdate) override;
    float getTxByteRate(bool bForceUpdate) override;
    int64_t getInternalQueueTimeUSecs(bool bForceUpdate) override;
    std::string getDetailedStatus() const override;

    /* implement stuff from PgeClient end */

private:

    // ---------------------------------------------------------------------------

    PGEcfgProfiles& m_cfgProfiles;
    PgeGsnClient& m_gsnClient;

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
    return m_gsnClient.init();
} // initialize()

/**
    This stops the client subsystem.
    No effect before initialization.
    After shutdown, initialize() and connectToServer() can be called again.

    @return True on successful shutdown, false otherwise.
*/
bool PgeClientImpl::shutdown()
{
    return m_gsnClient.destroy();
} // shutdown()

/**
    Gets the state of the client subsystem.
    @return True if the client subsystem is successfully connected to a server, false otherwise.
*/
bool PgeClientImpl::isInitialized() const
{
    return m_gsnClient.isInitialized();
} // isInitialized()

void PgeClientImpl::Update()
{
    m_gsnClient.pollIncomingMessages();
    m_gsnClient.pollConnectionStateChanges();  // this may also add packet(s) to m_gsnClient.queuePackets
}

bool PgeClientImpl::pollIncomingMessages()
{
    return m_gsnClient.pollIncomingMessages();
}

void PgeClientImpl::pollConnectionStateChanges()
{
    return m_gsnClient.pollConnectionStateChanges();
}

std::size_t PgeClientImpl::getPacketQueueSize() const
{
    return m_gsnClient.getPacketQueueSize();
}

pge_network::PgePacket PgeClientImpl::popFrontPacket() noexcept(false)
{
    return m_gsnClient.popFrontPacket();
}

std::set<pge_network::PgePktId>& PgeClientImpl::getAllowListedPgeMessages()
{
    return m_gsnClient.getAllowListedPgeMessages();
}

std::set<pge_network::TPgeMsgAppMsgId>& PgeClientImpl::getAllowListedAppMessages()
{
    return m_gsnClient.getAllowListedAppMessages();
}

void PgeClientImpl::send(const pge_network::PgePacket& pkt, const pge_network::PgeNetworkConnectionHandle& connHandle)
{
    if (connHandle != pge_network::ServerConnHandle)
    {
        getConsole().EOLn("%s: CLIENT target connHandle must be %u!", __func__, pge_network::ServerConnHandle);
        return;
    }
    m_gsnClient.sendToServer(pkt);
}

uint32_t PgeClientImpl::getRxPacketCount() const
{
    return m_gsnClient.getRxPacketCount();
}

uint32_t PgeClientImpl::getTxPacketCount() const
{
    return m_gsnClient.getTxPacketCount();
}

uint32_t PgeClientImpl::getInjectPacketCount() const
{
    return m_gsnClient.getInjectPacketCount();
}

uint32_t PgeClientImpl::getRxPacketPerSecondCount() const
{
    return m_gsnClient.getRxPacketPerSecondCount();
}

uint32_t PgeClientImpl::getTxPacketPerSecondCount() const
{
    return m_gsnClient.getTxPacketPerSecondCount();
}

uint32_t PgeClientImpl::getInjectPacketPerSecondCount() const
{
    return m_gsnClient.getInjectPacketPerSecondCount();
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
    return static_cast<pge_network::PgeNetworkConnectionHandle>(m_gsnClient.getConnectionHandle());
}

const pge_network::PgeNetworkConnectionHandle& PgeClientImpl::getConnectionHandleServerSide() const
{
    return static_cast<pge_network::PgeNetworkConnectionHandle>(m_gsnClient.getConnectionHandleServerSide());
}

const char* PgeClientImpl::getServerAddress() const
{
    return m_gsnClient.getServerAddress();
}

bool PgeClientImpl::connectToServer(const std::string& sServerAddress)
{
    return m_gsnClient.connectToServer(sServerAddress);
}

int PgeClientImpl::getPing(bool bForceUpdate)
{
    return m_gsnClient.getRealTimeStatus(bForceUpdate).m_nPing;
}

float PgeClientImpl::getQualityLocal(bool bForceUpdate)
{
    return m_gsnClient.getRealTimeStatus(bForceUpdate).m_flConnectionQualityLocal;
}

float PgeClientImpl::getQualityRemote(bool bForceUpdate)
{
    return m_gsnClient.getRealTimeStatus(bForceUpdate).m_flConnectionQualityRemote;
}

float PgeClientImpl::getRxByteRate(bool bForceUpdate)
{
    return m_gsnClient.getRealTimeStatus(bForceUpdate).m_flInBytesPerSec;
}

float PgeClientImpl::getTxByteRate(bool bForceUpdate)
{
    return m_gsnClient.getRealTimeStatus(bForceUpdate).m_flOutBytesPerSec;
}

int64_t PgeClientImpl::getInternalQueueTimeUSecs(bool bForceUpdate)
{
    return static_cast<int64_t>(m_gsnClient.getRealTimeStatus(bForceUpdate).m_usecQueueTime);
}

std::string PgeClientImpl::getDetailedStatus() const
{
    return m_gsnClient.getDetailedStatus();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PgeClientImpl::PgeClientImpl(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    m_gsnClient(PgeGsnClient::createAndGet(cfgProfiles))
{
    m_gsnClient.getAllowListedPgeMessages().insert(pge_network::MsgUserDisconnected::id);
    m_gsnClient.getAllowListedPgeMessages().insert(pge_network::MsgApp::id);
} // PgeClientImpl(...)

PgeClientImpl::PgeClientImpl(const PgeClientImpl& other) :
    m_cfgProfiles(other.m_cfgProfiles),
    m_gsnClient(PgeGsnClient::createAndGet(other.m_cfgProfiles))
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
pge_network::PgeClient& pge_network::PgeClient::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PgeClientImpl clientInst(cfgProfiles);
    return clientInst;
} // createAndGet()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* pge_network::PgeClient::getLoggerModuleName()
{
    return "PgeClient";
} // getLoggerModuleName()


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

