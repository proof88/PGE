/*
    ###################################################################################
    PgePacket.cpp
    This file is part of PGE.
    PR00F's Game Engine packets
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgePacket.h"

bool pge_network::initPktPgeMsgUserConnected(
    pge_network::PgePacket& pkt,
    const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
    bool bCurrentClient,
    const std::string& sIpAddress)
{
    memset(&pkt, 0, sizeof(pkt));
    pkt.m_connHandleServerSide = connHandleServerSide;
    pkt.pktId = pge_network::MsgUserConnected::id;
    pkt.msg.userConnected.bCurrentClient = bCurrentClient;
    strncpy_s(pkt.msg.userConnected.szIpAddress, sizeof(pkt.msg.userConnected.szIpAddress), sIpAddress.c_str(), sIpAddress.length());

    return true;
}