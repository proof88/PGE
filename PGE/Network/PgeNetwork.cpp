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
    bool ConnectClient(const std::string& sServerAddress) override; /* temporal */
    bool StartListening();
    void SendStringToClient(pge_network::PgeNetworkConnectionHandle connHandle, const char* str) override;
    void SendPacketToClient(pge_network::PgeNetworkConnectionHandle connHandle, const pge_network::PgePacket& pkt) override;
    void SendStringToAllClients(const char* str, pge_network::PgeNetworkConnectionHandle exceptConnHandle = 0) override;
    void SendPacketToAllClients(const pge_network::PgePacket& pkt, pge_network::PgeNetworkConnectionHandle exceptConnHandle = 0) override;
    void SendPacketToServer(const pge_network::PgePacket& pkt) override;
    std::deque<pge_network::PgePacket>& getPacketQueue() override;  // TODO: TEMPORAL: obviously we should not allow this kind of access
    std::set<uint32_t>& getBlackListedMessages() override;

    int getPing(bool bForceUpdate) override;
    float getQualityLocal(bool bForceUpdate) override;
    float getQualityRemote(bool bForceUpdate) override;
    float getRxByteRate(bool bForceUpdate) override;
    float getTxByteRate(bool bForceUpdate) override;
    int64_t getInternalQueueTimeUSecs(bool bForceUpdate) override;

    void WriteList() const override;

private:

    // ---------------------------------------------------------------------------

    PGESysNET m_PgeSysNET;

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
    return m_PgeSysNET.initSysNET();
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
    m_PgeSysNET.PollIncomingMessages();
    m_PgeSysNET.PollConnectionStateChanges();  // this may also add packet(s) to SysNET.queuePackets
}

bool PgeNetworkImpl::ConnectClient(const std::string& sServerAddress)
{
    return m_PgeSysNET.ConnectClient(sServerAddress);
}

bool PgeNetworkImpl::StartListening()
{
    return m_PgeSysNET.StartListening();
}

void PgeNetworkImpl::SendStringToClient(pge_network::PgeNetworkConnectionHandle connHandle, const char* str)
{
    // TODO add check: connHandle cannot be 0!
    m_PgeSysNET.SendStringToClient(static_cast<HSteamNetConnection>(connHandle), str);
}

void PgeNetworkImpl::SendPacketToClient(pge_network::PgeNetworkConnectionHandle connHandle, const pge_network::PgePacket& pkt)
{
    // TODO add check: connHandle cannot be 0!
    m_PgeSysNET.SendPacketToClient(static_cast<HSteamNetConnection>(connHandle), pkt);
}

void PgeNetworkImpl::SendStringToAllClients(const char* str, pge_network::PgeNetworkConnectionHandle exceptConnHandle)
{
    m_PgeSysNET.SendStringToAllClients(str, exceptConnHandle);
}

void PgeNetworkImpl::SendPacketToAllClients(const pge_network::PgePacket& pkt, pge_network::PgeNetworkConnectionHandle exceptConnHandle)
{
    m_PgeSysNET.SendPacketToAllClients(pkt, exceptConnHandle);
}

void PgeNetworkImpl::SendPacketToServer(const pge_network::PgePacket& pkt)
{
    m_PgeSysNET.SendPacketToServer(pkt);
}

std::deque<pge_network::PgePacket>& PgeNetworkImpl::getPacketQueue()
{
    return m_PgeSysNET.m_queuePackets;
}

std::set<pge_network::TPgeMsgAppMsgId>& PgeNetworkImpl::getBlackListedMessages()
{
    return m_PgeSysNET.m_blackListedMessages;
}

int PgeNetworkImpl::getPing(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_nPing;
}

float PgeNetworkImpl::getQualityLocal(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_flConnectionQualityLocal;
}

float PgeNetworkImpl::getQualityRemote(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_flConnectionQualityRemote;
}

float PgeNetworkImpl::getRxByteRate(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_flInBytesPerSec;
}

float PgeNetworkImpl::getTxByteRate(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_flOutBytesPerSec;
}

int64_t PgeNetworkImpl::getInternalQueueTimeUSecs(bool bForceUpdate)
{
    return static_cast<int64_t>(m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_usecQueueTime);
}


/**
    Writes statistics to console.
*/
void PgeNetworkImpl::WriteList() const
{
    getConsole().OLnOI("PgeNetwork::WriteList() start");
    getConsole().OLnOI("");
    if (isInitialized())
    {
        // TODO
    }
    else
    {
        getConsole().OLn("PgeNetwork is NOT initialized!");
    }

    getConsole().OLnOO("PgeNetwork::WriteList() end");
    getConsole().OLn("");
} // WriteList()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    NULLs members only.
*/
PgeNetworkImpl::PgeNetworkImpl() //:
    //  ssets(PRRESharedSettings::createAndGet()),
{

} // PgeNetworkImpl(...)


PgeNetworkImpl::PgeNetworkImpl(const PgeNetworkImpl&) //:
    //  ssets(PRRESharedSettings::createAndGet()),
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
    static PgeNetworkImpl PRREinstance;
    return PRREinstance;
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


