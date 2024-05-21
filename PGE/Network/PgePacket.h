#pragma once

/*
    ###################################################################################
    PgePacket.h
    This file is part of PGE.
    External header.
    PR00F's Game Engine packets
    Made by PR00F88
    ###################################################################################
*/

#include "../PGEallHeaders.h"

#include <stddef.h>  // offsetof()

#include <cassert>
#include <limits>
#include <type_traits>

// Make sure min/max weren't defined, even though now I'm including windows.h using winproof88.h that helps skip these definitions, and
// added NOMINMAX macro defined to project settings, I want to be sure that these are not defined by accident anywhere!
// Because it collides with std::min() / std::max() / std::numeric_limits<T>::max() !
#if defined(min) || defined(max)
#error "min or max are defined (maybe in windows.h)"
#endif

class PgePacketTest;  // forward declaring the unit test class so it can be friend of PgePacket for easier testing

/**
 * These packet and message structs are sent between server and clients.
 * Different endianness is not considered as an issue because all machines expected to use this have same endianness for now.
 * In case this changes in the future, use a lib like cereal to easily solve endianness issue.
 * 
 * Since we tx/rx/memcpy these structs, we have expectations of their memory layout, so we keep them POD as defined by C++11/14/17:
 * - they are trivial classes,
 * - they are standard-layout,
 * - they are trivially copyable.
 * Details: https://stackoverflow.com/questions/4178175/what-are-aggregates-and-trivial-types-pods-and-how-why-are-they-special/7189821#7189821
 * Note that POD is deprecated from C++20 hence we check the above 3 properties.
 * 
 * "A pointer to a standard-layout class may be converted (with reinterpret_cast) to a pointer to its first non-static data member and vice versa.
 *  If a standard-layout union holds two or more standard-layout structs, it is permitted to inspect the common initial part of them.
 *  The macro offsetof is only guaranteed to be usable with standard-layout classes." (https://en.cppreference.com/w/cpp/types/is_standard_layout)
 * 
 * "Objects of trivially-copyable types that are not potentially-overlapping subobjects are the only C++ objects that may be safely copied with
 *  std::memcpy or serialized to/from binary files with std::ofstream::write() / std::ifstream::read()."
 *  (https://en.cppreference.com/w/cpp/types/is_trivially_copyable)
*/

namespace pge_network
{

    typedef uint32_t PgeNetworkConnectionHandle;

    // since 0 is considered as an invalid handle at GNS level, we use this to refer to server, since
    // server never wants to send to this connection, or it automatically injects when this value is used.
    constexpr PgeNetworkConnectionHandle ServerConnHandle = 0;

    enum class PgePktId : uint32_t
    {
        UserConnectedServerSelf = 0,
        UserDisconnectedFromServer,
        ClientAppVersion,
        Application
    };

    // server -> self (injection)
    // PgeServer injects this message to the incoming packet queue so application level will receive it.
    // Server app will receive this message immediately after starting listening.
    // Server app will receive this message for each connected client with matching application version number.
    // Server app is NOT notified about clients with mismatching application version number.
    // Server app can process this in arbitrary way and might send another custom message to all clients about new user;
    // server app should not send this to clients; it is NOT allowlisted for clients by default;
    // it is NOT allowlisted either for server app because server should not receive this over network, it just injects it for itself.
    struct MsgUserConnectedServerSelf
    {
        static const PgePktId id = PgePktId::UserConnectedServerSelf;
        static const uint8_t nIpAddressMaxLength = 48;

        bool m_bCurrentClient;  // true means server injected itself its own birth, false means a client has connected
        char m_szIpAddress[nIpAddressMaxLength];
        // TODO: server always knows IP address of clients, but doesn't know its own IP address used by the clients to connect.
        // It should be grabbed from the first connecting client.
        // See if remote server address logged is the good server address in: PgeGnsWrapper::onSteamNetConnectionStatusChanged(),
        // client code, when client reaches k_ESteamNetworkingConnectionState_Connected. Jump there by searching for "KEKEKEKEKE"
    };
    static_assert(std::is_trivial_v<MsgUserConnectedServerSelf>);
    static_assert(std::is_trivially_copyable_v<MsgUserConnectedServerSelf>);
    static_assert(std::is_standard_layout_v<MsgUserConnectedServerSelf>);

    // server -> clients and to self
    // it is NOT allowlisted for server app because server should not receive this over network, it just injects it for itself
    struct MsgUserDisconnectedFromServer
    {
        static const PgePktId id = PgePktId::UserDisconnectedFromServer;
    };
    static_assert(std::is_trivial_v<MsgUserDisconnectedFromServer>);
    static_assert(std::is_trivially_copyable_v<MsgUserDisconnectedFromServer>);
    static_assert(std::is_standard_layout_v<MsgUserDisconnectedFromServer>);

    // clients -> server
    // PgeClient instance is expected to send this message to PgeServer instance right after connection is established.
    // This is managed by PGE automatically.
    // PgeServer expects PgeClient to have a matching application version.
    // PgeServer instance can set the expected application version using the startListening() API.
    // PgeClient instance can set the application version using the connectToServer() API.
    // Application versions on both sides are by default empty strings.
    struct MsgClientAppVersionFromClient
    {
        static const PgePktId id = PgePktId::ClientAppVersion;
        static const uint8_t nAppVersionMaxLength = 48;

        char m_szAppVersion[nAppVersionMaxLength];
    };
    static_assert(std::is_trivial_v<MsgClientAppVersionFromClient>);
    static_assert(std::is_trivially_copyable_v<MsgClientAppVersionFromClient>);
    static_assert(std::is_standard_layout_v<MsgClientAppVersionFromClient>);

    typedef uint8_t TByte;

    // application-specific message
    // server <-> client
    // With allowlisting, app messages can be separately allowed to be processed by clients and server based on MsgApp::TMsgId.
    struct MsgApp
    {
        friend class  PgePacketTest;
        friend struct PgePacket;     // PgePacket should have full r/w access since basically this is part of it

    public:
        typedef uint32_t TMsgId;   // TODO: could be decreased to uint16_t
        typedef uint8_t  TMsgSize;

        static const PgePktId id = PgePktId::Application;
        static const TMsgSize nMaxMessageLengthBytes = 240;

        static const TMsgId& getMsgAppMsgId(const MsgApp& msgApp);
        static const TMsgSize& getMsgAppDataActualSizeBytes(const MsgApp& msgApp);
        static const TMsgSize getMsgAppTotalActualSizeBytes(const MsgApp& msgApp);
        static const TByte* getMsgAppData(const MsgApp& msgApp);
        static TByte* getMsgAppData(MsgApp& msgApp);
        static bool fillMsgApp(
            MsgApp& myAppMsg,
            const TMsgId& msgAppMsgId,
            const TByte* msgAppData,
            TMsgSize nMsgAppDataSize);
        
    private:
        TMsgId m_msgId;  // this is checked by engine upon polling for new messages against the allowlists
        TMsgSize m_nMsgSize;
        /* This 'cMsgData' memory area is for 1 application message defined at application level, not here.
           So the application is responsible for copying the message here.
           The C++ standard guarantees that the members of a class or struct appear in memory in the same order as they are declared.
           This 'cMsgData' member needs to be the LAST member of this struct.
           When packaging MsgApp into MsgAppArea, we should put the actually used memory area (nMsgSize) and not the whole area, and
           increase MsgAppArea::m_nMessageCount by 1.
         */
        TByte m_cMsgData[nMaxMessageLengthBytes];

        static TMsgId& getMsgAppMsgId(MsgApp& msgApp);
        static TMsgSize& getMsgAppDataActualSizeBytes(MsgApp& msgApp);  // TODO: delete this non-const version later when not needed
    };
    static_assert(std::is_trivial_v<MsgApp>);
    static_assert(std::is_trivially_copyable_v<MsgApp>);
    static_assert(std::is_standard_layout_v<MsgApp>);

    static_assert(MsgApp::nMaxMessageLengthBytes <= std::numeric_limits<MsgApp::TMsgSize>::max(),
        "Size of MsgApp data should fit in MsgApp::nMsgSize");
    
    // memory area within a PgePacket, used when we are sending app message(s) in the packet
    struct MsgAppArea
    {
        friend class  PgePacketTest;
        friend struct PgePacket;     // PgePacket should have full r/w access since basically this is part of it

    public:
        typedef uint8_t TAreaLength;

        static const TAreaLength nMaxMessagesAreaLengthBytes = std::numeric_limits<TAreaLength>::max();

    private:
        uint8_t m_nMessageCount;                           // should be readable only by application
        TAreaLength m_nActualMessagesAreaLength;  // should be readable only by application
        /* This 'cData' memory area is for m_nMessageCount number of different app messages.
           The C++ standard guarantees that the members of a class or struct appear in memory in the same order as they are declared.
           This 'cData' member needs to be the LAST member of this struct.
           When sending, we should send the actually used memory area and not the whole area.
           In 'cData' we store at least 1 variable-sized MsgApp.
         */
        TByte m_cData[nMaxMessagesAreaLengthBytes];   // should be writable by application, but the pointer to it should be calculated by wrapper function
    };
    static_assert(std::is_trivial_v<MsgAppArea>);
    static_assert(std::is_trivially_copyable_v<MsgAppArea>);
    static_assert(std::is_standard_layout_v<MsgAppArea>);
    
    static_assert(
        sizeof(MsgApp) <= MsgAppArea::nMaxMessagesAreaLengthBytes,
        "At least 1 full MsgApp should fit into MsgAppArea.cData!");

    /**
    * PgePacket is the carrier of information over the network.
    * It always contains at least 1 message i.e. a pge_network struct starting with name Msg, since messages cannot be sent
    * on their own over the network.
    * Whenever we want to send data between server and clients, we put the message into a PgePacket instance and
    * pass it to PgeIServerClient::send(), the receiver will be notified about the arrival of this PgePacket instance and
    * can extract the message from it.
    */
    struct PgePacket
    {
        friend class PgePacketTest;

    public:
        enum class AutoFill {
            NONE,
            ZERO
        };

        // basic packet get/set

        static PgePktId& getPacketId(pge_network::PgePacket& pkt);
        static const PgePktId& getPacketId(const pge_network::PgePacket& pkt);

        static PgeNetworkConnectionHandle& getServerSideConnectionHandle(pge_network::PgePacket& pkt);
        static const PgeNetworkConnectionHandle& getServerSideConnectionHandle(const pge_network::PgePacket& pkt);

        static MsgUserConnectedServerSelf& getMessageAsUserConnected(pge_network::PgePacket& pkt);
        static const MsgUserConnectedServerSelf& getMessageAsUserConnected(const pge_network::PgePacket& pkt);

        static MsgUserDisconnectedFromServer& getMessageAsUserDisconnected(pge_network::PgePacket& pkt);
        static const MsgUserDisconnectedFromServer& getMessageAsUserDisconnected(const pge_network::PgePacket& pkt);

        static MsgClientAppVersionFromClient& getMessageAsClientAppVersionFromClient(pge_network::PgePacket& pkt);
        static const MsgClientAppVersionFromClient& getMessageAsClientAppVersionFromClient(const pge_network::PgePacket& pkt);

        static const uint8_t& getMessageAppCount(const pge_network::PgePacket& pkt);
        static const MsgAppArea::TAreaLength& getMessageAppsTotalActualLengthBytes(const pge_network::PgePacket& pkt);

        static bool isMessageAppAreaFull(const pge_network::PgePacket& pkt);

        static const pge_network::MsgApp* getMsgAppFromPkt(const pge_network::PgePacket& pkt);
        static pge_network::MsgApp* PgePacket::getMsgAppFromPkt(pge_network::PgePacket& pkt);

        static const pge_network::MsgApp::TMsgId& getMsgAppIdFromPkt(const pge_network::PgePacket& pkt);

        /**
        * This convenient function is for the application: the custom application-defined message can
        * be easily extracted from the packet.
        */
        template <class MsgAppDataType>
        static const MsgAppDataType& getMsgAppDataFromPkt(const pge_network::PgePacket& pkt)
        {
            return reinterpret_cast<const MsgAppDataType&>(getMsgAppFromPkt(pkt)->m_cMsgData);
        }

        /**
        * This convenient function is for the application: the custom application-defined message can
        * be easily extracted from the packet.
        */
        template <class MsgAppDataType>
        static MsgAppDataType& getMsgAppDataFromPkt(pge_network::PgePacket& pkt)
        {
            // simply invoke the const-version of getMsgAppDataFromPkt() above by const-casting:
            return reinterpret_cast<MsgAppDataType&>(
                const_cast<pge_network::MsgApp*>(getMsgAppFromPkt(const_cast<pge_network::PgePacket&>(pkt)))->m_cMsgData
            );
        }

        // packet initializers

        /*
        * Initializes the given packet to be used for sending MsgUserConnectedServerSelf.
        * A packet needs to be initialized only once.
        *
        * @param pkt                  The packet to be initialized.
        * @param connHandleServerSide The server-side connection handle of the sender (whoever is filling and sending the packet).
        * @param bCurrentClient       True if the user is the receiver as well (server notifies self about being born), false if the user is a different entity (a client).
        * @param sIpAddress           The IP address of the user being connected.
        * 
        * @return True if given PgePacket has been filled successfully, false otherwise.
        */
        static bool initPktPgeMsgUserConnected(
            pge_network::PgePacket& pkt,
            const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
            bool bCurrentClient,
            const std::string& sIpAddress);

        /*
        * Initializes the given packet to be used for sending MsgUserDisconnectedFromServer.
        * A packet needs to be initialized only once.
        *
        * @param pkt                  The packet to be initialized.
        * @param connHandleServerSide The server-side connection handle of the sender (whoever is filling and sending the packet).
        */
        static void initPktPgeMsgUserDisconnected(
            PgePacket& pkt,
            const PgeNetworkConnectionHandle& connHandleServerSide);

        /*
        * Initializes the given packet to be used for sending MsgClientAppVersionFromClient.
        * A packet needs to be initialized only once.
        *
        * @param pkt      The packet to be initialized.
        * @param sVersion The client application version to be sent to server.
        * 
        * @return True if given PgePacket has been filled successfully, false otherwise.
        */
        static bool initPktPgeMsgClientAppVersionFromClient(
            pge_network::PgePacket& pkt,
            const std::string& sClientAppVersion
        );

        /*
        * Initializes the given packet to be used to store app messages.
        * A packet needs to be initialized only once.
        * Then one or more addPktMsgApp() or preparePktMsgAppFill() can be used to store app messages inside it.
        * 
        * @param pkt                  The packet to be initialized.
        * @param connHandleServerSide The server-side connection handle of the sender (whoever is filling and sending the packet).
        * @param autoFill             If we want to fill the whole memory area or leave it untouched.
        */
        static void initPktMsgApp(
            pge_network::PgePacket& pkt,
            const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
            const AutoFill& autoFill = AutoFill::ZERO);

        /*
        * Copies the given msgApp into the given pkt.
        * Due to copying the given msgApp, from performance perspective try to avoid using this function and prefer preparePktMsgAppFill()!
        * Note that not the full MsgApp is copied, only the actually used memory area of the struct.
        * 
        * @param pkt    The packet to where we are going to copy the given msgApp struct.
        * @param msgApp The struct which will be copied into the given packet.
        * 
        * @return True on success, false otherwise.
        */
        static bool addPktMsgApp(
            pge_network::PgePacket& pkt,
            const pge_network::MsgApp& msgApp);

        /*
        * Gives a pointer to the proper memory area within the given pkt to where the application can write its custom app message data.
        * It checks if there is enough space available in the given packet for the app message data we are planning to write, and if so,
        * it sets the given app message id and app message data size at the proper location within the packet and returns a pointer pointing
        * to that memory area within the packet that can be directly written by the application to store the actual app message data.
        * 
        * From performance perspective using this function is preferred over addPktMsgApp() because in practice it results in less memory
        * copy operations since the caller can directly write its data instead of copying a MsgApp struct.
        * 
        * @param pkt             The packet to where we are going to copy the given msgApp struct.
        * @param msgAppId        The id of the app message we are planning to write into the given packet.
        * @param nMsgAppDataSize Size of the app message data we are planning to write into the given packet.
        * 
        * @return On success: a valid pointer pointing into the proper area of the given packet, nullptr otherwise.
        */
        static TByte* preparePktMsgAppFill(
            pge_network::PgePacket& pkt,
            MsgApp::TMsgId msgAppId,
            MsgApp::TMsgSize nMsgAppDataSize
        );

    private:
        PgePktId m_pktId;
        PgeNetworkConnectionHandle m_connHandleServerSide;

        union
        {
            MsgUserConnectedServerSelf m_userConnected;
            MsgUserDisconnectedFromServer m_userDisconnected;
            MsgClientAppVersionFromClient m_clientAppVersionFromClient;
            MsgAppArea m_app; // application should load/store its custom messages here
        } m_msg;

        // private getters/setter

        static MsgAppArea& getMessageAppArea(pge_network::PgePacket& pkt);
        static const MsgAppArea& getMessageAppArea(const pge_network::PgePacket& pkt);

        static uint8_t& getMessageAppCount(pge_network::PgePacket& pkt);
        static MsgAppArea::TAreaLength& getMessageAppsTotalActualLengthBytes(pge_network::PgePacket& pkt);

        // private initializers 

        static void initPktBasic(
            pge_network::PgePacket& pkt,
            const PgePktId& pktId,
            const pge_network::PgeNetworkConnectionHandle& connHandleServerSide,
            const AutoFill& autoFill);

    }; // struct PgePacket
    static_assert(std::is_trivial_v<PgePacket>);
    static_assert(std::is_trivially_copyable_v<PgePacket>);
    static_assert(std::is_standard_layout_v<PgePacket>);

} // namespace pge_network
