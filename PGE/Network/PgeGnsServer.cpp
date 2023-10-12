/*
    ###################################################################################
    PgeGnsServer.cpp
    This file is part of PGE.
    Server Wrapper for GameNetworkingSockets library
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include <cassert>

#include "PgeGnsServer.h"
#include "../PGEincludes.h"
#include "../PGEpragmas.h"


/*
   PgeGnsServer
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PgeGnsServer& PgeGnsServer::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PgeGnsServer inst(cfgProfiles);
    return inst;
} // createAndGet()

PgeGnsServer::~PgeGnsServer()
{
    destroy();
} // ~PgeGnsServer()

bool PgeGnsServer::destroy()
{
    return stopListening() && PgeGnsWrapper::destroy();
} // destroy()

bool PgeGnsServer::isInitialized() const
{
    return m_hListenSock != k_HSteamListenSocket_Invalid;
} // isInitialized()

bool PgeGnsServer::startListening()
{
    if (isInitialized())
    {
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() ERROR: Server is already listening on port %d", __func__, m_nPort);
        return false;
    }

    SteamNetworkingIPAddr serverLocalAddr;
    serverLocalAddr.Clear();
    serverLocalAddr.m_port = m_nPort;
    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)steamNetConnectionStatusChangedCallback);

    // GameNetworkingSockets - 1.4.0\tests\test_connection.cpp :: Test_Connection() contains example how to initiate connection to ourselves!
    m_hListenSock = m_pInterface->CreateListenSocketIP(serverLocalAddr, 1, &opt);
    if (m_hListenSock == k_HSteamListenSocket_Invalid)
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s() Failed to listen on port %d!", __func__, m_nPort);
        return false;
    }

    m_hPollGroup = m_pInterface->CreatePollGroup();
    if (m_hPollGroup == k_HSteamNetPollGroup_Invalid)
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s() Failed to create poll group!", __func__);
        destroy();
        return false;
    }
    CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() Server listening on port %d", __func__, m_nPort);

    // Add ourselves to the client list, using std::map wacky syntax
    // k_HSteamNetConnection_Invalid will mean the server itself
    m_mapClients[k_HSteamNetConnection_Invalid];
    memset(m_mapClients[k_HSteamNetConnection_Invalid].m_szAddr, sizeof(m_mapClients[k_HSteamNetConnection_Invalid].m_szAddr), 0);

    // here we create a client connect pkt that will be injected to our queue so app level will process it and create
    // player object or whatever they want for the server itself, as it was a real client
    pge_network::PgePacket pkt;
    pge_network::PgePacket::initPktPgeMsgUserConnected(
        pkt,
        static_cast<pge_network::PgeNetworkConnectionHandle>(k_HSteamNetConnection_Invalid),
        true,
        "" /* we dont know our own IP address, later the 1st connecting client will tell us anyway */);

    // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
    m_queuePackets.push_back(pkt);

    // TODOOO: network layer needs to set user name! SetClientNick(k_HSteamNetConnection_Invalid, PgePacket::getMessageAsUserConnected(pkt).sUserName);

    return true;
}

bool PgeGnsServer::stopListening()
{
    if (!isInitialized())
    {
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() Server was not listening, no need to stop.", __func__);
        return true;
    }

    // Close all the connections
    CConsole::getConsoleInstance("PgeGnsServer").OLn("Server closing connections for %u clients ...", m_mapClients.size());
    for (const auto& it : m_mapClients)
    {
        const HSteamNetConnection& hClientConnection = it.first;
        if (hClientConnection == 0) {
            // this is us
            continue;
        }

        CConsole::getConsoleInstance("PgeGnsServer").OLn("Detailed Connection Status for client %u: ", hClientConnection);
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s", PgeGnsWrapper::getDetailedConnectionStatus(hClientConnection).c_str());
        CConsole::getConsoleInstance("PgeGnsServer").OLn("");

        // Send them one more goodbye message.  Note that we also have the
        // connection close reason as a place to send final data.  However,
        // that's usually best left for more diagnostic/debug text not actual
        // protocol strings.
        //SendStringToClient(hClientConnection, "Server is shutting down.  Goodbye.");

        // Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
        // to flush this out and close gracefully.
        m_pInterface->CloseConnection(hClientConnection, k_ESteamNetConnectionEnd_App_Generic, "PgeGnsServer Server Graceful shutdown", true);
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

void PgeGnsServer::sendToClient(const HSteamNetConnection& conn, const pge_network::PgePacket& pkt)
{
    static_assert(k_HSteamNetConnection_Invalid == 0U, "on upper layers we use connHandle 0 to identify server, so here k_HSteamNetConnection_Invalid must be 0");
    if (conn == k_HSteamNetConnection_Invalid)
    {
        // silent ignore, in the future maybe we will simply inject a message to ourselves' message queue
        // that is how server can handle itself as a client similar to any other client
        return;
    }

    uint32_t nActualPktSize;
    if (pge_network::PgePacket::getPacketId(pkt) == pge_network::PgePktId::Application)
    {
        // We need the real used memory size so we can truncate the sent pkt to that.
        // We need to consider member padding and the actual message sizes to have a correct value.
        // 'cData' is our point of view since from there we need to calculate.
        const pge_network::MsgApp* pMsgApp = pge_network::PgePacket::getMsgAppFromPkt(pkt);
        assert(pMsgApp);  // never null since it points into pkt
        const pge_network::TByte* pMsgAppInByteSteps = reinterpret_cast<const pge_network::TByte*>(pMsgApp);  // we can step this ptr in Bytes

        const uint8_t nMessageCount = pge_network::PgePacket::getMessageAppCount(pkt);
        assert(nMessageCount == 1); // for now only 1 msg/pkt
        for (uint8_t i = 0; i < nMessageCount; i++)
        {
            pMsgApp = reinterpret_cast<const pge_network::MsgApp*>(pMsgAppInByteSteps);
            // moving pMsgAppInByteSteps by the _actual_ size of the current MsgApp struct (considering the actual app message size there)
            pMsgAppInByteSteps += pge_network::MsgApp::getMsgAppTotalActualSizeBytes(*pMsgApp);
        }
        // now pMsgAppInByteSteps points to RIGHT AFTER the 1st byte of the last application message
        const pge_network::TByte* pPkt = (const pge_network::TByte*)(&pkt);
        nActualPktSize = pMsgAppInByteSteps - pPkt;
    }
    else
    {
        nActualPktSize = static_cast<uint32_t>(sizeof(pkt));
    }

    m_pInterface->SendMessageToConnection(conn, &pkt, nActualPktSize, k_nSteamNetworkingSend_Reliable, nullptr);
    if (m_nTxPktCount == 1)
    {
        m_time1stTxPkt = std::chrono::steady_clock::now();
    }
    m_nTxPktCount++;
    if (pge_network::PgePacket::getPacketId(pkt) == pge_network::PgePktId::Application)
    {
        const pge_network::MsgApp* pMsgApp = pge_network::PgePacket::getMsgAppFromPkt(pkt);
        const uint8_t nMessageCount = pge_network::PgePacket::getMessageAppCount(pkt);
        for (uint8_t i = 0; i < nMessageCount; i++)
        {
            // TODO: nooo, I need to properly iterate to next appmsg as I iterate in above loop!
            ++m_nTxMsgCount[pge_network::MsgApp::getMsgAppMsgId(*pMsgApp)];
        }
    }
    m_nTxByteCount += nActualPktSize;
}

void PgeGnsServer::sendToAllClientsExcept(const pge_network::PgePacket& pkt, const HSteamNetConnection& except)
{
    static_assert(k_HSteamNetConnection_Invalid == 0U, "on upper layers we use connHandle 0 to identify server, so here k_HSteamNetConnection_Invalid must be 0");
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
            sendToClient(client.first, pkt);
        }
    }
}

void PgeGnsServer::inject(const pge_network::PgePacket& pkt)
{
    m_queuePackets.push_back(pkt);
    if (m_nInjectPktCount == 1)
    {
        m_time1stInjectPkt = std::chrono::steady_clock::now();
    }
    m_nInjectPktCount++;
    if (pge_network::PgePacket::getPacketId(pkt) == pge_network::PgePktId::Application)
    {
        const pge_network::MsgApp* pMsgApp = pge_network::PgePacket::getMsgAppFromPkt(pkt);
        const uint8_t nMessageCount = pge_network::PgePacket::getMessageAppCount(pkt);
        assert(nMessageCount == 1); // for now only 1 msg/pkt
        for (uint8_t i = 0; i < nMessageCount; i++)
        {
            // TODO: nooo, I need to properly iterate to next appmsg as I iterate in sendToClient()!
            ++m_nInjectMsgCount[pge_network::MsgApp::getMsgAppMsgId(*pMsgApp)];
        }
    }
    m_nInjectByteCount += sizeof(pkt);
}

void PgeGnsServer::WriteServerClientList()
{
    CConsole::getConsoleInstance("PgeGnsServer").OLnOI("Listing Clients:");
    for (const auto& client : m_mapClients)
    {
        if (client.first == k_HSteamNetConnection_Invalid)
        {
            CConsole::getConsoleInstance("PgeGnsServer").OLn("connHandle: %u (this is me); Name: %s; Address: %s",
                client.first, client.second.m_sCustomName.c_str(), client.second.m_szAddr);
        }
        else
        {
            CConsole::getConsoleInstance("PgeGnsServer").OLn("connHandle: %u; Name: %s; Address: %s",
                client.first, client.second.m_sCustomName.c_str(), client.second.m_szAddr);
        }
    }
    CConsole::getConsoleInstance("PgeGnsServer").OO();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PgeGnsServer::PgeGnsServer(PGEcfgProfiles& cfgProfiles) :
    PgeGnsWrapper(cfgProfiles),
    m_nPort(DEFAULT_SERVER_PORT),
    m_hListenSock(k_HSteamListenSocket_Invalid),
    m_hPollGroup(k_HSteamNetPollGroup_Invalid)
{
} // PgeGnsServer()

PgeGnsServer::PgeGnsServer(const PgeGnsServer& other) :
    PgeGnsWrapper(other.m_cfgProfiles)
{

}

PgeGnsServer& PgeGnsServer::operator=(const PgeGnsServer&)
{
    return *this;
}

int PgeGnsServer::receiveMessages(ISteamNetworkingMessage** pIncomingMsg, int nIncomingMsgArraySize) const
{
    // ReceiveMessagesOnPollGroup() basically copies the pointers to messages from GNS's internal linked list,
    // and unlinks these from that internal linked list, so it is cheap copy to our array
    return m_pInterface->ReceiveMessagesOnPollGroup(m_hPollGroup, pIncomingMsg, nIncomingMsgArraySize);
}

bool PgeGnsServer::validateSteamNetworkingMessage(const HSteamNetConnection& connHandle) const
{
    const auto itClient = m_mapClients.find(connHandle);
    if (itClient == m_mapClients.end())
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: SERVER failed to find connection %u in m_mapClients!",
            __func__, static_cast<unsigned int>(connHandle));
        return false;
    }
    return true;
}

void PgeGnsServer::updateIncomingPgePacket(pge_network::PgePacket& pkt, const HSteamNetConnection& connHandle) const
{
    pge_network::PgePacket::getServerSideConnectionHandle(pkt) = static_cast<pge_network::PgeNetworkConnectionHandle>(connHandle);
}

void PgeGnsServer::onSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    // This function is also invoked on main thread when I call PgeGnsServer.PollConnectionStateChanges() from PGE::runGame()
    // so no need to utilize mutexes around here.
    // And the other function PollIncomingMessages() is also invoked by PGE::runGame().
    // So it is safe to do operations on m_queuePackets.
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
            CConsole::getConsoleInstance("PgeGnsServer").OLn("%s: SERVER Connection %s (handle %u) %s, reason %d: %s\n",
                __func__,
                pInfo->m_info.m_szConnectionDescription,
                pInfo->m_hConn,
                pszDebugLogAction,
                pInfo->m_info.m_eEndReason,
                pInfo->m_info.m_szEndDebug
            );
            CConsole::getConsoleInstance("PgeGnsServer").OLn("Detailed Connection Status for client %u: ", pInfo->m_hConn);
            CConsole::getConsoleInstance("PgeGnsServer").OLn("%s", PgeGnsWrapper::getDetailedConnectionStatus(pInfo->m_hConn).c_str());
            CConsole::getConsoleInstance("PgeGnsServer").OLn("");

            pge_network::PgePacket pkt;
            pge_network::PgePacket::initPktPgeMsgUserDisconnected(pkt, pInfo->m_hConn);

            m_mapClients.erase(itClient);  // dont try to send anything to the disconnected client :)

            // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
            m_queuePackets.push_back(pkt);
            sendToAllClientsExcept(pkt);
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

        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s: SERVER Connection request from %s", __func__, pInfo->m_info.m_szConnectionDescription);

        // A client is attempting to connect
        // Try to accept the connection.
        if (m_pInterface->AcceptConnection(pInfo->m_hConn) != k_EResultOK)
        {
            // This could fail.  If the remote host tried to connect, but then
            // disconnected, the connection may already be half closed.  Just
            // destroy whatever we have on our side.
            m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
            CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: SERVER Can't accept connection. (It was already closed?)", __func__);
            break;
        }

        // Assign the poll group
        if (!m_pInterface->SetConnectionPollGroup(pInfo->m_hConn, m_hPollGroup))
        {
            m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
            CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: SERVER Failed to set poll group!", __func__);
            break;
        }

        // Add them to the client list, using std::map wacky syntax
        m_mapClients[pInfo->m_hConn];
        pInfo->m_info.m_addrRemote.ToString(m_mapClients[pInfo->m_hConn].m_szAddr, sizeof(m_mapClients[pInfo->m_hConn].m_szAddr), true);
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s: SERVER A client is connecting from %s ...", __func__, m_mapClients[pInfo->m_hConn].m_szAddr);

        pge_network::PgePacket pkt;
        pge_network::PgePacket::initPktPgeMsgUserConnected(
            pkt,
            static_cast<pge_network::PgeNetworkConnectionHandle>(pInfo->m_hConn),
            false,
            m_mapClients[pInfo->m_hConn].m_szAddr);

        // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
        m_queuePackets.push_back(pkt);

        // TODOOO: network layer needs to set user name! SetClientNick(pInfo->m_hConn, PgePacket::getMessageAsUserConnected(pkt).sUserName);

        break;
    }

    case k_ESteamNetworkingConnectionState_Connected:
    {
        // We will get a callback immediately after accepting the connection.
        // Since we are the server, we can ignore this, it's not news to us.

        const auto itClient = m_mapClients.find(pInfo->m_hConn);
        if (itClient == m_mapClients.end())
        {
            CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: SERVER Cannot happen: a client (%u) has reached state Connected but not present in clients map!",
                __func__, pInfo->m_hConn);
            assert(false);
        }
        else
        {
            CConsole::getConsoleInstance("PgeGnsServer").OLn("%s: SERVER Client with connHandle %u has reached state Connected!", __func__, pInfo->m_hConn);
        }
        break;
    }

    default:
        // Silences -Wswitch
        break;
    }
}

void PgeGnsServer::SetClientNick(HSteamNetConnection hConn, const char* nick)
{
    // Remember their nick
    m_mapClients[hConn].m_sCustomName = nick;

    // Set the connection name, too, which is useful for debugging
    m_pInterface->SetConnectionName(hConn, nick);
}