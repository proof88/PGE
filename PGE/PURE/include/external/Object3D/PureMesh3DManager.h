#pragma once

/*
    ###################################################################################
    PureMesh3DManager.h
    This file is part of PURE.
    External header.
    PureMesh3DManager and PureMesh3D classes.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "../PureAllHeaders.h"
#include "../PureFiledManager.h"
#include "../Material/PureTextureManager.h"
#include "../Material/PureMaterialManager.h"
#include "../Math/PureVector.h"


/**
    Possible primitive formats.
*/
enum TPure_PRIMITIVE_FORMAT
{
    Pure_PM_TRIANGLES,         /**< Geometry will be built up by triangles. */
    Pure_PM_TRIANGLE_STRIPS,   /**< Geometry will be built up by triangle strips. */
    Pure_PM_TRIANGLE_FANS,     /**< Geometry will be built up by triangle fans. */
    Pure_PM_QUADS,             /**< Geometry will be built up by quadrilaterals. */
    Pure_PM_QUAD_STRIPS,       /**< Geometry will be built up by quadrilateral strips. */
    Pure_PM_POINTS,            /**< Geometry will be built up by points. */
    Pure_PM_LINES,             /**< Geometry will be built up by lines. */
    Pure_PM_LINE_STRIPS        /**< Geometry will be built up by line strips. */
}; // TPure_PRIMITIVE_FORMAT


class PureMesh3DManager;
class PureObject3DManager;
class SampleManagerForDescendantFromVertexTransfer;

/**
    3D Mesh class.
    Mesh objects are 2-level entities:
     - first level (parent) has no geometry, owns submeshes,
     - second level meshes own geometry.
    This is why a Mesh is both a Managed and a Manager too: it is owned by Mesh3DManager but it owns its submeshes.
*/
class PureMesh3D :
    public PureFiledManaged,
    public PureFiledManager
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMesh3D is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureMesh3D();

    CConsole&   getManagedConsole() const;            /**< Returns access to console preset with logger module name as this class. */

    virtual TPurebool isLevel1() const;   /**< Tells if the mesh is a level-1 parent mesh. */
    virtual TPurebool isLevel2() const;   /**< Tells if the mesh is a level-2 submesh. */
    
    virtual TPure_PRIMITIVE_FORMAT getPrimitiveFormat() const;   /**< Gets the primitives' format. */

    virtual TPureuint   getVerticesCount() const;                                              /**< Gets the number of total vertices. */
    virtual const TXYZ* getVertices(TPurebool implicitAccessSubobject = true) const;           /**< Gets the pointer to vertices. */
    virtual       TXYZ* getVertices(TPurebool implicitAccessSubobject = true);                 /**< Gets the pointer to vertices. */

    virtual  TPureuint   getVertexIndicesCount() const;                                         /**< Gets the number of total vertex indices. */
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

          PureVector& getPosVec();                  /**< Gets the position. */
    const PureVector& getPosVec() const;            /**< Gets the position. */
    virtual const PureVector& getRelPosVec() const; /**< Gets the mesh-local relative position. */
    virtual const PureVector& getSizeVec() const;   /**< Gets the base sizes. */
    void              RecalculateSize();            /**< Recalculates the sizes. */

    const PureMaterial& getMaterial(TPurebool implicitAccessSubobject = true) const;   /**< Gets the material. */
          PureMaterial& getMaterial(TPurebool implicitAccessSubobject = true);         /**< Gets the material. */

    virtual TPureuint getUsedSystemMemory() const;    /**< Gets the amount of allocated system memory. */

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    PureMesh3D(TPure_PRIMITIVE_FORMAT prfmt = Pure_PM_TRIANGLES);         /**< Only PureMesh3DManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
    PureMesh3D(const PureMesh3D&);
    PureMesh3D& operator=(const PureMesh3D&);

private:

    class PureMesh3DImpl;
    PureMesh3DImpl* pImpl;

    friend class PureMesh3DManager; 
    friend class PureObject3DManager;
    friend class SampleManagerForDescendantFromVertexTransfer;

}; // class PureMesh3D


/**
    3D mesh manager class.
*/
class PureMesh3DManager :
    public PureFiledManager
{
#ifdef Pure_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMesh3DManager is included")
#endif

public:
    static const char* getLoggerModuleName();         /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureMesh3DManager(PureMaterialManager& matMgr);   /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureMesh3DManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPurebool isInitialized() const;                  /**< Tells whether the object is correctly initialized or not. */

    TPurebool isMinimalIndexStorageEnabled() const;              /**< Tells whether storage of indices is minimalized or not. */
    void      SetMinimalIndexStorageEnabled(TPurebool state);    /**< Sets whether storage of indices is minimalized or not. */
    
    PureMesh3D* createPlane(
        TPurefloat a, TPurefloat b);                             /**< Creates a new plane with the given sizes. */
    
    PureMesh3D* createBox(
        TPurefloat a, TPurefloat b, TPurefloat c);               /**< Creates a new box with the given sizes. */
    
    PureMesh3D* createCube(TPurefloat a);                        /**< Creates a new cube with the given sizes. */

    PureMesh3D* createFromFile(const char* filename);            /**< Creates object from the given file. */

    virtual void WriteList() const;                              /**< From PureFiledManager, adding logging mesh data. */

protected:

    PureMesh3DManager();
    PureMesh3DManager(const PureMesh3DManager&);
    PureMesh3DManager& operator=(const PureMesh3DManager&);

    virtual void WriteListCallback(const PureManaged& mngd) const;

private:
    class PureMesh3DManagerImpl;
    PureMesh3DManagerImpl* pImpl;  

    friend class PureObject3DManager;
    friend class SampleManagerForDescendantFromVertexTransfer;

}; // class PureMesh3DManager