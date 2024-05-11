#pragma once

/*
    ###################################################################################
    PgeNetwork.h
    This file is part of PGE.
    External header.
    PGE Network Main class
    Made by PR00F88
    ###################################################################################
*/

#include <string>

#include "../../../Console/CConsole/src/CConsole.h"

#include "../PGEallHeaders.h"
#include "../Config/PGEcfgProfiles.h"
#include "PgeINetwork.h"
#include "PgePacket.h"

namespace pge_network
{

    /**
        The primary interface to networking functionality.
        Singleton, can be instantiated with static createAndGet().
    */
    class PgeNetwork : public PgeINetwork
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeNetwork is included")   
#endif

    public:

        static PgeINetwork& createAndGet(
            PGEcfgProfiles& cfgProfiles);           /**< Creates and gets the singleton implementation instance. */

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

        /* all abstract functions from PgeINetwork are overrid by hidden implementation */

    }; // class PgeNetwork

} // namespace pge_network
