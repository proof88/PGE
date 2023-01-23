/*
    ###################################################################################
    PureMesh3D.cpp
    This file is part of PURE.
    PureMesh3D class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Object3D/PureMesh3DManager.h"
#include "../../include/internal/Object3D/PureMesh3DImpl.h"
#include "../../include/internal/PureGLsnippets.h"
#include "../../include/internal/Purepragmas.h"
#include "../../../../../PFL/PFL/PFL.h"

using namespace std;

/*
   PureMesh3D::PureMesh3DImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureMesh3D::PureMesh3DImpl::~PureMesh3DImpl()
{
    _pOwner->getManagedConsole().OLnOI("~PureMesh3DImpl() ...");

    delete[] pVertices;
    delete[] pNormals;
    delete[] pVertexIndices;
    pVertices = PGENULL;
    pNormals = PGENULL;
    pVertexIndices = PGENULL;

    delete pMaterial;
    pMaterial = PGENULL;

    _pOwner->DeleteAll();

    _pOwner->getManagedConsole().SOLnOO("Done!");
} // ~PureTexture()


TPureBool PureMesh3D::PureMesh3DImpl::isLevel1() const
{
    return _pOwner->getCount() > 0;
}

TPureBool PureMesh3D::PureMesh3DImpl::isLevel2() const
{
    return !isLevel1();
}


TPURE_PRIMITIVE_FORMAT PureMesh3D::PureMesh3DImpl::getPrimitiveFormat() const
{
    return primitiveFormat;
} // getPrimitiveFormat()


TPureUInt PureMesh3D::PureMesh3DImpl::getVerticesCount() const
{
    if ( isLevel1() )
    {
        TPureUInt sum = 0;
        for (TPureInt i = 0; i < _pOwner->getSize(); i++)
        {
            if ( _pOwner->getAttachedAt(i) == PGENULL )
                continue;
            
            sum += ((PureMesh3D*)(_pOwner->getAttachedAt(i)))->getVerticesCount();
        }
        return sum;
    }
    else
        return nVertices_h;
} // getVerticesCount()


const TXYZ* PureMesh3D::PureMesh3DImpl::getVertices(TPureBool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getVertices();
    else
        return pVertices;
} // getVertices()


TXYZ* PureMesh3D::PureMesh3DImpl::getVertices(TPureBool implicitAccessSubobject)
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getVertices();
    else
        return pVertices;
} // getVertices()


TPureUInt PureMesh3D::PureMesh3DImpl::getVertexIndicesCount() const
{
    if ( isLevel1() )
    {
        TPureUInt sum = 0;
        for (TPureInt i = 0; i < _pOwner->getSize(); i++)
        {
            if ( _pOwner->getAttachedAt(i) == PGENULL )
                continue;
            
            sum += ((PureMesh3D*)(_pOwner->getAttachedAt(i)))->getVertexIndicesCount();
        }
        return sum;
    }
    else
        return nVertexIndices_h; 
}


const void* PureMesh3D::PureMesh3DImpl::getVertexIndices(TPureBool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getVertexIndices();
    else
        return pVertexIndices;
}


unsigned int PureMesh3D::PureMesh3DImpl::getVertexIndicesType(TPureBool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getVertexIndicesType();
    else
        return nIndicesType;
}


TPureUInt PureMesh3D::PureMesh3DImpl::getMinVertexIndex(TPureBool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getMinVertexIndex();
    else
        return nMinVertexIndex;
}


TPureUInt PureMesh3D::PureMesh3DImpl::getMaxVertexIndex(TPureBool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getMaxVertexIndex();
    else
        return nMaxVertexIndex;
}


TPureUInt PureMesh3D::PureMesh3DImpl::getVertexIndex(TPureUInt index, TPureBool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
    {
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getVertexIndex(index);
    }

    if ( (index >= nVertexIndices_h) || (index > nMaxVertexIndex) || (index < nMinVertexIndex) )
    {
        _pOwner->getManagedConsole().EOLn("getVertexIndex(%d) out of range: nVertexIndices_h: %d, nMinVertexIndex: %d, nMaxVertexIndex: %d!",
            index, nVertexIndices_h, nMinVertexIndex, nMaxVertexIndex);
        return 0;
    }

    if ( !pVertexIndices )
    {
        _pOwner->getManagedConsole().EOLn("getVertexIndex(%d) pVertexIndices is NULL! Range looks ok though: nVertexIndices_h: %d, nMinVertexIndex: %d, nMaxVertexIndex: %d!",
            index, nVertexIndices_h, nMinVertexIndex, nMaxVertexIndex);
        return 0;
    }

    return PureGLsnippets::getVertexIndex(pVertexIndices, index, nIndicesType);
}


const TXYZ* PureMesh3D::PureMesh3DImpl::getNormals(TPureBool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getNormals();
    else
        return pNormals;
} // getNormals()


TPureUInt PureMesh3D::PureMesh3DImpl::getFaceCount() const
{
    if ( isLevel1() )
    {
        TPureUInt sum = 0;
        for (TPureInt i = 0; i < _pOwner->getSize(); i++)
        {
            if ( _pOwner->getAttachedAt(i) == PGENULL )
                continue;
            
            sum += ((PureMesh3D*)(_pOwner->getAttachedAt(i)))->getFaceCount();
        }
        return sum;
    }

    switch ( _pOwner->getPrimitiveFormat() )
    {
    case PURE_PM_TRIANGLES:
        return nVertexIndices_h / 3;
    default:
        /* the other only choice is PURE_PM_QUADS currently */
        return nVertexIndices_h / 4;
    }
} // getFaceCount()


TPureUInt PureMesh3D::PureMesh3DImpl::getTriangleCount() const
{
    if ( isLevel1() )
    {
        TPureUInt sum = 0;
        for (TPureInt i = 0; i < _pOwner->getSize(); i++)
        {
            if ( _pOwner->getAttachedAt(i) == PGENULL )
                continue;
            
            sum += ((PureMesh3D*)(_pOwner->getAttachedAt(i)))->getTriangleCount();
        }
        return sum;
    }

    switch ( _pOwner->getPrimitiveFormat() )
    {
    case PURE_PM_TRIANGLES:
        return nVertexIndices_h / 3;
    default:
        /* the other only choice is PURE_PM_QUADS currently */
        return nVertexIndices_h / 4 * 2;
    }
} // getTriangleCount()


PureVector& PureMesh3D::PureMesh3DImpl::getPosVec()
{
    return vPos;
} // getPosVec()


const PureVector& PureMesh3D::PureMesh3DImpl::getRelPosVec() const
{
    return vRelPos;
} // getRelPosVec()


const PureVector& PureMesh3D::PureMesh3DImpl::getPosVec() const
{
    return vPos;
} // getPosVec()


const PureVector& PureMesh3D::PureMesh3DImpl::getSizeVec() const
{
    return vSize;
} // getSizeVec()


void PureMesh3D::PureMesh3DImpl::RecalculateSize()
{
    if ( isLevel1() )
    {
        // we are parent, ask our submeshes to determine their sizes and object-local positions
        for (TPureInt i = 0; i < _pOwner->getCount(); i++)
            ((PureMesh3D*)_pOwner->getAttachedAt(i))->RecalculateSize();

        // now we can determine our own size as parent based on min and max subobject positions and size
        TXYZ minvals, maxvals;
        const PureMesh3D* subMesh = (PureMesh3D*)_pOwner->getAttachedAt(0);
        minvals.x = subMesh->getPosVec().getX() - subMesh->getSizeVec().getX() / 2.0f;
        minvals.y = subMesh->getPosVec().getY() - subMesh->getSizeVec().getY() / 2.0f;
        minvals.z = subMesh->getPosVec().getZ() - subMesh->getSizeVec().getZ() / 2.0f;
        maxvals.x = subMesh->getPosVec().getX() + subMesh->getSizeVec().getX() / 2.0f;
        maxvals.y = subMesh->getPosVec().getY() + subMesh->getSizeVec().getY() / 2.0f;
        maxvals.z = subMesh->getPosVec().getZ() + subMesh->getSizeVec().getZ() / 2.0f;

        for (TPureInt i = 1; i < _pOwner->getCount(); i++)
        {
            subMesh = (PureMesh3D*)_pOwner->getAttachedAt(i);
            if ( minvals.x > subMesh->getPosVec().getX() - subMesh->getSizeVec().getX() / 2.0f )
                minvals.x = subMesh->getPosVec().getX() - subMesh->getSizeVec().getX() / 2.0f;
            if ( maxvals.x < subMesh->getPosVec().getX() + subMesh->getSizeVec().getX() / 2.0f )
                maxvals.x = subMesh->getPosVec().getX() + subMesh->getSizeVec().getX() / 2.0f;
            if ( minvals.y > subMesh->getPosVec().getY() - subMesh->getSizeVec().getY() / 2.0f )
                minvals.y = subMesh->getPosVec().getY() - subMesh->getSizeVec().getY() / 2.0f;
            if ( maxvals.y < subMesh->getPosVec().getY() + subMesh->getSizeVec().getY() / 2.0f )
                maxvals.y = subMesh->getPosVec().getY() + subMesh->getSizeVec().getY() / 2.0f;
            if ( minvals.z > subMesh->getPosVec().getZ() - subMesh->getSizeVec().getZ() / 2.0f )
                minvals.z = subMesh->getPosVec().getZ() - subMesh->getSizeVec().getZ() / 2.0f;
            if ( maxvals.z < subMesh->getPosVec().getZ() + subMesh->getSizeVec().getZ() / 2.0f )
                maxvals.z = subMesh->getPosVec().getZ() + subMesh->getSizeVec().getZ() / 2.0f;
        }

        vSize.SetX( abs( minvals.x - maxvals.x ) );
        vSize.SetY( abs( minvals.y - maxvals.y ) );
        vSize.SetZ( abs( minvals.z - maxvals.z ) );

        // since we are parent we dont need to modify our world-space position at all based on our subobjects

        vRelPos.SetX( minvals.x + vSize.getX() / 2.0f );
        vRelPos.SetY( minvals.y + vSize.getY() / 2.0f );
        vRelPos.SetZ( minvals.z + vSize.getZ() / 2.0f );

        return;
    }

    // as level-2 in theory we have geometry but check it for sure
    if ( (nVertices_h == 0) || (pVertices == PGENULL) )
    {
        _pOwner->getManagedConsole().EOLn("PureMesh3D::PureMesh3DImpl::RecalculateSize() in level-2 but vertex data is not present!");
        return;
    }

    // we are subobject, so we need to calculate our own size and object-local position based on min and max vertex positions

    TXYZ minvals, maxvals;
    minvals.x = pVertices[0].x;
    minvals.y = pVertices[0].y;
    minvals.z = pVertices[0].z;
    maxvals.x = minvals.x;
    maxvals.y = minvals.y;
    maxvals.z = minvals.z;

    for (TPureUInt i = 1; i < nVertices_h; i++)
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

    // vRelPos is not updated for level-2, as default [0,0,0] is always valid since we set position above based on minvals and size!
    
} // RecalculateSize()


const PureMaterial& PureMesh3D::PureMesh3DImpl::getMaterial(TPureBool implicitAccessSubobject) const
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getMaterial();
    else
        return *pMaterial;
} // getMaterial()


PureMaterial& PureMesh3D::PureMesh3DImpl::getMaterial(TPureBool implicitAccessSubobject)
{
    if ( implicitAccessSubobject && isLevel1() && (_pOwner->getCount() == 1) )
        return ((PureMesh3D*) (_pOwner->getAttachedAt(0)))->getMaterial();
    else
        return *pMaterial;
} // getMaterial()


TPureUInt PureMesh3D::PureMesh3DImpl::getUsedSystemMemory() const
{
    return sizeof(*this) + (pMaterial ? pMaterial->getUsedSystemMemory() : 0) + 
        nVertices_h * sizeof(TXYZ) * 2 /* normals and vertices */  +
        ( PureGLsnippets::getSizeofIndexType(nIndicesType) * nVertexIndices_h );
} // getUsedSystemMemory() 


/**
    Acquires the resources and properties of the given Mesh3D object, meaning that
    the given mesh will lose the geometry and other resources.
    This is a way of moving assets of a mesh to another mesh, MOVING, so no array copy is involved.
    Can be done only for level-2 meshes by level-2 meshes, or for level-1 meshes by level-1 meshes.
    This means that in order to fully cannibalize a level-1 mesh, this function should be called for
    the level-1 mesh by the level-1 mesh and then for all of its submeshes one by one by the submeshes
    of the cannibalizer mesh.

    @return True on success, false on error: if the cannibalizer mesh already has any submesh or existing geometry.
*/
TPureBool PureMesh3D::PureMesh3DImpl::cannibalize(PureMesh3D& victim)
{
    // Note that we cannot easily copy victim's Impl since copy ctor and assignment operators are empty!
    // Thus the following assignment cannot be used: *(this->pImpl) = *(victim.pImpl)
    // We wouldn't even want to use copy ctor or assignment here since we do not want any array copy.
    // That is why this function is written, to "steal" the arrays of the victim so victim will be "empty".
    // In C++11 this could be done with a move ctor.

    // Note: this function could be recursive and be called for level-1 mesh only, it would get the job
    // done for all the submeshes too. But now it was easier to implement this silly way so it needs to be
    // invoked manually for all submeshes too.
    
    // we should not have any submesh at this point, and no geometry yet
    if ( (_pOwner->getCount() != 0) || (pVertices != PGENULL) || (nVertices_h > 0) )
    {
        _pOwner->getManagedConsole().EOLn(
            "PureMesh3D::cannibalize() ERROR: cannibalizer has non-zero managed count (%d) or has geometry (%d)!", _pOwner->getCount(), nVertices_h);
        return false;
    }

    // at this point we do not need to deal with the Manager or FiledManager parts

    _pOwner->SetName(victim.getName());          // copy the Managed part
    _pOwner->SetFilename(victim.getFilename());  // copy the FiledManaged part

    primitiveFormat = victim.pImpl->primitiveFormat;

    pVertices = victim.pImpl->pVertices;
    pNormals = victim.pImpl->pNormals;
    pVertexIndices = victim.pImpl->pVertexIndices;
    nMinVertexIndex = victim.pImpl->nMinVertexIndex;
    nMaxVertexIndex = victim.pImpl->nMaxVertexIndex;
    nIndicesType = victim.pImpl->nIndicesType;

    nVertices_h = victim.pImpl->nVertices_h;
    nVertexIndices_h = victim.pImpl->nVertexIndices_h;
    nFaces_h = victim.pImpl->nFaces_h;

    vPos = victim.pImpl->vPos;
    vRelPos = victim.pImpl->vRelPos;
    vSize = victim.pImpl->vSize;

    pMaterial = victim.pImpl->pMaterial;
    pMaterial->SetUtiliser(this->_pOwner);

    victim.pImpl->pVertices = PGENULL;
    victim.pImpl->pNormals = PGENULL;
    victim.pImpl->pVertexIndices = PGENULL;
    victim.pImpl->pMaterial = PGENULL;
    victim.pImpl->nMinVertexIndex = UINT_MAX;
    victim.pImpl->nMaxVertexIndex = 0;
    victim.pImpl->nIndicesType = GL_UNSIGNED_BYTE;
    victim.pImpl->nVertices_h = 0;
    victim.pImpl->nVertexIndices_h = 0;
    victim.pImpl->nFaces_h = 0;

    return true;
}  // cannibalize()


// ############################## PROTECTED ##############################


/**
    @param owner The public PureMesh3D class instance owning this pimpl object.
    @param prfmt Requested primitive format for this mesh.
*/
PureMesh3D::PureMesh3DImpl::PureMesh3DImpl(PureMesh3D* owner, TPURE_PRIMITIVE_FORMAT prfmt)
{
    _pOwner = owner;
    _pOwner->getManagedConsole().OLnOI("PureMesh3DImpl() ...");

    primitiveFormat = prfmt;
    pVertices = PGENULL;
    pNormals = PGENULL;
    pVertexIndices = PGENULL;

    nMinVertexIndex = UINT_MAX;
    nMaxVertexIndex = 0;
    nIndicesType = GL_UNSIGNED_BYTE;

    nVertices_h = 0;
    nVertexIndices_h = 0;
    nFaces_h = 0;

    pMaterial = PGENULL;

    _pOwner->getManagedConsole().SOLnOO("Done!");
} // PureMesh3D()


PureMesh3D::PureMesh3DImpl::PureMesh3DImpl(const PureMesh3DImpl& mimpl)
{
    /*
    // _pOwner must not be set here, owner will overwrite anyway
    nMinVertexIndex = mimpl.nMinVertexIndex;
    nMaxVertexIndex = mimpl.nMaxVertexIndex;
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
        pVertexIndices = malloc( PureGLsnippets::getSizeofIndexType(nIndicesType) * nVertexIndices_h );
        memcpy(pVertexIndices, mimpl.pVertexIndices, sizeof(TXYZ) * nVertexIndices_h);
    }
    else
        pVertexIndices = PGENULL;

    pMaterial = PGENULL; // should be copied
    */
}


PureMesh3D::PureMesh3DImpl& PureMesh3D::PureMesh3DImpl::operator=(const PureMesh3DImpl&)
{
    return *this;
}


/**
    Sets an index value in the vertex index array.
    Since this also checks for violating range defined by nMinVertexIndex and nMaxVertexIndex, even during building up a geometry,
    those limits should be properly updated before using this function.

    @return True on success, false otherwise.
*/
TPureBool PureMesh3D::PureMesh3DImpl::setVertexIndex(TPureUInt index, TPureUInt value)
{
    if ( (index >= nVertexIndices_h) || (index > nMaxVertexIndex) || (index < nMinVertexIndex) )
    {
        _pOwner->getManagedConsole().EOLn("setVertexIndex(%d) out of range: nVertexIndices_h: %d, nMinVertexIndex: %d, nMaxVertexIndex: %d!",
            index, nVertexIndices_h, nMinVertexIndex, nMaxVertexIndex);
        return false;
    }

    return PureGLsnippets::setVertexIndex(pVertexIndices, index, value, this->nIndicesType);
}


// ############################### PRIVATE ###############################


/*
   PureMesh3D
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureMesh3D::~PureMesh3D()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureTexture()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureMesh3D::getManagedConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureMesh3D::getLoggerModuleName()
{
    return "PureMesh3D";
} // getLoggerModuleName()


/**
    Tells if the mesh is a level-1 parent mesh.
    A mesh cannot be both level-1 and level-2 at the same time.
    A level-1 mesh does not have vertex data on its own, but its level-2 submeshes own vertex data.

    @return True if the mesh is a level-1 parent mesh i.e. it has at least 1 submesh, false otherwise.
*/
TPureBool PureMesh3D::isLevel1() const
{
    return pImpl->isLevel1();
}

/**
    Tells if the mesh is a level-2 submesh.
    A mesh cannot be both level-1 and level-2 at the same time.
    A level-1 mesh does not have vertex data on its own, but its level-2 submeshes own vertex data.

    @return True if the mesh is a level-2 submesh i.e. it doesn't have any submesh, false otherwise.
*/
TPureBool PureMesh3D::isLevel2() const
{
    return pImpl->isLevel2();
}


/**
    Gets the primitives' format.
    Primitive format is the same for the mesh and its submeshes.
*/
TPURE_PRIMITIVE_FORMAT PureMesh3D::getPrimitiveFormat() const
{
    return pImpl->getPrimitiveFormat();
} // getPrimitiveFormat()


/**
    Gets the number of total vertices.
    Number of total vertices for a level-1 mesh is the sum of number of vertices of its level-2 submeshes.

    @return Number of total vertices.
*/
TPureUInt PureMesh3D::getVerticesCount() const
{
    return pImpl->getVerticesCount();
} // getVerticesCount()


/**
    Gets the pointer to vertices.
    Pointer to vertices is NULL for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    Note: in special case this returns non-NULL value even for a level-1 mesh, see below.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return Pointer to vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const TXYZ* PureMesh3D::getVertices(TPureBool implicitAccessSubobject) const
{
    return pImpl->getVertices(implicitAccessSubobject);
} // getVertices()


/**
    Gets the pointer to vertices.
    Pointer to vertices is NULL for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    Note: in special case this returns non-NULL value even for a level-1 mesh, see below.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return Pointer to vertices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's vertices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TXYZ* PureMesh3D::getVertices(TPureBool implicitAccessSubobject)
{
    return pImpl->getVertices(implicitAccessSubobject);
} // getVertices()


/**
    Gets the number of total vertex indices.
    Number of total vertex indices for a level-1 mesh is the sum of number of vertex indices of its level-2 submeshes.

    @return Number of total vertex indices.
*/
TPureUInt PureMesh3D::getVertexIndicesCount() const
{
    return pImpl->getVertexIndicesCount();
}


/**
    Gets the pointer to vertex indices.
    Pointer to vertex indices is NULL for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    Note: in special case this returns non-NULL value even for a level-1 mesh, see below.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return Pointer to vertex indices. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's vertex indices pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const void* PureMesh3D::getVertexIndices(TPureBool implicitAccessSubobject) const
{
    return pImpl->getVertexIndices(implicitAccessSubobject);
}


/**
    Gets the type of the indices.
    Index type is irrelevant for a level-1 mesh since the geometry is owned by its level-2 submeshes and it is unique
    for each submesh.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return Vertex index type. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            type is the subobject's vertex index type. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
unsigned int PureMesh3D::getVertexIndicesType(TPureBool implicitAccessSubobject) const
{
    return pImpl->getVertexIndicesType(implicitAccessSubobject);
}


/**
    Gets the smallest index in the vertex indices array.
    This value is irrelevant for a level-1 mesh since the geometry is owned by its level-2 submeshes and it is unique
    for each submesh. Still the returned value for a level-1 mesh can be a positive value, see below.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return The smallest index in the vertex index array. If the object's own vertex count is 0 but it has exactly 1 subobject,
            the returned index is the subobject's smallest index. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPureUInt PureMesh3D::getMinVertexIndex(TPureBool implicitAccessSubobject) const
{
    return pImpl->getMinVertexIndex(implicitAccessSubobject);
}

/**
    Gets the greatest index in the vertex indices array.
    This value is irrelevant for a level-1 mesh since the geometry is owned by its level-2 submeshes and it is unique
    for each submesh. Still the returned value for a level-1 mesh can be a positive value, see below.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return The greatest index in the vertex index array. If the object's own vertex count is 0 but it has exactly 1 subobject,
            the returned index is the subobject's greatest index. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPureUInt PureMesh3D::getMaxVertexIndex(TPureBool implicitAccessSubobject) const
{
    return pImpl->getMaxVertexIndex(implicitAccessSubobject);
}

/**
    Gets an index value from the vertex indices array.
    This function is highly recommended to be used for reading indices from the index array since the type of indices can vary
    from subobject to subobject and this function considers the index type.
    This function is irrelevant for a level-1 mesh since the geometry is owned by its level-2 submeshes. Still the returned value
    for a level-1 mesh can be a valid value, see below.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return An index read from the given array. If the object's own vertex count is 0 but it has exactly 1 subobject,
            the returned index is the subobject's index. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
TPureUInt PureMesh3D::getVertexIndex(TPureUInt index, TPureBool implicitAccessSubobject) const
{
    return pImpl->getVertexIndex(index, implicitAccessSubobject);
}


/**
    Gets the pointer to normals.
    Pointer to normals is NULL for a level-1 mesh since the geometry is owned by its level-2 submeshes.
    Note: in special case this returns non-NULL value even for a level-1 mesh, see below.

    In indexed vertex referencing mode, the vertex indices can be used to reference normals in the array, same way
    as how vertices can be referenced.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return Pointer to normals. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned
            pointer is the subobject's normals pointer. This implicit behavior is for convenience for objects storing
            only 1 subobject like internally created objects.
*/
const TXYZ* PureMesh3D::getNormals(TPureBool implicitAccessSubobject) const
{
    return pImpl->getNormals(implicitAccessSubobject);
} // getNormals()


/**
    Gets the number of faces/polygons/primitives formed by the vertices.
    Face count doesn't only depend on vertex or vertex index count but also on the primitive format (see: getPrimitiveFormat()).
    Note that face count is not necessarily equal to getTriangleCount() because e.q. if primitive format is quads/quadrilaterals, then obviously
    face count will be smaller than triangle count. Even if primitive format indicates quads/quadrilaterals, they are split into triangles at some
    point when transferred through the graphics pipeline.

    @return Number of faces/polygons/primitives formed by the vertices.
*/
TPureUInt PureMesh3D::getFaceCount() const
{
    return pImpl->getFaceCount();
}

/**
    Gets the number of triangles formed by the vertices.
    Triangle count doesn't only depend on vertex or vertex index count but also on the primitive format (see: getPrimitiveFormat()).
    Note that triangle count is not necessarily equal to getFaceCount() because e.q. if primitive format is quads/quadrilaterals, then obviously
    triangle count will be greater than face count. Even if primitive format indicates quads/quadrilaterals, they are split into triangles at some
    point when transferred through the graphics pipeline.

    @return Number of triangles formed by the vertices.
*/
TPureUInt PureMesh3D::getTriangleCount() const
{
    return pImpl->getTriangleCount();
}


/**
    Gets the position.
    For a level-1 mesh object, this vector is always zero. It can be utilized by a descendant class though.
    For a level-2 (sub)mesh object, this vector represents the position of the submesh relative to the position of its parent mesh object.
    @return Position vector.
*/
PureVector& PureMesh3D::getPosVec()
{
    return pImpl->getPosVec();
} // getPosVec()


/**
    Gets the position.
    For a level-1 mesh object, this vector is always zero. It can be utilized by a descendant class though.
    For a level-2 (sub)mesh object, this vector represents the position of the submesh relative to the position of its parent mesh object.
    @return Position vector.
*/
const PureVector& PureMesh3D::getPosVec() const
{
    return pImpl->getPosVec();
} // getPosVec()


/**
    Gets the mesh-local relative position.
    This position tells the offset of the vertex positions relative to the center of the mesh [0,0,0].
    For level-2 (sub)mesh, this vector is always expected to be [0,0,0] since the position of submeshes is calculated with vertex positions and size.
    @return Mesh-local relative position vector.
*/
const PureVector& PureMesh3D::getRelPosVec() const
{
    return pImpl->getRelPosVec();
} // getRelPosVec()

/**
    Gets the base sizes.
    @return Base sizes vector.
*/
const PureVector& PureMesh3D::getSizeVec() const
{
    return pImpl->getSizeVec();
} // getSizeVec()


/**
    Recalculates the sizes.
*/
void PureMesh3D::RecalculateSize()
{
    pImpl->RecalculateSize();
} // RecalculateSize()


/**
    Gets the material.
    Note that some properties of a level-1 mesh are irrelevant because renderer doesn't even access them, e.g. texture, because
    they are fetched from the level-2 meshes. Some other material properties of level-2 meshes are irrelevant because they are
    inherited from the material of their parent level-1 mesh, e.g. texture environment color. Thus make sure you are careful
    when you specify the argument as described below.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return Material. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned material is the subobject's material.
            This implicit behavior is for convenience for objects storing only 1 subobject like internally created objects.
*/
const PureMaterial& PureMesh3D::getMaterial(TPureBool implicitAccessSubobject) const
{
    return pImpl->getMaterial(implicitAccessSubobject);
} // getMaterial()


/**
    Gets the material.
    Note that some properties of a level-1 mesh are irrelevant because renderer doesn't even access them, e.g. texture, because
    they are fetched from the level-2 meshes. Some other material properties of level-2 meshes are irrelevant because they are
    inherited from the material of their parent level-1 mesh, e.g. texture environment color. Thus make sure you are careful
    when you specify the argument as described below.

    @param implicitAccessSubobject If true (default), and this mesh has exactly 1 submesh, the accessed value
           will be that submesh's value. If false, the accessed value will be this mesh's value.

    @return Material. If the object's own vertex count is 0 but it has exactly 1 subobject, the returned material is the subobject's material.
            This implicit behavior is for convenience for objects storing only 1 subobject like internally created objects.
*/
PureMaterial& PureMesh3D::getMaterial(TPureBool implicitAccessSubobject)
{
    return pImpl->getMaterial(implicitAccessSubobject);
} // getMaterial()


/**
    Gets the amount of allocated system memory.
    It includes the allocated Material size as well (getMaterial()).
    Level-1 (parent) meshes summarize the memory usage of their level-2 submeshes and include it in the returned value.

    @return Amount of allocated system memory in Bytes.
*/
TPureUInt PureMesh3D::getUsedSystemMemory() const
{
    TPureUInt sumSubMeshMemoryUsage = 0;
    for (TPureInt i = 0; i < getSize(); i++)
    {
        if ( getAttachedAt(i) == PGENULL )
            continue;
        sumSubMeshMemoryUsage += getAttachedAt(i)->getUsedSystemMemory();
    }
    return sumSubMeshMemoryUsage +
        PureFiledManaged::getUsedSystemMemory() - sizeof(PureFiledManaged) +
        sizeof(*this) + pImpl->getUsedSystemMemory();
} // getUsedSystemMemory()


// ############################## PROTECTED ##############################


/**
    Only PureObject3DManager creates it.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureMesh3D::PureMesh3D(TPURE_PRIMITIVE_FORMAT prfmt)
{
    pImpl = new PureMesh3DImpl(this, prfmt);
} // PureMesh3D()


PureMesh3D::PureMesh3D(const PureMesh3D& mesh)
{

}


PureMesh3D& PureMesh3D::operator=(const PureMesh3D&)
{
    return *this;
}


// ############################### PRIVATE ###############################


