\page pure_used_libs Used 3rd Party Libraries

[TOC]

This page lists the 3rd party libraries used primarily by PURE.  
Additional 3rd party libraries used primarily by PGE are listed in [PGE's own documentation](https://proof88.github.io/pge-doc/index.html).

When choosing a 3rd party library to be used in my project, there are some general factors that must be satisfied by the lib:
 - it shall be cross-platform, including Android (because I have plans for releasing to Android phones);
 - its license shall allow me to use it with my GPL-3.0/LGPL-3.0 software;
 - it shall be still maintained, so that at least severe security issues get fixed over time, and also I can apply stricter compiler warning/error settings when building the lib with recent C++ compilers;
 - unit tests and/or other tests are there so I can see the developer is doing some form of automated verification;
 - there are some actual users of the library, so I'm not the first human trying to actually use it;
 - C++ interface is highly appreciated over C.
 
So, for example, if there is a nice image loading library, working fine but not updated since 2004, then sorry but it is a no-go for me.

\section pure_used_libs_windowing Windowing

To render something, PURE requires a window.  
To be more detailed, a window is required by PURE not only for rendering, but also by PGE to handle user input.  
Before v0.5, `PureWindow` had WinAPI-based implementation to create and manage such window.  
This was not platform-independent, so I decided to find a better solution.

TODO: add sequence of selecting display mode and creating window, with extra step when MSAA is requested!
TODO: add what commands I use for beginning a frame and ending a frame, including swapbuffers, glflush.

My requirements for windowing libraries:
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

I considered these libs:
 - [SDL](https://github.com/libsdl-org/SDL)
 - [SFML](https://github.com/SFML/SFML)
 - [GLFW](https://github.com/glfw/glfw)

TODO: raw mouse input?
TODO: DPI ?
TODO: resolution?
TODO: vsync?
 
All of them are cross-platform, widely used, and still maintained.  
SDL and SFML provide much more: image file loading, audio support, networking, etc.  
If you do not use any libs yet for these purposes, I recommend selecting either SDL or SFML so later you can benefit from other features too.  
Otherwise GLFW might be just fine for you.

Android is currently NOT officially supported by GLFW ([but some work is already being done](https://github.com/glfw/glfw/issues/1726)).

\section pure_used_libs_hw_detect_ogl_extensions HW Detection, Part 1: Rendering

Properly detecting the rendering hardware is important because based on that the engine can decide what rendering feature it can use.  
**Features detection** in OpenGL is done by parsing OpenGL extensions.  
PURE v0.4 parses the supported extensions and also does an extensive per-version and per-feature check, so the user can use the `PureHwVideo` class to understand the capabilities of the current rendering hardware.  
PURE does this by not only expecting the presence of specific extensions strings but also expecting the related function pointers and in some cases it also compares some queried values to an expected range.  
Since I'm proud of it and did not see anything else similar with this level of verification, I want to keep this behavior.  
TODO: check gpu-z!  
BUT getting the function pointers for the extension-specific functions might need to be done in different way on desktop Linux and on mobile devices with OpenGL ES.  
To avoid doing too much unnecessary work, I'm planning to use a 3rd party lib for extension handling, and `PureHwVideo` will also rely on that lib and on my existing per-feature checking implementation.  
I have not yet done too much study on this but [GLEW](https://github.com/nigels-com/glew) and [Glad](https://github.com/Dav1dde/glad) came into scope so far.  
PRooFPS-dd has 2 tickets that will need to be closed once this work is done: [ticket 1](https://github.com/proof88/PRooFPS-dd/issues/28), [ticket 2](https://github.com/proof88/PRooFPS-dd/issues/30).

The other topic that is also related to rendering hardware, is the **amount of video memory**.  
Based on this, a renderer might not use some features and can also recommend automatic downsizing of textures.  
For decades, querying the total amount and the free available video memory was not possible to be done in a standard way.  
DirectX has implementation for it, but OpenGL does not have.  
Would be nice to find some cross-platform solution for this.

\section pure_used_libs_hw_detect_other HW Detection, Part 2: CPU, RAM, etc.

When generating logs for troubleshooting, it would be nice to better understand the client's (customer) hardware setup.  
PURE v0.4 has some classes for that but these are unfinished and NOT cross-platform: `PureHwAudio`, `PureHwCentralProcessor`, `PureHwSystemMemory`.  
Will need to do more research on this.

\section pure_used_libs_images Image File Loading

Requirements for image handling libraries:
 - shall support images with **transparency** so that I can utilize alpha values for [blending](https://www.khronos.org/opengl/wiki/blending);
 - shall support **animated** images (e.g. GIF) so that I can easily load sequence of frames for animation purpose;
 - shall support **DDS format** [[link]](https://en.wikipedia.org/wiki/DirectDraw_Surface) because that is king for textures;
 - shall support **resizing** because I want image resolution downscaling option before uploading to video memory, based on user's video settings, for slower hardware;
 - shall **not force me to use its own texture class** as I want to use my own texture class with the decoded image data provided by the library.
 
The [SAIL lib lists a few competitor libs](https://github.com/HappySeaFox/sail?tab=readme-ov-file#competitors).  
From this list:
 - I skipped [CImg](https://github.com/GreycLab/CImg) because it looks to be more like an image manipulation library, probably too much for me;
 - I skipped [stb](https://github.com/nothings/stb) because its code and API is not even C99 but older standard, I'm looking for something bit more modern;
 - I skipped [Boost.GIL](https://www.boost.org/doc/libs/1_68_0/libs/gil/doc/html/index.html) for unknown reason, somehow I feel like I will never start using Boost even though I know it is very good;
 - the rest was not updated for many years.

So it boiled down to the following list:
 - [SFML](https://github.com/SFML/SFML), considered also in the [Windowing libraries section](#pure_used_libs_windowing);
 - [SDL](https://github.com/libsdl-org/SDL), considered also in the [Windowing libraries section](#pure_used_libs_windowing);
 - [SAIL](https://github.com/HappySeaFox/sail).

TODO: conclude, but first finish the windowing section!

\section pure_used_libs_models Model File Loading

TODO

https://github.com/StefanJohnsen/WavefrontMTL?tab=readme-ov-file
