#pragma once

/*
    ###################################################################################
    PgeNetworkStub.h
    This file is part of PGE.
    External header.
    PGE Network Main class stub
    Made by PR00F88
    ###################################################################################
*/

#include <string>

#include "../../PGEallHeaders.h"
#include "../../Config/PGEcfgProfiles.h"
#include "../PgeINetwork.h"
#include "../PgePacket.h"
#include "PgeClientStub.h"
#include "PgeServerStub.h"

namespace pge_network
{

    /**
        The primary interface to stubbed networking functionality.
    */
    class PgeNetworkStub : public PgeINetwork
    {
#ifdef PGE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PgeNetwork is included")   
#endif

    public:
        PgeNetworkStub(PGEcfgProfiles& cfgProfiles) :
            m_cfgProfiles(cfgProfiles),
            m_client(cfgProfiles),
            m_server(cfgProfiles)
        {

        }

        PgeNetworkStub(const PgeNetworkStub&) = delete;
        PgeNetworkStub& operator=(const PgeNetworkStub&) = delete;
        PgeNetworkStub(PgeNetworkStub&&) = delete;
        PgeNetworkStub&& operator=(PgeNetworkStub&&) = delete;

        virtual ~PgeNetworkStub()
        {
            shutdown();
        }

        bool initialize() override
        {
            if (isInitialized())
            {
                return false;
            }

            m_bServer = m_cfgProfiles.getVars()[pge_network::PgeINetwork::CVAR_NET_SERVER].getAsBool();
            
            // I dont know why the following line is not compiling:
            // m_pServerClient = m_bServer ? (&m_iserver) : (&m_iclient);
            if (m_bServer)
            {
                m_pServerClient = &m_iserver;
            }
            else
            {
                m_pServerClient = &m_iclient;
            }

            return m_pServerClient->initialize();
        }

        bool shutdown() override
        { 
            if (m_pServerClient)
            {
                m_pServerClient->shutdown();
            }
            m_pServerClient = nullptr;
            return true;
        }

        bool isInitialized() const override
        {
            return m_pServerClient && m_pServerClient->isInitialized();
        }

        bool reinitialize() override
        {
            if (shutdown())
            {
                return initialize();
            }
            return false;
        }

        void disconnect(const std::string& /*sExtraDebugText = ""*/) override {}

        bool isServer() const override
        {
            return m_bServer;
        }

        void Update() override {}

        pge_network::PgeIServerClient* getServerClientInstance() override { return m_pServerClient; }
        pge_network::PgeIClient& getClient() override { return m_client; }
        pge_network::PgeIServer& getServer() override { return m_server; }

        void WriteList() const override {}


    private:
        PGEcfgProfiles& m_cfgProfiles;
        pge_network::PgeIServerClient* m_pServerClient{nullptr};
        pge_network::PgeClientStub m_client;
        pge_network::PgeServerStub m_server;
        pge_network::PgeIClient& m_iclient{ m_client };
        pge_network::PgeIServer& m_iserver{ m_server };
        bool m_bServer{false};
    }; // class PgeNetworkStub

} // namespace pge_network
