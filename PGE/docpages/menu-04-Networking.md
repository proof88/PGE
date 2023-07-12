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
Here is a **small game project** utilizing **GSN**: https://github.com/David-Elle-Jack-Oisin/TeamProject/tree/main/src/network

\section client_server Client and Server

I'm implementing the **client-server** networking model in the engine because it sounds suitable for my future projects and hopefully easier to implement than P2P.  
In this model, there is always exactly 1 machine with server role, and some clients connected to the server.  
There is no communication between clients, server keeps all clients updated about game state.

\subsection pge_network_access Access to Networking Functionality

PGE provides networking functionality through the PgeNetwork class instance in pge_network namespace, that can be accessed by PGE::getNetwork() function.  
This PgeNetwork instance must be initialized before networking functionality can be used, however PGE initializes it automatically upon startup.  
It will be either a client or server instance based on the value of CVAR_NET_SERVER.  
Common functions can be accessed through PgeNetwork::getServerClientInstance(), more specifics are accessed through either PgeNetwork::getClient() or PgeNetwork::getServer() functions.

TODO: ask doxygen to insert value of CVAR_NET_SERVER above!

TODO: make a separate page explaining CVARs!

\subsection pge_network_packets Packets and Messages

Server and client instances communicate by sending and receiving PgePacket structs that encapsulate single messages.  
As of PGE v0.4 there can be only 1 message within a PgePacket, and there is only 3 different kind of messages:
 - MsgUserConnected;
 - MsgUserDisconnected;
 - MsgApp.

An application can define its custom messages, all those will be MsgApp kind.

TODO: make link to tag PGE v0.4 above!

TODO: should we use pge_network namespace everywhere explicitly?

\subsection pge_network_allowlisting Allowlisting

In order to be able to receive messages over the network, the messages need to be explicitly allowlisted, otherwise they are ignored.  
By default server instance is allowed to receive MsgApp messages only, client instance is allowed to receive MsgUserDisconnected and MsgApp messages.  
Server also processes MsgUserConnected and MsgUserDisconnected, however it just self-injects these, that is why it is NOT allowed to receive these over network.  

It is very important to understand that even though MsgApp is allowed for both client and server, **by default any kind of custom message inside MsgApp will be ignored**.  
**Application MUST explicitly allowlist its custom messages separately for both client and server.**  
This can be done by adding the allowed custom messages to the container accessed by PgeNetwork::getServerClientInstance().getAllowListedAppMessages().

\subsection multiplayer_cheating Cheating in Multiplayer

There are many ways to cheat in multiplayer games, and PGE doesn't provide protection against it.  
However, a good implementation in application level can overcome some of the forms of cheating.  
A common approach is to treat the **server as the only authorative instance** and make clients show only a replication of server state.  
For example: player movement. When a player presses a button to move, it should send a request/command message to the server about the keypress, and the server calculates the actual movement of the player.  
Then it replies back to the client(s) with the updated position of the player who requested the move, client(s) receive(s) the reply and move(s) the player to the position calculated by the server.  
Since server takes care of the entire game state, simulate physics, calculate new positions, etc. and replicates game state to clients, it is the only authorative element of the multiplayer game session.  
This way it is more difficult for clients to do anything from an illegal position, e.g. put themselves out of map bounds intentionally because always the server calculates their position based on client inputs that can be rejected as well.

\subsection original_naive_impl Original Naive Implementation

PGE currently does not give explicit support on features like client-side prediction, it just gives a basic framework to establish connections between clients and the server as described above.  
However, my game [PRooFPS-dd](https://github.com/proof88/PRooFPS-dd) implements these features so I'm giving some words about this topic here.

Until [PRooFPS-dd v0.1.2 Private Beta](https://github.com/proof88/PRooFPS-dd/releases/tag/v0.1.2-PrivateBeta) my naive approach was to tie input sampling to rendering frame rate and send messages between server and clients as soon as input was detected.  
As already explained above, in general it is good to select the server as the only authorative instance in the network to provide a basic implementation against cheating.  
So in my naive approach, when client player pressed a button to move, it did not move the player object, just sent a message to server about the input.  
The server processed the message and replied back as soon as possible.  
The updated player positions were in server's response, so on client side I updated the player position upon receiving the response from the server.

This approach looks good if you have high frame rate (e.g. 60 FPS) because client request and server response messages happen very fast, so player movement looks smooth.  
However, it has multiple downsides as well:
 - because sending and processing messages is tied to rendering framerate, so player movement speed will heavily rely on the framerate;
 - the higher the framerate is, the more messages will be sent across computers, that could overload a machine having lower framerate (i.e. packet buffer might get full leading to packet drops).

Although the first issue could be solved by calculating new player position based on measured delta time elapsed since last update instead of using constant values, we would still have the second issue.  
Although physics is not part of this page, in general the variable delta time-based physics is not a good approach anyway because of multiple reasons (e.g. different machines with different delta will calculate different floating point results) so I've implemented fixed delta time approach, more you can read about it:
 - here: https://fabiensanglard.net/timer_and_framerate/index.php
 - and here: https://gafferongames.com/post/fix_your_timestep/

\subsection new_tick_based_implementation New Tick-Based Implementation

Now back to the networking part.  
From now on the techniques I'm describing are very common in multiplayer games and the terms I'm using are same or very similar as in [Counter-Strike](https://developer.valvesoftware.com/wiki/Source_Multiplayer_Networking).

We want framerate-independent player movement, so we have our framerate-independent physics implemented as well, we should tie the input sampling, physics and messaging together, so they will be done with a different rate than the framerate.  
These things (input sampling, game state update, simulation, physics, messaging) tied together we call **tick**.  
This different rate we call **tickrate**.

So the **essence of moving away from the naive approach is to understand that we execute different part of core game code at different rates**:
 - **framerate**: frequency of frame rendering;
 - **tickrate**: frequency of ticks i.e. input sampling, game state update and messaging together.
 
Another rule is that **framerate > tickrate**.  
So it is totally ok to have framerate 60 while having tickrate 20. This means that we maximize the number of iterations of main game loop at 60, we target 60 rendered frames every second, and we do 20 ticks every second.

\subsection client_behavior New Client Behavior

With the tickrate introduced, we successfully solved the problem of a slower machine might not be able to keep up with processing messages when faster machines are also present in the network.  
The implementation changes like this:  
In every tick (instead of every frame), player input is sampled and sent as a message to the server.  
It is important to understand that we should not only send out message when a button is pressed but also when it is released.
Player is not moved in the moment of detected player input, because player is moved only when server responds with the updated coordinates.  

Note: we don't even need to send messages in every tick, we can just further enqueue messages over multiple ticks, and send them at lower rate than tickrate, to further reduce required bandwidth.  
This lower rate is called **command rate**, rule is: **tickrate >= command rate**.  
However, this change is optional to be implemented.

Note: we can send these client messages in 1 single packet to the server as an optimization, since sending each message in different packet would introduce too high overhead.  
They say that the ["maximum safe UDP payload is 508 bytes"](https://stackoverflow.com/questions/1098897/what-is-the-largest-safe-udp-packet-size-on-the-internet).  
As of June of 2023 (in PRooFPS-dd v0.1.2 Private Beta), size of PgePacket was 268 Bytes, room for application message (MsgApp struct) was 260 Bytes.  
Size of a MsgUserCmdMove struct was 20 Bytes, which means that by implementing placing multiple messages into a single packet we could send more than 10 such messages in a single PgePacket.

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

TODO: I should also check if GNS automatically re-sends lost messages or not? Is the order of messages is guaranteed?

\subsection server_behavior New Server Behavior

**Without client-side prediction**:
Remember that with the naive approach, we immediately processed the messages received from clients.  
We don't do this anymore, since server also has tickrate and we should stick to it: game simulation and input sampling is happening in each tick, not in each frame.  
So whenever a client input message is received on server-side, instead of processing it immediately, we enqueue it.  
Server dequeues all received messages at its next tick, and responses will be also sent out at this time in the separate sendUserUpdates().  
Alternatively, we don't need to send out response/update messages with the tickrate frequency, we can have a separate frequency for that, called **cl_updaterate**.  
Rule is: **tickrate >= cl_updaterate**. We can lower the required bandwidth if we set a lower value. The lower the value, the more we depend on client-side lerp and input-prediction to smooth out player movement experience.

Interesting fact: the original Doom [used P2P lockstep mechanism multiplayer](https://www.gabrielgambetta.com/client-side-prediction-server-reconciliation.html), which at that time was not good to be player over the Internet.  
Then Quake introduced the client-server model with the client-side lerp, which was good for LAN, but less good on Internet with bigger distances between machines.  
So they introduced client-side prediction in QuakeWorld.

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



