#pragma once

/*
    ###################################################################################
    PRREMesh3DManager.h
    This file is part of PRRE.
    External header.
    PRREMesh3DManager and PRREMesh3D classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../PRREallHeaders.h"
#include "../PRREFiledManager.h"
#include "../Material/PRRETextureManager.h"
#include "../Material/PRREMaterialManager.h"
#include "../Math/PRREVector.h"


/**
    Possible primitive formats.
*/
enum TPRRE_PRIMITIVE_FORMAT
{
    PRRE_PM_TRIANGLES,         /**< Geometry will be built up by triangles. */
    PRRE_PM_TRIANGLE_STRIPS,   /**< Geometry will be built up by triangle strips. */
    PRRE_PM_TRIANGLE_FANS,     /**< Geometry will be built up by triangle fans. */
    PRRE_PM_QUADS,             /**< Geometry will be built up by quadrilaterals. */
    PRRE_PM_QUAD_STRIPS,       /**< Geometry will be built up by quadrilateral strips. */
    PRRE_PM_POINTS,            /**< Geometry will be built up by points. */
    PRRE_PM_LINES,             /**< Geometry will be built up by lines. */
    PRRE_PM_LINE_STRIPS        /**< Geometry will be built up by line strips. */
}; // TPRRE_PRIMITIVE_FORMAT


class PRREMesh3DManager;
class PRREObject3DManager;
class SampleManagerForDescendantFromVertexTransfer;

/**
    3D Mesh class.
    Mesh objects are 2-level entities:
     - first level (parent) has no geometry, owns submeshes,
     - second level meshes own geometry.
    This is why a Mesh is both a Managed and a Manager too: it is owned by Mesh3DManager but it owns its submeshes.
*/
class PRREMesh3D :
    public PRREFiledManaged,
    public PRREFiledManager
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREMesh3D is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PRREMesh3D();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

    TPRREbool isLevel1() const;   /**< Tells if the mesh is a level-1 parent mesh. */
    TPRREbool isLevel2() const;   /**< Tells if the mesh is a level-2 submesh. */
    
    TPRRE_PRIMITIVE_FORMAT getPrimitiveFormat() const;   /**< Gets the primitives' format. */

    TPRREuint   getVerticesCount(TPRREbool implicitAccessSubobject = true) const;      /**< Gets the number of vertices. */
    const TXYZ* getVertices(TPRREbool implicitAccessSubobject = true) const;           /**< Gets the pointer to vertices. */
          TXYZ* getVertices(TPRREbool implicitAccessSubobject = true);                 /**< Gets the pointer to vertices. */

    TPRREuint   getVertexIndicesCount(TPRREbool implicitAccessSubobject = true) const; /**< Gets the number of vertex indices. */
    const void* getVertexIndices(TPRREbool implicitAccessSubobject = true) const;      /**< Gets the pointer to vertex indices. */
    /* HACK: using unsigned int here instead of GLenum to avoid using GL header. */
    /* TODO: create own type for the index type. */
    unsigned int getVertexIndicesType(TPRREbool implicitAccessSubobject = true) const; /**< Gets the type of the indices. */
    TPRREuint    getMinVertexIndex(TPRREbool implicitAccessSubobject = true) const;    /**< Gets the smallest index in the vertex indices array. */
    TPRREuint    getMaxVertexIndex(TPRREbool implicitAccessSubobject = true) const;    /**< Gets the greatest index in the vertex indices array. */
    TPRREuint    getVertexIndex(
        TPRREuint index, TPRREbool implicitAccessSubobject = true) const;              /**< Gets an index value from the vertex indices array. */

    const TXYZ* getNormals(TPRREbool implicitAccessSubobject = true) const;            /**< Gets the pointer to normals. */

          PRREVector& getPosVec();                  /**< Gets the position. */
    const PRREVector& getPosVec() const;            /**< Gets the position. */
    virtual const PRREVector& getSizeVec() const;   /**< Gets the base sizes. */
    void              RecalculateSize();            /**< Recalculates the sizes. */

    const PRREMaterial& getMaterial(TPRREbool implicitAccessSubobject = true) const;   /**< Gets the material. */
          PRREMaterial& getMaterial(TPRREbool implicitAccessSubobject = true);         /**< Gets the material. */

    virtual TPRREuint getUsedSystemMemory() const;    /**< Gets the amount of allocated system memory. */

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    PRREMesh3D(TPRRE_PRIMITIVE_FORMAT prfmt = PRRE_PM_TRIANGLES);         /**< Only PRREMesh3DManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PRREMesh3D(const PRREMesh3D&);
    PRREMesh3D& operator=(const PRREMesh3D&);

private:

    class PRREMesh3DImpl;
    PRREMesh3DImpl* pImpl;

    friend class PRREMesh3DManager; 
    friend class PRREObject3DManager;
    friend class SampleManagerForDescendantFromVertexTransfer;

}; // class PRREMesh3D


/**
    3D mesh manager class.
*/
class PRREMesh3DManager :
    public PRREFiledManager
{
#ifdef PRRE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PRREMesh3DManager is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PRREMesh3DManager(PRREMaterialManager& matMgr); /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PRREMesh3DManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPRREbool isInitialized() const;  /**< Tells whether the object is correctly initialized or not. */

    TPRREbool isMinimalIndexStorageEnabled() const;              /**< Tells whether storage of indices is minimalized or not. */
    void      SetMinimalIndexStorageEnabled(TPRREbool state);    /**< Sets whether storage of indices is minimalized or not. */
    
    PRREMesh3D* createPlane(
        TPRREfloat a, TPRREfloat b);                             /**< Creates a new plane with the given sizes. */
    
    PRREMesh3D* createBox(
        TPRREfloat a, TPRREfloat b, TPRREfloat c);               /**< Creates a new box with the given sizes. */
    
    PRREMesh3D* createCube(TPRREfloat a);                        /**< Creates a new cube with the given sizes. */

    PRREMesh3D* createFromFile(const char* filename);            /**< Creates object from the given file. */

    virtual void WriteList() const;

protected:

    PRREMesh3DManager();
    PRREMesh3DManager(const PRREMesh3DManager&);
    PRREMesh3DManager& operator=(const PRREMesh3DManager&);

    virtual void WriteListCallback(const PRREManaged& mngd) const;

private:
    class PRREMesh3DManagerImpl;
    PRREMesh3DManagerImpl* pImpl;  

    friend class PRREObject3DManager;
    friend class SampleManagerForDescendantFromVertexTransfer;

}; // class PRREMesh3DManager