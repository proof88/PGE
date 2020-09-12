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

#include "../PRREallHeaders.h"
#include "PRREMesh3DManager.h"
#include "../gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PRREMesh3D::PRREMesh3DImpl
{

public:

    // ---------------------------------------------------------------------------

    virtual ~PRREMesh3DImpl();

    TPRRE_PRIMITIVE_FORMAT getPrimitiveFormat() const;     

    TPRREuint   getVerticesCount(TPRREbool implicitAccessSubobject = true) const;      
    const TXYZ* getVertices(TPRREbool implicitAccessSubobject = true) const;  
          TXYZ* getVertices(TPRREbool implicitAccessSubobject = true);

    TPRREuint   getVertexIndicesCount(TPRREbool implicitAccessSubobject = true) const;
    const void* getVertexIndices(TPRREbool implicitAccessSubobject = true) const;

    unsigned int getVertexIndicesType(TPRREbool implicitAccessSubobject = true) const;
    TPRREuint    getMinIndexValue(TPRREbool implicitAccessSubobject = true) const;
    TPRREuint    getMaxIndexValue(TPRREbool implicitAccessSubobject = true) const;
    TPRREuint    getIndexFromArray(const void* arr, TPRREuint index) const;

    TPRREuint   getNormalsCount(TPRREbool implicitAccessSubobject = true) const;       
    const TXYZ* getNormals(TPRREbool implicitAccessSubobject = true) const;  

    PRREVector&       getPosVec();                  
    const PRREVector& getPosVec() const;
    const PRREVector& getSizeVec() const; 
    void              RecalculateSize();

    const PRREMaterial& getMaterial(TPRREbool implicitAccessSubobject = true) const;
          PRREMaterial& getMaterial(TPRREbool implicitAccessSubobject = true);

    TPRREuint getUsedSystemMemory() const;                 

    // ---------------------------------------------------------------------------

protected:

    // ---------------------------------------------------------------------------

    TPRRE_PRIMITIVE_FORMAT primitiveFormat;              /**< Primitives' format. */
    TXYZ*       pVertices;          /**< Pointer to vertices. */
    TXYZ*       pNormals;           /**< Pointer to normals. */
    void*       pVertexIndices;     /**< Pointer to vertex indices. We use these to index into arrays of Material too. */
    TPRREuint   nVertexIndices_h;   /**< Number of vertex indices. */
    TPRREuint   nMinIndex;          /**< Smallest value in the pVertexIndices array. Used by glDrawRangeElementsEXT(). */
    TPRREuint   nMaxIndex;          /**< Biggest value in the pVertexIndices array. Used by glDrawRangeElementsEXT(). */
    GLenum      nIndicesType;       /**< Type of indices stored in pVertexIndices array. Should be as small as possible per object. */
    TPRREuint   nVertices_h;        /**< Number of vertices. */
    TPRREuint   nNormals_h;         /**< Number of normals. TODO: maybe can be removed as nVertices_h is enough. */
    TPRREuint   nFaces_h;           /**< Number of faces. */

    PRREVector vPos;                /**< 3D Position. */
    PRREVector vSize;               /**< 3D Size. */
    
    PRREMesh3DImpl(PRREMesh3D* owner, TPRRE_PRIMITIVE_FORMAT prfmt);
    
    PRREMesh3DImpl(const PRREMesh3DImpl&);
    PRREMesh3DImpl& operator=(const PRREMesh3DImpl&);

    void      SetIndexInArray(void* arr, TPRREuint index, TPRREuint value);  /**< Sets an index value in a given index array. */

private:
    PRREMesh3D* _pOwner;            /**< The owner public object who creates this pimpl object. */
    PRREMaterial* pMaterial;        /**< Pointer to material. */

    // ---------------------------------------------------------------------------

    CConsole& getConsole() const;               /**< Hack to be able to use CConsole singleton instance instead of owner's protected console instance. */
   
    friend class PRREMesh3D;
    friend class PRREMesh3DManager;

}; 