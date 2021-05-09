\page occlusion_culling Occlusion Culling

[TOC]

\section occlusion_basics Basics of Occlusion Culling

Idea of occlusion culling is that we separate objects into 2 groups: occluders and occludees.  
**Occluders** are usually the big static objects like buildings in a map, we render them first so their depth data is written into the depth buffer first.  
**Occludees** are usually smaller, but may be even more complex objects that might be occluded by occluders.  
We do occlusion tests for each of the bounding box of the occludees, and see if we need to actually render any occludees.

Overall this can save some time, provided that:  
 - the occludee geometry is reasonably more complex than its bounding box geometry and we have bottleneck in the geometry stage, or
 - if the fragment shaders that would normally had been executed for the occludee object in the rasterization stage would be more expensive than the rasterization of the bounding
box itself.

"For the cost of rendering a bounding box, you can potentially save rendering a normal object. A bounding box consists of only 12 triangles, whereas the original object might have contained
thousands or even millions of triangles." (https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_occlusion_query.txt)  

\subsection occlusion_stalling What Should Not Be an Occludee

"Because we can see through translucent objects (and they cannot be written to the depth buffer!), these can act only as occludees and not occluders.  
On the other hand, opaque objects can be both occluders and occludees, which means that they should be sorted from front to back and rendered before
translucent objects. After that, translucent objects are sorted back to front and rendered on-screen as well."  
(https://developer.nvidia.com/gpugems/gpugems/part-v-performance-and-practicalities/chapter-29-efficient-occlusion-culling)

"Using bounding box occlusion queries may either help or hurt in fill-limited situations, because rendering the pixels of a bounding box is not free.
In most situations, a bounding box will probably have more pixels than the original object. Those pixels can probably be rendered more quickly, though, since they involve only Z reads
(no Z writes or color traffic), and they need not be textured or otherwise shaded."  
(https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_occlusion_query.txt)

"When only depth testing or stencil writing is taking place, some new GPUs (such as the GeForce FX family) use a higher-performance rendering path.  
In the past, hardware didn't benefit much when the color buffer was switched off during testing for the visibility of a bounding box, because the circuits for color and depth value output were in the
same pipeline. Now, however, on newer hardware, if the color-buffer output is switched off, the pipelines are used for depth testing and the bounding box testing is faster."  
(https://developer.nvidia.com/gpugems/gpugems/part-v-performance-and-practicalities/chapter-29-efficient-occlusion-culling)

"The fill-rate problem can get even worse, because sometimes a bounding box needs to be rendered from both sides, with front-facing and back-facing polygons! The catch is that when you test an object
that is very close to the view origin, the viewer ends up inside the bounding box. That's why we need to render back faces also; otherwise, because of back-face culling, no pixels will be drawn
on-screen, and we'll get a false result. To be safe, skip occlusion testing for models that are too near the viewer; it can really do more harm than good, and an object that close is definitely visible."  
(https://developer.nvidia.com/gpugems/gpugems/part-v-performance-and-practicalities/chapter-29-efficient-occlusion-culling)

\subsection occlusion_stalling Stalling the Pipeline with Synchronization

Note that since we are already in the rendering pipeline when making these decisions, and both the CPU and the GPU is involved, the process itself can slow down rendering from the perspective
of the required synchronization between the CPU and GPU. More on this below.

Basically there are 2 techniques. Both has the potential bottleneck of CPU-GPU synchronization.

One way is to rasterize the occluders into a **coverage buffer on the CPU**, and then the depth testing for the bounding boxes of the occludees can be done on the CPU as well. This way the CPU on
its own can decide which objects are occluded and should be skipped, but the rest are actually need to be rendered by the GPU. The latency of this decision on the CPU can introduce GPU starvation.

The other technique is if we use the GPU for deciding which objects are occluded. For this, **HW occlusion queries** can be used. This involves similar technique: first render the occluders, and then
for the bounding box of each occludee, start an occlusion query. Although the queries are started by the CPU, they are executed by the GPU. Still CPU has to make the decision of skipping actually
occluded objects based on the HW query results, so the CPU might need to wait for them to be finished. The result will tell if any pixels or fragments of the bounding box passed the depth test.
This can also introduce potential GPU starvation. Note that such HW occlusion queries are not supported by mobile GPUs, in such case the first CPU-based method should be used.  

\subsection occlusion_temporal_coherence Solving Stalls by Exploiting Temporal Coherence

**Temporal coherence**: if we know what's visible and what's occluded in one frame, it is very likely that the same classification will be correct for most objects in the following frame as well.  
To get rid of potential pipeline stalls, in any of the above 2 cases, a more sophisticated approach is needed, where we use the information about occlusion during the rendering of the next frame.  
"Usually, if an object is visible one frame, it will be visible the next frame, and if it is not visible, it will not be visible the next frame. Of course, for most applications, "usually" isn't
good enough. It is undesirable, but acceptable, to render an object that *isn't* visible, because that only costs performance. It is generally unacceptable to *not* render an object that *is*
visible.  
The simplest approach is that visible objects should be checked every N frames (where, say, N=5) to see if they have become occluded, while objects that were occluded last frame must be rechecked
again in the current frame to guarantee that they are still occluded. This will reduce the number of wasteful occlusion queries by almost a factor of N."  
(https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_occlusion_query.txt)

\subsection occlusion_simple_pseudocode Simple Pseudo-code

Pseudo-code for an async (non-stop-and-wait) way of querying:

```.cpp
	For every object, we need 3 variables:
     - iOcclusionQuery - the ID of the OpenGL occlusion query;
     - nFramesWithoutOcclusionTest - how many frames elapsed without testing if the object is occluded;
     - bOcclusionQueryStarted - if true, there is an ongoing occlusion query for this object.

    When creating an object, an occlusion query object should be created (but not started) for it, and:
      - iOcclusionQuery = 0;
      - nFramesWithoutOcclusionTest = MAX_FRAMES_WO_OCCLUSION_TESTING;
      - bOcclusionQueryStarted = false;
      - nFramesWaitedForOcclusionTestResult = 0;  // just for statistics
      - nFramesWaitedForOcclusionTestResultMin = MAX_UINT;  // just for statistics
      - nFramesWaitedForOcclusionTestResultMax = 0;  // just for statistics
    
    The bOcclusionQueryStarted member is needed because although theoretically we could get GL_GET_QUERY_AVAILABLE
    for queries never yet started, I did not find any requirement on implementing the result for GL_GET_QUERY_AVAILABLE
    when a query has never started yet. I'm afraid of differences in implementations.
    I would rather maintain my own bOcclusionQueryStarted.

    Frustum cull your scene against any kind of object (translucent or opaque)
    Sort remaining objects opaque coarsely from front to back
    Render a z-pre-pass (no fragment shading, color masking turned on), maybe just large occluders here, like terrain, large buildings and stuff
    For each of N occludees:
      - if ( framesWithoutOcclusionTest >= MAX_FRAMES_WO_OCCLUSION_TESTING ):
        - yes: either this has been declared as visible object for the previous few frames or this was declared as occluded in last frame or this is the first rendered frame;
          - bOcclusionQueryStarted ?
            - yes: nothing
            - no:
              - start query;
              - bOcclusionQueryStarted = true;
              - nFramesWaitedForOcclusionTestResult = 0;
              - Disable color- and depth-writes (you’ll use the depth buffer written by rendering occluders):
                - glColorMask(false, false, false, false);
                - glDepthMask(false);
              - "Render" bounding volume of occludee;
              - end query;
              - Enable color- and depth-writes:
                - glColorMask(true, true, true, true); 
                - glDepthMask(true);
        - no:
          - framesWithoutOcclusionTest++;
          - render the occludee object;

      // this is intentionally not in else branch, so we check if query already finished in the same frame
      // although I think I will see no query being finished in the same frame ... but this is the first approach!
      // on the long run maybe we can move this into the yes-branch of the above "bOcclusionQueryStarted ?" condition
      - bOcclusionQueryStarted ?
        - yes:
          - nFramesWaitedForOcclusionTestResult++;
          - get(GL_GET_QUERY_AVAILABLE) ?
            - no:
              - render the occludee object;  // this could be controlled with a const boolean: render unsure occludees or not?
                // if we render here then hidden occludees will be rendered every next frame if query result need more than 1 frame!!!
            - yes:
              - if ( nFramesWaitedForOcclusionTestResult < nFramesWaitedForOcclusionTestResultMin )
                - nFramesWaitedForOcclusionTestResultMin = nFramesWaitedForOcclusionTestResult;
              - if ( nFramesWaitedForOcclusionTestResult > nFramesWaitedForOcclusionTestResultMax )
                - nFramesWaitedForOcclusionTestResultMax = nFramesWaitedForOcclusionTestResult;
              - nFramesWaitedForOcclusionTestResult = 0;
              - bOcclusionQueryStarted = false;
              - get query result;
              - number of visible fragments is greater than 0 ?:
                - yes:
                  - render the occludee object;
                  - framesWithoutOcclusionTest = 0;  // we can wait for a few frames before testing again
                - no:
                  - framesWithoutOcclusionTest = MAX_FRAMES_WO_OCCLUSION_TESTING;   // this is occluded now, skip draw, but re-test is needed immediately!
        - no: nothing.

    Render translucent object from back to front
```

\section chc CHC: Coherent Hierarchical Culling

"The algorithm exploits the spatial and temporal coherence of visibility. (...)  
**Spatial coherence**: storing the scene in a hierarchical data structure, processing nodes of the hierarchy in a front-to-back order. (...) If geometry is not the main rendering bottleneck,
but rather the number of draw calls issued (Wloka 2003), then making additional draw calls to issue the occlusion queries is a performance loss. With hierarchies, though, interior nodes group
a larger number of draw calls, which are all saved if the node is occluded using a single query. (...)  
**Temporal coherence**: if we know what's visible and what's occluded in one frame, it is very likely that the same classification will be correct for most objects in the following frame as well.
(...) If we want to have correct images, we need to verify our guess and rectify our choice in case it was wrong. In the first case (the node was actually occluded), we update the classification
for the next frame. In the second case (the node was actually visible), we just process (that is, traverse or render) the node normally. The good news is that we can do all of this later, whenever
the query result arrives. Note also that the accuracy of our guess is not critical, because we are going to verify it anyway. (...)  
Issue occlusion queries only for previously visible leaf nodes and for the largest possible occluded nodes in the hierarchy (in other words, an occluded node is not tested if its parent is occluded
as well). The number of queries issued is therefore linear in the number of visible objects."  
(https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-6-hardware-occlusion-queries-made-useful)  
The algorithm can be further optimized with a few ideas mentioned on the page in "6.6 Optimizations".
    
"The CHC algorithm works well in densely occluded scenes, but the overhead of hardware occlusion queries makes it fall behind even simple view-frustum culling (VFC) in some situations. (...)
It traverses the hierarchy in a front-to-back order and issues queries only for previously visible leaves and nodes of the previously invisible boundary. Previously visible leaves are assumed to stay
visible in the current frame, and hence they are rendered immediately. The result of the query for these nodes only updates their classification for the next frame. The invisible nodes are assumed to
stay invisible, but the algorithm retrieves the query result in the current frame in order to discover visibility changes. (...) The algorithm works very well for scenarios that have a lot of occlusion.
However, on newer hardware where rendering geometry becomes cheap compared to querying, or view points where much of the scene is visible, the method can become even slower than conventional
view-frustum culling. This is a result of wasted queries and unnecessary state changes."  
(https://www.cg.tuwien.ac.at/research/publications/2008/mattausch-2008-CHC/mattausch-2008-CHC-draft.pdf)

\section chcpp CHC++: Enhanced Coherent Hierarchical Culling

CHC++: a further optimized form of CHC bringing much better speedup.

Following texts are citations from: https://www.cg.tuwien.ac.at/research/publications/2008/mattausch-2008-CHC/mattausch-2008-CHC-draft.pdf

**Queues for batching of queries**: Before a node is queried, it is appended to a queue. Separate queues are used for accumulating previously visible and previously invisible nodes. We use the queues to
issue batches of queries instead of individual queries. This reduces state changes by one to two orders of magnitude.  
Details:  
"It turns out that changes of rendering state cause an even larger overhead than the query itself. (...) Game developers refer to about 200 state changes per frame as an acceptable value on current hardware.
The invisible nodes to be queried are appended to a queue which we call **i-queue**. When the number of nodes in the i-queue reaches a user-defined batch size b, we change the rendering state for querying
and issue an occlusion query for each node in the i-queue. (...)  
Similarly to CHC, our proposed method renders the geometry of previously visible nodes during the hierarchy traversal. However the queries are not issued immediately. Instead the corresponding nodes are
stored in a queue which we call **v-queue**. (...) The queries for these nodes are not critical for the current frame since their result will only be used in the next frame. We exploit this observation by
using nodes from the v-queue to fill up waiting time: whenever the traversal queue is empty and no outstanding query result is available, we process nodes from the v-queue. (...)  
We propose to use an additional queue in the algorithm which we call **render queue**. This queue accumulates all nodes scheduled for rendering and is processed when a batch of queries is about to be issued.
When processing the render queue the engine can apply its internal material shader sorting and then render the objects stored in the queue in the new order."

**Multiqueries**. We compile multiqueries (Section 5.1), which are able to cover more nodes by a single occlusion query. This reduces the number of queries for previously invisible nodes up to an order of
magnitude.  
Details:  
"If some previously invisible part of a scene remains invisible in the current frame, a single occlusion query for the whole part is sufficient to verify its visibility status. Such a query would render all
bounding boxes of primitives in this scene part, and return zero if all primitives remain occluded. (...) Our new technique aims to identify such scene parts by forming groups of previously invisible nodes
that are equally likely to remain invisible. A single occlusion query is issued for each such group, which we call a multiquery. If the multiquery returns zero, all nodes in the group remain invisible and
their status has been updated by the single query. Otherwise the coherence was broken for this group and we issue individual queries for all nodes by reinserting them in the i-queue."

**Randomized sampling pattern for visible nodes**. We apply a temporally jittered sampling pattern (Section 5.2) for scheduling queries for previously visible nodes. This reduces the number of queries for
visible nodes and while spreading them evenly over the frames of the walkthrough.  
Details:  
"The original CHC algorithm introduced an important optimization in order to reduce the number of queries on previously visible nodes. A visible node is assumed to stay visible for nav frames and it will only
be tested in the frame nav +1. (...) This simple method however has a problem that the queries can be temporally aligned. This query alignment becomes problematic in situations when nodes tend to become visible
in the same frame. (...) The average number of queries per frame will still be reduced, but the alignment can cause observable frame rate drops. (...)  
We found that the most satisfying solution is achieved by randomizing the first invocation of the occlusion query. After a node has turned visible, we use a random value 0 < r < nav for determining the next
frame when a query will be issued. Subsequently, if the node was already visible in the previous test, we use a regular sampling interval given by nav."

**Tight bounding volumes**. We use tight bounding volumes (Section 6) without the need for their explicit construction. This provides a reduction of the number of rendered triangles as well as a reduction of
the number of queries.  
Details:  
"We propose a simple method for determining tighter bounds for inner nodes in the context of hardware occlusion queries applied to an arbitrary bounding volume hierarchy. For a particular node we determine its
tight bounding volume as a collection of bounding volumes of its children at a particular depth. (...)  
Tight bounding volumes provide several benefits at almost no cost: (1) earlier culling of interior nodes of the hierarchy, (2) culling of leaves which would otherwise be classified as visible, (3) increase of
coherence of visibility classification of interior nodes. The first property leads to a reduction of the number of queries. The second property provides a reduction of the number of rendered triangles.
Finally, the third benefit avoids changes in visibility classification for interior nodes caused by repeated pull-up and pull-down of visibility."

\section occlusion_materials Materials to Read

https://documents.pub/document/visibility-optimization-for-games-sampo-lappalainen-lead-programmer-umbra-software-ltd.html  
documents.pub_visibility-optimization-for-games-sampo-lappalainen-lead-programmer-umbra-software-ltd.ppt  

https://developer.nvidia.com/gpugems/gpugems/part-v-performance-and-practicalities/chapter-29-efficient-occlusion-culling  
https://community.khronos.org/t/occlusion-culling-with-arb-occlusion-query/68688/4  
https://gamedev.stackexchange.com/questions/118651/opengl-occlusion-culling-huge-performance-drop  
https://github.com/ychding11/GraphicsCollection/wiki/Occlusion-Culling-and-Visibility-Filter-in-Graphics  
CHC: https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-6-hardware-occlusion-queries-made-useful  
CHC++: https://www.cg.tuwien.ac.at/research/publications/2008/mattausch-2008-CHC/  
(mattausch-2008-CHC-draft.pdf)  
Battlefield: Bad Company: https://blog.selfshadow.com/publications/practical-visibility/  
https://software.intel.com/content/www/us/en/develop/topics/gamedev.html

Software Occlusion Culling  
https://software.intel.com/content/www/us/en/develop/articles/software-occlusion-culling.html  
https://software.intel.com/content/www/us/en/develop/articles/masked-software-occlusion-culling.html  
https://software.intel.com/content/www/us/en/develop/articles/merging-masked-occlusion-culling-hierarchical-buffers-for-faster-rendering.html

Core Techniques and Algorithms in Game Programming - Daniel Sanchez-Crespo Dalmau.pdf  
3D Game Engine Architecture - Engineering Real Time Applications with Wild Magic - David H. Eberl.pdf  
3D Game Engine Design - David H. Eberly.pdf

\section occlusion_extensions OpenGL Extensions for HW Occlusion Culling Support

\subsection GL_HP_occlusion_test GL_HP_occlusion_test

 - https://www.scitepress.org/Papers/2007/20725/20725.pdf  
 - GDC2002_occlusion.pdf  
 - SimpleFastHWaccelPoint-in-PolygonTest.pdf  
 - was supported in early 2000s, also supported by Geforce 7600 GT in 2006  
 - doesn't look to be supported by Geforce 1060 GT in 2021  
 - Desktop: Looks like for some reason vendors supported it around 2005-2007, but then they stopped supporting it!  
   Mobile: no support at all!  
 - simple GL_TRUE/GL_FALSE result;  
 - simple "stop-and-wait" model for using multiple queries. The application begins an occlusion test and ends it;  
   then, at some later point, it asks for the result, at which point the driver must stop and wait until the result  
   from the previous test is back before the application can even begin the next one. This means that usage of this  
   is not recommended as it can easily stall the graphics pipeline.  
 - **Verdict: don't use it.**

\subsection GL_HP_visibility_test GL_HP_visibility_test

 - http://www.vgamuseum.info/images/doc/profi/hpfx10.txt  
 - Desktop: nothing found.  
   Mobile: nothing found.  
 - improves upon HP's occlusion test extension by letting applications perform multiple visibility tests before getting  
   back the results. This is different than the occlusion test extension which provides only a single visibility test result.  
   To perform multiple visibility tests with the occlusion test extension, the result of the previous test must be obtained before  
   starting a new test. With the Visibility Test extension, performance is improved because the results of many tests can be obtained in  
   a single call.  
 - **Verdict: don't use it.**

\subsection GL_HP_visibility_stats GL_HP_visibility_stats

 - http://www.vgamuseum.info/images/doc/profi/hpfx10.txt  
 - Desktop: nothing found.  
   Mobile: nothing found.  
 - Visibility Statistics is a logical extension to HP's occlusion test and visibility test extensions. While visibility testing gives a  
   binary answer of visible or NOT visible, visibility statistics gives a quantative answer of how visible in the form of a pass count and  
   a fail count. Applications can use these counts to compute "percent visible" numbers and use the results in Level Of Detail (LOD) algorithms.  
 - **Verdict: don't use it.**

\subsection GL_NV_occlusion_query GL_NV_occlusion_query

 - https://www.khronos.org/registry/OpenGL/extensions/NV/NV_occlusion_query.txt  
 - GDC2002_occlusion.pdf  
 - vendor-specification from around 2002;  
 - Desktop: some NV and AMD drivers support it;  
   Mobile: nothing found.  
 - not supported widely enough, better to favor standard GL_ARB_occlusion_query  
 - **Verdict: don't use this.**

\subsection GL_NV_occlusion_query_samples GL_NV_occlusion_query_samples

 - cannot even find an official page about this extension;  
 - Desktop: nothing found.  
   Mobile: only with nvidia Tegra!  
 - **Verdict: don't use this.**

\subsection GL_ARB_occlusion_query GL_ARB_occlusion_query

 - https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_occlusion_query.txt  
 - 2003  
 - its result the number of samples that pass the depth and stencil tests. Samples are counted immediately after  
   _both_ the depth and stencil tests, i.e., samples that pass both;  
 - occlusion queries in "query objects" that allow applications to issue many queries before asking for the result of  
   any one. As a result, they can overlap the time it takes for the occlusion query results to be returned with other,  
   more useful work, such as rendering other parts of the scene or performing other computations on the CPU;  
 - occlusion queries return in order. If occlusion test X occurred before occlusion query Y, and the driver informs the app  
   that occlusion query Y is done, the app can infer that occlusion query X is also done.  
 - the polling  method introduced in the NV_occlusion_query spec allowed for a potential infinite loop if the application  
   does not do a flush. This version of the spec clarifies the behavior which now makes such a flush unnecessary.  
 - "An implementation can either set QUERY_COUNTER_BITS_ARB to the value 0, or to some number greater than or equal to n.  
   If an implementation returns 0 for QUERY_COUNTER_BITS_ARB, then the occlusion queries will always return that zero samples  
   passed the occlusion test, and so an application should not use occlusion queries on that implementation."  
 - Desktop: widely supported.  
   Mobile: nothing found.  
 - **Verdict: use this for desktop.**

\subsection GL_ARB_occlusion_query2 GL_ARB_occlusion_query2

 - https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_occlusion_query2.txt  
 - https://www.diva-portal.org/smash/get/diva2:830875/FULLTEXT01.pdf  
 - 2010, looks to be widely supported until today  
 - depends on GL_ARB_occlusion_query, the difference is that it can simply return bool true/false instead of the number  
   of passing samples. Probably faster, since it can stop the query at the first passing fragment depth test. Need to verify this though.  
 - **Verdict: use this for desktop.** Unclear if it has speed advantage over the first version if this ext.

\subsection GL_EXT_occlusion_query_boolean GL_EXT_occlusion_query_boolean

 - https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_occlusion_query_boolean.txt  
 - 2011  
 - I guess this is more likely for mobile instead of GL_ARB_occlusion_query2;  
 - This is also async query as GL_ARB_occlusion_query, so it can avoid stalling pipeline;  
 - Desktop: nothing found.  
   Mobile: lot of Mali, Tegra, Rogue, SGX chips support it.  
 - **Verdict: use this for mobile.** Note that, even in 2020, article states that occlusion culling on mobile is slow, and  
   a software occlusion culling implementation is recommended. In Unreal engine, software occlusion culling is recommended:  
   https://docs.microsoft.com/en-us/windows/mixed-reality/develop/unreal/performance-recommendations-for-unreal  
   Software occlusion culling like PVS and/or partial software rendering to find out if something is occluded or not.