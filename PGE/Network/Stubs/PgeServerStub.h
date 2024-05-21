#pragma once

/*
    ###################################################################################
    PgeServerStub.h
    This file is part of PGE.
    External header.
    PGE Network Server class stub
    Made by PR00F88
    ###################################################################################
*/

#include "../../PGEallHeaders.h"
#include "../../Config/PGEcfgProfiles.h"
#include "../PgeIServer.h"
#include "../PgeIServerClient.h"
#include "../PgePacket.h"

namespace pge_network
{

    /**
        The primary interface to stubbed server networking functionality.
        With current implementation is it more like a spy, but anyway I still call it stub.

        Regarding the packet sender functions: we have 1 virtual client connected, so that:
         - sendToAllClientsExcept() acts like sending to 1 client, and
         - sendToAll() acts like sending to the server itself and to 1 client, resulting in 2 sent packets.
    */
    class PgeServerStub : public PgeIServer
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeServerStub is included")   
#endif

    public:

        PgeServerStub(PGEcfgProfiles& cfgProfiles) :
            m_cfgProfiles(cfgProfiles)
        {

        }

        PgeServerStub(const PgeServerStub&) = delete;
        PgeServerStub& operator=(const PgeServerStub&) = delete;
        PgeServerStub(PgeServerStub&&) = delete;
        PgeServerStub&& operator=(PgeServerStub&&) = delete;

        virtual ~PgeServerStub() { shutdown(); };

        /* implement stuff from PgeIServerClient start */

        bool initialize() override
        {
            if (m_bInited)
            {
                return false;
            }

            m_bInited = true;
            return true;
        }

        bool shutdown() override
        {
            m_bInited = false;
            m_nPktCountTx = 0;
            m_mapTxMsgCount.clear();

            return true;
        }

        bool isInitialized() const override
        {
            return m_bInited;
        }

        void disconnect(const std::string& /*sExtraDebugText = ""*/) override
        {}

        void Update() override
        {}

        bool pollIncomingMessages() override
        {
            return m_bInited;
        }

        void pollConnectionStateChanges() override
        {
        }

        std::size_t getPacketQueueSize() const override
        { 
            return 0;
        }

        pge_network::PgePacket popFrontPacket() noexcept(false) override
        {
            throw std::exception("unimplemented");
        }

        std::set<pge_network::PgePktId>& getAllowListedPgeMessages() override
        { 
            throw std::exception("unimplemented");
        }

        std::set<pge_network::MsgApp::TMsgId>& getAllowListedAppMessages() override
        {
            throw std::exception("unimplemented");
        }

        void send(
            const pge_network::PgePacket& pkt,
            [[maybe_unused]] const pge_network::PgeNetworkConnectionHandle& connHandle = pge_network::ServerConnHandle) override
        {
            if (!isInitialized())
            {
                return;
            }

            ++m_nPktCountTx;
            if (pge_network::PgePacket::getPacketId(pkt) == pge_network::PgePktId::Application)
            {
                const pge_network::MsgApp* pMsgApp = pge_network::PgePacket::getMsgAppFromPkt(pkt);
                const uint8_t nMessageCount = pge_network::PgePacket::getMessageAppCount(pkt);
                // for now there is always exactly 1 message in each PgePacket, otherwise in future we will need to iterate
                assert(nMessageCount == 1);
                ++m_mapTxMsgCount[pge_network::MsgApp::getMsgAppMsgId(*pMsgApp)];
            }
        }

        uint32_t getRxPacketCount() const override { return 0; }
        uint32_t getTxPacketCount() const override { return m_nPktCountTx; }
        uint32_t getInjectPacketCount() const override { return 0; }

        uint32_t getRxPacketPerSecondCount() const override { return 0; }
        uint32_t getTxPacketPerSecondCount() const override { return 0; }
        uint32_t getInjectPacketPerSecondCount() const override { return 0; }

        const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getRxMsgCount() const override { throw std::exception("unimplemented"); }
        const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getTxMsgCount() const override { return m_mapTxMsgCount; }
        const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getInjectMsgCount() const override { throw std::exception("unimplemented"); }

        std::map<pge_network::MsgApp::TMsgId, std::string>& getMsgAppId2StringMap() override { throw std::exception("unimplemented"); }

        uint32_t getRxByteCount() const override { return 0; }
        uint32_t getTxByteCount() const override { return 0; }
        uint32_t getInjectByteCount() const override { return 0; }

        void WriteList() const override {}

        /* implement stuff from PgeIServerClient end */

        /* implement stuff from PgeServer start */

        bool startListening(const std::string& /*sAppVersion*/) override { return m_bInited; }

        void sendToAllClientsExcept(
            const pge_network::PgePacket& pkt,
            [[maybe_unused]] const pge_network::PgeNetworkConnectionHandle& exceptConnHandle = 0) override
        {
            if (!isInitialized())
            {
                return;
            }

            // we have 1 virtual client connected always
            send(pkt);
        }

        void sendToAll(const pge_network::PgePacket& pkt) override
        {
            if (!isInitialized())
            {
                return;
            }

            send(pkt);
            sendToAllClientsExcept(pkt);
        }

        /* Debug functions. */

        void setDebugNickname(const pge_network::PgeNetworkConnectionHandle& /*connHandle*/, const std::string& /*sNickname*/) override {}

        int getPing(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/,
            bool /*bForceUpdate*/) override
        { 
            return 0;
        }

        float getQualityLocal(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/,
            bool /*bForceUpdate*/) override
        {
            return 0.f;
        }

        float getQualityRemote(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/,
            bool /*bForceUpdate*/) override
        {
            return 0.f;
        }

        float getRxByteRate(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/,
            bool /*bForceUpdate*/) override
        {
            return 0.f;
        }

        float getTxByteRate(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/,
            bool /*bForceUpdate*/) override
        {
            return 0.f;
        }

        int64_t getPendingUnreliableBytes(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/,
            bool /*bForceUpdate*/) override
        {
            return 0;
        }

        int64_t getPendingReliableBytes(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/,
            bool /*bForceUpdate*/) override
        {
            return 0;
        }

        int64_t getSentButUnAckedReliableBytes(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/,
            bool /*bForceUpdate*/) override
        {
            return 0;
        }

        int64_t getInternalQueueTimeUSecs(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/,
            bool /*bForceUpdate*/) override
        {
            return 0;
        }

        std::string getDetailedConnectionStatus(
            const pge_network::PgeNetworkConnectionHandle& /*connHandle*/) const override
        {
            return "";
        }

        /* implement stuff from PgeServer end */

     private:
            PGEcfgProfiles& m_cfgProfiles;

            bool m_bInited{false};

            uint32_t m_nPktCountTx{ 0 };
            std::map<pge_network::MsgApp::TMsgId, uint32_t> m_mapTxMsgCount;

    }; // class PgeServerStub

} // namespace pge_network
