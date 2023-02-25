/*
    ###################################################################################
    PGESysNET.cpp
    This file is part of PGE.
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include <cassert>
#include <filesystem>  // requires cpp17
#include <set>

#include "PGESysNET.h"
#include "../PGEincludes.h"
#include "../PGEpragmas.h"


/*
   PGESysNET
   ###########################################################################
*/


// ############################### PUBLIC ################################

static constexpr char* CVAR_NET_SERVER = "net_server";

const uint16 PGESysNET::DEFAULT_SERVER_PORT;

/**
    Creates and gets the singleton instance.
*/
PGESysNET& PGESysNET::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PGESysNET inst(cfgProfiles);
    return inst;
} // createAndGet()

bool PGESysNET::isServer()
{
    return s_bServer;
}


PGESysNET::~PGESysNET()
{
    destroySysNET();
} // ~PGESysNET()


// TODO: move this down to private area later ...
static void NetworkDbg(ESteamNetworkingSocketsDebugOutputType eType, const char* pszMsg)
{
    if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug)
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("%s: %s", __func__, pszMsg);
    }
    else
    {
        CConsole::getConsoleInstance("PGESysNET").OLn("%s: %s", __func__, pszMsg);
    }
}


bool PGESysNET::initSysNET(void)
{
    SteamDatagramErrMsg errMsg;

    if ( GameNetworkingSockets_Init(nullptr, errMsg) )
    {
        CConsole::getConsoleInstance("PGESysNET").SOLn("Initialized GameNetworkingSockets %s!", GAMENETWORKINGSOCKETS_VER_STR);
    }
    else
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("Failed to initialize GameNetworkingSockets %s: %s!", GAMENETWORKINGSOCKETS_VER_STR, errMsg);
        return false;
    }

    if (m_cfgProfiles.getVars()[CVAR_NET_SERVER].getAsString().empty())
    {
        s_bServer = (IDYES == MessageBox(0, "Want to be a Server?", ":)", MB_YESNO | MB_ICONQUESTION | MB_SETFOREGROUND));
    }
    else
    {
        s_bServer = m_cfgProfiles.getVars()[CVAR_NET_SERVER].getAsBool();
        CConsole::getConsoleInstance("PGESysNET").OLn("s_bServer from config: %b", s_bServer);
    }

    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, NetworkDbg);

    // Select instance to use. For now we'll always use the default.
    // But we could use SteamGameServerNetworkingSockets() on Steam.
    m_pInterface = SteamNetworkingSockets();

    // now main engine loop can invoke PollIncomingMessages() and PollConnectionStateChanges()

    return true;
} // initSysNET()


bool PGESysNET::destroySysNET(void)
{
    if (!m_pInterface)
    {
        return true;
    }

    if (isServer())
    {
        stopListening();
    }
    else
    {
        CConsole::getConsoleInstance("PGESysNET").OLn("Detailed Connection Status:");
        CConsole::getConsoleInstance("PGESysNET").OLn("%s", getDetailedStatus().c_str());
        DisconnectClient();
    }
    m_pInterface = nullptr;

    const auto nSecsSince1stTxPkt =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_time1stTxPkt).count();
    const auto nSecsSince1stRxPkt =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_time1stRxPkt).count();
    const auto nSecsSince1stInjectPkt =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_time1stInjectPkt).count();

    CConsole::getConsoleInstance("PGESysNET").OLn("");
    CConsole::getConsoleInstance("PGESysNET").OLn("Total Tx'd Pkt Count : %u, %u pkt/s", getTxPacketCount(), getTxPacketCount() / nSecsSince1stTxPkt);
    CConsole::getConsoleInstance("PGESysNET").OLn("Total Rx'd Pkt Count : %u, %u pkt/s", getRxPacketCount(), getRxPacketCount() / nSecsSince1stRxPkt);
    CConsole::getConsoleInstance("PGESysNET").OLn("Total Inj'd Pkt Count: %u, %u pkt/s", getInjectPacketCount(), getInjectPacketCount() / nSecsSince1stInjectPkt);

    GameNetworkingSockets_Kill();  // hopefully this can be invoked even if GNS has been already killed
    return true;
} // destroySysNET()

bool PGESysNET::isInitialized() const
{
    if (isServer())
    {
        return m_hListenSock != k_HSteamListenSocket_Invalid;
    }
    else
    {
        return m_hConnection != k_HSteamNetConnection_Invalid;
    }
} // isInitialized()

const HSteamNetConnection& PGESysNET::getConnectionHandle() const
{
    return m_hConnection;
}

const HSteamNetConnection& PGESysNET::getConnectionHandleServerSide() const
{
    return m_hConnectionServerSide;
}

const char* PGESysNET::getServerAddress() const
{
    return m_szAddr;
}

bool PGESysNET::PollIncomingMessages()
{
    static const int nIncomingMsgArraySize = 10; // TODO: make this configurable from outside
    ISteamNetworkingMessage* pIncomingMsg[nIncomingMsgArraySize];
    if (isServer())
    {
        // ReceiveMessagesOnPollGroup() basically copies the pointers to messages from GNS's internal linked list,
        // and unlinks these from the linked list
        int numMsgs = m_pInterface->ReceiveMessagesOnPollGroup(m_hPollGroup, pIncomingMsg, nIncomingMsgArraySize);
        if (numMsgs == 0)
        {
            return false;                              
        }

        if (numMsgs < 0)
        {
            CConsole::getConsoleInstance("PGESysNET").EOLn("%s: SERVER Error checking for messages!", __func__);
            return false;
        }

        if (!pIncomingMsg)
        {
            CConsole::getConsoleInstance("PGESysNET").EOLn("%s: SERVER pIncomingMsg null!", __func__);
            return false;
        }

        for (int i = 0; i < numMsgs; i++)
        {
            auto itClient = m_mapClients.find(pIncomingMsg[i]->m_conn);
            if (itClient == m_mapClients.end())
            {
                CConsole::getConsoleInstance("PGESysNET").EOLn("%s: SERVER failed to find connection %u in m_mapClients!",
                    __func__, pIncomingMsg[i]->m_conn);
                continue;
            }

            pge_network::PgePacket pkt;
            assert((pIncomingMsg[i])->m_cbSize == sizeof(pkt));
            memcpy(&pkt, (pIncomingMsg[i])->m_pData, (pIncomingMsg[i])->m_cbSize);
            pkt.m_connHandleServerSide = static_cast<pge_network::PgeNetworkConnectionHandle>(pIncomingMsg[i]->m_conn);

            // We don't need this anymore.
            // Note that we could even push pIncomingMsg to a queue, and process it later, and
            // release it later, that could be a good speed optimization.
            (pIncomingMsg[i])->Release();

            if (pkt.pktId == pge_network::PgePktId::APP)
            {
                if (m_blackListedAppMessages.end() != m_blackListedAppMessages.find(pkt.msg.app.msgId))
                {
                    CConsole::getConsoleInstance("PGESysNET").EOLn("%s: SERVER blacklisted app message received: %u from connection %u!",
                        __func__, pkt.msg.app.msgId, pkt.m_connHandleServerSide);
                    assert(false);
                    continue;
                }
            }
            else
            {
                if (m_blackListedPgeMessages.end() != m_blackListedPgeMessages.find(pkt.pktId))
                {
                    CConsole::getConsoleInstance("PGESysNET").EOLn("%s: SERVER blacklisted pge message received: %u from connection %u!",
                        __func__, pkt.pktId, pkt.m_connHandleServerSide);
                    assert(false);
                    continue;
                }
            }

            m_queuePackets.push_back(pkt);
        } // for i

        if (m_nRxPktCount == 0)
        {
            m_time1stRxPkt = std::chrono::steady_clock::now();
        }
        m_nRxPktCount += numMsgs;

        return true;
    }
    else
    {   // client
        if (m_hConnection == k_HSteamNetConnection_Invalid)
        {
            return false;
        }

        // ReceiveMessagesOnConnection() basically copies the pointers to messages from GNS's internal linked list,
        // and unlinks these from the linked list
        const int numMsgs = m_pInterface->ReceiveMessagesOnConnection(m_hConnection, pIncomingMsg, nIncomingMsgArraySize);
        
        // ### from here same as server code above
        if (numMsgs == 0)
        {
            return false;
        }

        if (numMsgs < 0)
        {
            CConsole::getConsoleInstance("PGESysNET").EOLn("%s: CLIENT Error checking for messages!", __func__);
            return false;
        }

        if (!pIncomingMsg)
        {
            CConsole::getConsoleInstance("PGESysNET").EOLn("%s: CLIENT pIncomingMsg null!", __func__);
            return false;
        }
        // ### until here

        for (int i = 0; i < numMsgs; i++)
        {
            pge_network::PgePacket pkt;
            assert((pIncomingMsg[i])->m_cbSize == sizeof(pkt));
            memcpy(&pkt, (pIncomingMsg[i])->m_pData, (pIncomingMsg[i])->m_cbSize);
            // here we are client, we don't set pkt.connHandle because we expect it to be already properly filled in by sender (server)!

            // We don't need this anymore.
            // Note that we could even push pIncomingMsg to a queue, and process it later, and
            // release it later, that could be a good speed optimization.
            (pIncomingMsg[i])->Release();
            
            if (pkt.pktId == pge_network::PgePktId::APP)
            {
                if (m_blackListedAppMessages.end() != m_blackListedAppMessages.find(pkt.msg.app.msgId))
                {
                    CConsole::getConsoleInstance("PGESysNET").EOLn("%s: CLIENT blacklisted app message received from server: %u with m_connHandleServerSide %u!",
                        __func__, pkt.msg.app.msgId, pkt.m_connHandleServerSide);
                    assert(false);
                    continue;
                }
            }
            else
            {
                if (m_blackListedPgeMessages.end() != m_blackListedPgeMessages.find(pkt.pktId))
                {
                    CConsole::getConsoleInstance("PGESysNET").EOLn("%s: CLIENT blacklisted pge message received: %u from server with m_connHandleServerSide %u!",
                        __func__, pkt.pktId, pkt.m_connHandleServerSide);
                    assert(false);
                    continue;
                }
            }

            m_queuePackets.push_back(pkt);
        } // for i

        if (m_nRxPktCount == 0)
        {
            m_time1stRxPkt = std::chrono::steady_clock::now();
        }
        m_nRxPktCount += numMsgs;

        return true;
    }
}

void PGESysNET::PollConnectionStateChanges()
{
    s_pCallbackInstance = this;
    m_pInterface->RunCallbacks();
}

// ### from here server only
void PGESysNET::SendPacketToClient(HSteamNetConnection conn, const pge_network::PgePacket& pkt)
{
    if (conn == k_HSteamNetConnection_Invalid)
    {
        // silent ignore, in the future maybe we will simply inject a message to ourselves' message queue
        // that is how server can handle itself as a client similar to any other client
        return;
    }
    m_pInterface->SendMessageToConnection(conn, &pkt, (uint32)sizeof(pkt), k_nSteamNetworkingSend_Reliable, nullptr);
    m_nTxPktCount++;
    if (m_nTxPktCount == 1)
    {
        m_time1stTxPkt = std::chrono::steady_clock::now();
    }
}

void PGESysNET::SendPacketToAllClients(const pge_network::PgePacket& pkt, HSteamNetConnection except)
{
    for (auto& client : m_mapClients)
    {
        if (client.first == k_HSteamNetConnection_Invalid)
        {
            // silent ignore, in the future maybe we will simply inject a message to ourselves' message queue
            // that is how server can handle itself as a client similar to any other client
            continue;
        }
        if (client.first != except)
        {
            SendPacketToClient(client.first, pkt);
        }
    }
}
// ### server only until here


// ### from here client only
void PGESysNET::SendToServer(const pge_network::PgePacket& pkt)
{
    if (m_hConnection == k_HSteamNetConnection_Invalid)
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("%s(): Client Invalid connection handle to server!", __func__);
        return;
    }

    m_pInterface->SendMessageToConnection(m_hConnection, &pkt, (uint32)sizeof(pkt), k_nSteamNetworkingSend_Reliable, nullptr);
    m_nTxPktCount++;
    if (m_nTxPktCount == 1)
    {
        m_time1stTxPkt = std::chrono::steady_clock::now();
    }
}

void PGESysNET::InjectPacket(const pge_network::PgePacket& pkt)
{
    m_queuePackets.push_back(pkt);
    m_nInjectPktCount++;
    if (m_nInjectPktCount == 1)
    {
        m_time1stInjectPkt = std::chrono::steady_clock::now();
    }
}

std::size_t PGESysNET::getPacketQueueSize() const
{
    return m_queuePackets.size();
}

pge_network::PgePacket PGESysNET::popFrontPacket() noexcept(false)
{
    pge_network::PgePacket pkt = m_queuePackets.front();
    m_queuePackets.pop_front();
    return pkt;
}

std::set<pge_network::PgePktId>& PGESysNET::getBlackListedPgeMessages()
{
    return m_blackListedPgeMessages;
}

std::set<pge_network::TPgeMsgAppMsgId>& PGESysNET::getBlackListedAppMessages()
{
    return m_blackListedAppMessages;
}

const SteamNetConnectionRealTimeStatus_t& PGESysNET::getRealTimeStatus(bool bForceUpdate)
{
    if (isServer())
    {
        // I cannot retrieve such info for server
        return m_connRtStatus;
    }

    if (m_hConnection == k_HSteamNetConnection_Invalid)
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("%s invalid connection handle!", __func__);
        assert(false);
        return m_connRtStatus;
    }

    if (bForceUpdate)
    {
        m_pInterface->GetConnectionRealTimeStatus(m_hConnection, &m_connRtStatus, 0, NULL);
    }

    return m_connRtStatus;
}

std::string PGESysNET::getDetailedStatus() const
{
    if (isServer())
    {
        // I cannot retrieve such info for server
        return "";
    }

    if (m_hConnection == k_HSteamNetConnection_Invalid)
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("%s invalid connection handle!", __func__);
        assert(false);
        return "";
    }

    char szDetailedStatus[4096];
    const int nRes = m_pInterface->GetDetailedConnectionStatus(m_hConnection, szDetailedStatus, sizeof(szDetailedStatus));
    if (nRes == 0)
    {
        // only in this case szDetailedStatus is null-terminated and perfect!
        return std::string(szDetailedStatus);
    }
    else
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("%s GetDetailedConnectionStatus() returned %d!", __func__, nRes);
        return "";
    }
}

bool PGESysNET::connectToServer(const std::string& sServerAddress)
{
    m_addrServer.Clear();
    if (!m_addrServer.ParseString(sServerAddress.c_str()))
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("%s() Failed to parse address: %s!", __func__, sServerAddress.c_str());
        return false;
    }

    if (m_addrServer.m_port == 0)
    {
        m_addrServer.m_port = DEFAULT_SERVER_PORT;
        CConsole::getConsoleInstance("PGESysNET").OLn("%s() Using default port: %d", __func__, m_addrServer.m_port);
    }
    else
    {
        CConsole::getConsoleInstance("PGESysNET").OLn("%s() Using custom port: %d", __func__, m_addrServer.m_port);
    }

    // Parse back to string just to make sure we are logging and connecting to proper address
    m_addrServer.ToString(m_szAddr, sizeof(m_szAddr), true);
    CConsole::getConsoleInstance("PGESysNET").OLn("%s Connecting to server at %s", __func__, m_szAddr);

    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);

    m_hConnection = m_pInterface->ConnectByIPAddress(m_addrServer, 1, &opt);
    if (m_hConnection == k_HSteamNetConnection_Invalid)
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("Client Failed to create connection");
        return false;
    }

    //SteamNetConnectionInfo_t connInfo;
    //m_pInterface->GetConnectionInfo(m_hConnection, &connInfo);

    return true;
}

bool PGESysNET::DisconnectClient()
{
    if (m_hConnection != k_HSteamNetConnection_Invalid)
    {
        m_pInterface->CloseConnection(m_hConnection, k_ESteamNetConnectionEnd_App_Generic, "PGESysNET Client Graceful shutdown", true);
        m_hConnection = k_HSteamNetConnection_Invalid;
    }
    m_hConnectionServerSide = k_HSteamNetConnection_Invalid;
    m_addrServer.Clear();
    memset(m_szAddr, sizeof(m_szAddr), 0);
    memset(&m_connRtStatus, 0, sizeof(m_connRtStatus));

    return true;
}
// ### client only until here


// ### from here server only
bool PGESysNET::startListening()
{
    SteamNetworkingIPAddr serverLocalAddr;
    serverLocalAddr.Clear();
    serverLocalAddr.m_port = m_nPort;
    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);

    // GameNetworkingSockets - 1.4.0\tests\test_connection.cpp :: Test_Connection() contains example how to initiate connection to ourselves!
    m_hListenSock = m_pInterface->CreateListenSocketIP(serverLocalAddr, 1, &opt);
    if (m_hListenSock == k_HSteamListenSocket_Invalid)
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("%s() Failed to listen on port %d!", __func__, m_nPort);
        return false;
    }

    m_hPollGroup = m_pInterface->CreatePollGroup();
    if (m_hPollGroup == k_HSteamNetPollGroup_Invalid)
    {
        CConsole::getConsoleInstance("PGESysNET").EOLn("%s() Failed to create poll group!", __func__);
        destroySysNET();
        return false;
    }
    CConsole::getConsoleInstance("PGESysNET").OLn("%s() Server listening on port %d", __func__, m_nPort);

    // Add ourselves to the client list, using std::map wacky syntax
    // k_HSteamNetConnection_Invalid will mean the server itself
    m_mapClients[k_HSteamNetConnection_Invalid];
    memset(m_mapClients[k_HSteamNetConnection_Invalid].m_szAddr, sizeof(m_mapClients[k_HSteamNetConnection_Invalid].m_szAddr), 0);

    // here we create a client connect pkt that will be injected to our queue so app level will process it and create
    // player object or whatever they want for the server itself, as it was a real client
    pge_network::PgePacket pkt;
    pge_network::initPktPgeMsgUserConnected(
        pkt,
        static_cast<pge_network::PgeNetworkConnectionHandle>(k_HSteamNetConnection_Invalid),
        true,
        "" /* we dont know our IP address, later the 1st connecting client will tell us anyway */);

    // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
    m_queuePackets.push_back(pkt);

    // TODOOO: network layer needs to set user name! SetClientNick(k_HSteamNetConnection_Invalid, pkt.msg.userConnected.sUserName);

    return true;
}

bool PGESysNET::stopListening()
{
    // Close all the connections
    CConsole::getConsoleInstance("PGESysNET").OLn("Server closing connections...");
    for (const auto& it : m_mapClients)
    {
        // Send them one more goodbye message.  Note that we also have the
        // connection close reason as a place to send final data.  However,
        // that's usually best left for more diagnostic/debug text not actual
        // protocol strings.
        //SendStringToClient(it.first, "Server is shutting down.  Goodbye.");

        // Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
        // to flush this out and close gracefully.
        m_pInterface->CloseConnection(it.first, k_ESteamNetConnectionEnd_App_Generic, "PGESysNET Server Graceful shutdown", true);
    }
    m_mapClients.clear();

    if (m_hListenSock != k_HSteamListenSocket_Invalid)
    {
        m_pInterface->CloseListenSocket(m_hListenSock);
        m_hListenSock = k_HSteamListenSocket_Invalid;
    }

    if (m_hPollGroup != k_HSteamNetPollGroup_Invalid)
    {
        m_pInterface->DestroyPollGroup(m_hPollGroup);
        m_hPollGroup = k_HSteamNetPollGroup_Invalid;
    }

    return true;
}

uint32_t PGESysNET::getRxPacketCount() const
{
    return m_nRxPktCount;
}

uint32_t PGESysNET::getTxPacketCount() const
{
    return m_nTxPktCount;
}

uint32_t PGESysNET::getInjectPacketCount() const
{
    return m_nInjectPktCount;
}

void PGESysNET::WriteServerClientList()
{
    CConsole::getConsoleInstance("PGESysNET").OLnOI("Listing Clients:");
    for (const auto& client : m_mapClients)
    {
        if (client.first == k_HSteamNetConnection_Invalid)
        {
            CConsole::getConsoleInstance("PGESysNET").OLn("connHandle: %u (this is me); Name: %s; Address: %s",
                client.first, client.second.m_sCustomName.c_str(), client.second.m_szAddr);
        }
        else
        {
            CConsole::getConsoleInstance("PGESysNET").OLn("connHandle: %u; Name: %s; Address: %s",
                client.first, client.second.m_sCustomName.c_str(), client.second.m_szAddr);
        }
    }
    CConsole::getConsoleInstance("PGESysNET").OO();
}
// ### server only until here


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PGESysNET* PGESysNET::s_pCallbackInstance = nullptr;
bool PGESysNET::s_bServer = false;

PGESysNET::PGESysNET(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    m_nPort(DEFAULT_SERVER_PORT),
    m_hConnection(k_HSteamNetConnection_Invalid),
    m_hConnectionServerSide(k_HSteamNetConnection_Invalid),
    m_pInterface(nullptr),
    m_hListenSock(k_HSteamListenSocket_Invalid),
    m_hPollGroup(k_HSteamNetPollGroup_Invalid),
    m_nRxPktCount(0),
    m_nTxPktCount(0),
    m_nInjectPktCount(0)
{
    m_addrServer.Clear();
    memset(&m_connRtStatus, 0, sizeof(m_connRtStatus));
} // PGESysNET()

PGESysNET::PGESysNET(const PGESysNET& other) :
    m_cfgProfiles(other.m_cfgProfiles)
{

}

PGESysNET& PGESysNET::operator=(const PGESysNET&)
{
    return *this;
}

void PGESysNET::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    // This function is also invoked on main thread when I call PGESysNET.PollConnectionStateChanges() from PGE::runGame()
    // so no need to utilize mutexes around here.
    // And the other function PollIncomingMessages() is also invoked by PGE::runGame().
    // So it is safe to do operations on m_queuePackets.
    // The problem related to CConsole not being threadsafe was not due to the logs within this function
    // but to the callback function I've set with SteamNetworkingUtils()->SetDebugOutputFunction(), as it can be called
    // in parallel by SteamNetworking thread.

    if (isServer())
    {
        char szTemp[1024];

        // What's the state of the connection?
        switch (pInfo->m_info.m_eState)
        {
        case k_ESteamNetworkingConnectionState_None:
            // NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
            break;

        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            // Ignore if they were not previously connected.  (If they disconnected
            // before we accepted the connection.)
            if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
            {
                // Locate the client.  Note that it should have been found, because this
                // is the only codepath where we remove clients (except on shutdown),
                // and connection change callbacks are dispatched in queue order.
                auto itClient = m_mapClients.find(pInfo->m_hConn);
                assert(itClient != m_mapClients.end());

                // Select appropriate log messages
                const char* pszDebugLogAction;
                if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
                {
                    pszDebugLogAction = "problem detected locally";
                    sprintf(szTemp, "%s disappeared (%s)", itClient->second.m_sCustomName.c_str(), pInfo->m_info.m_szEndDebug);
                }
                else
                {
                    // Note that here we could check the reason code to see if
                    // it was a "usual" connection or an "unusual" one.
                    pszDebugLogAction = "closed by peer";
                    sprintf(szTemp, "%s closed connection", itClient->second.m_sCustomName.c_str());
                }

                // Spew something to our own log.  Note that because we put their nick
                // as the connection description, it will show up, along with their
                // transport-specific data (e.g. their IP address)
                CConsole::getConsoleInstance("PGESysNET").OLn("%s: SERVER Connection %s %s, reason %d: %s\n",
                    __func__,
                    pInfo->m_info.m_szConnectionDescription,
                    pszDebugLogAction,
                    pInfo->m_info.m_eEndReason,
                    pInfo->m_info.m_szEndDebug
                );

                pge_network::PgePacket pkt;
                memset(&pkt, 0, sizeof(pkt));
                pkt.pktId = pge_network::MsgUserDisconnected::id;
                pkt.m_connHandleServerSide = static_cast<pge_network::PgeNetworkConnectionHandle>(pInfo->m_hConn);

                m_mapClients.erase(itClient);  // dont try to send anything to the disconnected client :)

                // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
                m_queuePackets.push_back(pkt);
                SendPacketToAllClients(pkt);  
            }
            else
            {
                assert(pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
            }

            // Clean up the connection.  This is important!
            // The connection is "closed" in the network sense, but
            // it has not been destroyed.  We must close it on our end, too
            // to finish up.  The reason information do not matter in this case,
            // and we cannot linger because it's already closed on the other end,
            // so we just pass 0's.
            m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
            break;
        }

        case k_ESteamNetworkingConnectionState_Connecting:
        {
            // This must be a new connection
            assert(m_mapClients.find(pInfo->m_hConn) == m_mapClients.end());

            CConsole::getConsoleInstance("PGESysNET").OLn("%s: SERVER Connection request from %s", __func__, pInfo->m_info.m_szConnectionDescription);

            // A client is attempting to connect
            // Try to accept the connection.
            if (m_pInterface->AcceptConnection(pInfo->m_hConn) != k_EResultOK)
            {
                // This could fail.  If the remote host tried to connect, but then
                // disconnected, the connection may already be half closed.  Just
                // destroy whatever we have on our side.
                m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
                CConsole::getConsoleInstance("PGESysNET").EOLn("%s: SERVER Can't accept connection. (It was already closed?)", __func__);
                break;
            }

            // Assign the poll group
            if (!m_pInterface->SetConnectionPollGroup(pInfo->m_hConn, m_hPollGroup))
            {
                m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
                CConsole::getConsoleInstance("PGESysNET").EOLn("%s: SERVER Failed to set poll group!", __func__);
                break;
            }

            // Add them to the client list, using std::map wacky syntax
            m_mapClients[pInfo->m_hConn];
            pInfo->m_info.m_addrRemote.ToString(m_mapClients[pInfo->m_hConn].m_szAddr, sizeof(m_mapClients[pInfo->m_hConn].m_szAddr), true);
            CConsole::getConsoleInstance("PGESysNET").OLn("%s: SERVER A client is connecting from %s ...", __func__, m_mapClients[pInfo->m_hConn].m_szAddr);

            pge_network::PgePacket pkt;
            pge_network::initPktPgeMsgUserConnected(
                pkt,
                static_cast<pge_network::PgeNetworkConnectionHandle>(pInfo->m_hConn),
                false,
                m_mapClients[pInfo->m_hConn].m_szAddr);

            // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
            m_queuePackets.push_back(pkt);

            // TODOOO: network layer needs to set user name! SetClientNick(pInfo->m_hConn, pkt.msg.userConnected.sUserName);
           
            break;
        }

        case k_ESteamNetworkingConnectionState_Connected:
        {
            // We will get a callback immediately after accepting the connection.
            // Since we are the server, we can ignore this, it's not news to us.

            const auto itClient = m_mapClients.find(pInfo->m_hConn);
            if (itClient == m_mapClients.end())
            {
                CConsole::getConsoleInstance("PGESysNET").EOLn("%s: SERVER Cannot happen: a client (%u) has reached state Connected but not present in clients map!",
                    __func__, pInfo->m_hConn);
                assert(false);
            }
            else
            {
                CConsole::getConsoleInstance("PGESysNET").OLn("%s: SERVER Client with connHandle %u has reached state Connected!", __func__, pInfo->m_hConn);
            }
            break;
        }

        default:
            // Silences -Wswitch
            break;
        }
    } // end server
    else
    {
        assert(pInfo->m_hConn == m_hConnection || m_hConnection == k_HSteamNetConnection_Invalid);

        // What's the state of the connection?
        switch (pInfo->m_info.m_eState)
        {
        case k_ESteamNetworkingConnectionState_None:
            // NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
            break;

        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            // Print an appropriate message
            if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
            {
                // Note: we could distinguish between a timeout, a rejected connection,
                // or some other transport problem.
                CConsole::getConsoleInstance("PGESysNET").EOLn("%s: CLIENT problem 1 (rejected I guess) (%s)",
                    __func__, pInfo->m_info.m_szEndDebug);
            }
            else if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
            {
                CConsole::getConsoleInstance("PGESysNET").EOLn("%s: CLIENT problem 2 (guess our machine has lost network connection) (%s)",
                    __func__, pInfo->m_info.m_szEndDebug);
            }
            else
            {
                // NOTE: We could check the reason code for a normal disconnection
                CConsole::getConsoleInstance("PGESysNET").EOLn("%s: CLIENT problem 3 (closed by peer) (%s)",
                    __func__, pInfo->m_info.m_szEndDebug);
            }

            // Clean up the connection.  This is important!
            // The connection is "closed" in the network sense, but
            // it has not been destroyed.  We must close it on our end, too
            // to finish up.  The reason information do not matter in this case,
            // and we cannot linger because it's already closed on the other end,
            // so we just pass 0's.
            m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
            m_hConnection = k_HSteamNetConnection_Invalid;
            break;
        }

        case k_ESteamNetworkingConnectionState_Connecting:
            // We will get this callback when we start connecting.
            // We can ignore this.
            CConsole::getConsoleInstance("PGESysNET").OLn("%s: CLIENT Connecting ...", __func__);
            break;

        case k_ESteamNetworkingConnectionState_Connected:
        {
            CConsole::getConsoleInstance("PGESysNET").OLn("%s: CLIENT Connected to server OK", __func__);
            
            // KEKEKEKEKE
            // you jumped here from PgePacket.h MsgUserConnected.
            char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
            pInfo->m_info.m_addrRemote.ToString(szAddr, sizeof(szAddr), true);
            CConsole::getConsoleInstance("PGESysNET").OLn("%s: CLIENT Remote address: %s", __func__, szAddr);

            CConsole::getConsoleInstance("PGESysNET").OLn("%s: CLIENT conn. description: %s", __func__, pInfo->m_info.m_szConnectionDescription);

            break;
        }
        default:
            // Silences -Wswitch
            break;
        }
    }
}

void PGESysNET::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    s_pCallbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
}

void PGESysNET::SetClientNick(HSteamNetConnection hConn, const char* nick)
{
    // Remember their nick
    m_mapClients[hConn].m_sCustomName = nick;

    // Set the connection name, too, which is useful for debugging
    m_pInterface->SetConnectionName(hConn, nick);
}