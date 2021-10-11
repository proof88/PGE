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

    struct CurrentStats
    {
        PFL::timeval timeLongestGlobalWaitForSyncQueryFinish;       /**< Maximum measured per-object timeLongestWaitForSyncQueryFinish value. */
        TPRREuint    nFramesWaitedForOcclusionTestResultGlobalMin;  /**< Async: Minimum counted per-object nFramesWaitedForOcclusionTestResultMin value. */
        TPRREuint    nFramesWaitedForOcclusionTestResultGlobalMax;  /**< Async: Maximum counted per-object nFramesWaitedForOcclusionTestResultMax value. */

        CurrentStats();
    };

    static const TPRREuint OQ_MAX_FRAMES_WO_START_QUERY_WHEN_VISIBLE;
    static const TPRREuint OQ_MAX_FRAMES_WO_START_QUERY_WHEN_OCCLUDED;

    static std::vector<CurrentStats> stats;

    static std::deque<PRREObject3D*> occluders;              /**< List of 3D occluders, should be updated whenever a 3D object becomes an occluder. */
    static std::deque<PRREObject3D*> occludees_opaque;       /**< List of 3D opaque (non-blended) occludees, should be updated whenever a 3D (non-sticked) object becomes an opauqe occludee. */
    static std::deque<PRREObject3D*> occludees_blended;      /**< List of 3D transparent/blended occludees, should be updated whenever a 3D (non-sticked) object becomes a blended occludee. */
    static std::deque<PRREObject3D*> occludees_2d_opaque;    /**< List of 2D opaque (non-blended) occludees, should be updated whenever a 2D (sticked) object becomes an opauqe occludee. */
    static std::deque<PRREObject3D*> occludees_2d_blended;   /**< List of 2D transparent/blended occludees, should be updated whenever a 2D (sticked) object becomes a blended occludee. */

    // ---------------------------------------------------------------------------

    virtual ~PRREObject3DImpl();

    PRREObject3D* getReferredObject() const;   
    const std::set<PRREObject3D*>& getReferrerObjects() const;

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

    TPRREfloat        getBiggestAreaScaled() const;
    TPRREfloat        recalculateBiggestAreaScaled();

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
    void                ForceFinishOcclusionTest();

    TPRREuint getUsedSystemMemory() const; 

    TPRREuint draw(
        const TPRRE_RENDER_PASS& renderPass,
        TPRREbool bASyncQuery,
        TPRREbool bRenderIfQueryPending);

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
    PRREObject3D* _pOwner;              /**< The owner public object who creates this pimpl object. */
    PRREObject3D* pRefersto;            /**< Pointer to the original object when we are just a cloned object. */
    std::set<PRREObject3D*> referrers;  /**< Cloned objects currently referring to this object. */

    PRREVector vAngle;              /**< 3D Angle. */
    PRREVector vScaling;            /**< 3D Scaling. */
    TPRREfloat fBiggestAreaScaled;  /**< Biggest area of object on either plane (XY, XZ or YZ), scaled by current scaling factor. */
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

    TPRRE_TRANSFORMED_VERTEX* pVerticesTransf;    /**< Pointer to transformed vertices. NULL for level 1. Only for level 2. */
    GLfloat*                  pFbBuffer;          /**< Feedback buffer. NULL for level 1. Only for level 2. */
    GLsizei                   nFbBuffer_h;        /**< Size of feedback buffer. 0 for level 1. Only for level 2. */

    TPRREbool     bOccluder;                                 /**< Should renderer treat this as occluder in occlusion culling tests? */   
    // following variables are for the sync occlusion query
    GLuint        nOcclusionQuery;                           /**< OpenGL Occlusion query id. Can stay 0 if occlusion query will never be run for this object. */
    PRREObject3D* pBoundingBox;                              /**< Box to be rendered for occlusion testing. Stays NULL when nOcclusionQuery also stays 0. */
    // following variables extend the previous variables for the async occlusion query
    TPRREbool     bOccluded;                                 /**< True if occlusion query resulted in it is occluded, false otherwise. Default value is false. */
    TPRREbool     bOcclusionQueryStarted;                    /**< Is nOcclusionQuery currently running? */
    PFL::timeval  timeLongestWaitForSyncQueryFinish;         /**< Sync: Maximum time we had to wait for the query to finish. */
    TPRREuint     nFramesWithoutOcclusionTest;               /**< Async: How many frames elapsed without testing if the object is occluded? */
    TPRREuint     nFramesWaitedForOcclusionTestResult;       /**< Async: Counting frames elapsed since query start until we finally had result; always reset to 0 when we have a result. */
    TPRREuint     nFramesWaitedForOcclusionTestResultMin;    /**< Async: Minimum number of frames elapsed since query start until we finally had result. */
    TPRREuint     nFramesWaitedForOcclusionTestResultMax;    /**< Async: Maximum number of frames elapsed since query start until we finally had result. */

    // ---------------------------------------------------------------------------

    void      Draw_LoadTextureIntoTMU(const PRRETexture* tex, TPRREuint iTMU = 0) const;   /**< Loads the given texture into the texture mapping unit. */
    void      Draw_LoadTexturesAndSetBlendState() const;                                   /**< Loads all textures into all texture mapping units and sets blending if needed. */
    void      Draw_FeedbackBuffer_Start();                                                 /**< Set render mode to feedback and allocate buffer for transformed vertices. */
    void      Draw_FeedbackBuffer_Finish();                                                /**< Set render mode to default render. */
    void      Draw_ApplyTransformations() const ;                                          /**< Applies transformations to the current modelview matrix based on the given object. */
    void      Draw_PrepareGLBeforeDrawNormal(bool bLighting) const;
    void      glBeginOcclusionQuery() const;
    void      glEndOcclusionQuery() const;
    TPRREuint draw_RenderBoundingBox() const;
    TPRREuint draw_OcclusionQuery_Start(TPRREbool async);
    TPRREbool draw_OcclusionQuery_Finish(TPRREbool async, TPRREbool bRenderIfQueryPending);
    TPRREuint draw_DrawSW();

    friend class PRREObject3D;
    friend class PRREObject3DManager;

}; 