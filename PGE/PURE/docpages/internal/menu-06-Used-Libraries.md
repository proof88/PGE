\page pure_used_libs Used 3rd Party Libraries

[TOC]

This page lists the 3rd party libraries used primarily by PURE.  
Additional 3rd party libraries used primarily by PGE are listed in [PGE's own documentation](https://proof88.github.io/pge-doc/index.html).

When choosing a 3rd party library to be used in my project, there are some **general requirements that must be satisfied by the lib**:
 - it shall be cross-platform, including Android (because I have plans for releasing to Android phones);
 - its license shall allow me to use it with my GPL-3.0/LGPL-3.0 software;
 - it shall be still maintained, so that at least severe security issues get fixed over time, and also I can apply stricter compiler warning/error settings when building the lib with recent C++ compilers;
 - unit tests and/or other tests are there so I can see the developer is doing some form of automated verification;
 - there are some actual users of the library, so I'm not the first human trying to actually use it;
 - C++ interface is highly appreciated over C.
 
So, for example, if there is a nice image loading library, working fine but not updated since 2004, then sorry but it is a no-go for me.

\section pure_used_libs_gui GUI

Under GUI, I mean the HW-accelerated 2D graphical user interface in the rendering area, the usual elements like text, combo box, list box, buttons, etc.  
I don't discuss the platform windowing here because that is elaborated in the next section.  
GUI has its own [sub-page within PURE documentation where I also cover the chosen library](#gui).

Note: GUI and windowing libs need to be compatible with each other because windowing libraries provide platform-independent input handling as well, and the GUI elements managed by the GUI libs take input from the windowing library's input handling.  
Therefore, it is recommended to select a windowing library which is supported by your selected GUI lib, or vice versa.

\section pure_used_libs_windowing Windowing

To render something, PURE requires a window.  
To be more detailed, a window is required by PURE not only for rendering, but also by PGE to handle user input.  
Before v0.5, `PureWindow` had WinAPI-based implementation to create and manage such window, and using this, `PGEInputHandler` handled keyboard- and mouse input.  
This was not platform-independent, so I decided to find a better solution.

TODO: add sequence of selecting display mode and creating window, with extra step when MSAA is requested!
TODO: add what commands I use for beginning a frame and ending a frame, including swapbuffers, glflush.

TODO: add [freeglut](https://github.com/freeglut/freeglut)

**My requirements for windowing libraries:**
 - shall support **creating window for 3D-acceleration** (e.g. on Windows, the selected [pixel format shall have PFD_GENERIC_ACCELERATED, PFD_SUPPORT_OPENGL and PFD_DOUBLEBUFFER flags](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-pixelformatdescriptor#members)) which implies it shall support selecting proper pixel format as well;
 - shall support pixel format supporting **MSAA**, implying that [WGL pixel format](https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt) can be also selected;
 - shall support **setting display mode** including screen resolution, screen refresh rate, color depth, z-buffer depth, stencil buffer depth;
 - shall support enabling/disabling **vertical synchronization** or let me control it using WGL;
 - shall support both **resizable-bordered and borderless fullscreen windows**;
 - shall support handling **screen DPI**: [[link 1]](https://learn.microsoft.com/en-us/windows/win32/learnwin32/dpi-and-device-independent-pixels#dpi-aware-applications), [[link 2]](https://learn.microsoft.com/en-us/windows/win32/hidpi/setting-the-default-dpi-awareness-for-a-process), so my game window and its contents won't be disturbed by different DPI settings;
 - shall support **input handling**, including [raw mouse input](https://learn.microsoft.com/en-us/windows/win32/inputdev/about-raw-input) so my games can process precise mouse movements, independent of screen resolution, and can take advantage of high performance gaming mouses.
 
I don't care about **"fullscreen exclusive mode"** [because it is not an explicit thing](https://www.gamedeveloper.com/programming/fullscreen-exclusive-is-a-lie-sort-of-), and also from Windows 10, it is automatic:  
*"we enhanced the DWM to recognize when a game is running in a borderless full screen window with no other applications on the screen. In this circumstance, the DWM gives control of the display and almost all the CPU/GPU power to the game. Which in turn allows equivalent performance to running a game in FSE. Fullscreen Optimizations is essentially FSE with the flexibility to go back to DWM composition in a simple manner".*  
[[link to source]](https://devblogs.microsoft.com/directx/demystifying-full-screen-optimizations/)

**I considered these libs:**
 - [SDL](https://github.com/libsdl-org/SDL)
 - [SFML](https://github.com/SFML/SFML)
 - [GLFW](https://github.com/glfw/glfw)

All of them are cross-platform, widely used, and still maintained.  
SDL and SFML provide much more: image file loading, audio support, networking, etc.  
If you do not use any libs yet for these purposes, I recommend selecting either SDL or SFML so later you can benefit from other features too.  
Otherwise GLFW might be just fine for you.

As of December 2024, all 3 libs meet the abovementioned requirements.  
Some of them might not be as good as the other from screen DPI handling perspective though but at least some support is there anyway.  
A small note: [TGUI coincidentally also compares these 3 libs in the context of DPI awareness and MSAA](https://tgui.eu/tutorials/latest-stable/dpi-scaling/).

My selected GUI library [is Dear ImGui](#gui), it officially has support for SDL and GLFW, but not for SFML.  
There is an [unofficial repo adding SFML support to Dear ImGui though](https://github.com/SFML/imgui-sfml).  
Note: if you use [TGUI](https://github.com/texus/TGUI/) then it has support for all 3 windowing libraries though.  
Android is currently NOT officially supported by GLFW ([but some work is already being done](https://github.com/glfw/glfw/issues/1726)).

**So, from all the officially supported features, SDL seems to be the winner (for me), however I might come back to this topic in 2025 when I finally start using one of them.**

\section pure_used_libs_hw_detect_renderer HW Detection, Part 1: Renderer

\subsection pure_used_libs_hw_detect_renderer_ogl_extensions Rendering HW Features, OpenGL Extensions

Properly detecting the rendering hardware is important because based on that the engine can decide what rendering features it can use.  
**Features detection** in OpenGL is done by **parsing OpenGL extensions**.  
PURE v0.4 parses the supported extensions and also does an extensive per-version and per-feature check, so the user can use the `PureHwVideo` class to understand the capabilities of the current rendering hardware.  
PURE does this by not only expecting the presence of specific extensions strings but also expecting the related function pointers and in some cases it also compares some queried values to an expected range.  
Since I'm proud of this and did not see anything else similar with this level of verification (except probably [GLEW](https://github.com/nigels-com/glew)), I want to keep this behavior.   
BUT getting the function pointers for the extension-specific functions might need to be done in different way on desktop Windows than on Linux and on mobile devices with OpenGL ES.  
For this, **I also need a platform-independent solution in the form of a 3rd party lib for extension handling**, and `PureHwVideo` will also rely on that lib and on my existing per-feature checking implementation.  
I have not yet done too much study on this but the following libs came into scope so far:
 - [GLEW](https://github.com/nigels-com/glew)
 - [Glad](https://github.com/Dav1dde/glad)
 - [libepoxy](https://github.com/anholt/libepoxy)
 - [glbinding](https://github.com/cginternals/glbinding)

[GLEE](https://sourceforge.net/projects/glee/) is not really maintained anymore. Might be good short-term but long-term might not be enough.

PRooFPS-dd has 2 tickets that will need to be closed once this work is done: [ticket 1](https://github.com/proof88/PRooFPS-dd/issues/28), [ticket 2](https://github.com/proof88/PRooFPS-dd/issues/30).

**TODO: conclude on the libs.**

\subsection pure_used_libs_hw_detect_renderer_vidmem Video Memory

The other topic that is also related to rendering hardware, is the **amount of video memory**.  
Based on this, a renderer might not use some features and can also recommend automatic downsizing of textures.  
For decades, querying the **total amount** and the **free available video memory** was not possible to be done in a standard way.  
DirectX has implementation for it, but OpenGL does not have.  
Would be nice to find some cross-platform solution for this.

\section pure_used_libs_hw_detect_other HW Detection, Part 2: CPU, RAM, etc.

When generating logs for troubleshooting, it would be nice to better understand the client's (customer) hardware setup.  
PURE v0.4 has some classes for that but these are unfinished and NOT cross-platform: `PureHwAudio`, `PureHwCentralProcessor`, `PureHwSystemMemory`.  
I found the following libs in this topic:
 - [CPU-X](https://github.com/TheTumultuousUnicornOfDarkness/CPU-X)
 - [hardinfo](https://github.com/lpereira/hardinfo)
 - [cpufetch](https://github.com/Dr-Noob/cpufetch)
 - [hwinfo](https://github.com/openSUSE/hwinfo)

Will need to do more research on this.

**TODO: conclude on the libs.**

\section pure_used_libs_images Image File Loading

**Requirements for image handling libraries:**
 - shall support images with **transparency** so that I can utilize alpha values for [blending](https://www.khronos.org/opengl/wiki/blending);
 - shall support **animated** images (e.g. GIF) so that I can easily load sequence of frames for animation purpose;
 - shall support **DDS format** [[link to DDS]](https://en.wikipedia.org/wiki/DirectDraw_Surface), [[link to S3TC]](https://en.wikipedia.org/wiki/S3_Texture_Compression), because that is king for textures;
 - shall support **resizing** because I want image resolution downscaling option before uploading to video memory, based on user's video settings, for slower hardware;
 - shall **not force me to use its own texture class** as I want to use my own existing texture class with the decoded image data provided by the library.
 
The [SAIL lib lists a few competitor libs](https://github.com/HappySeaFox/sail?tab=readme-ov-file#competitors).  
From this list:
 - I skipped [CImg](https://github.com/GreycLab/CImg) because it looks to be more like an image manipulation library, probably too much for me;
 - I skipped [stb](https://github.com/nothings/stb) because its code and API is not even C99 but older standard, I'm looking for something bit more modern;
 - I skipped [Boost.GIL](https://www.boost.org/doc/libs/1_68_0/libs/gil/doc/html/index.html) for unknown reason, somehow I feel like I will never start using Boost even though I know it is very good;
 - the rest was not updated for many years.

**So it boiled down to the following list:**
 - [SFML](https://github.com/SFML/SFML), considered also in the [Windowing libraries section](#pure_used_libs_windowing), note it does NOT support DDS;
 - [SDL](https://github.com/libsdl-org/SDL), considered also in the [Windowing libraries section](#pure_used_libs_windowing), note it does NOT support DDS either;
 - [SAIL](https://github.com/HappySeaFox/sail), note it does NOT support DDS either.

**TODO: conclude!**

It is interesting to see **none of them support DDS or KTX either** (DevIL and stb support DDS but they failed on some other requirement).  
For DDS support, it is ok if a given lib does not support resizing because DDS stores different MIP map levels, so downscaling can be done by simply selecting a specific MIP map level as base texture, instead of actually resizing.  
For a DDS-only lib, either I can use [DDS++](https://github.com/redorav/ddspp), or worst-case I can use my own implementation I made around 2008.

Another thing to mention is **ETC** format [[link to ETCPACK]](https://github.com/Ericsson/ETCPACK), which reminds me to the DXT compressions used in DDS files.  
If this one is widely supported on OpenGL ES devices and even more preferred than DXT compressions I might need to support this as well.

**TODO: conclude, but first finish the windowing section!**

\section pure_used_libs_models Model File Loading

\subsection pure_used_libs_models_obj_mtl OBJ and MTL

PURE v0.4 has its own OBJ model file parser implementation, however it still does NOT support MTL file format which is a very fundamental addition to OBJ, since OBJ can reference MTL file to describe material data.  
To add MTL support, I think [Stefan Johnsen's library](https://github.com/StefanJohnsen/WavefrontMTL) is a good idea.  
Later in the future I can also use [his OBJ parser lib as well](https://github.com/StefanJohnsen/WavefrontOBJ).

\subsection pure_used_libs_models_fbx_gltf FBX and glTF

Since OBJ format does not store model animation, in the future I need to support another 3D model file format as well with animation.  
[FBX](https://en.wikipedia.org/wiki/FBX) and [glTF](https://www.khronos.org/Gltf) formats both support model animation and lots of other features, while they also get more and more popular nowadays.  
The plan is to include parser libs for these as well in the future.

**TODO: conclude on the libs.**

\section pure_used_libs_lightmaps Lightmaps

TODO

\section pure_used_libs_shaders Shaders

TODO

\section pure_used_libs_state_cache OpenGL State Caching

TODO