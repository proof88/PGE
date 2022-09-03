#pragma once

/*
    ###################################################################################
    PgePacket.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine packets
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PGEallHeaders.h"

// These packet and message structs are sent between server and clients.
// Different endianness is not considered as an issue because all machines expected to use this have same endianness for now.
// In case this changes in the future, use a lib like cereal to easily solve endianness issue.

namespace pge_network
{

    typedef uint32_t PgeNetworkConnectionHandle; // 0 is considered as an invalid handle

    enum class PgePktId : uint32_t
    {
        USER_CONNECTED = 0,
        USER_DISCONNECTED,
        APP
    };

    // server -> self (injection)
    // server app can process this in arbitrary way and might send custom message to all clients about new user
    // server app should not send this to clients; it is blacklisted for clients by default;
    // it is blacklisted also for server app because it should not receive this over network, it just injects it for itself
    struct PgeMsgUserConnected
    {
        static const PgePktId id = PgePktId::USER_CONNECTED;

        bool bCurrentClient;
    };

    // server -> clients and to self
    // it is blacklisted for server app because it should not receive this over network, it just injects it for itself
    struct PgeMsgUserDisconnected
    {
        static const PgePktId id = PgePktId::USER_DISCONNECTED;
    };

    typedef uint32_t TPgeMsgAppMsgId;

    // application-specific message
    // server <-> client
    struct PgeMsgApp
    {
        static const PgePktId id = PgePktId::APP;
        static const uint16_t nMessageMaxLength = 256;

        TPgeMsgAppMsgId msgId;
        char cData[nMessageMaxLength];
    };

    struct PgePacket
    {
        PgePktId pktId;
        PgeNetworkConnectionHandle m_connHandleServerSide;
        union
        {
            PgeMsgUserConnected userConnected;
            PgeMsgUserDisconnected userDisconnected;
            PgeMsgApp app; // application should load/store its custom messages here
        } msg;
    };

} // namespace pge_network
