#pragma once

/*
    ###################################################################################
    PGEconsts.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine constants
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PGEallHeaders.h"

// server -> clients
struct PgePktUserConnected
{
    static const uint32_t id = 0;
    bool bCurrentClient;
    char sUserName[64];
};

enum class Direction : std::uint8_t
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};

// PgePktUserCmd messages are sent from clients to server, so server will do sg and then update all the clients
// clients -> server
struct PgePktUserCmdMove
{
    static const uint32_t id = 1;
    Direction direction;
};

struct PgePacket
{
    int32_t pktId;
    union
    {
        PgePktUserConnected userConnected;
        PgePktUserCmdMove userCmdMove;
    } msg;
};
