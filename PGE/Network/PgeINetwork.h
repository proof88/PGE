#pragma once

/*
    ###################################################################################
    PgeINetwork.h
    This file is part of PGE.
    External header.
    PGE Network main interface.
    Made by PR00F88
    ###################################################################################
*/

#include <string>

#include "../PGEallHeaders.h"
#include "PgeIClient.h"
#include "PgeIServer.h"

namespace pge_network
{

    /**
        The primary interface to networking functionality.
    */
    class PgeINetwork
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeINetwork is included")   
#endif

    public:

        static constexpr char* CVAR_NET_SERVER = "net_server";

        /**
            Returns the logger module name of this class.
            Not private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

            @return The logger module name of this class.
        */
        static const char* getLoggerModuleName()
        {
            return "PgeNetwork";
        }

        // ---------------------------------------------------------------------------

        virtual bool initialize() = 0;                   /**< Initialize the networking subsystem. */
        virtual bool shutdown() = 0;                     /**< Stops the networking subsystem. */
        virtual bool isInitialized() const = 0;          /**< Gets the state of the networking subsystem. */
        virtual bool reinitialize() = 0;                 /**< Stop and then initialize the networking subsystem. */
        virtual void disconnect(const std::string& sExtraDebugText) = 0;  /**< Disconnects the initialized networking subsystem. */

        virtual bool isServer() const = 0;               /**< Returns whether the initialized network subsystem should be the server instance. */
        virtual void Update() = 0;                       /**< Handles connection state changes and forwards messages to the application. */

        virtual PgeIServerClient* getServerClientInstance() = 0; /**< Returns access to the initialized networking subsystem. */
        virtual PgeIClient& getClient() = 0;                     /**< Returns the client instance. */
        virtual PgeIServer& getServer() = 0;                     /**< Returns the server instance. */

        //virtual int getIpAddress() = 0;

        virtual void WriteList() const = 0;              /**< Writes statistics to console. */
    }; // class PgeINetwork

} // namespace pge_network
