#pragma once

/*
    ###################################################################################
    PgeNetwork.h
    This file is part of PGE.
    External header.
    PGE Network Main class
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
#include "PgeClient.h"
#include "PgePacket.h"
#include "PgeServer.h"

namespace pge_network
{

    /**
        The primary interface to networking functionality.
        Singleton, can be instantiated with static createAndGet().
    */
    class PgeNetwork
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeNetwork is included")   
#endif

    public:
        static PgeNetwork& createAndGet(
            PGEcfgProfiles& cfgProfiles);           /**< Creates and gets the singleton implementation instance. */

        static const char* getLoggerModuleName();   /**< Returns the logger module name of this class. */

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

        virtual bool initialize() = 0;                   /**< Initialize the networking subsystem. */
        virtual bool shutdown() = 0;                     /**< Stops the networking subsystem. */
        virtual bool isInitialized() const = 0;          /**< Gets the state of the networking subsystem. */

        virtual bool isServer() const = 0;               /**< Returns whether the initialized network subsystem should be the server instance. */
        virtual void Update() = 0;                       /**< Handles connection state changes and forwards messages to the application. */

        virtual PgeIServerClient* getServerClientInstance() = 0; /**< Returns access to the initialized networking subsystem. */
        virtual PgeClient& getClient() = 0;                      /**< Returns the client instance. */
        virtual PgeServer& getServer() = 0;                      /**< Returns the server instance. */

        //virtual int getIpAddress() = 0;

        virtual void WriteList() const = 0;              /**< Writes statistics to console. */
    }; // class PgeNetwork

} // namespace pge_network
