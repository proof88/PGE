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
        The primary interface to client networking functionality.
        Singleton, can be instantiated with static createAndGet().
    */
    class PgeClient : public PgeIServerClient
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeClient is included")   
#endif

    public:
        static PgeClient& createAndGet(
            PGEcfgProfiles& cfgProfiles);           /**< Creates and gets the singleton implementation instance. */

        static const char* getLoggerModuleName();   /**< Returns the logger module name of this class. */

        // ---------------------------------------------------------------------------

        CConsole& getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

        /* all abstract functions from PgeIServerClient are overridden by hidden implementation */

        /**
        * Opens a connection to a server instance.
        * If the function is successful, any call to the derived isInitialized() and isConnected() is expected to return true.
        * Note: you can disconnect from server by invoking the derived disconnect() or shutdown() which are implemented already in hidden class.
        *
        * @return True on success, false otherwise or when it is already connected to server.
        */
        virtual bool connectToServer(const std::string& sServerAddress) = 0; /* temporal */

        /** Returns client's handle to the connection opened towards the server. */
        virtual const pge_network::PgeNetworkConnectionHandle& getConnectionHandle() const = 0;
        
        /** Returns server's handle to the connection opened towards this client. */
        virtual const pge_network::PgeNetworkConnectionHandle& getConnectionHandleServerSide() const = 0;

        virtual const char* getServerAddress() const = 0;

        /* Debug functions. */

        virtual int getPing(bool bForceUpdate) = 0;
        virtual float getQualityLocal(bool bForceUpdate) = 0;
        virtual float getQualityRemote(bool bForceUpdate) = 0;
        virtual float getRxByteRate(bool bForceUpdate) = 0;
        virtual float getTxByteRate(bool bForceUpdate) = 0;
        virtual int64_t getPendingUnreliableBytes(bool bForceUpdate) = 0;
        virtual int64_t getPendingReliableBytes(bool bForceUpdate) = 0;
        virtual int64_t getSentButUnAckedReliableBytes(bool bForceUpdate) = 0;
        virtual int64_t getInternalQueueTimeUSecs(bool bForceUpdate) = 0;
        virtual std::string getDetailedConnectionStatus() const = 0;
    }; // class PgeClient

} // namespace pge_network
