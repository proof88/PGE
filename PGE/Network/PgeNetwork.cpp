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
    public PgeNetwork
{
public:
    virtual ~PgeNetworkImpl();       /**< Calls shutdown(). */

    bool initialize() override;
    bool shutdown() override;
    bool isInitialized() const override;

    bool isServer() const override;
    void Update() override;
    bool ConnectClient() override; /* temporal */
    void SendStringToClient(const char* str) override;
    void SendPacketToClient(const PgePacket& pkt) override;
    void SendStringToAllClients(const char* str) override;
    void SendPacketToAllClients(const PgePacket& pkt) override;
    void SendPacketToServer(const PgePacket& pkt) override;
    std::deque<PgePacket>& getPacketQueue() override;  // TODO: TEMPORAL: obviously we should not allow this kind of access
    std::map<std::string, Player_t>& getPlayers() override;

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
    std::map<std::string, Player_t> m_mapPlayers;  // used by both server and clients

    PgeNetworkImpl();                /**< NULLs members only. */
    PgeNetworkImpl(const PgeNetworkImpl&);
    PgeNetworkImpl& operator=(const PgeNetworkImpl&);

    friend class PgeNetwork;
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

bool PgeNetworkImpl::ConnectClient()
{
    return m_PgeSysNET.ConnectClient();
}

void PgeNetworkImpl::SendStringToClient(const char* str)
{
    m_PgeSysNET.SendStringToClient(m_PgeSysNET.m_hConnection, str);
}

void PgeNetworkImpl::SendPacketToClient(const PgePacket& pkt)
{
    m_PgeSysNET.SendPacketToClient(m_PgeSysNET.m_hConnection, pkt);
}

void PgeNetworkImpl::SendStringToAllClients(const char* str)
{
    m_PgeSysNET.SendStringToAllClients(str);
}

void PgeNetworkImpl::SendPacketToAllClients(const PgePacket& pkt)
{
    m_PgeSysNET.SendPacketToAllClients(pkt);
}

void PgeNetworkImpl::SendPacketToServer(const PgePacket& pkt)
{
    m_PgeSysNET.SendPacketToServer(pkt);
}

std::deque<PgePacket>& PgeNetworkImpl::getPacketQueue()
{
    return m_PgeSysNET.queuePackets;
}

std::map<std::string, Player_t>& PgeNetworkImpl::getPlayers()
{
    return m_mapPlayers;
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
PgeNetwork& PgeNetwork::createAndGet()
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
const char* PgeNetwork::getLoggerModuleName()
{
    return "PgeNetwork";
} // getLoggerModuleName()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PgeNetwork::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


