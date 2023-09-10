#pragma once

/*
    ###################################################################################
    PgeIServerClient.h
    This file is part of PGE.
    External header.
    PGE Network Client and Server Common Interface class
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
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

        virtual void Update() = 0;

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
