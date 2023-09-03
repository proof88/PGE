#pragma once

/*
    ###################################################################################
    PgeGnsServer.h
    This file is part of PGE.
    Internal header.
    Server Wrapper for GameNetworkingSockets library
    PR00F's Game Engine networking subsystem
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PGEallHeaders.h"

#include <chrono>  // requires cpp11
#include <cstdint>
#include <deque>
#include <map>
#include <string>

#include "../Config/PGEcfgProfiles.h"
#include "PgePacket.h"
#include "PgeGnsWrapper.h"

struct TClient
{
    std::string m_sCustomName;  /**< App level can set a custom name for client which is useful for debugging. */
    char m_szAddr[SteamNetworkingIPAddr::k_cchMaxString];
};

/**
    Server Wrapper for GameNetworkingSockets library
*/
class PgeGnsServer final : public PgeGnsWrapper
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeGnsServer is included")   
#endif

public:
    static PgeGnsServer& createAndGet(PGEcfgProfiles& cfgProfiles);   /**< Creates and gets the singleton implementation instance. */

    // ---------------------------------------------------------------------------

    virtual ~PgeGnsServer();

    /* implement stuff from PgeGnsWrapper start */

    /**
    * Calls stopListening() and PgeGnsWrapper::destroy().
    *
    * @return True if uninitialization is successful or not initialized, false otherwise.
    */
    virtual bool destroy() override;

    /**
    * Gets the state of initialization of the PgeGnsServer instance i.e. if listening to incoming client connections.
    *
    * @return True if initialized, false otherwise.
    */
    virtual bool isInitialized() const override;

    /* implement stuff from PgeGnsWrapper end */

    /**
    * Basically the initialization function for PgeGnsServer.
    * Initializes GameNetworkingSockets server listening.
    *
    * @return True on success, false on failure.
    */
    bool startListening();

    /**
    * Destroys all connections to clients and stops listening.
    * On success, startListening() can be invoked again.
    *
    * @return Always true, even if listening was not even initiated before calling this function.
    */
    bool stopListening();

    void sendToClient(const HSteamNetConnection& conn, const pge_network::PgePacket& pkt);
    void sendToAllClientsExcept(const pge_network::PgePacket& pkt, const HSteamNetConnection& except = k_HSteamNetConnection_Invalid);

    void inject(const pge_network::PgePacket& pkt);

    void WriteServerClientList();

protected:
    virtual int receiveMessages(ISteamNetworkingMessage** pIncomingMsg, int nIncomingMsgArraySize) const override;
    virtual bool validateSteamNetworkingMessage(const HSteamNetConnection& connHandle) const override;
    virtual void updateIncomingPgePacket(pge_network::PgePacket& pkt, const HSteamNetConnection& connHandle) const override;
    virtual void onSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) override;

private:

    uint16 m_nPort;

    HSteamListenSocket m_hListenSock;
    HSteamNetPollGroup m_hPollGroup;

    std::map< HSteamNetConnection, TClient > m_mapClients;
    // The connection handle is used to identify clients in the map.
    // This handle for a client is NOT the same as the handle on the client side, obviously.
    // Note that the first connection is this map is an invalid connection (k_HSteamNetConnection_Invalid), which
    // is the server itself, this is how this layer stores nickname for the server (self).

    // ---------------------------------------------------------------------------

    explicit PgeGnsServer(PGEcfgProfiles& cfgProfiles);
    PgeGnsServer(const PgeGnsServer&);
    PgeGnsServer& operator=(const PgeGnsServer&);

    void SetClientNick(HSteamNetConnection hConn, const char* nick);
}; // class PgeGnsServer