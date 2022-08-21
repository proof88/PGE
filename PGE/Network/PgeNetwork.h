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

#include <map>

#include "../PGEallHeaders.h"
#include "../../../CConsole/CConsole/src/CConsole.h"
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"
#include "PgePacket.h"


struct Player_t
{
    PRREObject3D* pObject3D;
};

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
    static PgeNetwork& createAndGet();          /**< Creates and gets the singleton implementation instance. */

    static const char* getLoggerModuleName();   /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole& getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    virtual bool initialize() = 0;                   /**< Initialize the networking subsystem. */
    virtual bool shutdown() = 0;                     /**< This stops the networking subsystem. */
    virtual bool isInitialized() const = 0;          /**< Gets the state of the networking subsystem. */

    virtual bool isServer() const = 0;
    virtual void Update() = 0;
    virtual bool ConnectClient() = 0; /* temporal */
    virtual void SendStringToClient(const char* str) = 0;
    virtual void SendPacketToClient(const PgePacket& pkt) = 0;
    virtual void SendStringToAllClients(const char* str) = 0;
    virtual void SendPacketToAllClients(const PgePacket& pkt) = 0;
    virtual void SendPacketToServer(const PgePacket& pkt) = 0;
    virtual std::deque<PgePacket>& getPacketQueue() = 0;  // TODO: TEMPORAL: obviously we should not allow this kind of access
    virtual std::map<std::string, Player_t>& getPlayers() = 0;

    virtual void WriteList() const = 0;    /**< Writes statistics to console. */
}; // class PgeNetwork
