#pragma once

/*
    ###################################################################################
    PureVertexTransfer.h
    This file is part of PURE.
    External header.
    PureVertexTransfer class.
    Made by PR00F88
    ###################################################################################
*/


#include "../PureAllHeaders.h"
#include "../../../../../../PFL/PFL/PFL.h"
#include "PureMesh3DManager.h"
#include "../Material/PureMaterialManager.h"

/**
    Vertex Transfer Mode (TPURE_VERTEX_TRANSFER_MODE) specifies how exactly the graphics pipeline is fed during geometry rendering.
    It is a simple number where each bit has different meaning.

    Automatically selected by PPP based on capabilities of the current hardware and the 2 PPP-settings:
    - vertex modifying habit (vmod),
    - vertex referencing mode (vref).

    Manual selection of the transfer mode is also available for custom purposes like testing, but keep in mind
    that manual selection may not take effect if the chosen mode is invalid for the current hardware and geometry data.

    It's not recommended to change transfer mode often during rendering, because it is a slow procedure:
    structure of geometry data may need to be reorganized and/or geometry data needs to be uploaded to host (video) memory.

    This setting is per-object, since different objects may need different transfer modes. For example, static geometry that is
    modified never or very rarely (e.g. buildings) should be placed in host/video memory, while other geometry changed often
    may be better to be in client/system memory, especially if the CPU is manipulating it instead of the GPU.
*/
typedef TPureUInt TPURE_VERTEX_TRANSFER_MODE;

/**
    Vertex modifying habit (vmod) bit specifies how often the geometry data (vertices, normals, etc.) will be
    modified during the life of the application. This also specifies the primary location from where
    the graphics pipeline will be fed: if supported, all geometry data or some of it will be placed
    in server memory which is usually faster than client memory.
    This is a mandatory input setting of PPP.
*/
typedef TPureUInt TPURE_VERTEX_MODIFYING_HABIT;

#define PURE_VMOD_STATIC  0u                     /**< Vertices will be specified once and rarely or never modified. Faster. */
#define PURE_VMOD_DYNAMIC BIT(PURE_VT_VMOD_BIT)  /**< Vertices will be specified once and often or always modified. Slower. */


/**
    Vertex referencing mode (vref) specifies how the geometry data (vertices, normals, etc.) will be
    referenced while feeding them into the graphics pipeline.
    This is a mandatory input setting of PPP.
*/
typedef TPureUInt TPURE_VERTEX_REFERENCING_MODE;

#define PURE_VREF_DIRECT  0u                     /**< Vertex data will be fed into the pipeline by directly passing them.
                                                      Doesn't need an element (index) array to be specified but this disables
                                                      the use of shared vertices between primitives so this may be slower than indexed. */

#define PURE_VREF_INDEXED BIT(PURE_VT_VREF_BIT)  /**< Vertex data will be fed into the pipeline by indexing them.
                                                      Can be faster than direct feeding mode if the geometry is built up using
                                                      shared vertices, especially when shared vertices are referenced close to
                                                      each other (ordered). */

      
/* what purpose each bit stands for in TPURE_VERTEX_TRANSFER_MODE */

#define PURE_VT_VMOD_BIT    0      /**< vertex modifying habit (vmod) bit */
#define PURE_VT_VREF_BIT    1      /**< vertex referencing mode (vref) bit */
#define PURE_VT_VA_BIT      2      /**< passing vertices one-by-one (0) or in one shot as array (1) */
#define PURE_VT_SVA_BIT     3      /**< regular vertex array (0) or server-side (1) */
#define PURE_VT_CVA_BIT     4      /**< compiled array (0 no, 1 yes) */
#define PURE_VT_RNG_BIT     5      /**< element array is ranged (0 no, 1 yes) */
#define PURE_VT_VENDOR_BITS 6      /**< using generic solution (0) or vendor-specific (>0) */

/* 3 bits for vendor-specific starting from PURE_VT_VENDOR_BITS */
#define PURE_VT_VENDOR_GENERIC  0u
#define PURE_VT_VENDOR_NVIDIA_1 1u
#define PURE_VT_VENDOR_NVIDIA_2 2u
#define PURE_VT_VENDOR_ATI_1    3u
#define PURE_VT_VENDOR_ATI_2    4u
#define PURE_VT_VENDOR_RSRVD_1  5u
#define PURE_VT_VENDOR_RSRVD_2  6u
#define PURE_VT_VENDOR_RSRVD_3  7u


/* predefined vertex transfer modes for convenience */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_DIR_1_BY_1             = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT; /**< SUPP! Basic direct immediate mode. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_DIR_RVA                = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT); /**< SUPP! Regular vertex array, drawArrays. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_DIR_RVA_CVA            = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_CVA_BIT); /**< SUPP! Regular vertex array, drawArrays, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_DIR_SVA_GEN            = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT); /**< SUPP! VBO, dynamic, array buffer. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_DIR_SVA_ATI            = PURE_VT_DYN_DIR_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_ATI_1,PURE_VT_VENDOR_BITS,3); /**< ATI VAO, dynamic. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_DIR_SVA_ATI_CVA        = PURE_VT_DYN_DIR_SVA_ATI | BIT(PURE_VT_CVA_BIT); /**< ATI VAO, dynamic, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_DIR_SVA_NV             = PURE_VT_DYN_DIR_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_NVIDIA_1,PURE_VT_VENDOR_BITS,3); /**< NV VAR,  dynamic. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_DIR_SVA_NV_CVA         = PURE_VT_DYN_DIR_SVA_NV | BIT(PURE_VT_CVA_BIT); /**< NV VAR,  dynamic, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_1_BY_1             = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED; /**< SUPP! Basic indexed immediate mode. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_RVA                = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT); /**< SUPP! Regular vertex array, drawElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_RVA_RNG            = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_RNG_BIT); /**< SUPP! Regular vertex array, drawRangeElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_RVA_CVA            = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_CVA_BIT); /**< SUPP! Regular vertex array, drawElements, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_RVA_CVA_RNG        = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_CVA_BIT) | BIT(PURE_VT_RNG_BIT); /**< SUPP! Regular vertex array, drawRangeElements, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_GEN            = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT); /**< SUPP! VBO, dynamic, element buffer. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_ATI            = PURE_VT_DYN_IND_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_ATI_1,PURE_VT_VENDOR_BITS,3); /**< ATI VAO, dynamic, drawElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_ATI_RNG        = PURE_VT_DYN_IND_SVA_ATI | BIT(PURE_VT_RNG_BIT); /**< ATI VAO, dynamic, drawRangeElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_ATI_CVA        = PURE_VT_DYN_IND_SVA_ATI | BIT(PURE_VT_CVA_BIT); /**< ATI VAO, dynamic, drawElements, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_ATI_CVA_RNG    = PURE_VT_DYN_IND_SVA_ATI | BIT(PURE_VT_CVA_BIT) | BIT(PURE_VT_RNG_BIT); /**< ATI VAO, dynamic, drawRangeElements, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_ATI_EA         = PURE_VT_DYN_IND_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_ATI_2,PURE_VT_VENDOR_BITS,3); /**< ATI VAO, dynamic, drawElementArrayATI. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_ATI_EA_RNG     = PURE_VT_DYN_IND_SVA_ATI_EA | BIT(PURE_VT_RNG_BIT); /**< ATI VAO, dynamic, drawRangeElementsArrayATI. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_ATI_EA_CVA     = PURE_VT_DYN_IND_SVA_ATI_EA | BIT(PURE_VT_CVA_BIT); /**< ATI VAO, dynamic, drawElementArrayATI, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_ATI_EA_CVA_RNG = PURE_VT_DYN_IND_SVA_ATI_EA | BIT(PURE_VT_CVA_BIT) | BIT(PURE_VT_RNG_BIT); /**< ATI VAO, dynamic, drawRangeElementsArrayATI, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_NV             = PURE_VT_DYN_IND_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_NVIDIA_1,PURE_VT_VENDOR_BITS,3); /**< NV VAR, dynamic, drawElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_NV_RNG         = PURE_VT_DYN_IND_SVA_NV | BIT(PURE_VT_RNG_BIT); /**< NV VAR, dynamic, drawRangeElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_NV_CVA         = PURE_VT_DYN_IND_SVA_NV | BIT(PURE_VT_CVA_BIT); /**< NV VAR, dynamic, drawElements, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_NV_CVA_RNG     = PURE_VT_DYN_IND_SVA_NV | BIT(PURE_VT_CVA_BIT) | BIT(PURE_VT_RNG_BIT); /**< NV VAR, dynamic, drawRangeElements, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_NV_EA          = PURE_VT_DYN_IND_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_NVIDIA_2,PURE_VT_VENDOR_BITS,3); /**< NV VAR, dynamic, drawElementsNV. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_NV_EA_RNG      = PURE_VT_DYN_IND_SVA_NV_EA | BIT(PURE_VT_RNG_BIT); /**< NV VAR, dynamic, drawRangeElementsNV. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_NV_EA_CVA      = PURE_VT_DYN_IND_SVA_NV_EA | BIT(PURE_VT_CVA_BIT); /**< NV VAR, dynamic, drawElementsNV, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_DYN_IND_SVA_NV_EA_CVA_RNG  = PURE_VT_DYN_IND_SVA_NV_EA | BIT(PURE_VT_CVA_BIT) | BIT(PURE_VT_RNG_BIT); /**< NV VAR, dynamic, drawRangeElementsNV, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_DIR_DL                 = PURE_VMOD_STATIC  | PURE_VREF_DIRECT; /**< SUPP! Display list built using direct data. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_DIR_SVA_GEN            = PURE_VMOD_STATIC  | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT); /**< SUPP! VBO, static, array buffer. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_DIR_SVA_ATI            = PURE_VT_STA_DIR_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_ATI_1,PURE_VT_VENDOR_BITS,3); /**< ATI VAO, static. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_DIR_SVA_ATI_CVA        = PURE_VT_STA_DIR_SVA_GEN | BIT(PURE_VT_CVA_BIT) | BITF_PREP(PURE_VT_VENDOR_ATI_1,PURE_VT_VENDOR_BITS,3); /**< ATI VAO, static, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_DIR_SVA_NV             = PURE_VT_STA_DIR_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_NVIDIA_1,PURE_VT_VENDOR_BITS,3); /**< NV VAR,  static. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_DIR_SVA_NV_CVA         = PURE_VT_STA_DIR_SVA_GEN | BIT(PURE_VT_CVA_BIT) | BITF_PREP(PURE_VT_VENDOR_NVIDIA_1,PURE_VT_VENDOR_BITS,3); /**< NV VAR,  static, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_DL                 = PURE_VMOD_STATIC  | PURE_VREF_INDEXED; /**< SUPP! Display list built using indexed data. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_GEN            = PURE_VMOD_STATIC  | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT); /**< SUPP! VBO, static, element buffer. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_ATI            = PURE_VT_STA_IND_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_ATI_1,PURE_VT_VENDOR_BITS,3); /**< ATI VAO, static, drawElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_ATI_RNG        = PURE_VT_STA_IND_SVA_ATI | BIT(PURE_VT_RNG_BIT); /**< ATI VAO, static, drawRangeElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_ATI_CVA        = PURE_VT_STA_IND_SVA_ATI | BIT(PURE_VT_CVA_BIT); /**< ATI VAO, static, drawElements, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_ATI_CVA_RNG    = PURE_VT_STA_IND_SVA_ATI | BIT(PURE_VT_RNG_BIT) | BIT(PURE_VT_CVA_BIT); /**< ATI VAO, static, drawRangeElements, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_ATI_EA         = PURE_VT_STA_IND_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_ATI_2,PURE_VT_VENDOR_BITS,3); /**< ATI VAO, static, drawElementsATI. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_ATI_EA_RNG     = PURE_VT_STA_IND_SVA_ATI_EA | BIT(PURE_VT_RNG_BIT); /**< ATI VAO, static, drawRangeElementsATI. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_ATI_EA_CVA     = PURE_VT_STA_IND_SVA_ATI_EA | BIT(PURE_VT_CVA_BIT); /**< ATI VAO, static, drawElementsATI, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_ATI_EA_CVA_RNG = PURE_VT_STA_IND_SVA_ATI_EA | BIT(PURE_VT_RNG_BIT) | BIT(PURE_VT_CVA_BIT); /**< ATI VAO, static, drawRangeElementsATI, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_NV             = PURE_VT_STA_IND_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_NVIDIA_1,PURE_VT_VENDOR_BITS,3); /**< NV VAR,  static, drawElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_NV_RNG         = PURE_VT_STA_IND_SVA_NV | BIT(PURE_VT_RNG_BIT); /**< NV VAR,  static, drawRangeElements. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_NV_CVA         = PURE_VT_STA_IND_SVA_NV | BIT(PURE_VT_CVA_BIT); /**< NV VAR,  static, drawElements, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_NV_CVA_RNG     = PURE_VT_STA_IND_SVA_NV | BIT(PURE_VT_RNG_BIT) | BIT(PURE_VT_CVA_BIT); /**< NV VAR,  static, drawRangeElements, compiled. */

const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_NV_EA          = PURE_VT_STA_IND_SVA_GEN | BITF_PREP(PURE_VT_VENDOR_NVIDIA_2,PURE_VT_VENDOR_BITS,3); /**< NV VAR,  static, drawElementsNV. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_NV_EA_RNG      = PURE_VT_STA_IND_SVA_NV_EA | BIT(PURE_VT_RNG_BIT); /**< NV VAR,  static, drawRangeElementsNV. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_NV_EA_CVA      = PURE_VT_STA_IND_SVA_NV_EA | BIT(PURE_VT_CVA_BIT); /**< NV VAR,  static, drawElementsNV, compiled. */
const TPURE_VERTEX_TRANSFER_MODE PURE_VT_STA_IND_SVA_NV_EA_CVA_RNG  = PURE_VT_STA_IND_SVA_NV_EA | BIT(PURE_VT_CVA_BIT) | BIT(PURE_VT_RNG_BIT); /**< NV VAR,  static, drawRangeElementsNV, compiled. */


/**
    VertexTransfer class extends Mesh3D by providing ways to transfer vertices of the mesh over the vertex pipeline.

    Similar to Mesh3D, VertexTransfer objects are 2-level entities:
     - first level (parent) derives from a level-1 VertexTransfer object, and owns properties such as vertex transfer mode;
     - second level objects derive from level-2 VertexTransfer objects, particularly the submeshes of the level-1 mesh from where the level-1 VertexTransfer is also derived.

    The biggest addition of VertexTransfer to Mesh3D is the wide selection of vertex transfer modes and management of the related OpenGL resources such as vertex buffer objects.
*/
class PureVertexTransfer :
    public PureMesh3D 
{
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureVertexTransfer is included")
#endif

public:
    /** Tells whether the given Vertex Transfer Mode is available on the current hardware. */
    static TPureBool isVertexTransferModeSelectable(TPURE_VERTEX_TRANSFER_MODE vtrans);

    /** Tells whether the given Vertex Transfer Mode references vertices by indexing. */
    static TPureBool isVertexReferencingIndexed(TPURE_VERTEX_TRANSFER_MODE vtrans);

    /** Tells whether the given Vertex Transfer Mode means dynamic modifying habit. */
    static TPureBool isVertexModifyingDynamic(TPURE_VERTEX_TRANSFER_MODE vtrans);

    /** Tells whether the given Vertex Transfer Mode uses VRAM. */
    static TPureBool isVideoMemoryUsed(TPURE_VERTEX_TRANSFER_MODE vtrans);

    static TPURE_VERTEX_TRANSFER_MODE selectVertexTransferMode(
        TPURE_VERTEX_MODIFYING_HABIT vmod,
        TPURE_VERTEX_REFERENCING_MODE vref,
        TPureBool bForceUseClientMemory );                    /**< Selects a suitable vertex transfer mode. */

    static const char* getLoggerModuleName();                 /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureVertexTransfer();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

    virtual TPURE_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;           /**< Gets vertex modifying habit. */
    virtual TPureBool setVertexModifyingHabit(TPURE_VERTEX_MODIFYING_HABIT vmod);   /**< Sets vertex modifying habit. */
    virtual TPURE_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;         /**< Gets vertex referencing mode. */
    virtual TPureBool setVertexReferencingMode(TPURE_VERTEX_REFERENCING_MODE vref); /**< Sets vertex referencing mode. */
    virtual TPURE_VERTEX_TRANSFER_MODE getVertexTransferMode() const;               /**< Gets vertex transfer mode. */
    virtual TPureBool setVertexTransferMode(TPURE_VERTEX_TRANSFER_MODE vtrans);     /**< Sets vertex transfer mode. */

    virtual TPureUInt getLastTransferredVertexCount() const;     /**< Gets the number of vertices sent to graphics pipeline by the last transferVertices() call. */
    virtual TPureUInt getLastTransferredTriangleCount() const;   /**< Gets the number of triangles sent to graphics pipeline by the last transferVertices() call. */

    virtual TPureUInt getUsedSystemMemory() const;    /**< Gets the amount of allocated system memory. */
    virtual TPureUInt getUsedVideoMemory() const;     /**< Gets the amount of allocated video memory. */

protected:

    // ---------------------------------------------------------------------------
    
    PureVertexTransfer(
        PureMaterialManager& matMgr,
        const TPURE_VERTEX_MODIFYING_HABIT& vmod = PURE_VMOD_STATIC,
        const TPURE_VERTEX_REFERENCING_MODE& vref = PURE_VREF_DIRECT,
        TPureBool bForceUseClientMemory = false); /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PureVertexTransfer();
    
    PureVertexTransfer(const PureVertexTransfer&);                                   
    PureVertexTransfer& operator=(const PureVertexTransfer&);

    TPureUInt transferVertices();                     /**< Sends vertices to the graphics pipeline. */
    virtual void ResetLastTransferredCounts();        /**< Reset counters used for measuring number of vertices, triangles, etc. sent to the graphics pipeline by the last transferVertices(). */

private:
    class PureVertexTransferImpl;
    PureVertexTransferImpl* pImpl;

    friend class SampleManagerForDescendantFromVertexTransfer;

}; // class PureVertexTransfer