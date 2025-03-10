/*
    ###################################################################################
    PgeGnsServer.cpp
    This file is part of PGE.
    Server Wrapper for GameNetworkingSockets library
    PR00F's Game Engine networking subsystem
    Made by PR00F88
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
    if (!isInitialized())
    {
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() Already destroyed.", __func__);
        return true;
    }

    return stopListening() && PgeGnsWrapper::destroy();
} // destroy()

bool PgeGnsServer::startListening(const std::string& sAppVersion)
{
    if (isListening())
    {
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() ERROR: Server is already listening on port %d", __func__, m_nPort);
        return false;
    }

    if (!isInitialized())
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s() ERROR: not initialized!", __func__);
        return true;
    }

    m_sAppVersion = sAppVersion;
    if (m_sAppVersion.empty())
    {
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() no server app version is specified!", __func__);
    }
    else
    {
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() server app version is specified as: %s!", __func__, m_sAppVersion.c_str());
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
    memset(&m_mapClients[k_HSteamNetConnection_Invalid].m_connRtStatus, sizeof(m_mapClients[k_HSteamNetConnection_Invalid].m_connRtStatus), 0);

    // here we create a client connect pkt that will be injected to our queue so app level will process it and create
    // player object or whatever they want for the server itself, as it was a real client
    pge_network::PgePacket pktUserConnected;
    pge_network::PgePacket::initPktPgeMsgUserConnected(
        pktUserConnected,
        static_cast<pge_network::PgeNetworkConnectionHandle>(k_HSteamNetConnection_Invalid),
        true /* bCurrentClient */,
        "" /* we dont know our own IP address, later the 1st connecting client will tell us anyway */);

    // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
    m_queuePackets.push_back(pktUserConnected);

    // TODOOO: network layer needs to set user name! SetClientNick(k_HSteamNetConnection_Invalid, PgePacket::getMessageAsUserConnected(pkt).sUserName);

    return true;
}

bool PgeGnsServer::stopListening(const std::string& sExtraDebugText)
{
    if (!isListening())
    {
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() Server was not listening, no need to stop.", __func__);
        return true;
    }

    // Close all the connections
    CConsole::getConsoleInstance("PgeGnsServer").OLn(
        "Server closing connections for %u client(s) (including itself) ... Reason: %s",
        m_mapClients.size(),
        sExtraDebugText.empty() ? "unspecified" : sExtraDebugText.c_str());

    // I always forget but m_mapClients also contains the server itself with invalid handle, this should be renamed so I never forget again!
    for (const auto& itClient : m_mapClients)
    {
        const HSteamNetConnection& hClientConnection = itClient.first;
        logDetailedConnectionStatus(hClientConnection);

        // inform all clients about disconnecting this client;
        // basically this way all clients will know about all clients got disconnected.
        pge_network::PgePacket pkt;
        pge_network::PgePacket::initPktPgeMsgUserDisconnected(
            pkt,
            static_cast<pge_network::PgeNetworkConnectionHandle>(hClientConnection));
        sendToAllClientsExcept(pkt, hClientConnection);

        if (hClientConnection == k_HSteamNetConnection_Invalid) {
            // this is us, we don't have GNS connection to ourselves, so continue with real clients
            continue;
        }

        const std::string sExtraDebugTextToSend =
            sExtraDebugText.empty() ?
            "PgeGnsServer Graceful stopListening()" :
            "PgeGnsServer Graceful stopListening(), reason: " + sExtraDebugText;

        // Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
        // to flush this out (pending reliable message are actually sent) and close gracefully.
        m_pInterface->CloseConnection(hClientConnection, k_ESteamNetConnectionEnd_App_Generic, sExtraDebugTextToSend.c_str(), true);
    }

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

    // here we create a client disconnect pkt that will be injected to our queue so app level can process it
    // for the server itself, as it was a real client disconnecting
    pge_network::PgePacket pkt;
    pge_network::PgePacket::initPktPgeMsgUserDisconnected(
        pkt,
        static_cast<pge_network::PgeNetworkConnectionHandle>(k_HSteamNetConnection_Invalid));
    
    // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
    m_queuePackets.push_back(pkt);

    return true;
}

bool PgeGnsServer::isListening() const
{
    return m_hListenSock != k_HSteamListenSocket_Invalid;
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
            //if (pge_network::MsgApp::getMsgAppMsgId(*pMsgApp) == 1u )
            //{
            //    CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() Sending out UserSetupFromServer to %u", __func__, conn);
            //}
        }
    }
    m_nTxByteCount += nActualPktSize;
}

void PgeGnsServer::sendToAllClientsExcept(const pge_network::PgePacket& pkt, const HSteamNetConnection& except)
{
    static_assert(k_HSteamNetConnection_Invalid == 0U, "on upper layers we use connHandle 0 to identify server, so here k_HSteamNetConnection_Invalid must be 0");
    //CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() start to %u clients ...", __func__, m_mapClients.size());
    for (auto& client : m_mapClients)
    {
        //CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() client: %u ...", __func__, client.first);
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
    //CConsole::getConsoleInstance("PgeGnsServer").OLn("%s() end", __func__);
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

void PgeGnsServer::setClientDebugName(HSteamNetConnection connHandle, const char* nick)
{
    const auto pClient = isClientConnectionHandleValid(connHandle);
    if (!pClient)
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: connHandle %u not valid!", __func__, connHandle);
        return;
    }

    pClient->m_sCustomName = nick;

    // Set the connection name too, which is useful for debugging
    m_pInterface->SetConnectionName(connHandle, nick);
}

const SteamNetConnectionRealTimeStatus_t& PgeGnsServer::getRealTimeStatus(const HSteamNetConnection& connHandle, bool bForceUpdate)
{
    const auto pClient = isClientConnectionHandleValid(connHandle);
    if (!pClient)
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: connHandle %u not valid!", __func__, connHandle);
        assert(!m_mapClients.empty());                         // never empty, first elem is always server (we)
        return m_mapClients.begin()->second.m_connRtStatus;    // return with server's full zero data
    }

    if (bForceUpdate)
    {
        m_pInterface->GetConnectionRealTimeStatus(connHandle, &pClient->m_connRtStatus, 0, NULL);
    }

    return pClient->m_connRtStatus;
}

std::string PgeGnsServer::getDetailedConnectionStatus(const HSteamNetConnection& connHandle) const
{
    const auto pClient = isClientConnectionHandleValid(connHandle);
    if (!pClient)
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: connHandle %u not valid!", __func__, connHandle);
        return "";
    }

    return PgeGnsWrapper::getDetailedConnectionStatus(connHandle);
}


// ############################## PROTECTED ##############################


bool PgeGnsServer::pgeMessageIsHandledAtGnsLevel(const pge_network::PgePacket& pktReceivedFromClient)
{
    if (pge_network::PgePacket::getPacketId(pktReceivedFromClient) == pge_network::MsgClientAppVersionFromClient::id)
    {
        // this should be the very 1st PgePacket we receive from a just connected client
        const auto nClientConnHandle = pge_network::PgePacket::getServerSideConnectionHandle(pktReceivedFromClient);
        const auto itClient = m_mapClients.find(nClientConnHandle);
        if (itClient == m_mapClients.end())
        {
            CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: SERVER Cannot happen: a client (%u) sent MsgClientAppVersionFromClient but not present in clients map!",
                __func__, nClientConnHandle);
            assert(false);
        }
        else
        {
            const auto& msgClientAppVersion = pge_network::PgePacket::getMessageAsClientAppVersionFromClient(pktReceivedFromClient);
            if (m_sAppVersion == msgClientAppVersion.m_szAppVersion)
            {
                CConsole::getConsoleInstance("PgeGnsServer").OLn("%s: SERVER client (%u) app version matching (%s), admitting client!",
                    __func__, nClientConnHandle, msgClientAppVersion.m_szAppVersion);

                // here we create a client connect pkt that will be injected to our queue so app level will process it and create
                // player object or whatever they want for the client
                pge_network::PgePacket pktUserConnected;
                pge_network::PgePacket::initPktPgeMsgUserConnected(
                    pktUserConnected,
                    itClient->first,
                    false /* bCurrentClient */,
                    itClient->second.m_szAddr);
            
                // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
                m_queuePackets.push_back(pktUserConnected);
            }
            else
            {
                CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: SERVER client (%u) app version mismatching (%s), disconnecting client!",
                    __func__, nClientConnHandle, msgClientAppVersion.m_szAppVersion);

                m_mapClients.erase(itClient);
                const std::string sDebugText = "Server app version mismatching with client app version: " + m_sAppVersion + " != " + std::string(msgClientAppVersion.m_szAppVersion);
                m_pInterface->CloseConnection(nClientConnHandle, 0, sDebugText.c_str(), false);
            }
        }
        return true;
    }
    return false;
}

int PgeGnsServer::receiveMessages(ISteamNetworkingMessage** pIncomingMsg, int nIncomingMsgArraySize) const
{
    // ReceiveMessagesOnPollGroup() basically copies the pointers to messages from GNS's internal linked list,
    // and unlinks these from that internal linked list, so it is cheap copy to our array
    return m_pInterface->ReceiveMessagesOnPollGroup(m_hPollGroup, pIncomingMsg, nIncomingMsgArraySize);
}

bool PgeGnsServer::validateSteamNetworkingMessage(const HSteamNetConnection& connHandle) const
{
    if (connHandle == k_HSteamNetConnection_Invalid)
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: SERVER received with invalid connhandle!", __func__);
        return false;
    }

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

    const char* pszDebugLogAction;
    switch (pInfo->m_info.m_eState)
    {
    case k_ESteamNetworkingConnectionState_None:
        // sometimes we fall here immediately with m_eOldState k_ESteamNetworkingConnectionState_Connected when closing down connection
        // fall-through
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
        // fall-through
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    {
        if ((pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected) || (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting))
        {
            switch (pInfo->m_info.m_eState)
            {
            case k_ESteamNetworkingConnectionState_None:
                pszDebugLogAction = "disappeared immediately";
                break;
            case k_ESteamNetworkingConnectionState_ClosedByPeer:
                // Note that here we could check the reason code to see if it was a "usual" or an "unusual" one.
                pszDebugLogAction = "disconnected";
                break;
            default: // k_ESteamNetworkingConnectionState_ProblemDetectedLocally
                pszDebugLogAction = "disappeared, problem detected locally";
            } // end switch m_eState
            const auto itClient = m_mapClients.find(pInfo->m_hConn);
            if (itClient == m_mapClients.end())
            {
                sprintf(szTemp, "Not stored (maybe already deleted) client %s, reason %d: %s", pszDebugLogAction, pInfo->m_info.m_eEndReason, pInfo->m_info.m_szEndDebug);
            }
            else
            {
                sprintf(szTemp, "%s %s, reason %d: %s", itClient->second.m_sCustomName.c_str(), pszDebugLogAction, pInfo->m_info.m_eEndReason, pInfo->m_info.m_szEndDebug);
                m_mapClients.erase(itClient);  // dont try to send anything to the disconnected client :)

                // App level should be notified only if we found it in m_mapClients, otherwise probably this client was never known by app level
                pge_network::PgePacket pkt;
                pge_network::PgePacket::initPktPgeMsgUserDisconnected(pkt, pInfo->m_hConn);
                // we push this packet to our pkt queue, this is how we "send" message to ourselves so server game loop can process it
                m_queuePackets.push_back(pkt);
                sendToAllClientsExcept(pkt);
            }
            CConsole::getConsoleInstance("PgeGnsServer").OLn("%s: SERVER Connection %s (handle %u) %s",
                __func__,
                pInfo->m_info.m_szConnectionDescription,
                pInfo->m_hConn,
                szTemp
            );
            logDetailedConnectionStatus(pInfo->m_hConn);

            // Clean up the connection.  This is important!
            // The connection is "closed" in the network sense, but
            // it has not been destroyed.  We must close it on our end, too
            // to finish up.  The reason information do not matter in this case,
            // and we cannot linger because it's already closed on the other end.
            m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
        }
        else
        {
            CConsole::getConsoleInstance("PgeGnsServer").OLn("%s: SERVER NO ACTION TAKEN: Connection %s (handle %u)",
                __func__,
                pInfo->m_info.m_szConnectionDescription,
                pInfo->m_hConn
            );
        }
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
            m_pInterface->CloseConnection(pInfo->m_hConn, 0, "Server failed SetConnectionPollGroup()", false);
            CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: SERVER Failed to set poll group!", __func__);
            break;
        }

        // Add them to the client list, using std::map wacky syntax
        m_mapClients[pInfo->m_hConn];
        pInfo->m_info.m_addrRemote.ToString(m_mapClients[pInfo->m_hConn].m_szAddr, sizeof(m_mapClients[pInfo->m_hConn].m_szAddr), true);
        CConsole::getConsoleInstance("PgeGnsServer").OLn("%s: SERVER A client is connecting from %s ...", __func__, m_mapClients[pInfo->m_hConn].m_szAddr);

        // since v0.2.4, we do NOT inject MsgUserConnected, instead we expect client now to send us a MsgClientAppVersionFromClient that we handle in
        // pgeMessageIsHandledAtGnsLevel(), and based on version match, we inject MsgUserConnected there!
        
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
            CConsole::getConsoleInstance("PgeGnsServer").OLn(
                "%s: SERVER Client with connHandle %u has reached state Connected, now waiting for MsgClientAppVersionFromClient!",
                __func__, pInfo->m_hConn);
        }
        break;
    }

    default:
        // Silences -Wswitch
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: SERVER Connection %s (handle %u) %s became UNKNOWN, reason %d: %s, prev state: %u\n",
            __func__,
            pInfo->m_info.m_szConnectionDescription,
            pInfo->m_hConn,
            "",
            pInfo->m_info.m_eEndReason,
            pInfo->m_info.m_szEndDebug,
            pInfo->m_eOldState
        );

        break;
    }
}  // onSteamNetConnectionStatusChanged()


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

const PgeGnsServer::TClient* PgeGnsServer::isClientConnectionHandleValid(const HSteamNetConnection& connHandle) const
{
    static_assert(k_HSteamNetConnection_Invalid == 0U, "on upper layers we use connHandle 0 to identify server, so here k_HSteamNetConnection_Invalid must be 0");

    if (connHandle == k_HSteamNetConnection_Invalid)
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: Invalid (server) connHandle specified!", __func__);
        return nullptr;
    }

    if (!isInitialized())
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s not listening!", __func__);
        return nullptr;
    }

    const auto itClient = m_mapClients.find(connHandle);
    if (itClient == m_mapClients.end())
    {
        CConsole::getConsoleInstance("PgeGnsServer").EOLn("%s: connHandle %u not found!", __func__, connHandle);
        return nullptr;
    }

    return &itClient->second;
}

PgeGnsServer::TClient* PgeGnsServer::isClientConnectionHandleValid(const HSteamNetConnection& connHandle)
{
    // simply invoke the const-version of isClientConnectionHandleValid() above by const-casting:
    return const_cast<TClient*>((const_cast<const PgeGnsServer* const>(this))->isClientConnectionHandleValid(connHandle));
}
