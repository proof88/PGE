#pragma once

/*
    ###################################################################################
    PureVertexTransfer.h
    This file is part of PURE.
    External header.
    PureVertexTransfer class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../PureAllHeaders.h"
#include "../../../../../../PFL/PFL/PFL.h"
#include "PureMesh3DManager.h"
#include "../Material/PureMaterialManager.h"

/**
    Vertex Transfer Mode (TPure_VERTEX_TRANSFER_MODE) specifies how exactly the graphics pipeline is fed during geometry rendering.
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
typedef TPureuint TPure_VERTEX_TRANSFER_MODE;

/**
    Vertex modifying habit (vmod) bit specifies how often the geometry data (vertices, normals, etc.) will be
    modified during the life of the application. This also specifies the primary location from where
    the graphics pipeline will be fed: if supported, all geometry data or some of it will be placed
    in server memory which is usually faster than client memory.
    This is a mandatory input setting of PPP.
*/
typedef TPureuint TPure_VERTEX_MODIFYING_HABIT;

#define Pure_VMOD_STATIC  0u                     /**< Vertices will be specified once and rarely or never modified. Faster. */
#define Pure_VMOD_DYNAMIC BIT(Pure_VT_VMOD_BIT)  /**< Vertices will be specified once and often or always modified. Slower. */


/**
    Vertex referencing mode (vref) specifies how the geometry data (vertices, normals, etc.) will be
    referenced while feeding them into the graphics pipeline.
    This is a mandatory input setting of PPP.
*/
typedef TPureuint TPure_VERTEX_REFERENCING_MODE;

#define Pure_VREF_DIRECT  0u                     /**< Vertex data will be fed into the pipeline by directly passing them.
                                                      Doesn't need an element (index) array to be specified but this disables
                                                      the use of shared vertices between primitives so this may be slower than indexed. */

#define Pure_VREF_INDEXED BIT(Pure_VT_VREF_BIT)  /**< Vertex data will be fed into the pipeline by indexing them.
                                                      Can be faster than direct feeding mode if the geometry is built up using
                                                      shared vertices, especially when shared vertices are referenced close to
                                                      each other (ordered). */

      
/* what purpose each bit stands for in TPure_VERTEX_TRANSFER_MODE */

#define Pure_VT_VMOD_BIT    0      /**< vertex modifying habit (vmod) bit */
#define Pure_VT_VREF_BIT    1      /**< vertex referencing mode (vref) bit */
#define Pure_VT_VA_BIT      2      /**< passing vertices one-by-one (0) or in one shot as array (1) */
#define Pure_VT_SVA_BIT     3      /**< regular vertex array (0) or server-side (1) */
#define Pure_VT_CVA_BIT     4      /**< compiled array (0 no, 1 yes) */
#define Pure_VT_RNG_BIT     5      /**< element array is ranged (0 no, 1 yes) */
#define Pure_VT_VENDOR_BITS 6      /**< using generic solution (0) or vendor-specific (>0) */

/* 3 bits for vendor-specific starting from Pure_VT_VENDOR_BITS */
#define Pure_VT_VENDOR_GENERIC  0u
#define Pure_VT_VENDOR_NVIDIA_1 1u
#define Pure_VT_VENDOR_NVIDIA_2 2u
#define Pure_VT_VENDOR_ATI_1    3u
#define Pure_VT_VENDOR_ATI_2    4u
#define Pure_VT_VENDOR_RSRVD_1  5u
#define Pure_VT_VENDOR_RSRVD_2  6u
#define Pure_VT_VENDOR_RSRVD_3  7u


/* predefined vertex transfer modes for convenience */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_DIR_1_BY_1             = Pure_VMOD_DYNAMIC | Pure_VREF_DIRECT; /**< SUPP! Basic direct immediate mode. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_DIR_RVA                = Pure_VMOD_DYNAMIC | Pure_VREF_DIRECT | BIT(Pure_VT_VA_BIT); /**< SUPP! Regular vertex array, drawArrays. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_DIR_RVA_CVA            = Pure_VMOD_DYNAMIC | Pure_VREF_DIRECT | BIT(Pure_VT_VA_BIT) | BIT(Pure_VT_CVA_BIT); /**< SUPP! Regular vertex array, drawArrays, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_DIR_SVA_GEN            = Pure_VMOD_DYNAMIC | Pure_VREF_DIRECT | BIT(Pure_VT_VA_BIT) | BIT(Pure_VT_SVA_BIT); /**< SUPP! VBO, dynamic, array buffer. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_DIR_SVA_ATI            = Pure_VT_DYN_DIR_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_ATI_1,Pure_VT_VENDOR_BITS,3); /**< ATI VAO, dynamic. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_DIR_SVA_ATI_CVA        = Pure_VT_DYN_DIR_SVA_ATI | BIT(Pure_VT_CVA_BIT); /**< ATI VAO, dynamic, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_DIR_SVA_NV             = Pure_VT_DYN_DIR_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_NVIDIA_1,Pure_VT_VENDOR_BITS,3); /**< NV VAR,  dynamic. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_DIR_SVA_NV_CVA         = Pure_VT_DYN_DIR_SVA_NV | BIT(Pure_VT_CVA_BIT); /**< NV VAR,  dynamic, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_1_BY_1             = Pure_VMOD_DYNAMIC | Pure_VREF_INDEXED; /**< SUPP! Basic indexed immediate mode. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_RVA                = Pure_VMOD_DYNAMIC | Pure_VREF_INDEXED | BIT(Pure_VT_VA_BIT); /**< SUPP! Regular vertex array, drawElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_RVA_RNG            = Pure_VMOD_DYNAMIC | Pure_VREF_INDEXED | BIT(Pure_VT_VA_BIT) | BIT(Pure_VT_RNG_BIT); /**< SUPP! Regular vertex array, drawRangeElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_RVA_CVA            = Pure_VMOD_DYNAMIC | Pure_VREF_INDEXED | BIT(Pure_VT_VA_BIT) | BIT(Pure_VT_CVA_BIT); /**< SUPP! Regular vertex array, drawElements, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_RVA_CVA_RNG        = Pure_VMOD_DYNAMIC | Pure_VREF_INDEXED | BIT(Pure_VT_VA_BIT) | BIT(Pure_VT_CVA_BIT) | BIT(Pure_VT_RNG_BIT); /**< SUPP! Regular vertex array, drawRangeElements, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_GEN            = Pure_VMOD_DYNAMIC | Pure_VREF_INDEXED | BIT(Pure_VT_VA_BIT) | BIT(Pure_VT_SVA_BIT); /**< SUPP! VBO, dynamic, element buffer. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_ATI            = Pure_VT_DYN_IND_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_ATI_1,Pure_VT_VENDOR_BITS,3); /**< ATI VAO, dynamic, drawElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_ATI_RNG        = Pure_VT_DYN_IND_SVA_ATI | BIT(Pure_VT_RNG_BIT); /**< ATI VAO, dynamic, drawRangeElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_ATI_CVA        = Pure_VT_DYN_IND_SVA_ATI | BIT(Pure_VT_CVA_BIT); /**< ATI VAO, dynamic, drawElements, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_ATI_CVA_RNG    = Pure_VT_DYN_IND_SVA_ATI | BIT(Pure_VT_CVA_BIT) | BIT(Pure_VT_RNG_BIT); /**< ATI VAO, dynamic, drawRangeElements, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_ATI_EA         = Pure_VT_DYN_IND_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_ATI_2,Pure_VT_VENDOR_BITS,3); /**< ATI VAO, dynamic, drawElementArrayATI. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_ATI_EA_RNG     = Pure_VT_DYN_IND_SVA_ATI_EA | BIT(Pure_VT_RNG_BIT); /**< ATI VAO, dynamic, drawRangeElementsArrayATI. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_ATI_EA_CVA     = Pure_VT_DYN_IND_SVA_ATI_EA | BIT(Pure_VT_CVA_BIT); /**< ATI VAO, dynamic, drawElementArrayATI, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_ATI_EA_CVA_RNG = Pure_VT_DYN_IND_SVA_ATI_EA | BIT(Pure_VT_CVA_BIT) | BIT(Pure_VT_RNG_BIT); /**< ATI VAO, dynamic, drawRangeElementsArrayATI, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_NV             = Pure_VT_DYN_IND_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_NVIDIA_1,Pure_VT_VENDOR_BITS,3); /**< NV VAR, dynamic, drawElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_NV_RNG         = Pure_VT_DYN_IND_SVA_NV | BIT(Pure_VT_RNG_BIT); /**< NV VAR, dynamic, drawRangeElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_NV_CVA         = Pure_VT_DYN_IND_SVA_NV | BIT(Pure_VT_CVA_BIT); /**< NV VAR, dynamic, drawElements, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_NV_CVA_RNG     = Pure_VT_DYN_IND_SVA_NV | BIT(Pure_VT_CVA_BIT) | BIT(Pure_VT_RNG_BIT); /**< NV VAR, dynamic, drawRangeElements, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_NV_EA          = Pure_VT_DYN_IND_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_NVIDIA_2,Pure_VT_VENDOR_BITS,3); /**< NV VAR, dynamic, drawElementsNV. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_NV_EA_RNG      = Pure_VT_DYN_IND_SVA_NV_EA | BIT(Pure_VT_RNG_BIT); /**< NV VAR, dynamic, drawRangeElementsNV. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_NV_EA_CVA      = Pure_VT_DYN_IND_SVA_NV_EA | BIT(Pure_VT_CVA_BIT); /**< NV VAR, dynamic, drawElementsNV, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_DYN_IND_SVA_NV_EA_CVA_RNG  = Pure_VT_DYN_IND_SVA_NV_EA | BIT(Pure_VT_CVA_BIT) | BIT(Pure_VT_RNG_BIT); /**< NV VAR, dynamic, drawRangeElementsNV, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_DIR_DL                 = Pure_VMOD_STATIC  | Pure_VREF_DIRECT; /**< SUPP! Display list built using direct data. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_DIR_SVA_GEN            = Pure_VMOD_STATIC  | Pure_VREF_DIRECT | BIT(Pure_VT_VA_BIT) | BIT(Pure_VT_SVA_BIT); /**< SUPP! VBO, static, array buffer. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_DIR_SVA_ATI            = Pure_VT_STA_DIR_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_ATI_1,Pure_VT_VENDOR_BITS,3); /**< ATI VAO, static. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_DIR_SVA_ATI_CVA        = Pure_VT_STA_DIR_SVA_GEN | BIT(Pure_VT_CVA_BIT) | BITF_PREP(Pure_VT_VENDOR_ATI_1,Pure_VT_VENDOR_BITS,3); /**< ATI VAO, static, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_DIR_SVA_NV             = Pure_VT_STA_DIR_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_NVIDIA_1,Pure_VT_VENDOR_BITS,3); /**< NV VAR,  static. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_DIR_SVA_NV_CVA         = Pure_VT_STA_DIR_SVA_GEN | BIT(Pure_VT_CVA_BIT) | BITF_PREP(Pure_VT_VENDOR_NVIDIA_1,Pure_VT_VENDOR_BITS,3); /**< NV VAR,  static, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_DL                 = Pure_VMOD_STATIC  | Pure_VREF_INDEXED; /**< SUPP! Display list built using indexed data. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_GEN            = Pure_VMOD_STATIC  | Pure_VREF_INDEXED | BIT(Pure_VT_VA_BIT) | BIT(Pure_VT_SVA_BIT); /**< SUPP! VBO, static, element buffer. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_ATI            = Pure_VT_STA_IND_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_ATI_1,Pure_VT_VENDOR_BITS,3); /**< ATI VAO, static, drawElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_ATI_RNG        = Pure_VT_STA_IND_SVA_ATI | BIT(Pure_VT_RNG_BIT); /**< ATI VAO, static, drawRangeElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_ATI_CVA        = Pure_VT_STA_IND_SVA_ATI | BIT(Pure_VT_CVA_BIT); /**< ATI VAO, static, drawElements, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_ATI_CVA_RNG    = Pure_VT_STA_IND_SVA_ATI | BIT(Pure_VT_RNG_BIT) | BIT(Pure_VT_CVA_BIT); /**< ATI VAO, static, drawRangeElements, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_ATI_EA         = Pure_VT_STA_IND_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_ATI_2,Pure_VT_VENDOR_BITS,3); /**< ATI VAO, static, drawElementsATI. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_ATI_EA_RNG     = Pure_VT_STA_IND_SVA_ATI_EA | BIT(Pure_VT_RNG_BIT); /**< ATI VAO, static, drawRangeElementsATI. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_ATI_EA_CVA     = Pure_VT_STA_IND_SVA_ATI_EA | BIT(Pure_VT_CVA_BIT); /**< ATI VAO, static, drawElementsATI, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_ATI_EA_CVA_RNG = Pure_VT_STA_IND_SVA_ATI_EA | BIT(Pure_VT_RNG_BIT) | BIT(Pure_VT_CVA_BIT); /**< ATI VAO, static, drawRangeElementsATI, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_NV             = Pure_VT_STA_IND_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_NVIDIA_1,Pure_VT_VENDOR_BITS,3); /**< NV VAR,  static, drawElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_NV_RNG         = Pure_VT_STA_IND_SVA_NV | BIT(Pure_VT_RNG_BIT); /**< NV VAR,  static, drawRangeElements. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_NV_CVA         = Pure_VT_STA_IND_SVA_NV | BIT(Pure_VT_CVA_BIT); /**< NV VAR,  static, drawElements, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_NV_CVA_RNG     = Pure_VT_STA_IND_SVA_NV | BIT(Pure_VT_RNG_BIT) | BIT(Pure_VT_CVA_BIT); /**< NV VAR,  static, drawRangeElements, compiled. */

const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_NV_EA          = Pure_VT_STA_IND_SVA_GEN | BITF_PREP(Pure_VT_VENDOR_NVIDIA_2,Pure_VT_VENDOR_BITS,3); /**< NV VAR,  static, drawElementsNV. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_NV_EA_RNG      = Pure_VT_STA_IND_SVA_NV_EA | BIT(Pure_VT_RNG_BIT); /**< NV VAR,  static, drawRangeElementsNV. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_NV_EA_CVA      = Pure_VT_STA_IND_SVA_NV_EA | BIT(Pure_VT_CVA_BIT); /**< NV VAR,  static, drawElementsNV, compiled. */
const TPure_VERTEX_TRANSFER_MODE Pure_VT_STA_IND_SVA_NV_EA_CVA_RNG  = Pure_VT_STA_IND_SVA_NV_EA | BIT(Pure_VT_CVA_BIT) | BIT(Pure_VT_RNG_BIT); /**< NV VAR,  static, drawRangeElementsNV, compiled. */


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
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureVertexTransfer is included")
#endif

public:
    /** Tells whether the given Vertex Transfer Mode is available on the current hardware. */
    static TPurebool isVertexTransferModeSelectable(TPure_VERTEX_TRANSFER_MODE vtrans);

    /** Tells whether the given Vertex Transfer Mode references vertices by indexing. */
    static TPurebool isVertexReferencingIndexed(TPure_VERTEX_TRANSFER_MODE vtrans);

    /** Tells whether the given Vertex Transfer Mode means dynamic modifying habit. */
    static TPurebool isVertexModifyingDynamic(TPure_VERTEX_TRANSFER_MODE vtrans);

    /** Tells whether the given Vertex Transfer Mode uses VRAM. */
    static TPurebool isVideoMemoryUsed(TPure_VERTEX_TRANSFER_MODE vtrans);

    static TPure_VERTEX_TRANSFER_MODE selectVertexTransferMode(
        TPure_VERTEX_MODIFYING_HABIT vmod,
        TPure_VERTEX_REFERENCING_MODE vref,
        TPurebool bForceUseClientMemory );                    /**< Selects a suitable vertex transfer mode. */

    static const char* getLoggerModuleName();                 /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureVertexTransfer();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

    virtual TPure_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;           /**< Gets vertex modifying habit. */
    virtual TPurebool setVertexModifyingHabit(TPure_VERTEX_MODIFYING_HABIT vmod);   /**< Sets vertex modifying habit. */
    virtual TPure_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;         /**< Gets vertex referencing mode. */
    virtual TPurebool setVertexReferencingMode(TPure_VERTEX_REFERENCING_MODE vref); /**< Sets vertex referencing mode. */
    virtual TPure_VERTEX_TRANSFER_MODE getVertexTransferMode() const;               /**< Gets vertex transfer mode. */
    virtual TPurebool setVertexTransferMode(TPure_VERTEX_TRANSFER_MODE vtrans);     /**< Sets vertex transfer mode. */

    virtual TPureuint getLastTransferredVertexCount() const;     /**< Gets the number of vertices sent to graphics pipeline by the last transferVertices() call. */
    virtual TPureuint getLastTransferredTriangleCount() const;   /**< Gets the number of triangles sent to graphics pipeline by the last transferVertices() call. */

    virtual TPureuint getUsedSystemMemory() const;    /**< Gets the amount of allocated system memory. */
    virtual TPureuint getUsedVideoMemory() const;     /**< Gets the amount of allocated video memory. */

protected:

    // ---------------------------------------------------------------------------
    
    PureVertexTransfer(
        PureMaterialManager& matMgr,
        const TPure_VERTEX_MODIFYING_HABIT& vmod = Pure_VMOD_STATIC,
        const TPure_VERTEX_REFERENCING_MODE& vref = Pure_VREF_DIRECT,
        TPurebool bForceUseClientMemory = false); /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PureVertexTransfer();
    
    PureVertexTransfer(const PureVertexTransfer&);                                   
    PureVertexTransfer& operator=(const PureVertexTransfer&);

    TPureuint transferVertices();                     /**< Sends vertices to the graphics pipeline. */
    virtual void ResetLastTransferredCounts();        /**< Reset counters used for measuring number of vertices, triangles, etc. sent to the graphics pipeline by the last transferVertices(). */

private:
    class PureVertexTransferImpl;
    PureVertexTransferImpl* pImpl;

    friend class SampleManagerForDescendantFromVertexTransfer;

}; // class PureVertexTransfer