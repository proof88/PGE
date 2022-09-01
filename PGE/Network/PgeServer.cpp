/*
    ###################################################################################
    PgeServer.cpp
    This file is part of PGE.
    PGE Network Main class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH

#include "PgeServer.h"
#include "PGESysNET.h"

/*
   PgeServerImpl
   ###########################################################################
*/

class PgeServerImpl :
    public pge_network::PgeServer
{
public:
    virtual ~PgeServerImpl();       /**< Calls shutdown(). */

    bool initialize() override;
    bool shutdown() override;
    bool isInitialized() const override;

    void Update() override;
    bool startListening();
    void SendPacketToClient(pge_network::PgeNetworkConnectionHandle connHandle, const pge_network::PgePacket& pkt) override;
    void SendPacketToAllClients(const pge_network::PgePacket& pkt, pge_network::PgeNetworkConnectionHandle exceptConnHandle = 0) override;
    std::deque<pge_network::PgePacket>& getPacketQueue() override;  // TODO: TEMPORAL: obviously we should not allow this kind of access
    std::set<pge_network::TPgeMsgAppMsgId>& getBlackListedMessages() override;

    void WriteList() const override;

private:

    // ---------------------------------------------------------------------------

    PGESysNET& m_PgeSysNET;

    PgeServerImpl();                /**< NULLs members only. */
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
    return true;
} // initialize()


/**
    This stops the server subsystem.
    No effect before initialization.
    After shutdown, initialize() and startListening() can be called again.

    @return True on successful shutdown, false otherwise.
*/
bool PgeServerImpl::shutdown()
{
    return m_PgeSysNET.StopListening();
} // shutdown()

/**
    Gets the state of the server subsystem.
    @return True if the server subsystem is successfully initialized and listening, false otherwise.
*/
bool PgeServerImpl::isInitialized() const
{
    return m_PgeSysNET.isInitialized();
} // isInitialized()

void PgeServerImpl::Update()
{
    m_PgeSysNET.PollIncomingMessages();
    m_PgeSysNET.PollConnectionStateChanges();  // this may also add packet(s) to SysNET.queuePackets
}

bool PgeServerImpl::startListening()
{
    return m_PgeSysNET.startListening();
}

void PgeServerImpl::SendPacketToClient(pge_network::PgeNetworkConnectionHandle connHandle, const pge_network::PgePacket& pkt)
{
    // TODO add check: connHandle cannot be 0!
    m_PgeSysNET.SendPacketToClient(static_cast<HSteamNetConnection>(connHandle), pkt);
}

void PgeServerImpl::SendPacketToAllClients(const pge_network::PgePacket& pkt, pge_network::PgeNetworkConnectionHandle exceptConnHandle)
{
    m_PgeSysNET.SendPacketToAllClients(pkt, exceptConnHandle);
}

std::deque<pge_network::PgePacket>& PgeServerImpl::getPacketQueue()
{
    return m_PgeSysNET.m_queuePackets;
}

std::set<pge_network::TPgeMsgAppMsgId>& PgeServerImpl::getBlackListedMessages()
{
    return m_PgeSysNET.m_blackListedMessages;
}

/**
    Writes statistics to console.
*/
void PgeServerImpl::WriteList() const
{
    getConsole().OLnOI("PgeServer::WriteList() start");
    getConsole().OLnOI("");
    if (isInitialized())
    {
        // TODO
    }
    else
    {
        getConsole().OLn("PgeServer is NOT initialized!");
    }

    getConsole().OLnOO("PgeServer::WriteList() end");
    getConsole().OLn("");
} // WriteList()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    NULLs members only.
*/
PgeServerImpl::PgeServerImpl() :
    m_PgeSysNET(PGESysNET::createAndGet())
{

} // PgeServerImpl(...)


PgeServerImpl::PgeServerImpl(const PgeServerImpl&) :
    m_PgeSysNET(PGESysNET::createAndGet())
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
pge_network::PgeServer& pge_network::PgeServer::createAndGet()
{
    static PgeServerImpl serverInst;
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


