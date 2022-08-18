\page networking Networking

[TOC]

\section networking_intro Intro

When it was high time to add multiplayer support to my game engine, I browsed the internet for already existing solutions I could integrate, because the networking topic itself on its own contains lots of challenges.  
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

Unfortunately, **yojimbo** doesn't have **NAT punch-through** support, for which I decided not to use it, because I don't want to implement / or use additional library for this purpose.  
Both **GNS** and **SLN** offer sophisticated connection establish between computers NOT connected directly to the internet, this is not in **yojimbo**.  

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
Here is a **small game project** utilizing **GSN**: https://github.com/David-Elle-Jack-Oisin/TeamProject/tree/main/src/network

I'm implementing the **client-server** networking model in the engine because it sounds suitable for my future projects and hopefully easier to implement than P2P.
 
\section networking_pages Web Pages about this Topic

Client-server vs P2P model:
 - https://gafferongames.com/post/what_every_programmer_needs_to_know_about_game_networking/
 - https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/Networking/Overview/
 - https://gamedev.stackexchange.com/questions/67738/limitations-of-p2p-multiplayer-games-vs-client-server

Importance of backend services:
 - https://www.gamedeveloper.com/programming/half-life-and-team-fortress-networking-closing-the-loop-on-scalable-network-gaming-backend-services
 
NAT Punch-through:
 - https://stackoverflow.com/questions/43181893/nat-punchthrough-understanding-p2p-concept
 - https://keithjohnston.wordpress.com/2014/02/17/nat-punch-through-for-multiplayer-games/
 - http://www.raknet.net/raknet/manual/natpunchthrough.html
 - https://bford.info/pub/net/p2pnat/

Floating Point Determinism:
 - https://gafferongames.com/post/floating_point_determinism/

Serialization, compression, encryption, client-side prediction, latency mitigation, etc:
 - https://gafferongames.com/post/reading_and_writing_packets/
 - https://pvigier.github.io/2019/09/08/beginner-guide-game-networking.html
 - https://www.gabrielgambetta.com/client-server-game-architecture.html
 - https://gafferongames.com/post/serialization_strategies/
 - https://isetta.io/blogs/week-9/
 - https://developer.valvesoftware.com/wiki/Latency_Compensating_Methods_in_Client/Server_In-game_Protocol_Design_and_Optimization
 - https://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking
 
Security Issues of Online Gaming:
 - https://www.gamedev.net/reference/articles/article2062.asp
 - https://www.gamedeveloper.com/disciplines/security-in-online-games

Misc:
 - file:///D:/_DOKSIK_/J%C3%A1t%C3%A9kfejleszt%C3%A9s/networking/faq.htm
 - https://www.gamedeveloper.com/programming/multiplayer-math
 - file:///D:/_DOKSIK_/J%C3%A1t%C3%A9kfejleszt%C3%A9s/networking/TheQuake3NetworkingModel.html
 - https://derekzeng.me/articles/zt-bookofhook-the-quake3-networking-model
 - https://gamedev.net/forums/topic/291287-the-quake3-networking-model-question/2841358/
 - https://www.jfedor.org/quake3/
 - https://gamasutra.com/view/news/35929/Opinion_Synchronous_RTS_Engines_And_A_Tale_of_Desy
 - http://blog.wolfire.com/2011/03/GDC-Session-Summary-Halo-networking
 - https://docs.unrealengine.com/udk/Three/NetworkingOverview.html

 - http://www.cs.uu.nl/docs/vakken/magr/portfolio/HA2/lecture3.pdf

