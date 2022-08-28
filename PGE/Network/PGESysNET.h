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

    static bool isServer();

    // ---------------------------------------------------------------------------

    // TODO: these needs to be private, temporarily here!
    HSteamNetConnection m_hConnection; // used by client only
    std::deque<PgePacket> queuePackets;  // used by both client and server

    PGESysNET();
    virtual ~PGESysNET();

    bool initSysNET(void);
    bool destroySysNET(void);
    bool isInitialized() const;
    
    bool PollIncomingMessages();
    void PollConnectionStateChanges();

    void SendStringToClient(HSteamNetConnection conn, const char* str);
    void SendPacketToClient(HSteamNetConnection conn, const PgePacket& pkt);
    void SendStringToAllClients(const char* str, HSteamNetConnection except = k_HSteamNetConnection_Invalid);
    void SendPacketToAllClients(const PgePacket& pkt, HSteamNetConnection except = k_HSteamNetConnection_Invalid);
    void SendPacketToServer(const PgePacket& pkt);
    const SteamNetConnectionRealTimeStatus_t& getRealTimeStatus(bool bForceUpdate);

    bool ConnectClient(const std::string& sServerAddress); /* temporal, now I dont have better idea in this time */
    bool StartListening();

private:

    static PGESysNET* s_pCallbackInstance;
    static bool bServer;

    uint16 nPort;
    SteamNetworkingIPAddr addrServer;  // used by client only
    ISteamNetworkingSockets* m_pInterface;
    HSteamListenSocket m_hListenSock;  // used by server only
    HSteamNetPollGroup m_hPollGroup;   // used by server only
    SteamNetConnectionRealTimeStatus_t connRtStatus;  // used by client only

    std::map< HSteamNetConnection, std::string > m_mapClients;  // used by server only
    // note that the first connection is this map is an invalid connection (k_HSteamNetConnection_Invalid), which
    // is the server itself, this is how this layer stores nickname for the server (self)

    static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pInfo);

    // ---------------------------------------------------------------------------

    PGESysNET(const PGESysNET&); 
    PGESysNET& operator=(const PGESysNET&);

    void SetClientNick(HSteamNetConnection hConn, const char* nick);
    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo);

    friend class PGE;

}; // class PGESysNET