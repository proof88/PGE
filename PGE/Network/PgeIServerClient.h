#pragma once

/*
    ###################################################################################
    PgeIServerClient.h
    This file is part of PGE.
    External header.
    PGE Network Client and Server Common Interface class
    Made by PR00F88
    ###################################################################################
*/

#include <cstdint>
#include <deque>
#include <map>
#include <set>
#include <string>

#include "../../../Console/CConsole/src/CConsole.h"

#include "../PGEallHeaders.h"
#include "../Config/PGEcfgProfiles.h"
#include "PgePacket.h"

namespace pge_network
{

    /**
        PGE Network Client and Server Common Interface class.
        Let the application be either client or server, it is supposed to access functionality through this common interface.
    */
    class PgeIServerClient
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeIServerClient is included")   
#endif

    public:
        virtual bool initialize() = 0;                   /**< Initialize the network instance. */
        virtual bool shutdown() = 0;                     /**< This stops the network instance. */
        virtual bool isInitialized() const = 0;          /**< Gets the state of the network instance. */

        /**
            Terminates the active connection of the network instance.

            In case of server instance, this terminates the active connections of the server instance and stops listening.
            It also sends as many MsgUserDisconnectedFromServer to all clients as the number of clients, so all clients
            will be notified about all other clients disconnecting.
            A MsgUserDisconnectedFromServer will be injected into the message queue with the server's connection handle, so at
            application level the handleUserDisconnected() is expected to be invoked with connHandleServerSide = pge_network::ServerConnHandle.

            In case of client instance, this disconnects the client from the server.
            If client was connected at time of calling this function, a MsgUserDisconnectedFromServer will be injected into the message queue
            with the server's connection handle, so at application level the handleUserDisconnected() is expected to be invoked with
            connHandleServerSide = pge_network::ServerConnHandle, for which the client is expected to remove all other players as well since
            they are/were also connecting to the same server and not available anymore from this client's perspective.

            @param sExtraDebugText An optional text that will be sent to the other side and will be logged to help debugging.
        */
        virtual void disconnect(const std::string& sExtraDebugText = "") = 0;

        virtual void Update() = 0;

        /**
        * Moves incoming packets from the underlying network layer to PGE network layer as PgePackets.
        *
        * @return True on success, false on error.
        */
        virtual bool pollIncomingMessages() = 0;

        virtual void pollConnectionStateChanges() = 0;

        virtual std::size_t getPacketQueueSize() const = 0;
        virtual pge_network::PgePacket popFrontPacket() noexcept(false) = 0;

        virtual std::set<pge_network::PgePktId>& getAllowListedPgeMessages() = 0;
        virtual std::set<pge_network::MsgApp::TMsgId>& getAllowListedAppMessages() = 0;

        /**
        * Sends the given packet to the network instance specified.
        * 
        * @param pkt        The packet to be sent.
        * @param connHandle The addressed network instance, that can be either a server or client instance.
        *                   The default parameter means the addressed instance is the server instance.
        *                   If server is specified and current instance is the server, the packet
        *                   will be injected to self instead of actually sending it to the lower network level.
        */
        virtual void send(const pge_network::PgePacket& pkt, const pge_network::PgeNetworkConnectionHandle& connHandle = pge_network::ServerConnHandle) = 0;

        virtual uint32_t getRxPacketCount() const = 0;
        virtual uint32_t getTxPacketCount() const = 0;
        virtual uint32_t getInjectPacketCount() const = 0;

        virtual uint32_t getRxPacketPerSecondCount() const = 0;
        virtual uint32_t getTxPacketPerSecondCount() const = 0;
        virtual uint32_t getInjectPacketPerSecondCount() const = 0;

        virtual const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getRxMsgCount() const = 0;
        virtual const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getTxMsgCount() const = 0;
        virtual const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getInjectMsgCount() const = 0;

        virtual std::map<pge_network::MsgApp::TMsgId, std::string>& getMsgAppId2StringMap() = 0;

        virtual uint32_t getRxByteCount() const = 0;
        virtual uint32_t getTxByteCount() const = 0;
        virtual uint32_t getInjectByteCount() const = 0;

        virtual void WriteList() const = 0;    /**< Writes statistics to console. */
    }; // class PgeIServerClient

} // namespace pge_network
