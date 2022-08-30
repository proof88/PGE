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

// These packet structs are sent between server and clients.
// Different endianness is not considered as an issue because all machines expected to use this have same endianness for now.
// In case this changes in the future, use a lib like cereal to easily solve endianness issue.

// server -> clients and to self
struct PgePktUserConnected
{
    static const uint32_t id = 0;

    bool bCurrentClient;
    char szUserName[64];
    char szTrollfaceTex[64];
};

// server -> clients and to self
struct PgePktUserDisconnected
{
    static const uint32_t id = 1;
    char szUserName[64];
};

// application-specific packet
struct PgePktUserCustom
{
    static const uint32_t id = 2;
    char cData[256];
};

struct PgePacket
{
    uint32_t pktId;
    uint32_t connHandle;
    union
    {
        PgePktUserConnected userConnected;
        PgePktUserDisconnected userDisconnected;
        PgePktUserCustom userCustom;
    } msg;
};
