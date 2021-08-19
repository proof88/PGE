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

#include "../../external/PRREallHeaders.h"
#include <vector>
#include "../../external/Object3D/PRREMesh3DManager.h"
#include "../../external/Object3D/PRREObject3DManager.h"
#include "../gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PRREObject3D::PRREObject3DImpl
{

public:

    static TPRREuint OQ_MAX_FRAMES_WO_START_QUERY_WHEN_VISIBLE;
    static TPRREuint OQ_MAX_FRAMES_WO_QUERY_START_WHEN_OCCLUDED;
    static TPRREbool OQ_ALWAYS_RENDER_WHEN_QUERY_IS_PENDING;
    static TPRREbool OQ_RENDER_BOUNDING_BOXES;

    // ---------------------------------------------------------------------------

    virtual ~PRREObject3DImpl();

    TPRRE_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;
    TPRREbool setVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod);
    TPRRE_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;
    TPRREbool setVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref);
    TPRRE_VERTEX_TRANSFER_MODE getVertexTransferMode() const;
    TPRREbool setVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans);

    PRREObject3D* getReferredObject() const;   

    TPRRE_TRANSFORMED_VERTEX* getTransformedVertices(
        TPRREbool implicitAccessSubobject = true);
                
    PRREVector&       getAngleVec();                
    const PRREVector& getAngleVec() const;          
              
    PRREVector        getScaledSizeVec() const;     
                
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

    TPRREbool           isOccluder() const;
    void                SetOccluder(TPRREbool value);
    TPRREbool           isOccluded() const;
    TPRREbool           isOcclusionTested() const;
    void                SetOcclusionTested(TPRREbool state);
    const PRREObject3D* getBoundingBoxObject() const;

    TPRREuint getUsedSystemMemory() const; 

    void Draw(const TPRRE_RENDER_PASS& renderPass);
    void DrawASyncQuery(const TPRRE_RENDER_PASS& renderPass);

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
    TPRREbool bParentInitiatedOperation;   /**< Parent objects set this to true at the beginning of draw, subobjects ignore draw if this is not true in parent. */

    TPRREbool bColliding;           /**< Colliding state. DEPRECATED: to be removed ... */

    TPRRE_TRANSFORMED_VERTEX* pVerticesTransf;    /**< Pointer to transformed vertices. */
    GLfloat*                  pFbBuffer;          /**< Feedback buffer. */
    GLsizei                   nFbBuffer_h;        /**< Size of feedback buffer. */

    TPRREbool     bOccluder;                                 /**< Should renderer treat this as occluder in occlusion culling tests? */   
    // following variables are for the sync occlusion query
    GLuint        nOcclusionQuery;                           /**< OpenGL Occlusion query id. Can stay 0 if occlusion query will never be run for this object. */
    PRREObject3D* pBoundingBox;                              /**< Box to be rendered for occlusion testing. Stays NULL when nOcclusionQuery also stays 0. */
    // following variables extend the previous variables for the async occlusion query
    TPRREbool     bOccluded;                                 /**< True if occlusion query resulted in it is occluded, false otherwise. Default value is false. */
    TPRREbool     bOcclusionQueryStarted;                    /**< Is nOcclusionQuery currently running? */
    TPRREuint     nFramesWithoutOcclusionTest;               /**< How many frames elapsed without testing if the object is occluded? */
    TPRREuint     nFramesWaitedForOcclusionTestResult;       // just for statistics
    TPRREuint     nFramesWaitedForOcclusionTestResultMin;    // just for statistics
    TPRREuint     nFramesWaitedForOcclusionTestResultMax;    // just for statistics

    // ---------------------------------------------------------------------------

    void      Draw_LoadTextureIntoTMU(const PRRETexture* tex, TPRREuint iTMU = 0) const;   /**< Loads the given texture into the texture mapping unit. */
    void      Draw_LoadTexturesAndSetBlendState() const;                                   /**< Loads all textures into all texture mapping units and sets blending if needed. */
    void      Draw_FeedbackBuffer_Start();                                                 /**< Set render mode to feedback and allocate buffer for transformed vertices. */
    void      Draw_FeedbackBuffer_Finish();                                                /**< Set render mode to default render. */
    void      Draw_ApplyTransformations() const ;                                          /**< Applies transformations to the current modelview matrix based on the given object. */
    void      Draw_PrepareGLBeforeDrawNormal(bool bLighting) const;
    void      glBeginOcclusionQuery() const;
    void      glEndOcclusionQuery() const;
    void      Draw_RenderBoundingBox() const;
    void      Draw_Sync_OcclusionQuery_Start();
    void      Draw_ASync_OcclusionQuery_Start();
    TPRREbool Draw_Sync_OcclusionQuery_Finish_And_Occluded();
    TPRREbool Draw_ASync_OcclusionQuery_Finish_And_Occluded();
    void      Draw_DrawSW(); 

    friend class PRREObject3D;
    friend class PRREObject3DManager;

}; 