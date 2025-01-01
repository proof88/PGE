\page gui GUI

[TOC]

\section gui_intro Intro

I have found the following web page that has a comprehensive list of GUI libraries for C++:
https://philippegroarke.com/posts/2018/c++_ui_solutions/

From the list, in 2024 **I revised the following 4 libraries** that are suitable for OpenGL and have hardly any dependencies:
 - GiGi: http://gigi.sourceforge.net/index.php
 - Turbo Badger: https://github.com/fruxo/turbobadger
 - Nuklear: https://github.com/Immediate-Mode-UI/Nuklear
 - Dear ImGui: https://github.com/ocornut/imgui

The first 2 are not maintaned anymore, the third is pure C, so **I have opted for the last one: Dear ImGui**.  
Note: later I noticed the [existence of TGUI](https://github.com/texus/TGUI/), which seems to meet all the requirements as well, including my general requirements for 3rd party libraries that I listed at [the 3rd Party libs page](#pure_used_libs).

\section gui_usage Usage

For a real-world example on **how to use Dear ImGui with PURE**, check the [PRooFPS-dd project](https://github.com/proof88/PRooFPS-dd/) which has a menu and also in-game HUD elements utilizing Dear ImGui.  
Basically, PURE takes care of initializing and destroying Dear ImGui when needed, you can simply concentrate on calling the GUI drawing functions of Dear ImGui without any hassle.  
There is a single callback function in PRooFPS-dd called [drawDearImGuiCb()](https://github.com/search?q=repo%3Aproof88/PRooFPS-dd%20proofps_dd%3A%3AGUI%3A%3AdrawDearImGuiCb&type=code) which is invoked by PURE in every frame.  
Then [drawDearImGuiCb()](https://github.com/search?q=repo%3Aproof88/PRooFPS-dd%20proofps_dd%3A%3AGUI%3A%3AdrawDearImGuiCb&type=code) decides exactly what GUI elements to render, based on the current game state.
