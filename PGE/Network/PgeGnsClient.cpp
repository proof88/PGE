/*
    ###################################################################################
    PgeGnsClient.cpp
    This file is part of PGE.
    Client Wrapper for GameNetworkingSockets library
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include <cassert>

#include "PgeGnsClient.h"
#include "../PGEincludes.h"
#include "../PGEpragmas.h"


/*
   PgeGnsClient
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PgeGnsClient& PgeGnsClient::createAndGet(PGEcfgProfiles& cfgProfiles)
{
    static PgeGnsClient inst(cfgProfiles);
    return inst;
} // createAndGet()

PgeGnsClient::~PgeGnsClient()
{
    destroy();
} // ~PgeGnsClient()

bool PgeGnsClient::destroy()
{
    if (!isInitialized())
    {
        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s() Already destroyed.", __func__);
        return true;
    }
    if (isConnected())
    {
        logDetailedConnectionStatus(m_hConnection);
    }
    
    return disconnectClient() && PgeGnsWrapper::destroy();
} // destroy()

bool PgeGnsClient::connectToServer(
    const std::string& sServerAddress,
    const std::string& sAppVersion)
{
    if (isConnected())
    {
        CConsole::getConsoleInstance("PgeGnsClient").EOLn("%s ERROR: already connected to %s!", __func__, m_szAddr);
        return false;
    }

    if (!isInitialized())
    {
        CConsole::getConsoleInstance("PgeGnsClient").EOLn("%s() ERROR: not initialized!", __func__);
        return true;
    }

    // currently there is no separate initialization logic here, only connecting to server, however
    // we still have a logical separation of isInitialized() and isConnected()

    m_addrServer.Clear();
    if (!m_addrServer.ParseString(sServerAddress.c_str()))
    {
        CConsole::getConsoleInstance("PgeGnsClient").EOLn("%s() Failed to parse address: %s!", __func__, sServerAddress.c_str());
        return false;
    }

    if (m_addrServer.m_port == 0)
    {
        m_addrServer.m_port = DEFAULT_SERVER_PORT;
        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s() Using default port: %d", __func__, m_addrServer.m_port);
    }
    else
    {
        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s() Using custom port: %d", __func__, m_addrServer.m_port);
    }

    // Parse back to string just to make sure we are logging and connecting to proper address
    m_addrServer.ToString(m_szAddr, sizeof(m_szAddr), true);
    CConsole::getConsoleInstance("PgeGnsClient").OLn("%s Connecting to server at %s", __func__, m_szAddr);

    m_sAppVersion = sAppVersion;
    if (m_sAppVersion.empty())
    {
        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s() no client app version is specified!", __func__);
    }
    else
    {
        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s() client app version is specified as: %s!", __func__, m_sAppVersion.c_str());
    }

    SteamNetworkingConfigValue_t opt;
    opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)steamNetConnectionStatusChangedCallback);

    // From now steamNetConnectionStatusChangedCallback() will be called back for connection status updates and that is where m_sAppVersion is also sent to server
    m_hConnection = m_pInterface->ConnectByIPAddress(m_addrServer, 1, &opt);
    if (m_hConnection == k_HSteamNetConnection_Invalid)
    {
        CConsole::getConsoleInstance("PgeGnsClient").EOLn("Client Failed to create connection");
        return false;
    }

    //SteamNetConnectionInfo_t connInfo;
    //m_pInterface->GetConnectionInfo(m_hConnection, &connInfo);

    return true;
}

bool PgeGnsClient::disconnectClient(const std::string& sExtraDebugText)
{
    if (!isConnected())
    {
        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s not connected.", __func__);
        return true;
    }

    const std::string sExtraDebugTextToSend =
        sExtraDebugText.empty() ?
        "PgeGnsClient Client Graceful disconnectClient()" :
        "PgeGnsClient Client Graceful disconnectClient(), reason: " + sExtraDebugText;

    CConsole::getConsoleInstance("PgeGnsClient").OLn(
        "Client closing connection ... Reason: %s",
        sExtraDebugText.empty() ? "" : sExtraDebugText.c_str());

    // here we create a server disconnect pkt that will be injected to our queue so app level can process it
    // for the server itself, as it was a real server disconnecting
    pge_network::PgePacket pkt;
    pge_network::PgePacket::initPktPgeMsgUserDisconnected(
        pkt,
        static_cast<pge_network::PgeNetworkConnectionHandle>(k_HSteamNetConnection_Invalid));
    // we push this packet to our pkt queue, this is how we "send" message to ourselves so client game loop can process it
    m_queuePackets.push_back(pkt);

    // we cannot inject similar pkt about ourselves because we dont have our server-side connection handle (that would be stored in
    // m_hConnectionServerSide - somehow I never implemented it)

    m_pInterface->CloseConnection(m_hConnection, k_ESteamNetConnectionEnd_App_Generic, sExtraDebugTextToSend.c_str(), true);
    m_hConnection = k_HSteamNetConnection_Invalid;
    m_hConnectionServerSide = k_HSteamNetConnection_Invalid;
    m_addrServer.Clear();
    memset(m_szAddr, sizeof(m_szAddr), 0);
    memset(&m_connRtStatus, 0, sizeof(m_connRtStatus));

    return true;
}

bool PgeGnsClient::isConnected() const
{
    return m_hConnection != k_HSteamNetConnection_Invalid;
} // isConnected()

const HSteamNetConnection& PgeGnsClient::getConnectionHandle() const
{
    return m_hConnection;
}

const HSteamNetConnection& PgeGnsClient::getConnectionHandleServerSide() const
{
    return m_hConnectionServerSide;
}

const char* PgeGnsClient::getServerAddress() const
{
    return m_szAddr;
}

void PgeGnsClient::sendToServer(const pge_network::PgePacket& pkt)
{
    if (!isConnected())
    {
        CConsole::getConsoleInstance("PgeGnsClient").EOLn("%s not connected!", __func__);
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

    m_pInterface->SendMessageToConnection(m_hConnection, &pkt, nActualPktSize, k_nSteamNetworkingSend_Reliable, nullptr);
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

const SteamNetConnectionRealTimeStatus_t& PgeGnsClient::getRealTimeStatus(bool bForceUpdate)
{
    if (!isConnected())
    {
        CConsole::getConsoleInstance("PgeGnsClient").EOLn("%s not connected!", __func__);
        assert(false);
        return m_connRtStatus;
    }

    if (bForceUpdate)
    {
        m_pInterface->GetConnectionRealTimeStatus(m_hConnection, &m_connRtStatus, 0, NULL);
    }

    return m_connRtStatus;
}

std::string PgeGnsClient::getDetailedConnectionStatus() const
{
    if (!isConnected())
    {
        CConsole::getConsoleInstance("PgeGnsClient").EOLn("%s not connected!", __func__);
        assert(false);
        return "";
    }

    return PgeGnsWrapper::getDetailedConnectionStatus(m_hConnection);
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################

PgeGnsClient::PgeGnsClient(PGEcfgProfiles& cfgProfiles) :
    PgeGnsWrapper(cfgProfiles),
    m_hConnection(k_HSteamNetConnection_Invalid),
    m_hConnectionServerSide(k_HSteamNetConnection_Invalid)
{
    m_addrServer.Clear();
    memset(&m_connRtStatus, 0, sizeof(m_connRtStatus));
} // PgeGnsClient()

PgeGnsClient::PgeGnsClient(const PgeGnsClient& other) :
    PgeGnsWrapper(other.m_cfgProfiles)
{

}

PgeGnsClient& PgeGnsClient::operator=(const PgeGnsClient&)
{
    return *this;
}

bool PgeGnsClient::pgeMessageIsHandledAtGnsLevel(const pge_network::PgePacket& /*pktReceivedFromServer*/)
{
    // client currently does not do such
    return false;
}

int PgeGnsClient::receiveMessages(ISteamNetworkingMessage** pIncomingMsg, int nIncomingMsgArraySize) const
{
    // ReceiveMessagesOnConnection() basically copies the pointers to messages from GNS's internal linked list,
    // and unlinks these from the linked list, so it is cheap copy to our array
    return m_pInterface->ReceiveMessagesOnConnection(m_hConnection, pIncomingMsg, nIncomingMsgArraySize);
}

bool PgeGnsClient::validateSteamNetworkingMessage(const HSteamNetConnection&) const
{
    return true;
}

void PgeGnsClient::updateIncomingPgePacket(pge_network::PgePacket&, const HSteamNetConnection&) const
{
    // here we are client, we don't set pkt.connHandle because we expect it to be already properly filled in by sender (server)!
}

void PgeGnsClient::onSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    // This function is also invoked on main thread when I call PgeGnsClient.pollConnectionStateChanges() from PGE::runGame()
    // so no need to utilize mutexes around here.
    // And the other function pollIncomingMessages() is also invoked by PGE::runGame().
    // So it is safe to do operations on m_queuePackets.

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
            CConsole::getConsoleInstance("PgeGnsClient").EOLn("%s: CLIENT problem 1 (rejected I guess) (%s)",
                __func__, pInfo->m_info.m_szEndDebug);
        }
        else if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
        {
            CConsole::getConsoleInstance("PgeGnsClient").EOLn("%s: CLIENT problem 2 (guess our machine has lost network connection) (%s)",
                __func__, pInfo->m_info.m_szEndDebug);
        }
        else
        {
            // NOTE: We could check the reason code for a normal disconnection
            CConsole::getConsoleInstance("PgeGnsClient").EOLn("%s: CLIENT problem 3 (closed by peer) (%s)",
                __func__, pInfo->m_info.m_szEndDebug);
        }

        logDetailedConnectionStatus(pInfo->m_hConn);

        // Clean up the connection.  This is important!
        // The connection is "closed" in the network sense, but
        // it has not been destroyed.  We must close it on our end, too
        // to finish up.  The reason information do not matter in this case,
        // and we cannot linger because it's already closed on the other end,
        // so we just pass 0's.
        m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
        m_hConnection = k_HSteamNetConnection_Invalid;

        // TODO: it would be better to invoke disconnectClient() because if we are here because server disappeared unexpectedly, then
        // it would be better to call disconnectClient() which also pushes a server disconnect message to application level.
        // However, when I invoked it, then in the meantime PRooFPS-dd's onGameRunning() loop detects the connection to be lost and
        // invokes connectToServer() which will eventually update m_hConnection with a new handle, since isConnected() is NOT reliable,
        // as described here: https://github.com/proof88/PRooFPS-dd/issues/261 .
        // So to properly solve this issue, https://github.com/proof88/PRooFPS-dd/issues/261 needs to be solved.
        // And then we can invoke disconnectClient here. Until it is solved, there is no proper way to handle on client-side at application
        // level when the server makes an ungraceful shutdown (except if we remove the auto reconnect thing from app level).
        // I'm tracking this bug in this ticket: https://github.com/proof88/PRooFPS-dd/issues/272 .
        //disconnectClient("Server disappeared");
        break;
    }

    case k_ESteamNetworkingConnectionState_Connecting:
        // We will get this callback when we start connecting.
        // We can ignore this.
        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s: CLIENT Connecting ...", __func__);
        break;

    case k_ESteamNetworkingConnectionState_Connected:
    {
        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s: CLIENT Connected to server OK", __func__);

        // KEKEKEKEKE
        // you jumped here from PgePacket.h MsgUserConnectedServerSelf.
        char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
        pInfo->m_info.m_addrRemote.ToString(szAddr, sizeof(szAddr), true);
        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s: CLIENT Remote address: %s", __func__, szAddr);

        CConsole::getConsoleInstance("PgeGnsClient").OLn("%s: CLIENT conn. description: %s", __func__, pInfo->m_info.m_szConnectionDescription);

        // server accepted our connection, now immediately send our app version, otherwise server app won't start talking to us.
        pge_network::PgePacket pktMsgClientAppVersion;
        pge_network::PgePacket::initPktPgeMsgClientAppVersionFromClient(
            pktMsgClientAppVersion,
            m_sAppVersion);

        sendToServer(pktMsgClientAppVersion);

        break;
    }
    default:
        // Silences -Wswitch
        break;
    }
}
