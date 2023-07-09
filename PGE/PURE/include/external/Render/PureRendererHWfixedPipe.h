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

#include "../../../../../../Console/CConsole/src/CConsole.h"
#include "../../../../../../PFL/PFL/PFL.h"
#include "../../../../Config/PGEcfgProfiles.h"
#include "PureIRenderer.h"
#include "../Display/PureScreen.h"
#include "../Display/PureWindow.h"
#include "../Hardware/PureHwInfo.h"

/* what purpose each bit stands for in TPURE_RENDER_HINT */
#define PURE_RH_RENDER_PATH_BITS             0      /**< Which rendering path to use. */
#define PURE_RH_OQ_METHOD_BITS               3      /**< Which occlusion query method to use. */
#define PURE_RH_OQ_DRAW_BOUNDING_BOXES_BIT   5      /**< Draw colorized bounding boxes for occlusion-tested objects. */
#define PURE_RH_OQ_DRAW_IF_QUERY_PENDING_BIT 6      /**< Draw occlusion-tested object with full detail when its query is still pending. */
#define PURE_RH_ORDERING_BY_DISTANCE_BIT     7      /**< Order the rendering of objects based on their distance to camera. */

/* 3 bits for selecting 1 rendering path, starting from PURE_RH_RENDER_PATH_BITS */
#define PURE_RH_RP_LEGACY_PR00FPS   0u  /**< The legacy PR00FPS-style rendering path. */
#define PURE_RH_RP_DISTANCE_ORDERED 1u  /**< The legacy but using different Object3D containers, ordered by Z-distance to camera. */
#define PURE_RH_RP_OCCLUSION_CULLED 2u  /**< Rendering with occlusion culling, using different Object3D containers. */
#define PURE_RH_RP_RSRVD_1          3u
#define PURE_RH_RP_RSRVD_2          4u
#define PURE_RH_RP_RSRVD_3          5u
#define PURE_RH_RP_RSRVD_4          6u
#define PURE_RH_RP_RSRVD_5          7u

/* 2 bits for selecting 1 occlusion query method, starting from PURE_RH_OQ_METHOD_BITS */
#define PURE_RH_OQ_METHOD_SYNC    0u  /**< Sync: all occlusion queries must be finished within the same frame they are issued. */
#define PURE_RH_OQ_METHOD_ASYNC   1u  /**< Async: occlusion queries can finish in consecutive frames. */
#define PURE_RH_OQ_METHOD_RSRVD_1 2u
#define PURE_RH_OQ_METHOD_RSRVD_2 3u

/* PURE_RH_OQ_DRAW_BOUNDING_BOXES_BIT */
#define PURE_RH_OQ_DRAW_BOUNDING_BOXES_OFF  0u                                       /**< Not drawing bounding boxes for occlusion-tested objects. */
#define PURE_RH_OQ_DRAW_BOUNDING_BOXES_ON   BIT(PURE_RH_OQ_DRAW_BOUNDING_BOXES_BIT)  /**< Draw bounding boxes for occlusion-tested objects. */

/* PURE_RH_OQ_DRAW_EVEN_IF_QUERY_PENDING_BIT */
#define PURE_RH_OQ_DRAW_IF_QUERY_PENDING_OFF  0u                                         /**< Not drawing occlusion-tested objects with full detail if their query is still pending. */
#define PURE_RH_OQ_DRAW_IF_QUERY_PENDING_ON   BIT(PURE_RH_OQ_DRAW_IF_QUERY_PENDING_BIT)  /**< Draw occlusion-tested objects with full detail if their query is still pending. */

/* PURE_RH_ORDERING_BY_DISTANCE_BIT */
#define PURE_RH_ORDERING_BY_DISTANCE_OFF  0u                                     /**< Not ordering the renderin of objects based on their distance to camera. */
#define PURE_RH_ORDERING_BY_DISTANCE_ON   BIT(PURE_RH_ORDERING_BY_DISTANCE_BIT)  /**< Order the rendering of objects based on their distance to camera. */

/**
    Incremental fixed-function hardware renderer path.
    Only deviations from the original PureIRenderer documentation are mentioned here.
    This renderer has multiple rendering paths, controlled by the render hints.
    The default render hints for this renderer is DefaultHintsForHWfixedPipeRenderer.
*/
class PureRendererHWfixedPipe :
    public PureIRenderer
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureRendererHWfixedPipe is included")
#endif

public:
    /** Default render hints for PureRendererHWfixedPipe. */
    static const TPURE_RENDER_HINT DefaultHints =
            BITF_PREP(PURE_RH_RP_OCCLUSION_CULLED, PURE_RH_RENDER_PATH_BITS, 3) |
            BITF_PREP(PURE_RH_OQ_METHOD_ASYNC, PURE_RH_OQ_METHOD_BITS, 2) |
            PURE_RH_OQ_DRAW_BOUNDING_BOXES_OFF |
            PURE_RH_OQ_DRAW_IF_QUERY_PENDING_ON |
            PURE_RH_ORDERING_BY_DISTANCE_ON;

    static PureRendererHWfixedPipe& createAndGet(
        PGEcfgProfiles& cfgProfiles,
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

    virtual void ShowGuiDemo(TPureBool state) = 0;

    virtual TPureUInt getLastFrameObjectsVisible() const = 0;                         /**< Returns number of objects allowed to be visible in last frame. */
    virtual TPureUInt getLastFrameOccluders() const = 0;                              /**< Returns number of objects that were occluders in last frame. */
    virtual TPureUInt getLastFrameOccludeesNonOcclusionTested() const = 0;            /**< Returns number of occludees that had occlusion test disabled in last frame. */
    virtual TPureUInt getLastFrameOccludeesOcclusionTested() const = 0;               /**< Returns number of occludees that had occlusion test enabled (not necessarily ongoing) in last frame. */
    virtual TPureUInt getLastFrameOccludeesOcclusionTestedAndOccluded() const = 0;    /**< Returns number of occluded occludees in last frame (only counted if occlusion test was also enabled). */
    virtual TPureUInt getLastFrameOccludeesOcclusionTestedAndNonOccluded() const = 0; /**< Returns number of non-occluded occludees in last frame (only counted if occlusion test was also enabled). */
    virtual TPureUInt getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway() const = 0; /**< Returns number of non-occluded occludees in last frame (only counted if occlusion test was also enabled but visibility was not). */
    virtual TPureUInt getLastFrameTransferredVertices() const = 0;                    /**< Returns number of transferred vertices in last frame. */
    virtual TPureUInt getLastFrameTransferredTriangles() const = 0;                   /**< Returns number of transferred triangles in last frame. */

}; // PureRendererHWfixedPipe

