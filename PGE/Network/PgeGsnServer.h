#pragma once

/*
    ###################################################################################
    PgeGsnServer.h
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
#include "PgeGsnWrapper.h"

struct TClient
{
    std::string m_sCustomName;  /**< App level can set a custom name for client which is useful for debugging. */
    char m_szAddr[SteamNetworkingIPAddr::k_cchMaxString];
};

/**
    Server Wrapper for GameNetworkingSockets library
*/
class PgeGsnServer final : public PgeGsnWrapper
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeGsnServer is included")   
#endif

public:
    static PgeGsnServer& createAndGet(PGEcfgProfiles& cfgProfiles);   /**< Creates and gets the singleton implementation instance. */

    // ---------------------------------------------------------------------------

    virtual ~PgeGsnServer();

    /* implement stuff from PgeGsnWrapper start */

    /**
    * Calls stopListening() and PgeGsnWrapper::destroy().
    *
    * @return True if uninitialization is successful or not initialized, false otherwise.
    */
    virtual bool destroy() override;

    /**
    * Gets the state of initialization of the PgeGsnServer instance i.e. if listening to incoming client connections.
    *
    * @return True if initialized, false otherwise.
    */
    virtual bool isInitialized() const override;

    /* implement stuff from PgeGsnWrapper end */

    /**
    * Basically the initialization function for PgeGsnServer.
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

    void sendToClient(HSteamNetConnection conn, const pge_network::PgePacket& pkt);
    void sendToAllClients(const pge_network::PgePacket& pkt, HSteamNetConnection except = k_HSteamNetConnection_Invalid);

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

    explicit PgeGsnServer(PGEcfgProfiles& cfgProfiles);
    PgeGsnServer(const PgeGsnServer&);
    PgeGsnServer& operator=(const PgeGsnServer&);

    void SetClientNick(HSteamNetConnection hConn, const char* nick);
}; // class PgeGsnServer