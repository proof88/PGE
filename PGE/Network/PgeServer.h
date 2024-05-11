#pragma once

/*
    ###################################################################################
    PgeServer.h
    This file is part of PGE.
    External header.
    PGE Network Server class
    Made by PR00F88
    ###################################################################################
*/

#include "../../../Console/CConsole/src/CConsole.h"

#include "../PGEallHeaders.h"
#include "../Config/PGEcfgProfiles.h"
#include "PgeIServer.h"
#include "PgeIServerClient.h"
#include "PgePacket.h"

namespace pge_network
{

    /**
        The primary interface to server networking functionality.
        Singleton, can be instantiated with static createAndGet().
    */
    class PgeServer : public PgeIServer
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeServer is included")   
#endif

    public:
        static PgeIServer& createAndGet(
            PGEcfgProfiles& cfgProfiles);           /**< Creates and gets the singleton implementation instance. */

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

        /* all abstract functions from PgeIServer and PgeIServerClient are overrid by hidden implementation */

    }; // class PgeServer

} // namespace pge_network
