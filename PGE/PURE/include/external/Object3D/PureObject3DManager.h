#pragma once

/*
    ###################################################################################
    PureObject3DManager.h
    This file is part of PURE.
    External header.
    PureObject3DManager and PureObject3D classes.
    Made by PR00F88
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
enum TPURE_ROTATION_ORDER
{
    PURE_XYZ,
    PURE_ZYX,
    PURE_YXZ,
    PURE_YZX,
    PURE_XZY,
    PURE_ZXY
}; // TPURE_ROTATION_ORDER


enum TPURE_RENDER_PASS
{
    PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY = 0,
    PURE_RPASS_Z_ONLY,
    PURE_RPASS_START_OCCLUSION_QUERY,
    PURE_RPASS_NORMAL,
}; // TPURE_RENDER_PASS


/**
    Array of transformed vertices stores the result XYZW-coordinates of vertices calculated by CPU after Vertex Processing stage.
    This is currently used by software renderer implementation only as hardware renderer does such calculation in GPU.
*/
struct TPURE_TRANSFORMED_VERTEX
{
    TXYZW coords;
    TPureBool draw;
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
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureObject3D is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureObject3D();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

    virtual TPureBool isLevel1() const;   /**< Tells if the mesh is a level-1 parent mesh. */
    virtual TPureBool isLevel2() const;   /**< Tells if the mesh is a level-2 submesh. */

    virtual TPURE_PRIMITIVE_FORMAT getPrimitiveFormat() const;   /**< Gets the primitives' format. */

    virtual TPureUInt   getVerticesCount() const;                                              /**< Gets the number of total vertices. */
    virtual const TXYZ* getVertices(TPureBool implicitAccessSubobject = true) const;           /**< Gets the pointer to vertices. */
    virtual       TXYZ* getVertices(TPureBool implicitAccessSubobject = true);                 /**< Gets the pointer to vertices. */

    virtual TPureUInt   getVertexIndicesCount() const;                                         /**< Gets the number of total vertex indices. */
    virtual const void* getVertexIndices(TPureBool implicitAccessSubobject = true) const;      /**< Gets the pointer to vertex indices. */
    /* HACK: using unsigned int here instead of GLenum to avoid using GL header. */
    /* TODO: create own type for the index type. */
    virtual unsigned int getVertexIndicesType(TPureBool implicitAccessSubobject = true) const; /**< Gets the type of the indices. */
    virtual TPureUInt    getMinVertexIndex(TPureBool implicitAccessSubobject = true) const;    /**< Gets the smallest index in the vertex indices array. */
    virtual TPureUInt    getMaxVertexIndex(TPureBool implicitAccessSubobject = true) const;    /**< Gets the greatest index in the vertex indices array. */
    virtual TPureUInt    getVertexIndex(
        TPureUInt index, TPureBool implicitAccessSubobject = true) const;              /**< Gets an index value from the vertex indices array. */

    virtual const TXYZ* getNormals(TPureBool implicitAccessSubobject = true) const;            /**< Gets the pointer to normals. */

    virtual TPureUInt getFaceCount() const;      /**< Gets the number of faces/polygons formed by the vertices. */
    virtual TPureUInt getTriangleCount() const;  /**< Gets the number of triangles formed by the vertices. */

    virtual TPURE_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;            /**< Gets vertex modifying habit. */
    virtual TPURE_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;          /**< Gets vertex referencing mode. */
    virtual TPURE_VERTEX_TRANSFER_MODE getVertexTransferMode() const;                /**< Gets vertex transfer mode. */
    virtual TPureBool setVertexTransferMode(TPURE_VERTEX_TRANSFER_MODE vtrans);      /**< Sets vertex transfer mode. */

    virtual TPureUInt getLastTransferredVertexCount() const;     /**< Gets the number of vertices sent to graphics pipeline by the last transferVertices() call. */
    virtual TPureUInt getLastTransferredTriangleCount() const;   /**< Gets the number of triangles sent to graphics pipeline by the last transferVertices() call. */

    PureObject3D* getReferredObject() const;                    /**< Gets the original object which was cloned to create this object. */
    const std::set<PureObject3D*>& getReferrerObjects() const;  /**< Gets the cloned objects referring to this object. */

    const TPURE_TRANSFORMED_VERTEX* getTransformedVertices(
        TPureBool implicitAccessSubobject = true) const;               /**< Gets the pointer to transformed vertices. */

    TPURE_TRANSFORMED_VERTEX* getTransformedVertices(
        TPureBool implicitAccessSubobject = true);                     /**< Gets the pointer to transformed vertices. */

          PureVector& getAngleVec();                /**< Gets the rotation angles. */
    const PureVector& getAngleVec() const;          /**< Gets the rotation angles. */

    virtual const PureVector& getRelPosVec() const; /**< Gets the mesh-local relative position. */
    virtual const PureVector& getSizeVec() const;   /**< Gets the base sizes. */

          PureVector  getScaledSizeVec() const;     /**< Gets the real sizes considering the geometry size calculated from vertex data and the current scaling factor. */

    const PureVector& getScaling() const;           /**< Gets the scaling factor. */

    void  SetScaling(TPureFloat value);             /**< Sets the scaling factor to given scalar. */
    void  SetScaling(const PureVector& value);      /**< Sets the scaling factor to given vector. */

    void  Scale(TPureFloat value);                  /**< Scales by the given scalar value. */
    void  Scale(const PureVector& value);           /**< Scales by the given vector. */

    TPureFloat getBiggestAreaScaled() const;        /**< Gets the biggest area of the object on either plane (XY, XZ or YZ), scaled by current scaling factor. */
    TPureFloat recalculateBiggestAreaScaled();      /**< Recalculates biggest area of object on either plane (XY, XZ or YZ), scaled by current scaling factor. */

    TPureBool isRenderingAllowed() const;           /**< Gets if rendering is allowed. */
    void      SetRenderingAllowed(TPureBool state); /**< Sets if rendering is allowed. */
    void      Show();                               /**< Enables rendering of this object. */
    void      Hide();                               /**< Disables rendering of this object. */
    TPureBool isColliding_TO_BE_REMOVED() const;              /**< Gets whether colliding is enabled. */
    void      SetColliding_TO_BE_REMOVED(TPureBool value);    /**< Sets whether colliding is enabled. */

    TPURE_ROTATION_ORDER getRotationOrder() const;      /**< Gets the rotation order. */
    void SetRotationOrder(TPURE_ROTATION_ORDER value);  /**< Sets the rotation order. */

    TPureBool isLit() const;                              /**< Gets the lit state. */
    void      SetLit(TPureBool value);                    /**< Sets the lit state. */
    TPureBool isDoubleSided() const;                      /**< Gets the double sided state. */
    void      SetDoubleSided(TPureBool value);            /**< Sets the double sided state. */
    TPureBool isWireframed() const;                       /**< Gets the wireframed state. */
    void      SetWireframed(TPureBool value);             /**< Sets the wireframed state. */
    TPureBool isWireframedCulled() const;                 /**< Gets the wireframed culling state. */
    void      SetWireframedCulled(TPureBool value);       /**< Sets the wireframed culling state. */
    TPureBool isAffectingZBuffer() const;                 /**< Gets whether we write to the Z-Buffer while rendering. */
    void      SetAffectingZBuffer(TPureBool value);       /**< Sets whether we write to the Z-Buffer while rendering. */
    TPureBool isTestingAgainstZBuffer() const;            /**< Gets whether we test against the Z-Buffer while rendering. */
    void      SetTestingAgainstZBuffer(TPureBool value);  /**< Sets whether we test against the Z-Buffer while rendering. */
    TPureBool isStickedToScreen() const;                  /**< Gets the sticked-to-screen state. */
    void      SetStickedToScreen(TPureBool value);        /**< Sets the sticked-to-screen state. */ 

    TPureBool isOccluder() const;                         /**< Gets whether this object should be considered as an occluder during rendering. */
    void      SetOccluder(TPureBool value);               /**< Sets whether this object should be considered as an occluder during rendering. */
    TPureBool isOccluded() const;                         /**< Gets whether this object was occluded or not based on the last finished occlusion test. */
    TPureBool isOcclusionTested() const;                  /**< Gets whether this object is being tested if it is occluded or not. */
    void      SetOcclusionTested(TPureBool state);        /**< Sets whether this object should be tested if it is occluded or not. */
    const PureObject3D* getBoundingBoxObject() const;     /**< Gets the bounding box object used for occlusion tests. */
    void      ForceFinishOcclusionTest();                 /**< Waits for any pending occlusion test to be finished and reset occluded state. */

    // remember: since Object3D is also a manager, it could also implement property change handler for changes in any of its subobjects made "outside" e.g. in their material!
    /*virtual void HandleManagedPropertyChanged(
                 PureManaged& m);*/

    virtual TPureUInt getUsedSystemMemory() const;        /**< Gets the amount of allocated system memory. */
    virtual TPureUInt getUsedVideoMemory() const;         /**< Gets the amount of allocated video memory. */

    TPureUInt draw(
        const TPURE_RENDER_PASS& renderPass,
        TPureBool bASyncQuery,
        TPureBool bRenderIfQueryPending);                 /**< Draws the object. */

protected:

    // ---------------------------------------------------------------------------
    
    PureObject3D(
        PureMaterialManager& matMgr,
        const TPURE_VERTEX_MODIFYING_HABIT& vmod = PURE_VMOD_STATIC,
        const TPURE_VERTEX_REFERENCING_MODE& vref = PURE_VREF_DIRECT,
        TPureBool bForceUseClientMemory = false);                        /**< Only PureObject3DManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
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
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
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

    TPureBool isInitialized() const;                  /**< Tells whether the manager is correctly initialized or not. */

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
        TPureFloat a, TPureFloat b,
        TPURE_VERTEX_MODIFYING_HABIT vmod = PURE_VMOD_STATIC,
        TPURE_VERTEX_REFERENCING_MODE vref = PURE_VREF_DIRECT,
        TPureBool bForceUseClientMemory = false);                /**< Creates a new plane with the given sizes. */
    
    PureObject3D* createBox(
        TPureFloat a, TPureFloat b, TPureFloat c,
        TPURE_VERTEX_MODIFYING_HABIT vmod = PURE_VMOD_STATIC,
        TPURE_VERTEX_REFERENCING_MODE vref = PURE_VREF_DIRECT,
        TPureBool bForceUseClientMemory = false);                /**< Creates a new box with the given sizes. */
    
    PureObject3D* createCube(
        TPureFloat a,
        TPURE_VERTEX_MODIFYING_HABIT vmod = PURE_VMOD_STATIC,
        TPURE_VERTEX_REFERENCING_MODE vref = PURE_VREF_DIRECT,
        TPureBool bForceUseClientMemory = false);                /**< Creates a new cube with the given sizes. */

    PureObject3D* createFromFile(
        const char* filename,
        TPURE_VERTEX_MODIFYING_HABIT vmod,
        TPURE_VERTEX_REFERENCING_MODE vref,
        TPureBool bForceUseClientMemory = false);                /**< Creates object from the given file. */

    PureObject3D* createFromFile(const char* filename);          /**< Creates object from the given file. */

    PureObject3D* createCloned(PureObject3D& referredobj);       /**< Creates a new object by cloning an already existing object. */

    void UpdateOccluderStates();                                 /**< Iterates over its manageds and updates their occluder states. */
    TPureFloat getOccluderSelectionBias() const;                 /**< Gets the occluder selection bias. */
    void SetOccluderSelectionBias(TPureFloat fBias);             /**< Sets the occluder selection bias. */
    TPureUInt getMaxOccluderCount() const;                       /**< Gets the maximum number of occluders. */
    void SetMaxOccluderCount(TPureUInt nMax);                    /**< Sets the maximum number of occluders. */

    virtual void HandleManagedPropertyChanged(
                 PureManaged& m);                                /**< Should be invoked when a managed's property got changed from a different kind of manager or managed. */

    void         ResetStatistics();                              /**< Resets any kind of statistics collected during its lifetime. */

    TPureUInt    getUsedVideoMemory() const;                     /**< Gets the amount of allocated video memory for all objects owner by this manager. */

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
