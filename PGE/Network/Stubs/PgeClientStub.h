#pragma once

/*
    ###################################################################################
    PgeClientStub.h
    This file is part of PGE.
    External header.
    PGE Network Client class stub
    Made by PR00F88
    ###################################################################################
*/

#include "../../PGEallHeaders.h"
#include "../../Config/PGEcfgProfiles.h"
#include "../PgeIClient.h"
#include "../PgeIServerClient.h"
#include "../PgePacket.h"

namespace pge_network
{

    /**
        The primary interface to stubbed client networking functionality.
    */
    class PgeClientStub : public PgeIClient
    {
    public:

        PgeClientStub(PGEcfgProfiles& cfgProfiles) :
            m_cfgProfiles(cfgProfiles)
        {

        }

        PgeClientStub(const PgeClientStub&) = delete;
        PgeClientStub& operator=(const PgeClientStub&) = delete;
        PgeClientStub(PgeClientStub&&) = delete;
        PgeClientStub&& operator=(PgeClientStub&&) = delete;

        virtual ~PgeClientStub()
        {
            shutdown();
        }

        /* implement stuff from PgeIServerClient start */

        bool initialize() override { return true; }
        bool shutdown() override { return true; }
        bool isInitialized() const override { return false; }
        void disconnect(const std::string& /*sExtraDebugText = ""*/) override {}

        void Update() override {}

        bool pollIncomingMessages() override { return true; }
        void pollConnectionStateChanges() override {}

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
            const pge_network::PgePacket& /*pkt*/,
            const pge_network::PgeNetworkConnectionHandle& /*connHandle = pge_network::ServerConnHandle*/) override {}

        uint32_t getRxPacketCount() const override { return 0; }
        uint32_t getTxPacketCount() const override { return 0; }
        uint32_t getInjectPacketCount() const override { return 0; }

        uint32_t getRxPacketPerSecondCount() const override { return 0; }
        uint32_t getTxPacketPerSecondCount() const override { return 0; }
        uint32_t getInjectPacketPerSecondCount() const override { return 0; }

        const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getRxMsgCount() const override { throw std::exception("unimplemented"); }
        const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getTxMsgCount() const override { throw std::exception("unimplemented"); }
        const std::map<pge_network::MsgApp::TMsgId, uint32_t>& getInjectMsgCount() const override { throw std::exception("unimplemented"); }

        std::map<pge_network::MsgApp::TMsgId, std::string>& getMsgAppId2StringMap() override { throw std::exception("unimplemented"); }

        uint32_t getRxByteCount() const override { return 0; }
        uint32_t getTxByteCount() const override { return 0; }
        uint32_t getInjectByteCount() const override { return 0; }

        void WriteList() const override {}

        /* implement stuff from PgeIServerClient end */

        /* implement stuff from PgeClient start */

        bool connectToServer(const std::string& /*sServerAddress*/) override
        {
            return true;
        }

        const pge_network::PgeNetworkConnectionHandle& getConnectionHandle() const override
        {
            return m_myClientSideConnectionHandle;
        }

        const pge_network::PgeNetworkConnectionHandle& getConnectionHandleServerSide() const override
        {
            return m_myServerSideConnectionHandle;
        }

        const char* getServerAddress() const override
        {
            return "";
        }

        int getPing(bool /*bForceUpdate*/) override
        {
            return 0;
        }

        float getQualityLocal(bool /*bForceUpdate*/) override
        {
            return 0.f;
        }

        float getQualityRemote(bool /*bForceUpdate*/) override
        {
            return 0.f;
        }

        float getRxByteRate(bool /*bForceUpdate*/) override
        {
            return 0.f;
        }

        float getTxByteRate(bool /*bForceUpdate*/) override
        {
            return 0.f;
        }

        int64_t getPendingUnreliableBytes(bool /*bForceUpdate*/) override
        {
            return 0;
        }

        int64_t getPendingReliableBytes(bool /*bForceUpdate*/) override
        {
            return 0;
        }

        int64_t getSentButUnAckedReliableBytes(bool /*bForceUpdate*/) override
        {
            return 0;
        }

        int64_t getInternalQueueTimeUSecs(bool /*bForceUpdate*/) override
        {
            return 0;
        }

        std::string getDetailedConnectionStatus() const override
        {
            return "";
        }

        /* implement stuff from PgeClient end */

    private:

        // ---------------------------------------------------------------------------

        PGEcfgProfiles& m_cfgProfiles;
        pge_network::PgeNetworkConnectionHandle m_myClientSideConnectionHandle{ 0 };
        pge_network::PgeNetworkConnectionHandle m_myServerSideConnectionHandle{ 0 };
    };


} // namespace pge_network
