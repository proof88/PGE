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

### v0.4 (TBD)

TBD  

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
