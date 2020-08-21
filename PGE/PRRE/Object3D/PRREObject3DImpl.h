#pragma once

/*
    ###################################################################################
    PRREObject3DImpl.h
    This file is part of PRRE.
    Internal header.
    PRREObject3DImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../PRREallHeaders.h"
#include "PRREObject3DManager.h"
#include "../gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PRREObject3D::PRREObject3DImpl
{

public:

    static TPRRE_BLENDFACTORS
        getPRREblendFromGLblend(GLenum glb);             /**< Gets the appropriate PRRE blend factor for the given GL enum. */
    static GLenum
        getGLblendFromPRREblend(TPRRE_BLENDFACTORS bf);  /**< Gets the appropriate GL enum for the given PRRE blend factor. */

    static TPRRE_PRIMITIVE_FORMAT
        getPRREprimitiveFromGLprimitive(GLenum glprim);
    static GLenum
        getGLprimitiveFromPRREprimitive(TPRRE_PRIMITIVE_FORMAT pf);

    // ---------------------------------------------------------------------------

    virtual ~PRREObject3DImpl();

    PRREObject3D* getReferredObject() const;

    TPRRE_PRIMITIVE_FORMAT getPrimitiveFormat() const;   

    TPRRE_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;      
    void SetVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod);   
    TPRRE_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;    
    void SetVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref); 
    TPRRE_VERTEX_TRANSFER_MODE getVertexTransferMode() const;          
    void SetVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans);     

    TPRREuint   getVerticesCount() const;      
    const TXYZ* getVertices() const;  
          TXYZ* getVertices();
    TPRRE_TRANSFORMED_VERTEX* getTransformedVertices();

    TPRREuint   getVertexIndicesCount() const;
    const void* getVertexIndices() const;

    unsigned int getVertexIndicesType() const;

    TPRREuint   getNormalsCount() const;       
    const TXYZ* getNormals() const;  
    
    PRREVector&       getPosVec();                  
    const PRREVector& getPosVec() const;            
    PRREVector&       getAngleVec();                
    const PRREVector& getAngleVec() const;          
    const PRREVector& getSizeVec() const;           
    PRREVector        getScaledSizeVec() const;     
    void              RecalculateSize();            
    const PRREVector& getScaling() const;           
    void              SetScaling(TPRREfloat value);
    void              SetScaling(const PRREVector& value);
    void              Scale(TPRREfloat value);
    void              Scale(const PRREVector& value);

    TPRREbool isVisible() const;                    
    void      SetVisible(TPRREbool state);          
    void      Show();                               
    void      Hide();                               
    TPRREbool isColliding_TO_BE_REMOVED() const;              
    void      SetColliding_TO_BE_REMOVED(TPRREbool value);    

    TPRRE_ROTATION_ORDER getRotationOrder() const;      
    void SetRotationOrder(TPRRE_ROTATION_ORDER value);  

    TPRREbool isLit() const;                            
    void      SetLit(TPRREbool value);                    
    TPRREbool isDoubleSided() const;                     
    void      SetDoubleSided(TPRREbool value);          
    TPRREbool isWireframed() const;                     
    void      SetWireframed(TPRREbool value);           
    TPRREbool isWireframedCulled() const;               
    void      SetWireframedCulled(TPRREbool value);      
    TPRREbool isAffectingZBuffer() const;              
    void      SetAffectingZBuffer(TPRREbool value);      
    TPRREbool isTestingAgainstZBuffer() const;          
    void      SetTestingAgainstZBuffer(TPRREbool value); 
    TPRREbool isStickedToScreen() const;               
    void      SetStickedToScreen(TPRREbool value);  

    const PRREMaterial& getMaterial() const;
          PRREMaterial& getMaterial();

    TPRREuint getUsedSystemMemory() const; 

    void Draw(bool bLighting);                  

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    PRREObject3DImpl(
        PRREObject3D* owner,
        const TPRRE_VERTEX_MODIFYING_HABIT& vmod = PRRE_VMOD_STATIC,
        const TPRRE_VERTEX_REFERENCING_MODE& vref = PRRE_VREF_DIRECT,
        TPRREbool bForceUseClientMemory = false);
    
    PRREObject3DImpl(const PRREObject3DImpl&);
    PRREObject3DImpl& operator=(const PRREObject3DImpl&);

private:
    PRREObject3D* _pOwner;    /**< The owner public object who creates this pimpl object. */
    PRREObject3D* pRefersto;  /**< Pointer to the original object when we are just a cloned object. */

    PRREVector vPos;                /**< 3D Position. */
    PRREVector vSize;               /**< 3D Size. */
    PRREVector vAngle;              /**< 3D Angle. */
    PRREVector vScaling;            /**< 3D Scaling. */
    TPRREbool bVisible;             /**< Visible state. */
    TPRREbool bAffectedByLights;    /**< State of lit. */
    TPRREbool bDoubleSided;         /**< Double sided. */
    TPRREbool bWireframe;           /**< Wireframe mode. */
    TPRREbool bWireframedCull;      /**< Wireframed culling. */
    TPRREbool bAffectZBuffer;       /**< Writes to Z-Buffer. */
    TPRREbool bAllowZTesting;       /**< Tests to Z-Buffer. */
    TPRREbool bStickedToScreen;     /**< Sticked to screen. */
    TPRRE_ROTATION_ORDER rotation;  /**< Rotation order. */
    GLuint nDispList;               /**< OpenGL display list index. */
    GLuint nVerticesVBO;            /**< OpenGL VBO index of vertices. */
    GLuint nColorsVBO;              /**< OpenGL VBO index of colors. */
    std::vector<GLuint> nTexcoordsVBO;   /**< OpenGL VBO indices of texture mapping coordinates per layer. */
    GLuint nNormalsVBO;             /**< OpenGL VBO index of normals. */
    GLuint nIndicesVBO;             /**< OpenGL VBO index of indices. */
    TPRREbool bReadyToDraw;         /**< Ready to draw. */
    PRREMaterial* pMaterial;        /**< Pointer to material. */

    TPRREbool bColliding;           /**< Colliding state. DEPRECATED: to be removed ... */

    TPRRE_PRIMITIVE_FORMAT primitiveFormat;              /**< Primitives' format. */
    TPRRE_VERTEX_TRANSFER_MODE vertexTransferMode;       /**< Vertices storage. */
    TXYZ*       pVertices;          /**< Pointer to vertices. */
    TPRRE_TRANSFORMED_VERTEX*      pVerticesTransf;    /**< Pointer to transformed vertices. */
    TXYZ*       pNormals;           /**< Pointer to normals. */
    void*       pVertexIndices;     /**< Pointer to vertex indices. We use these to index into arrays of Material too. */
    TPRREuint   nVertexIndices_h;   /**< Number of vertex indices. */
    TPRREuint   nMinIndex;          /**< Smallest value in the pVertexIndices array. Used by glDrawRangeElementsEXT(). */
    TPRREuint   nMaxIndex;          /**< Biggest value in the pVertexIndices array. Used by glDrawRangeElementsEXT(). */
    GLenum      nIndicesType;       /**< Type of indices stored in pVertexIndices array. Should be as small as possible per object. */
    TPRREuint   nVertices_h;        /**< Number of vertices. */
    TPRREuint   nNormals_h;         /**< Number of normals. TODO: maybe can be removed as nVertices_h is enough. */
    TPRREuint   nFaces_h;           /**< Number of faces. */
    GLfloat*    pFbBuffer;          /**< Feedback buffer. */
    GLsizei     nFbBuffer_h;        /**< Size of feedback buffer. */

    // ---------------------------------------------------------------------------

    CConsole& getConsole() const;               /**< Hack to be able to use CConsole singleton instance instead of owner's protected console instance. */
    TPRREuint getIndexFromArray(const void* arr, TPRREuint index) const;     /**< Gets an index value from a given index array. */
    void      SetIndexInArray(void* arr, TPRREuint index, TPRREuint value);  /**< Sets an index value in a given index array. */
    TPRREbool isSwitchFromIndexedAllowed() const;                            /**< Tells whether it is allowed to switch from indexed to non-indexed vertex transfer mode. */
    void      ProcessGeometry(TPRREbool indexed) const;                      /**< Goes thru vertices and feeds them to OpenGL. */
    void      CompileIntoDisplayList(TPRREbool indexed);                     /**< Compiles OpenGL drawing commands into display list. */
    void      CompileIntoVertexBufferObjects(
        TPRREbool indexed, TPRREbool dynamic);                               /**< Compiles geometry into VBOs. */
    void      FreeGLresources();                                             /**< Frees up allocated resources from OpenGL. */
    void      SetArrayPointers(TPRREbool redirectToServer = false) const;    /**< Sets vertex data pointers prior to drawing. */
    void      ResetArrayPointers(TPRREbool redirectToServer = false) const;  /**< Resets pointers after drawing. */
    void      LoadTextureIntoTMU(const PRRETexture* tex, TPRREuint iTMU = 0) const;   /**< Loads the given texture into the texture mapping unit. */
    void      ApplyTransformations() const ;                                 /**< Applies transformations to the current modelview matrix based on the given object. */
    void      PrepareGLbeforeDraw(bool bLighting) const;
    void      DrawSW(); 

    friend class PRREObject3D;
    friend class PRREObject3DManager;

}; 