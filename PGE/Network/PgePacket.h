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

#include "PGEallHeaders.h"

// These packet structs are sent between server and clients.
// Different endianness is not considered as an issue because all machines expected to use this have same endianness for now.
// In case this changes in the future, use a lib like cereal to easily solve endianness issue.

// server -> clients and to self
struct PgePktUserConnected
{
    static const uint32_t id = 0;
    bool bCurrentClient;
    char sUserName[64];
    char sTrollfaceTex[64];
};

// server -> clients and to self
struct PgePktUserDisconnected
{
    static const uint32_t id = 1;
    char sUserName[64];
};

enum class VerticalDirection : std::uint8_t
{
    NONE = 0,
    UP,
    DOWN
};

enum class HorizontalDirection : std::uint8_t
{
    NONE = 0,
    LEFT,
    RIGHT
};

// PgePktUserCmd messages are sent from clients to server, so server will do sg and then update all the clients
// clients -> server
struct PgePktUserCmdMove
{
    static const uint32_t id = 2;
    char sUserName[64];  // obviously char array is overkill for identifying user but good for now ... server sets it, not the sender (client)
    HorizontalDirection horDirection;
    VerticalDirection verDirection;
};

// server -> clients
struct PgePktUserUpdate
{
    static const uint32_t id = 3;
    char sUserName[64];
    TXYZ pos;
};

struct PgePacket
{
    int32_t pktId;
    union
    {
        PgePktUserConnected userConnected;
        PgePktUserDisconnected userDisconnected;
        PgePktUserCmdMove userCmdMove;
        PgePktUserUpdate userUpdate;
    } msg;
};
