/*
    ###################################################################################
    PRREObject3D.cpp
    This file is part of PRRE.
    PRREObject3D class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Object3D/PRREObject3DManager.h"
#include "../../include/internal/Object3D/PRREObject3DImpl.h"
#include "../../include/internal/Object3D/PRREVertexTransferModeImpl.h"
#include <cassert>
#include "../../include/external/Hardware/PRREhwInfo.h"
#include "../../include/internal/PRREGLextensionFuncs.h"
#include "../../include/internal/PRREGLsafeFuncs.h"
#include "../../include/internal/PRREGLsnippets.h"
#include "../../include/internal/PRREpragmas.h"
#include "../../../../../PFL/PFL/PFL.h"

using namespace std;

/*

    Notes

    ***************************

    There are some online OpenGL capability reports can be used to find out how widely an extension is supported:
    https://www.gpuinfo.org/
     - https://opengl.gpuinfo.org/listextensions.php
     - https://opengles.gpuinfo.org/
    https://feedback.wildfiregames.com/report/opengl/


    Vertex Culling (Object-Space)
    *****************************

    GL_EXT_cull_vertex: https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_cull_vertex.txt
     - 1996
     - "Culling a polygon by examining its vertexes in object space can be more efficient than screen space polygon culling since
       the transformation to screen space (which may include a division by w) can be avoided for culled vertexes. Also, vertex culling
       can be computed before vertexes are assembled into primitives. This is a useful property when drawing meshes with shared vertexes,
       since a vertex can be culled once, and the resulting state can be used for all primitives which share the vertex."
     - This extension was most probably introduced for optimisation before HW T&L spread. Lack of HW T&L was present in the pre-Geforce era,
       i.e. TNT, etc.
       Desktop: was mostly supported by early integrated Intel VGA.
       Mobile: no support at all!
     - Verdict: don't use it if neither 3dfx nor other major vendors supported it before TnL!


    Occlusion Culling
    *****************

    Idea of occlusion culling is that we separate objects into 2 groups: occluders and occludees. Occluders are usually the big
    static objects like buildings in a map, we render them first so their depth data is written into the depth buffer first.
    Occludees are usually smaller, but may be even more complex objects that might be occluded by occluders. We do occlusion tests
    for each of the bounding box of the occludees, and see if we need to actually render any occludees.

    "For the cost of rendering a bounding box, you can potentially save rendering a normal object. A bounding box consists of only
    12 triangles, whereas the original object might have contained thousands or even millions of triangles.
    Using bounding box occlusion queries may either help or hurt in fill-limited situations, because rendering the pixels of a
    bounding box is not free. In most situations, a bounding box will probably have more pixels than the original object.
    Those pixels can probably be rendered more quickly, though, since they involve only Z reads (no Z writes or color traffic),
    and they need not be textured or otherwise shaded.
    [...]
    Usually, if an object is visible one frame, it will be visible the next frame, and if it is not visible, it will not be visible
    the next frame. Of course, for most applications, "usually" isn't good enough. It is undesirable, but acceptable, to render an
    object that *isn't* visible, because that only costs performance. It is generally unacceptable to *not* render an object that *is*
    visible.
    The simplest approach is that visible objects should be checked every N frames (where, say, N=5) to see if they have become
    occluded, while objects that were occluded last frame must be rechecked again in the current frame to guarantee that they are
    still occluded. This will reduce the number of wasteful occlusion queries by almost a factor of N."
    (https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_occlusion_query.txt)

    Rendering:
    "Because we can see through translucent objects (and they cannot be written to the depth buffer!), these can act only as occludees and not occluders.
    On the other hand, opaque objects can be both occluders and occludees, which means that they should be sorted from front to back and rendered before
    translucent objects. After that, translucent objects are sorted back to front and rendered on-screen as well."

    "The rule of thumb is that objects that are not fill-rate bound (that is, objects that don't use complex fragment programs, many texture layers, and so on)
    should not be tested for occlusion at higher resolutions. This is because there are more pixels to fill at a higher resolution, so it's likely the GPU will
    spend more time rendering the object's bounding box than the object itself. Keep in mind that if the object is occluded, the early-z rejection will do its
    work at the per-pixel level, so processing complex fragment programs will be avoided.
    The fill-rate problem can get even worse, because sometimes a bounding box needs to be rendered from both sides, with front-facing and back-facing polygons!
    The catch is that when you test an object that is very close to the view origin, the viewer ends up inside the bounding box. That's why we need to render
    back faces also; otherwise, because of back-face culling, no pixels will be drawn on-screen, and we'll get a false result. To be safe, skip occlusion testing
    for models that are too near the viewer; it can really do more harm than good, and an object that close is definitely visible."
    "When only depth testing or stencil writing is taking place, some new GPUs (such as the GeForce FX family) use a higher-performance rendering path.
    In the past, hardware didn't benefit much when the color buffer was switched off during testing for the visibility of a bounding box, because the circuits
    for color and depth value output were in the same pipeline. Now, however, on newer hardware, if the color-buffer output is switched off, the pipelines are
    used for depth testing and the bounding box testing is faster."
    (https://developer.nvidia.com/gpugems/gpugems/part-v-performance-and-practicalities/chapter-29-efficient-occlusion-culling)

    Pseudo-code for an async (non-stop-and-wait) way of querying:

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


    CHC: Coherent Hierarchical Culling

    "The algorithm exploits the spatial and temporal coherence of visibility. (...)
    Spatial coherence: storing the scene in a hierarchical data structure, processing nodes of the hierarchy in a front-to-back order. (...)
    If geometry is not the main rendering bottleneck, but rather the number of draw calls issued (Wloka 2003), then making additional draw calls to issue
    the occlusion queries is a performance loss. With hierarchies, though, interior nodes group a larger number of draw calls, which are all saved if the node
    is occluded using a single query. (...)
    Temporal coherence: if we know what's visible and what's occluded in one frame, it is very likely that the same classification will be correct for most
    objects in the following frame as well. (...) If we want to have correct images, we need to verify our guess and rectify our choice in case it was wrong.
    In the first case (the node was actually occluded), we update the classification for the next frame. In the second case (the node was actually visible),
    we just process (that is, traverse or render) the node normally. The good news is that we can do all of this later, whenever the query result arrives.
    Note also that the accuracy of our guess is not critical, because we are going to verify it anyway. (...)
    Issue occlusion queries only for previously visible leaf nodes and for the largest possible occluded nodes in the hierarchy (in other words, an occluded
    node is not tested if its parent is occluded as well). The number of queries issued is therefore linear in the number of visible objects."
    (https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-6-hardware-occlusion-queries-made-useful)
    The algorithm can be further optimized with a few ideas mentioned on the page in "6.6 Optimizations".
    
    "The CHC algorithm works well in densely occluded scenes, but the overhead of hardware occlusion queries makes it fall behind even simple view-frustum culling
    (VFC) in some situations. (...) It traverses the hierarchy in a front-to-back order and issues queries only for previously visible leaves and nodes of the
    previously invisible boundary. Previously visible leaves are assumed to stay visible in the current frame, and hence they are rendered immediately.
    The result of the query for these nodes only updates their classification for the next frame. The invisible nodes are assumed to stay invisible, but the
    algorithm retrieves the query result in the current frame in order to discover visibility changes.(...)
    The algorithm works very well for scenarios that have a lot of occlusion. However, on newer hardware where rendering geometry becomes cheap compared to querying,
    or view points where much of the scene is visible, the method can become even slower than conventional view-frustum culling. This is a result of wasted queries and
    unnecessary state changes. "
    (https://www.cg.tuwien.ac.at/research/publications/2008/mattausch-2008-CHC/mattausch-2008-CHC-draft.pdf)

    
    CHC++: a further optimized form of CHC bringing much better speedup

    Following citations are from: https://www.cg.tuwien.ac.at/research/publications/2008/mattausch-2008-CHC/mattausch-2008-CHC-draft.pdf

    "Queues for batching of queries: Before a node is queried, it is appended to a queue. Separate queues are used for accumulating previously visible and previously invisible
    nodes. We use the queues to issue batches of queries instead of individual queries. This reduces state changes by one to two orders of magnitude."
    Details:
    "It turns out that changes of rendering state cause an even larger overhead than the query itself. (...)
    Game developers refer to about 200 state changes per frame as an acceptable value on current hardware.
    The invisible nodes to be queried are appended to a queue which we call i-queue. When the number of nodes in the i-queue reaches a user-defined batch size b,
    we change the rendering state for querying and issue an occlusion query for each node in the i-queue. (...)
    Similarly to CHC, our proposed method renders the geometry of previously visible nodes during the hierarchy traversal. However the queries are not issued immediately.
    Instead the corresponding nodes are stored in a queue which we call v-queue. (...) The queries for these nodes are not critical for the current frame since their result
    will only be used in the next frame. We exploit this observation by using nodes from the v-queue to fill up waiting time: whenever the traversal queue is empty and no
    outstanding query result is available, we process nodes from the v-queue. (...)
    We propose to use an additional queue in the algorithm which we call render queue. This queue accumulates all nodes scheduled for rendering and is processed
    when a batch of queries is about to be issued. When processing the render queue the engine can apply its internal material shader sorting and then render the objects stored
    in the queue in the new order."
    
    "Multiqueries. We compile multiqueries (Section 5.1), which are able to cover more nodes by a single occlusion query. This reduces the number of queries for previously
    invisible nodes up to an order of magnitude."
    Details:
    "If some previously invisible part of a scene remains invisible in the current frame, a single occlusion query for the whole part is sufficient to verify its visibility status.
    Such a query would render all bounding boxes of primitives in this scene part, and return zero if all primitives remain occluded. (...) Our new technique aims to identify such
    scene parts by forming groups of previously invisible nodes that are equally likely to remain invisible. A single occlusion query is issued for each such group, which we call a
    multiquery. If the multiquery returns zero, all nodes in the group remain invisible and their status has been updated by the single query. Otherwise the coherence was broken for
    this group and we issue individual queries for all nodes by reinserting them in the i-queue."

    "Randomized sampling pattern for visible nodes. We apply a temporally jittered sampling pattern (Section 5.2) for scheduling queries for previously visible nodes.
    This reduces the number of queries for visible nodes and while spreading them evenly over the frames of the walkthrough."
    Details:
    "The original CHC algorithm introduced an important optimization in order to reduce the number of queries on previously visible nodes. A visible node is assumed to stay visible
    for nav frames and it will only be tested in the frame nav +1. (...) This simple method however has a problem that the queries can be temporally aligned. This query alignment
    becomes problematic in situations when nodes tend to become visible in the same frame. (...) The average number of queries per frame will still be reduced, but the
    alignment can cause observable frame rate drops. (...) We found that the most satisfying solution is achieved by randomizing the first invocation of the occlusion query. After
    a node has turned visible, we use a random value 0 < r < nav for determining the next frame when a query will be issued. Subsequently, if the node was already visible in the previous
    test, we use a regular sampling interval given by nav."

    "Tight bounding volumes. We use tight bounding volumes (Section 6) without the need for their explicit construction. This provides a reduction of the number of rendered
    triangles as well as a reduction of the number of queries."
    Details:
    "We propose a simple method for determining tighter bounds for inner nodes in the context of hardware occlusion queries applied to an arbitrary bounding volume hierarchy.
    For a particular node we determine its tight bounding volume as a collection of bounding volumes of its children at a particular depth. (...)
    Tight bounding volumes provide several benefits at almost no cost: (1) earlier culling of interior nodes of the hierarchy, (2) culling of leaves which would otherwise be classified
    as visible, (3) increase of coherence of visibility classification of interior nodes. The first property leads to a reduction of the number of queries. The second property provides a
    reduction of the number of rendered triangles. Finally, the third benefit avoids changes in visibility classification for interior nodes caused by repeated pull-up and pull-down of
    visibility."
    
    
    Material to read:

        https://documents.pub/document/visibility-optimization-for-games-sampo-lappalainen-lead-programmer-umbra-software-ltd.html
        documents.pub_visibility-optimization-for-games-sampo-lappalainen-lead-programmer-umbra-software-ltd.ppt

        https://developer.nvidia.com/gpugems/gpugems/part-v-performance-and-practicalities/chapter-29-efficient-occlusion-culling
        https://community.khronos.org/t/occlusion-culling-with-arb-occlusion-query/68688/4
        https://gamedev.stackexchange.com/questions/118651/opengl-occlusion-culling-huge-performance-drop
        https://github.com/ychding11/GraphicsCollection/wiki/Occlusion-Culling-and-Visibility-Filter-in-Graphics
        CHC: https://developer.nvidia.com/gpugems/gpugems2/part-i-geometric-complexity/chapter-6-hardware-occlusion-queries-made-useful
        CHC++: https://www.cg.tuwien.ac.at/research/publications/2008/mattausch-2008-CHC/
               mattausch-2008-CHC-draft.pdf
        Battlefield: Bad Company: https://blog.selfshadow.com/publications/practical-visibility/
        https://software.intel.com/content/www/us/en/develop/topics/gamedev.html

        Software Occlusion Culling
        https://software.intel.com/content/www/us/en/develop/articles/software-occlusion-culling.html
        https://software.intel.com/content/www/us/en/develop/articles/masked-software-occlusion-culling.html
        https://software.intel.com/content/www/us/en/develop/articles/merging-masked-occlusion-culling-hierarchical-buffers-for-faster-rendering.html

        Core Techniques and Algorithms in Game Programming - Daniel Sanchez-Crespo Dalmau.pdf
        3D Game Engine Architecture - Engineering Real Time Applications with Wild Magic - David H. Eberl.pdf
        3D Game Engine Design - David H. Eberly.pdf

    Related OpenGL extensions for HW occlusion culling support:

        GL_HP_occlusion_test
        (https://www.scitepress.org/Papers/2007/20725/20725.pdf)
        GDC2002_occlusion.pdf
        SimpleFastHWaccelPoint-in-PolygonTest.pdf
         - was supported in early 2000s, also supported by Geforce 7600 GT in 2006
         - doesn't look to be supported by Geforce 1060 GT in 2021
         - Desktop: Looks like for some reason vendors supported it around 2005-2007, but then they stopped supporting it!
           Mobile: no support at all!
         - simple GL_TRUE/GL_FALSE result;
         - simple "stop-and-wait" model for using multiple queries. The application begins an occlusion test and ends it;
           then, at some later point, it asks for the result, at which point the driver must stop and wait until the result
           from the previous test is back before the application can even begin the next one. This means that usage of this
           is not recommended as it can easily stall the graphics pipeline.
         - Verdict: don't use it.

        GL_HP_visibility_test: http://www.vgamuseum.info/images/doc/profi/hpfx10.txt
         - Desktop: nothing found.
           Mobile: nothing found.
         - improves upon HP's occlusion test extension by letting applications perform multiple visibility tests before getting
           back the results. This is different than the occlusion test extension which provides only a single visibility test result.
           To perform multiple visibility tests with the occlusion test extension, the result of the previous test must be obtained before
           starting a new test. With the Visibility Test extension, performance is improved because the results of many tests can be obtained in
           a single call.
         - Verdict: don't use it.

        GL_HP_visibility_stats: http://www.vgamuseum.info/images/doc/profi/hpfx10.txt
         - Desktop: nothing found.
           Mobile: nothing found.
         - Visibility Statistics is a logical extension to HP's occlusion test and visibility test extensions. While visibility testing gives a
           binary answer of visible or NOT visible, visibility statistics gives a quantative answer of how visible in the form of a pass count and
           a fail count. Applications can use these counts to compute "percent visible" numbers and use the results in Level Of Detail (LOD) algorithms.
         - Verdict: don't use it.

        GL_NV_occlusion_query: https://www.khronos.org/registry/OpenGL/extensions/NV/NV_occlusion_query.txt
        GDC2002_occlusion.pdf
         - vendor-specification from around 2002; 
         - Desktop: some NV and AMD drivers support it;
           Mobile: nothing found.
         - not supported widely enough, better to favor standard GL_ARB_occlusion_query
         - Verdict: don't use this. 

        GL_NV_occlusion_query_samples
         - cannot even find an official page about this extension;
         - Desktop: nothing found.
           Mobile: only with nvidia Tegra!
         - Verdict: don't use this.

        GL_ARB_occlusion_query: https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_occlusion_query.txt
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
         - Verdict: use this for desktop.

        GL_ARB_occlusion_query2: https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_occlusion_query2.txt
        https://www.diva-portal.org/smash/get/diva2:830875/FULLTEXT01.pdf
         - 2010, looks to be widely supported until today
         - depends on GL_ARB_occlusion_query, the difference is that it can simply return bool true/false instead of the number
           of passing samples. Probably faster, since it can stop the query at the first passing fragment depth test. Need to verify this though.
         - Verdict: use this for desktop. Unclear if it has speed advantage over the first version if this ext.

        GL_EXT_occlusion_query_boolean: https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_occlusion_query_boolean.txt
         - 2011
         - I guess this is more likely for mobile instead of GL_ARB_occlusion_query2;
         - This is also async query as GL_ARB_occlusion_query, so it can avoid stalling pipeline;
         - Desktop: nothing found.
           Mobile: lot of Mali, Tegra, Rogue, SGX chips support it.
         - Verdict: use this for mobile. Note that, even in 2020, article states that occlusion culling on mobile is slow, and
           a software occlusion culling implementation is recommended. In Unreal engine, software occlusion culling is recommended:
           https://docs.microsoft.com/en-us/windows/mixed-reality/develop/unreal/performance-recommendations-for-unreal
           Software occlusion culling like PVS and/or partial software rendering to find out if something is occluded or not.
    
*/

/*
   PRREObject3D::PRREObject3DImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Gets the appropriate PRRE blend factor for the given GL enum.

    @return The appropriate PRRE blend factor for the given GL enum.
            PRRE_ZERO for invalid GL enum.
*/
TPRRE_BLENDFACTORS PRREObject3D::PRREObject3DImpl::getPRREblendFromGLblend(GLenum glb)
{
    switch( glb )
    {
    case GL_ZERO                : return PRRE_ZERO;
    case GL_ONE                 : return PRRE_ONE;
    case GL_SRC_COLOR           : return PRRE_SRC_COLOR;
    case GL_ONE_MINUS_SRC_COLOR : return PRRE_ONE_MINUS_SRC_COLOR;
    case GL_DST_COLOR           : return PRRE_DST_COLOR;
    case GL_ONE_MINUS_DST_COLOR : return PRRE_ONE_MINUS_DST_COLOR;
    case GL_SRC_ALPHA           : return PRRE_SRC_ALPHA;
    case GL_ONE_MINUS_SRC_ALPHA : return PRRE_ONE_MINUS_SRC_ALPHA;
    case GL_DST_ALPHA           : return PRRE_DST_ALPHA;         
    case GL_ONE_MINUS_DST_ALPHA : return PRRE_ONE_MINUS_DST_ALPHA;
    case GL_SRC_ALPHA_SATURATE  : return PRRE_SRC_ALPHA_SATURATE;
    default                     : return PRRE_ZERO;
    }
} // getPRREblendFromGLblend()


/**
    Gets the appropriate GL enum for the given PRRE blend factor.

    @return The appropriate GL enum for the given PRRE blend factor.
            GL_ZERO for invalid PRRE blend factor.
*/
GLenum PRREObject3D::PRREObject3DImpl::getGLblendFromPRREblend(TPRRE_BLENDFACTORS bf)
{
    switch( bf )
    {
    case PRRE_ZERO                : return GL_ZERO;
    case PRRE_ONE                 : return GL_ONE;
    case PRRE_SRC_COLOR           : return GL_SRC_COLOR;
    case PRRE_ONE_MINUS_SRC_COLOR : return GL_ONE_MINUS_SRC_COLOR;
    case PRRE_DST_COLOR           : return GL_DST_COLOR;
    case PRRE_ONE_MINUS_DST_COLOR : return GL_ONE_MINUS_DST_COLOR;
    case PRRE_SRC_ALPHA           : return GL_SRC_ALPHA;
    case PRRE_ONE_MINUS_SRC_ALPHA : return GL_ONE_MINUS_SRC_ALPHA;
    case PRRE_DST_ALPHA           : return GL_DST_ALPHA;         
    case PRRE_ONE_MINUS_DST_ALPHA : return GL_ONE_MINUS_DST_ALPHA;
    case PRRE_SRC_ALPHA_SATURATE  : return GL_SRC_ALPHA_SATURATE;
    default                       : return GL_ZERO;
    }
} // getGLblendFromPRREblend()


PRREObject3D::PRREObject3DImpl::~PRREObject3DImpl()
{
    _pOwner->getManagedConsole().OLnOI("~PRREObject3D() ...");

    delete[] pVerticesTransf;
    delete[] pFbBuffer;
    pVerticesTransf = PGENULL;
    pFbBuffer = PGENULL;

    _pOwner->DeleteAll();

    // if we are just a cloned object, above code should have done essentially nothing
    if ( getReferredObject() )
    {
        // TODO: probably we should decrement reference count in referred object ...
    }

    _pOwner->getManagedConsole().SOLnOO("Done!");
} // ~PRRETexture()


TPRRE_VERTEX_MODIFYING_HABIT PRREObject3D::PRREObject3DImpl::getVertexModifyingHabit() const
{
    return getReferredObject() ? getReferredObject()->getVertexModifyingHabit() : _pOwner->PRREVertexTransfer::getVertexModifyingHabit();
} // getVertexModifyingHabit()


TPRREbool PRREObject3D::PRREObject3DImpl::setVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    return setVertexTransferMode( PRREVertexTransfer::selectVertexTransferMode(vmod, _pOwner->getVertexReferencingMode(), false) );
} // setVertexModifyingHabit()


TPRRE_VERTEX_REFERENCING_MODE PRREObject3D::PRREObject3DImpl::getVertexReferencingMode() const
{
    return getReferredObject() ? getReferredObject()->getVertexReferencingMode() : _pOwner->PRREVertexTransfer::getVertexReferencingMode();
} // getVertexReferencingMode()


TPRREbool PRREObject3D::PRREObject3DImpl::setVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    return setVertexTransferMode( PRREVertexTransfer::selectVertexTransferMode(_pOwner->getVertexModifyingHabit(), vref, false) );
} // setVertexReferencingMode()


TPRRE_VERTEX_TRANSFER_MODE PRREObject3D::PRREObject3DImpl::getVertexTransferMode() const
{
    return getReferredObject() ? getReferredObject()->getVertexTransferMode() : _pOwner->PRREVertexTransfer::getVertexTransferMode();
} // getVertexTransferMode()


TPRREbool PRREObject3D::PRREObject3DImpl::setVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    _pOwner->getManagedConsole().OLnOI("PRREObject3D::setVertexTransferMode()");

    if ( getReferredObject() )
    {
        _pOwner->getManagedConsole().EOLnOO("PRREObject3D::setVertexTransferMode() ignored because we are cloned object!");
        return false;
    }

    TPRREbool b = _pOwner->PRREVertexTransfer::setVertexTransferMode(vtrans);
    _pOwner->getManagedConsole().OO();
    return b;
}


PRREObject3D* PRREObject3D::PRREObject3DImpl::getReferredObject() const
{
    return pRefersto; 
}


TPRRE_TRANSFORMED_VERTEX* PRREObject3D::PRREObject3DImpl::getTransformedVertices(TPRREbool implicitAccessSubobject)
{
    if ( implicitAccessSubobject && _pOwner->isLevel1() && (_pOwner->getCount() == 1) )
        return ((PRREObject3D*) (_pOwner->getAttachedAt(0)))->getTransformedVertices();
    else
        return pVerticesTransf;
}


PRREVector& PRREObject3D::PRREObject3DImpl::getAngleVec()
{
    return vAngle;
} // getAngleVec()


const PRREVector& PRREObject3D::PRREObject3DImpl::getAngleVec() const
{
    return vAngle;
} // getAngleVec()


PRREVector PRREObject3D::PRREObject3DImpl::getScaledSizeVec() const
{
    return PRREVector(
        _pOwner->getSizeVec().getX() * vScaling.getX(),
        _pOwner->getSizeVec().getY() * vScaling.getY(),
        _pOwner->getSizeVec().getZ() * vScaling.getZ());
} // getScaledSizeVec()


const PRREVector& PRREObject3D::PRREObject3DImpl::getScaling() const
{
    return vScaling;
} // getScaling()


void PRREObject3D::PRREObject3DImpl::SetScaling(TPRREfloat value)
{
    vScaling.Set(value, value, value);
} // SetScaling()


void PRREObject3D::PRREObject3DImpl::SetScaling(const PRREVector& value)
{
    vScaling = value;
} // SetScaling()


void PRREObject3D::PRREObject3DImpl::Scale(TPRREfloat value)
{
    vScaling.Set( vScaling.getX() * value, vScaling.getY() * value, vScaling.getZ() * value );
} // Scale()


void PRREObject3D::PRREObject3DImpl::Scale(const PRREVector& value)
{
    vScaling.Set( vScaling.getX() * value.getX(), vScaling.getY() * value.getY(), vScaling.getZ() * value.getZ() );
} // Scale()


TPRREbool PRREObject3D::PRREObject3DImpl::isVisible() const
{
    return bVisible;
} // isVisible()


void PRREObject3D::PRREObject3DImpl::SetVisible(TPRREbool state)
{
    bVisible = state;
} // SetVisible()


void PRREObject3D::PRREObject3DImpl::Show()
{
    SetVisible(true);
} // Show()


void PRREObject3D::PRREObject3DImpl::Hide()
{
    SetVisible(false);
} // Hide()


TPRREbool PRREObject3D::PRREObject3DImpl::isColliding_TO_BE_REMOVED() const
{
    return bColliding;
}


void PRREObject3D::PRREObject3DImpl::SetColliding_TO_BE_REMOVED(TPRREbool value)
{
    bColliding = value;
}


TPRRE_ROTATION_ORDER PRREObject3D::PRREObject3DImpl::getRotationOrder() const
{
    return rotation;
} // getRotationOrder()


void PRREObject3D::PRREObject3DImpl::SetRotationOrder(TPRRE_ROTATION_ORDER value)
{
    rotation = value;
} // SetRotationOrder()


TPRREbool PRREObject3D::PRREObject3DImpl::isLit() const
{
    return bAffectedByLights;
} // isLit()


void PRREObject3D::PRREObject3DImpl::SetLit(TPRREbool value)
{
    bAffectedByLights = value;
} // SetLit()


TPRREbool PRREObject3D::PRREObject3DImpl::isDoubleSided() const
{
    return bDoubleSided;
} // isDoubleSided()


void PRREObject3D::PRREObject3DImpl::SetDoubleSided(TPRREbool value)
{
    bDoubleSided = value;
} // SetDoubleSided()


TPRREbool PRREObject3D::PRREObject3DImpl::isWireframed() const
{
    return bWireframe;
} // isWireframed()


void PRREObject3D::PRREObject3DImpl::SetWireframed(TPRREbool value)
{
    bWireframe = value;
} // SetWireframed()


TPRREbool PRREObject3D::PRREObject3DImpl::isWireframedCulled() const
{
    return bWireframedCull;
} // isWireframedCulled()


void PRREObject3D::PRREObject3DImpl::SetWireframedCulled(TPRREbool value)
{
    bWireframedCull = value;
} // SetWireframedCulled()


TPRREbool PRREObject3D::PRREObject3DImpl::isAffectingZBuffer() const
{
    return bAffectZBuffer;
} // isAffectingZBuffer()


void PRREObject3D::PRREObject3DImpl::SetAffectingZBuffer(TPRREbool value)
{
    bAffectZBuffer = value;
} // SetAffectingZBuffer()


TPRREbool PRREObject3D::PRREObject3DImpl::isTestingAgainstZBuffer() const
{
    return bAllowZTesting;
} // isTestingAgainstZBuffer()


void PRREObject3D::PRREObject3DImpl::SetTestingAgainstZBuffer(TPRREbool value)
{
    bAllowZTesting = value;
} // SetTestingAgainstZBuffer()


TPRREbool PRREObject3D::PRREObject3DImpl::isStickedToScreen() const
{
    return bStickedToScreen;
} // isStickedToScreen()


void PRREObject3D::PRREObject3DImpl::SetStickedToScreen(TPRREbool value)
{
    bStickedToScreen = value;
    SetDoubleSided(true);
    SetLit(false);
    SetTestingAgainstZBuffer(false);
} // SetStickedToScreen()


TPRREuint PRREObject3D::PRREObject3DImpl::getUsedSystemMemory() const
{
    return (
        sizeof(*this) +
        nFbBuffer_h * sizeof(GLfloat) +
        sizeof(TPRRE_TRANSFORMED_VERTEX) * _pOwner->getVertexIndicesCount(false)
    );
} // getUsedSystemMemory()     


void PRREObject3D::PRREObject3DImpl::Draw(bool bLighting)
{
    // caller renderer is expected to check for GL errors, so we don't check them here

    if ( !bVisible )
        return;

    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {
        DrawSW();
        return;
    }

    // see if we are parent or referring to another object i.e. we are cloned object
    if ( _pOwner->isLevel1() || getReferredObject() )
    {
        // Currently 3D objects are 2-level entities:
        // first level (parent) has no geometry, owns subobjects, sets basic things,
        // while second level (subobjects) own geometry, inherit basic things set by parent.

        // So transformations and other basic things are set by parent objects having subobjects.
        // Or by cloned objects which refer to another original objects but still have their own position, angle, etc.
        
        ApplyTransformations();
        PrepareGLbeforeDraw(bLighting);

        // take care of attached objects (subobjects)
        // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
        PRREObject3D* pWhichParent = getReferredObject() ? getReferredObject() : _pOwner;
        pWhichParent->pImpl->bParentInitiatedOperation = true;
        for (TPRREint i = 0; i < pWhichParent->getCount(); i++)
            ((PRREObject3D*)pWhichParent->getAttachedAt(i))->Draw(bLighting);
        pWhichParent->pImpl->bParentInitiatedOperation = false;
        return;
    }

    // if we reach this point then either our parent is drawing us as its subobject, or a cloned object is drawing us on behalf of our parent

    // subobject must ignore draw if its Draw() was not called by its parent level-1 object but someone else from outside ...
    if ( !((PRREObject3D*)_pOwner->getManager())->pImpl->bParentInitiatedOperation )
    {
        _pOwner->getManagedConsole().EOLn("Draw() of subobject called outside of its level-1 parent object, ignoring draw!");
        return;
    }

    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        if ( _pOwner->getMaterial().isMultiTextured() )
        {
            // enable blending of 2nd layer
            glEnable(GL_BLEND);
	          glBlendFunc(getGLblendFromPRREblend(_pOwner->getMaterial().getSourceBlendFunc(1)), getGLblendFromPRREblend(_pOwner->getMaterial().getDestinationBlendFunc(1)));
            LoadTextureIntoTMU( _pOwner->getMaterial().getTexture(0), 0 );
            LoadTextureIntoTMU( _pOwner->getMaterial().getTexture(1), 1 );
        }
        else
        {
            // disable blending only if base layer is not blended ...
            if ( ! PRREObject3DManager::isBlendFuncBlends(_pOwner->getMaterial().getSourceBlendFunc(), _pOwner->getMaterial().getDestinationBlendFunc()) )
                glDisable(GL_BLEND);
            LoadTextureIntoTMU( _pOwner->getMaterial().getTexture(0), 0 );
            LoadTextureIntoTMU( PGENULL, 1 );
        }
    }
    else
    {
        // disable blending only if base layer is not blended ...
        if ( ! PRREObject3DManager::isBlendFuncBlends(_pOwner->getMaterial().getSourceBlendFunc(), _pOwner->getMaterial().getDestinationBlendFunc()) )
            glDisable(GL_BLEND);
        LoadTextureIntoTMU( _pOwner->getMaterial().getTexture() );
    }

    /* currently not supporting any vendor-specific mode */
    if ( BITF_READ(_pOwner->getVertexTransferMode(),PRRE_VT_VENDOR_BITS,3) != 0 )
        return;

    if ( pFbBuffer == NULL )
    {
        /* We create this buffer only once at first run. */
        /* We intentionally switch to feedback mode only once since now it is for debugging purposes only, for breakpoint, etc.
           but in the future when HW transform and SW rasterization can be combined, this will be controlled by public API */
       
        /* num of values can be stored in this buffer */
        /* we need to store not only the transformed values but also some extra values for grammar */
        /* in this case, the items placed in this array are polygon items (GL_POLYGON_TOKEN) which look like this:
            GL_POLYGON_TOKEN 3 (value value value value (value value value value) (value value value value))
            So for a standard cube where each side has 2 triangles, it is 12 triangles in total, meaning
            12 polygons. GL_POLYGON_TOKEN itself is 1 value, 3 itself is also 1 value, and then 3*12 values for 3 vertices.
            So the total num of values is: numOfTriangles * (2+3*12)
            Example with some real data is in later comment with debugbuffer[]. */

       
        nFbBuffer_h = GLsizei(ceil((_pOwner->getVerticesCount(false) / 3.0f))) * (2+3*12);
        try
        {
            /* TODO: probably in future we should rather use pVerticesTransf here as well since we already have it for that purpose, right? ;) */
            pFbBuffer = new GLfloat[nFbBuffer_h];

            /* unfortunately only the most detailed option GL_4D_COLOR_TEXTURE will give us the w-coord of vertices so we need to use that */
            if ( pglFeedbackBuffer(nFbBuffer_h, GL_4D_COLOR_TEXTURE, pFbBuffer) )
            {
                // If you want all vertices to be transformed and catched in feedback mode then dont forget to disable culling and depth testing (maybe only 1 is needed to be disabled).
                //glDisable(GL_DEPTH_TEST);
                glDisable(GL_CULL_FACE); /* otherwise only the front facing side of cube would be written to feedback buffer */

                glRenderMode(GL_FEEDBACK);
            }
        }
        catch (const std::bad_alloc&)
        {
            _pOwner->getManagedConsole().EOLn("ERROR: PRREObject3D::PRREObject3DImpl::Draw() failed to allocate pFbBuffer!");
        }
    } 

    ((PRREVertexTransfer*)_pOwner)->pImpl->TransferVertices();

    /* The number of values (not vertices) transferred to the feedback buffer. */
    const GLint nFbBufferWritten_h = glRenderMode(GL_RENDER);

    GLfloat debugbuffer[500];

    if ( nFbBufferWritten_h > 0 )
    {
        // we are happy
        for (int i = 0; i < min(500,nFbBufferWritten_h); i++)
            debugbuffer[i] = pFbBuffer[i];
        //Sleep(1); // suitable for placing breakpoint
        /*
            debugbuffer[0] = GL_POLYGON_TOKEN;
            debugbuffer[1] = 3;
            debugbuffer[ 2.. 5] = x y z w (xyz are in window coordinates while w is clip coordinate)
            debugbuffer[ 6.. 9] = r g b a
            debugbuffer[10..13] = texture coords, probably x y u w
            debugbuffer[14..17] = x y z w (xyz are in window coordinates while w is clip coordinate)
            debugbuffer[18..21] = r g b a
            debugbuffer[22..25] = texture coords, probably x y u w
            debugbuffer[26..29] = x y z w (xyz are in window coordinates while w is clip coordinate)
            debugbuffer[30..33] = r g b a
            debugbuffer[34..37] = texture coords, probably x y u w
            debugbuffer[38] = GL_POLYGON_TOKEN;
            debugbuffer[39] = 3;

            example: gamedata\\models\\cube.obj
            assuming that nearplane is 0.1f, farplane is 100.0f, depthrange is [0, 1], fovy is 80
            box
            i=2 (3. vertex) debugbuffer[26]
                model space        -> world space       -> view/eye space    -> clip space                   -> normalized device coords     -> screen coords
            sw: [-0.5, -0.5, -0.5] -> [-0.5, -0.5, 2.5] -> [-0.5, -0.5, 2.5] -> [-0.446, -0.595, 2.348, 2.5] -> [-0.178, -0.238, 0.939, 2.5] -> [328, 228, 0.961, 2.5]
            hw:                                                                                                                                 [328, 228, 0.961, 2.5]

            i=6 (7. vertex) debugbuffer[?]
                model space        -> world space       -> view/eye space    -> clip space                   -> normalized device coords     -> screen coords
            sw: [ 0.5,  0.5,  0.5] -> [ 0.5,  0.5, 3.5] -> [ 0.5,  0.5, 3.5] -> [ 0.446,  0.595, 3.368, 3.5] -> [ 0.127,  0.170, 0.962, 3.5] -> [451, 351, 0.972, 3.5]
            hw:                                                                                                                                 [451, 351, 0.972, 3.5]

            ...
        */
    }
    else
    {
        // we were already in GL_RENDER (0) mode, OR nothing was returned, OR we were in FEEDBACK but buffer was not enough to hold all values (<0) ...
    }
} // Draw()


// ############################## PROTECTED ##############################



/**
    @param owner                 The public Object3D class instance owning this pimpl object.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.
*/
PRREObject3D::PRREObject3DImpl::PRREObject3DImpl(
    PRREObject3D* owner,
    const TPRRE_VERTEX_MODIFYING_HABIT& vmod,
    const TPRRE_VERTEX_REFERENCING_MODE& vref,
    TPRREbool bForceUseClientMemory )
{
    _pOwner = owner;
    _pOwner->getManagedConsole().OLnOI("PRREObject3D() ...");

    pRefersto = NULL;
    bAffectedByLights = bAffectZBuffer = bAllowZTesting = true;
    bVisible = true;
    bParentInitiatedOperation = false;
    bColliding = bDoubleSided = false;
    bWireframe = bWireframedCull = bStickedToScreen = false;

    vScaling.Set(1.0f, 1.0f, 1.0f);
    rotation = PRRE_YXZ;
    // nTexcoordsVBO to be resized by manager outside

    selectVertexTransferMode(vmod, vref, bForceUseClientMemory);
    pVerticesTransf = PGENULL;
    pFbBuffer = PGENULL;

    nFbBuffer_h = 0;

    _pOwner->getManagedConsole().SOLnOO("Done!");
} // PRREObject3D()


PRREObject3D::PRREObject3DImpl::PRREObject3DImpl(const PRREObject3DImpl&)
{

}


PRREObject3D::PRREObject3DImpl& PRREObject3D::PRREObject3DImpl::operator=(const PRREObject3DImpl&)
{
    return *this;
}


// ############################### PRIVATE ###############################


/**
    Loads the given texture into the texture mapping unit.

    @param tex  Texture to be loaded. If NULL, texturing will be disabled on the specified texture mapping unit.
    @param iTMU Into which TMU we want to load the texture. Currently it must be either 0 or 1.
*/
void PRREObject3D::PRREObject3DImpl::LoadTextureIntoTMU(const PRRETexture* tex, TPRREuint iTMU) const
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    // TODO: the PR00FPS-style lasttex method is not present here currently ... later it should be implemented.
    // A general TMU manager should record which TMU holds which texture, so that can avoid unnecessary glBindTexture() calls.

    // currently we support only 2 textured layers ...
    if ( iTMU > 1 )
    {
        _pOwner->getManagedConsole().EOLn("ERROR: LoadTextureIntoTMU(tex, %d)", iTMU);
        _ASSERT( false );
    }

    if ( PRREhwInfo::get().getVideo().isMultiTexturingSupported() )
    {
        glActiveTextureARB(GL_TEXTURE0_ARB + iTMU);
        if ( tex != PGENULL )
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex->getInternalNum());
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else
            glDisable(GL_TEXTURE_2D);
    }
    else
    {
        if ( tex != PGENULL )
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex->getInternalNum());
            if ( isStickedToScreen() )
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            else
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }
        else
            glDisable(GL_TEXTURE_2D);
    }
} // LoadTextureIntoTMU()


/**
    Applies transformations to the current modelview matrix based on the given object.
*/
void PRREObject3D::PRREObject3DImpl::ApplyTransformations() const    
{
    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    glTranslatef(_pOwner->getPosVec().getX(), _pOwner->getPosVec().getY(), _pOwner->getPosVec().getZ());
    
    switch ( getRotationOrder() )
    {
    case PRRE_XYZ:
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
        break;
    case PRRE_XZY:
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getY(), 0,1,0);
        break;
    case PRRE_YXZ:
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
         break;
    case PRRE_YZX:
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getX(), 1,0,0);
        break;
    case PRRE_ZXY:
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getX(), 1,0,0);
        glRotatef(getAngleVec().getY(), 0,1,0);
        break;
    case PRRE_ZYX:
        glRotatef(getAngleVec().getZ(), 0,0,1);
        glRotatef(getAngleVec().getY(), 0,1,0);
        glRotatef(getAngleVec().getX(), 1,0,0);
        break;
    } // switch

    glScalef(getScaling().getX(), getScaling().getY(), getScaling().getZ());
    /*
        Well, mirror-transforming an object (scaling by negative value) makes its faces flip:
        https://www.gamedev.net/forums/topic/640616-negative-scaling-flips-face-winding-affects-backface-culling/
        I wanted to X-mirror objects loaded by tmcsgfxlib-to-prre wrapper for legacy projects, because I noticed they appeared mirrored compared to proofps engine behavior.
        So I noticed when setting negative scale on X axis, faces also flipped, so objects became inside-out.
        This is expected since vertices' winding order also flips because we still feed triangle vertices in same order even though triangles become mirrored.
        But I don't treat this as a legacy-project-only issue, since even a new project with the new engine could come up for any reason with negative scaling for any mirroring reason.
        So, this is rather a feature: we need to detect negative scaling factor and change front face winding order on-the-fly.
    */
    if ( getScaling().getX() * getScaling().getY() * getScaling().getZ() <= (0.0f-PFL::E) )
        glFrontFace(GL_CW);
    else
        glFrontFace(GL_CCW);
}


void PRREObject3D::PRREObject3DImpl::PrepareGLbeforeDraw(bool bLighting) const
{
    /*AmbientLightPos[0] =  cam.getX() - obj->getPosVec().getX();
    AmbientLightPos[1] =  cam.getY() - obj->getPosVec().getY();
    AmbientLightPos[2] = -cam.getZ() - obj->getPosVec().getZ();*/
    /*AmbientLightPos[0] = cam.getX();
    AmbientLightPos[1] = cam.getY();
    AmbientLightPos[2] = -cam.getZ()-2.0f;
    AmbientLightPos[3] = 1.0f;
    glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos);  */

    /* Make sure we don't call GL functions when no accelerator is present */
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return;

    if ( isDoubleSided() )
    {
        glDisable(GL_CULL_FACE);
        if ( isLit() && bLighting )
        {
            glEnable(GL_LIGHTING);
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
            //glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos); 
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
    }
    else
    {
        glEnable(GL_CULL_FACE);
        if ( isLit() && bLighting )
        {
            glEnable(GL_LIGHTING);
            glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
            //glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos);
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
    } // isDoubleSided()

    if ( isWireframed() )
    {
        glDisable(GL_LIGHTING);
        if ( !isWireframedCulled() )
            glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        if ( isLit() && bLighting )
        {
            glEnable(GL_LIGHTING);
            //glLightfv(GL_LIGHT0, GL_POSITION, AmbientLightPos);
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } // isWireframed()

    if ( isTestingAgainstZBuffer() )
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    // TODO: nope, we shouldnt specify color this way because glColor4f() is called per-vertex anyway later
    // getTextureEnvColor() is used by texture environment mode / function, not blending!
    // we should rather use glBlendColor() for this with GL_*_CONSTANT_* blendfunc, or modify the code not to specify color per-vertex
    
    glColor4f(
        _pOwner->getMaterial().getTextureEnvColor().getRedAsFloat(),
        _pOwner->getMaterial().getTextureEnvColor().getGreenAsFloat(),
        _pOwner->getMaterial().getTextureEnvColor().getBlueAsFloat(),
        _pOwner->getMaterial().getTextureEnvColor().getAlphaAsFloat() );
                
                
    if ( PRREObject3DManager::isBlendFuncBlends(_pOwner->getMaterial().getSourceBlendFunc(), _pOwner->getMaterial().getDestinationBlendFunc()) )
    {
        glEnable(GL_BLEND);
        glBlendFunc(getGLblendFromPRREblend(_pOwner->getMaterial().getSourceBlendFunc()), getGLblendFromPRREblend(_pOwner->getMaterial().getDestinationBlendFunc()));
        glAlphaFunc(GL_GREATER, 0.1f);
        glEnable(GL_ALPHA_TEST);
    }
    else
    {
        glDisable(GL_BLEND);
        glDisable(GL_ALPHA_TEST);
    }
}


void PRREObject3D::PRREObject3DImpl::DrawSW()
{
    if ( !bVisible )
        return;

    // see if we are parent or referring to another object i.e. we are cloned object
    if ( _pOwner->isLevel1() || getReferredObject() )
    {
        // Currently 3D objects are 2-level things: first level (parent) has no geometry, owns subobjects, sets basic things,
        // while second level (subobjects) own geometry, inherit basic things set by parent.

        // So transformations and other basic things are set by parent objects having subobjects.
        // Or by cloned objects which refer to another original objects but still have their own position, angle, etc.

        ApplyTransformations();

        // take care of attached objects (subobjects)
        // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
        PRREObject3D* pWhichParent = getReferredObject() ? getReferredObject() : _pOwner;
        for (TPRREint i = 0; i < pWhichParent->getCount(); i++)
            ((PRREObject3D*)pWhichParent->getAttachedAt(i))->Draw(false);
        return;
    }

    // actual draw here
    // todo: wtf, we should finally decide if it is renderer's responsibility to actually render an object or object's responsibility?
}


/*
   PRREObject3D
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREObject3D::~PRREObject3D()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRRETexture()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRREObject3D::getManagedConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRREObject3D::getLoggerModuleName()
{
    return "PRREObject3D";
} // getLoggerModuleName()


/**
    The details of this function are described in VertexTransfer class.
    If this is a cloned object, the returned value is the same as the referred object's value.
*/
TPRRE_VERTEX_MODIFYING_HABIT PRREObject3D::getVertexModifyingHabit() const
{
    return pImpl->getVertexModifyingHabit();
} // getVertexModifyingHabit()


/**
    The details of this function are described in VertexTransfer class.
    Same functionality as defined originally in VertexTransfer but extended with a check for being a cloned object or not.
    This function has no effect when called for a cloned object.
    @param vmod Vertex modifying habit to be set.
    @return     True on success, false otherwise, including if called for a cloned object.
*/
TPRREbool PRREObject3D::setVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod)
{
    return pImpl->setVertexModifyingHabit(vmod);
} // setVertexModifyingHabit()


/**
    The details of this function are described in VertexTransfer class.
    If this is a cloned object, the returned value is the same as the referred object's value.
*/
TPRRE_VERTEX_REFERENCING_MODE PRREObject3D::getVertexReferencingMode() const
{
    return pImpl->getVertexReferencingMode();
} // getVertexReferencingMode()

 
/**
    The details of this function are described in VertexTransfer class.
    Same functionality as defined originally in VertexTransfer but extended with a check for being a cloned object or not.
    This function has no effect when called for a cloned object.
    @param  vref Vertex referencing mode to be set.
    @return      True on success, false otherwise, including if called for a cloned object.
*/
TPRREbool PRREObject3D::setVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref)
{
    return pImpl->setVertexReferencingMode(vref);
} // setVertexReferencingMode()


/**
    The details of this function are described in VertexTransfer class.
    If this is a cloned object, the returned value is the same as the referred object's value.
*/
TPRRE_VERTEX_TRANSFER_MODE PRREObject3D::getVertexTransferMode() const
{
    return pImpl->getVertexTransferMode();
} // getVertexTransferMode()


/**
    The details of this function are described in VertexTransfer class.
    Same functionality as defined originally in VertexTransfer but extended with a check for being a cloned object or not.
    This function has no effect when called for a cloned object.
    @param  vtrans Vertex referencing mode to be set.
    @return        True on success, false otherwise, including if called for a cloned object.
*/
TPRREbool PRREObject3D::setVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    return pImpl->setVertexTransferMode(vtrans);
} // setVertexTransferMode()


/**
    Gets the original object which was cloned to create this object.
    @return NULL if this is a non-cloned object, otherwise the pointer to the original object which was cloned to create this cloned object.
*/
PRREObject3D* PRREObject3D::getReferredObject() const
{
    return pImpl->getReferredObject();
}


/**
    Gets the pointer to transformed vertices.
    These are generated only during first rendering.
    This value is irrelevant for a level-1 object since the geometry is owned by its level-2 subobjects.
    Still the returned value for a level-1 object can be a non-NULL value, see below.

    @return Pointer to transformed vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's transformed vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const TPRRE_TRANSFORMED_VERTEX* PRREObject3D::getTransformedVertices(TPRREbool implicitAccessSubobject) const
{
    return pImpl->getTransformedVertices(implicitAccessSubobject);
} // getVertices(false)


/**
    Gets the pointer to transformed vertices.
    These are generated only during first rendering.
    This value is irrelevant for a level-1 object since the geometry is owned by its level-2 subobjects.
    Still the returned value for a level-1 object can be a non-NULL value, see below.

    @return Pointer to transformed vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's transformed vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRRE_TRANSFORMED_VERTEX* PRREObject3D::getTransformedVertices(TPRREbool implicitAccessSubobject)
{
    return pImpl->getTransformedVertices(implicitAccessSubobject);
} // getVertices(false)


/**
    Gets the rotation angles.
    Rotation angle is currently ignored for level-2 objects, they are rotated by the same factor as their level-1 parent object using the same pivot point.
    @return Rotation angles vector.
*/
PRREVector& PRREObject3D::getAngleVec()
{
    return pImpl->getAngleVec();
} // getAngleVec()


/**
    Gets the rotation angles.
    Rotation angle is currently ignored for level-2 objects, they are rotated by the same factor as their level-1 parent object using the same pivot point.
    @return Rotation angles vector.
*/
const PRREVector& PRREObject3D::getAngleVec() const
{
    return pImpl->getAngleVec();
} // getAngleVec()


/**
    Gets the base sizes.
    The Mesh3D part of a cloned object does not have its own geometry, thus even Mesh3D::RecalculateSize() would calculate it to 0.
    Thus if we are a cloned object, we need to return the size of the object we are referring to.
*/
const PRREVector& PRREObject3D::getSizeVec() const
{
    return (getReferredObject() == PGENULL) ? PRREMesh3D::getSizeVec() : getReferredObject()->getSizeVec();
}


/**
    Gets the real sizes considering the geometry size calculated from vertex data and the current scaling factor.
    @return Real sizes vector.
*/
PRREVector PRREObject3D::getScaledSizeVec() const
{
    return pImpl->getScaledSizeVec();
} // getScaledSizeVec()


/**
    Gets the scaling factor.
    @return Scaling.
*/
const PRREVector& PRREObject3D::getScaling() const
{
    return pImpl->getScaling();
} // getScaling()


/**
    Sets the scaling factor to given scalar.
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
*/
void PRREObject3D::SetScaling(TPRREfloat value)
{
    pImpl->SetScaling(value);
} // SetScaling()


/**
    Sets the scaling factor to given vector.
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
*/
void PRREObject3D::SetScaling(const PRREVector& value)
{
    pImpl->SetScaling(value);
} // SetScaling()


/**
    Scales by the given scalar value. 
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
*/
void PRREObject3D::Scale(TPRREfloat value)
{
    pImpl->Scale(value);
} // Scale()


/**
    Scales by the given vector.
    Scaling factor is currently ignored for level-2 objects, they are scaled by the same factor as their level-1 parent object.
*/
void PRREObject3D::Scale(const PRREVector& value)
{
    pImpl->Scale(value);
} // Scale()


/**
    Gets the visibility state.
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
    @return True, if visible, false otherwise.
*/
TPRREbool PRREObject3D::isVisible() const
{
    return pImpl->isVisible();
} // isVisible()


/**
    Sets the visibility state.
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
*/
void PRREObject3D::SetVisible(TPRREbool state)
{
    pImpl->SetVisible(state);
} // SetVisible()


/**
    Sets the visibility state to true.
    Equivalent to SetVisible(true).
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
*/
void PRREObject3D::Show()
{
    pImpl->Show();
} // Show()


/**
    Sets the visibility state to false.
    Equivalent to SetVisible(false).
    If an object is not visible, it is not rendered.
    Currently the visibility state is ignored for level-2 objects, the state of their level-1 parent object is applied.
*/
void PRREObject3D::Hide()
{
    pImpl->Hide();
} // Hide()


/**
    Gets whether colliding is enabled.
    Legacy function unrelated to the purpose of this class, will need to be removed!
    @return True, if colliding is enabled, false otherwise.
*/
TPRREbool PRREObject3D::isColliding_TO_BE_REMOVED() const
{
    return pImpl->isColliding_TO_BE_REMOVED();
}


/**
    Sets whether colliding is enabled.
    Legacy function unrelated to the purpose of this class, will need to be removed!
*/
void PRREObject3D::SetColliding_TO_BE_REMOVED(TPRREbool value)
{
    pImpl->SetColliding_TO_BE_REMOVED(value);
}


/**
    Gets the rotation order.
    Rotation order is currently ignored for level-2 objects, they are rotated the same way as their level-1 parent object.
    @return Rotation order.
*/
TPRRE_ROTATION_ORDER PRREObject3D::getRotationOrder() const
{
    return pImpl->getRotationOrder();
} // getRotationOrder()


/**
    Sets the rotation order.
    Rotation order is currently ignored for level-2 objects, they are rotated the same way as their level-1 parent object.
*/
void PRREObject3D::SetRotationOrder(TPRRE_ROTATION_ORDER value)
{
    pImpl->SetRotationOrder(value);
} // SetRotationOrder()


/**
    Gets the lit state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if lit, false otherwise.
*/
TPRREbool PRREObject3D::isLit() const
{
    return pImpl->isLit();
} // isLit()


/**
    Sets the lit state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetLit(TPRREbool value)
{
    pImpl->SetLit(value);
} // SetLit()


/**
    Gets the double sided state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if double sided, false otherwise.
*/
TPRREbool PRREObject3D::isDoubleSided() const
{
    return pImpl->isDoubleSided();
} // isDoubleSided()


/**
    Sets the double sided state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetDoubleSided(TPRREbool value)
{
    pImpl->SetDoubleSided(value);
} // SetDoubleSided()


/**
    Gets the wireframed state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if wireframed, false otherwise.
*/
TPRREbool PRREObject3D::isWireframed() const
{
    return pImpl->isWireframed();
} // isWireframed()


/**
    Sets the wireframed state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetWireframed(TPRREbool value)
{
    pImpl->SetWireframed(value);
} // SetWireframed()


/**
    Gets the wireframed culling state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if culling is enabled in wireframed state, false otherwise.
*/
TPRREbool PRREObject3D::isWireframedCulled() const
{
    return pImpl->isWireframedCulled();
} // isWireframedCulled()


/**
    Sets the wireframed culling state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetWireframedCulled(TPRREbool value)
{
    pImpl->SetWireframedCulled(value);
} // SetWireframedCulled()


/**
    Gets whether we write to the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if we write to Z-Buffer while rendering, false otherwise.
*/
TPRREbool PRREObject3D::isAffectingZBuffer() const
{
    return pImpl->isAffectingZBuffer();
} // isAffectingZBuffer()


/**
    Sets whether we write to the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetAffectingZBuffer(TPRREbool value)
{
    pImpl->SetAffectingZBuffer(value);
} // SetAffectingZBuffer()


/**
    Gets whether we test against the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if we test against the Z-Buffer while rendering, false otherwise.
*/
TPRREbool PRREObject3D::isTestingAgainstZBuffer() const
{
    return pImpl->isTestingAgainstZBuffer();
} // isTestingAgainstZBuffer()


/**
    Sets whether we test against the Z-Buffer while rendering.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetTestingAgainstZBuffer(TPRREbool value)
{
    pImpl->SetTestingAgainstZBuffer(value);
} // SetTestingAgainstZBuffer()


/**
    Gets the sticked-to-screen state.
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
    @return True if sticked to screen, false otherwise.
*/
TPRREbool PRREObject3D::isStickedToScreen() const
{
    return pImpl->isStickedToScreen();
} // isStickedToScreen()


/**
    Sets the sticked-to-screen state.
    Sticked to screen means the object is rendered with orthogonal projection and not overlapped by non-sticked objects.
    Suitable for 2D object rendering such as GUI elements.
    Note that this call not only changes sticked state of the object but also calls the following methods:
     - SetDoubleSided(true);
     - SetLit(false);
     - SetTestingAgainstZBuffer(false).
    This property is currently ignored for level-2 objects, they inherit this property from their level-1 parent object.
*/
void PRREObject3D::SetStickedToScreen(TPRREbool value)
{
    pImpl->SetStickedToScreen(value);
} // SetStickedToScreen()


/**
    Gets the amount of allocated system memory.
    It includes the allocated Material size as well (getMaterial(false)).
    Level-1 (parent) objects summarize the memory usage of their level-2 subobjects and include it in the returned value.

    @return Amount of allocated system memory in Bytes.
*/
TPRREuint PRREObject3D::getUsedSystemMemory() const
{
    TPRREuint sumSubObjectMemoryUsage = 0;
    for (TPRREint i = 0; i < getSize(); i++)
    {
        if ( getAttachedAt(i) == PGENULL )
            continue;
        sumSubObjectMemoryUsage += getAttachedAt(i)->getUsedSystemMemory();
    }
    return sumSubObjectMemoryUsage +
        PRREFiledManaged::getUsedSystemMemory() - sizeof(PRREFiledManaged) +
        PRREMesh3D::getUsedSystemMemory() - sizeof(PRREMesh3D) +
        PRREVertexTransfer::getUsedSystemMemory() - sizeof(PRREVertexTransfer) + 
        sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


/**
    The details of this function are described in VertexTransfer class.
    If this is a cloned object, the returned value is 0.

    @return Amount of allocated video memory in Bytes for storing geometry of the underlying mesh, including all subobjects.
            0 is returned if this is a cloned object.
*/
TPRREuint PRREObject3D::getUsedVideoMemory() const
{
    // cloned object must report 0
    if ( getReferredObject() )
        return 0;
    
    return PRREVertexTransfer::getUsedVideoMemory(); 
} // getUsedVideoMemory()


/**
    Draws the object.
    This is only valid from outside for level-1 objects.
    The call is ignored by level-2 subobjects. Only their level-1 parent object can call on its subobjects.
*/
void PRREObject3D::Draw(bool bLighting)
{
    pImpl->Draw(bLighting);
} // Draw()


// ############################## PROTECTED ##############################


/**
    Only PRREObject3DManager creates it.

    @param matMgr                A MaterialManager instance to be used for constructing of ancestor class.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.                             
*/
PRREObject3D::PRREObject3D(
    PRREMaterialManager& matMgr,
    const TPRRE_VERTEX_MODIFYING_HABIT& vmod,
    const TPRRE_VERTEX_REFERENCING_MODE& vref,
    TPRREbool bForceUseClientMemory ) : PRREVertexTransfer(matMgr, vmod, vref, bForceUseClientMemory)
{
    getManagedConsole().OLnOI("PRREObject3D() ...");
    pImpl = new PRREObject3DImpl(this, vmod, vref, bForceUseClientMemory);
    getManagedConsole().SOLnOO("Done!");
} // PRREObject3D()


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRREObject3D::PRREObject3D(const PRREObject3D& other)
    : PRREVertexTransfer(other)
{
}


PRREObject3D& PRREObject3D::operator=(const PRREObject3D&)
{
    return *this;
}


// ############################### PRIVATE ###############################


