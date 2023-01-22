#pragma once

/*
    ###################################################################################
    PureObject3DImpl.h
    This file is part of PURE.
    Internal header.
    PureObject3DImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
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
        TPureuint    nFramesWaitedForOcclusionTestResultGlobalMin;  /**< Async: Minimum counted per-object nFramesWaitedForOcclusionTestResultMin value. */
        TPureuint    nFramesWaitedForOcclusionTestResultGlobalMax;  /**< Async: Maximum counted per-object nFramesWaitedForOcclusionTestResultMax value. */

        CurrentStats();
    };

    static const TPureuint OQ_MAX_FRAMES_WO_START_QUERY_WHEN_VISIBLE;
    static const TPureuint OQ_MAX_FRAMES_WO_START_QUERY_WHEN_OCCLUDED;

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

    TPure_TRANSFORMED_VERTEX* getTransformedVertices(
        TPurebool implicitAccessSubobject = true);
                
    PureVector&       getAngleVec();                
    const PureVector& getAngleVec() const;          
              
    PureVector        getScaledSizeVec() const;     
                
    const PureVector& getScaling() const;           
    void              SetScaling(TPurefloat value);
    void              SetScaling(const PureVector& value);
    void              Scale(TPurefloat value);
    void              Scale(const PureVector& value);

    TPurefloat        getBiggestAreaScaled() const;
    TPurefloat        recalculateBiggestAreaScaled();

    TPurebool isRenderingAllowed() const;                    
    void      SetRenderingAllowed(TPurebool state);          
    void      Show();                               
    void      Hide();                               
    TPurebool isColliding_TO_BE_REMOVED() const;              
    void      SetColliding_TO_BE_REMOVED(TPurebool value);    

    TPure_ROTATION_ORDER getRotationOrder() const;      
    void SetRotationOrder(TPure_ROTATION_ORDER value);  

    TPurebool isLit() const;                            
    void      SetLit(TPurebool value);                    
    TPurebool isDoubleSided() const;                     
    void      SetDoubleSided(TPurebool value);          
    TPurebool isWireframed() const;                     
    void      SetWireframed(TPurebool value);           
    TPurebool isWireframedCulled() const;               
    void      SetWireframedCulled(TPurebool value);      
    TPurebool isAffectingZBuffer() const;              
    void      SetAffectingZBuffer(TPurebool value);      
    TPurebool isTestingAgainstZBuffer() const;          
    void      SetTestingAgainstZBuffer(TPurebool value); 
    TPurebool isStickedToScreen() const;               
    void      SetStickedToScreen(TPurebool value);  

    TPurebool           isOccluder() const;
    void                SetOccluder(TPurebool value);
    TPurebool           isOccluded() const;
    TPurebool           isOcclusionTested() const;
    void                SetOcclusionTested(TPurebool state);
    const PureObject3D* getBoundingBoxObject() const;
    void                ForceFinishOcclusionTest();

    TPureuint getUsedSystemMemory() const; 

    TPureuint draw(
        const TPure_RENDER_PASS& renderPass,
        TPurebool bASyncQuery,
        TPurebool bRenderIfQueryPending);

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    PureObject3DImpl(
        PureObject3D* owner,
        const TPure_VERTEX_MODIFYING_HABIT& vmod = Pure_VMOD_STATIC,
        const TPure_VERTEX_REFERENCING_MODE& vref = Pure_VREF_DIRECT,
        TPurebool bForceUseClientMemory = false);
    
    PureObject3DImpl(const PureObject3DImpl&);
    PureObject3DImpl& operator=(const PureObject3DImpl&);

private:
    PureObject3D* _pOwner;              /**< The owner public object who creates this pimpl object. */
    PureObject3D* pRefersto;            /**< Pointer to the original object when we are just a cloned object. */
    std::set<PureObject3D*> referrers;  /**< Cloned objects currently referring to this object. */

    PureVector vAngle;              /**< 3D Angle. */
    PureVector vScaling;            /**< 3D Scaling. */
    TPurefloat fBiggestAreaScaled;  /**< Biggest area of object on either plane (XY, XZ or YZ), scaled by current scaling factor. */
    TPurebool bVisible;             /**< Visible state. */
    TPurebool bAffectedByLights;    /**< State of lit. */
    TPurebool bDoubleSided;         /**< Double sided. */
    TPurebool bWireframe;           /**< Wireframe mode. */
    TPurebool bWireframedCull;      /**< Wireframed culling. */
    TPurebool bAffectZBuffer;       /**< Writes to Z-Buffer. */
    TPurebool bAllowZTesting;       /**< Tests to Z-Buffer. */
    TPurebool bStickedToScreen;     /**< Sticked to screen. */
    TPure_ROTATION_ORDER rotation;  /**< Rotation order. */
    TPurebool bParentInitiatedOperation;   /**< Parent objects set this to true at the beginning of draw, subobjects ignore draw if this is not true in parent. */

    TPurebool bColliding;           /**< Colliding state. DEPRECATED: to be removed ... */

    TPure_TRANSFORMED_VERTEX* pVerticesTransf;    /**< Pointer to transformed vertices. NULL for level 1. Only for level 2. */
    GLfloat*                  pFbBuffer;          /**< Feedback buffer. NULL for level 1. Only for level 2. */
    GLsizei                   nFbBuffer_h;        /**< Size of feedback buffer. 0 for level 1. Only for level 2. */

    TPurebool     bOccluder;                                 /**< Should renderer treat this as occluder in occlusion culling tests? */   
    // following variables are for the sync occlusion query
    GLuint        nOcclusionQuery;                           /**< OpenGL Occlusion query id. Can stay 0 if occlusion query will never be run for this object. */
    PureObject3D* pBoundingBox;                              /**< Box to be rendered for occlusion testing. Stays NULL when nOcclusionQuery also stays 0. */
    // following variables extend the previous variables for the async occlusion query
    TPurebool     bOccluded;                                 /**< True if occlusion query resulted in it is occluded, false otherwise. Default value is false. */
    TPurebool     bOcclusionQueryStarted;                    /**< Is nOcclusionQuery currently running? */
    PFL::timeval  timeLongestWaitForSyncQueryFinish;         /**< Sync: Maximum time we had to wait for the query to finish. */
    TPureuint     nFramesWithoutOcclusionTest;               /**< Async: How many frames elapsed without testing if the object is occluded? */
    TPureuint     nFramesWaitedForOcclusionTestResult;       /**< Async: Counting frames elapsed since query start until we finally had result; always reset to 0 when we have a result. */
    TPureuint     nFramesWaitedForOcclusionTestResultMin;    /**< Async: Minimum number of frames elapsed since query start until we finally had result. */
    TPureuint     nFramesWaitedForOcclusionTestResultMax;    /**< Async: Maximum number of frames elapsed since query start until we finally had result. */

    // ---------------------------------------------------------------------------

    void      Draw_LoadTextureIntoTMU(const PureTexture* tex, TPureuint iTMU = 0) const;   /**< Loads the given texture into the texture mapping unit. */
    void      Draw_LoadTexturesAndSetBlendState() const;                                   /**< Loads all textures into all texture mapping units and sets blending if needed. */
    void      Draw_FeedbackBuffer_Start();                                                 /**< Set render mode to feedback and allocate buffer for transformed vertices. */
    void      Draw_FeedbackBuffer_Finish();                                                /**< Set render mode to default render. */
    void      Draw_ApplyTransformations() const ;                                          /**< Applies transformations to the current modelview matrix based on the given object. */
    void      Draw_PrepareGLBeforeDrawNormal(bool bLighting) const;
    void      glBeginOcclusionQuery() const;
    void      glEndOcclusionQuery() const;
    TPureuint draw_RenderBoundingBox() const;
    TPureuint draw_OcclusionQuery_Start(TPurebool async);
    TPurebool draw_OcclusionQuery_Finish(TPurebool async, TPurebool bRenderIfQueryPending);
    TPureuint draw_DrawSW();

    friend class PureObject3D;
    friend class PureObject3DManager;

}; 