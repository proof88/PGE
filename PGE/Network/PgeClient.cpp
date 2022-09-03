/*
    ###################################################################################
    PgeClient.cpp
    This file is part of PGE.
    PGE Network Main class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH

#include "PgeClient.h"
#include "PGESysNET.h"

/*
   PgeClientImpl
   ###########################################################################
*/

class PgeClientImpl :
    public pge_network::PgeClient
{
public:
    virtual ~PgeClientImpl();       /**< Calls shutdown(). */

    bool initialize() override;
    bool shutdown() override;
    bool isInitialized() const override;

    const pge_network::PgeNetworkConnectionHandle& getConnectionHandle() const;
    const char* getServerAddress() const;

    void Update() override;
    bool connectToServer(const std::string& sServerAddress) override; /* temporal */
    void SendToServer(const pge_network::PgePacket& pkt) override;
    std::deque<pge_network::PgePacket>& getPacketQueue() override;  // TODO: TEMPORAL: obviously we should not allow this kind of access
    std::set<pge_network::PgePktId>& getBlackListedPgeMessages();
    std::set<pge_network::TPgeMsgAppMsgId>& getBlackListedAppMessages();

    int getPing(bool bForceUpdate) override;
    float getQualityLocal(bool bForceUpdate) override;
    float getQualityRemote(bool bForceUpdate) override;
    float getRxByteRate(bool bForceUpdate) override;
    float getTxByteRate(bool bForceUpdate) override;
    int64_t getInternalQueueTimeUSecs(bool bForceUpdate) override;

    void WriteList() const override;

private:

    // ---------------------------------------------------------------------------

    PGESysNET& m_PgeSysNET;

    PgeClientImpl();                /**< NULLs members only. */
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
    return true;
} // initialize()


/**
    This stops the client subsystem.
    No effect before initialization.
    After shutdown, initialize() and connectToServer() can be called again.

    @return True on successful shutdown, false otherwise.
*/
bool PgeClientImpl::shutdown()
{
    return m_PgeSysNET.DisconnectClient();
} // shutdown()

/**
    Gets the state of the client subsystem.
    @return True if the client subsystem is successfully connected to a server, false otherwise.
*/
bool PgeClientImpl::isInitialized() const
{
    return m_PgeSysNET.isInitialized();
} // isInitialized()

const pge_network::PgeNetworkConnectionHandle& PgeClientImpl::getConnectionHandle() const
{
    return static_cast<pge_network::PgeNetworkConnectionHandle>(m_PgeSysNET.getConnectionHandle());
}

const char* PgeClientImpl::getServerAddress() const
{
    return m_PgeSysNET.getServerAddress();
}

void PgeClientImpl::Update()
{
    m_PgeSysNET.PollIncomingMessages();
    m_PgeSysNET.PollConnectionStateChanges();  // this may also add packet(s) to SysNET.queuePackets
}

bool PgeClientImpl::connectToServer(const std::string& sServerAddress)
{
    return m_PgeSysNET.connectToServer(sServerAddress);
}

void PgeClientImpl::SendToServer(const pge_network::PgePacket& pkt)
{
    m_PgeSysNET.SendToServer(pkt);
}

std::deque<pge_network::PgePacket>& PgeClientImpl::getPacketQueue()
{
    return m_PgeSysNET.getPacketQueue();
}

std::set<pge_network::PgePktId>& PgeClientImpl::getBlackListedPgeMessages()
{
    return m_PgeSysNET.getBlackListedPgeMessages();
}

std::set<pge_network::TPgeMsgAppMsgId>& PgeClientImpl::getBlackListedAppMessages()
{
    return m_PgeSysNET.getBlackListedAppMessages();
}

int PgeClientImpl::getPing(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_nPing;
}

float PgeClientImpl::getQualityLocal(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_flConnectionQualityLocal;
}

float PgeClientImpl::getQualityRemote(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_flConnectionQualityRemote;
}

float PgeClientImpl::getRxByteRate(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_flInBytesPerSec;
}

float PgeClientImpl::getTxByteRate(bool bForceUpdate)
{
    return m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_flOutBytesPerSec;
}

int64_t PgeClientImpl::getInternalQueueTimeUSecs(bool bForceUpdate)
{
    return static_cast<int64_t>(m_PgeSysNET.getRealTimeStatus(bForceUpdate).m_usecQueueTime);
}

/**
    Writes statistics to console.
*/
void PgeClientImpl::WriteList() const
{
    getConsole().OLnOI("PgeClient::WriteList() start");
    if (isInitialized())
    {
        getConsole().OLn("Role: Client");
        if (getConnectionHandle() == 0)
        {
            getConsole().EOLn("Connection Handle: Invalid");
        }
        else
        {
            getConsole().SOLn("Connection Handle: %u", static_cast<uint32_t>(getConnectionHandle()));
            getConsole().OLn("Server Address: %s", getServerAddress());
        }
        getConsole().OLn("");
    }
    else
    {
        getConsole().OLn("PgeClient is NOT initialized!");
    }

    getConsole().OOOLn("PgeClient::WriteList() end");
    getConsole().OLn("");
} // WriteList()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    NULLs members only.
*/
PgeClientImpl::PgeClientImpl() :
    m_PgeSysNET(PGESysNET::createAndGet())
{
    m_PgeSysNET.getBlackListedPgeMessages().insert(pge_network::PgeMsgUserConnected::id);
} // PgeClientImpl(...)


PgeClientImpl::PgeClientImpl(const PgeClientImpl&) :
    m_PgeSysNET(PGESysNET::createAndGet())
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
pge_network::PgeClient& pge_network::PgeClient::createAndGet()
{
    static PgeClientImpl clientInst;
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

