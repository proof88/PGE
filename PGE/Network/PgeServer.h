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
#include "../Config/PGEcfgProfiles.h"
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
        static PgeServer& createAndGet(
            PGEcfgProfiles& cfgProfiles);           /**< Creates and gets the singleton implementation instance. */

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
        virtual void InjectPacket(const pge_network::PgePacket& pkt) = 0;
        virtual std::size_t getPacketQueueSize() const = 0;
        virtual pge_network::PgePacket popFrontPacket() noexcept(false) = 0;

        virtual std::set<pge_network::PgePktId>& getBlackListedPgeMessages() = 0;
        virtual std::set<pge_network::TPgeMsgAppMsgId>& getBlackListedAppMessages() = 0;

        virtual uint32_t getRxPacketCount() const = 0;
        virtual uint32_t getTxPacketCount() const = 0;
        virtual uint32_t getInjectPacketCount() const = 0;

        virtual uint32_t getRxPacketPerSecondCount() const = 0;
        virtual uint32_t getTxPacketPerSecondCount() const = 0;
        virtual uint32_t getInjectPacketPerSecondCount() const = 0;

        virtual void WriteList() const = 0;    /**< Writes statistics to console. */
    }; // class PgeServer

} // namespace pge_network
