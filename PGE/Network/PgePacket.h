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

    typedef uint32_t PgeNetworkConnectionHandle;

    // since 0 is considered as an invalid handle at GSN level, we use this to refer to server, since
    // server never wants to send to this connection, or it automatically injects when this value is used.
    constexpr PgeNetworkConnectionHandle ServerConnHandle = 0;

    enum class PgePktId : uint32_t
    {
        USER_CONNECTED = 0,
        USER_DISCONNECTED,
        APP
    };

    // server -> self (injection)
    // server app can process this in arbitrary way and might send another custom message to all clients about new user;
    // server app should not send this to clients; it is NOT allowlisted for clients by default;
    // it is NOT allowlisted either for server app because server should not receive this over network, it just injects it for itself
    struct MsgUserConnected
    {
        static const PgePktId id = PgePktId::USER_CONNECTED;
        static const uint8_t nIpAddressMaxLength = 48;

        bool bCurrentClient;
        char szIpAddress[nIpAddressMaxLength];
        // TODO: server always knows IP address of clients, but doesn't know its own IP address used by the clients to connect.
        // It should be grabbed from the first connecting client.
        // See if remote server address logged is the good server address in: PgeGsnWrapper::onSteamNetConnectionStatusChanged(),
        // client code, when client reaches k_ESteamNetworkingConnectionState_Connected. Jump there by searching for "KEKEKEKEKE"
    };

    // server -> clients and to self
    // it is NOT allowlisted for server app because server should not receive this over network, it just injects it for itself
    struct MsgUserDisconnected
    {
        static const PgePktId id = PgePktId::USER_DISCONNECTED;
    };

    typedef uint32_t TPgeMsgAppMsgId;

    struct MsgAppArea
    {
        static const uint16_t nMessagesAreaLength = 256;

        uint8_t m_nMessageCount;
        /* This 'cData' memory area is for m_nMessageCount number of different app messages.
           The C++ standard guarantees that the members of a class or struct appear in memory in the same order as they are declared.
           This 'cData' member needs to be the LAST member of this struct.
           When sending, we should send the actually used memory area and not the whole area.
           We don't need to store the actually used size since GSN will tell the actual pkt size anyway on polling messages.
           In 'cData' we store at least 1 variable-sized MsgApp.
         */
        uint8_t cData[nMessagesAreaLength];
    };

    // application-specific message
    // server <-> client
    // With allow listing, app messages can be separately allowed to be processed by clients and server based on TPgeMsgAppMsgId.
    struct MsgApp
    {
        static const PgePktId id = PgePktId::APP;
        static const uint16_t nMaxMessageLength = 250;
        
        TPgeMsgAppMsgId msgId;  // this is checked by engine upon polling for new messages against the allowlists
        uint8_t nMsgSize;
        /* This 'cMsgData' memory area is for 1 application message defined at application level, not here.
           So the application is responsible for copying the message here.
           The C++ standard guarantees that the members of a class or struct appear in memory in the same order as they are declared.
           This 'cMsgData' member needs to be the LAST member of this struct.
           When packaging MsgApp into MsgAppArea, we should put the actually used memory area (nMsgSize) and not the whole area, and
           increase MsgAppArea::m_nMessageCount by 1.
         */
        uint8_t cMsgData[nMaxMessageLength];
    };
    static_assert(
        sizeof(MsgApp) <= MsgAppArea::nMessagesAreaLength,
        "At least 1 MsgApp should fit into MsgAppArea.cData!");

    struct PgePacket
    {
        PgePktId pktId;
        PgeNetworkConnectionHandle m_connHandleServerSide;

        union
        {
            MsgUserConnected userConnected;
            MsgUserDisconnected userDisconnected;
            MsgAppArea app; // application should load/store its custom messages here
        } msg;
    };

    // packet initializers

    bool initPktPgeMsgUserConnected(
        pge_network::PgePacket& pkt,
        const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
        bool bCurrentClient,
        const std::string& sIpAddress);

} // namespace pge_network
