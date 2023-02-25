#pragma once

/*
    ###################################################################################
    PGESysNET.h
    This file is part of PGE.
    Internal header.
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PGEallHeaders.h"

#include <cstdint>
#include <deque>
#include <map>
#include <string>

#include "../Config/PGEcfgProfiles.h"
#include "PgePacket.h"

// this idea of building include paths is coming from:
// https://stackoverflow.com/questions/32066204/construct-path-for-include-directive-with-macro
#define IDENT(x) x
#define XSTR(x) #x
#define STR(x) XSTR(x)
#define PATH2(x,y) STR(IDENT(x)IDENT(y))
#define PATH3(x,y,z) STR(IDENT(x)IDENT(y)IDENT(z))

#define GAMENETWORKINGSOCKETS_VER 1.4.0
#define GAMENETWORKINGSOCKETS_VER_STR STR(GAMENETWORKINGSOCKETS_VER)

// TODO: add "Network/GameNetworkingSockets-1.4.0/include" to project include dirs, do not use that path here!
// TODO: to log lib version, extract version from %AdditionalIncludeDirectories
#include PATH3(GameNetworkingSockets-,GAMENETWORKINGSOCKETS_VER,/include/steam/steamnetworkingsockets.h)
#include PATH3(GameNetworkingSockets-,GAMENETWORKINGSOCKETS_VER,/include/steam/isteamnetworkingutils.h)

struct TClient
{
    std::string m_sCustomName;  /**< App level can set a custom name for client which is useful for debugging. */
    char m_szAddr[SteamNetworkingIPAddr::k_cchMaxString];
};

// TODO: this whole PGESysNET should be also separated into client and server code, similar to how PgeNetwork separates
// it into PgeClient and PgeServer, but currently no time for this ...

/**
    PR00F's Game Engine networking subsystem.
*/
class PGESysNET
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PGESysNET is included")   
#endif

public:
    static const uint16 DEFAULT_SERVER_PORT = 27020;

    static PGESysNET& createAndGet(PGEcfgProfiles& cfgProfiles);   /**< Creates and gets the singleton implementation instance. */

    static bool isServer();

    // ---------------------------------------------------------------------------

    virtual ~PGESysNET();

    bool initSysNET(void);
    bool destroySysNET(void);
    bool isInitialized() const;

    const HSteamNetConnection& getConnectionHandle() const;
    const HSteamNetConnection& getConnectionHandleServerSide() const;
    const char* getServerAddress() const;
    
    bool PollIncomingMessages();
    void PollConnectionStateChanges();

    void SendPacketToClient(HSteamNetConnection conn, const pge_network::PgePacket& pkt);
    void SendPacketToAllClients(const pge_network::PgePacket& pkt, HSteamNetConnection except = k_HSteamNetConnection_Invalid);
    void SendToServer(const pge_network::PgePacket& pkt);
    void InjectPacket(const pge_network::PgePacket& pkt);
    std::size_t getPacketQueueSize() const;
    pge_network::PgePacket popFrontPacket();

    std::set<pge_network::PgePktId>& getBlackListedPgeMessages();
    std::set<pge_network::TPgeMsgAppMsgId>& getBlackListedAppMessages();

    const SteamNetConnectionRealTimeStatus_t& getRealTimeStatus(bool bForceUpdate);
    std::string getDetailedStatus() const;

    bool connectToServer(const std::string& sServerAddress); /* temporal, now I dont have better idea in this time */
    bool DisconnectClient();
    bool startListening();
    bool stopListening();

    uint32_t getRxPacketCount() const;
    uint32_t getTxPacketCount() const;

    void WriteServerClientList();

private:

    static PGESysNET* s_pCallbackInstance;
    static bool s_bServer;

    PGEcfgProfiles& m_cfgProfiles;
    uint16 m_nPort;
    SteamNetworkingIPAddr m_addrServer;  // used by client only
    char m_szAddr[SteamNetworkingIPAddr::k_cchMaxString];  // used by client only
    HSteamNetConnection m_hConnection; // used by client only
    
    // TODO: this is not set currently. Because Client code at this level in PollConnectionStateChanges() knows only about its local
    // connection handle. Server should send a specific Pge message back to client holding this info.
    // Even though it is filled as m_connHandleServerSide in most packets, those packets are only sent when APP level sends something.
    // Because currently there is pge_network level packet sent by server to client during connection buildup.
    // I think the MsgUserConnected should contain this info and should be sent to clients by server upon establishing connection.
    // I also think that IP address should be also part of this enhanced MsgUserConnected messages. This is part of APP level MsgUserSetup
    // today.
    HSteamNetConnection m_hConnectionServerSide; // used by client only

    ISteamNetworkingSockets* m_pInterface;
    HSteamListenSocket m_hListenSock;  // used by server only
    HSteamNetPollGroup m_hPollGroup;   // used by server only
    SteamNetConnectionRealTimeStatus_t m_connRtStatus;  // used by client only

    std::map< HSteamNetConnection, TClient > m_mapClients;  // used by server only
    // The connection handle is used to identify clients in the map.
    // This handle for a client is NOT the same as the handle on the client side, obviously.
    // Note that the first connection is this map is an invalid connection (k_HSteamNetConnection_Invalid), which
    // is the server itself, this is how this layer stores nickname for the server (self).
    

    std::deque<pge_network::PgePacket> m_queuePackets;  // used by both client and server
    std::set<pge_network::PgePktId> m_blackListedPgeMessages;  // used by both client and server
    std::set<pge_network::TPgeMsgAppMsgId> m_blackListedAppMessages;  // used by both client and server

    uint32_t m_nRxPktCount;
    uint32_t m_nTxPktCount;

    static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);

    // ---------------------------------------------------------------------------

    explicit PGESysNET(PGEcfgProfiles& cfgProfiles);
    PGESysNET(const PGESysNET&); 
    PGESysNET& operator=(const PGESysNET&);

    void SetClientNick(HSteamNetConnection hConn, const char* nick);
    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

    friend class PGE;

}; // class PGESysNET