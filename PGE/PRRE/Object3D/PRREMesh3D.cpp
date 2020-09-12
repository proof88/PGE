/*
    ###################################################################################
    PRREMesh3D.cpp
    This file is part of PRRE.
    PRREMesh3D class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../PRREpragmas.h"
#include "../../../../PFL/PFL/PFL.h"
#include "../PRREGLsnippets.h"
#include "PRREMesh3DManager.h"
#include "PRREMesh3DImpl.h"

using namespace std;


/*
   PRREMesh3D::PRREMesh3DImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREMesh3D::PRREMesh3DImpl::~PRREMesh3DImpl()
{
    getConsole().OLnOI("~PRREMesh3D() ...");

    free(pVertices);
    free(pNormals);
    free(pVertexIndices);
    pVertices = PGENULL;
    pNormals = PGENULL;
    pVertexIndices = PGENULL;

    delete pMaterial;
    pMaterial = PGENULL;

    _pOwner->DeleteAll();

    getConsole().SOLnOO("Done!");
} // ~PRRETexture()


TPRRE_PRIMITIVE_FORMAT PRREMesh3D::PRREMesh3DImpl::getPrimitiveFormat() const
{
    return primitiveFormat;
} // getPrimitiveFormat()


TPRREuint PRREMesh3D::PRREMesh3DImpl::getVerticesCount(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getVerticesCount();
    else
        return nVertices_h;
} // getVerticesCount()


const TXYZ* PRREMesh3D::PRREMesh3DImpl::getVertices(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getVertices();
    else
        return pVertices;
} // getVertices()


TXYZ* PRREMesh3D::PRREMesh3DImpl::getVertices(TPRREbool implicitAccessSubobject)
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getVertices();
    else
        return pVertices;
} // getVertices()


/**
    Gets the number of vertex indices.
*/
TPRREuint PRREMesh3D::PRREMesh3DImpl::getVertexIndicesCount(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getVertexIndicesCount();
    else
        return nVertexIndices_h; 
}


/**
    Gets the pointer to vertex indices.
*/
const void* PRREMesh3D::PRREMesh3DImpl::getVertexIndices(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getVertexIndices();
    else
        return pVertexIndices;
}


/**
    Gets the type of the indices.
*/
unsigned int PRREMesh3D::PRREMesh3DImpl::getVertexIndicesType(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getVertexIndicesType();
    else
        return nIndicesType;
}


/**
    Gets the smallest index in the vertex indices array.
*/
TPRREuint PRREMesh3D::PRREMesh3DImpl::getMinIndexValue(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getMinIndexValue();
    else
        return nMinIndex;
}

/**
    Gets the greatest index in the vertex indices array.
*/
TPRREuint PRREMesh3D::PRREMesh3DImpl::getMaxIndexValue(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getMaxIndexValue();
    else
        return nMaxIndex;
}


/**
    Gets an index value from a given index array.
*/
TPRREuint PRREMesh3D::PRREMesh3DImpl::getIndexFromArray(const void* arr, TPRREuint index) const
{
    return PRREGLsnippets::getIndexFromArray(arr, index, this->nIndicesType);
}


TPRREuint PRREMesh3D::PRREMesh3DImpl::getNormalsCount(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getNormalsCount();
    else
        return nNormals_h;
} // getNormalsCount()


const TXYZ* PRREMesh3D::PRREMesh3DImpl::getNormals(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getNormals();
    else
        return pNormals;
} // getNormals()


PRREVector& PRREMesh3D::PRREMesh3DImpl::getPosVec()
{
    return vPos;
} // getPosVec()


const PRREVector& PRREMesh3D::PRREMesh3DImpl::getPosVec() const
{
    return vPos;
} // getPosVec()


const PRREVector& PRREMesh3D::PRREMesh3DImpl::getSizeVec() const
{
    return vSize;
} // getSizeVec()


void PRREMesh3D::PRREMesh3DImpl::RecalculateSize()
{
    if ( _pOwner->getCount() > 0 )
    {
        // we are parent, ask our submeshes to determine their sizes and object-local positions
        for (TPRREint i = 0; i < _pOwner->getCount(); i++)
            ((PRREMesh3D*)_pOwner->getAttachedAt(i))->RecalculateSize();

        // now we can determine our own size as parent based on min and max subobject positions and size
        TXYZ minvals, maxvals;
        minvals.x = ((PRREMesh3D*)_pOwner->getAttachedAt(0))->getPosVec().getX() - ((PRREMesh3D*)_pOwner->getAttachedAt(0))->getSizeVec().getX() / 2.0f;
        minvals.y = ((PRREMesh3D*)_pOwner->getAttachedAt(0))->getPosVec().getY() - ((PRREMesh3D*)_pOwner->getAttachedAt(0))->getSizeVec().getY() / 2.0f;
        minvals.z = ((PRREMesh3D*)_pOwner->getAttachedAt(0))->getPosVec().getZ() - ((PRREMesh3D*)_pOwner->getAttachedAt(0))->getSizeVec().getZ() / 2.0f;
        maxvals.x = -minvals.x;
        maxvals.y = -minvals.y;
        maxvals.z = -minvals.z;

        for (TPRREint i = 1; i < _pOwner->getCount(); i++)
        {
            if ( minvals.x > ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getX() - ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getX() / 2.0f )
                minvals.x = ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getX() - ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getX() / 2.0f;
            if ( maxvals.x < ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getX() + ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getX() / 2.0f )
                maxvals.x = ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getX() + ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getX() / 2.0f;
            if ( minvals.y > ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getY() - ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getY() / 2.0f )
                minvals.y = ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getY() - ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getY() / 2.0f;
            if ( maxvals.y < ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getY() + ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getY() / 2.0f )
                maxvals.y = ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getY() + ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getY() / 2.0f;
            if ( minvals.z > ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getZ() - ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getZ() / 2.0f )
                minvals.z = ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getZ() - ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getZ() / 2.0f;
            if ( maxvals.z < ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getZ() + ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getZ() / 2.0f )
                maxvals.z = ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getPosVec().getZ() + ((PRREMesh3D*)_pOwner->getAttachedAt(i))->getSizeVec().getZ() / 2.0f;
        }

        vSize.SetX( abs( minvals.x - maxvals.x ) );
        vSize.SetY( abs( minvals.y - maxvals.y ) );
        vSize.SetZ( abs( minvals.z - maxvals.z ) );

        // since we are parent we dont need to modify our world-space position at all based on our subobjects
    }

    if ( (nVertices_h == 0) || (pVertices == PGENULL ) )
        return;

    // we are subobject, so we need to calculate our own size and object-local position based on min and max vertex positions

    TXYZ minvals, maxvals;
    minvals.x = pVertices[0].x;
    minvals.y = pVertices[0].y;
    minvals.z = pVertices[0].z;
    maxvals.x = minvals.x;
    maxvals.y = minvals.y;
    maxvals.z = minvals.z;

    for (TPRREuint i = 1; i < nVertices_h; i++)
    {
        if ( minvals.x > pVertices[i].x ) minvals.x = pVertices[i].x;
        if ( maxvals.x < pVertices[i].x ) maxvals.x = pVertices[i].x;
        if ( minvals.y > pVertices[i].y ) minvals.y = pVertices[i].y;
        if ( maxvals.y < pVertices[i].y ) maxvals.y = pVertices[i].y;
        if ( minvals.z > pVertices[i].z ) minvals.z = pVertices[i].z;
        if ( maxvals.z < pVertices[i].z ) maxvals.z = pVertices[i].z;
    } 

    vSize.SetX( abs( minvals.x - maxvals.x ) );
    vSize.SetY( abs( minvals.y - maxvals.y ) );
    vSize.SetZ( abs( minvals.z - maxvals.z ) );

    vPos.SetX( minvals.x + vSize.getX() / 2.0f );
    vPos.SetY( minvals.y + vSize.getY() / 2.0f );
    vPos.SetZ( minvals.z + vSize.getZ() / 2.0f );
    
} // RecalculateSize()


const PRREMaterial& PRREMesh3D::PRREMesh3DImpl::getMaterial(TPRREbool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getMaterial();
    else
        return *pMaterial;
} // getMaterial()


PRREMaterial& PRREMesh3D::PRREMesh3DImpl::getMaterial(TPRREbool implicitAccessSubobject)
{
    if ( implicitAccessSubobject && (nVertices_h == 0) && (_pOwner->getCount() == 1) )
        return ((PRREMesh3D*) (_pOwner->getAttachedAt(0)))->getMaterial();
    else
        return *pMaterial;
} // getMaterial()


TPRREuint PRREMesh3D::PRREMesh3DImpl::getUsedSystemMemory() const
{
    return sizeof(*this) + (pMaterial ? pMaterial->getUsedSystemMemory() : 0);
} // getUsedSystemMemory() 


// ############################## PROTECTED ##############################


/**
    @param owner The public PRREMesh3D class instance owning this pimpl object.
*/
PRREMesh3D::PRREMesh3DImpl::PRREMesh3DImpl(PRREMesh3D* owner, TPRRE_PRIMITIVE_FORMAT prfmt)
{
    _pOwner = owner;
    getConsole().OLnOI("PRREMesh3D() ...");

    primitiveFormat = prfmt;
    pVertices = PGENULL;
    pNormals = PGENULL;
    pVertexIndices = PGENULL;

    nMinIndex = UINT_MAX;  // todo: should make macros like PRRE_UINT_MAX into prretypes.h
    nMaxIndex = 0;
    nIndicesType = GL_UNSIGNED_BYTE;

    nVertices_h = nNormals_h = 0;
    nVertexIndices_h = 0;
    nFaces_h = 0;

    pMaterial = PGENULL;

    getConsole().SOLnOO("Done!");
} // PRREMesh3D()


PRREMesh3D::PRREMesh3DImpl::PRREMesh3DImpl(const PRREMesh3DImpl& mimpl)
{
    /*
    // _pOwner must not be set here, owner will overwrite anyway
    nMinIndex = mimpl.nMinIndex;
    nMaxIndex = mimpl.nMaxIndex;
    nIndicesType = mimpl.nIndicesType;

    nVertices_h = mimpl.nVertices_h;
    nNormals_h = mimpl.nNormals_h;
    nVertexIndices_h = mimpl.nVertexIndices_h;
    nFaces_h = mimpl.nFaces_h;

    primitiveFormat = mimpl.primitiveFormat;

    if ( nVertices_h > 0 )
    {
        pVertices = (TXYZ*) malloc( sizeof(TXYZ) * nVertices_h );
        memcpy(pVertices, mimpl.pVertices, sizeof(TXYZ) * nVertices_h);
    }
    else
        pVertices = PGENULL;

    if ( nNormals_h > 0 )
    {
        pNormals  = (TXYZ*) malloc( sizeof(TXYZ) *  nNormals_h );
        memcpy(pNormals, mimpl.pNormals, sizeof(TXYZ) * nNormals_h);
    }
    else
        pNormals = PGENULL;

    if ( nVertexIndices_h > 0 )
    {
        pVertexIndices = malloc( PRREGLsnippets::getSizeofIndexType(nIndicesType) * nVertexIndices_h );
        memcpy(pVertexIndices, mimpl.pVertexIndices, sizeof(TXYZ) * nVertexIndices_h);
    }
    else
        pVertexIndices = PGENULL;

    pMaterial = PGENULL; // should be copied
    */
}


PRREMesh3D::PRREMesh3DImpl& PRREMesh3D::PRREMesh3DImpl::operator=(const PRREMesh3DImpl&)
{
    return *this;
}


/**
    Sets an index value in a given index array.
*/
void PRREMesh3D::PRREMesh3DImpl::SetIndexInArray(void* arr, TPRREuint index, TPRREuint value)
{
    PRREGLsnippets::SetIndexInArray(arr, index, value, this->nIndicesType);
}


// ############################### PRIVATE ###############################


/**
    Hack to be able to use CConsole singleton instance instead of owner's protected console instance.
    TODO: maybe the whole getConsole() stuff should be removed from everywhere because CConsole is a singleton class after all.
          Or maybe not, maybe this is pretty useful for per-manager filtering, because this way we can pass a manager handle to
          the static getConsoleInstance() function that returns a manager-specific instance!
*/
CConsole& PRREMesh3D::PRREMesh3DImpl::getConsole() const
{
    return CConsole::getConsoleInstance();
}


/*
   PRREMesh3D
   ###########################################################################
*/


// ############################### PUBLIC ################################


PRREMesh3D::~PRREMesh3D()
{
    delete p;
    p = NULL;
} // ~PRRETexture()


/**
    Gets the primitives' format.
    Primitive format is the same for the mesh and its submeshes.
*/
TPRRE_PRIMITIVE_FORMAT PRREMesh3D::getPrimitiveFormat() const
{
    return p->getPrimitiveFormat();
} // getPrimitiveFormat()


/**
    Gets the number of vertices.
    Number of vertices is 0 for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    The total number of vertices within a mesh can be calculated by summing the number of vertices of its submeshes.
    Note: in special case this returns positive value even for a level-1 mesh, see below.

    @return Number of vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            count is the subobject's vertex count. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRREuint PRREMesh3D::getVerticesCount(TPRREbool implicitAccessSubobject) const
{
    return p->getVerticesCount(implicitAccessSubobject);
} // getVerticesCount()


/**
    Gets the pointer to vertices.
    Pointer to vertices is NULL for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    Note: in special case this returns non-NULL value even for a level-1 mesh, see below.

    @return Pointer to vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const TXYZ* PRREMesh3D::getVertices(TPRREbool implicitAccessSubobject) const
{
    return p->getVertices(implicitAccessSubobject);
} // getVertices()


/**
    Gets the pointer to vertices.
    Pointer to vertices is NULL for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    Note: in special case this returns non-NULL value even for a level-1 mesh, see below.

    @return Pointer to vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TXYZ* PRREMesh3D::getVertices(TPRREbool implicitAccessSubobject)
{
    return p->getVertices(implicitAccessSubobject);
} // getVertices()


/**
    Gets the number of vertex indices.
    Number of vertex incides is 0 for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    The total number of vertex indices within a mesh can be calculated by summing the number of vertex indices of its submeshes.
    Note: in special case this returns positive value even for a level-1 mesh, see below.

    @return Number of vertex indices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            count is the subobject's vertex index count. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRREuint PRREMesh3D::getVertexIndicesCount(TPRREbool implicitAccessSubobject) const
{
    return p->getVertexIndicesCount(implicitAccessSubobject);
}


/**
    Gets the pointer to vertex indices.
    Pointer to vertex indices is NULL for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    Note: in special case this returns non-NULL value even for a level-1 mesh, see below.

    @return Pointer to vertex indices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's vertex indices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const void* PRREMesh3D::getVertexIndices(TPRREbool implicitAccessSubobject) const
{
    return p->getVertexIndices(implicitAccessSubobject);
}


/**
    Gets the type of the indices.
    Index type is irrelevant for a level-1 mesh since the geometry is owned by its level-2 submeshes and it is unique
    for each submesh.

    @return Vertex index type. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            type is the subobject's vertex index type. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
unsigned int PRREMesh3D::getVertexIndicesType(TPRREbool implicitAccessSubobject) const
{
    return p->getVertexIndicesType(implicitAccessSubobject);
}


/**
    Gets the smallest index in the vertex indices array.
    This value is irrelevant for a level-1 mesh since the geometry is owned by its level-2 submeshes and it is unique
    for each submesh. Still the returned value for a level-1 mesh can be a positive value, see below.

    @return The smallest index in the vertex index array. If the object's own vertex count is 0 but it has exactly 1 subobject,
            the returned index is the subobject's smallest index. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRREuint PRREMesh3D::getMinIndexValue(TPRREbool implicitAccessSubobject) const
{
    return p->getMinIndexValue(implicitAccessSubobject);
}

/**
    Gets the greatest index in the vertex indices array.
    This value is irrelevant for a level-1 mesh since the geometry is owned by its level-2 submeshes and it is unique
    for each submesh. Still the returned value for a level-1 mesh can be a positive value, see below.

    @return The greatest index in the vertex index array. If the object's own vertex count is 0 but it has exactly 1 subobject,
            the returned index is the subobject's greatest index. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRREuint PRREMesh3D::getMaxIndexValue(TPRREbool implicitAccessSubobject) const
{
    return p->getMaxIndexValue(implicitAccessSubobject);
}

/**
    Gets an index value from a given index array.
    This function is highly recommended to be used for reading indices from the index array since the type of indices can vary
    from subobject to subobject and this function considers the index type.
    This function is irrelevant for a level-1 mesh since the geometry is owned by its level-2 submeshes. Still the returned value
    for a level-1 mesh can be a valid value, see below.

    @return An index read from the given array. If the object's own vertex count is 0 but it has exactly 1 subobject,
            the returned index is the subobject's index. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRREuint PRREMesh3D::getIndexFromArray(const void* arr, TPRREuint index) const
{
    return p->getIndexFromArray(arr, index);
}


/**
    Gets the number of normals.
    Number of normals is 0 for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    The total number of normals within a mesh can be calculated by summing the number of normals of its submeshes.
    Note: in special case this returns positive value even for a level-1 mesh, see below.

    @return Number of normals. If the object's own normal count is 0 but it has exactly 1 subobject, the returned
            count is the subobject's normal count. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPRREuint PRREMesh3D::getNormalsCount(TPRREbool implicitAccessSubobject) const
{
    return p->getNormalsCount(implicitAccessSubobject);
} // getNormalsCount()


/**
    Gets the pointer to normals.
    Pointer to normals is NULL for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    Note: in special case this returns non-NULL value even for a level-1 mesh, see below.

    @return Pointer to normals. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's normals pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const TXYZ* PRREMesh3D::getNormals(TPRREbool implicitAccessSubobject) const
{
    return p->getNormals(implicitAccessSubobject);
} // getNormals()


/**
    Gets the position.
    For a level-1 mesh object, this vector is always zero. It can be utilized by a descendant class though.
    For a level-2 (sub)mesh object, this vector represents the position of the submesh relative to the position of its parent mesh object.
    @return Position vector.
*/
PRREVector& PRREMesh3D::getPosVec()
{
    return p->getPosVec();
} // getPosVec()


/**
    Gets the position.
    For a level-1 mesh object, this vector is always zero. It can be utilized by a descendant class though.
    For a level-2 (sub)mesh object, this vector represents the position of the submesh relative to the position of its parent mesh object.
    @return Position vector.
*/
const PRREVector& PRREMesh3D::getPosVec() const
{
    return p->getPosVec();
} // getPosVec()


/**
    Gets the base sizes.
    @return Base sizes vector.
*/
const PRREVector& PRREMesh3D::getSizeVec() const
{
    return p->getSizeVec();
} // getSizeVec()


/**
    Recalculates the sizes.
*/
void PRREMesh3D::RecalculateSize()
{
    p->RecalculateSize();
} // RecalculateSize()


/**
    Gets the material.

    @return Material. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned material is the subobject's material.
            This implicit behavior is for convenience for objects storing only 1 subobject like internally created objects.
*/
const PRREMaterial& PRREMesh3D::getMaterial(TPRREbool implicitAccessSubobject) const
{
    return p->getMaterial(implicitAccessSubobject);
} // getMaterial()


/**
    Gets the material.
    @return Material. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned material is the subobject's material.
            This implicit behavior is for convenience for objects storing only 1 subobject like internally created objects.
*/
PRREMaterial& PRREMesh3D::getMaterial(TPRREbool implicitAccessSubobject)
{
    return p->getMaterial(implicitAccessSubobject);
} // getMaterial()


/**
    Gets the amount of allocated system memory.
    It includes the allocated Material size as well (getMaterial()).

    @return Amount if allocated system memory in Bytes.
*/
TPRREuint PRREMesh3D::getUsedSystemMemory() const
{
    return PRREFiledManaged::getUsedSystemMemory() - sizeof(PRREFiledManaged) + sizeof(*this) + p->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


/**
    Only PRREObject3DManager creates it.
*/
PRREMesh3D::PRREMesh3D(TPRRE_PRIMITIVE_FORMAT prfmt)
{
    p = new PRREMesh3DImpl(this, prfmt);
} // PRREMesh3D()


PRREMesh3D::PRREMesh3D(const PRREMesh3D& mesh)
{
    /*
    p = new PRREMesh3DImpl(*(mesh.p));
    p->_pOwner = this;                             
    for (TPRREint i = 0; i < mesh.getCount(); i++) 
    {
        PRREMesh3D* subobj = new PRREMesh3D(*(PRREMesh3D*) (mesh.getAttachedAt(i)));
        Attach(*subobj);
    }
    */
}


PRREMesh3D& PRREMesh3D::operator=(const PRREMesh3D&)
{
    return *this;
}


/**
    Acquires the resources and properties of the given Mesh3D object, meaning that
    the given mesh will lost the geometry and other resources.
    This is a way of moving assets of a mesh to another mesh, MOVING, so no array copy is involved.
    Can be done only for level-2 meshes by level-2 meshes, or for level-1 meshes by level-1 meshes.
    This means that in order to fully cannibalize a level-1 mesh, this function should be called for
    the level-1 mesh by the level-1 mesh and then for all of its submeshes one by one by the submeshes
    of the cannibalizer mesh.
*/
void PRREMesh3D::Cannibalize(PRREMesh3D& victim)
{
    // Note that we cannot easily copy victim's Impl since copy ctor and assignment operators are empty!
    // Thus the following assignment cannot be used: *(this->p) = *(victim.p)
    // We wouldn't even want to use copy ctor or assignment here since we do not want any array copy.
    // That is why this function is written, to "steal" the arrays of the victim so victim will be "empty".
    // In C++11 this could be done with a move ctor.

    // Note: this function could be recursive and be called for level-1 mesh only, it would get the job
    // done for all the submeshes too. But now it was easier to implement this silly way so it needs to be
    // invoked manually for all submeshes too.
    
    // we should not have any submesh at this point, and no geometry yet
    if ( (getCount() != 0) || (p->pVertices != PGENULL) || (p->nVertices_h > 0) )
    {
        PRREFiledManaged::getConsole().EOLn(
            "PRREMesh3D::Cannibalize() ERROR: cannibalizer has non-zero managed count (%d) or has geometry (%d)!", getCount(), p->nVertices_h);
        return;
    }

    // at this point we do not need to deal with the Manager or FiledManager parts

    SetName(victim.getName());          // copy the Managed part
    SetFilename(victim.getFilename());  // copy the FiledManaged part

    p->primitiveFormat = victim.p->primitiveFormat;

    p->pVertices = victim.p->pVertices;
    p->pNormals = victim.p->pNormals;
    p->pVertexIndices = victim.p->pVertexIndices;
    p->nMinIndex = victim.p->nMinIndex;
    p->nMaxIndex = victim.p->nMaxIndex;
    p->nIndicesType = victim.p->nIndicesType;

    p->nVertices_h = victim.p->nVertices_h;
    p->nNormals_h = victim.p->nNormals_h;
    p->nVertexIndices_h = victim.p->nVertexIndices_h;
    p->nFaces_h = victim.p->nFaces_h;

    p->vPos = victim.p->vPos;
    p->vSize = victim.p->vSize;

    p->pMaterial = victim.p->pMaterial;

    victim.p->pVertices = PGENULL;
    victim.p->pNormals = PGENULL;
    victim.p->pVertexIndices = PGENULL;
    victim.p->pMaterial = PGENULL;
    victim.p->nMinIndex = UINT_MAX;
    victim.p->nMaxIndex = 0;
    victim.p->nIndicesType = GL_UNSIGNED_BYTE;
    victim.p->nVertices_h = victim.p->nNormals_h = 0;
    victim.p->nVertexIndices_h = 0;
    victim.p->nFaces_h = 0;
}


// ############################### PRIVATE ###############################


