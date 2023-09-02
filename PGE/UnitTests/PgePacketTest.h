#pragma once

/*
    ###################################################################################
    PgePacketTest.h
    Unit test for PgePacket.
    Made by PR00F88, West Whiskhyll Entertainment
    2023
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH

#include "../Network/PgePacket.h"

class PgePacketTest :
    public UnitTest
{
public:

    PgePacketTest() :
        UnitTest(__FILE__)/*,
        cfgProfiles("")*/
    {
        //engine = NULL;
    }

protected:

    virtual void Initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(Bullet::getLoggerModuleName(), true);
        //
        //PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        //
        //engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        //engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems

        AddSubTest("test_initPktBasic_ZeroFill", (PFNUNITSUBTEST)&PgePacketTest::test_initPktBasic_ZeroFill);
        AddSubTest("test_initPktBasic_NoFill", (PFNUNITSUBTEST)&PgePacketTest::test_initPktBasic_NoFill);
        AddSubTest("test_initPktPgeMsgUserConnected_Good", (PFNUNITSUBTEST)&PgePacketTest::test_initPktPgeMsgUserConnected_Good);
        AddSubTest("test_initPktPgeMsgUserConnected_Bad", (PFNUNITSUBTEST)&PgePacketTest::test_initPktPgeMsgUserConnected_Bad);
        AddSubTest("test_initPktPgeMsgUserDisconnected", (PFNUNITSUBTEST)&PgePacketTest::test_initPktPgeMsgUserDisconnected);
        AddSubTest("test_initPktMsgApp", (PFNUNITSUBTEST)&PgePacketTest::test_initPktMsgApp);
        AddSubTest("test_fillMsgApp", (PFNUNITSUBTEST)&PgePacketTest::test_fillMsgApp);
        AddSubTest("test_addMsgApp", (PFNUNITSUBTEST)&PgePacketTest::test_addMsgApp);
        //AddSubTest("test_iterateAllMsgApp", (PFNUNITSUBTEST)&PgePacketTest::test_iterateAllMsgApp);
    }

    //virtual bool setUp() override
    //{
    //    return assertTrue(engine && engine->isInitialized());
    //}

    virtual void TearDown() override
    {
    }

    virtual void Finalize() override
    {
        //if (engine)
        //{
        //    engine->shutdown();
        //    engine = NULL;
        //}
        //
        //CConsole::getConsoleInstance().SetLoggingState(Bullet::getLoggerModuleName(), false);
    }

private:

    //PR00FsUltimateRenderingEngine* engine;
    //PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PgePacketTest(const PgePacketTest&)/* :
        cfgProfiles("")*/
    {};

    PgePacketTest& operator=(const PgePacketTest&)
    {
        return *this;
    };

    bool test_initPktBasic_ZeroFill()
    {
        constexpr pge_network::PgePktId pktId = static_cast<pge_network::PgePktId>(15);
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 5;

        pge_network::PgePacket pkt;
        pge_network::PgePacket::initPktBasic(pkt, pktId, connHandle, pge_network::PgePacket::AutoFill::ZERO);

        pge_network::PgePacket pktJustAFullZeroMemoryAreaOfPgePacketSize;
        memset(&pktJustAFullZeroMemoryAreaOfPgePacketSize, 0, sizeof(pktJustAFullZeroMemoryAreaOfPgePacketSize));
        constexpr size_t nBytesShouldBeZero = sizeof(pkt) - (offsetof(pge_network::PgePacket, msg));
        
        return 
            assertEquals(static_cast<uint32_t>(pktId), static_cast<uint32_t>(pge_network::PgePacket::getPacketId(pkt)), "pktId 1") &
            assertEquals(connHandle, pge_network::PgePacket::getServerSideConnectionHandle(pkt), "connHandle 1") &
            assertEquals(
                0,
                memcmp(
                    static_cast<void*>(&pkt.msg),
                    static_cast<void*>(&pktJustAFullZeroMemoryAreaOfPgePacketSize),
                    nBytesShouldBeZero),
                "remaining area is clean 1");
    }

    bool test_initPktBasic_NoFill()
    {
        constexpr pge_network::PgePktId pktId = static_cast<pge_network::PgePktId>(15);
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 5;

        pge_network::PgePacket pkt;
        pge_network::PgePacket::getMessageAsUserConnected(pkt).szIpAddress[0] = 'A';
        pge_network::PgePacket::initPktBasic(pkt, pktId, connHandle, pge_network::PgePacket::AutoFill::NONE);

        return
            assertEquals(static_cast<uint32_t>(pktId), static_cast<uint32_t>(pge_network::PgePacket::getPacketId(pkt)), "pktId 1") &
            assertEquals(connHandle, pge_network::PgePacket::getServerSideConnectionHandle(pkt), "connHandle 1") &
            assertEquals('A', pge_network::PgePacket::getMessageAsUserConnected(pkt).szIpAddress[0], "not zeroed out");
    }

    bool test_initPktPgeMsgUserConnected_Good()
    {
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 5;
        constexpr char szIp[] = "192.168.1.3";

        pge_network::PgePacket pkt;
        
        // good case
        bool b = assertTrue(pge_network::PgePacket::initPktPgeMsgUserConnected(pkt, connHandle, true, szIp), "ret 1");

        pge_network::PgePacket pktJustAFullZeroMemoryAreaOfPgePacketSize;
        memset(&pktJustAFullZeroMemoryAreaOfPgePacketSize, 0, sizeof(pktJustAFullZeroMemoryAreaOfPgePacketSize));
        constexpr size_t nBytesShouldBeZero = sizeof(pkt) - (offsetof(pge_network::PgePacket, msg.userConnected.szIpAddress) + sizeof(szIp));

        b = b &
            assertEquals(static_cast<uint32_t>(pge_network::MsgUserConnected::id), static_cast<uint32_t>(pge_network::PgePacket::getPacketId(pkt)), "pktId 1") &
            assertEquals(connHandle, pge_network::PgePacket::getServerSideConnectionHandle(pkt), "connHandle 1") &
            assertTrue(pge_network::PgePacket::getMessageAsUserConnected(pkt).bCurrentClient, "currentClient 1") &
            assertEquals(
                0,
                strncmp(pge_network::PgePacket::getMessageAsUserConnected(pkt).szIpAddress, szIp, sizeof(szIp)-1/*terminating nullchar*/),
                "IP 1") &
            assertEquals(
                0,
                memcmp(
                    static_cast<void*>(pge_network::PgePacket::getMessageAsUserConnected(pkt).szIpAddress + sizeof(szIp)-1/*terminating nullchar*/),
                    static_cast<void*>(&pktJustAFullZeroMemoryAreaOfPgePacketSize),
                    nBytesShouldBeZero),
                "remaining area is clean 1");

        return b;
    }

    bool test_initPktPgeMsgUserConnected_Bad()
    {
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 5;
        constexpr char szIp[] = "192.168.1.3";

        pge_network::PgePacket pkt;

        // string too long case
        char szTooLongIp[pge_network::MsgUserConnected::nIpAddressMaxLength + 2];
        memset(szTooLongIp, 'A', sizeof(szTooLongIp) - 1);
        szTooLongIp[sizeof(szTooLongIp) - 1] = '\0';
        return assertFalse(pge_network::PgePacket::initPktPgeMsgUserConnected(pkt, connHandle, true, szTooLongIp), "ret 2");
    }

    bool test_initPktPgeMsgUserDisconnected()
    {
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 5;

        pge_network::PgePacket pkt;

        pge_network::PgePacket::initPktPgeMsgUserDisconnected(pkt, connHandle);

        pge_network::PgePacket pktJustAFullZeroMemoryAreaOfPgePacketSize;
        memset(&pktJustAFullZeroMemoryAreaOfPgePacketSize, 0, sizeof(pktJustAFullZeroMemoryAreaOfPgePacketSize));
        constexpr size_t nBytesShouldBeZero = sizeof(pkt) - (offsetof(pge_network::PgePacket, msg.userDisconnected));

        return
            assertEquals(static_cast<uint32_t>(pge_network::MsgUserDisconnected::id), static_cast<uint32_t>(pge_network::PgePacket::getPacketId(pkt)), "pktId 1") &
            assertEquals(connHandle, pge_network::PgePacket::getServerSideConnectionHandle(pkt), "connHandle 1") &
            assertEquals(
                0,
                memcmp(
                    static_cast<void*>(&pge_network::PgePacket::getMessageAsUserDisconnected(pkt)),
                    static_cast<void*>(&pktJustAFullZeroMemoryAreaOfPgePacketSize),
                    nBytesShouldBeZero),
                "remaining area is clean 1");
    }

    bool test_initPktMsgApp()
    {
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 5;

        pge_network::PgePacket pkt;
        pge_network::PgePacket::initPktMsgApp(pkt, connHandle);

        pge_network::PgePacket pktJustAFullZeroMemoryAreaOfPgePacketSize;
        memset(&pktJustAFullZeroMemoryAreaOfPgePacketSize, 0, sizeof(pktJustAFullZeroMemoryAreaOfPgePacketSize));
        constexpr size_t nBytesShouldBeZero = sizeof(pkt) - (offsetof(pge_network::PgePacket, msg.app));

        return
            assertEquals(static_cast<uint32_t>(pge_network::MsgApp::id), static_cast<uint32_t>(pge_network::PgePacket::getPacketId(pkt)), "pktId 1") &
            assertEquals(connHandle, pge_network::PgePacket::getServerSideConnectionHandle(pkt), "connHandle 1") &
            assertEquals(0u, static_cast<uint32_t>(pge_network::PgePacket::getMessageAppCount(pkt)), "msg app count 1") &
            assertEquals(
                0,
                memcmp(
                    static_cast<void*>(&pge_network::PgePacket::getMessageAppArea(pkt)),
                    static_cast<void*>(&pktJustAFullZeroMemoryAreaOfPgePacketSize),
                    nBytesShouldBeZero),
                "remaining area is clean 1");
    }

    bool test_fillMsgApp()
    {
        constexpr pge_network::TPgeMsgAppMsgId msgAppMsgId = 23u;
        constexpr char msgAppMsgData[] = "almafa";

        pge_network::MsgApp myAppMsg;
        bool b = assertTrue(
            pge_network::MsgApp::fillMsgApp(
                myAppMsg,
                msgAppMsgId,
                reinterpret_cast<const uint8_t*>(msgAppMsgData),
                sizeof(msgAppMsgData)),
            "fill msg app 1");

        return b &
            assertEquals(msgAppMsgId, pge_network::MsgApp::getMsgAppMsgId(myAppMsg), "msg app id") &
            assertEquals(sizeof(msgAppMsgData), pge_network::MsgApp::getMsgAppDataActualSize(myAppMsg), "msg app data actual size") &
            assertEquals(
                offsetof(pge_network::MsgApp, cMsgData) + pge_network::MsgApp::getMsgAppDataActualSize(myAppMsg),
                pge_network::MsgApp::getMsgAppTotalActualSize(myAppMsg),
                "msg app total actual size") &
            assertEquals(
                0,
                memcmp(
                    static_cast<const void*>(pge_network::MsgApp::getMsgAppData(myAppMsg)),
                    static_cast<const void*>(msgAppMsgData),
                    sizeof(msgAppMsgData)),
                "msg app data ok");
    }

    bool test_addMsgApp()
    {
        constexpr pge_network::PgeNetworkConnectionHandle connHandle = 5u;
        constexpr pge_network::TPgeMsgAppMsgId msgAppMsgId = 23u;
        constexpr char msgAppMsgData[] = "almafa";

        pge_network::PgePacket pkt;
        pge_network::PgePacket::initPktMsgApp(pkt, connHandle);

        pge_network::MsgApp myAppMsg;
        bool b = assertTrue(
            pge_network::MsgApp::fillMsgApp(
                myAppMsg,
                msgAppMsgId,
                reinterpret_cast<const uint8_t*>(msgAppMsgData),
                sizeof(msgAppMsgData)),
            "fill msg app 1");

        b &= assertTrue(pge_network::PgePacket::addPktMsgApp(pkt, myAppMsg), "add msg app 1");

        pge_network::PgePacket pktJustAFullZeroMemoryAreaOfPgePacketSize;
        memset(&pktJustAFullZeroMemoryAreaOfPgePacketSize, 0, sizeof(pktJustAFullZeroMemoryAreaOfPgePacketSize));
        const size_t nOffsetOfMsgAppDataEndInPkt = (offsetof(pge_network::PgePacket, msg.app) +
            offsetof(pge_network::MsgAppArea, cData) +
            offsetof(pge_network::MsgApp, cMsgData) +
            pge_network::MsgApp::getMsgAppDataActualSize(myAppMsg));
        const size_t nBytesShouldBeZero = sizeof(pkt) - nOffsetOfMsgAppDataEndInPkt;           

        return b &
            assertEquals(static_cast<uint32_t>(pge_network::MsgApp::id), static_cast<uint32_t>(pge_network::PgePacket::getPacketId(pkt)), "pktId 1") &
            assertEquals(connHandle, pge_network::PgePacket::getServerSideConnectionHandle(pkt), "connHandle 1") &
            assertEquals(1u, static_cast<uint32_t>(pge_network::PgePacket::getMessageAppCount(pkt)), "msg app count 1") &
            assertEquals(
                pge_network::MsgApp::getMsgAppTotalActualSize(myAppMsg),
                pge_network::PgePacket::getMessageAppsTotalActualLength(pkt),
                "msg apps total length should be same now as the actual size of the only 1 msg app") &
            assertEquals(
                0,
                memcmp(
                    static_cast<void*>(pge_network::PgePacket::getMessageAppArea(pkt).cData),
                    static_cast<void*>(&myAppMsg),
                    pge_network::MsgApp::getMsgAppTotalActualSize(myAppMsg)),
                "pkt msg app area ok") &
            assertEquals(
                0,
                memcmp(
                    static_cast<void*>(reinterpret_cast<uint8_t*>(&pkt) + nOffsetOfMsgAppDataEndInPkt),
                    static_cast<void*>(&pktJustAFullZeroMemoryAreaOfPgePacketSize),
                    nBytesShouldBeZero),
                "pkt remaining area is clean 1");
    }

    //bool test_iterateAllMsgApp()
    //{
    //    return false;
    //}
};
