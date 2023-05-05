#pragma once

/*
    ###################################################################################
    PgeGsnClient.h
    This file is part of PGE.
    Internal header.
    Client Wrapper for GameNetworkingSockets library
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

/**
    Client Wrapper for GameNetworkingSockets library
*/
class PgeGsnClient final : public PgeGsnWrapper
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeGsnClient is included")   
#endif

public:
    static PgeGsnClient& createAndGet(PGEcfgProfiles& cfgProfiles);   /**< Creates and gets the singleton implementation instance. */

    // ---------------------------------------------------------------------------

    virtual ~PgeGsnClient();

    /* implement stuff from PgeGsnWrapper start */

    /**
    * Calls disconnectClient() and PgeGsnWrapper::destroy().
    *
    * @return True if uninitialization is successful or not initialized, false otherwise.
    */
    virtual bool destroy() override;

    /**
    * Gets the state of initialization of the PgeGsnClient instance i.e. if connected to a GameNetworkingSockets server instance.
    *
    * @return True if initialized, false otherwise.
    */
    virtual bool isInitialized() const override;

    /* implement stuff from PgeGsnWrapper end */

    /**
    * Basically the initialization function for PgeGsnClient.
    * Initializes GameNetworkingSockets client connection to a GameNetworkingSockets server instance.
    *
    * @return True on success, false on failure.
    */
    bool connectToServer(const std::string& sServerAddress); /* temporal, now I dont have better idea in this time */

    /**
    * Destroys client connection.
    * On success, connectToServer() can be invoked again.
    *
    * @return Always true, even if client was not connected before calling this function.
    */
    bool disconnectClient();

    const HSteamNetConnection& getConnectionHandle() const;
    const HSteamNetConnection& getConnectionHandleServerSide() const;
    const char* getServerAddress() const;

    void sendToServer(const pge_network::PgePacket& pkt);

    const SteamNetConnectionRealTimeStatus_t& getRealTimeStatus(bool bForceUpdate);
    std::string getDetailedStatus() const;

protected:
    virtual int receiveMessages(ISteamNetworkingMessage** pIncomingMsg, int nIncomingMsgArraySize) const override;
    virtual bool validateSteamNetworkingMessage(const HSteamNetConnection& connHandle) const override;
    virtual void updateIncomingPgePacket(pge_network::PgePacket& pkt, const HSteamNetConnection& connHandle) const override;
    virtual void onSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* pInfo) override;

private:

    SteamNetworkingIPAddr m_addrServer;
    char m_szAddr[SteamNetworkingIPAddr::k_cchMaxString];
    HSteamNetConnection m_hConnection;

    // TODO: this is not set currently. Because Client code at this level in pollConnectionStateChanges() knows only about its local
    // connection handle. Server should send a specific Pge message back to client holding this info.
    // Even though it is filled as m_connHandleServerSide in most packets, those packets are only sent when APP level sends something.
    // Because currently there is pge_network level packet sent by server to client during connection buildup.
    // I think the MsgUserConnected should contain this info and should be sent to clients by server upon establishing connection.
    // I also think that IP address should be also part of this enhanced MsgUserConnected messages. This is part of APP level MsgUserSetup
    // today.
    HSteamNetConnection m_hConnectionServerSide;

    SteamNetConnectionRealTimeStatus_t m_connRtStatus;

    // ---------------------------------------------------------------------------

    explicit PgeGsnClient(PGEcfgProfiles& cfgProfiles);
    PgeGsnClient(const PgeGsnClient&);
    PgeGsnClient& operator=(const PgeGsnClient&);
}; // class PgeGsnClient