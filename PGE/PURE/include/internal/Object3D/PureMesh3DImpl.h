#pragma once

/*
    ###################################################################################
    PureMesh3DImpl.h
    This file is part of PURE.
    Internal header.
    PureMesh3DImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../external/Object3D/PureMesh3DManager.h"
#include "../gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PureMesh3D::PureMesh3DImpl
{

public:

    // ---------------------------------------------------------------------------

    virtual ~PureMesh3DImpl();

    TPureBool isLevel1() const;
    TPureBool isLevel2() const;

    TPURE_PRIMITIVE_FORMAT getPrimitiveFormat() const;     

    TPureUInt   getVerticesCount() const;      
    const TXYZ* getVertices(TPureBool implicitAccessSubobject = true) const;  
          TXYZ* getVertices(TPureBool implicitAccessSubobject = true);

    TPureUInt   getVertexIndicesCount() const;
    const void* getVertexIndices(TPureBool implicitAccessSubobject = true) const;

    unsigned int getVertexIndicesType(TPureBool implicitAccessSubobject = true) const;
    TPureUInt    getMinVertexIndex(TPureBool implicitAccessSubobject = true) const;
    TPureUInt    getMaxVertexIndex(TPureBool implicitAccessSubobject = true) const;
    TPureUInt    getVertexIndex(TPureUInt index, TPureBool implicitAccessSubobject = true) const;
     
    const TXYZ* getNormals(TPureBool implicitAccessSubobject = true) const;  

    TPureUInt getFaceCount() const;
    TPureUInt getTriangleCount() const;

    PureVector&       getPosVec();                  
    const PureVector& getPosVec() const;
    const PureVector& getRelPosVec() const;
    const PureVector& getSizeVec() const; 
    void              RecalculateSize();

    const PureMaterial& getMaterial(TPureBool implicitAccessSubobject = true) const;
          PureMaterial& getMaterial(TPureBool implicitAccessSubobject = true);

    TPureUInt getUsedSystemMemory() const; 

    TPureBool cannibalize(PureMesh3D& victim);

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------

    TPURE_PRIMITIVE_FORMAT primitiveFormat;              /**< Primitives' format. */
    TXYZ*       pVertices;          /**< Pointer to vertices. */
    TXYZ*       pNormals;           /**< Pointer to normals. */
    void*       pVertexIndices;     /**< Pointer to vertex indices. We use these to index into arrays of Material too. */
    TPureUInt   nVertexIndices_h;   /**< Number of vertex indices. */
    TPureUInt   nMinVertexIndex;    /**< Smallest value in the pVertexIndices array. Used by glDrawRangeElementsEXT(). */
    TPureUInt   nMaxVertexIndex;    /**< Biggest value in the pVertexIndices array. Used by glDrawRangeElementsEXT(). */
    GLenum      nIndicesType;       /**< Type of indices stored in pVertexIndices array. Should be as small as possible per object. */
    TPureUInt   nVertices_h;        /**< Number of vertices. */
    TPureUInt   nFaces_h;           /**< Number of faces. */

    PureVector vPos;                /**< 3D World-space Position. */
    PureVector vRelPos;             /**< 3D offset position of vertices relative to center of mesh [0,0,0]. */
    PureVector vSize;               /**< 3D Size. */
    
    PureMesh3DImpl(PureMesh3D* owner, TPURE_PRIMITIVE_FORMAT prfmt);
    
    PureMesh3DImpl(const PureMesh3DImpl&);
    PureMesh3DImpl& operator=(const PureMesh3DImpl&);

    TPureBool setVertexIndex(TPureUInt index, TPureUInt value);  /**< Sets an index value in the vertex index array. */

private:
    PureMesh3D* _pOwner;            /**< The owner public object who creates this pimpl object. */
    PureMaterial* pMaterial;        /**< Pointer to material. */

    // ---------------------------------------------------------------------------
   
    friend class PureMesh3D;
    friend class PureMesh3DManager;

}; // class PureMesh3D::PureMesh3DImpl