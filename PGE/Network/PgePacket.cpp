/*
    ###################################################################################
    PgePacket.cpp
    This file is part of PGE.
    PR00F's Game Engine packets
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "PgePacket.h"

#include <cassert>

namespace pge_network {

    PgePktId& pge_network::PgePacket::getPacketId(PgePacket& pkt)
    {
        return pkt.m_pktId;
    }

    const PgePktId& pge_network::PgePacket::getPacketId(const PgePacket& pkt)
    {
        return pkt.m_pktId;
    }

    PgeNetworkConnectionHandle& pge_network::PgePacket::getServerSideConnectionHandle(PgePacket& pkt)
    {
        return pkt.m_connHandleServerSide;
    }

    const PgeNetworkConnectionHandle& pge_network::PgePacket::getServerSideConnectionHandle(const PgePacket& pkt)
    {
        return pkt.m_connHandleServerSide;
    }

    MsgUserConnectedServerSelf& PgePacket::getMessageAsUserConnected(PgePacket& pkt)
    {
        return pkt.m_msg.m_userConnected;
    }

    const MsgUserConnectedServerSelf& PgePacket::getMessageAsUserConnected(const PgePacket& pkt)
    {
        return pkt.m_msg.m_userConnected;
    }

    MsgUserDisconnectedFromServer& PgePacket::getMessageAsUserDisconnected(PgePacket& pkt)
    {
        return pkt.m_msg.m_userDisconnected;
    }

    const MsgUserDisconnectedFromServer& PgePacket::getMessageAsUserDisconnected(const PgePacket& pkt)
    {
        return pkt.m_msg.m_userDisconnected;
    }

    MsgClientAppVersionFromClient& PgePacket::getMessageAsClientAppVersionFromClient(PgePacket& pkt)
    {
        return pkt.m_msg.m_clientAppVersionFromClient;
    }

    const MsgClientAppVersionFromClient& PgePacket::getMessageAsClientAppVersionFromClient(const PgePacket& pkt)
    {
        return pkt.m_msg.m_clientAppVersionFromClient;
    }

    MsgAppArea& pge_network::PgePacket::getMessageAppArea(PgePacket& pkt)
    {
        return pkt.m_msg.m_app;
    }

    const MsgAppArea& pge_network::PgePacket::getMessageAppArea(const PgePacket& pkt)
    {
        return pkt.m_msg.m_app;
    }

    const uint8_t& pge_network::PgePacket::getMessageAppCount(const pge_network::PgePacket& pkt)
    {
        return getMessageAppArea(pkt).m_nMessageCount;
    }

    const MsgAppArea::TAreaLength& pge_network::PgePacket::getMessageAppsTotalActualLengthBytes(const pge_network::PgePacket& pkt)
    {
        return getMessageAppArea(pkt).m_nActualMessagesAreaLength;
    }

    uint8_t& pge_network::PgePacket::getMessageAppCount(pge_network::PgePacket& pkt)
    {
        return getMessageAppArea(pkt).m_nMessageCount;
    }

    MsgAppArea::TAreaLength& pge_network::PgePacket::getMessageAppsTotalActualLengthBytes(pge_network::PgePacket& pkt)
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
        if (sIpAddress.length() >= sizeof(PgePacket::getMessageAsUserConnected(pkt).m_szIpAddress))
        {
            return false;
        }
        pge_network::PgePacket::initPktBasic(pkt, MsgUserConnectedServerSelf::id, connHandleServerSide, pge_network::PgePacket::AutoFill::ZERO);

        MsgUserConnectedServerSelf& msgUserConnected = PgePacket::getMessageAsUserConnected(pkt);
        msgUserConnected.m_bCurrentClient = bCurrentClient;

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
            msgUserConnected.m_szIpAddress,
            sizeof(msgUserConnected.m_szIpAddress),
            sIpAddress.c_str(),
            sIpAddress.length()) == 0);
    }

    bool PgePacket::isMessageAppAreaFull(const pge_network::PgePacket& pkt)
    {
        return pge_network::PgePacket::getMessageAppArea(pkt).m_nActualMessagesAreaLength == pge_network::MsgAppArea::nMaxMessagesAreaLengthBytes;
    }

    const pge_network::MsgApp* PgePacket::getMsgAppFromPkt(const pge_network::PgePacket& pkt)
    {
        const pge_network::MsgApp* const pMsgApp = reinterpret_cast<const pge_network::MsgApp*>(pkt.m_msg.m_app.m_cData);
        return pMsgApp;
    }

    pge_network::MsgApp* PgePacket::getMsgAppFromPkt(pge_network::PgePacket& pkt)
    {
        // simply invoke the const-version of getMsgAppDataFromPkt() above by const-casting:
        return const_cast<pge_network::MsgApp*>(getMsgAppFromPkt(const_cast<const pge_network::PgePacket&>(pkt)));
    }

    const pge_network::MsgApp::TMsgId& PgePacket::getMsgAppIdFromPkt(const pge_network::PgePacket& pkt)
    {
        return pge_network::MsgApp::getMsgAppMsgId(*pge_network::PgePacket::getMsgAppFromPkt(pkt));
    }

    void pge_network::PgePacket::initPktPgeMsgUserDisconnected(
        PgePacket& pkt,
        const PgeNetworkConnectionHandle& connHandleServerSide)
    {
        pge_network::PgePacket::initPktBasic(pkt, MsgUserDisconnectedFromServer::id, connHandleServerSide, pge_network::PgePacket::AutoFill::ZERO);
    }

    bool pge_network::PgePacket::initPktPgeMsgClientAppVersionFromClient(
        pge_network::PgePacket& pkt,
        const std::string& sClientAppVersion)
    {
        if (sClientAppVersion.length() >= sizeof(PgePacket::getMessageAsClientAppVersionFromClient(pkt).m_szAppVersion))
        {
            return false;
        }
        pge_network::PgePacket::initPktBasic(pkt, MsgClientAppVersionFromClient::id, 0 /*connHandle not used here */, pge_network::PgePacket::AutoFill::ZERO);

        MsgClientAppVersionFromClient& msgClientAppVersion = PgePacket::getMessageAsClientAppVersionFromClient(pkt);

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
            msgClientAppVersion.m_szAppVersion,
            sizeof(msgClientAppVersion.m_szAppVersion),
            sClientAppVersion.c_str(),
            sClientAppVersion.length()) == 0);
    }

    void pge_network::PgePacket::initPktMsgApp(
        pge_network::PgePacket& pkt,
        const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
        const AutoFill& autoFill)
    {
        pge_network::PgePacket::initPktBasic(pkt, MsgApp::id, connHandleServerSide, autoFill);
        
        // need to zero these members cause autoFill might be set to NONE
        pge_network::PgePacket::getMessageAppArea(pkt).m_nMessageCount = 0;
        pge_network::PgePacket::getMessageAppArea(pkt).m_nActualMessagesAreaLength = 0;
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

        // we receive msgApp structure from outside, dont take the validity of its content granted!
        if (MsgApp::getMsgAppDataActualSizeBytes(msgApp) > MsgApp::nMaxMessageLengthBytes)
        {
            // given nMsgAppDataSize indicates too big app msg data
            return false;
        }

        /* use uint32_t instead of uint8_t or MsgApp::TMsgSize here to avoid numeric overflow */
        const uint32_t nActualTotalAppMsgSize = offsetof(pge_network::MsgApp, m_cMsgData) + MsgApp::getMsgAppDataActualSizeBytes(msgApp);
        assert(nActualTotalAppMsgSize <= std::numeric_limits<MsgApp::TMsgSize>::max());
        if (nActualTotalAppMsgSize > std::numeric_limits<MsgApp::TMsgSize>::max())
        {
            // given nMsgAppDataSize indicates too big app msg data
            return false;
        }

        if (pge_network::PgePacket::getMessageAppArea(pkt).m_nActualMessagesAreaLength + nActualTotalAppMsgSize > pge_network::MsgAppArea::nMaxMessagesAreaLengthBytes)
        {
            // not enough space in this packet to store the given app msg
            return false;
        }

        if (pge_network::PgePacket::getMessageAppArea(pkt).m_nMessageCount == std::numeric_limits<uint8_t>::max())
        {
            // already reached max app msg count
            return false;
        }

        // TODO: find location for next app msg (separate function)
        // TODO: store the given app msg at the proper location

        memcpy(pge_network::PgePacket::getMessageAppArea(pkt).m_cData, &msgApp, nActualTotalAppMsgSize);
        pge_network::PgePacket::getMessageAppArea(pkt).m_nMessageCount++;
        pge_network::PgePacket::getMessageAppArea(pkt).m_nActualMessagesAreaLength += static_cast<MsgAppArea::TAreaLength>(nActualTotalAppMsgSize);

        return true;
    }

    TByte* pge_network::PgePacket::preparePktMsgAppFill(
        pge_network::PgePacket& pkt,
        MsgApp::TMsgId msgAppId,
        MsgApp::TMsgSize nMsgAppDataSize
    )
    {
        if (pge_network::PgePacket::getPacketId(pkt) != MsgApp::id)
        {
            // this packet is initialized for non-MsgApp messages
            return nullptr;
        }

        if (nMsgAppDataSize > MsgApp::nMaxMessageLengthBytes)
        {
            // given nMsgAppDataSize indicates too big app msg data
            return nullptr;
        }

        /* use uint32_t instead of uint8_t or MsgApp::TMsgSize here to avoid numeric overflow */
        const uint32_t nActualTotalAppMsgSize = offsetof(pge_network::MsgApp, m_cMsgData) + nMsgAppDataSize;
        assert(nActualTotalAppMsgSize <= std::numeric_limits<MsgApp::TMsgSize>::max());
        if (nActualTotalAppMsgSize > std::numeric_limits<MsgApp::TMsgSize>::max())
        {
            // given nMsgAppDataSize indicates too big app msg data
            return nullptr;
        }

        if (pge_network::PgePacket::getMessageAppArea(pkt).m_nActualMessagesAreaLength + nActualTotalAppMsgSize > pge_network::MsgAppArea::nMaxMessagesAreaLengthBytes)
        {
            // not enough space in this packet to store the given app msg
            return nullptr;
        }

        if (pge_network::PgePacket::getMessageAppArea(pkt).m_nMessageCount == std::numeric_limits<uint8_t>::max())
        {
            // already reached max app msg count
            return nullptr;
        }

        pge_network::PgePacket::getMessageAppArea(pkt).m_nMessageCount = 1; // TODO: increase
        pge_network::PgePacket::getMessageAppArea(pkt).m_nActualMessagesAreaLength += static_cast<MsgAppArea::TAreaLength>(nActualTotalAppMsgSize);

        // TODO: find location for next app msg (separate function)
        // TODO: store the given app msg at the proper location

        pge_network::MsgApp* pMsgApp = reinterpret_cast<pge_network::MsgApp*>(pge_network::PgePacket::getMessageAppArea(pkt).m_cData);
        pge_network::MsgApp::getMsgAppMsgId(*pMsgApp) = static_cast<pge_network::MsgApp::TMsgId>(msgAppId);
        pge_network::MsgApp::getMsgAppDataActualSizeBytes(*pMsgApp) = nMsgAppDataSize;

        return pge_network::MsgApp::getMsgAppData(*pMsgApp);
    }

    const MsgApp::TMsgId& MsgApp::getMsgAppMsgId(const MsgApp& msgApp)
    {
        return msgApp.m_msgId;
    }

    MsgApp::TMsgId& MsgApp::getMsgAppMsgId(MsgApp& msgApp)
    {
        return msgApp.m_msgId;
    }

    MsgApp::TMsgSize& MsgApp::getMsgAppDataActualSizeBytes(MsgApp& msgApp)
    {
        return msgApp.m_nMsgSize;
    }

    const MsgApp::TMsgSize& MsgApp::getMsgAppDataActualSizeBytes(const MsgApp& msgApp)
    {
        return msgApp.m_nMsgSize;
    }

    const MsgApp::TMsgSize MsgApp::getMsgAppTotalActualSizeBytes(const MsgApp& msgApp)
    {
        return offsetof(MsgApp, m_cMsgData) + getMsgAppDataActualSizeBytes(msgApp);
    }

    const TByte* MsgApp::getMsgAppData(const MsgApp& msgApp)
    {
        return msgApp.m_cMsgData;
    }

    TByte* MsgApp::getMsgAppData(MsgApp& msgApp)
    {
        return msgApp.m_cMsgData;
    }

    bool MsgApp::fillMsgApp(
        pge_network::MsgApp& myAppMsg,
        const pge_network::MsgApp::TMsgId& msgAppMsgId,
        const TByte* msgAppData,
        MsgApp::TMsgSize nMsgAppDataSize)
    {
        if (nMsgAppDataSize > pge_network::MsgApp::nMaxMessageLengthBytes)
        {
            // given data cannot fit into target struct
            return false;
        }

        pge_network::MsgApp::getMsgAppMsgId(myAppMsg) = msgAppMsgId;
        memcpy_s(
            pge_network::MsgApp::getMsgAppData(myAppMsg),
            pge_network::MsgApp::nMaxMessageLengthBytes,
            static_cast<const void*>(msgAppData),
            nMsgAppDataSize);
        pge_network::MsgApp::getMsgAppDataActualSizeBytes(myAppMsg) = nMsgAppDataSize;

        return true;
    }

}  // namespace pge_network