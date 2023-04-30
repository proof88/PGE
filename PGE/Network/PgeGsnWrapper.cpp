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

#include <cassert>
#include <filesystem>  // requires cpp17
#include <set>

#include "PgeGsnWrapper.h"
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

    // now main engine loop can invoke PollIncomingMessages() and PollConnectionStateChanges()

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

    GameNetworkingSockets_Kill();  // hopefully this can be invoked even if GNS has been already killed
    return true;
} // destroy()

bool PgeGsnWrapper::isInitialized() const
{
    return m_pInterface != nullptr;
}

void PgeGsnWrapper::PollConnectionStateChanges()
{
    s_pCallbackInstance = this; // TODO: this could be set only once somewhere in init or ctor
    m_pInterface->RunCallbacks();
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


// ############################## PROTECTED ##############################


PgeGsnWrapper* PgeGsnWrapper::s_pCallbackInstance = nullptr;

void PgeGsnWrapper::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo)
{
    s_pCallbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
}

PgeGsnWrapper::PgeGsnWrapper(PGEcfgProfiles& cfgProfiles) :
    m_cfgProfiles(cfgProfiles),
    m_pInterface(nullptr),
    m_nRxPktCount(0),
    m_nTxPktCount(0),
    m_nInjectPktCount(0)
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


// ############################### PRIVATE ###############################
