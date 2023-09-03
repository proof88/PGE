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

#include <stddef.h>  // offsetof()

#include <limits>

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
        UserConnectedServerSelf = 0,
        UserDisconnectedFromServer,
        Application
    };

    // server -> self (injection)
    // server app can process this in arbitrary way and might send another custom message to all clients about new user;
    // server app should not send this to clients; it is NOT allowlisted for clients by default;
    // it is NOT allowlisted either for server app because server should not receive this over network, it just injects it for itself
    struct MsgUserConnected
    {
        static const PgePktId id = PgePktId::UserConnectedServerSelf;
        static const uint8_t nIpAddressMaxLength = 48;

        bool bCurrentClient;
        char szIpAddress[nIpAddressMaxLength];
        // TODO: server always knows IP address of clients, but doesn't know its own IP address used by the clients to connect.
        // It should be grabbed from the first connecting client.
        // See if remote server address logged is the good server address in: PgeGnsWrapper::onSteamNetConnectionStatusChanged(),
        // client code, when client reaches k_ESteamNetworkingConnectionState_Connected. Jump there by searching for "KEKEKEKEKE"
    };

    // server -> clients and to self
    // it is NOT allowlisted for server app because server should not receive this over network, it just injects it for itself
    struct MsgUserDisconnected
    {
        static const PgePktId id = PgePktId::UserDisconnectedFromServer;
    };

    typedef uint32_t TPgeMsgAppMsgId;

    // application-specific message
    // server <-> client
    // With allow listing, app messages can be separately allowed to be processed by clients and server based on TPgeMsgAppMsgId.
    struct MsgApp
    {
        static const PgePktId id = PgePktId::Application;
        static const uint8_t nMaxMessageLength = 240;

        static TPgeMsgAppMsgId& getMsgAppMsgId(MsgApp& msgApp);
        static uint8_t&  getMsgAppDataActualSize(MsgApp& msgApp);  // TODO: delete this non-const version later when not needed
        static const uint8_t& getMsgAppDataActualSize(const MsgApp& msgApp);
        static const uint8_t getMsgAppTotalActualSize(const MsgApp& msgApp);
        static uint8_t* getMsgAppData(MsgApp& msgApp);
        static bool fillMsgApp(
            pge_network::MsgApp& myAppMsg,
            const pge_network::TPgeMsgAppMsgId& msgAppMsgId,
            const uint8_t* msgAppData,
            uint8_t nMsgAppDataSize);
        
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

    static_assert(MsgApp::nMaxMessageLength <= 255u /*std::numeric_limits<uint8_t>::max()  TODO: #define NOMINMAX so using windows headers wont define min and max macros! */,
        "Size of MsgApp data should fit in MsgApp::nMsgSize");

    // memory area within a PgePacket, used when we are sending app message(s) in the packet
    struct MsgAppArea
    {
        static const uint16_t nMessagesAreaLength = 256;

        uint8_t m_nMessageCount;
        uint8_t m_nActualMessagesAreaLength;
        /* This 'cData' memory area is for m_nMessageCount number of different app messages.
           The C++ standard guarantees that the members of a class or struct appear in memory in the same order as they are declared.
           This 'cData' member needs to be the LAST member of this struct.
           When sending, we should send the actually used memory area and not the whole area.
           We don't need to store the actually used size since GSN will tell the actual pkt size anyway on polling messages.
           In 'cData' we store at least 1 variable-sized MsgApp.
         */
        uint8_t cData[nMessagesAreaLength];
    };
    
    static_assert(
        sizeof(MsgApp) <= MsgAppArea::nMessagesAreaLength,
        "At least 1 full MsgApp should fit into MsgAppArea.cData!");

    struct PgePacket
    {
        enum class AutoFill {
            NONE,
            ZERO
        };

        // basic packet get/set

        static PgePktId& getPacketId(pge_network::PgePacket& pkt);
        static const PgePktId& getPacketId(const pge_network::PgePacket& pkt);

        static PgeNetworkConnectionHandle& getServerSideConnectionHandle(pge_network::PgePacket& pkt);
        static const PgeNetworkConnectionHandle& getServerSideConnectionHandle(const pge_network::PgePacket& pkt);

        static MsgUserConnected& getMessageAsUserConnected(pge_network::PgePacket& pkt);
        static const MsgUserConnected& getMessageAsUserConnected(const pge_network::PgePacket& pkt);

        static MsgUserDisconnected& getMessageAsUserDisconnected(pge_network::PgePacket& pkt);
        static const MsgUserDisconnected& getMessageAsUserDisconnected(const pge_network::PgePacket& pkt);

        static MsgAppArea& getMessageAppArea(pge_network::PgePacket& pkt);
        static const MsgAppArea& getMessageAppArea(const pge_network::PgePacket& pkt);

        static uint8_t getMessageAppCount(const pge_network::PgePacket& pkt);
        static uint8_t getMessageAppsTotalActualLength(const pge_network::PgePacket& pkt);

        static bool isMessageAppAreaFull(const pge_network::PgePacket& pkt);

        // packet initializers

        static void initPktBasic(
            pge_network::PgePacket& pkt,
            const PgePktId& pktId,
            const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
            const AutoFill& autoFill);

        static bool initPktPgeMsgUserConnected(
            pge_network::PgePacket& pkt,
            const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
            bool bCurrentClient,
            const std::string& sIpAddress);

        static void initPktPgeMsgUserDisconnected(
            PgePacket& pkt,
            const PgeNetworkConnectionHandle& connHandleServerSide);

        static void initPktMsgApp(
            pge_network::PgePacket& pkt,
            const pge_network::PgeNetworkConnectionHandle& connHandleServerSide);

        static bool addPktMsgApp(
            pge_network::PgePacket& pkt,
            const pge_network::MsgApp& msgApp);

        PgePktId pktId;
        PgeNetworkConnectionHandle m_connHandleServerSide;

        union
        {
            MsgUserConnected userConnected;
            MsgUserDisconnected userDisconnected;
            MsgAppArea app; // application should load/store its custom messages here
        } msg;
    };

} // namespace pge_network
