#pragma once

/*
    ###################################################################################
    PureVertexTransferImpl.h
    This file is part of PURE.
    Internal header.
    PureVertexTransferImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
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

    static TPure_PRIMITIVE_FORMAT
        getPureprimitiveFromGLprimitive(GLenum glprim);
    static GLenum
        getGLprimitiveFromPureprimitive(TPure_PRIMITIVE_FORMAT pf);

    // ---------------------------------------------------------------------------

    virtual ~PureVertexTransferImpl();

    TPure_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;      
    TPure_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;    
    TPure_VERTEX_TRANSFER_MODE getVertexTransferMode() const;          
    TPurebool setVertexTransferMode(TPure_VERTEX_TRANSFER_MODE vtrans);

    TPureuint getLastTransferredVertexCount() const;
    TPureuint getLastTransferredTriangleCount() const;

    TPureuint getUsedSystemMemory() const; 

protected:

    // ---------------------------------------------------------------------------
    
    PureVertexTransferImpl(
        PureVertexTransfer* pOwner,
        PureMaterialManager& matMgr,
        const TPure_VERTEX_MODIFYING_HABIT& vmod = Pure_VMOD_STATIC,
        const TPure_VERTEX_REFERENCING_MODE& vref = Pure_VREF_DIRECT,
        TPurebool bForceUseClientMemory = false);
    
    PureVertexTransferImpl(const PureVertexTransferImpl&);
    PureVertexTransferImpl& operator=(const PureVertexTransferImpl&);

    TPureuint transferVertices();
    void ResetLastTransferredCounts();

private:

    PureMaterialManager& materialMgr;                    /**< Used to know number of texture layers/units. */
    PureVertexTransfer* _pOwner;
    TPure_VERTEX_TRANSFER_MODE vertexTransferMode;       /**< Vertices storage. */

    TPurebool bParentInitiatedOperation;   /**< Parent VertexTransfer sets this to true at the beginning of setVertexTransferMode(), submeshes ignore if this is not true in parent. */

    GLuint nDispList;               /**< OpenGL display list index. */
    GLuint nVerticesVBO;            /**< OpenGL VBO index of vertices. */
    GLuint nColorsVBO;              /**< OpenGL VBO index of colors. */
    std::vector<GLuint> nTexcoordsVBO;   /**< OpenGL VBO indices of texture mapping coordinates per layer. */
    GLuint nNormalsVBO;             /**< OpenGL VBO index of normals. */
    GLuint nIndicesVBO;             /**< OpenGL VBO index of indices. */

    TPureuint nLastTransferredVertices;   /**< Number of vertices transferred by last transferVertices(). */
    TPureuint nLastTransferredTriangles;  /**< Number of vertices transferred by last transferVertices(). */

    // ---------------------------------------------------------------------------

    TPurebool isSwitchFromIndexedAllowed() const;                            /**< Tells whether it is allowed to switch from indexed to non-indexed vertex transfer mode. */
    void      ProcessGeometry(TPurebool indexed) const;                      /**< Goes thru vertices and feeds them to OpenGL. */
    TPurebool compileIntoDisplayList(TPurebool indexed);                     /**< Compiles OpenGL drawing commands into display list. */
    TPurebool compileIntoVertexBufferObjects(
        TPurebool indexed, TPurebool dynamic);                               /**< Compiles geometry into VBOs. */
    void      FreeGLresources();                                             /**< Frees up allocated resources from OpenGL. */
    void      SetArrayPointers(TPurebool redirectToServer = false) const;    /**< Sets vertex data pointers prior to drawing. */
    void      ResetArrayPointers(TPurebool redirectToServer = false) const;  /**< Resets pointers after drawing. */

    friend class PureVertexTransfer;
    friend class PureObject3D;
}; // PureVertexTransfer::PureVertexTransferImpl 