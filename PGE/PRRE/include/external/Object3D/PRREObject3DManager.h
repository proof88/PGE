#pragma once

/*
    ###################################################################################
    PRREObject3DManager.h
    This file is part of PRRE.
    External header.
    PRREObject3DManager and PRREObject3D classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <deque>
#include <set>

#include "../PRREallHeaders.h"
#include "../PRREFiledManager.h"
#include "PRREMesh3DManager.h"
#include "PRREVertexTransferMode.h"
#include "../Math/PRREVector.h"


/**
    Rotation order of 3D objects.
*/
enum TPRRE_ROTATION_ORDER
{
    PRRE_XYZ,
    PRRE_ZYX,
    PRRE_YXZ,
    PRRE_YZX,
    PRRE_XZY,
    PRRE_ZXY
}; // TPRRE_ROTATION_ORDER


enum TPRRE_RENDER_PASS
{
    PRRE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY = 0,
    PRRE_RPASS_Z_ONLY,
    PRRE_RPASS_START_OCCLUSION_QUERY,
    PRRE_RPASS_NORMAL,
}; // TPRRE_RENDER_PASS


/**
    Array of transformed vertices stores the result XYZW-coordinates of vertices calculated by CPU after Vertex Processing stage.
    This is currently used by software renderer implementation only as hardware renderer does such calculation in GPU.
*/
struct TPRRE_TRANSFORMED_VERTEX
{
    TXYZW coords;
    TPRREbool draw;
};


class PRREObject3DManager;

/**
    3D object class.

    Object3D objects are 2-level entities:
     - first level (parent) derives from a level-1 Mesh3D object, and owns properties such as world position, angle, etc.;
     - second level objects derive from level-2 Mesh3D objects, particularly the submeshes of the level-1 mesh from where the level-1 Object3D is also derived.
    This is way a Object3D is both a Managed and a Manager too: it is owned by Object3DManager but it owns its subobjects.

    The biggest addition of Object3D to VertexTransfer is the renderability.
*/
class PRREObject3D :
    public PRREVertexTransfer
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREObject3D is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PRREObject3D();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

    virtual TPRREbool isLevel1() const;   /**< Tells if the mesh is a level-1 parent mesh. */
    virtual TPRREbool isLevel2() const;   /**< Tells if the mesh is a level-2 submesh. */

    virtual TPRRE_PRIMITIVE_FORMAT getPrimitiveFormat() const;   /**< Gets the primitives' format. */

    virtual TPRREuint   getVerticesCount() const;                                              /**< Gets the number of total vertices. */
    virtual const TXYZ* getVertices(TPRREbool implicitAccessSubobject = true) const;           /**< Gets the pointer to vertices. */
    virtual       TXYZ* getVertices(TPRREbool implicitAccessSubobject = true);                 /**< Gets the pointer to vertices. */

    virtual TPRREuint   getVertexIndicesCount() const;                                         /**< Gets the number of total vertex indices. */
    virtual const void* getVertexIndices(TPRREbool implicitAccessSubobject = true) const;      /**< Gets the pointer to vertex indices. */
    /* HACK: using unsigned int here instead of GLenum to avoid using GL header. */
    /* TODO: create own type for the index type. */
    virtual unsigned int getVertexIndicesType(TPRREbool implicitAccessSubobject = true) const; /**< Gets the type of the indices. */
    virtual TPRREuint    getMinVertexIndex(TPRREbool implicitAccessSubobject = true) const;    /**< Gets the smallest index in the vertex indices array. */
    virtual TPRREuint    getMaxVertexIndex(TPRREbool implicitAccessSubobject = true) const;    /**< Gets the greatest index in the vertex indices array. */
    virtual TPRREuint    getVertexIndex(
        TPRREuint index, TPRREbool implicitAccessSubobject = true) const;              /**< Gets an index value from the vertex indices array. */

    virtual const TXYZ* getNormals(TPRREbool implicitAccessSubobject = true) const;            /**< Gets the pointer to normals. */

    virtual TPRREuint getFaceCount() const;      /**< Gets the number of faces/polygons formed by the vertices. */
    virtual TPRREuint getTriangleCount() const;  /**< Gets the number of triangles formed by the vertices. */

    virtual TPRRE_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;            /**< Gets vertex modifying habit. */
    virtual TPRRE_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;          /**< Gets vertex referencing mode. */
    virtual TPRRE_VERTEX_TRANSFER_MODE getVertexTransferMode() const;                /**< Gets vertex transfer mode. */
    virtual TPRREbool setVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans);      /**< Sets vertex transfer mode. */

    virtual TPRREuint getLastTransferredVertexCount() const;     /**< Gets the number of vertices sent to graphics pipeline by the last transferVertices() call. */
    virtual TPRREuint getLastTransferredTriangleCount() const;   /**< Gets the number of triangles sent to graphics pipeline by the last transferVertices() call. */

    PRREObject3D* getReferredObject() const;                    /**< Gets the original object which was cloned to create this object. */
    const std::set<PRREObject3D*>& getReferrerObjects() const;  /**< Gets the cloned objects referring to this object. */

    const TPRRE_TRANSFORMED_VERTEX* getTransformedVertices(
        TPRREbool implicitAccessSubobject = true) const;               /**< Gets the pointer to transformed vertices. */

    TPRRE_TRANSFORMED_VERTEX* getTransformedVertices(
        TPRREbool implicitAccessSubobject = true);                     /**< Gets the pointer to transformed vertices. */

          PRREVector& getAngleVec();                /**< Gets the rotation angles. */
    const PRREVector& getAngleVec() const;          /**< Gets the rotation angles. */

    virtual const PRREVector& getRelPosVec() const; /**< Gets the mesh-local relative position. */
    virtual const PRREVector& getSizeVec() const;   /**< Gets the base sizes. */

          PRREVector  getScaledSizeVec() const;     /**< Gets the real sizes considering the geometry size calculated from vertex data and the current scaling factor. */

    const PRREVector& getScaling() const;           /**< Gets the scaling factor. */

    void  SetScaling(TPRREfloat value);             /**< Sets the scaling factor to given scalar. */
    void  SetScaling(const PRREVector& value);      /**< Sets the scaling factor to given vector. */

    void  Scale(TPRREfloat value);                  /**< Scales by the given scalar value. */
    void  Scale(const PRREVector& value);           /**< Scales by the given vector. */

    TPRREfloat getBiggestAreaScaled() const;        /**< Gets the biggest area of the object on either plane (XY, XZ or YZ), scaled by current scaling factor. */
    TPRREfloat recalculateBiggestAreaScaled();      /**< Recalculates biggest area of object on either plane (XY, XZ or YZ), scaled by current scaling factor. */

    TPRREbool isRenderingAllowed() const;           /**< Gets if rendering is allowed. */
    void      SetRenderingAllowed(TPRREbool state); /**< Sets if rendering is allowed. */
    void      Show();                               /**< Enables rendering of this object. */
    void      Hide();                               /**< Disables rendering of this object. */
    TPRREbool isColliding_TO_BE_REMOVED() const;              /**< Gets whether colliding is enabled. */
    void      SetColliding_TO_BE_REMOVED(TPRREbool value);    /**< Sets whether colliding is enabled. */

    TPRRE_ROTATION_ORDER getRotationOrder() const;      /**< Gets the rotation order. */
    void SetRotationOrder(TPRRE_ROTATION_ORDER value);  /**< Sets the rotation order. */

    TPRREbool isLit() const;                              /**< Gets the lit state. */
    void      SetLit(TPRREbool value);                    /**< Sets the lit state. */
    TPRREbool isDoubleSided() const;                      /**< Gets the double sided state. */
    void      SetDoubleSided(TPRREbool value);            /**< Sets the double sided state. */
    TPRREbool isWireframed() const;                       /**< Gets the wireframed state. */
    void      SetWireframed(TPRREbool value);             /**< Sets the wireframed state. */
    TPRREbool isWireframedCulled() const;                 /**< Gets the wireframed culling state. */
    void      SetWireframedCulled(TPRREbool value);       /**< Sets the wireframed culling state. */
    TPRREbool isAffectingZBuffer() const;                 /**< Gets whether we write to the Z-Buffer while rendering. */
    void      SetAffectingZBuffer(TPRREbool value);       /**< Sets whether we write to the Z-Buffer while rendering. */
    TPRREbool isTestingAgainstZBuffer() const;            /**< Gets whether we test against the Z-Buffer while rendering. */
    void      SetTestingAgainstZBuffer(TPRREbool value);  /**< Sets whether we test against the Z-Buffer while rendering. */
    TPRREbool isStickedToScreen() const;                  /**< Gets the sticked-to-screen state. */
    void      SetStickedToScreen(TPRREbool value);        /**< Sets the sticked-to-screen state. */ 

    TPRREbool isOccluder() const;                         /**< Gets whether this object should be considered as an occluder during rendering. */
    void      SetOccluder(TPRREbool value);               /**< Sets whether this object should be considered as an occluder during rendering. */
    TPRREbool isOccluded() const;                         /**< Gets whether this object was occluded or not based on the last finished occlusion test. */
    TPRREbool isOcclusionTested() const;                  /**< Gets whether this object is being tested if it is occluded or not. */
    void      SetOcclusionTested(TPRREbool state);        /**< Sets whether this object should be tested if it is occluded or not. */
    const PRREObject3D* getBoundingBoxObject() const;     /**< Gets the bounding box object used for occlusion tests. */
    void      ForceFinishOcclusionTest();                 /**< Waits for any pending occlusion test to be finished and reset occluded state. */

    // remember: since Object3D is also a manager, it could also implement property change handler for changes in any of its subobjects made "outside" e.g. in their material!
    /*virtual void HandleManagedPropertyChanged(
                 PRREManaged& m);*/

    virtual TPRREuint getUsedSystemMemory() const;        /**< Gets the amount of allocated system memory. */
    virtual TPRREuint getUsedVideoMemory() const;         /**< Gets the amount of allocated video memory. */

    TPRREuint draw(
        const TPRRE_RENDER_PASS& renderPass,
        TPRREbool bASyncQuery,
        TPRREbool bRenderIfQueryPending);                 /**< Draws the object. */

protected:

    // ---------------------------------------------------------------------------
    
    PRREObject3D(
        PRREMaterialManager& matMgr,
        const TPRRE_VERTEX_MODIFYING_HABIT& vmod = PRRE_VMOD_STATIC,
        const TPRRE_VERTEX_REFERENCING_MODE& vref = PRRE_VREF_DIRECT,
        TPRREbool bForceUseClientMemory = false);                        /**< Only PRREObject3DManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PRREObject3D(const PRREObject3D&);                                   
    PRREObject3D& operator=(const PRREObject3D&);

    virtual void ResetLastTransferredCounts();        /**< Reset counters used for measuring number of vertices, triangles, etc. sent to the graphics pipeline by the last transferVertices(). */

private:
    class PRREObject3DImpl;
    PRREObject3DImpl* pImpl;

    friend class PRREObject3DManager;  

}; // class PRREObject3D


/**
    3D object manager class.
*/
class PRREObject3DManager :
    public PRREMesh3DManager
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREObject3DManager is included")
#endif

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PRREObject3DManager(PRRETextureManager& texMgr, PRREMaterialManager& matMgr); /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PRREObject3DManager();

    virtual void Attach(PRREManaged& m);              /**< Adds the given managed to the manager, if the managed has no manager yet. */
    virtual void Detach(PRREManaged& m);              /**< Removes the given managed from the manager, so the managed will have no manager. */ 

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPRREbool isInitialized() const;                  /**< Tells whether the manager is correctly initialized or not. */

    std::deque<PRREObject3D*>& getOccluders();                       /**< Get a list of occluders. */
    const std::deque<PRREObject3D*>& getOccluders() const;           /**< Get a list of occluders. */
    std::deque<PRREObject3D*>& get3dOpaqueOccludees();               /**< Get a list of 3D (non-sticked) opaque (non-blended) occludees. */
    const std::deque<PRREObject3D*>& get3dOpaqueOccludees() const;   /**< Get a list of 3D (non-sticked) opaque (non-blended) occludees. */
    std::deque<PRREObject3D*>& get3dBlendedOccludees();              /**< Get a list of 3D (non-sticked) blended occludees. */
    const std::deque<PRREObject3D*>& get3dBlendedOccludees() const;  /**< Get a list of 3D (non-sticked) blended occludees. */
    std::deque<PRREObject3D*>& get2dOpaqueOccludees();               /**< Get a list of 2D (sticked) opaque (non-blended) occludees. */
    const std::deque<PRREObject3D*>& get2dOpaqueOccludees() const;   /**< Get a list of 2D (sticked) opaque (non-blended) occludees. */
    std::deque<PRREObject3D*>& get2dBlendedOccludees();              /**< Get a list of 2D (sticked) blended occludees. */
    const std::deque<PRREObject3D*>& get2dBlendedOccludees() const;  /**< Get a list of 2D (sticked) blended occludees. */
    
    PRREObject3D* createPlane(
        TPRREfloat a, TPRREfloat b,
        TPRRE_VERTEX_MODIFYING_HABIT vmod = PRRE_VMOD_STATIC,
        TPRRE_VERTEX_REFERENCING_MODE vref = PRRE_VREF_DIRECT,
        TPRREbool bForceUseClientMemory = false);                /**< Creates a new plane with the given sizes. */
    
    PRREObject3D* createBox(
        TPRREfloat a, TPRREfloat b, TPRREfloat c,
        TPRRE_VERTEX_MODIFYING_HABIT vmod = PRRE_VMOD_STATIC,
        TPRRE_VERTEX_REFERENCING_MODE vref = PRRE_VREF_DIRECT,
        TPRREbool bForceUseClientMemory = false);                /**< Creates a new box with the given sizes. */
    
    PRREObject3D* createCube(
        TPRREfloat a,
        TPRRE_VERTEX_MODIFYING_HABIT vmod = PRRE_VMOD_STATIC,
        TPRRE_VERTEX_REFERENCING_MODE vref = PRRE_VREF_DIRECT,
        TPRREbool bForceUseClientMemory = false);                /**< Creates a new cube with the given sizes. */

    PRREObject3D* createFromFile(
        const char* filename,
        TPRRE_VERTEX_MODIFYING_HABIT vmod,
        TPRRE_VERTEX_REFERENCING_MODE vref,
        TPRREbool bForceUseClientMemory = false);                /**< Creates object from the given file. */

    PRREObject3D* createFromFile(const char* filename);          /**< Creates object from the given file. */

    PRREObject3D* createCloned(PRREObject3D& referredobj);       /**< Creates a new object by cloning an already existing object. */

    void UpdateOccluderStates();                                 /**< Iterates over its manageds and updates their occluder states. */

    virtual void HandleManagedPropertyChanged(
                 PRREManaged& m);                                /**< Should be invoked when a managed's property got changed from a different kind of manager or managed. */

    void         ResetStatistics();                              /**< Resets any kind of statistics collected during its lifetime. */

    TPRREuint    getUsedVideoMemory() const;                     /**< Gets the amount of allocated video memory for all objects owner by this manager. */

    virtual void WriteList() const;

protected:

    PRREObject3DManager();  /* TODO: mark this as noexcept(false) when using newer compiler! */

    PRREObject3DManager(const PRREObject3DManager&);
    PRREObject3DManager& operator=(const PRREObject3DManager&);

    virtual void WriteListCallback(const PRREManaged& mngd) const;

private:
    class PRREObject3DManagerImpl;
    PRREObject3DManagerImpl* pImpl;  

}; // class PRREObject3DManager
