#pragma once

/*
    ###################################################################################
    PureObject3DManager.h
    This file is part of PURE.
    External header.
    PureObject3DManager and PureObject3D classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include <deque>
#include <set>

#include "../PureAllHeaders.h"
#include "../PureFiledManager.h"
#include "PureMesh3DManager.h"
#include "PureVertexTransferMode.h"
#include "../Math/PureVector.h"


/**
    Rotation order of 3D objects.
*/
enum TPure_ROTATION_ORDER
{
    Pure_XYZ,
    Pure_ZYX,
    Pure_YXZ,
    Pure_YZX,
    Pure_XZY,
    Pure_ZXY
}; // TPure_ROTATION_ORDER


enum TPure_RENDER_PASS
{
    Pure_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY = 0,
    Pure_RPASS_Z_ONLY,
    Pure_RPASS_START_OCCLUSION_QUERY,
    Pure_RPASS_NORMAL,
}; // TPure_RENDER_PASS


/**
    Array of transformed vertices stores the result XYZW-coordinates of vertices calculated by CPU after Vertex Processing stage.
    This is currently used by software renderer implementation only as hardware renderer does such calculation in GPU.
*/
struct TPure_TRANSFORMED_VERTEX
{
    TXYZW coords;
    TPurebool draw;
};


class PureObject3DManager;

/**
    3D object class.

    Object3D objects are 2-level entities:
     - first level (parent) derives from a level-1 Mesh3D object, and owns properties such as world position, angle, etc.;
     - second level objects derive from level-2 Mesh3D objects, particularly the submeshes of the level-1 mesh from where the level-1 Object3D is also derived.
    This is way a Object3D is both a Managed and a Manager too: it is owned by Object3DManager but it owns its subobjects.

    The biggest addition of Object3D to VertexTransfer is the renderability.
*/
class PureObject3D :
    public PureVertexTransfer
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureObject3D is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureObject3D();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

    virtual TPurebool isLevel1() const;   /**< Tells if the mesh is a level-1 parent mesh. */
    virtual TPurebool isLevel2() const;   /**< Tells if the mesh is a level-2 submesh. */

    virtual TPure_PRIMITIVE_FORMAT getPrimitiveFormat() const;   /**< Gets the primitives' format. */

    virtual TPureuint   getVerticesCount() const;                                              /**< Gets the number of total vertices. */
    virtual const TXYZ* getVertices(TPurebool implicitAccessSubobject = true) const;           /**< Gets the pointer to vertices. */
    virtual       TXYZ* getVertices(TPurebool implicitAccessSubobject = true);                 /**< Gets the pointer to vertices. */

    virtual TPureuint   getVertexIndicesCount() const;                                         /**< Gets the number of total vertex indices. */
    virtual const void* getVertexIndices(TPurebool implicitAccessSubobject = true) const;      /**< Gets the pointer to vertex indices. */
    /* HACK: using unsigned int here instead of GLenum to avoid using GL header. */
    /* TODO: create own type for the index type. */
    virtual unsigned int getVertexIndicesType(TPurebool implicitAccessSubobject = true) const; /**< Gets the type of the indices. */
    virtual TPureuint    getMinVertexIndex(TPurebool implicitAccessSubobject = true) const;    /**< Gets the smallest index in the vertex indices array. */
    virtual TPureuint    getMaxVertexIndex(TPurebool implicitAccessSubobject = true) const;    /**< Gets the greatest index in the vertex indices array. */
    virtual TPureuint    getVertexIndex(
        TPureuint index, TPurebool implicitAccessSubobject = true) const;              /**< Gets an index value from the vertex indices array. */

    virtual const TXYZ* getNormals(TPurebool implicitAccessSubobject = true) const;            /**< Gets the pointer to normals. */

    virtual TPureuint getFaceCount() const;      /**< Gets the number of faces/polygons formed by the vertices. */
    virtual TPureuint getTriangleCount() const;  /**< Gets the number of triangles formed by the vertices. */

    virtual TPure_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;            /**< Gets vertex modifying habit. */
    virtual TPure_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;          /**< Gets vertex referencing mode. */
    virtual TPure_VERTEX_TRANSFER_MODE getVertexTransferMode() const;                /**< Gets vertex transfer mode. */
    virtual TPurebool setVertexTransferMode(TPure_VERTEX_TRANSFER_MODE vtrans);      /**< Sets vertex transfer mode. */

    virtual TPureuint getLastTransferredVertexCount() const;     /**< Gets the number of vertices sent to graphics pipeline by the last transferVertices() call. */
    virtual TPureuint getLastTransferredTriangleCount() const;   /**< Gets the number of triangles sent to graphics pipeline by the last transferVertices() call. */

    PureObject3D* getReferredObject() const;                    /**< Gets the original object which was cloned to create this object. */
    const std::set<PureObject3D*>& getReferrerObjects() const;  /**< Gets the cloned objects referring to this object. */

    const TPure_TRANSFORMED_VERTEX* getTransformedVertices(
        TPurebool implicitAccessSubobject = true) const;               /**< Gets the pointer to transformed vertices. */

    TPure_TRANSFORMED_VERTEX* getTransformedVertices(
        TPurebool implicitAccessSubobject = true);                     /**< Gets the pointer to transformed vertices. */

          PureVector& getAngleVec();                /**< Gets the rotation angles. */
    const PureVector& getAngleVec() const;          /**< Gets the rotation angles. */

    virtual const PureVector& getRelPosVec() const; /**< Gets the mesh-local relative position. */
    virtual const PureVector& getSizeVec() const;   /**< Gets the base sizes. */

          PureVector  getScaledSizeVec() const;     /**< Gets the real sizes considering the geometry size calculated from vertex data and the current scaling factor. */

    const PureVector& getScaling() const;           /**< Gets the scaling factor. */

    void  SetScaling(TPurefloat value);             /**< Sets the scaling factor to given scalar. */
    void  SetScaling(const PureVector& value);      /**< Sets the scaling factor to given vector. */

    void  Scale(TPurefloat value);                  /**< Scales by the given scalar value. */
    void  Scale(const PureVector& value);           /**< Scales by the given vector. */

    TPurefloat getBiggestAreaScaled() const;        /**< Gets the biggest area of the object on either plane (XY, XZ or YZ), scaled by current scaling factor. */
    TPurefloat recalculateBiggestAreaScaled();      /**< Recalculates biggest area of object on either plane (XY, XZ or YZ), scaled by current scaling factor. */

    TPurebool isRenderingAllowed() const;           /**< Gets if rendering is allowed. */
    void      SetRenderingAllowed(TPurebool state); /**< Sets if rendering is allowed. */
    void      Show();                               /**< Enables rendering of this object. */
    void      Hide();                               /**< Disables rendering of this object. */
    TPurebool isColliding_TO_BE_REMOVED() const;              /**< Gets whether colliding is enabled. */
    void      SetColliding_TO_BE_REMOVED(TPurebool value);    /**< Sets whether colliding is enabled. */

    TPure_ROTATION_ORDER getRotationOrder() const;      /**< Gets the rotation order. */
    void SetRotationOrder(TPure_ROTATION_ORDER value);  /**< Sets the rotation order. */

    TPurebool isLit() const;                              /**< Gets the lit state. */
    void      SetLit(TPurebool value);                    /**< Sets the lit state. */
    TPurebool isDoubleSided() const;                      /**< Gets the double sided state. */
    void      SetDoubleSided(TPurebool value);            /**< Sets the double sided state. */
    TPurebool isWireframed() const;                       /**< Gets the wireframed state. */
    void      SetWireframed(TPurebool value);             /**< Sets the wireframed state. */
    TPurebool isWireframedCulled() const;                 /**< Gets the wireframed culling state. */
    void      SetWireframedCulled(TPurebool value);       /**< Sets the wireframed culling state. */
    TPurebool isAffectingZBuffer() const;                 /**< Gets whether we write to the Z-Buffer while rendering. */
    void      SetAffectingZBuffer(TPurebool value);       /**< Sets whether we write to the Z-Buffer while rendering. */
    TPurebool isTestingAgainstZBuffer() const;            /**< Gets whether we test against the Z-Buffer while rendering. */
    void      SetTestingAgainstZBuffer(TPurebool value);  /**< Sets whether we test against the Z-Buffer while rendering. */
    TPurebool isStickedToScreen() const;                  /**< Gets the sticked-to-screen state. */
    void      SetStickedToScreen(TPurebool value);        /**< Sets the sticked-to-screen state. */ 

    TPurebool isOccluder() const;                         /**< Gets whether this object should be considered as an occluder during rendering. */
    void      SetOccluder(TPurebool value);               /**< Sets whether this object should be considered as an occluder during rendering. */
    TPurebool isOccluded() const;                         /**< Gets whether this object was occluded or not based on the last finished occlusion test. */
    TPurebool isOcclusionTested() const;                  /**< Gets whether this object is being tested if it is occluded or not. */
    void      SetOcclusionTested(TPurebool state);        /**< Sets whether this object should be tested if it is occluded or not. */
    const PureObject3D* getBoundingBoxObject() const;     /**< Gets the bounding box object used for occlusion tests. */
    void      ForceFinishOcclusionTest();                 /**< Waits for any pending occlusion test to be finished and reset occluded state. */

    // remember: since Object3D is also a manager, it could also implement property change handler for changes in any of its subobjects made "outside" e.g. in their material!
    /*virtual void HandleManagedPropertyChanged(
                 PureManaged& m);*/

    virtual TPureuint getUsedSystemMemory() const;        /**< Gets the amount of allocated system memory. */
    virtual TPureuint getUsedVideoMemory() const;         /**< Gets the amount of allocated video memory. */

    TPureuint draw(
        const TPure_RENDER_PASS& renderPass,
        TPurebool bASyncQuery,
        TPurebool bRenderIfQueryPending);                 /**< Draws the object. */

protected:

    // ---------------------------------------------------------------------------
    
    PureObject3D(
        PureMaterialManager& matMgr,
        const TPure_VERTEX_MODIFYING_HABIT& vmod = Pure_VMOD_STATIC,
        const TPure_VERTEX_REFERENCING_MODE& vref = Pure_VREF_DIRECT,
        TPurebool bForceUseClientMemory = false);                        /**< Only PureObject3DManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PureObject3D(const PureObject3D&);                                   
    PureObject3D& operator=(const PureObject3D&);

    virtual void ResetLastTransferredCounts();        /**< Reset counters used for measuring number of vertices, triangles, etc. sent to the graphics pipeline by the last transferVertices(). */

private:
    class PureObject3DImpl;
    PureObject3DImpl* pImpl;

    friend class PureObject3DManager;  

}; // class PureObject3D


/**
    3D object manager class.
    Besides the usual PureManager way of iterating over its managed instances, a few more functions are available for iterating:
     - getOccluders()
     - get3dOpaqueOccludees()
     - get3dBlendedOccludees()
     - get2dOpaqueOccludees()
     - get2dBlendedOccludees()
*/
class PureObject3DManager :
    public PureMesh3DManager
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureObject3DManager is included")
#endif

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureObject3DManager(PureTextureManager& texMgr, PureMaterialManager& matMgr); /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureObject3DManager();

    virtual void Attach(PureManaged& m);              /**< Adds the given managed to the manager, if the managed has no manager yet. */
    virtual void Detach(PureManaged& m);              /**< Removes the given managed from the manager, so the managed will have no manager. */ 

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPurebool isInitialized() const;                  /**< Tells whether the manager is correctly initialized or not. */

    std::deque<PureObject3D*>& getOccluders();                       /**< Get a list of occluders. */
    const std::deque<PureObject3D*>& getOccluders() const;           /**< Get a list of occluders. */
    std::deque<PureObject3D*>& get3dOpaqueOccludees();               /**< Get a list of 3D (non-sticked) opaque (non-blended) occludees. */
    const std::deque<PureObject3D*>& get3dOpaqueOccludees() const;   /**< Get a list of 3D (non-sticked) opaque (non-blended) occludees. */
    std::deque<PureObject3D*>& get3dBlendedOccludees();              /**< Get a list of 3D (non-sticked) blended occludees. */
    const std::deque<PureObject3D*>& get3dBlendedOccludees() const;  /**< Get a list of 3D (non-sticked) blended occludees. */
    std::deque<PureObject3D*>& get2dOpaqueOccludees();               /**< Get a list of 2D (sticked) opaque (non-blended) occludees. */
    const std::deque<PureObject3D*>& get2dOpaqueOccludees() const;   /**< Get a list of 2D (sticked) opaque (non-blended) occludees. */
    std::deque<PureObject3D*>& get2dBlendedOccludees();              /**< Get a list of 2D (sticked) blended occludees. */
    const std::deque<PureObject3D*>& get2dBlendedOccludees() const;  /**< Get a list of 2D (sticked) blended occludees. */
    
    PureObject3D* createPlane(
        TPurefloat a, TPurefloat b,
        TPure_VERTEX_MODIFYING_HABIT vmod = Pure_VMOD_STATIC,
        TPure_VERTEX_REFERENCING_MODE vref = Pure_VREF_DIRECT,
        TPurebool bForceUseClientMemory = false);                /**< Creates a new plane with the given sizes. */
    
    PureObject3D* createBox(
        TPurefloat a, TPurefloat b, TPurefloat c,
        TPure_VERTEX_MODIFYING_HABIT vmod = Pure_VMOD_STATIC,
        TPure_VERTEX_REFERENCING_MODE vref = Pure_VREF_DIRECT,
        TPurebool bForceUseClientMemory = false);                /**< Creates a new box with the given sizes. */
    
    PureObject3D* createCube(
        TPurefloat a,
        TPure_VERTEX_MODIFYING_HABIT vmod = Pure_VMOD_STATIC,
        TPure_VERTEX_REFERENCING_MODE vref = Pure_VREF_DIRECT,
        TPurebool bForceUseClientMemory = false);                /**< Creates a new cube with the given sizes. */

    PureObject3D* createFromFile(
        const char* filename,
        TPure_VERTEX_MODIFYING_HABIT vmod,
        TPure_VERTEX_REFERENCING_MODE vref,
        TPurebool bForceUseClientMemory = false);                /**< Creates object from the given file. */

    PureObject3D* createFromFile(const char* filename);          /**< Creates object from the given file. */

    PureObject3D* createCloned(PureObject3D& referredobj);       /**< Creates a new object by cloning an already existing object. */

    void UpdateOccluderStates();                                 /**< Iterates over its manageds and updates their occluder states. */
    TPurefloat getOccluderSelectionBias() const;                 /**< Gets the occluder selection bias. */
    void SetOccluderSelectionBias(TPurefloat fBias);             /**< Sets the occluder selection bias. */
    TPureuint getMaxOccluderCount() const;                       /**< Gets the maximum number of occluders. */
    void SetMaxOccluderCount(TPureuint nMax);                    /**< Sets the maximum number of occluders. */

    virtual void HandleManagedPropertyChanged(
                 PureManaged& m);                                /**< Should be invoked when a managed's property got changed from a different kind of manager or managed. */

    void         ResetStatistics();                              /**< Resets any kind of statistics collected during its lifetime. */

    TPureuint    getUsedVideoMemory() const;                     /**< Gets the amount of allocated video memory for all objects owner by this manager. */

    virtual void WriteList() const;                              /**< From PureMesh3DManager, adding logging vertex transfer and object data, including some occlusion query stats. */

protected:

    PureObject3DManager();  /* TODO: mark this as noexcept(false) when using newer compiler! */

    PureObject3DManager(const PureObject3DManager&);
    PureObject3DManager& operator=(const PureObject3DManager&);

    virtual void WriteListCallback(const PureManaged& mngd) const;

private:
    class PureObject3DManagerImpl;
    PureObject3DManagerImpl* pImpl;  

}; // class PureObject3DManager
