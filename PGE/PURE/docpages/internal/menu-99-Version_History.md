\page version_history Version History

[TOC]

\section v0_4 v0.4 - December 19, 2024

Just a few changes happened in the recent 3 years as my focus was on developing my game: [PRooFPS-dd](https://github.com/proof88/PRooFPS-dd).

Changes:
 - all "PRRE" text has been renamed to "Pure" within functions, filenames, variable names, etc;
 - GUI: included [Dear ImGui](https://github.com/ocornut/imgui) to be used in any future games for handling **graphic user interface**, exposed over the original `PureUiManager` class;
 - GUI: `PureUiText` is now considered **deprecated** but kept to stay compatible with older applications such as the legacy PR00FPS from 2007;
 - GUI: added **drop shadow feature for PureUiText**;
 - added `PureProjection` class to be used for **2D->3D unprojection or 3D->2D projection** calculations in any future games;
 - added `PureMaterial::setDecalOffset()` to support rendering of **decals on surfaces** such as bullet holes on walls.

\section v0_3 v0.3 - November 1, 2021

Following features have been implemented:
 - \ref occlusion_culling to increment rendering speed.
 - **Depth Sorting:** order of rendering objects depends on their Z-distance to the camera. This can improve rendering speed while it can also fix incorrect rendering of transparent objects.
 - **Current and Last Frame Statistics:** number of rendered frames, frame times, etc. can be measured and such measurements can be restarted, previous measurements are saved, last frame statistics include number of vertices and triangles rendered, etc.

Following features have been postponed:
 - making the code **platform-independent** (ability to build to Linux);
 - \ref vfc;
 - \ref chc and \ref chcpp : although the required \ref structures have been implemented, they are currently unused.

\section v0_2 v0.2 - March 31, 2021

Main target was to refactor the existing codebase, hardening, bug fixes, make documentation, and make the first performance demo to measure rendering speed.

The **repositories have been uploaded to github**, as previously only local version control was used on the main dev machine.  
The **refactor** included separating source files into different subdirectories, extracting PRREObject3D functionality to separate PRREMesh3D and PRREVertexTransfer classes.  
**Hardening** included checking all memory allocations and OpenGL function calls and adding error handling to them where it was missing, extending logging, changing compiler settings to treat all warnings as errors, implemented per-module logging setting in [CConsole logger](https://github.com/proof88/Console).

\section v0_1 v0.1 - April 20, 2019

Main target here was the ability to **run PR00FPS** with the new engine.  
[Legacy PR00FPS](https://github.com/proof88/PR00FPS) is able to use the new engine without modifying a single line in PR00FPS code. This is achieved by adding a [wrapper library](https://github.com/proof88/gfxcore2) in between [legacy PR00FPS](https://github.com/proof88/PR00FPS) and the new [PURE graphics engine](https://github.com/proof88/PGE).