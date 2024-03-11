#pragma once

/*
    ###################################################################################
    PgeGnsServer.h
    This file is part of PGE.
    Internal header.
    Server Wrapper for GameNetworkingSockets library
    PR00F's Game Engine networking subsystem
    Made by PR00F88
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

    /* implement stuff from PgeGnsWrapper end */

    /**
    * Starts GameNetworkingSockets server listening to incoming GameNetworkingSockets client connections.
    * First you need to initialize GameNetworkingSockets subsystem by calling PgeGnsWrapper::init(), and only after that
    * you can try start listening.
    * After a successful call, isListening() is expected to return true.
    *
    * @return True on success, false on failure.
    */
    bool startListening();

    /**
    * Destroys all connections to clients and stops listening.
    * On success, startListening() can be invoked again.
    * After a successful call, isListening() is expected to return false, but PgeGnsWrapper::isInitialized() state stays unchanged.
    * 
    * @param sExtraDebugText An optional text that will be sent to the other side and will be logged to help debugging.
    *
    * @return Always true, even if listening was not even initiated before calling this function.
    */
    bool stopListening(const std::string& sExtraDebugText = "");

    /**
    * Gets if we are listening to incoming client connections.
    * Basically it always returns true after a successful call to startListening(), and returns false after a call to stopListening().
    *
    * @return True if listening, false otherwise.
    */
    bool isListening() const;

    void sendToClient(const HSteamNetConnection& conn, const pge_network::PgePacket& pkt);
    void sendToAllClientsExcept(const pge_network::PgePacket& pkt, const HSteamNetConnection& except = k_HSteamNetConnection_Invalid);

    void inject(const pge_network::PgePacket& pkt);

    void WriteServerClientList();

    /* Debug functions. */

    void setClientDebugName(HSteamNetConnection connHandle, const char* nick);

    const SteamNetConnectionRealTimeStatus_t& getRealTimeStatus(const HSteamNetConnection& connHandle, bool bForceUpdate);
    std::string getDetailedConnectionStatus(const HSteamNetConnection& connHandle) const;

protected:
    virtual int receiveMessages(ISteamNetworkingMessage** pIncomingMsg, int nIncomingMsgArraySize) const override;
    virtual bool validateSteamNetworkingMessage(const HSteamNetConnection& connHandle) const override;
    virtual void updateIncomingPgePacket(pge_network::PgePacket& pkt, const HSteamNetConnection& connHandle) const override;
    virtual void onSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) override;

private:

    struct TClient
    {
        std::string m_sCustomName;  /**< App level can set a custom name for client which is useful for debugging. */
        char m_szAddr[SteamNetworkingIPAddr::k_cchMaxString];
        SteamNetConnectionRealTimeStatus_t m_connRtStatus;
    };

    typedef std::map< HSteamNetConnection, TClient > SteamNetConnHandle2TClientMap;

    uint16 m_nPort;

    HSteamListenSocket m_hListenSock;
    HSteamNetPollGroup m_hPollGroup;

    SteamNetConnHandle2TClientMap m_mapClients;
    // The connection handle is used to identify clients in the map.
    // This handle for a client is NOT the same as the handle on the client side, obviously.
    // Note that the first connection is this map is an invalid connection (k_HSteamNetConnection_Invalid), which
    // is the server itself, this is how this layer stores nickname for the server (self).

    // ---------------------------------------------------------------------------

    explicit PgeGnsServer(PGEcfgProfiles& cfgProfiles);
    PgeGnsServer(const PgeGnsServer&);
    PgeGnsServer& operator=(const PgeGnsServer&);

    const TClient* isClientConnectionHandleValid(const HSteamNetConnection& connHandle) const;
    TClient* isClientConnectionHandleValid(const HSteamNetConnection& connHandle);

}; // class PgeGnsServer