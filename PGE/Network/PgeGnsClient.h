#pragma once

/*
    ###################################################################################
    PgeGnsClient.h
    This file is part of PGE.
    Internal header.
    Client Wrapper for GameNetworkingSockets library
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
    Client Wrapper for GameNetworkingSockets library
*/
class PgeGnsClient final : public PgeGnsWrapper
{
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeGnsClient is included")   
#endif

public:
    static PgeGnsClient& createAndGet(PGEcfgProfiles& cfgProfiles);   /**< Creates and gets the singleton implementation instance. */

    // ---------------------------------------------------------------------------

    virtual ~PgeGnsClient();

    /* implement stuff from PgeGnsWrapper start */

    /**
    * Calls disconnectClient() and PgeGnsWrapper::destroy().
    * Normally this should be called only once: exiting the application.
    * You don't need to call this just for disconnecting the client, for that use disconnectClient().
    * 
    * After a call to destroy(), and before you can connect to a server again, you need to reinitialize the GameNetworkingSockets subsystem
    * by calling PgeGnsWrapper::init().
    *
    * @return True if uninitialization is successful or not initialized, false otherwise.
    */
    virtual bool destroy() override;

    /* implement stuff from PgeGnsWrapper end */

    /**
    * Sets up GameNetworkingSockets client connection to a GameNetworkingSockets server instance.
    * First you need to initialize GameNetworkingSockets subsystem by calling PgeGnsWrapper::init(), and only after that
    * you can try establishing connection to a server instance.
    * After a successful call, isConnected() is expected to return true.
    * 
    * @param sServerAddress IPv4 or IPv6 address of GameNetworkingSockets server we want to connect to.
    * @param sAppVersion    Client application version. If server expects a specific client version, we should fill it here.
    *
    * @return True on success, false on failure.
    */
    bool connectToServer(
        const std::string& sServerAddress,
        const std::string& sAppVersion = ""); /* temporal, now I dont have better idea at this time */

    /**
    * Disconnects the client from the server.
    * 
    * If client was connected at time of calling this function, a MsgUserDisconnectedFromServer will be injected into the message queue
    * with the server's connection handle, so at application level the handleUserDisconnected() is expected to be invoked with
    * connHandleServerSide = pge_network::ServerConnHandle, for which the client is expected to remove all other players as well since
    * they are/were also connecting to the same server.
    * 
    * After a successful call, isConnected() is expected to return false, but PgeGnsWrapper::isInitialized() state stays unchanged, so
    * connectToServer() can be invoked again.
    * 
    * @param sExtraDebugText An optional text that will be sent to the other side and will be logged to help debugging.
    *
    * @return Always true, even if client was not connected before calling this function.
    */
    bool disconnectClient(const std::string& sExtraDebugText = "");

    /**
    * Gets if we are logically connected to a server.
    * Basically it always returns true after a successful call to connectToServer(), and returns false after a call to disconnectClient().
    *
    * @return True if connected, false otherwise.
    */
    bool isConnected() const;

    const HSteamNetConnection& getConnectionHandle() const;
    const HSteamNetConnection& getConnectionHandleServerSide() const;
    const char* getServerAddress() const;

    void sendToServer(const pge_network::PgePacket& pkt);

    /* Debug functions. */

    const SteamNetConnectionRealTimeStatus_t& getRealTimeStatus(bool bForceUpdate);
    std::string getDetailedConnectionStatus() const;

protected:
    virtual bool pgeMessageIsHandledAtGnsLevel(const pge_network::PgePacket& pktReceivedFromServer);
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
    // I think the MsgUserConnectedServerSelf should contain this info and should be sent to clients by server upon establishing connection.
    // I also think that IP address should be also part of this enhanced MsgUserConnectedServerSelf messages. This is part of APP level MsgUserSetup
    // today.
    HSteamNetConnection m_hConnectionServerSide;

    SteamNetConnectionRealTimeStatus_t m_connRtStatus;

    // ---------------------------------------------------------------------------

    explicit PgeGnsClient(PGEcfgProfiles& cfgProfiles);
    PgeGnsClient(const PgeGnsClient&);
    PgeGnsClient& operator=(const PgeGnsClient&);
}; // class PgeGnsClient