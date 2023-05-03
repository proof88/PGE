/*
    ###################################################################################
    PgeGsnServer.cpp
    This file is part of PGE.
    Server Wrapper for GameNetworkingSockets library
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include <cassert>

#include "PgeGsnServer.h"
#include "../PGEincludes.h"
#include "../PGEpragmas.h"


/*
   PgeGsnServer
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PgeGsnServer& PgeGsnServer::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PgeGsnServer inst(cfgProfiles);
    return inst;
} // createAndGet()

PgeGsnServer::~PgeGsnServer()
{
    destroy();
} // ~PgeGsnServer()

bool PgeGsnServer::destroy()
{
    return stopListening() && PgeGsnWrapper::destroy();
} // destroy()

bool PgeGsnServer::isInitialized() const
{
    return m_hListenSock != k_HSteamListenSocket_Invalid;
} // isInitialized()

bool PgeGsnServer::startListening()
{
    if (isInitialized())
    {
        CConsole::getConsoleInstance("PgeGsnServer").OLn("%s() ERROR: Server is already listening on port %d", __func__, m_nPort);
        return false;
    }

    SteamNetworkingIPAddr serverLocalAddr;
    serverLocalAddr.Clear();
    serverLocalAddr.m_port = m_nPort;
    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)SteamNetConnectionStatusChangedCallback);

    // GameNetworkingSockets - 1.4.0\tests\test_connection.cpp :: Test_Connection() contains example how to initiate connection to ourselves!
    m_hListenSock = m_pInterface->CreateListenSocketIP(serverLocalAddr, 1, &opt);
    if (m_hListenSock == k_HSteamListenSocket_Invalid)
    {
        CConsole::getConsoleInstance("PgeGsnServer").EOLn("%s() Failed to listen on port %d!", __func__, m_nPort);
        return false;
    }

    m_hPollGroup = m_pInterface->CreatePollGroup();
    if (m_hPollGroup == k_HSteamNetPollGroup_Invalid)
    {
        CConsole::getConsoleInstance("PgeGsnServer").EOLn("%s() Failed to create poll group!", __func__);
        destroy();
        return false;
    }
    CConsole::getConsoleInstance("PgeGsnServer").OLn("%s() Server listening on port %d", __func__, m_nPort);

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

bool PgeGsnServer::stopListening()
{
    if (!isInitialized())
    {
        CConsole::getConsoleInstance("PgeGsnServer").OLn("%s() Server was not listening, no need to stop.", __func__);
        return true;
    }

    // Close all the connections
    CConsole::getConsoleInstance("PgeGsnServer").OLn("Server closing connections...");
    for (const auto& it : m_mapClients)
    {
        // Send them one more goodbye message.  Note that we also have the
        // connection close reason as a place to send final data.  However,
        // that's usually best left for more diagnostic/debug text not actual
        // protocol strings.
        //SendStringToClient(it.first, "Server is shutting down.  Goodbye.");

        // Close the connection.  We use "linger mode" to ask SteamNetworkingSockets
        // to flush this out and close gracefully.
        m_pInterface->CloseConnection(it.first, k_ESteamNetConnectionEnd_App_Generic, "PgeGsnServer Server Graceful shutdown", true);
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

void PgeGsnServer::SendPacketToClient(HSteamNetConnection conn, const pge_network::PgePacket& pkt)
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

void PgeGsnServer::SendPacketToAllClients(const pge_network::PgePacket& pkt, HSteamNetConnection except)
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

void PgeGsnServer::InjectPacket(const pge_network::PgePacket& pkt)
{
    m_queuePackets.push_back(pkt);
    m_nInjectPktCount++;
    if (m_nInjectPktCount == 1)
    {
        m_time1stInjectPkt = std::chrono::steady_clock::now();
    }
}

void PgeGsnServer::WriteServerClientList()
{
    CConsole::getConsoleInstance("PgeGsnServer").OLnOI("Listing Clients:");
    for (const auto& client : m_mapClients)
    {
        if (client.first == k_HSteamNetConnection_Invalid)
        {
            CConsole::getConsoleInstance("PgeGsnServer").OLn("connHandle: %u (this is me); Name: %s; Address: %s",
                client.first, client.second.m_sCustomName.c_str(), client.second.m_szAddr);
        }
        else
        {
            CConsole::getConsoleInstance("PgeGsnServer").OLn("connHandle: %u; Name: %s; Address: %s",
                client.first, client.second.m_sCustomName.c_str(), client.second.m_szAddr);
        }
    }
    CConsole::getConsoleInstance("PgeGsnServer").OO();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PgeGsnServer::PgeGsnServer(PGEcfgProfiles& cfgProfiles) :
    PgeGsnWrapper(cfgProfiles),
    m_nPort(DEFAULT_SERVER_PORT),
    m_hListenSock(k_HSteamListenSocket_Invalid),
    m_hPollGroup(k_HSteamNetPollGroup_Invalid)
{
} // PgeGsnServer()

PgeGsnServer::PgeGsnServer(const PgeGsnServer& other) :
    PgeGsnWrapper(other.m_cfgProfiles)
{

}

PgeGsnServer& PgeGsnServer::operator=(const PgeGsnServer&)
{
    return *this;
}

int PgeGsnServer::receiveMessages(ISteamNetworkingMessage** pIncomingMsg, int nIncomingMsgArraySize) const
{
    // ReceiveMessagesOnPollGroup() basically copies the pointers to messages from GNS's internal linked list,
    // and unlinks these from that internal linked list, so it is cheap copy to our array
    return m_pInterface->ReceiveMessagesOnPollGroup(m_hPollGroup, pIncomingMsg, nIncomingMsgArraySize);
}

bool PgeGsnServer::validateSteamNetworkingMessage(const HSteamNetConnection& connHandle) const
{
    const auto itClient = m_mapClients.find(connHandle);
    if (itClient == m_mapClients.end())
    {
        CConsole::getConsoleInstance("PgeGsnServer").EOLn("%s: SERVER failed to find connection %u in m_mapClients!",
            __func__, static_cast<unsigned int>(connHandle));
        return false;
    }
    return true;
}

void PgeGsnServer::updateIncomingPgePacket(pge_network::PgePacket& pkt, const HSteamNetConnection& connHandle) const
{
    pkt.m_connHandleServerSide = static_cast<pge_network::PgeNetworkConnectionHandle>(connHandle);
}

void PgeGsnServer::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    // This function is also invoked on main thread when I call PgeGsnServer.PollConnectionStateChanges() from PGE::runGame()
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
            CConsole::getConsoleInstance("PgeGsnServer").OLn("%s: SERVER Connection %s %s, reason %d: %s\n",
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

        CConsole::getConsoleInstance("PgeGsnServer").OLn("%s: SERVER Connection request from %s", __func__, pInfo->m_info.m_szConnectionDescription);

        // A client is attempting to connect
        // Try to accept the connection.
        if (m_pInterface->AcceptConnection(pInfo->m_hConn) != k_EResultOK)
        {
            // This could fail.  If the remote host tried to connect, but then
            // disconnected, the connection may already be half closed.  Just
            // destroy whatever we have on our side.
            m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
            CConsole::getConsoleInstance("PgeGsnServer").EOLn("%s: SERVER Can't accept connection. (It was already closed?)", __func__);
            break;
        }

        // Assign the poll group
        if (!m_pInterface->SetConnectionPollGroup(pInfo->m_hConn, m_hPollGroup))
        {
            m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
            CConsole::getConsoleInstance("PgeGsnServer").EOLn("%s: SERVER Failed to set poll group!", __func__);
            break;
        }

        // Add them to the client list, using std::map wacky syntax
        m_mapClients[pInfo->m_hConn];
        pInfo->m_info.m_addrRemote.ToString(m_mapClients[pInfo->m_hConn].m_szAddr, sizeof(m_mapClients[pInfo->m_hConn].m_szAddr), true);
        CConsole::getConsoleInstance("PgeGsnServer").OLn("%s: SERVER A client is connecting from %s ...", __func__, m_mapClients[pInfo->m_hConn].m_szAddr);

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
            CConsole::getConsoleInstance("PgeGsnServer").EOLn("%s: SERVER Cannot happen: a client (%u) has reached state Connected but not present in clients map!",
                __func__, pInfo->m_hConn);
            assert(false);
        }
        else
        {
            CConsole::getConsoleInstance("PgeGsnServer").OLn("%s: SERVER Client with connHandle %u has reached state Connected!", __func__, pInfo->m_hConn);
        }
        break;
    }

    default:
        // Silences -Wswitch
        break;
    }
}

void PgeGsnServer::SetClientNick(HSteamNetConnection hConn, const char* nick)
{
    // Remember their nick
    m_mapClients[hConn].m_sCustomName = nick;

    // Set the connection name, too, which is useful for debugging
    m_pInterface->SetConnectionName(hConn, nick);
}