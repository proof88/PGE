#pragma once

/*
    ###################################################################################
    PureRendererHWfixedPipe.h
    This file is part of PURE.
    External header.
    Fixed-Pipeline HW Renderer class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../../../../../CConsole/CConsole/src/CConsole.h"
#include "../../../../../../PFL/PFL/PFL.h"
#include "PureIRenderer.h"
#include "../Display/PureScreen.h"
#include "../Display/PureWindow.h"
#include "../Hardware/PureHwInfo.h"

/* what purpose each bit stands for in TPure_RENDER_HINT */
#define Pure_RH_RENDER_PATH_BITS             0      /**< Which rendering path to use. */
#define Pure_RH_OQ_METHOD_BITS               3      /**< Which occlusion query method to use. */
#define Pure_RH_OQ_DRAW_BOUNDING_BOXES_BIT   5      /**< Draw colorized bounding boxes for occlusion-tested objects. */
#define Pure_RH_OQ_DRAW_IF_QUERY_PENDING_BIT 6      /**< Draw occlusion-tested object with full detail when its query is still pending. */
#define Pure_RH_ORDERING_BY_DISTANCE_BIT     7      /**< Order the rendering of objects based on their distance to camera. */

/* 3 bits for selecting 1 rendering path, starting from Pure_RH_RENDER_PATH_BITS */
#define Pure_RH_RP_LEGACY_PR00FPS   0u  /**< The legacy PR00FPS-style rendering path. */
#define Pure_RH_RP_DISTANCE_ORDERED 1u  /**< The legacy but using different Object3D containers, ordered by Z-distance to camera. */
#define Pure_RH_RP_OCCLUSION_CULLED 2u  /**< Rendering with occlusion culling, using different Object3D containers. */
#define Pure_RH_RP_RSRVD_1          3u
#define Pure_RH_RP_RSRVD_2          4u
#define Pure_RH_RP_RSRVD_3          5u
#define Pure_RH_RP_RSRVD_4          6u
#define Pure_RH_RP_RSRVD_5          7u

/* 2 bits for selecting 1 occlusion query method, starting from Pure_RH_OQ_METHOD_BITS */
#define Pure_RH_OQ_METHOD_SYNC    0u  /**< Sync: all occlusion queries must be finished within the same frame they are issued. */
#define Pure_RH_OQ_METHOD_ASYNC   1u  /**< Async: occlusion queries can finish in consecutive frames. */
#define Pure_RH_OQ_METHOD_RSRVD_1 2u
#define Pure_RH_OQ_METHOD_RSRVD_2 3u

/* Pure_RH_OQ_DRAW_BOUNDING_BOXES_BIT */
#define Pure_RH_OQ_DRAW_BOUNDING_BOXES_OFF  0u                                       /**< Not drawing bounding boxes for occlusion-tested objects. */
#define Pure_RH_OQ_DRAW_BOUNDING_BOXES_ON   BIT(Pure_RH_OQ_DRAW_BOUNDING_BOXES_BIT)  /**< Draw bounding boxes for occlusion-tested objects. */

/* Pure_RH_OQ_DRAW_EVEN_IF_QUERY_PENDING_BIT */
#define Pure_RH_OQ_DRAW_IF_QUERY_PENDING_OFF  0u                                         /**< Not drawing occlusion-tested objects with full detail if their query is still pending. */
#define Pure_RH_OQ_DRAW_IF_QUERY_PENDING_ON   BIT(Pure_RH_OQ_DRAW_IF_QUERY_PENDING_BIT)  /**< Draw occlusion-tested objects with full detail if their query is still pending. */

/* Pure_RH_ORDERING_BY_DISTANCE_BIT */
#define Pure_RH_ORDERING_BY_DISTANCE_OFF  0u                                     /**< Not ordering the renderin of objects based on their distance to camera. */
#define Pure_RH_ORDERING_BY_DISTANCE_ON   BIT(Pure_RH_ORDERING_BY_DISTANCE_BIT)  /**< Order the rendering of objects based on their distance to camera. */

/**
    Incremental fixed-function hardware renderer path.
    Only deviations from the original PureIRenderer documentation are mentioned here.
    This renderer has multiple rendering paths, controlled by the render hints.
    The default render hints for this renderer is DefaultHintsForHWfixedPipeRenderer.
*/
class PureRendererHWfixedPipe :
    public PureIRenderer
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureRendererHWfixedPipe is included")
#endif

public:
    /** Default render hints for PureRendererHWfixedPipe. */
    static const TPure_RENDER_HINT DefaultHints =
            BITF_PREP(Pure_RH_RP_OCCLUSION_CULLED, Pure_RH_RENDER_PATH_BITS, 3) |
            BITF_PREP(Pure_RH_OQ_METHOD_ASYNC, Pure_RH_OQ_METHOD_BITS, 2) |
            Pure_RH_OQ_DRAW_BOUNDING_BOXES_OFF |
            Pure_RH_OQ_DRAW_IF_QUERY_PENDING_ON |
            Pure_RH_ORDERING_BY_DISTANCE_ON;

    static PureRendererHWfixedPipe& createAndGet(
        PureWindow& _wnd,
        PureScreen& _scr,
        PureHwInfo& _hwinfo );    /**< Creates and gets the singleton implementation instance. */

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                     /**< Returns access to console preset with logger module name as this class. */

    /**
        Not implemented.
        Use RenderScene() to render all objects.
    */
    virtual void RenderObject(PureObject3D& object) = 0;

    virtual void ShowGuiDemo(TPurebool state) = 0;

    virtual TPureuint getLastFrameObjectsVisible() const = 0;                         /**< Returns number of objects allowed to be visible in last frame. */
    virtual TPureuint getLastFrameOccluders() const = 0;                              /**< Returns number of objects that were occluders in last frame. */
    virtual TPureuint getLastFrameOccludeesNonOcclusionTested() const = 0;            /**< Returns number of occludees that had occlusion test disabled in last frame. */
    virtual TPureuint getLastFrameOccludeesOcclusionTested() const = 0;               /**< Returns number of occludees that had occlusion test enabled (not necessarily ongoing) in last frame. */
    virtual TPureuint getLastFrameOccludeesOcclusionTestedAndOccluded() const = 0;    /**< Returns number of occluded occludees in last frame (only counted if occlusion test was also enabled). */
    virtual TPureuint getLastFrameOccludeesOcclusionTestedAndNonOccluded() const = 0; /**< Returns number of non-occluded occludees in last frame (only counted if occlusion test was also enabled). */
    virtual TPureuint getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway() const = 0; /**< Returns number of non-occluded occludees in last frame (only counted if occlusion test was also enabled but visibility was not). */
    virtual TPureuint getLastFrameTransferredVertices() const = 0;                    /**< Returns number of transferred vertices in last frame. */
    virtual TPureuint getLastFrameTransferredTriangles() const = 0;                   /**< Returns number of transferred triangles in last frame. */

}; // PureRendererHWfixedPipe

