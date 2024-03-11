#pragma once

/*
    ###################################################################################
    PureVertexTransferImpl.h
    This file is part of PURE.
    Internal header.
    PureVertexTransferImpl class.
    Made by PR00F88
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../external/Object3D/PureVertexTransferMode.h"
#include <vector>
#include "../gl/gl.h"
#include "../gl/glext.h"

class PureObject3D;

class PureVertexTransfer::PureVertexTransferImpl
{

public:

    static TPURE_PRIMITIVE_FORMAT
        getPureprimitiveFromGLprimitive(GLenum glprim);
    static GLenum
        getGLprimitiveFromPureprimitive(TPURE_PRIMITIVE_FORMAT pf);

    // ---------------------------------------------------------------------------

    virtual ~PureVertexTransferImpl();

    TPURE_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;      
    TPURE_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;    
    TPURE_VERTEX_TRANSFER_MODE getVertexTransferMode() const;          
    TPureBool setVertexTransferMode(TPURE_VERTEX_TRANSFER_MODE vtrans);

    TPureUInt getLastTransferredVertexCount() const;
    TPureUInt getLastTransferredTriangleCount() const;

    TPureUInt getUsedSystemMemory() const; 

protected:

    // ---------------------------------------------------------------------------
    
    PureVertexTransferImpl(
        PureVertexTransfer* pOwner,
        PureMaterialManager& matMgr,
        const TPURE_VERTEX_MODIFYING_HABIT& vmod = PURE_VMOD_STATIC,
        const TPURE_VERTEX_REFERENCING_MODE& vref = PURE_VREF_DIRECT,
        TPureBool bForceUseClientMemory = false);
    
    PureVertexTransferImpl(const PureVertexTransferImpl&);
    PureVertexTransferImpl& operator=(const PureVertexTransferImpl&);

    TPureUInt transferVertices();
    void ResetLastTransferredCounts();

private:

    PureMaterialManager& materialMgr;                    /**< Used to know number of texture layers/units. */
    PureVertexTransfer* _pOwner;
    TPURE_VERTEX_TRANSFER_MODE vertexTransferMode;       /**< Vertices storage. */

    TPureBool bParentInitiatedOperation;   /**< Parent VertexTransfer sets this to true at the beginning of setVertexTransferMode(), submeshes ignore if this is not true in parent. */

    GLuint nDispList;               /**< OpenGL display list index. */
    GLuint nVerticesVBO;            /**< OpenGL VBO index of vertices. */
    GLuint nColorsVBO;              /**< OpenGL VBO index of colors. */
    std::vector<GLuint> nTexcoordsVBO;   /**< OpenGL VBO indices of texture mapping coordinates per layer. */
    GLuint nNormalsVBO;             /**< OpenGL VBO index of normals. */
    GLuint nIndicesVBO;             /**< OpenGL VBO index of indices. */

    TPureUInt nLastTransferredVertices;   /**< Number of vertices transferred by last transferVertices(). */
    TPureUInt nLastTransferredTriangles;  /**< Number of vertices transferred by last transferVertices(). */

    // ---------------------------------------------------------------------------

    TPureBool isSwitchFromIndexedAllowed() const;                            /**< Tells whether it is allowed to switch from indexed to non-indexed vertex transfer mode. */
    void      ProcessGeometry(TPureBool indexed) const;                      /**< Goes thru vertices and feeds them to OpenGL. */
    TPureBool compileIntoDisplayList(TPureBool indexed);                     /**< Compiles OpenGL drawing commands into display list. */
    TPureBool compileIntoVertexBufferObjects(
        TPureBool indexed, TPureBool dynamic);                               /**< Compiles geometry into VBOs. */
    void      FreeGLresources();                                             /**< Frees up allocated resources from OpenGL. */
    void      SetArrayPointers(TPureBool redirectToServer = false) const;    /**< Sets vertex data pointers prior to drawing. */
    void      ResetArrayPointers(TPureBool redirectToServer = false) const;  /**< Resets pointers after drawing. */

    friend class PureVertexTransfer;
    friend class PureObject3D;
}; // PureVertexTransfer::PureVertexTransferImpl 