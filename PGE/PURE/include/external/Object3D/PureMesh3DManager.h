#pragma once

/*
    ###################################################################################
    PureMesh3DManager.h
    This file is part of PURE.
    External header.
    PureMesh3DManager and PureMesh3D classes.
    Made by PR00F88
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
enum TPURE_PRIMITIVE_FORMAT
{
    PURE_PM_TRIANGLES,         /**< Geometry will be built up by triangles. */
    PURE_PM_TRIANGLE_STRIPS,   /**< Geometry will be built up by triangle strips. */
    PURE_PM_TRIANGLE_FANS,     /**< Geometry will be built up by triangle fans. */
    PURE_PM_QUADS,             /**< Geometry will be built up by quadrilaterals. */
    PURE_PM_QUAD_STRIPS,       /**< Geometry will be built up by quadrilateral strips. */
    PURE_PM_POINTS,            /**< Geometry will be built up by points. */
    PURE_PM_LINES,             /**< Geometry will be built up by lines. */
    PURE_PM_LINE_STRIPS        /**< Geometry will be built up by line strips. */
}; // TPURE_PRIMITIVE_FORMAT


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
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMesh3D is included")
#endif

public:
    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureMesh3D();

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

          PureVector& getPosVec();                  /**< Gets the position. */
    const PureVector& getPosVec() const;            /**< Gets the position. */
    virtual const PureVector& getRelPosVec() const; /**< Gets the mesh-local relative position. */
    virtual const PureVector& getSizeVec() const;   /**< Gets the base sizes. */
    void              RecalculateSize();            /**< Recalculates the sizes. */

    const PureMaterial& getMaterial(TPureBool implicitAccessSubobject = true) const;   /**< Gets the material. */
          PureMaterial& getMaterial(TPureBool implicitAccessSubobject = true);         /**< Gets the material. */

    virtual TPureUInt getUsedSystemMemory() const;    /**< Gets the amount of allocated system memory. */

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------
    
    PureMesh3D(TPURE_PRIMITIVE_FORMAT prfmt = PURE_PM_TRIANGLES);         /**< Only PureMesh3DManager creates it. */ /* TODO: mark this as noexcept(false) when using newer compiler! */
    
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
#ifdef PURE_CLASS_IS_INCLUDED_NOTIFICATION
#pragma message("  PureMesh3DManager is included")
#endif

public:
    static const char* getLoggerModuleName();         /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    PureMesh3DManager(PureMaterialManager& matMgr);   /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureMesh3DManager();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPureBool isInitialized() const;                  /**< Tells whether the object is correctly initialized or not. */

    TPureBool isMinimalIndexStorageEnabled() const;              /**< Tells whether storage of indices is minimalized or not. */
    void      SetMinimalIndexStorageEnabled(TPureBool state);    /**< Sets whether storage of indices is minimalized or not. */
    
    PureMesh3D* createPlane(
        TPureFloat a, TPureFloat b);                             /**< Creates a new plane with the given sizes. */
    
    PureMesh3D* createBox(
        TPureFloat a, TPureFloat b, TPureFloat c);               /**< Creates a new box with the given sizes. */
    
    PureMesh3D* createCube(TPureFloat a);                        /**< Creates a new cube with the given sizes. */

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