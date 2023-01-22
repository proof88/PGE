\page debugging Debugging

[TOC]

\section logging Logging

The engine uses [CConsole logger](https://github.com/proof88/Console) for logging purpose.  
Logging has to be explicitly enabled to see the logs and to generate log file. 
You can get the CConsole instance from the engine by PR00FsReducedRenderingEngine::getConsole().  
However, if you want to turn on logging before the engine is created/initialized, you should implicitly create the singleton CConsole instance with **CConsole::getConsoleInstance()** and set it up properly. For this you can see example of how I do it in [tmcsEnableDebugging() of the gfxcore2 project](https://github.com/proof88/gfxcore2/blob/master/gfxcore2/gfxcore2/gfxcore2.cpp).

\section memory_usage Memory Usage

Each PRREManager and PRREManaged instance is able to report its used memory by invoking its **getUsedSystemMemory()** method.  
Some managers and manageds can also report their used video memory by invoking their **getUsedVideoMemory()** method.  
Invoking any of the above functions of a manager instance reports the summarized amount of resource usage for all of its managed instances.

\section manager_write_list Manager WriteList()

Each PRREManager -derived manager is able to log its collected statistics and list of its managed objects by invoking its **WriteList()** method.

\section render_hints Render Hints

Render Hints are debug switches for the current renderer. These can be switched on-the-fly, so no reinitialization is needed.  
Each renderer (a class implementing PRREIRenderer interface) implements its own switches that are stored in a **TPRRE_RENDER_HINT** variable. See the detailed description at the actual renderer.  
Use the current renderer's **getRenderHints()** and **SetRenderHints()** functions to access these switches.  
These render hints can be logged anytime by invoking current renderer's PRREIRenderer::WriteStats() implementation.

\section current_stats Current Statistics

Each renderer (a class implementing PRREIRenderer interface) implements so called "Current Statistics" that are being collected for arbitrary time duration. The first duration starts when the engine is initialized. From that point, the current renderer is updating its "Current Statistics" whenever its PRREIRenderer::RenderScene() is invoked.  
Normally this duration lasts until the engine is shut down.  
However, if the current renderer's PRREIRenderer::ResetStatistics() implementation is invoked, the "Current Statistics" are concluded, saved for later observation and reset. Reset means that a new duration is started from where new statistics are collected that are valid only for the new duration. This is why we say these are collected for arbitrary time duration, since you can always start a new duration whenever you want.  
This feature is very useful when e.g. you want to measure average FPS from different camera views of the scene, or with different display settings, etc, without restarting your client application.  
I personally used this feature when I was switching back-and-forth between different occlusion culling method settings, render paths, etc. without changing anything in the scene itself, to see how FPS was varying.  
Resetting "Current Statistics" also saves the current \ref render_hints so you always know which render debug switches were applied to different "Current Statistics" durations if you are observing the log.  
Currently following "Current Statistics" are collected per duration:
 - length of duration;
 - number of rendered frames;
 - min/max/avg frame time;
 - average fps;
 - rendering hints active for this specific duration.

These statistics can be logged anytime by invoking current renderer's PRREIRenderer::WriteStats() implementation.

\section last_frame_stats Last Frame Statistics

Currently only PRRERendererHWfixedPipe implements "Last Frame Statistics". These stats are automatically reset at the beginning of each frame and concluded at the end of each frame, meaning that these are just momentary values.  
Currently following "Last Frame Statistics" are maintained:
 - number of 3D objects iterated on by the renderer;
   - grouped by occluder/occludee/occlusion-testing/occlusion-status properties;
 - number of triangles and vertices processed.

These statistics can be logged anytime by invoking current renderer's PRREIRenderer::WriteStats() implementation, and can be also programmatically queried by the **getLastFrame...()** functions of the current renderer.

\section other_stats Other Statistics

PRREObject3DManager collects HW occlusion query related statistics:
 - Sync method: longest wait time for a HW occlusion query to finish (in the same frame it was started);
 - ASync method: minimum and maximum number of frames elapsed between starting and finishing a HW occlusion query.

PRREObject3DManager::WriteList() logs these with per-object granularity.  
The current renderer's PRREIRenderer::ResetStatistics() implementation saves and resets these stats too.
 
\section engine_write_list Engine WriteList()

All above information mentioned previously on this page can be logged by a single call to PR00FsReducedRenderingEngine::WriteList().  
It also summarizes the summarized memory usages calculated by the managers.  
Engine's **WriteList()** is automatically invoked if the user previously invokes **SetAutoWriteStatsAtShutdown(true)**.
 
