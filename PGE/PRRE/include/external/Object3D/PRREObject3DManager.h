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
    virtual ~PRREObject3D();

    virtual TPRRE_VERTEX_MODIFYING_HABIT getVertexModifyingHabit() const;            /**< Gets vertex modifying habit. */
    virtual TPRREbool setVertexModifyingHabit(TPRRE_VERTEX_MODIFYING_HABIT vmod);    /**< Sets vertex modifying habit. */
    virtual TPRRE_VERTEX_REFERENCING_MODE getVertexReferencingMode() const;          /**< Gets vertex referencing mode. */
    virtual TPRREbool setVertexReferencingMode(TPRRE_VERTEX_REFERENCING_MODE vref);  /**< Sets vertex referencing mode. */
    virtual TPRRE_VERTEX_TRANSFER_MODE getVertexTransferMode() const;                /**< Gets vertex transfer mode. */
    virtual TPRREbool setVertexTransferMode(TPRRE_VERTEX_TRANSFER_MODE vtrans);      /**< Sets vertex transfer mode. */

    PRREObject3D* getReferredObject() const;   /**< Gets the original object which was cloned to create this object. */

    const TPRRE_TRANSFORMED_VERTEX* getTransformedVertices(
        TPRREbool implicitAccessSubobject = true) const;               /**< Gets the pointer to transformed vertices. */

    TPRRE_TRANSFORMED_VERTEX* getTransformedVertices(
        TPRREbool implicitAccessSubobject = true);                     /**< Gets the pointer to transformed vertices. */

          PRREVector& getAngleVec();                /**< Gets the rotation angles. */
    const PRREVector& getAngleVec() const;          /**< Gets the rotation angles. */

    virtual const PRREVector& getSizeVec() const;   /**< Gets the base sizes. */

          PRREVector  getScaledSizeVec() const;     /**< Gets the real sizes considering the geometry size calculated from vertex data and the current scaling factor. */

    const PRREVector& getScaling() const;           /**< Gets the scaling factor. */

    void  SetScaling(TPRREfloat value);             /**< Sets the scaling factor to given scalar. */
    void  SetScaling(const PRREVector& value);      /**< Sets the scaling factor to given vector. */

    void  Scale(TPRREfloat value);                  /**< Scales by the given scalar value. */
    void  Scale(const PRREVector& value);           /**< Scales by the given vector. */

    TPRREbool isVisible() const;                    /**< Gets the visibility state. */
    void      SetVisible(TPRREbool state);          /**< Sets the visibility state. */
    void      Show();                               /**< Sets the visibility state to true. */
    void      Hide();                               /**< Sets the visibility state to false. */
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

    virtual TPRREuint getUsedSystemMemory() const;    /**< Gets the amount of allocated system memory. */
    virtual TPRREuint getUsedVideoMemory() const;     /**< Gets the amount of allocated video memory. */

    void Draw(bool bLighting);          /**< Draws the object. */

protected:

    // ---------------------------------------------------------------------------
    
    PRREObject3D(
        PRREMaterialManager& matMgr,
        const TPRRE_VERTEX_MODIFYING_HABIT& vmod = PRRE_VMOD_STATIC,
        const TPRRE_VERTEX_REFERENCING_MODE& vref = PRRE_VREF_DIRECT,
        TPRREbool bForceUseClientMemory = false);                        /**< Only PRREObject3DManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PRREObject3D(const PRREObject3D&);                                   
    PRREObject3D& operator=(const PRREObject3D&);

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
    static TPRREbool isBlendFuncBlends(
        TPRRE_BLENDFACTORS sfactor,     
        TPRRE_BLENDFACTORS dfactor);    /**< Gets whether the given source and destination factors really mean blending or not. */

    static TPRREbool isBFB(
        TPRRE_BLENDFACTORS sfactor,                 
        TPRRE_BLENDFACTORS dfactor);    /**< Same as isBlendFuncBlends(). */

    // ---------------------------------------------------------------------------

    PRREObject3DManager(PRRETextureManager& texMgr, PRREMaterialManager& matMgr); /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PRREObject3DManager();

    TPRREbool isInitialized() const;  /**< Tells whether the object is correctly initialized or not. */
    
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
