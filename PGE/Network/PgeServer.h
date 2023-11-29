#pragma once

/*
    ###################################################################################
    PgeServer.h
    This file is part of PGE.
    External header.
    PGE Network Server class
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
#include "PgeIServerClient.h"
#include "PgePacket.h"

namespace pge_network
{

    /**
        The primary interface to server networking functionality.
        Singleton, can be instantiated with static createAndGet().
    */
    class PgeServer : public PgeIServerClient
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeServer is included")   
#endif

    public:
        static PgeServer& createAndGet(
            PGEcfgProfiles& cfgProfiles);           /**< Creates and gets the singleton implementation instance. */

        static const char* getLoggerModuleName();   /**< Returns the logger module name of this class. */

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

        /* all abstract functions from PgeIServerClient are overrid by hidden implementation */

        /**
        * Starts listening to incoming client connections.
        * If the function is successful, any call to the derived isListening() is expected to return true.
        * Note: you can stop listening by invoking the derived disconnect() or shutdown() which are implemented already in hidden class.
        * 
        * @return True on success, false otherwise or when it is already listening.
        */
        virtual bool startListening() = 0;

        /**
        * Sends the given packet to all client instances except the optionally specified client.
        * This function is never able to send to server, not even injecting, hence the name contains "Clients".
        * 
        * @param pkt              The packet to be sent.
        * @param exceptConnHandle The server-side connection of handle of a client to which we DO NOT want to send the packet.
        *                         Since this function never sends to server, we use pge_network::ServerConnHandle as default, in such case
        *                         the packet is sent to all clients.
        *                         Even if you specify a real server-side connection handle of a client, this packet will never be
        *                         sent to server, that is why function name contains "Clients".
        */
        virtual void sendToAllClientsExcept(
            const pge_network::PgePacket& pkt,
            const pge_network::PgeNetworkConnectionHandle& exceptConnHandle = pge_network::ServerConnHandle) = 0;

        /**
        * Sends the given packet to all known network instances, including the server itself (self).
        * Basically equals to:
        *   send(pkt);
        *   sendToAllClientsExcept(pkt);
        *
        * @param pkt The packet to be sent.
        */
        virtual void sendToAll(const pge_network::PgePacket& pkt) = 0;

        /* Debug functions. */

        virtual int getPing(
            const pge_network::PgeNetworkConnectionHandle& connHandle,
            bool bForceUpdate) = 0;
        virtual float getQualityLocal(
            const pge_network::PgeNetworkConnectionHandle& connHandle,
            bool bForceUpdate) = 0;
        virtual float getQualityRemote(
            const pge_network::PgeNetworkConnectionHandle& connHandle,
            bool bForceUpdate) = 0;
        virtual float getRxByteRate(
            const pge_network::PgeNetworkConnectionHandle& connHandle,
            bool bForceUpdate) = 0;
        virtual float getTxByteRate(
            const pge_network::PgeNetworkConnectionHandle& connHandle,
            bool bForceUpdate) = 0;
        virtual int64_t getPendingUnreliableBytes(
            const pge_network::PgeNetworkConnectionHandle& connHandle,
            bool bForceUpdate) = 0;
        virtual int64_t getPendingReliableBytes(
            const pge_network::PgeNetworkConnectionHandle& connHandle,
            bool bForceUpdate) = 0;
        virtual int64_t getSentButUnAckedReliableBytes(
            const pge_network::PgeNetworkConnectionHandle& connHandle,
            bool bForceUpdate) = 0;
        virtual int64_t getInternalQueueTimeUSecs(
            const pge_network::PgeNetworkConnectionHandle& connHandle,
            bool bForceUpdate) = 0;
        virtual std::string getDetailedConnectionStatus(
            const pge_network::PgeNetworkConnectionHandle& connHandle) const = 0;
    }; // class PgeServer

} // namespace pge_network
