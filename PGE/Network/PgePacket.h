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
    // server app can process this in arbitrary way and might send custom message to all clients about new user;
    // server app should not send this to clients; it is blacklisted for clients by default;
    // it is blacklisted also for server app because server should not receive this over network, it just injects it for itself
    struct MsgUserConnected
    {
        static const PgePktId id = PgePktId::USER_CONNECTED;
        static const uint8_t nIpAddressMaxLength = 48;

        bool bCurrentClient;
        char szIpAddress[nIpAddressMaxLength];
        // TODO: server always knows IP address of clients, but doesn't know its own IP address used by the clients to connect.
        // It should be grabbed from the first connecting client.
    };

    // server -> clients and to self
    // it is blacklisted for server app because server should not receive this over network, it just injects it for itself
    struct MsgUserDisconnected
    {
        static const PgePktId id = PgePktId::USER_DISCONNECTED;
    };

    typedef uint32_t TPgeMsgAppMsgId;

    // application-specific message
    // server <-> client
    struct MsgApp
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
            MsgUserConnected userConnected;
            MsgUserDisconnected userDisconnected;
            MsgApp app; // application should load/store its custom messages here
        } msg;
    };

    // packet initializers

    bool initPktPgeMsgUserConnected(
        pge_network::PgePacket& pkt,
        const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
        bool bCurrentClient,
        const std::string& sIpAddress);

} // namespace pge_network
