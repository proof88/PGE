#pragma once

/*
    ###################################################################################
    PRREVertexTransferImpl.h
    This file is part of PRRE.
    Internal header.
    PRREVertexTransferImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PRREallHeaders.h"
#include "../../external/Object3D/PRREVertexTransferMode.h"
#include <vector>
#include "../gl/gl.h"
#include "../gl/glext.h"

class PRREVertexTransfer::PRREVertexTransferImpl
{

public:

    static TPRRE_PRIMITIVE_FORMAT
        getPRREprimitiveFromGLprimitive(GLenum glprim);
    static GLenum
        getGLprimitiveFromPRREprimitive(TPRRE_PRIMITIVE_FORMAT pf);

    // ---------------------------------------------------------------------------

    virtual ~PRREVertexTransferImpl();

    TPRRE_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;      
    void SetVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod);   
    TPRRE_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;    
    void SetVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref); 
    TPRRE_VERTEX_TRANSFER_MODE getVertexTransferMode() const;          
    void SetVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans);    

    TPRREuint getUsedSystemMemory() const; 

protected:

    // ---------------------------------------------------------------------------
    
    PRREVertexTransferImpl(
        PRREVertexTransfer* pOwner,
        PRREMaterialManager& matMgr,
        const TPRRE_VERTEX_MODIFYING_HABIT& vmod = PRRE_VMOD_STATIC,
        const TPRRE_VERTEX_REFERENCING_MODE& vref = PRRE_VREF_DIRECT,
        TPRREbool bForceUseClientMemory = false);
    
    PRREVertexTransferImpl(const PRREVertexTransferImpl&);
    PRREVertexTransferImpl& operator=(const PRREVertexTransferImpl&);

    void TransferVertices();

private:

    PRREMaterialManager& materialMgr;                    /**< Used to know number of texture layers/units. */
    PRREVertexTransfer* _pOwner;
    TPRRE_VERTEX_TRANSFER_MODE vertexTransferMode;       /**< Vertices storage. */

    TPRREbool bParentInitiatedOperation;   /**< Parent VertexTransfer sets this to true at the beginning of SetVertexTransferMode(), submeshes ignore if this is not true in parent. */

    GLuint nDispList;               /**< OpenGL display list index. */
    GLuint nVerticesVBO;            /**< OpenGL VBO index of vertices. */
    GLuint nColorsVBO;              /**< OpenGL VBO index of colors. */
    std::vector<GLuint> nTexcoordsVBO;   /**< OpenGL VBO indices of texture mapping coordinates per layer. */
    GLuint nNormalsVBO;             /**< OpenGL VBO index of normals. */
    GLuint nIndicesVBO;             /**< OpenGL VBO index of indices. */

    // ---------------------------------------------------------------------------

    CConsole& getConsole() const;               /**< Hack to be able to use CConsole singleton instance instead of owner's protected console instance. */
    TPRREbool isSwitchFromIndexedAllowed() const;                            /**< Tells whether it is allowed to switch from indexed to non-indexed vertex transfer mode. */
    void      ProcessGeometry(TPRREbool indexed) const;                      /**< Goes thru vertices and feeds them to OpenGL. */
    TPRREbool compileIntoDisplayList(TPRREbool indexed);                     /**< Compiles OpenGL drawing commands into display list. */
    TPRREbool compileIntoVertexBufferObjects(
        TPRREbool indexed, TPRREbool dynamic);                               /**< Compiles geometry into VBOs. */
    void      FreeGLresources();                                             /**< Frees up allocated resources from OpenGL. */
    void      SetArrayPointers(TPRREbool redirectToServer = false) const;    /**< Sets vertex data pointers prior to drawing. */
    void      ResetArrayPointers(TPRREbool redirectToServer = false) const;  /**< Resets pointers after drawing. */

    friend class PRREVertexTransfer;
}; // PRREVertexTransfer::PRREVertexTransferImpl 