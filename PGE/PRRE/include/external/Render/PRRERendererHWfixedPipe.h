#pragma once

/*
    ###################################################################################
    PRRERendererHWfixedPipe.h
    This file is part of PRRE.
    External header.
    Fixed-Pipeline HW Renderer class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../../../../../CConsole/CConsole/src/CConsole.h"
#include "../../../../../../PFL/PFL/PFL.h"
#include "PRREIRenderer.h"
#include "../Display/PRREScreen.h"
#include "../Display/PRREWindow.h"
#include "../Hardware/PRREhwInfo.h"

/* what purpose each bit stands for in TPRRE_RENDER_HINT */
#define PRRE_RH_RENDER_PATH_BITS             0      /**< Which rendering path to use. */
#define PRRE_RH_OQ_METHOD_BITS               3      /**< Which occlusion query method to use. */
#define PRRE_RH_OQ_DRAW_BOUNDING_BOXES_BIT   5      /**< Draw colorized bounding boxes for occlusion-tested objects. */
#define PRRE_RH_OQ_DRAW_IF_QUERY_PENDING_BIT 6      /**< Draw occlusion-tested object with full detail when its query is still pending. */
#define PRRE_RH_ORDERING_BY_DISTANCE_BIT     7      /**< Order the rendering of objects based on their distance to camera. */

/* 3 bits for selecting 1 rendering path, starting from PRRE_RH_RENDER_PATH_BITS */
#define PRRE_RH_RP_LEGACY_PR00FPS   0u  /**< The legacy PR00FPS-style rendering path. */
#define PRRE_RH_RP_OCCLUSION_CULLED 1u  /**< Rendering with occlusion culling, using different Object3D containers. */
#define PRRE_RH_RP_RSRVD_1          2u
#define PRRE_RH_RP_RSRVD_2          3u
#define PRRE_RH_RP_RSRVD_3          4u
#define PRRE_RH_RP_RSRVD_4          5u
#define PRRE_RH_RP_RSRVD_5          6u
#define PRRE_RH_RP_RSRVD_6          7u

/* 2 bits for selecting 1 occlusion query method, starting from PRRE_RH_OQ_METHOD_BITS */
#define PRRE_RH_OQ_METHOD_SYNC    0u  /**< Sync: all occlusion queries must be finished within the same frame they are issued. */
#define PRRE_RH_OQ_METHOD_ASYNC   1u  /**< Async: occlusion queries can finish in consecutive frames. */
#define PRRE_RH_OQ_METHOD_RSRVD_1 2u
#define PRRE_RH_OQ_METHOD_RSRVD_2 3u

/* PRRE_RH_OQ_DRAW_BOUNDING_BOXES_BIT */
#define PRRE_RH_OQ_DRAW_BOUNDING_BOXES_OFF  0u                                       /**< Not drawing bounding boxes for occlusion-tested objects. */
#define PRRE_RH_OQ_DRAW_BOUNDING_BOXES_ON   BIT(PRRE_RH_OQ_DRAW_BOUNDING_BOXES_BIT)  /**< Draw bounding boxes for occlusion-tested objects. */

/* PRRE_RH_OQ_DRAW_EVEN_IF_QUERY_PENDING_BIT */
#define PRRE_RH_OQ_DRAW_IF_QUERY_PENDING_OFF  0u                                         /**< Not drawing occlusion-tested objects with full detail if their query is still pending. */
#define PRRE_RH_OQ_DRAW_IF_QUERY_PENDING_ON   BIT(PRRE_RH_OQ_DRAW_IF_QUERY_PENDING_BIT)  /**< Draw occlusion-tested objects with full detail if their query is still pending. */

/* PRRE_RH_ORDERING_BY_DISTANCE_BIT */
#define PRRE_RH_ORDERING_BY_DISTANCE_OFF  0u                                     /**< Not ordering the renderin of objects based on their distance to camera. */
#define PRRE_RH_ORDERING_BY_DISTANCE_ON   BIT(PRRE_RH_ORDERING_BY_DISTANCE_BIT)  /**< Order the rendering of objects based on their distance to camera. */

/**
    Incremental fixed-function hardware renderer path.
    Only deviations from the original PRREIRenderer documentation are mentioned here.
    This renderer has multiple rendering paths, controlled by the render hints.
    The default render hints for this renderer is DefaultHintsForHWfixedPipeRenderer.
*/
class PRRERendererHWfixedPipe :
    public PRREIRenderer
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRRERendererHWfixedPipe is included")
#endif

public:
    /** Default render hints for PRRERendererHWfixedPipe. */
    static const TPRRE_RENDER_HINT DefaultHints =
            BITF_PREP(PRRE_RH_RP_OCCLUSION_CULLED, PRRE_RH_RENDER_PATH_BITS, 3) |
            BITF_PREP(PRRE_RH_OQ_METHOD_ASYNC, PRRE_RH_OQ_METHOD_BITS, 2) |
            PRRE_RH_OQ_DRAW_BOUNDING_BOXES_OFF |
            PRRE_RH_OQ_DRAW_IF_QUERY_PENDING_ON |
            PRRE_RH_ORDERING_BY_DISTANCE_ON;

    static PRRERendererHWfixedPipe& createAndGet(
        PRREWindow& _wnd,
        PRREScreen& _scr,
        PRREhwInfo& _hwinfo );    /**< Creates and gets the singleton implementation instance. */

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    CConsole&  getConsole() const;                     /**< Returns access to console preset with logger module name as this class. */

    /**
        Not implemented.
        Use RenderScene() to render all objects.
    */
    virtual void RenderObject(PRREObject3D& object) = 0;


}; // PRRERendererHWfixedPipe

