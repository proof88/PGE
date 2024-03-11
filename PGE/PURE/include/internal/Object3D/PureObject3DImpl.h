#pragma once

/*
    ###################################################################################
    PureObject3DImpl.h
    This file is part of PURE.
    Internal header.
    PureObject3DImpl class.
    Made by PR00F88
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include <vector>
#include "../../external/Object3D/PureMesh3DManager.h"
#include "../../external/Object3D/PureObject3DManager.h"
#include "../gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PureObject3D::PureObject3DImpl
{

public:

    struct CurrentStats
    {
        PFL::timeval timeLongestGlobalWaitForSyncQueryFinish;       /**< Maximum measured per-object timeLongestWaitForSyncQueryFinish value. */
        TPureUInt    nFramesWaitedForOcclusionTestResultGlobalMin;  /**< Async: Minimum counted per-object nFramesWaitedForOcclusionTestResultMin value. */
        TPureUInt    nFramesWaitedForOcclusionTestResultGlobalMax;  /**< Async: Maximum counted per-object nFramesWaitedForOcclusionTestResultMax value. */

        CurrentStats();
    };

    static const TPureUInt OQ_MAX_FRAMES_WO_START_QUERY_WHEN_VISIBLE;
    static const TPureUInt OQ_MAX_FRAMES_WO_START_QUERY_WHEN_OCCLUDED;

    static std::vector<CurrentStats> stats;

    static std::deque<PureObject3D*> occluders;              /**< List of 3D occluders, should be updated whenever a 3D object becomes an occluder. */
    static std::deque<PureObject3D*> occludees_opaque;       /**< List of 3D opaque (non-blended) occludees, should be updated whenever a 3D (non-sticked) object becomes an opauqe occludee. */
    static std::deque<PureObject3D*> occludees_blended;      /**< List of 3D transparent/blended occludees, should be updated whenever a 3D (non-sticked) object becomes a blended occludee. */
    static std::deque<PureObject3D*> occludees_2d_opaque;    /**< List of 2D opaque (non-blended) occludees, should be updated whenever a 2D (sticked) object becomes an opauqe occludee. */
    static std::deque<PureObject3D*> occludees_2d_blended;   /**< List of 2D transparent/blended occludees, should be updated whenever a 2D (sticked) object becomes a blended occludee. */

    // ---------------------------------------------------------------------------

    virtual ~PureObject3DImpl();

    PureObject3D* getReferredObject() const;   
    const std::set<PureObject3D*>& getReferrerObjects() const;

    TPURE_TRANSFORMED_VERTEX* getTransformedVertices(
        TPureBool implicitAccessSubobject = true);
                
    PureVector&       getAngleVec();                
    const PureVector& getAngleVec() const;          
              
    PureVector        getScaledSizeVec() const;     
                
    const PureVector& getScaling() const;           
    void              SetScaling(TPureFloat value);
    void              SetScaling(const PureVector& value);
    void              Scale(TPureFloat value);
    void              Scale(const PureVector& value);

    TPureFloat        getBiggestAreaScaled() const;
    TPureFloat        recalculateBiggestAreaScaled();

    TPureBool isRenderingAllowed() const;                    
    void      SetRenderingAllowed(TPureBool state);          
    void      Show();                               
    void      Hide();                               
    TPureBool isColliding_TO_BE_REMOVED() const;              
    void      SetColliding_TO_BE_REMOVED(TPureBool value);    

    TPURE_ROTATION_ORDER getRotationOrder() const;      
    void SetRotationOrder(TPURE_ROTATION_ORDER value);  

    TPureBool isLit() const;                            
    void      SetLit(TPureBool value);                    
    TPureBool isDoubleSided() const;                     
    void      SetDoubleSided(TPureBool value);          
    TPureBool isWireframed() const;                     
    void      SetWireframed(TPureBool value);           
    TPureBool isWireframedCulled() const;               
    void      SetWireframedCulled(TPureBool value);      
    TPureBool isAffectingZBuffer() const;              
    void      SetAffectingZBuffer(TPureBool value);      
    TPureBool isTestingAgainstZBuffer() const;          
    void      SetTestingAgainstZBuffer(TPureBool value); 
    TPureBool isStickedToScreen() const;               
    void      SetStickedToScreen(TPureBool value);  

    TPureBool           isOccluder() const;
    void                SetOccluder(TPureBool value);
    TPureBool           isOccluded() const;
    TPureBool           isOcclusionTested() const;
    void                SetOcclusionTested(TPureBool state);
    const PureObject3D* getBoundingBoxObject() const;
    void                ForceFinishOcclusionTest();

    TPureUInt getUsedSystemMemory() const; 

    TPureUInt draw(
        const TPURE_RENDER_PASS& renderPass,
        TPureBool bASyncQuery,
        TPureBool bRenderIfQueryPending);

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    PureObject3DImpl(
        PureObject3D* owner,
        const TPURE_VERTEX_MODIFYING_HABIT& vmod = PURE_VMOD_STATIC,
        const TPURE_VERTEX_REFERENCING_MODE& vref = PURE_VREF_DIRECT,
        TPureBool bForceUseClientMemory = false);
    
    PureObject3DImpl(const PureObject3DImpl&);
    PureObject3DImpl& operator=(const PureObject3DImpl&);

private:
    PureObject3D* _pOwner;              /**< The owner public object who creates this pimpl object. */
    PureObject3D* pRefersto;            /**< Pointer to the original object when we are just a cloned object. */
    std::set<PureObject3D*> referrers;  /**< Cloned objects currently referring to this object. */

    PureVector vAngle;              /**< 3D Angle. */
    PureVector vScaling;            /**< 3D Scaling. */
    TPureFloat fBiggestAreaScaled;  /**< Biggest area of object on either plane (XY, XZ or YZ), scaled by current scaling factor. */
    TPureBool bVisible;             /**< Visible state. */
    TPureBool bAffectedByLights;    /**< State of lit. */
    TPureBool bDoubleSided;         /**< Double sided. */
    TPureBool bWireframe;           /**< Wireframe mode. */
    TPureBool bWireframedCull;      /**< Wireframed culling. */
    TPureBool bAffectZBuffer;       /**< Writes to Z-Buffer. */
    TPureBool bAllowZTesting;       /**< Tests to Z-Buffer. */
    TPureBool bStickedToScreen;     /**< Sticked to screen. */
    TPURE_ROTATION_ORDER rotation;  /**< Rotation order. */
    TPureBool bParentInitiatedOperation;   /**< Parent objects set this to true at the beginning of draw, subobjects ignore draw if this is not true in parent. */

    TPureBool bColliding;           /**< Colliding state. DEPRECATED: to be removed ... */

    TPURE_TRANSFORMED_VERTEX* pVerticesTransf;    /**< Pointer to transformed vertices. NULL for level 1. Only for level 2. */
    GLfloat*                  pFbBuffer;          /**< Feedback buffer. NULL for level 1. Only for level 2. */
    GLsizei                   nFbBuffer_h;        /**< Size of feedback buffer. 0 for level 1. Only for level 2. */

    TPureBool     bOccluder;                                 /**< Should renderer treat this as occluder in occlusion culling tests? */   
    // following variables are for the sync occlusion query
    GLuint        nOcclusionQuery;                           /**< OpenGL Occlusion query id. Can stay 0 if occlusion query will never be run for this object. */
    PureObject3D* pBoundingBox;                              /**< Box to be rendered for occlusion testing. Stays NULL when nOcclusionQuery also stays 0. */
    // following variables extend the previous variables for the async occlusion query
    TPureBool     bOccluded;                                 /**< True if occlusion query resulted in it is occluded, false otherwise. Default value is false. */
    TPureBool     bOcclusionQueryStarted;                    /**< Is nOcclusionQuery currently running? */
    PFL::timeval  timeLongestWaitForSyncQueryFinish;         /**< Sync: Maximum time we had to wait for the query to finish. */
    TPureUInt     nFramesWithoutOcclusionTest;               /**< Async: How many frames elapsed without testing if the object is occluded? */
    TPureUInt     nFramesWaitedForOcclusionTestResult;       /**< Async: Counting frames elapsed since query start until we finally had result; always reset to 0 when we have a result. */
    TPureUInt     nFramesWaitedForOcclusionTestResultMin;    /**< Async: Minimum number of frames elapsed since query start until we finally had result. */
    TPureUInt     nFramesWaitedForOcclusionTestResultMax;    /**< Async: Maximum number of frames elapsed since query start until we finally had result. */

    // ---------------------------------------------------------------------------

    void      Draw_LoadTextureIntoTMU(const PureTexture* tex, TPureUInt iTMU = 0) const;   /**< Loads the given texture into the texture mapping unit. */
    void      Draw_LoadTexturesAndSetBlendState() const;                                   /**< Loads all textures into all texture mapping units and sets blending if needed. */
    void      Draw_FeedbackBuffer_Start();                                                 /**< Set render mode to feedback and allocate buffer for transformed vertices. */
    void      Draw_FeedbackBuffer_Finish();                                                /**< Set render mode to default render. */
    void      Draw_ApplyTransformations() const ;                                          /**< Applies transformations to the current modelview matrix based on the given object. */
    void      Draw_PrepareGLBeforeDrawNormal(bool bLighting) const;
    void      glBeginOcclusionQuery() const;
    void      glEndOcclusionQuery() const;
    TPureUInt draw_RenderBoundingBox() const;
    TPureUInt draw_OcclusionQuery_Start(TPureBool async);
    TPureBool draw_OcclusionQuery_Finish(TPureBool async, TPureBool bRenderIfQueryPending);
    TPureUInt draw_DrawSW();

    friend class PureObject3D;
    friend class PureObject3DManager;

}; 