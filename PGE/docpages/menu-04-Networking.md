\page networking Networking

[TOC]

\section networking_intro Intro

When it was time to add multiplayer support to my game engine, I browsed the internet for already existing solutions I could integrate, because the networking topic itself on its own contains lots of challenges.  
I didn't want to reinvent the wheel, for me it was enough to read about the problems and possible solutions in this area, and to use an existing solution instead of implementing my own.  
Establishing communication between computers behind NAT devices, handling different endianness of computers, or lag compensation are just a few examples of the challenges in this area.

\section networking_libs Networking Libraries

For lists of networking libraries, check these pages:

 - https://www.reddit.com/r/gamedev/comments/93kr9h/recommended_c_udp_networking_library/
 - https://github.com/MFatihMAR/Game-Networking-Resources

During my research, I checked the following 3 popular libraries from a closer perspective:
 - **yojimbo**: https://github.com/networkprotocol/yojimbo
   - https://github.com/networkprotocol/yojimbo/issues/25
   - https://www.gamedev.net/forums/topic/697635-is-anyone-using-yojimbo-for-networking/
 - **GNS (GameNetworkingSockets)**: https://github.com/ValveSoftware/GameNetworkingSockets
 - **SLN (SLikeNet)**: https://github.com/SLikeSoft/SLikeNet

All 3 libs offer features like **message encryption between players, packet fragmentation and reassembly, cross-platform support, traffic statistics**.  
So I don't need to implement any such low-level features, I can more concentrate on higher-level network-related features.

Unfortunately, **yojimbo** doesn't have **NAT punch-through** support, for which I decided not to use it, because I don't want to implement / or use additional library for this purpose.  
Both **GNS** and **SLN** offer sophisticated connection establish between computers that are NOT connected directly to the internet, this is not in **yojimbo**.  

Although the codebase of **SLN** hasn't changed since 2019, it has a solid base: it is a fork of **RakNet** which was developed for more than a decade, and it was also used by Unity engine (maybe it is still used under the hood).  
In theory, **SLN** fixes some IPv6-related bugs of **RakNet** but I don't know if it has become really bug-free.  

Comparing **GNS** and **SLN** today is more difficult than it used to be in ~2019 because since then **GNS** added some missing features already available in **SLN**.  
Both libs provide:
 - experience by others used in actual games;
 - not only client-server model but also P2P;
 - IPv6 support.
 
**SLN** offers support for game object serialization and data compression, I don't see much support for these in **GNS** as of June 2022 with version 1.4.0.  
It also has some other fancy stuff like game lobby, autopatcher, etc.  
It also has much less dependencies and easier to build.

**GNS** on the other hand, is still actively maintained by Valve, used by Dota and CSGO.  
Because of this, I opted for **GNS**.  
Here is a **small game project** utilizing **GNS**: https://github.com/David-Elle-Jack-Oisin/TeamProject/tree/main/src/network

\section client_server Client-Server Model

I'm implementing the **client-server** networking model in the engine because it sounds suitable for my future projects and hopefully easier to implement than P2P.  
In this model, there is always exactly 1 machine with server role, and some clients connected to the server.  
There is no communication between clients, server keeps all clients updated about game state.

\section pge_network_access Access to Networking Functionality

PGE provides networking functionality through the PgeNetwork class instance in pge_network namespace, that can be accessed by PGE::getNetwork() function.  
This PgeNetwork instance must be initialized before networking functionality can be used, however PGE initializes it automatically upon startup.  
It will be either a client or server instance based on the value of CVAR_NET_SERVER.  
Common functions can be accessed through PgeNetwork::getServerClientInstance(), more specifics are accessed through either PgeNetwork::getClient() or PgeNetwork::getServer() functions.

TODO: ask doxygen to insert value of CVAR_NET_SERVER above!

TODO: make a separate page explaining CVARs!

\section pge_network_packets Packets and Messages

Server and client instances communicate by sending and receiving PgePacket structs that encapsulate single messages.  
As of PGE v0.4 there can be only 1 message within a PgePacket, and there is only 3 different kind of messages:
 - MsgUserConnected;
 - MsgUserDisconnected;
 - MsgApp.

An application can define its custom messages, all those will be MsgApp kind.

TODO: make link to tag PGE v0.4 above!

TODO: should we use pge_network namespace everywhere explicitly?

\section pge_network_allowlisting Allowlisting

In order to be able to receive messages over the network, the messages need to be explicitly allowlisted, otherwise they are ignored.  
By default server instance is allowed to receive MsgApp messages only, client instance is allowed to receive MsgUserDisconnected and MsgApp messages.  
Server also processes MsgUserConnected and MsgUserDisconnected, however it just self-injects these, that is why it is NOT allowed to receive these over network.  

It is very important to understand that even though MsgApp is allowed for both client and server, **by default any kind of custom message inside MsgApp will be ignored**.  
**Application MUST explicitly allowlist its custom messages separately for both client and server.**  
This can be done by adding the allowed custom messages to the container accessed by PgeNetwork::getServerClientInstance().getAllowListedAppMessages().

\section networking_pages Networking Reading Materials

\subsection low_level_networking_pages Low-level Networking Topics

The Low-level networking topics for me are usually those topics which I found very fundamental to understand and use BUT I don't want to implement.  
For example, packet fragmentation and reassembly: it is good to know why it is needed and I want to have it by using the already existing implementation in the chosen library instead of implementing it by myself.

Packet Basics:
 - https://gafferongames.com/post/reading_and_writing_packets/
 - https://gafferongames.com/post/serialization_strategies/

Floating Point Determinism:
 - https://gafferongames.com/post/floating_point_determinism/
 
NAT Punch-through:
 - https://stackoverflow.com/questions/43181893/nat-punchthrough-understanding-p2p-concept
 - https://keithjohnston.wordpress.com/2014/02/17/nat-punch-through-for-multiplayer-games/
 - http://www.raknet.net/raknet/manual/natpunchthrough.html
 - https://bford.info/pub/net/p2pnat/

Importance of backend services:
 - https://www.gamedeveloper.com/programming/half-life-and-team-fortress-networking-closing-the-loop-on-scalable-network-gaming-backend-services
 
Security Issues of Online Gaming:
 - https://www.gamedev.net/reference/articles/article2062.asp
 - https://www.gamedeveloper.com/disciplines/security-in-online-games

\subsection high_level_networking_pages High-level Networking Topics

The High-level networking topics for me are usually those topics which I also want/have to implement in my software.  
For example, scheduling of sending packets, tick rate, etc.

Client-server vs P2P model:
 - https://gafferongames.com/post/what_every_programmer_needs_to_know_about_game_networking/
 - https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/Networking/Overview/
 - https://gamedev.stackexchange.com/questions/67738/limitations-of-p2p-multiplayer-games-vs-client-server

Client-side prediction, interpolation, latency mitigation, etc:
 - https://www.gabrielgambetta.com/client-server-game-architecture.html
 - https://isetta.io/blogs/week-9/
 - https://learn.colyseus.io/phaser/2-linear-interpolation.html
 - https://www.gabrielgambetta.com/client-side-prediction-server-reconciliation.html
 - https://gafferongames.com/post/what_every_programmer_needs_to_know_about_game_networking/
 - https://www.codersblock.org/blog/multiplayer-fps-part-8
 - https://learn.colyseus.io/phaser/1-basic-player-movement.html
 - https://forum.unity.com/threads/tips-for-server-authoritative-player-movement.199538/
 - https://gamedev.stackexchange.com/questions/184334/how-does-a-server-receive-every-player-input-with-a-server-authoritative-structu
 - https://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking
 - CS 1.6 Rates: https://steamcommunity.com/sharedfiles/filedetails/?id=126383209
 - https://developer.valvesoftware.com/wiki/Latency_Compensating_Methods_in_Client/Server_In-game_Protocol_Design_and_Optimization
 - https://docs.unrealengine.com/udk/Three/NetworkingOverview.html
 - https://docs.unrealengine.com/udk/Three/NetworkingOverview.html#Player%20Movement%20and%20Prediction
 - http://www.cs.uu.nl/docs/vakken/magr/portfolio/HA2/lecture3.pdf
 - http://blog.wolfire.com/2011/03/GDC-Session-Summary-Halo-networking
 
Misc:
 - https://pvigier.github.io/2019/09/08/beginner-guide-game-networking.html
 - file:///D:/_DOKSIK_/J%C3%A1t%C3%A9kfejleszt%C3%A9s/networking/faq.htm
 - https://www.gamedeveloper.com/programming/multiplayer-math
 - file:///D:/_DOKSIK_/J%C3%A1t%C3%A9kfejleszt%C3%A9s/networking/TheQuake3NetworkingModel.html
 - https://derekzeng.me/articles/zt-bookofhook-the-quake3-networking-model
 - https://gamedev.net/forums/topic/291287-the-quake3-networking-model-question/2841358/
 - https://www.jfedor.org/quake3/
 - https://gamasutra.com/view/news/35929/Opinion_Synchronous_RTS_Engines_And_A_Tale_of_Desy



