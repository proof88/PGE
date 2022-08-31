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

namespace PgePkt
{

    enum class PgePktId : uint32_t
    {
        USER_CONNECTED = 0,
        USER_DISCONNECTED,
        APP
    };

    // server -> clients and to self
    struct PgeMsgUserConnected
    {
        static const PgePktId id = PgePktId::USER_CONNECTED;

        bool bCurrentClient;
        char szUserName[64];      // TODO: move to app
        char szTrollfaceTex[64];  // TODO: move to app
    };

    // server -> clients and to self
    struct PgeMsgUserDisconnected
    {
        static const PgePktId id = PgePktId::USER_DISCONNECTED;

        char szUserName[64]; // TODO: move to app
    };

    typedef uint32_t TPgeMsgAppMsgId;

    // application-specific message
    struct PgeMsgApp
    {
        static const PgePktId id = PgePktId::APP;

        TPgeMsgAppMsgId msgId;
        char cData[256];
    };

    struct PgePacket
    {
        PgePktId pktId;
        uint32_t connHandle;
        union
        {
            PgeMsgUserConnected userConnected;
            PgeMsgUserDisconnected userDisconnected;
            PgeMsgApp app; // application should load/store its custom messages here
        } msg;
    };

} // namespace PgePkt
