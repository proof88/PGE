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

namespace pge_network {

    PgePktId& pge_network::PgePacket::getPacketId(PgePacket& pkt)
    {
        return pkt.pktId;
    }

    const PgePktId& pge_network::PgePacket::getPacketId(const PgePacket& pkt)
    {
        return pkt.pktId;
    }

    PgeNetworkConnectionHandle& pge_network::PgePacket::getServerSideConnectionHandle(PgePacket& pkt)
    {
        return pkt.m_connHandleServerSide;
    }

    const PgeNetworkConnectionHandle& pge_network::PgePacket::getServerSideConnectionHandle(const PgePacket& pkt)
    {
        return pkt.m_connHandleServerSide;
    }

    MsgUserConnected& PgePacket::getMessageAsUserConnected(PgePacket& pkt)
    {
        return pkt.msg.userConnected;
    }

    const MsgUserConnected& PgePacket::getMessageAsUserConnected(const PgePacket& pkt)
    {
        return pkt.msg.userConnected;
    }

    MsgUserDisconnected& PgePacket::getMessageAsUserDisconnected(PgePacket& pkt)
    {
        return pkt.msg.userDisconnected;
    }

    const MsgUserDisconnected& PgePacket::getMessageAsUserDisconnected(const PgePacket& pkt)
    {
        return pkt.msg.userDisconnected;
    }

    MsgAppArea& pge_network::PgePacket::getMessageAppArea(PgePacket& pkt)
    {
        return pkt.msg.app;
    }

    const MsgAppArea& pge_network::PgePacket::getMessageAppArea(const PgePacket& pkt)
    {
        return pkt.msg.app;
    }

    uint8_t pge_network::PgePacket::getMessageAppCount(const pge_network::PgePacket& pkt)
    {
        return getMessageAppArea(pkt).m_nMessageCount;
    }

    uint8_t pge_network::PgePacket::getMessageAppsTotalActualLength(const pge_network::PgePacket& pkt)
    {
        return getMessageAppArea(pkt).m_nActualMessagesAreaLength;
    }

    void pge_network::PgePacket::initPktBasic(
        pge_network::PgePacket& pkt,
        const PgePktId& pktId,
        const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
        const pge_network::PgePacket::AutoFill& autoFill)
    {
        if (autoFill == pge_network::PgePacket::AutoFill::ZERO)
        {
            memset(&pkt, 0, sizeof(pkt));
        }
        pge_network::PgePacket::getPacketId(pkt) = pktId;
        pge_network::PgePacket::getServerSideConnectionHandle(pkt) = connHandleServerSide;
    }

    bool pge_network::PgePacket::initPktPgeMsgUserConnected(
        PgePacket& pkt,
        const PgeNetworkConnectionHandle& connHandleServerSide,
        bool bCurrentClient,
        const std::string& sIpAddress)
    {
        if (sIpAddress.length() >= sizeof(PgePacket::getMessageAsUserConnected(pkt).szIpAddress))
        {
            return false;
        }
        pge_network::PgePacket::initPktBasic(pkt, MsgUserConnected::id, connHandleServerSide, pge_network::PgePacket::AutoFill::ZERO);

        MsgUserConnected& msgUserConnected = PgePacket::getMessageAsUserConnected(pkt);
        msgUserConnected.bCurrentClient = bCurrentClient;

        /*
           I explicitly zero the whole pkt area above because I want clean pkt.
           However, the debug versions of some security-enhanced CRT functions fill the buffer passed to them with
           a special character (0xFE):
           https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/crtsetdebugfillthreshold?view=msvc-170

           This can turn it off: _CrtSetDebugFillThreshold( 0 );
           And I'm not turning this back on because I don't want to use this debug feature.
         */
        _CrtSetDebugFillThreshold(0);
        return (strncpy_s(
            msgUserConnected.szIpAddress,
            sizeof(msgUserConnected.szIpAddress),
            sIpAddress.c_str(),
            sIpAddress.length()) == 0);
    }

    bool PgePacket::isMessageAppAreaFull(const pge_network::PgePacket& pkt)
    {
        return pge_network::PgePacket::getMessageAppArea(pkt).m_nActualMessagesAreaLength == pge_network::MsgAppArea::nMessagesAreaLength;
    }

    void pge_network::PgePacket::initPktPgeMsgUserDisconnected(
        PgePacket& pkt,
        const PgeNetworkConnectionHandle& connHandleServerSide)
    {
        pge_network::PgePacket::initPktBasic(pkt, MsgUserDisconnected::id, connHandleServerSide, pge_network::PgePacket::AutoFill::ZERO);
    }

    void pge_network::PgePacket::initPktMsgApp(
        pge_network::PgePacket& pkt,
        const pge_network::PgeNetworkConnectionHandle& connHandleServerSide)
    {
        pge_network::PgePacket::initPktBasic(pkt, MsgApp::id, connHandleServerSide, pge_network::PgePacket::AutoFill::ZERO);
    }

    bool PgePacket::addPktMsgApp(
        pge_network::PgePacket& pkt,
        const pge_network::MsgApp& msgApp)
    {
        if (pge_network::PgePacket::getPacketId(pkt) != MsgApp::id)
        {
            // this packet is initialized for non-MsgApp messages
            return false;
        }

        const uint8_t nActualTotalAppMsgSize = pge_network::MsgApp::getMsgAppTotalActualSize(msgApp);
        if (pge_network::PgePacket::getMessageAppArea(pkt).m_nActualMessagesAreaLength + nActualTotalAppMsgSize > pge_network::MsgAppArea::nMessagesAreaLength)
        {
            // not enough space in this packet to store the given app msg
            return false;
        }

        // TODO: find location for next app msg (separate function)
        // TODO: store the given app msg at the proper location

        memcpy(pge_network::PgePacket::getMessageAppArea(pkt).cData, &msgApp, nActualTotalAppMsgSize);
        pge_network::PgePacket::getMessageAppArea(pkt).m_nMessageCount++;
        pge_network::PgePacket::getMessageAppArea(pkt).m_nActualMessagesAreaLength += nActualTotalAppMsgSize;

        return true;
    }

    TPgeMsgAppMsgId& MsgApp::getMsgAppMsgId(MsgApp& msgApp)
    {
        return msgApp.msgId;
    }

    uint8_t& MsgApp::getMsgAppDataActualSize(MsgApp& msgApp)
    {
        return msgApp.nMsgSize;
    }

    const uint8_t& MsgApp::getMsgAppDataActualSize(const MsgApp& msgApp)
    {
        return msgApp.nMsgSize;
    }

    const uint8_t MsgApp::getMsgAppTotalActualSize(const MsgApp& msgApp)
    {
        return offsetof(MsgApp, cMsgData) + getMsgAppDataActualSize(msgApp);
    }

    uint8_t* MsgApp::getMsgAppData(MsgApp& msgApp)
    {
        return msgApp.cMsgData;
    }

    bool MsgApp::fillMsgApp(
        pge_network::MsgApp& myAppMsg,
        const pge_network::TPgeMsgAppMsgId& msgAppMsgId,
        const uint8_t* msgAppData,
        uint8_t nMsgAppDataSize)
    {
        if (nMsgAppDataSize > pge_network::MsgApp::nMaxMessageLength)
        {
            // given data cannot fit into target struct
            return false;
        }

        pge_network::MsgApp::getMsgAppMsgId(myAppMsg) = msgAppMsgId;
        memcpy_s(
            pge_network::MsgApp::getMsgAppData(myAppMsg),
            pge_network::MsgApp::nMaxMessageLength,
            static_cast<const void*>(msgAppData),
            nMsgAppDataSize);
        pge_network::MsgApp::getMsgAppDataActualSize(myAppMsg) = nMsgAppDataSize;

        return true;
    }

}  // namespace pge_network