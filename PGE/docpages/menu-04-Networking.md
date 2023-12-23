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

\section multiplayer_cheating Cheating in Multiplayer

There are many ways to cheat in multiplayer games, and PGE doesn't provide protection against it.  
However, a good implementation in application level can overcome some forms of cheating.  
A common approach is to treat the **server as the only authorative instance** and make clients show only a replication of server state.  
For example: player movement. When a player presses a button to move, it should send a request/command message to the server about the keypress, and the server calculates the actual movement of the player.  
Then it replies back to the client(s) with the updated position of the player who requested the move, client(s) receive(s) the reply and move(s) the player to the position calculated by the server.  
Since server takes care of the entire game state, simulate physics, calculate new positions, etc. and replicates game state to clients, it is the only authorative element of the multiplayer game session.  
This way it is more difficult for clients to do anything from an illegal position, e.g. put themselves out of map bounds intentionally because always the server calculates their position based on client inputs that can be rejected as well.

\section implementation_details Implementation Details

PGE currently does not give explicit support on features like linearly interpolated player positions, it just gives a basic framework to establish connections between clients and the server as described above.  
However, my game [PRooFPS-dd](https://github.com/proof88/PRooFPS-dd) implements some of these features so I'm giving some words about this topic here.

\subsection original_naive_impl Original Naive Implementation (in v0.1.2)

Until [PRooFPS-dd v0.1.2 Private Beta](https://github.com/proof88/PRooFPS-dd/releases/tag/v0.1.2-PrivateBeta) **my naive approach was to tie input sampling to rendering frame rate and send messages between server and clients as soon as input was detected**.  
As already explained above, in general it is good to select the server as the only authorative instance in the network to provide a basic implementation against cheating.  
So in my naive approach, when client player pressed a button to move, it did not move the player object, just sent a message to server about the input.  
The server processed the message and replied back as soon as possible.  
The updated player positions were in server's response, so on client side I updated the player position upon receiving the response from the server.

This approach looks good if you have high frame rate (e.g. 60 FPS) because client request and server response messages happen very fast, so player movement looks smooth.  
However, it has multiple downsides as well:
 - because sending and processing messages is tied to rendering framerate, so player movement speed will heavily rely on the framerate;
 - the higher the framerate is, the more messages will be sent across computers, that could overload a machine having lower framerate (i.e. packet buffer might get full leading to packet drops).

Although the first issue could be solved by calculating new player position based on measured delta time elapsed since last update instead of using constant values, we would still have the second issue.  
Although physics is not part of this page, in general the variable delta time-based physics is not a good approach anyway because of multiple reasons (e.g. different machines with different delta will calculate different floating point results) so **I've implemented fixed delta time approach**, more you can read about it:
 - here: https://fabiensanglard.net/timer_and_framerate/index.php
 - and here: https://gafferongames.com/post/fix_your_timestep/

Implementing phyics update with fixed delta time approach helped a lot to introduce the tick-based implementation as described below.

\subsection new_tick_based_implementation New Tick-Based Implementation (from v0.1.3)

Now back to the networking part.  
From now on the techniques I'm describing are very common in multiplayer games and the terms I'm using are same or very similar as in [Counter-Strike](https://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking).

We want framerate-independent player movement, so we have our framerate-independent physics implemented as well, we should tie the input sampling, physics and messaging together, so they will be done with a different rate than the framerate.  
These things (input sampling, game state update, simulation, physics, messaging) tied together into a single operation called **tick**.  
The number of a tick is executed per second is called **tickrate**.

This is the theory you can read everywhere on the internet.  
But actually my implementation still does the input sampling part per frame and I'm more carefully explaining this later.

So the **essence of moving away from the naive approach is to understand that we execute different part of core game code at different rates**:
 - **framerate**: frequency of frame rendering;
 - **tickrate**: frequency of ticks i.e. input sampling, game state update and messaging together.
 
Another rule is that **framerate >= tickrate**.  
So it is totally ok to have framerate 60 while having tickrate 20. This means that we maximize the number of iterations of main game loop at 60, we target 60 rendered frames every second, and we do only 20 ticks every second.  
This also eases the requirement of CPU processing power and network bandwidth.

However, it is not that trivial to say that: okay, from now on I'm just sampling and sending input from client to server at 20 Hz because then the player will really feel the delay in the movement.  
So **with my implementation the framerate -> tickrate transition mostly helped reducing network traffic in server -> client direction but not in the other direction**.

So in the next sections I explain in more detail how I introduced rate-limiting on client- and server-side.

\subsection server_behavior_v014 New Server Behavior (v0.1.3, v0.1.4)

Since physics simulation is done on server-side, introducing **tickrate** mostly affected the server: using fixed delta approach lead to more reliable player position updates.  
Sending new player states to clients is done now at tickrate, which in case of 20 Hz introduced a ~66% reduction in network traffic in server -> client direction compared to having the framerate for this rate as in v0.1.2.  

Even though the **bullet travel update traffic** from server to client direction was also reduced due to the above, I **decided to simply stop** sending this kind of traffic.  
The reason is that even though server simulates bullet travel, clients can also do it on their side. They just simulate the bullet travel, don't care about the hits.  
So still server informs the clients about the born and removal of a bullet (e.g. if the bullet hit a player or wall), but between bullet born and removal the clients can move the bullet on their own.  
This also greatly reduced server -> client direction traffic.

Some operations became **continuous operations**: when enabled, server executes the action in every tick until explicitly stopped.  
An example for this is player strafe: once player starts strafing, server simulates it at tickrate until player stops strafing.  
This way we managed to stop clients from storming the server with inputs such as strafe at their framerate.  
It is important to understand that for such actions we should not only send out message when a button is pressed but also when it is released.

\subsection client_behavior_v014 New Client Behavior (from v0.1.4)

With the tickrate introduced, we successfully solved the problem of a slower machine not be able to keep up with processing messages when faster machines are also present in the network.  
As I already explained above, with my implementation the framerate -> tickrate transition mostly helped reducing network traffic in server -> client direction but not in the other direction.

To reduce traffic in client -> server direction, one idea was the **continuous operation** that I already explained above, so I'm not explaining it here again.  
This was used also for the attack (left mouse button) action, once the button is pressed, server simulates that, no need to continuously send it by client.

The basic rule doesn't change:
 - player is not moved in the moment of detected player input, because player is moved only when server responds with the updated coordinates.  
 - player is repositioned on client side when server sends the new coordinates (this was same in the naive approach as well).

For other actions such as changing weapon with mouse scroll or keyboard, or reloading the weapon, I **introduced rate-limiting with a simple delay**:  
a predefined amount of time MUST elapse before the client can send another same kind of message to the server.  
Note that there is no use of storming the server with higher rate in tricky ways because the server also calculates with the minimum delays for rate-limiting thus there is no benefit for the player to storm the server.

For other actions like updating weapon angle (client is moving the crosshair by mouse movement), I had to **introduce a more sophisticated rate-limiting method**:
 - if the crosshair movement results in changing weapon angle bigger than a specific amount of degree, client sends such update max 10 times per second;
 - otherwise client sends update max 5 times per second.

Details in pseudocode can be checked later on this page in function handleInputAndSendUserCmdMove().

\subsection server_behavior_v015 New Server Behavior (v0.1.5)

With low tickrate the physics calculations might not be precise enough. Even with 20 Hz tickrate we [saw we could not jump on some boxes or fall in between some boxes](https://github.com/proof88/PRooFPS-dd/issues/234).

So I decided to introduce the **cl_updaterate** CVAR that controls how often server should send the updates to clients.  
It is somehow dependent of the tickrate: in every tick we can either send out updates to clients or postpone to another tick.  
So if we set high tickrate like 60 Hz, we can have the very precise physics calculations while having cl_updaterate as 20 Hz keeps the required bandwidth low.  
Rules:
 - **0 < cl_updaterate <= tickrate**;
 - **tickrate % cl_updaterate == 0** (clients should receive UPDATED physics results evenly distributed in time).

I also introduced another CVAR called **physics_rate_min**.  
It allows running multiple physics iterations per tick, so if after all you still set a lower tickrate, you can still have more precise physics.  

The question in this case: why do we even have tickrate then if physics and server -> client updates can have different rate?  
The answer is that not only these are handled in a tick on server-side, but other stuff also like updating player- and map item-respawn timers, and in the future some more additions will be in place too.  
So in general it is good if we have the flexibility of fine-tuning these values.

\subsection new_behavior_overview Old and New Behavior Pseudocode

The following pseudocode shows what functions are invoked by runGame() that are relevant from networking perspective.  
Server and client instances have the same runGame() code. Some parts are executed only by the server or the client, that is visible from the pseudocode anyway.  
Some parts were changed between different versions, in those cases I specified the changes with version numbers.  
In the comments I mention what kind of messages are generated with approximated rates total PKT rate and per-client PKT rate.  
**We are estimating with an intense situation when 8 players are playing the game, and everyone is moving, shooting, and picking up an item at the same time.**

I also mention **AP (action point)** wherever I think change should be introduced.

```.cpp

// some constants just for the pkt count calculations in this example
constexpr int nClientsCount = 7;
constexpr int nPlayerCount = nClientsCount + 1;  // +1 is the server itself
constexpr int nBulletsCount =
              nPlayerCount * 6 = 48;             // assuming all players have 6 travelling bullets in-air at the moment (~worst-case)
constexpr int nMapItemsCount = 25;               // e.g. 9 pistols + 9 mchguns + 7 medkits

PGE::runGame() {
  while ( isGameRunning() ) {
  @FRAMERATE (ideally 60 FPS)
  
      onGameFrameBegin() {
        proofps_dd::PRooFPSddPGE::onGameFrameBegin()  // nothing relevant for now
      }
    
  // START transfer PKTs from GNS to PGE level
      getNetwork().Update()
  // END transfer PKTs from GNS to PGE level
    
  // START transfer PKTs from PGE to APP (proofps) level
  // last change in v0.1.4
      while (getNetwork().getServerClientInstance()->getPacketQueueSize() > 0) {
        onPacketReceived( getNetwork().getServerClientInstance()->popFrontPacket() ) {       // invoke application code for all received pkt in m_queuePackets
          proofps_dd::PRooFPSddPGE::onPacketReceived() {
            handleUserConnected();               // v0.1.4: might generate a few packets but that is only when a new user has just connected, nothing to do here.
            handleUserDisconnected();            // no networking
            handleUserSetupFromServer();         // no networking
            handleUserCmdMoveFromClient();       // v0.1.4: 18 PKT/s @ 60 FPS server -> client
            handleUserUpdateFromServer();        // no networking
            handleBulletUpdateFromServer();      // no networking
            handleMapItemUpdateFromServer();     // no networking
            handleWpnUpdateFromServer();         // no networking
            handleWpnUpdateCurrentFromServer();  // no networking
          }
        }
      }
  // END transfer PKTs from PGE to APP (proofps) level
  
      onGameRunning() {
        proofps_dd::PRooFPSddPGE::onGameRunning() {
          // this block contains updates in v0.1.3, v0.1.4 and v0.1.5
          
          serverUpdateWeapons();                   // v0.1.4: 0 PKT/s @ 60 FPS server -> client (see later in handleUserCmdMoveFromClient() why this is 0)
          
          if (getNetwork().isServer()) {
          @TICKRATE (ideally 60 Hz)                // v0.1.3: this was @FRAMERATE in v0.1.2, so all results of v0.1.3 in this block are only 1/3 of v0.1.2.
                                                   // v0.1.5: this is ideally 60 because cl_updaterate controls server->client updates, physics_rate_min allows more precise physics 
              mainLoopServerOnlyOneTick() {
              @PHYSICS_RATE_MIN (ideally 60 Hz)      // v0.1.5: physics_rate_min introduced, physics_rate_min >= tickrate
                  serverGravity();                   // no networking
                  serverPlayerCollisionWithWalls();  // no networking
                  serverUpdateBullets();             // v0.1.4: 160 PKT/s @ 20 Hz tickrate server -> client
                                                     // v0.1.5: 480 PKT/s @ 60 Hz physics_rate_min server -> client
                  serverPickupAndRespawnItems();     // v0.1.4: 180 PKT/s @ 20 Hz tickrate server -> client
                                                     // v0.1.5: 540 PKT/s @ 60 Hz physics_rate_min server -> client
                  updatePlayersOldValues();          // no networking
              @END PHYSICS_RATE_MIN
              
              serverUpdateRespawnTimers();           // no networking
              
              @CL_UPDATERATE (ideally 20 Hz)
                  serverSendUserUpdates();           // v0.1.4, v0.1.5: 160 PKT/s @ 20 Hz server -> client
              @END CL_UPDATERATE
              
              }  // end mainLoopServerOnlyOneTick()
          @END TICKRATE
          }  // end isServer()
          else {
          @TICKRATE (ideally 20 Hz)                
              mainLoopClientOnlyOneTick() {        // v0.1.4: client-side tick got introduced (same value as for server-side)
              @PHYSICS_RATE_MIN (ideally 60 Hz)
                  clientUpdateBullets()            // v0.1.4: no networking, client-side bullet travel simulation got introduced (without collision-detection)
              @END PHYSICS_RATE_MIN
              }
          @END TICKRATE
          }  // end client
          
          @FRAMERATE again
          mainLoopShared() {
              handleInputAndSendUserCmdMove();     // v0.1.4: 80 PKT/s with 7 clients (8 players) @ 60 FPS client -> server
              
          }  // end mainLoopShared
        }  // end proofps_dd::PRooFPSddPGE::onGameRunning()
      }  // end onGameRunning()
    
      RenderScene();
      frameLimit();    
  
  @END FRAMERATE
  }  // end while ( isGameRunning() )
}

```

\subsection packet_rate Packet Rate and Packet Data Rate

**Packet Rate** shows the number of packets processed per second by server or client.  
I also calculate the estimated **Packet Data Rate** based on the packet rate and size of packets.  
The improvements through versions are very decent and were really needed to solve [the packet congestion issue](https://github.com/proof88/PRooFPS-dd/issues/184).

\subsubsection server_packet_rate Server Packet Rate and Packet Data Rate

In this section we talk about **client -> server** traffic.

Considering 8 players:
 - **v0.1.2:**
   - **480 PKT/s with 7 clients (8 players) @ 60 FPS** as per handleInputAndSendUserCmdMove(),  
   - **120 PKT/s with 1 client (2 players) @ 60 FPS** (1 will be by the server by injection though).  
   - Since a PgePacket size was fix 268 Bytes, this lead to:  
     **128 640 Byte/s Packet Data Rate** on server-side with 8 players.  
 - **v0.1.3:**
   - same as v0.1.2 since clients are still storming the server with same pkt rate and pkt size;
 - **v0.1.4:**
   - **128 PKT/s with 7 clients (8 players) @ 60 FPS** as per handleInputAndSendUserCmdMove(),  
   - **32 PKT/s with 1 client (2 players) @ 60 FPS** (1 will be by the server by injection though).  
   This is only the 27% of the packet rate of v0.1.3!  
   - Since **variable packet size** was introduced also in this version in PGE, and MsgUserCmdFromClient is 16 Bytes, PgePacket overhead 15 Bytes so total PgePacket size is 31 Bytes, this leads to:  
     31 \* 128 = **3 968 Byte/s Packet Data Rate** on server-side with 8 players, which is only the 3% of v0.1.3!
 - **v0.1.6.1:**
   - same as v0.1.4, the new features did not affect network traffic.

\subsubsection client_packet_rate Client Packet Rate and Packet Data Rate

In this section we talk about **server -> client** traffic.

Considering 8 players, the results are to a single client from the server:
 - **v0.1.2:**
   - **4 320 PKT/s @ 60 FPS**, with 4320 \* 268 = **1 157 760 Byte/s Packet Data Rate**:
     - 420 PKT/s @ 60 FPS as per handleUserCmdMoveFromClient();
     - 2880 PKT/s @ 60 FPS as per serverUpdateBullets();
     - 540 PKT/s @ 60 FPS as per serverPickupAndRespawnItems();
     - 480 PKT/s @ 60 FPS as per serverSendUserUpdates().
 - **v0.1.3:**
   - **1 720 PKT/s @ 60 FPS & 20 Hz**, with 1720 \* 268 = **460 960 Byte/s Packet Data Rate** (which is only the 40% of v0.1.2 rates):
     - 420 PKT/s @ 60 FPS as per handleUserCmdMoveFromClient();
     - 960 PKT/s @ 20 Hz as per serverUpdateBullets();
     - 180 PKT/s @ 20 Hz as per serverPickupAndRespawnItems();
     - 160 PKT/s @ 20 Hz as per serverSendUserUpdates().
 - **v0.1.4:**
   - **518 PKT/s @ 60 FPS & 20 Hz** (this is only the 12% of v0.1.2 rate!), with 1422 + 11360 + 5500 + 8800 = **27 082 Byte/s Packet Data Rate** (which is only the 2% of v0.1.2 data rate!)  
     (if you set tickrate to 60 Hz in this version, it will be 1518 PKT/s that is still only the 35% of v0.1.2 rate, and 1422 + 34080 + 16500 + 26400 = 78 402 Byte/s Packet Data Rate that is still only the 7% of v0.1.2 data rate!).
     - 18 PKT/s @ 60 FPS as per handleUserCmdMoveFromClient(), with 18 \* 79 = 1422 Byte/s Packet Data Rate (size of MsgCurrentWpnUpdateFromServer is 64 Bytes, PgePacket overhead is 15 Bytes);
     - 0 PKT/s @ 60 FPS as per serverUpdateWeapons() (that was not relevant in previous versions), now it is still not relevant because
                          the rate it could produce is less than handleUserCmdMoveFromClient()'s rate in case of weapon changing, and
                          firing is impossible during weapon changing so handleUserCmdMoveFromClient() is considered in the calculation only.
                          I'm still showing this in the list with 0 rate though.                          
     - 160 PKT/s @ 20 Hz as per serverUpdateBullets(), with 160 \* 71 = 11360 Byte/s Packet Data Rate (size of MsgBulletUpdateFromServer is 56 Bytes, PgePacket overhead is 15 Bytes);
     - 180 PKT/s @ 20 Hz as per serverPickupAndRespawnItems(), with 20 \* 91 + (8\*20) \* 23 = 5500 Byte/s Packet Data Rate
                         (size of MsgWpnUpdateFromServer is 76 Bytes, size of MsgMapItemUpdateFromServer is 8 Bytes, PgePacket overhead is 15 Bytes);
     - 160 PKT/s @ 20 Hz as per serverSendUserUpdates(), with 160 \* 55 = 8800 Byte/s Packet Data Rate (size of MsgUserUpdateFromServer is 40 Bytes, PgePacket overhead is 15 Bytes).
 - **v0.1.5:**
   - **1198 PKT/s @ 60 FPS & 60 Hz Tickrate & 20 Hz cl_updaterate & 60 Hz physics_rate_min** (this is only the 28% of v0.1.2 rate!), with 1422 + 34080 + 16500 + 8800 = **60 802 Byte/s Packet Data Rate** (which is only the 5% of v0.1.2 data rate!).
     - 18 PKT/s @ 60 FPS as per handleUserCmdMoveFromClient(), with 18 \* 79 = 1422 Byte/s Packet Data Rate (size of MsgCurrentWpnUpdateFromServer is 64 Bytes, PgePacket overhead is 15 Bytes);
     - 0 PKT/s @ 60 FPS as per serverUpdateWeapons() (that was not relevant in previous versions), now it is still not relevant because
                          the rate it could produce is less than handleUserCmdMoveFromClient()'s rate in case of weapon changing, and
                          firing is impossible during weapon changing so handleUserCmdMoveFromClient() is considered in the calculation only.
                          I'm still showing this in the list with 0 rate though.                          
     - 480 PKT/s @ 60 Hz as per serverUpdateBullets(), with 480 \* 71 = 34080 Byte/s Packet Data Rate (size of MsgBulletUpdateFromServer is 56 Bytes, PgePacket overhead is 15 Bytes);
     - 540 PKT/s @ 60 Hz as per serverPickupAndRespawnItems(), with 60 \* 91 + (8\*60) \* 23 = 16500 Byte/s Packet Data Rate
                         (size of MsgWpnUpdateFromServer is 76 Bytes, size of MsgMapItemUpdateFromServer is 8 Bytes, PgePacket overhead is 15 Bytes);
     - 160 PKT/s @ 20 Hz as per serverSendUserUpdates(), with 160 \* 55 = 8800 Byte/s Packet Data Rate (size of MsgUserUpdateFromServer is 40 Bytes, PgePacket overhead is 15 Bytes).
 - **v0.1.6.1:**
     - same as v0.1.5, the new features did not affect network traffic.
  
Considering 8 players, the results to ALL clients from the server (because above shows results to 1 client from the server):  
just multiply above results by 7 (server sending to itself avoids GNS level thus we multiply by nClientsCount instead of nPlayersCount):
 - **v0.1.2:** 30 240 PKT/s with 8 104 320 Byte/s Outgoing Packet Data Rate Total;
 - **v0.1.4:** 3 626 PKT/s with 189 574 Byte/s Outgoing Packet Data Rate Total (88% decrease in packet rate and 98% decrease in packet data rate) @ 20 Hz Tickrate  
   (10 626 PKT/s with 548 814 Byte/s Outgoing Packet Data Rate Total that is 65% decrease in packet rate and 93% decrease in packet data rate @ 60 Hz Tickrate);
 - **v0.1.5:** 8 386 PKT/s with 425 614 Byte/s Outgoing Packet Data Rate Total (72% decrease in packet rate and 95% decrease in packet data rate compared to v0.1.2) @ 60 Hz Tickrate & 20 Hz cl_updaterate & 60 Hz physics_rate_min.
 - **v0.1.6.1:** same as with v0.1.5.

\subsubsection detailed_packet_rate Detailed Packet Rate per Function

The detailed explanation of the packet rates of each function is below:

```.cpp
  getNetwork().Update() {
  // no changes since v0.1.2
    while (
      m_pServerClient->pollIncomingMessages() {
        ISteamNetworkingMessage* pIncomingGnsMsg[const nIncomingMsgArraySize = 10];
        const int numGnsMsgs = receiveMessages(pIncomingGnsMsg, nIncomingMsgArraySize);  // so we read max 10 PKTs from GNS each frame (max. 600 PKT/s @ 60 FPS)
        
        for (int i = 0; i < numGnsMsgs; i++) {
          pge_network::PgePacket pkt;
          memcpy(&pkt, (pIncomingGnsMsg[i])->m_pData, nActualPktSize);                   // !!! expensive deep copy !!!
          (pIncomingGnsMsg[i])->Release();                                               // cheap unlink from linked list
          m_queuePackets.push_back(pktAsConst);                                          // !!! expensive deep copy !!!
                                                                                         // AP-99: find a way to link these into m_queuePackets maybe?
        }
      }
    ) {}  // end while
  }
```

```.cpp
  handleUserCmdMoveFromClient() {
      // Process inputs from clients.
      // Here we dont discuss how clients can flood the server, that is discussed in handleInputAndSendUserCmdMove().
      // Instead, here we discuss what packets with what rate are generated here as response to clients!
      // Worst case we need to multiply everything by nClientsCount and by their send rate:
      // - in v0.1.2 most input were sent at their frame rate.
      // - in v0.1.3 things were similar.
      // In v0.1.3, server polled max nIncomingMsgArraySize (= 10) packets / frame: 10 * 60 = 600 PKT/s @ 60 FPS poll, so
      // with nPlayerCount = 8 players, each player sending a message in each frame, that is 8 * 60 = 480 PKT/s @ 60 FPS, so
      // server COULD really poll nPlayerCount number of such message in each frame.
      // That leads to the following OUTGOING packet rate from server to clients in this function:
      //
      //  - in v0.1.3 it might had generated nClientsCount number of MsgCurrentWpnUpdateFromServer, so:
      //    7*7 * 60 = 2940 PKT/s @ 60 FPS total outgoing,
      //    that is 420 PKT/s @ 60 FPS to a single client.
      //    This is true only if clients are constantly changing their current weapons in each frame, which unfortunately can happen if everybody is
      //    using the mouse scroll continuously just for fun and that is detected at framerate at client-side, triggering message to server every frame.
      //
      //    In v0.1.4 we introduced a 500ms minimum elapse time for weapon change, so max 2 weapon changes are allowed per second.
      //    This changes the calculation to:
      //    7*7 * 2 = 98 PKT/s @ 60 FPS total outgoing,
      //    that is 7*2 = 18 PKT/s @ 60 FPS to a single client.
      //
      //    To clarify: even though we said the situation is everyone is shooting, here we will select the weapon change calculation because it generates more
      //    traffic than continuous shooting. Because when shooting is continuous, this function has makes traffic as described below, and it is LESS than
      //    the weapon changing traffic described above.
      //
      //  - before v0.1.4:
      //    might generate 1 MsgWpnUpdateFromServer for updating bullet count for firing (wpn->pullTrigger() returns true), so:
      //    1*7 * 60 = 420 PKT/s @ 60 FPS outgoing total,
      //    that is 60 PKT/s @ 60 FPS to a single client.
      //    This is true only if a shot is actually fired in every frame, that actually cannot happen due to the fastest firing weapon is mchgun with
      //    150 msec firing_cooldown, effectively limiting the value to:
      //    1*7 * 6.7 = ~ 49 PKT/s @ 60 FPS outgoing total,
      //    that is ~6.7 PKT/s @ 60 FPS to a single client.
      //
      //    v0.1.4: this logic has been moved to serverUpdateWeapons() because firing became a continuous operation.
      //
      //  - above cannot really be mixed with each other since after changing weapon, there is a m_nWeaponActionMinimumWaitMillisecondsAfterSwitch = 1000 required
      //    timeout before a bullet can be fired.
  }
```

```.cpp
  serverUpdateWeapons() {
      // This block is at framerate since v0.1.4, previously it was in server tick.
      // The reason of moving this to framerate is that in v0.1.4 attack/mouse left button became a continuous operation, thus player attacking state
      // is saved on server-side until explicitly said so, and need to invoke wpn->pullTrigger() as often as possible which cannot be done simply with
      // tickrate. A fast-firing weapon such as the mchun with ~6 shots/second need to be "triggered" more frequently than tickrate, otherwise some
      // bullets will be fired some millisecs later than expected.
      // And anyway even with a pistol it is not acceptable if client pulls trigger at time point T but server actually creates bullet at T + (50-16.6) msec
      // later (worst case).
      
      // v0.1.4: Might generate 1 MsgWpnUpdateFromServer for updating bullet count for firing (wpn->pullTrigger() returns true), so:
      // 1*7 * 60 = 420 PKT/s @ 60 FPS outgoing total,
      // that is 60 PKT/s @ 60 FPS to a single client.
      // This is true only if a shot is actually fired in every frame, that actually cannot happen due to the fastest firing weapon is mchgun with
      // 150 msec firing_cooldown, effectively limiting the value to:
      // 1*7 * 6.7 = ~ 49 PKT/s @ 60 FPS outgoing total,
      // that is ~6.7 PKT/s @ 60 FPS to a single client.
      
      // Might send nClientsCount number of MsgWpnUpdateFromServer, currently only for reloading a weapon:
      // max. 7 * 60 = 420 PKT/s @ 60 FPS total outgoing to clients,
      // that is max 1 * 60 = 60 PKT/s @ 60 FPS to a single client.
      // However reloading is also rate-limited by reload_time weapon cvar, that is 1500 msecs currently for both pistol and mchgun.
      // Thus this is not considered yet. Later, if per-bullet-reloadable weapons will be available, we will consider this again.
      // Note that if a reload_time is less than or close to tickrate, it won't be properly updated as intended. That is also a reason why this function
      // is not in the server tick anymore.
  }
```

```.cpp
  serverUpdateBullets() {
      // Until v0.1.3 server continuously sent all bullet movements to clients.     
      // v0.1.3 sends nBulletsCount number of MsgBulletUpdateFromServer to ALL clients:
      // 48*7 PKT * 20 Hz = 6720 PKT/s @ 20 Hz total outgoing,
      // that is 960 PKT/s @ 20 Hz to a single client.
      //
      // But from v0.1.4 it just sends msg about new bullet or bullet delete, clients also simulate the bullet travel so it doesn't eat network traffic.
      // v0.1.4 sends nPlayerCount number of MsgBulletUpdateFromServer to ALL clients in case everyone is shooting a new bullet in each tick, or
      // in case 1-1 bullet of each player hits something so needs to be deleted. Traffic is reduced to:
      // 8*7 PKT * 20 Hz = 1120 PKT/s @ 20 Hz total outgoing,
      // that is 160 PKT/s @ 20 Hz to a single client.
  }
```

```.cpp
  serverPickupAndRespawnItems() {
      // might send nClientsCount number of MsgWpnUpdateFromServer and (nMapItemsCount * nClientsCount) number of MsgMapItemUpdateFromServer:
      // max. (7 + 25*7) PKT * 20 Hz = 3640 PKT/s @ 20 Hz total outgoing,
      // that is 26 PKT * 20 Hz = 520 PKT/s @ 20 Hz to a single client.
      // However very unlikely that all players are picking up an item in every tick AND every item is respawning also in every tick.
      // The realistic is that per tick as many items are respawning as the number of players since those players could pick up only 1 item at a time.
      // So I would rather calculate with: might send nClientsCount number of MsgWpnUpdateFromServer and nPlayerCount number of MsgMapItemUpdateFromServer:
      // (7 + 8) PKT * 20 Hz = 300 PKT/s @ 20 Hz total outgoing,
      // that is (1 + 8) PKT * 20 Hz = 180 PKT/s @ 20 Hz to a single client.
  }
```

```.cpp
  serverSendUserUpdates() {
      // max. nPlayerCount number of MsgUserUpdateFromServer to ALL clients (and to server by injection though):
      // 8*7 PKT * 20 Hz = 1120 PKT/s @ 20 Hz total outgoing,
      // that is 8 PKT * 20 Hz = 160 PKT/s @ 20 Hz to a single client.
  }
```

```.cpp
  handleInputAndSendUserCmdMove() {
      // Here we talk about how clients can flood the server.
      // Might send 1 MsgUserCmdFromClient to server in case of input.
      // We dont want to storm the server at FPS rate, so here we introduced ways of rate-limiting client input in later versions.
      //
      // v0.1.2: didnt do any rate-limiting. Thus with clients constantly generating input, that means nPlayerCount number of MsgUserCmdFromClient:
      // 1*8 * 60 = 480 PKT/s @ 60 FPS total incoming to server (1 will be by the server by injection though),
      // that is 1*60 = 60 PKT/s @ 60 FPS from a single client.
      //
      // v0.1.3: introduced the concept "continuous operation", it means: once you set the action, server keeps doing the action until explicitly said to stop.
      // This is very handy from physics simulation perspective as well since server can simulate something precisely without continuously telling it to do so,
      // and this also greatly decreases client -> server traffic.
      // Strafe has been changed to be such continuous operation.
      // But due to a bug the traffic was not decreased.
      // 
      // v0.1.4: strafe pkt traffic got fixed so now really 1 PKT is sent per strafe-state-change!
      // v0.1.6: crouching added as new action as continuous-op like strafe. However, the packet size did not change and we dont need to consider higher frequency
      //         than what we already considered with strafing, calculations don't need to be changed.
      //
      // Some operations don't need to be set as continuous operation because they are triggered by keyup-keydown pairs (controlled by getKeyboard().isKeyPressedOnce())
      // thus cannot flood the server by simply pressing the relevant buttons continuously: jump, toggleRunWalk, requestReload, weapon switch.
      // Before v0.1.4: so these are rate-limited at client-side implicitly by the fact that there is a physical limit how many times a player can do key-down-key-up
      // series within a second, for now we can say worst-case 5/sec, that is so low I'm not considering now.
      // However, weapon changing by keyboard, currently for switching between pistol and mchgun back-and-forth, could reach worst-case 15 PKT/sec in v0.1.3.
      //
      // v0.1.4: a 500 millisecs time elapse is also required between wpn change keystrokes thus I believe switching back-and-forth is now worst-case 5 PKT/sec.
      //         Also introduced rate-limit for weapon changing by mouse scrolling which was still at framerate in v0.1.3.
      //         Wpn change rate by mouse scroll or keyboard are measured together so they are worst-case 5 PKT/sec even if you are mixing them.
      //         Other actions such as run toggling, jumping, wpn reload also got the 500 millisecs rate-limiting at client-side.
      //         All the mentioned rate-limits are also enforced at server-side, thus storming the server programmatically won't lead to any benefit.
      //
      // AP-99: drop clients who are storming the server with too high rate (maybe programmatically).
      //
      // v0.1.4: attack i.e. left mouse button press also became a continuous operation like strafe, so from now only the press and release generate packets towards server.
      //         So now the more clicks, the more packets: worst-case 5 PKT/sec for 1 rapidly clicking player.
      //         With nPlayerCount number of MsgUserCmdFromClient:
      //         8 * 5 = 40 PKT/s @ 60 FPS total incoming to server (1 will be by the server by injection though),
      //         that is 1 * 5 = 5 PKT/s @ 60 FPS from a single client.
      //
      // v0.1.4: mouse/xhair moving-induced player- and weapon-angle update sending is also rate-limited.
      //         Because the same message type is used for updating weapon angle and attacking state, the exact weapon angle will be sent to server at the moment of starting the attack, so
      //         the bullet born on server-side is expected to have the proper same angle.  
      //         However, if too low rate is set and the client is rapidly changing weapon angle during continuous shooting with an automatic weapon like mchgun, some newborn bullets might have
      //         a bit different angle on server-side compared to the client-side angle of the weapon. To avoid this, we cannot set too low rates for this.
      //         Originally I used 200 millisec intervals for player angle and bigger (more than 30 degrees) weapon angle changes, and 300 millisec for smaller changes, which shows noticeable
      //         artifacts of this kind.
      //         So for now I use 100 instead of 200 millisec, and use 200 instead of 300 millisec intervals.
      //         Note that I could also do something like dynamically switching to higher send rate whenever the user is attacking, and switch back to lower when user is not attacking.
      //         But for now I just stick to the fixed higher rates because the number of sent packets is still reasonable I think.
      //         AP-99: introduce unreliable sending of packets so that they are always sent out to server continuously, in that case weapon angle could be sent with higher rate too!
      //         This means max ~16 PKTs, so:
      //         1*8 * 16 = 128 PKT/s @ 60 FPS total incoming to server (1 will be by the server by injection though),
      //         that is 1 * 16 = 16 PKT/s @ 60 FPS from a single client.
  }
```

\section future_plans Future Improvement Plans

\subsection future_plans_server Server Future Improvement Plans

Currently server in every tick invokes serverSendUserUpdates() that sends out MsgUserUpdateFromServer to all players about the state of a player if any state is dirty.  
Because this is happening frequently, **we could use unreliable connection for this instead of reliable** to reduce overhead of reliable connection, even though I'm not sure about the amount of the overhead.  
Note that reliable and unreliable are 2 different ways of sending messages using GNS, and none of them is using TCP.  
**Both use UDP.**  
The difference is that **reliable messages are automatically retransmitted in case of loss, can be expected to be received exactly once, and their order is guaranteed to be same on the receiver side as on the sender side**.  
Some info about the [message segment differences here](https://github.com/ValveSoftware/GameNetworkingSockets/blob/master/src/steamnetworkingsockets/clientlib/SNP_WIRE_FORMAT.md).  
Because loss of such message would NOT be a big problem. But it should be done as QuakeWorld/Counter-Strike does: even if player data is NOT dirty, the state is sent out.  
This can overcome the inconsistency issues caused by packet loss: if a packet is missing, no problem, the next packet will bring fresher data anyway.  
However, at this point I'm still not convinced if I should start experimenting with this though.  
At the same time I'm also thinking that there is no packet loss in small LAN environment.

**How server code should work without client-side prediction**:  
Remember that with the naive approach, we immediately processed the messages received from clients.  
We don't do this anymore, since server also has tickrate and we should stick to it: game simulation and input sampling is happening in each tick, not in each frame.  
So whenever a client input message is received on server-side, instead of processing it immediately, we enqueue it.  
Server dequeues all received messages at its next tick, and responses will be also sent out at this time in the separate serverSendUserUpdates().  
Remember: the lower the value of **cl_updaterate**, the more we depend on client-side lerp and input-prediction to smooth out player movement experience.

Interesting fact: the original Doom [used P2P lockstep mechanism multiplayer](https://www.gabrielgambetta.com/client-side-prediction-server-reconciliation.html), which at that time was not good to be played over the Internet.  
Then Quake introduced the client-server model with the client-side lerp, which was good for LAN, but less good on Internet with bigger distances between machines.  
So they introduced client-side prediction in QuakeWorld.

\subsection future_plans_client Client Future Improvement Plans

On the internet you can read that: in every tick (instead of every frame), player input is sampled and sent as a message to the server.  
I already described this earlier why I think this is not good as it introduces noticable delay:  
with framerate 60 versus tickrate 20, a keypress might be sent to server (1000/20) - (1000/60) = ~33 milliseconds later.  
And this latency would be on client-side, that would be added to latency between client-server. I think that would be NOT acceptable.  
Maybe later I will change my mind.

Another improvement would be: we don't even need to send messages in every tick, we can just further enqueue messages over multiple ticks, and send them at lower rate than tickrate, to further reduce required bandwidth.  
This lower rate is called **command rate**, rule is: **tickrate >= command rate**.  
As optimization, we could send these client messages in 1 single packet to the server, since sending each message in different packet introduces too high overhead.  
They say that the ["maximum safe UDP payload is 508 bytes"](https://stackoverflow.com/questions/1098897/what-is-the-largest-safe-udp-packet-size-on-the-internet).  
GNS also uses UDP under the hood.  
As of June of 2023 (in PRooFPS-dd v0.1.2 Private Beta), size of PgePacket was 268 Bytes, room for application message (MsgApp struct) was 260 Bytes.  
Size of a MsgUserCmdMove struct was 20 Bytes, which means that by implementing placing multiple messages into a single packet we could send more than 10 such messages in a single PgePacket.  
Even though I'm not sure what is the size of the whole packet/message sent by GNS, I'm pretty sure it is still below this 508 Bytes when being added to the size of a PgePacket.

Due to the low tickrate (e.g. 50 ms, meaning 20 ticks per second), the **player movement can appear choppy and delayed**.  
This is why we need some tricks here:
 - either client-side lerp (linear interpolation);
 - and/or client-side prediction.

With **client-side lerp**, the last received player coordinate (from server) is cached, the player object is NOT YET positioned to that coordinate.  
Instead, the player object is moved between its current position and the cached position using linear interpolation. The object position is updated in every rendered frame.  
This way the **movement of player object will be continuous even though we receive updated positions less frequently. This removes the choppiness but delay will remain.**  
Note that we apply this technique for all player objects at client-side, and also at server-side.  
We have to be careful though, because **this introduces a bit of lag**, due to player object position will be always some frames behind the cached server position.  
So it is better to do the interpolation fast to keep object position close to the cached position i.e. keep interpolation time **cl_interp** as a small value.

Note that it might be a good idea to cache not only the latest but the 2 latest positions received from server, and set the lerp time to be as long as twice the delay between 2 updates received from server.  
For example, if tickrate is 20 i.e. delay between updates from server is 50 ms, we can set lerp time cl_interp to 2x50ms, so if 1 update is dropped for any reason, the lerp can still continue as it is not yet finished anyway.

TODO: add debug CVAR that can show the cached/server position on client side of objects so we can see the delay compared to server.  
I'm expecting the player object to be delayed relative to the debug box with the lerp, but ahead with the client-side prediction!

With **client-side prediction**, we don't need to use lerp for the current player, because **we don't wait for server response for the client's user input**.  
We keep lerp only for other players' objects.  
This is a fundamentally different approach because we discussed earlier that clients always wait the response from the server.  
With this approach we move our player object immediately based on local input, and send the usual message to server.  
We need to introduce a unique message index in the sent message and the response message as well. This index to be used later when processing response from server.
Client also saves the sent messages to a queue along with the calculated player object position, because it will need them later when it receives response from server.  
Server will respond back as usual, and upon receiving the new coordinates, we check if the predicted values are correct: the truth is always what server responds.  
We can dequeue the stored messages having message index less than or equal to the message index present in server response, and if there is difference in player position in server response compared to the player position in the enqueued message with same index, we align player object position to what server has just responded to us and we replay the remaining stored messages at client-side so that the player object will be correctly positioned based on server's latest confirmed state.  
Note that obviously we don't need to send the replayed messages again to server, since those commands were already sent to server earlier, we will get response for them too a bit later from the server.  
This way **server remains the only authoritive instance in the network**, but we let clients see themselves a bit ahead in time compared to the server, and hopefully there will be only rare occasions when we need to correct the predicted positions at client side.  
["The client sees the game world in present time, but because of lag, the updates it gets from the server are actually the state of the game in the past. By the time the server sent the updated game state, it hadn’t processed all the commands sent by the client."](https://www.gabrielgambetta.com/client-side-prediction-server-reconciliation.html)
Note that this approach also means that clients also have to simulate physics, otherwise they cannot properly predict new player positions, e.g. they need to do collision check against walls.  

\section cs_1_6_rates_explained Counter-Strike 1.6 Rates Explained

Following CVARs (config variables) are available for tweaking networking in CS 1.6:
 - **tickrate** - rate at which the server is running simulation, e.g. 20 means 20 ticks per second i.e. in every 50 ms. This value is also used on client side, there this is the rate the client is processing user input.
 - **rate** - ingress bandwidth of client (client tells this to server). Bytes/sec. Half-Life 1/CS actually has a maximum rate of 20000 Bytes/sec.
 - **sv_lan_rate** - same as rate but on LAN.
 - **cl_updaterate** - rate at which the server is sending snapshot updates to the client. It cannot be higher than **tickrate** and cannot exceed bandwith specified by **rate**.
 - **cl_cmdrate** - rate at which client is sending user input packets to the server. Note that although user input is processed at **tickrate** rate, the command packets are sent out at **cl_cmdrate rate**, which means that multiple commands might be tied together into a single packet.
 - **cl_interp** - client-side interpolation period: interpolating entity positions between 2 snapshots so that if there is too big time difference between 2 snapshots it will still look continuous.
 - **cl_extrapolate** - extrapolation is used if interpolation cannot be done due to too many lost snapshots.
 - **cl_predict** - turn on/off client input prediction.
 - **cl_smooth** - turn on/off client-side input prediction error smoothing.
 - **sv_showhitboxes** - if enabled, clients will draw the hitboxes used on server side. They are expected of ahead of the player by the lerp (linear interpolation) period.

Details:
 - https://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking
 - https://steamcommunity.com/sharedfiles/filedetails/?id=126383209

\section networking_pages Web Pages about this Topic

\subsection low_level_networking_pages Web Pages about Low-level Networking Topics

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

\subsection high_level_networking_pages Web Pages about High-level Networking Topics

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



