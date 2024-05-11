#pragma once

/*
    ###################################################################################
    PgeClient.h
    This file is part of PGE.
    External header.
    PGE Network Client class
    Made by PR00F88
    ###################################################################################
*/

#include "../../../Console/CConsole/src/CConsole.h"

#include "../PGEallHeaders.h"
#include "../Config/PGEcfgProfiles.h"
#include "PgeIClient.h"
#include "PgeIServerClient.h"
#include "PgePacket.h"

namespace pge_network
{

    /**
        The primary interface to client networking functionality.
        Singleton, can be instantiated with static createAndGet().
    */
    class PgeClient : public PgeIClient
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeClient is included")   
#endif

    public:
        static PgeIClient& createAndGet(
            PGEcfgProfiles& cfgProfiles);           /**< Creates and gets the singleton implementation instance. */

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

        /* all abstract functions from PgeIClient and PgeIServerClient are overridden by hidden implementation */

    }; // class PgeClient

} // namespace pge_network
