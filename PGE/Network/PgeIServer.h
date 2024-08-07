#pragma once

/*
    ###################################################################################
    PgeIServer.h
    This file is part of PGE.
    External header.
    PGE Network Server interface
    Made by PR00F88
    ###################################################################################
*/

#include <string>

#include "../PGEallHeaders.h"
#include "PgeIServerClient.h"
#include "PgePacket.h"

namespace pge_network
{

    /**
        The primary interface to server networking functionality.
    */
    class PgeIServer : public PgeIServerClient
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeIServer is included")   
#endif

    public:

        /**
            Returns the logger module name of this class.
            Not private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

            @return The logger module name of this class.
        */
        static const char* getLoggerModuleName()
        {
            return "PgeServer";
        }

        /**
        * Starts listening to incoming PgeClient connections.
        * If the function is successful, any call to the derived isListening() is expected to return true.
        * Note: you can stop listening by invoking the derived disconnect() or shutdown() which are implemented already in hidden class.
        *
        * @param sAppVersion Server application version. We should fill it in if we expect connecting clients to have this same application version.
        * 
        * @return True on success, false otherwise or when it is already listening.
        */
        virtual bool startListening(const std::string& sAppVersion = "") = 0;

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

        /**
        * Sets a debug helper name associated to the given connection handle.
        * Useful because it makes reading debug log easier when not only connection handle but also this debug helper name is also logged.
        * Can be anything, for example it can be the same name as the player name on higher application level associated to this connection.
        *
        * @param connHandle The connection for which we want to set the debug helper name.
        *                   Cannot be ServerConnHandle.
        * @param sNickname The debug helper name we want to see associated to the specified connection in the debug logs.
        */
        virtual void setDebugNickname(const pge_network::PgeNetworkConnectionHandle& connHandle, const std::string& sNickname) = 0;

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
    }; // class PgeIServer

} // namespace pge_network
