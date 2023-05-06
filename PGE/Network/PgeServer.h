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

        virtual bool startListening() = 0;

        virtual void sendToAllClientsExcept(const pge_network::PgePacket& pkt, const pge_network::PgeNetworkConnectionHandle& exceptConnHandle = 0) = 0;
    }; // class PgeServer

} // namespace pge_network
