# PGE
PR00F's Game Engine

This C++ open-source game engine is being actively developed to make my games with it.  
My primary game project is [PRooFPS-dd](https://github.com/proof88/PRooFPS-dd).

[Online documentation](https://proof88.github.io/).

PGE uses the following open-source libraries:
 - [Dear ImGUI](https://github.com/ocornut/imgui) for GUI;
 - [SoLoud](https://github.com/jarikomppa/soloud) for Audio;
 - [GameNetworkingSockets](https://github.com/ValveSoftware/GameNetworkingSockets) for Networking.

## History

Since the PURE graphics engine is part of the PGE game engine, their versioning is hard-tied and incremented together.  
So whenever you read something like `PGE vA.B` it always implicitly means `PURE vA.B` and vice versa.

### v0.5 (TBD)

### v0.4 (Dec 19, 2024)

**v0.4 Requires:**
 - [PFL v1.2](https://github.com/proof88/PFL);
 - [CConsole v1.3](https://github.com/proof88/Console).

During this 3 years period since the previous release, all engine developments were on-demand as required by [my game PRooFPS-dd](https://github.com/proof88/PRooFPS-dd) which had the primary focus.

Change list:
 - started writing **engine doc** in mediawiki format, and occasionally [an online html version](https://proof88.github.io/pge-doc/index.html) is generated from that;
 - build: **windows.h** is now not included anywhere directly, instead [PFL/winproof88.h](https://github.com/proof88/PFL/blob/master/PFL/winproof88.h) is used with fine-grained control of exactly what portions we really include from windows.h and relevant headers;
 - build: **UnitTest.h** has been moved to a separate repo: [455-355-7357-88](https://github.com/proof88/455-355-7357-88);
 - engine loop frequency, also called as **target FPS** can be set now, see `PGE::setGameRunningFrequency()`;
 - added `PGE::setCookie()` and revised shutdown for supporting **application restart** without starting a new process;
 - added a basic **PgeOldNewValue** class for holding arbitrary-typed variable with an old and a new value, revert/commit/isDirty() functions, very useful to decide if there is anything we need to send to network by the end of the game loop;
 - added new classes **Bullet, Weapon and WeaponManager** that can be used in any future games for **handling weapons**;
 - added **PgeObjectPool** for rapid creation/deletion of repeated game objects such as particle objects or bullets;
 - input: added **mouse button click** handling in **PureWindow**;
 - input: added `PGEInputKeyboard::isKeyPressedOnce()` for handling single **keypress-and-release actions** instead of continuous keypress actions;
 - input: added optional `nFilterMillisecs` argument to `PGEInputKeyboard::isKeyPressed()` to set minimum required time between 2 same consecutive keypress;
 - config: added **PGEcfgFile** for config file handling that can be used in any future games for **handling user configuration**;
 - config: added **PGEcfgProfiles** for user profile handling, on top of PGEcfgFile;
 - network: included [Valve's GameNetworkingSockets library](https://github.com/ValveSoftware/GameNetworkingSockets) and added **PgeNetwork, PgeServer, PgeClient** classes for basic client-server model **multiplayer** handling in any future games;
 - audio: included [SoLoud](https://github.com/jarikomppa/soloud) and added **PgeAudio** class to be used in any future games for handling sound effects and music;
 - graphics: all "PRRE" text has been renamed to "Pure" within functions, filenames, variable names, etc.;
 - graphics: GUI: included [Dear ImGui](https://github.com/ocornut/imgui) to be used in any future games for handling **graphic user interface**, exposed over the original PureUiManager class;
 - graphics: GUI: **PureUiText** is now considered **deprecated** but kept to stay compatible with older applications such as the legacy PR00FPS from 2007;
 - graphics: GUI: added **drop shadow feature for PureUiText**;
 - graphics: added **PureProjection** class to be used for **2D->3D unprojection or 3D->2D projection** calculations in any future games;
 - graphics: added `PureMaterial::setDecalOffset()` to support rendering of **decals on surfaces** such as bullet holes on walls.

### v0.3 (Nov 1, 2021)

[Only PURE graphics engine improvements](https://proof88.github.io/pure-doc/version_history.html) : occlusion culling, depth sorting, frame stats.

**v0.3 Requires:**
 - [PFL v1.1](https://github.com/proof88/PFL);
 - [CConsole v1.1](https://github.com/proof88/Console).

### v0.2 (Mar 31, 2021)

[Only PURE graphics engine improvements](https://proof88.github.io/pure-doc/version_history.html) : refactor, hardening, bug fixes, documentation, 1st performance demo.

### v0.1 (Apr 20, 2019)

The aim of the first release was to be able to run the [legacy PR00FPS game](https://github.com/proof88/PR00FPS) with the new PURE graphics engine without touching the original game code.  
For this a [wrapper library](https://github.com/proof88/gfxcore2) was also written that translates the legacy API calls into new API calls.

The Visual Studio project file is included.<br/>
However, if you want to **build** or see example of integration in other projects, you should have the Visual Studio solution file including other relevant projects as well in [PGE-misc](https://github.com/proof88/PGE-misc) repo. **Follow the build instructions** in [PGE-WoW.txt](https://github.com/proof88/PGE-misc/blob/master/src/PGE-WoW.txt).
