#pragma once

/*
    ###################################################################################
    PRREMesh3DImpl.h
    This file is part of PRRE.
    Internal header.
    PRREMesh3DImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PRREallHeaders.h"
#include "../../external/Object3D/PRREMesh3DManager.h"
#include "../gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PRREMesh3D::PRREMesh3DImpl
{

public:

    // ---------------------------------------------------------------------------

    virtual ~PRREMesh3DImpl();

    TPRREbool isLevel1() const;
    TPRREbool isLevel2() const;

    TPRRE_PRIMITIVE_FORMAT getPrimitiveFormat() const;     

    TPRREuint   getVerticesCount(TPRREbool implicitAccessSubobject = true) const;      
    const TXYZ* getVertices(TPRREbool implicitAccessSubobject = true) const;  
          TXYZ* getVertices(TPRREbool implicitAccessSubobject = true);

    TPRREuint   getVertexIndicesCount(TPRREbool implicitAccessSubobject = true) const;
    const void* getVertexIndices(TPRREbool implicitAccessSubobject = true) const;

    unsigned int getVertexIndicesType(TPRREbool implicitAccessSubobject = true) const;
    TPRREuint    getMinVertexIndex(TPRREbool implicitAccessSubobject = true) const;
    TPRREuint    getMaxVertexIndex(TPRREbool implicitAccessSubobject = true) const;
    TPRREuint    getVertexIndex(TPRREuint index, TPRREbool implicitAccessSubobject = true) const;
     
    const TXYZ* getNormals(TPRREbool implicitAccessSubobject = true) const;  

    PRREVector&       getPosVec();                  
    const PRREVector& getPosVec() const;
    const PRREVector& getSizeVec() const; 
    void              RecalculateSize();

    const PRREMaterial& getMaterial(TPRREbool implicitAccessSubobject = true) const;
          PRREMaterial& getMaterial(TPRREbool implicitAccessSubobject = true);

    TPRREuint getUsedSystemMemory() const; 

    TPRREbool cannibalize(PRREMesh3D& victim);

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------

    TPRRE_PRIMITIVE_FORMAT primitiveFormat;              /**< Primitives' format. */
    TXYZ*       pVertices;          /**< Pointer to vertices. */
    TXYZ*       pNormals;           /**< Pointer to normals. */
    void*       pVertexIndices;     /**< Pointer to vertex indices. We use these to index into arrays of Material too. */
    TPRREuint   nVertexIndices_h;   /**< Number of vertex indices. */
    TPRREuint   nMinVertexIndex;    /**< Smallest value in the pVertexIndices array. Used by glDrawRangeElementsEXT(). */
    TPRREuint   nMaxVertexIndex;    /**< Biggest value in the pVertexIndices array. Used by glDrawRangeElementsEXT(). */
    GLenum      nIndicesType;       /**< Type of indices stored in pVertexIndices array. Should be as small as possible per object. */
    TPRREuint   nVertices_h;        /**< Number of vertices. */
    TPRREuint   nFaces_h;           /**< Number of faces. */

    PRREVector vPos;                /**< 3D Position. */
    PRREVector vSize;               /**< 3D Size. */
    
    PRREMesh3DImpl(PRREMesh3D* owner, TPRRE_PRIMITIVE_FORMAT prfmt);
    
    PRREMesh3DImpl(const PRREMesh3DImpl&);
    PRREMesh3DImpl& operator=(const PRREMesh3DImpl&);

    TPRREbool setVertexIndex(TPRREuint index, TPRREuint value);  /**< Sets an index value in the vertex index array. */

private:
    PRREMesh3D* _pOwner;            /**< The owner public object who creates this pimpl object. */
    PRREMaterial* pMaterial;        /**< Pointer to material. */

    // ---------------------------------------------------------------------------
   
    friend class PRREMesh3D;
    friend class PRREMesh3DManager;

}; // class PRREMesh3D::PRREMesh3DImpl