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

#include "../../../CConsole/CConsole/src/CConsole.h"

#include "../PGEallHeaders.h"
#include "PgePacket.h"

namespace pge_network
{

    /**
        The primary interface to server networking functionality.
        Singleton, can be instantiated with static createAndGet().
    */
    class PgeServer
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeServer is included")   
#endif

    public:
        static PgeServer& createAndGet();           /**< Creates and gets the singleton implementation instance. */

        static const char* getLoggerModuleName();   /**< Returns the logger module name of this class. */

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

        virtual bool initialize() = 0;                   /**< Initialize the server subsystem. */
        virtual bool shutdown() = 0;                     /**< This stops the server subsystem. */
        virtual bool isInitialized() const = 0;          /**< Gets the state of the server subsystem. */

        virtual void Update() = 0;
        virtual bool startListening() = 0;
        virtual void SendPacketToClient(pge_network::PgeNetworkConnectionHandle connHandle, const pge_network::PgePacket& pkt) = 0;
        virtual void SendPacketToAllClients(const pge_network::PgePacket& pkt, pge_network::PgeNetworkConnectionHandle exceptConnHandle = 0) = 0;
        virtual std::deque<pge_network::PgePacket>& getPacketQueue() = 0;  // TODO: TEMPORAL: obviously we should not allow this kind of access
        virtual std::set<pge_network::TPgeMsgAppMsgId>& getBlackListedMessages() = 0;

        virtual void WriteList() const = 0;    /**< Writes statistics to console. */
    }; // class PgeServer

} // namespace pge_network
