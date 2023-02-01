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

#include "../../../CConsole/CConsole/src/CConsole.h"

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
        virtual bool shutdown() = 0;                     /**< This stops the networking subsystem. */
        virtual bool isInitialized() const = 0;          /**< Gets the state of the networking subsystem. */

        virtual bool isServer() const = 0;
        virtual void Update() = 0;

        virtual PgeClient& getClient() = 0;
        virtual PgeServer& getServer() = 0;

        //virtual int getIpAddress() = 0;

        virtual void WriteList() const = 0;    /**< Writes statistics to console. */
    }; // class PgeNetwork

} // namespace pge_network
