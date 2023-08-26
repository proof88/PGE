/*
    ###################################################################################
    PgeGsnWrapper.cpp
    This file is part of PGE.
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgeGsnWrapper.h"

#include <cassert>

#include "../PGEincludes.h"
#include "../PGEpragmas.h"


static void NetworkDbg(ESteamNetworkingSocketsDebugOutputType eType, const char* pszMsg)
{
    if (eType == k_ESteamNetworkingSocketsDebugOutputType_Bug)
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").EOLn("%s: %s", __func__, pszMsg);
    }
    else
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").OLn("%s: %s", __func__, pszMsg);
    }
}


/*
   PgeGsnWrapper
   ###########################################################################
*/


// ############################### PUBLIC ################################


const uint16 PgeGsnWrapper::DEFAULT_SERVER_PORT;

bool PgeGsnWrapper::init()
{
    if (isInitialized())
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").EOLn("ERROR: GameNetworkingSockets is already initialized!");
        return false;
    }

    SteamDatagramErrMsg errMsg;

    if (GameNetworkingSockets_Init(nullptr, errMsg))
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").SOLn("Initialized GameNetworkingSockets %s!", GAMENETWORKINGSOCKETS_VER_STR);
    }
    else
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").EOLn("Failed to initialize GameNetworkingSockets %s: %s!", GAMENETWORKINGSOCKETS_VER_STR, errMsg);
        return false;
    }

    SteamNetworkingUtils()->SetDebugOutputFunction(k_ESteamNetworkingSocketsDebugOutputType_Msg, NetworkDbg);

    // Select instance to use. For now we'll always use the default.
    // But we could use SteamGameServerNetworkingSockets() on Steam.
    m_pInterface = SteamNetworkingSockets();

    // now main engine loop can invoke pollIncomingMessages() and pollConnectionStateChanges()

    return true;
} // init()

PgeGsnWrapper::~PgeGsnWrapper()
{
    destroy();
} // ~PgeGsnWrapper()

bool PgeGsnWrapper::destroy()
{
    if (!m_pInterface)
    {
        return true;
    }

    m_pInterface = nullptr;

    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("Total Tx'd Pkt Count : %u, %u pkt/s", getTxPacketCount(), getTxPacketPerSecondCount());
    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("Total Rx'd Pkt Count : %u, %u pkt/s", getRxPacketCount(), getRxPacketPerSecondCount());
    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("Total Inj'd Pkt Count: %u, %u pkt/s", getInjectPacketCount(), getInjectPacketPerSecondCount());

    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("Total Tx'd Bytes : %u", getTxByteCount());
    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("Total Rx'd Bytes : %u", getRxByteCount());
    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("Total Inj'd Bytes: %u", getInjectByteCount());
    
    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGsnWrapper").OLnOI("Total Tx'd App Msg Count per AppMsgId:");
    for (const auto& txMsgCount : m_nTxMsgCount)
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").OLn("Id %u %s: %u", txMsgCount.first, getStringByMsgAppId(txMsgCount.first).c_str(), txMsgCount.second);
    }
    CConsole::getConsoleInstance("PgeGsnWrapper").OO();

    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGsnWrapper").OLnOI("Total Rx'd App Msg Count per AppMsgId:");
    for (const auto& rxMsgCount : m_nRxMsgCount)
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").OLn("Id %u %s: %u", rxMsgCount.first, getStringByMsgAppId(rxMsgCount.first).c_str(), rxMsgCount.second);
    }
    CConsole::getConsoleInstance("PgeGsnWrapper").OO();

    CConsole::getConsoleInstance("PgeGsnWrapper").OLn("");
    CConsole::getConsoleInstance("PgeGsnWrapper").OLnOI("Total Inj'd App Msg Count per AppMsgId:");
    for (const auto& injectMsgCount : m_nInjectMsgCount)
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").OLn("Id %u %s: %u", injectMsgCount.first, getStringByMsgAppId(injectMsgCount.first).c_str(), injectMsgCount.second);
    }
    CConsole::getConsoleInstance("PgeGsnWrapper").OLnOO("");

    GameNetworkingSockets_Kill();  // hopefully this can be invoked even if GNS has been already killed
    return true;
} // destroy()

bool PgeGsnWrapper::isInitialized() const
{
    return m_pInterface != nullptr;
}

bool PgeGsnWrapper::pollIncomingMessages()
{
    static const int nIncomingMsgArraySize = 10; // TODO: make this configurable from outside
    ISteamNetworkingMessage* pIncomingGnsMsg[nIncomingMsgArraySize];

    if (!isInitialized())
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").EOLn("%s not initialized!", __func__);
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
        CConsole::getConsoleInstance("PgeGsnWrapper").EOLn("%s: Error checking for messages!", __func__);
        return false;
    }

    if (!pIncomingGnsMsg)
    {
        CConsole::getConsoleInstance("PgeGsnWrapper").EOLn("%s: pIncomingGnsMsg null!", __func__);
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
        
        memcpy(&pkt, (pIncomingGnsMsg[i])->m_pData, nActualPktSize);
        updateIncomingPgePacket(pkt, pIncomingGnsMsg[i]->m_conn);

        // We don't need this anymore.
        // Note that we could even push pIncomingGnsMsg to a queue, and process it later, and
        // release it later, that could be a good speed optimization.
        (pIncomingGnsMsg[i])->Release();

        if (pkt.pktId == pge_network::PgePktId::APP)
        {
            if (pkt.msg.app.m_nMessageCount == 0)
            {
                CConsole::getConsoleInstance("PgeGsnWrapper").EOLn("%s: app message pkt with msg count 0 from connection %u!",
                    __func__, pkt.m_connHandleServerSide);
                assert(false);
                continue;
            }

            // for now we support only 1 app msg / pkt
            assert(pkt.msg.app.m_nMessageCount == 1);

            const pge_network::MsgApp* pMsgApp = reinterpret_cast<const pge_network::MsgApp*>(pkt.msg.app.cData);
            for (uint8_t iAppMsg = 0; iAppMsg < pkt.msg.app.m_nMessageCount; iAppMsg++)
            {
                // TODO: nooo I need proper iteration for every msgId!
                if (m_allowListedAppMessages.end() == m_allowListedAppMessages.find(pMsgApp->msgId))
                {
                    CConsole::getConsoleInstance("PgeGsnWrapper").EOLn("%s: non-allowlisted app message received: %u from connection %u!",
                        __func__, pMsgApp->msgId, pkt.m_connHandleServerSide);
                    assert(false);
                    continue;
                }
                ++m_nRxMsgCount[pMsgApp->msgId];
                
                // we could also check if nMsgSize is non-zero, however we shouldnt: app is allowed to define zero-size AppMsg, it is
                // not our business here to judge.
                pMsgApp += pMsgApp->nMsgSize;
            }
        }
        else
        {
            if (m_allowListedPgeMessages.end() == m_allowListedPgeMessages.find(pkt.pktId))
            {
                CConsole::getConsoleInstance("PgeGsnWrapper").EOLn("%s: non-allowlisted pge message received: %u from connection %u!",
                    __func__, pkt.pktId, pkt.m_connHandleServerSide);
                assert(false);
                continue;
            }
        }

        m_nRxByteCount += nActualPktSize;
        // TODO: would be nice if even the size of pushed packets would be the actual to copy less memory
        // or at least use emplace_back()
        m_queuePackets.push_back(pkt);
    } // for i

    if (m_nRxPktCount == 0)
    {
        m_time1stRxPkt = std::chrono::steady_clock::now();
    }
    m_nRxPktCount += numGnsMsgs;

    return true;
}

void PgeGsnWrapper::pollConnectionStateChanges()
{
    // since this is also static, we cannot set it in ctor, because both PgeGsnServer and PgeGsnClient are instantiated, and
    // their ctor would set this same static variable ... so it won't work to set it in the ctor.
    // For now I'm just leaving this here so it is always set by the proper instance.
    s_pCallbackInstance = this;
    m_pInterface->RunCallbacks(); // triggers steamNetConnectionStatusChangedCallback()
}

std::size_t PgeGsnWrapper::getPacketQueueSize() const
{
    return m_queuePackets.size();
}

pge_network::PgePacket PgeGsnWrapper::popFrontPacket() noexcept(false)
{
    pge_network::PgePacket pkt = m_queuePackets.front();
    m_queuePackets.pop_front();
    return pkt;
}

std::set<pge_network::PgePktId>& PgeGsnWrapper::getAllowListedPgeMessages()
{
    return m_allowListedPgeMessages;
}

std::set<pge_network::TPgeMsgAppMsgId>& PgeGsnWrapper::getAllowListedAppMessages()
{
    return m_allowListedAppMessages;
}

uint32_t PgeGsnWrapper::getRxPacketCount() const
{
    return m_nRxPktCount;
}

uint32_t PgeGsnWrapper::getTxPacketCount() const
{
    return m_nTxPktCount;
}

uint32_t PgeGsnWrapper::getInjectPacketCount() const
{
    return m_nInjectPktCount;
}

uint32_t PgeGsnWrapper::getRxPacketPerSecondCount() const
{
    const auto nSecsSince1stRxPkt =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_time1stRxPkt).count();

    return static_cast<uint32_t>(nSecsSince1stRxPkt != 0 ? (getRxPacketCount() / nSecsSince1stRxPkt) : 0);
}

uint32_t PgeGsnWrapper::getTxPacketPerSecondCount() const
{
    const auto nSecsSince1stTxPkt =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_time1stTxPkt).count();

    return static_cast<uint32_t>(nSecsSince1stTxPkt != 0 ? (getTxPacketCount() / nSecsSince1stTxPkt) : 0);
}

uint32_t PgeGsnWrapper::getInjectPacketPerSecondCount() const
{
    const auto nSecsSince1stInjectPkt =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_time1stInjectPkt).count();

    return static_cast<uint32_t>(nSecsSince1stInjectPkt != 0 ? (getInjectPacketCount() / nSecsSince1stInjectPkt) : 0);
}

const std::map<pge_network::TPgeMsgAppMsgId, uint32_t>& PgeGsnWrapper::getRxMsgCount() const
{
    return m_nRxMsgCount;
}

const std::map<pge_network::TPgeMsgAppMsgId, uint32_t>& PgeGsnWrapper::getTxMsgCount() const
{
    return m_nTxMsgCount;
}

const std::map<pge_network::TPgeMsgAppMsgId, uint32_t>& PgeGsnWrapper::getInjectMsgCount() const
{
    return m_nInjectMsgCount;
}

std::map<pge_network::TPgeMsgAppMsgId, std::string>& PgeGsnWrapper::getMsgAppId2StringMap()
{
    return m_mapMsgAppId2String;
}

uint32_t PgeGsnWrapper::getRxByteCount() const
{
    return m_nRxByteCount;
}

uint32_t PgeGsnWrapper::getTxByteCount() const
{
    return m_nTxByteCount;
}

uint32_t PgeGsnWrapper::getInjectByteCount() const
{
    return m_nInjectByteCount;
}


// ############################## PROTECTED ##############################


PgeGsnWrapper* PgeGsnWrapper::s_pCallbackInstance = nullptr;

void PgeGsnWrapper::steamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    s_pCallbackInstance->onSteamNetConnectionStatusChanged(pInfo);
}

PgeGsnWrapper::PgeGsnWrapper(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    m_pInterface(nullptr),
    m_nRxPktCount(0),
    m_nTxPktCount(0),
    m_nInjectPktCount(0),
    m_nRxByteCount(0),
    m_nTxByteCount(0),
    m_nInjectByteCount(0)
{
} // PgeGsnWrapper()

PgeGsnWrapper::PgeGsnWrapper(const PgeGsnWrapper& other) :
    m_cfgProfiles(other.m_cfgProfiles)
{

}

PgeGsnWrapper& PgeGsnWrapper::operator=(const PgeGsnWrapper&)
{
    return *this;
}

std::string PgeGsnWrapper::getStringByMsgAppId(const pge_network::TPgeMsgAppMsgId& id) const
{
    const auto& it = m_mapMsgAppId2String.find(id);
    if (it != m_mapMsgAppId2String.end())
    {
        return it->second;
    }
    return "UNKNOWN_MSG";
}


// ############################### PRIVATE ###############################
