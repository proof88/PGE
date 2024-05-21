#pragma once

/*
    ###################################################################################
    PgeIClient.h
    This file is part of PGE.
    External header.
    PGE Network Client interface
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
        The primary interface to client networking functionality.
    */
    class PgeIClient : public PgeIServerClient
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeIClient is included")   
#endif

    public:

        /**
            Returns the logger module name of this class.
            Not private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

            @return The logger module name of this class.
        */
        static const char* getLoggerModuleName()
        {
            return "PgeClient";
        }

        // ---------------------------------------------------------------------------

        /**
        * Opens a connection to a server instance.
        * If the function is successful, any call to the derived isInitialized() and isConnected() is expected to return true.
        * Note: you can disconnect from server by invoking the derived disconnect() or shutdown() which are implemented already in hidden class.
        * 
        * @param sServerAddress IPv4 or IPv6 address of PgeServer we want to connect to.
        * @param sAppVersion    Client application version. If server expects a specific client version, we should fill it here.
        *
        * @return True on success, false otherwise or when it is already connected to server.
        *         Note that mismatch between the specified client application version and server application version will be found out later and
        *         it does NOT have effect on the return value: the server will disconnect the mismatching client a bit later.
        */
        virtual bool connectToServer(
            const std::string& sServerAddress,
            const std::string& sAppVersion = "") = 0; /* temporal */

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
    }; // class PgeIClient

} // namespace pge_network
