/*
    ###################################################################################
    PgeGnsWrapper.cpp
    This file is part of PGE.
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgeGnsWrapper.h"

#include <cassert>

#include "../PGEincludes.h"
#include "../PGEpragmas.h"


static void NetworkDbg(ESteamNetworkingSocketsDebugOutputType eType, const char* pszMsg)
{
    if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug)
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("%s: %s", __func__, pszMsg);
    }
    else
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").OLn("%s: %s", __func__, pszMsg);
    }
}


/*
   PgeGnsWrapper
   ###########################################################################
*/


// ############################### PUBLIC ################################


const uint16 PgeGnsWrapper::DEFAULT_SERVER_PORT;

bool PgeGnsWrapper::init()
{
    if (isInitialized())
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("ERROR: GameNetworkingSockets is already initialized!");
        return false;
    }

    SteamDatagramErrMsg errMsg;

    if (GameNetworkingSockets_Init(nullptr, errMsg))
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").SOLn("Initialized GameNetworkingSockets %s!", GAMENETWORKINGSOCKETS_VER_STR);
    }
    else
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("Failed to initialize GameNetworkingSockets %s: %s!", GAMENETWORKINGSOCKETS_VER_STR, errMsg);
        return false;
    }

    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, NetworkDbg);

    // Select instance to use. For now we'll always use the default.
    // But we could use SteamGameServerNetworkingSockets() on Steam.
    m_pInterface = SteamNetworkingSockets();

    // now main engine loop can invoke pollIncomingMessages() and pollConnectionStateChanges()

    return true;
} // init()

PgeGnsWrapper::~PgeGnsWrapper()
{
    destroy();
} // ~PgeGnsWrapper()

bool PgeGnsWrapper::destroy()
{
    if (!m_pInterface)
    {
        return true;
    }

    m_pInterface = nullptr;

    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Total Tx'd Pkt Count : %u, %u pkt/s", getTxPacketCount(), getTxPacketPerSecondCount());
    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Total Rx'd Pkt Count : %u, %u pkt/s", getRxPacketCount(), getRxPacketPerSecondCount());
    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Total Inj'd Pkt Count: %u, %u pkt/s", getInjectPacketCount(), getInjectPacketPerSecondCount());

    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Total Tx'd Bytes : %u", getTxByteCount());
    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Total Rx'd Bytes : %u", getRxByteCount());
    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Total Inj'd Bytes: %u", getInjectByteCount());
    
    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGnsWrapper").OLnOI("Total Tx'd App Msg Count per AppMsgId:");
    for (const auto& txMsgCount : m_nTxMsgCount)
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Id %u %s: %u", txMsgCount.first, getStringByMsgAppId(txMsgCount.first).c_str(), txMsgCount.second);
    }
    CConsole::getConsoleInstance("PgeGnsWrapper").OO();

    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGnsWrapper").OLnOI("Total Rx'd App Msg Count per AppMsgId:");
    for (const auto& rxMsgCount : m_nRxMsgCount)
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Id %u %s: %u", rxMsgCount.first, getStringByMsgAppId(rxMsgCount.first).c_str(), rxMsgCount.second);
    }
    CConsole::getConsoleInstance("PgeGnsWrapper").OO();

    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGnsWrapper").OLnOI("Total Inj'd App Msg Count per AppMsgId:");
    for (const auto& injectMsgCount : m_nInjectMsgCount)
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Id %u %s: %u", injectMsgCount.first, getStringByMsgAppId(injectMsgCount.first).c_str(), injectMsgCount.second);
    }
    CConsole::getConsoleInstance("PgeGnsWrapper").OLnOO("");

    GameNetworkingSockets_Kill();  // hopefully this can be invoked even if GNS has been already killed
    return true;
} // destroy()

bool PgeGnsWrapper::isInitialized() const
{
    return m_pInterface != nullptr;
}

bool PgeGnsWrapper::pollIncomingMessages()
{
    static const int nIncomingMsgArraySize = 10;
    ISteamNetworkingMessage* pIncomingGnsMsg[nIncomingMsgArraySize];

    if (!isInitialized())
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("%s not initialized!", __func__);
        return false;
    }

    // numGnsMsgs is actually the number of received PgePackets, however GNS talks about SteamNetworkingMessage,
    // but PGE puts a PgePacket into such message, and a PgePacket can contain multiple MsgApps, so pls don't
    // mix the MsgApps with SteamNetworkingMessages, they are not the same messages.
    const int numGnsMsgs = receiveMessages(pIncomingGnsMsg, nIncomingMsgArraySize);
    if (numGnsMsgs == 0)
    {
        return false;
    }

    if (numGnsMsgs < 0)
    {
        // This case falling here would be normal when GNS instance is not connected, actually sometimes
        // we do disconnect on purpose in different cases other than exiting the application.
        // However, we still log this as error and expect the caller NOT invoke us when they know we are not connected.
        CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("%s: Error checking for messages!", __func__);
        return false;
    }

    if (!pIncomingGnsMsg)
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("%s: pIncomingGnsMsg null!", __func__);
        return false;
    }

    for (int i = 0; i < numGnsMsgs; i++)
    {
        if (!validateSteamNetworkingMessage(pIncomingGnsMsg[i]->m_conn))
        {
            continue;
        }

        // PgePacket is a fixed-size memory area, during sending we truncate it to actual size, which is this size:
        const int nActualPktSize = (pIncomingGnsMsg[i])->m_cbSize;
        pge_network::PgePacket pkt;
        assert(nActualPktSize <= sizeof(pkt));
        assert(nActualPktSize > 0);
        
        memcpy(&pkt, (pIncomingGnsMsg[i])->m_pData, nActualPktSize);
        updateIncomingPgePacket(pkt, pIncomingGnsMsg[i]->m_conn);

        // We don't need this anymore.
        // Note that we could even push pIncomingGnsMsg to a queue, and process it later, and
        // release it later, that could be a good speed optimization.
        (pIncomingGnsMsg[i])->Release();

        const pge_network::PgePacket& pktAsConst = pkt;  // from now on we use this const version

        if (pge_network::PgePacket::getPacketId(pktAsConst) == pge_network::PgePktId::Application)
        {
            const uint8_t nMessageCount = pge_network::PgePacket::getMessageAppCount(pktAsConst);
            if (nMessageCount == 0)
            {
                CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("%s: app message pkt with msg count 0 from connection %u!",
                    __func__, pge_network::PgePacket::getServerSideConnectionHandle(pktAsConst));
                assert(false);
                continue;
            }

            // for now we support only 1 app msg / pkt
            assert(nMessageCount == 1);

            const pge_network::MsgApp* pMsgApp = pge_network::PgePacket::getMsgAppFromPkt(pktAsConst);
            assert(pMsgApp);  // never null since it points into pkt
            
            for (uint8_t iAppMsg = 0; iAppMsg < nMessageCount; iAppMsg++)
            {
                // TODO: nooo I need proper iteration for every msgId!
                const pge_network::MsgApp::TMsgId& msgAppId = pge_network::MsgApp::getMsgAppMsgId(*pMsgApp);
                if (m_allowListedAppMessages.end() == m_allowListedAppMessages.find(msgAppId))
                {
                    CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("%s: non-allowlisted app message received: %u from connection %u!",
                        __func__, msgAppId, pge_network::PgePacket::getServerSideConnectionHandle(pktAsConst));
                    assert(false);
                    continue;
                }
                ++m_nRxMsgCount[msgAppId];
                
                // we could also check if nMsgSize is non-zero, however we shouldnt: app is allowed to define zero-size AppMsg, it is
                // not our business here to judge.
                pMsgApp += pge_network::MsgApp::getMsgAppTotalActualSizeBytes(*pMsgApp);
            }
        }
        else
        {
            if (m_allowListedPgeMessages.end() == m_allowListedPgeMessages.find(pge_network::PgePacket::getPacketId(pktAsConst)))
            {
                CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("%s: non-allowlisted pge message received: %u from connection %u!",
                    __func__, pge_network::PgePacket::getPacketId(pktAsConst), pge_network::PgePacket::getServerSideConnectionHandle(pktAsConst));
                assert(false);
                continue;
            }
        }

        m_nRxByteCount += nActualPktSize;
        // TODO: would be nice if even the size of pushed packets would be the actual to copy less memory
        // or at least use emplace_back()
        m_queuePackets.push_back(pktAsConst);
    } // for i

    if (m_nRxPktCount == 0)
    {
        m_time1stRxPkt = std::chrono::steady_clock::now();
    }
    m_nRxPktCount += numGnsMsgs;

    return true;
}

void PgeGnsWrapper::pollConnectionStateChanges()
{
    // since this is also static, we cannot set it in ctor, because both PgeGnsServer and PgeGnsClient are instantiated, and
    // their ctor would set this same static variable ... so it won't work to set it in the ctor.
    // For now I'm just leaving this here so it is always set by the proper instance.
    s_pCallbackInstance = this;
    m_pInterface->RunCallbacks(); // triggers steamNetConnectionStatusChangedCallback()
}

std::size_t PgeGnsWrapper::getPacketQueueSize() const
{
    return m_queuePackets.size();
}

pge_network::PgePacket PgeGnsWrapper::popFrontPacket() noexcept(false)
{
    pge_network::PgePacket pkt = m_queuePackets.front();
    m_queuePackets.pop_front();
    return pkt;
}

std::set<pge_network::PgePktId>& PgeGnsWrapper::getAllowListedPgeMessages()
{
    return m_allowListedPgeMessages;
}

std::set<pge_network::MsgApp::TMsgId>& PgeGnsWrapper::getAllowListedAppMessages()
{
    return m_allowListedAppMessages;
}

uint32_t PgeGnsWrapper::getRxPacketCount() const
{
    return m_nRxPktCount;
}

uint32_t PgeGnsWrapper::getTxPacketCount() const
{
    return m_nTxPktCount;
}

uint32_t PgeGnsWrapper::getInjectPacketCount() const
{
    return m_nInjectPktCount;
}

uint32_t PgeGnsWrapper::getRxPacketPerSecondCount() const
{
    const auto nSecsSince1stRxPkt =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_time1stRxPkt).count();

    return static_cast<uint32_t>(nSecsSince1stRxPkt != 0 ? (getRxPacketCount() / nSecsSince1stRxPkt) : 0);
}

uint32_t PgeGnsWrapper::getTxPacketPerSecondCount() const
{
    const auto nSecsSince1stTxPkt =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_time1stTxPkt).count();

    return static_cast<uint32_t>(nSecsSince1stTxPkt != 0 ? (getTxPacketCount() / nSecsSince1stTxPkt) : 0);
}

uint32_t PgeGnsWrapper::getInjectPacketPerSecondCount() const
{
    const auto nSecsSince1stInjectPkt =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_time1stInjectPkt).count();

    return static_cast<uint32_t>(nSecsSince1stInjectPkt != 0 ? (getInjectPacketCount() / nSecsSince1stInjectPkt) : 0);
}

const std::map<pge_network::MsgApp::TMsgId, uint32_t>& PgeGnsWrapper::getRxMsgCount() const
{
    return m_nRxMsgCount;
}

const std::map<pge_network::MsgApp::TMsgId, uint32_t>& PgeGnsWrapper::getTxMsgCount() const
{
    return m_nTxMsgCount;
}

const std::map<pge_network::MsgApp::TMsgId, uint32_t>& PgeGnsWrapper::getInjectMsgCount() const
{
    return m_nInjectMsgCount;
}

std::map<pge_network::MsgApp::TMsgId, std::string>& PgeGnsWrapper::getMsgAppId2StringMap()
{
    return m_mapMsgAppId2String;
}

uint32_t PgeGnsWrapper::getRxByteCount() const
{
    return m_nRxByteCount;
}

uint32_t PgeGnsWrapper::getTxByteCount() const
{
    return m_nTxByteCount;
}

uint32_t PgeGnsWrapper::getInjectByteCount() const
{
    return m_nInjectByteCount;
}


// ############################## PROTECTED ##############################


PgeGnsWrapper* PgeGnsWrapper::s_pCallbackInstance = nullptr;

void PgeGnsWrapper::steamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    s_pCallbackInstance->onSteamNetConnectionStatusChanged(pInfo);
}

PgeGnsWrapper::PgeGnsWrapper(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    m_pInterface(nullptr),
    m_nRxPktCount(0),
    m_nTxPktCount(0),
    m_nInjectPktCount(0),
    m_nRxByteCount(0),
    m_nTxByteCount(0),
    m_nInjectByteCount(0)
{
} // PgeGnsWrapper()

PgeGnsWrapper::PgeGnsWrapper(const PgeGnsWrapper& other) :
    m_cfgProfiles(other.m_cfgProfiles)
{

}

PgeGnsWrapper& PgeGnsWrapper::operator=(const PgeGnsWrapper&)
{
    return *this;
}

std::string PgeGnsWrapper::getStringByMsgAppId(const pge_network::MsgApp::TMsgId& id) const
{
    const auto& it = m_mapMsgAppId2String.find(id);
    if (it != m_mapMsgAppId2String.end())
    {
        return it->second;
    }
    return "UNKNOWN_MSG";
}

std::string PgeGnsWrapper::getDetailedConnectionStatus(const HSteamNetConnection& connHandle) const
{
    if (!isInitialized() || (connHandle == k_HSteamNetConnection_Invalid))
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("%s not connected or invalid connHandle: %u!", __func__, connHandle);
        assert(false);
        return "";
    }

    char szDetailedStatus[4096];
    const int nRes = m_pInterface->GetDetailedConnectionStatus(connHandle, szDetailedStatus, sizeof(szDetailedStatus));
    if (nRes == 0)
    {
        // only in this case szDetailedStatus is null-terminated and perfect!
        return std::string(szDetailedStatus);
        //const std::string sDetailedStatusWithUnixNewlines(szDetailedStatus);
        //const size_t nNewlineCount = std::count(sDetailedStatusWithUnixNewlines.cbegin(), sDetailedStatusWithUnixNewlines.cend(), '\n');
        //std::string sDetailedStatusWithWindowsNewlines;
        //sDetailedStatusWithWindowsNewlines.reserve(sDetailedStatusWithUnixNewlines.size() + nNewlineCount);
        //for (const auto& c : sDetailedStatusWithUnixNewlines)
        //{
        //    sDetailedStatusWithWindowsNewlines += c;
        //    if (c == '\n')
        //    {
        //        sDetailedStatusWithWindowsNewlines += '\r';
        //    }
        //}
        //return sDetailedStatusWithWindowsNewlines;
    }
    else
    {
        CConsole::getConsoleInstance("PgeGnsWrapper").EOLn("%s GetDetailedConnectionStatus() returned %d!", __func__, nRes);
        return "";
    }
}

void PgeGnsWrapper::logDetailedConnectionStatus(const HSteamNetConnection& connHandle) const
{
    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("Detailed Connection Status for connHandle %u: ", connHandle);
    // we need this line-by-line logging otherwise no paragraph breaks will be inserted into the html at each newline;
    // note that this could be a feature of the logger lib in the future.
    std::stringstream ssDetailedConnStatus(getDetailedConnectionStatus(connHandle));
    while (ssDetailedConnStatus)
    {
        std::string sLine;
        std::getline(ssDetailedConnStatus, sLine);
        CConsole::getConsoleInstance("PgeGnsWrapper").OLn("%s", sLine.c_str());
    }
    CConsole::getConsoleInstance("PgeGnsWrapper").OLn("");
}


// ############################### PRIVATE ###############################
