/*
    ###################################################################################
    PureMesh3DManager.cpp
    This file is part of PURE.
    PureMesh3DManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Object3D/PureMesh3DManager.h"
#include "../../include/internal/Object3D/PureMesh3DImpl.h"
#include "../../include/internal/Object3D/PureMesh3DManagerImpl.h"

#include <cassert>

#include "../../include/external/Hardware/PureHwInfo.h"
#include "../../include/internal/PureGLextensionFuncs.h"
#include "../../include/internal/PureGLsnippets.h"
#include "../../include/internal/PurePragmas.h"

using namespace std;

/*
   PureMesh3DManager::PureMesh3DManagerImpl
   ###########################################################################
*/


// ############################### PUBLIC ################################


PureMesh3DManager::PureMesh3DManagerImpl::~PureMesh3DManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PureMesh3DManager() ...");
    _pOwner->DeleteAll();
    _pOwner->getConsole().OO();    
    _pOwner = NULL;
} // ~PureMesh3DManager()


TPureBool PureMesh3DManager::PureMesh3DManagerImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


TPureBool PureMesh3DManager::PureMesh3DManagerImpl::isMinimalIndexStorageEnabled() const
{
    /* currently not used, but it is in the todo list history txt */
    return bMinimalIndexStorage;
}


void PureMesh3DManager::PureMesh3DManagerImpl::SetMinimalIndexStorageEnabled(TPureBool state)
{
    /* currently not used, but it is in the todo list history txt */
    bMinimalIndexStorage = state;
}


/**
    Converts the given Mesh to a Plane Mesh with the given sizes.
    Expects level-1 Mesh (so it must have subobject) and it must have exactly 1 subobject only.
    This function is very useful for descendant classes that want to construct custom objects
    based on Mesh3D, since during the constructing of custom object, this function can be invoked
    to set up the underlying geometry.
    Example use case: Object3D, when being constructed, can be given as parameter to this function,
    so it will be a renderable Plane object, without any array copy operation during construction.

    @param mesh The Mesh instance to be converted to a Plane Mesh.
    @param a    Length of plane (size on X-axis).
    @param b    Height of plane (size on Y-axis).
*/
TPureBool PureMesh3DManager::PureMesh3DManagerImpl::convertToPlane(
        PureMesh3D& mesh, TPureFloat a, TPureFloat b)
{
    _pOwner->getConsole().OLn("PureMesh3DManager::convertToPlane(%f, %f)", a, b);

    if ( !mesh.isLevel1() )
    {
        _pOwner->getConsole().EOLn("ERROR: given Mesh is NOT level-1!", mesh.getCount());
        return false;
    }

    if ( mesh.getCount() != 1 )
    {
        _pOwner->getConsole().EOLn("ERROR: given Mesh has %d manageds, expected is 1!", mesh.getCount());
        return false;
    }

    mesh.pImpl->primitiveFormat = PURE_PM_QUADS;
    mesh.pImpl->vPos.Set(0.0f, 0.0f, 0.0f);
    mesh.pImpl->vSize.Set(a, b, 0.0f);

    if ( mesh.pImpl->pMaterial != PGENULL )
    {
        materialMgr.DeleteAttachedInstance( *(mesh.pImpl->pMaterial) );
    }
    createMaterialForMesh(mesh);
    if ( mesh.pImpl->pMaterial == PGENULL )
    {
        _pOwner->getConsole().EOLn("ERROR: failed to create material for mesh!");
        return false;
    }

    mesh.pImpl->nMinVertexIndex = 0;
    mesh.pImpl->nMaxVertexIndex = 0;

    PureMesh3D* const submesh = (PureMesh3D*) mesh.getAttachedAt(0);

    submesh->pImpl->primitiveFormat = mesh.pImpl->primitiveFormat;
    submesh->pImpl->vPos.Set(0.0f, 0.0f, 0.0f);
    submesh->pImpl->vSize.Set(a, b, 0.0f);
    mesh.pImpl->nIndicesType = GL_UNSIGNED_BYTE;
    submesh->pImpl->nVertices_h  = 4;
    submesh->pImpl->nFaces_h     = 1;
    submesh->pImpl->nVertexIndices_h   = submesh->pImpl->nVertices_h;

    if ( submesh->pImpl->pMaterial != PGENULL )
    {
        materialMgr.DeleteAttachedInstance( *(submesh->pImpl->pMaterial) );
    }
    createMaterialForMesh(*submesh);
    if ( submesh->pImpl->pMaterial == PGENULL )
    {
        materialMgr.DeleteAttachedInstance( *(mesh.pImpl->pMaterial) );
        _pOwner->getConsole().EOLn("ERROR: failed to create material for submesh!");
        return false;
    }

    delete[] submesh->pImpl->pVertices;
    delete[] submesh->pImpl->pNormals;
    delete[] submesh->pImpl->pVertexIndices;

    submesh->pImpl->pVertices = PGENULL;
    submesh->pImpl->pNormals = PGENULL;
    submesh->pImpl->pVertexIndices = PGENULL;

    try
    {
        submesh->pImpl->pVertices = new TXYZ[submesh->pImpl->nVertices_h];
        submesh->pImpl->pNormals = new TXYZ[submesh->pImpl->nVertices_h];
        //submesh->pImpl->pFaces     = (TFACE4*)     malloc( sizeof(TFACE4) * submesh->pImpl->nFaces_h );
        submesh->pImpl->nIndicesType = mesh.pImpl->nIndicesType;
        submesh->pImpl->pVertexIndices = new char[PureGLsnippets::getSizeofIndexType(submesh->pImpl->nIndicesType) * submesh->pImpl->nVertexIndices_h];
    }
    catch (const std::bad_alloc&)
    {
        delete[] submesh->pImpl->pVertices;
        delete[] submesh->pImpl->pNormals;
        delete[] submesh->pImpl->pVertexIndices;
        submesh->pImpl->pVertices = PGENULL;
        submesh->pImpl->pNormals = PGENULL;
        submesh->pImpl->pVertexIndices = PGENULL;
        materialMgr.DeleteAttachedInstance( *(mesh.pImpl->pMaterial) );
        mesh.pImpl->pMaterial = PGENULL;
        _pOwner->getConsole().EOLn("ERROR: failed to allocate memory for geometry!");
        return false;
    }

    submesh->pImpl->pMaterial->allocateArrays(
        submesh->pImpl->nVertices_h,
        submesh->pImpl->nVertices_h,
        submesh->pImpl->nVertexIndices_h,
        PureGLsnippets::getSizeofIndexType(submesh->pImpl->nIndicesType) );

    submesh->pImpl->pVertices[0].x  = -a / 2.0f;
    submesh->pImpl->pVertices[0].y  = -b / 2.0f;
    submesh->pImpl->pVertices[0].z  = 0.0f;
    // no need to fill pVerticesTransf array, that task is for during rendering
    submesh->pImpl->pNormals[0].x   = 0.0f;
    submesh->pImpl->pNormals[0].y   = 0.0f;
    submesh->pImpl->pNormals[0].z   = -1.0f;
    submesh->pImpl->pMaterial->getColors()[0].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[0].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[0].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[0].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[0].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[0].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[0].w = 0.0f;

    submesh->pImpl->pVertices[1].x  = a / 2.0f;
    submesh->pImpl->pVertices[1].y  = -b / 2.0f;
    submesh->pImpl->pVertices[1].z  = 0.0f;
    submesh->pImpl->pNormals[1].x   = 0.0f;
    submesh->pImpl->pNormals[1].y   = 0.0f;
    submesh->pImpl->pNormals[1].z   = -1.0f;
    submesh->pImpl->pMaterial->getColors()[1].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[1].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[1].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[1].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[1].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[1].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[1].w = 0.0f;

    submesh->pImpl->pVertices[2].x  = a / 2.0f;
    submesh->pImpl->pVertices[2].y  = b / 2.0f;
    submesh->pImpl->pVertices[2].z  = 0.0f;
    submesh->pImpl->pNormals[2].x   = 0.0f;
    submesh->pImpl->pNormals[2].y   = 0.0f;
    submesh->pImpl->pNormals[2].z   = -1.0f;
    submesh->pImpl->pMaterial->getColors()[2].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[2].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[2].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[2].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[2].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[2].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[2].w = 0.0f;

    submesh->pImpl->pVertices[3].x  = -a / 2.0f;
    submesh->pImpl->pVertices[3].y  = b / 2.0f;
    submesh->pImpl->pVertices[3].z  = 0.0f;
    submesh->pImpl->pNormals[3].x   = 0.0f;
    submesh->pImpl->pNormals[3].y   = 0.0f;
    submesh->pImpl->pNormals[3].z   = -1.0f;
    submesh->pImpl->pMaterial->getColors()[3].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[3].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[3].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[3].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[3].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[3].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[3].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->pImpl->pFaces[0].vertex_ids[i] = i;
        submesh->pImpl->pFaces[0].texcoord_ids[i] = i;
        submesh->pImpl->pFaces[0].normal_ids[i] = i;
    }*/

    for (TPureUInt i = 0; i < submesh->pImpl->nVertexIndices_h; i++)
    {
        if ( submesh->pImpl->nMinVertexIndex > i )
            submesh->pImpl->nMinVertexIndex = i;
        if ( submesh->pImpl->nMaxVertexIndex < i )
            submesh->pImpl->nMaxVertexIndex = i;
        bool res = submesh->pImpl->setVertexIndex(i, i);
        assert( res );
    }
    return true;
} // convertToPlane


/**
    Converts the given Mesh to a Box Mesh with the given sizes.
    Expects level-1 Mesh (so it must have subobject) and it must have exactly 1 subobject only.
    This function is very useful for descendant classes that want to construct custom objects
    based on Mesh3D, since during the constructing of custom object, this function can be invoked
    to set up the underlying geometry.
    Example use case: Object3D, when being constructed, can be given as parameter to this function,
    so it will be a renderable Box object, without any array copy operation during construction.

    @param mesh The Mesh instance to be converted to a Box Mesh.
    @param a Length of box (size on X-axis).
    @param b Height of box (size on Y-axis).
    @param c Depth of box (size on Z-axis).
*/
TPureBool PureMesh3DManager::PureMesh3DManagerImpl::convertToBox(
        PureMesh3D& mesh, TPureFloat a, TPureFloat b, TPureFloat c)
{
    _pOwner->getConsole().OLn("PureMesh3DManager::convertToBox(%f, %f, %f)", a, b, c);

    if ( !mesh.isLevel1() )
    {
        _pOwner->getConsole().EOLn("ERROR: given Mesh is NOT level-1!", mesh.getCount());
        return false;
    }

    if ( mesh.getCount() != 1 )
    {
        _pOwner->getConsole().EOLn("ERROR: given Mesh has %d manageds, expectedg is 1!", mesh.getCount());
        return false;
    }

    mesh.pImpl->primitiveFormat = PURE_PM_QUADS;
    mesh.pImpl->vPos.Set(0.0f, 0.0f, 0.0f);
    mesh.pImpl->vSize.Set(a, b, c);
    mesh.pImpl->nIndicesType = GL_UNSIGNED_BYTE;

    if ( mesh.pImpl->pMaterial != PGENULL )
    {
        materialMgr.DeleteAttachedInstance( *(mesh.pImpl->pMaterial) );
    }
    createMaterialForMesh(mesh);
    if ( mesh.pImpl->pMaterial == PGENULL )
    {
        _pOwner->getConsole().EOLn("ERROR: failed to create material for mesh!");
        return false;
    }

    mesh.pImpl->nMinVertexIndex = 0;
    mesh.pImpl->nMaxVertexIndex = 0;

    PureMesh3D* const submesh = (PureMesh3D*) mesh.getAttachedAt(0);
    submesh->pImpl->primitiveFormat = mesh.pImpl->primitiveFormat;
    submesh->pImpl->vPos.Set(0.0f, 0.0f, 0.0f);
    submesh->pImpl->vSize.Set(a, b, c);
    submesh->pImpl->nVertices_h  = 24;
    submesh->pImpl->nFaces_h     = 6;
    submesh->pImpl->nVertexIndices_h   = submesh->pImpl->nVertices_h;

    if ( submesh->pImpl->pMaterial != PGENULL )
    {
        materialMgr.DeleteAttachedInstance( *(submesh->pImpl->pMaterial) );
    }
    createMaterialForMesh(*submesh);
    if ( submesh->pImpl->pMaterial == PGENULL )
    {
        materialMgr.DeleteAttachedInstance( *(mesh.pImpl->pMaterial) );
        _pOwner->getConsole().EOLn("ERROR: failed to create material for submesh!");
        return false;
    }

    delete[] submesh->pImpl->pVertices;
    delete[] submesh->pImpl->pNormals;
    delete[] submesh->pImpl->pVertexIndices;

    submesh->pImpl->pVertices = PGENULL;
    submesh->pImpl->pNormals = PGENULL;
    submesh->pImpl->pVertexIndices = PGENULL;

    try
    {
        submesh->pImpl->pVertices = new TXYZ[submesh->pImpl->nVertices_h];
        submesh->pImpl->pNormals = new TXYZ[submesh->pImpl->nVertices_h];
        //submesh->pImpl->pFaces     = (TFACE4*)     malloc( sizeof(TFACE4) * submesh->pImpl->nFaces_h );
        submesh->pImpl->nIndicesType = mesh.pImpl->nIndicesType;
        submesh->pImpl->pVertexIndices = new char[PureGLsnippets::getSizeofIndexType(submesh->pImpl->nIndicesType) * submesh->pImpl->nVertexIndices_h];
    }
    catch (const std::bad_alloc&)
    {
        delete[] submesh->pImpl->pVertices;
        delete[] submesh->pImpl->pNormals;
        delete[] submesh->pImpl->pVertexIndices;
        submesh->pImpl->pVertices = PGENULL;
        submesh->pImpl->pNormals = PGENULL;
        submesh->pImpl->pVertexIndices = PGENULL;
        materialMgr.DeleteAttachedInstance( *(mesh.pImpl->pMaterial) );
        mesh.pImpl->pMaterial = PGENULL;
        _pOwner->getConsole().EOLn("ERROR: failed to allocate memory for geometry!");
        return false;
    }

    submesh->pImpl->pMaterial->allocateArrays(
        submesh->pImpl->nVertices_h,
        submesh->pImpl->nVertices_h,
        submesh->pImpl->nVertexIndices_h,
        PureGLsnippets::getSizeofIndexType(submesh->pImpl->nIndicesType) );

    // front face
    submesh->pImpl->pVertices[0].x  = -a / 2.0f;
    submesh->pImpl->pVertices[0].y  = -b / 2.0f;
    submesh->pImpl->pVertices[0].z  = -c / 2.0f;
    submesh->pImpl->pNormals[0].x   = 0.0f;
    submesh->pImpl->pNormals[0].y   = 0.0f;
    submesh->pImpl->pNormals[0].z   = -1.0f;
    submesh->pImpl->pMaterial->getColors()[0].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[0].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[0].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[0].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[0].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[0].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[0].w = 0.0f;

    submesh->pImpl->pVertices[1].x  = a / 2.0f;
    submesh->pImpl->pVertices[1].y  = -b / 2.0f;
    submesh->pImpl->pVertices[1].z  = -c / 2.0f;
    submesh->pImpl->pNormals[1].x   = 0.0f;
    submesh->pImpl->pNormals[1].y   = 0.0f;
    submesh->pImpl->pNormals[1].z   = -1.0f;
    submesh->pImpl->pMaterial->getColors()[1].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[1].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[1].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[1].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[1].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[1].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[1].w = 0.0f;

    submesh->pImpl->pVertices[2].x  = a / 2.0f;
    submesh->pImpl->pVertices[2].y  = b / 2.0f;
    submesh->pImpl->pVertices[2].z  = -c / 2.0f;
    submesh->pImpl->pNormals[2].x   = 0.0f;
    submesh->pImpl->pNormals[2].y   = 0.0f;
    submesh->pImpl->pNormals[2].z   = -1.0f;
    submesh->pImpl->pMaterial->getColors()[2].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[2].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[2].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[2].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[2].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[2].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[2].w = 0.0f;

    submesh->pImpl->pVertices[3].x  = -a / 2.0f;
    submesh->pImpl->pVertices[3].y  = b / 2.0f;
    submesh->pImpl->pVertices[3].z  = -c / 2.0f;
    submesh->pImpl->pNormals[3].x   = 0.0f;
    submesh->pImpl->pNormals[3].y   = 0.0f;
    submesh->pImpl->pNormals[3].z   = -1.0f;
    submesh->pImpl->pMaterial->getColors()[3].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[3].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[3].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[3].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[3].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[3].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[3].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->pImpl->pFaces[0].vertex_ids[i] = i;
        submesh->pImpl->pFaces[0].texcoord_ids[i] = i;
        submesh->pImpl->pFaces[0].normal_ids[i] = i;
    }*/

    // back face
    submesh->pImpl->pVertices[4].x  = a / 2.0f;
    submesh->pImpl->pVertices[4].y  = -b / 2.0f;
    submesh->pImpl->pVertices[4].z  = c / 2.0f;
    submesh->pImpl->pNormals[4].x   = 0.0f;
    submesh->pImpl->pNormals[4].y   = 0.0f;
    submesh->pImpl->pNormals[4].z   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[4].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[4].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[4].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[4].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[4].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[4].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[4].w = 0.0f;

    submesh->pImpl->pVertices[5].x  = -a / 2.0f;
    submesh->pImpl->pVertices[5].y  = -b / 2.0f;
    submesh->pImpl->pVertices[5].z  = c / 2.0f;
    submesh->pImpl->pNormals[5].x   = 0.0f;
    submesh->pImpl->pNormals[5].y   = 0.0f;
    submesh->pImpl->pNormals[5].z   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[5].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[5].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[5].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[5].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[5].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[5].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[5].w = 0.0f;

    submesh->pImpl->pVertices[6].x  = -a / 2.0f;
    submesh->pImpl->pVertices[6].y  = b / 2.0f;
    submesh->pImpl->pVertices[6].z  = c / 2.0f;
    submesh->pImpl->pNormals[6].x   = 0.0f;
    submesh->pImpl->pNormals[6].y   = 0.0f;
    submesh->pImpl->pNormals[6].z   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[6].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[6].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[6].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[6].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[6].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[6].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[6].w = 0.0f;

    submesh->pImpl->pVertices[7].x  = a / 2.0f;
    submesh->pImpl->pVertices[7].y  = b / 2.0f;
    submesh->pImpl->pVertices[7].z  = c / 2.0f;
    submesh->pImpl->pNormals[7].x   = 0.0f;
    submesh->pImpl->pNormals[7].y   = 0.0f;
    submesh->pImpl->pNormals[7].z   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[7].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[7].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[7].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[7].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[7].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[7].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[7].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->pImpl->pFaces[1].vertex_ids[i] = 4+i;
        submesh->pImpl->pFaces[1].texcoord_ids[i] = 4+i;
        submesh->pImpl->pFaces[1].normal_ids[i] = 4+i;
    }*/

    // left face
    submesh->pImpl->pVertices[8].x  = -a / 2.0f;
    submesh->pImpl->pVertices[8].y  = -b / 2.0f;
    submesh->pImpl->pVertices[8].z  = c / 2.0f;
    submesh->pImpl->pNormals[8].x   = -1.0f;
    submesh->pImpl->pNormals[8].y   = 0.0f;
    submesh->pImpl->pNormals[8].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[8].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[8].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[8].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[8].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[8].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[8].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[8].w = 0.0f;

    submesh->pImpl->pVertices[9].x  = -a / 2.0f;
    submesh->pImpl->pVertices[9].y  = -b / 2.0f;
    submesh->pImpl->pVertices[9].z  = -c / 2.0f;
    submesh->pImpl->pNormals[9].x   = -1.0f;
    submesh->pImpl->pNormals[9].y   = 0.0f;
    submesh->pImpl->pNormals[9].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[9].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[9].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[9].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[9].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[9].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[9].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[9].w = 0.0f;

    submesh->pImpl->pVertices[10].x  = -a / 2.0f;
    submesh->pImpl->pVertices[10].y  = b / 2.0f;
    submesh->pImpl->pVertices[10].z  = -c / 2.0f;
    submesh->pImpl->pNormals[10].x   = -1.0f;
    submesh->pImpl->pNormals[10].y   = 0.0f;
    submesh->pImpl->pNormals[10].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[10].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[10].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[10].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[10].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[10].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[10].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[10].w = 0.0f;

    submesh->pImpl->pVertices[11].x  = -a / 2.0f;
    submesh->pImpl->pVertices[11].y  = b / 2.0f;
    submesh->pImpl->pVertices[11].z  = c / 2.0f;
    submesh->pImpl->pNormals[11].x   = -1.0f;
    submesh->pImpl->pNormals[11].y   = 0.0f;
    submesh->pImpl->pNormals[11].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[11].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[11].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[11].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[11].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[11].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[11].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[11].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->pImpl->pFaces[2].vertex_ids[i] = 8+i;
        submesh->pImpl->pFaces[2].texcoord_ids[i] = 8+i;
        submesh->pImpl->pFaces[2].normal_ids[i] = 8+i;
    }*/

    // right face
    submesh->pImpl->pVertices[12].x  = a / 2.0f;
    submesh->pImpl->pVertices[12].y  = -b / 2.0f;
    submesh->pImpl->pVertices[12].z  = -c / 2.0f;
    submesh->pImpl->pNormals[12].x   = 1.0f;
    submesh->pImpl->pNormals[12].y   = 0.0f;
    submesh->pImpl->pNormals[12].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[12].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[12].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[12].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[12].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[12].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[12].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[12].w = 0.0f;

    submesh->pImpl->pVertices[13].x  = a / 2.0f;
    submesh->pImpl->pVertices[13].y  = -b / 2.0f;
    submesh->pImpl->pVertices[13].z  = c / 2.0f;
    submesh->pImpl->pNormals[13].x   = 1.0f;
    submesh->pImpl->pNormals[13].y   = 0.0f;
    submesh->pImpl->pNormals[13].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[13].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[13].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[13].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[13].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[13].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[13].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[13].w = 0.0f;

    submesh->pImpl->pVertices[14].x  = a / 2.0f;
    submesh->pImpl->pVertices[14].y  = b / 2.0f;
    submesh->pImpl->pVertices[14].z  = c / 2.0f;
    submesh->pImpl->pNormals[14].x   = 1.0f;
    submesh->pImpl->pNormals[14].y   = 0.0f;
    submesh->pImpl->pNormals[14].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[14].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[14].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[14].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[14].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[14].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[14].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[14].w = 0.0f;

    submesh->pImpl->pVertices[15].x  = a / 2.0f;
    submesh->pImpl->pVertices[15].y  = b / 2.0f;
    submesh->pImpl->pVertices[15].z  = -c / 2.0f;
    submesh->pImpl->pNormals[15].x   = 1.0f;
    submesh->pImpl->pNormals[15].y   = 0.0f;
    submesh->pImpl->pNormals[15].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[15].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[15].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[15].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[15].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[15].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[15].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[15].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->pImpl->pFaces[3].vertex_ids[i] = 12+i;
        submesh->pImpl->pFaces[3].texcoord_ids[i] = 12+i;
        submesh->pImpl->pFaces[3].normal_ids[i] = 12+i;
    }*/

    // top face
    submesh->pImpl->pVertices[16].x  = -a / 2.0f;
    submesh->pImpl->pVertices[16].y  = b / 2.0f;
    submesh->pImpl->pVertices[16].z  = -c / 2.0f;
    submesh->pImpl->pNormals[16].x   = 0.0f;
    submesh->pImpl->pNormals[16].y   = 1.0f;
    submesh->pImpl->pNormals[16].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[16].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[16].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[16].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[16].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[16].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[16].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[16].w = 0.0f;

    submesh->pImpl->pVertices[17].x  = a / 2.0f;
    submesh->pImpl->pVertices[17].y  = b / 2.0f;
    submesh->pImpl->pVertices[17].z  = -c / 2.0f;
    submesh->pImpl->pNormals[17].x   = 0.0f;
    submesh->pImpl->pNormals[17].y   = 1.0f;
    submesh->pImpl->pNormals[17].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[17].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[17].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[17].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[17].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[17].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[17].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[17].w = 0.0f;

    submesh->pImpl->pVertices[18].x  = a / 2.0f;
    submesh->pImpl->pVertices[18].y  = b / 2.0f;
    submesh->pImpl->pVertices[18].z  = c / 2.0f;
    submesh->pImpl->pNormals[18].x   = 0.0f;
    submesh->pImpl->pNormals[18].y   = 1.0f;
    submesh->pImpl->pNormals[18].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[18].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[18].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[18].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[18].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[18].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[18].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[18].w = 0.0f;

    submesh->pImpl->pVertices[19].x  = -a / 2.0f;
    submesh->pImpl->pVertices[19].y  = b / 2.0f;
    submesh->pImpl->pVertices[19].z  = c / 2.0f;
    submesh->pImpl->pNormals[19].x   = 0.0f;
    submesh->pImpl->pNormals[19].y   = 1.0f;
    submesh->pImpl->pNormals[19].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[19].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[19].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[19].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[19].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[19].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[19].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[19].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->pImpl->pFaces[4].vertex_ids[i] = 16+i;
        submesh->pImpl->pFaces[4].texcoord_ids[i] = 16+i;
        submesh->pImpl->pFaces[4].normal_ids[i] = 16+i;
    }*/

    // bottom face
    submesh->pImpl->pVertices[20].x  = a / 2.0f;
    submesh->pImpl->pVertices[20].y  = -b / 2.0f;
    submesh->pImpl->pVertices[20].z  = -c / 2.0f;
    submesh->pImpl->pNormals[20].x   = 0.0f;
    submesh->pImpl->pNormals[20].y   = -1.0f;
    submesh->pImpl->pNormals[20].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[20].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[20].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[20].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[20].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[20].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[20].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[20].w = 0.0f;

    submesh->pImpl->pVertices[21].x  = -a / 2.0f;
    submesh->pImpl->pVertices[21].y  = -b / 2.0f;
    submesh->pImpl->pVertices[21].z  = -c / 2.0f;
    submesh->pImpl->pNormals[21].x   = 0.0f;
    submesh->pImpl->pNormals[21].y   = -1.0f;
    submesh->pImpl->pNormals[21].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[21].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[21].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[21].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[21].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[21].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[21].v = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[21].w = 0.0f;

    submesh->pImpl->pVertices[22].x  = -a / 2.0f;
    submesh->pImpl->pVertices[22].y  = -b / 2.0f;
    submesh->pImpl->pVertices[22].z  = c / 2.0f;
    submesh->pImpl->pNormals[22].x   = 0.0f;
    submesh->pImpl->pNormals[22].y   = -1.0f;
    submesh->pImpl->pNormals[22].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[22].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[22].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[22].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[22].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[22].u = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[22].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[22].w = 0.0f;

    submesh->pImpl->pVertices[23].x  = a / 2.0f;
    submesh->pImpl->pVertices[23].y  = -b / 2.0f;
    submesh->pImpl->pVertices[23].z  = c / 2.0f;
    submesh->pImpl->pNormals[23].x   = 0.0f;
    submesh->pImpl->pNormals[23].y   = -1.0f;
    submesh->pImpl->pNormals[23].z   = 0.0f;
    submesh->pImpl->pMaterial->getColors()[23].red    = 1.0f;
    submesh->pImpl->pMaterial->getColors()[23].green  = 1.0f;
    submesh->pImpl->pMaterial->getColors()[23].blue   = 1.0f;
    submesh->pImpl->pMaterial->getColors()[23].alpha  = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[23].u = 0.0f;
    submesh->pImpl->pMaterial->getTexcoords()[23].v = 1.0f;
    submesh->pImpl->pMaterial->getTexcoords()[23].w = 0.0f;

    /*for (int i = 0; i < 4; i++)
    {
        submesh->pImpl->pFaces[5].vertex_ids[i] = 20+i;
        submesh->pImpl->pFaces[5].texcoord_ids[i] = 20+i;
        submesh->pImpl->pFaces[5].normal_ids[i] = 20+i;
    }*/

    for (TPureUInt i = 0; i < submesh->pImpl->nVertexIndices_h; i++)
    {
        if ( submesh->pImpl->nMinVertexIndex > i )
            submesh->pImpl->nMinVertexIndex = i;
        if ( submesh->pImpl->nMaxVertexIndex < i )
            submesh->pImpl->nMaxVertexIndex = i;
        bool res = submesh->pImpl->setVertexIndex(i, i);
        assert( res );
    }
    return true;
} // convertToBox()


/**
    Creates a material for the given Mesh if it doesn't yet have one.
    Won't create new material if the given Mesh already has a material.

    @param mesh The mesh for we want to create a material.
    
    @return The created or already existing material. PGENULL in case of error.
*/
PureMaterial* PureMesh3DManager::PureMesh3DManagerImpl::createMaterialForMesh(PureMesh3D& mesh) const 
{
    if ( mesh.pImpl->pMaterial == PGENULL )
    {
        mesh.pImpl->pMaterial = materialMgr.createMaterial();
        if ( mesh.pImpl->pMaterial == PGENULL )
        {
            _pOwner->getConsole().EOLn("ERROR: PureMesh3DManager::createMaterialForMesh(), returned material is NULL!");
        }
        else
        {
            mesh.pImpl->pMaterial->SetUtiliser(&mesh);
            _pOwner->getConsole().SOLn("PureMesh3DManager::createMaterialForMesh() created material!");
        }
    }
    else
    {
        _pOwner->getConsole().OLn("PureMesh3DManager::createMaterialForMesh(), mesh already has material!");
    }

    return mesh.pImpl->pMaterial;
} // createMaterialForMesh()


// ############################## PROTECTED ##############################


/**
    Loads OBJ files, handles the actual file operations, and creates the Mesh3D object.

    @param filename The model file to be loaded to be an Mesh3D instance.

    @return The created Mesh3D object on success, PGENULL otherwise.
*/
PureMesh3D* PureMesh3DManager::PureMesh3DManagerImpl::loadOBJ(const char* filename)
{
    const HANDLE objfile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);

    DWORD     bytesread = 0;
    TPureUInt lines_h = 0;
    DWORD     filebuffer_size = 0;
    DWORD     nLastError;

    _pOwner->getConsole().OLnOI("PureMesh3DManager::Load_OBJ(""%s"")", filename);
    if ( objfile == INVALID_HANDLE_VALUE )
    {
        _pOwner->getConsole().EOLnOO("ERROR: objfile == INVALID_HANDLE_VALUE, returning NULL");
        return PGENULL;
    }

    filebuffer_size = GetFileSize(objfile, NULL);
    if ( (filebuffer_size == 0) || (filebuffer_size == INVALID_FILE_SIZE) )
    {
        nLastError = GetLastError();
        _pOwner->getConsole().EOLnOO("ERROR: filebuffer_size == %d, nLastError == %d, returning NULL", filebuffer_size, nLastError);
        // Note that nLastError can still be NO_ERROR. In that case it means that file size is actually INVALID_FILE_SIZE (0xFFFFFFFF) or
        // larger (for that case the lpFileSizeHigh param could be used or GetFileSizeEx()). In any case, we don't want to proceed
        // further, as we don't want to try malloc 0xFFFFFFFF or larger memory area. No model files should reach this size, or
        // a different approach is needed to read the whole file.
        CloseHandle(objfile);
        return PGENULL;
    }

    char* filebuffer = NULL;
    try
    {
        filebuffer = new char[filebuffer_size];
        ReadFile(objfile, filebuffer, filebuffer_size, &bytesread, NULL);
        CloseHandle(objfile);
        if ( bytesread != filebuffer_size )
        {
            _pOwner->getConsole().EOLnOO("ERROR: %d != %d (filebuffer_size != bytesread), returning NULL", filebuffer_size, bytesread);
            delete[] filebuffer;
            return PGENULL;
        }
    }
    catch (const std::bad_alloc&)
    {
        CloseHandle(objfile);
        _pOwner->getConsole().EOLn("  ERROR: failed to allocate filebuffer!");
        return PGENULL;
    }

    _pOwner->getConsole().OLn("File read into buffer (%d bytes) and closed, preprocessing file ...", filebuffer_size);
    lines_h = PFL::numCharAppears(10, filebuffer, filebuffer_size) + 1; // total lines in file
    _pOwner->getConsole().OLn("lines_h = %d", lines_h);

    const char** lines = NULL;
    try
    {
        // each ptr in lines[] will be set to point to the beginning of lines in filebuffer, thus filebuffer is also needed to be kept in memory!
        lines = new const char*[lines_h]();  // replacing calloc(), this kind of usage of operator new is value-initialization, by using (), will take care of zeroing in this case!
        char* pLineEnd = filebuffer;
        char* pPrevLineEnd;
        TPureUInt actline = 0;
        do
        {                                                                            
            if ( pLineEnd == filebuffer )
            {
                pPrevLineEnd = pLineEnd;
                pLineEnd = pLineEnd-1;
            }
            else
            {
                pPrevLineEnd = pLineEnd;
            }
            // finding line end (LF char)
            pLineEnd = (char*) memchr(pLineEnd+1, 10, filebuffer_size-((DWORD)(pLineEnd-filebuffer)));
                        
            if ( (pLineEnd != NULL) && (pPrevLineEnd != NULL) )
            {    // found a new line, allocating mem for it and put it in
                size_t lineLength = pLineEnd - pPrevLineEnd;
                if ( lineLength > 0 )
                {
                    if ( lineLength > 1 )
                    {
                        // for some reason, after pushing some .obj files to github, all lines have a \r char added!
                        // realized that probably this was done at checkout by git.
                        // we need to get rid of them.
                        // this is controlled by global git setting core.autocrlf, which is probably true by default.
                        if ( *(pLineEnd-1) == '\r' )
                        {
                            // optionally there can be CR character too, that should be the new endline
                            --lineLength;
                        }
                    }
                    
                    if ( pPrevLineEnd == filebuffer )
                    {   // if this is the 1st line
                        lines[actline] = pPrevLineEnd;
                        *(pPrevLineEnd+lineLength) = '\0';
                    }
                    else
                    {
                        lines[actline] = pPrevLineEnd+1;
                        *(pPrevLineEnd+lineLength) = '\0';
                    }
                }
                actline++;
            }
            else
            {    // no more endline chars but chars after the last endline char are still needed
                if ( (DWORD) (pPrevLineEnd - filebuffer) < filebuffer_size )
                {
                    lines[actline] = pPrevLineEnd;
                    *(filebuffer+filebuffer_size-1) = '\0';
                }
            }
        } while (pLineEnd != NULL);
    } // try
    catch (const std::bad_alloc&)
    {
        delete[] lines;
        delete[] filebuffer;
        _pOwner->getConsole().EOLn("  ERROR: failed to allocate lines array!");
        return NULL;
    }

    _pOwner->getConsole().OLn("preprocessing done, gathering data ...");
    
    PureMesh3D* obj = NULL;
    TPureUInt submeshes_h = 0;
    TXYZ** tmpSubMeshesVertices  = NULL;
    TUVW** tmpSubMeshesTexcoords = NULL;
    TXYZ** tmpSubMeshesNormals   = NULL;
    TPureUInt* tmpSubMeshesVertices_h  = NULL;
    TPureUInt* tmpSubMeshesTexcoords_h = NULL;
    TPureUInt* tmpSubMeshesNormals_h   = NULL;

    TPureUInt* lines_start = NULL;
    TPureUInt* lines_end = NULL;
    try
    {
        _pOwner->getConsole().OLnOI("creating parent object ...");
        obj = new PureMesh3D(PURE_PM_TRIANGLES);
        createMaterialForMesh(*obj);
        if ( obj->pImpl->pMaterial == PGENULL )
        {
            // this should not be like this, createMaterialForMesh() should throw.
            throw std::bad_alloc();
        }
        _pOwner->getConsole().OLnOO("parent object created!");

        _pOwner->getConsole().OLnOI("creating subobjects ...");
        // counting submeshes
        for (TPureUInt i = 0; i < lines_h; i++)
        {
            if ( memchr(lines[i], '#', strlen((char*)lines[i])) == 0 )
            {
                if ( strchr((char*) lines[i],'g') > 0  )
                    submeshes_h++;
            }
        }
        submeshes_h = submeshes_h / 2; // number of 'g'-s are twice as the number of submeshes
        obj->PreAlloc(submeshes_h);
        for (TPureUInt i = 0; i < submeshes_h; i++)
        {
            _pOwner->getConsole().OI();
            PureMesh3D* subobj = new PureMesh3D(PURE_PM_TRIANGLES);
            createMaterialForMesh(*subobj);
            if ( subobj->pImpl->pMaterial == PGENULL )
            {
                // this should not be like this, createMaterialForMesh() should throw.
                throw std::bad_alloc();
            }
            _pOwner->getConsole().OO();
            obj->Attach(*subobj);
        }
        _pOwner->getConsole().OLn("submesh count = %d", submeshes_h);
        _pOwner->getConsole().OLnOO("subobjects created!");

        // We read distinct vertices, texcoords and normals data into tmp arrays.
        // At the end, we fill the subObjects' vertices, texcoords and normals arrays with redundant
        // data based on the temporary arrays, so both direct referencing and indexing modes will be available.
        // Redundant storage is essential for indexed vertex arrays (element arrays). Indexing in non-redundant
        // format is only ok when multiple index arrays could be used for the same mesh: 1 for vertices, 1 for texcoords, etc.
        // But when only 1 element array can be used for all of these, like glDrawElements(), these must be stored
        // redundantly without implementing extra logic.
        // Update: actually we could still use only 1 element array with any glDrawElements()-style command without
        // much redundancy in vertex data, but we need to fill that element array in a smart way, considering that
        // OBJ format still uses different indices for vertex position, normals, and texture coordinates. We need to
        // unify these into 1 index array, and to avoid redundancy, we need to check if the specified vertex data
        // is new data or we already encountered such, if so, we can use same index, no need to put same vertex data
        // into data array. But this requires some extra logic. If this gets solved, huge speedup is expected with
        // indexed referencing compared to direct referencing because shared vertices will be actually shared.
        // Note that, if we reach this and minimize redundancy, direct referencing wont be available.
        // Also, this can be taken to a next level, by analysing the vertex indices used by the faces, and then
        // the faces could be ordered by the referenced vertices, so that faces using shared vertices will be
        // rendered after each other, benefiting from vertex cache.
        tmpSubMeshesVertices  = new TXYZ*[submeshes_h](); // using () to intentionally zeroing out pointers for easier exception handling
        tmpSubMeshesTexcoords = new TUVW*[submeshes_h]();
        tmpSubMeshesNormals   = new TXYZ*[submeshes_h]();
        tmpSubMeshesVertices_h  = new TPureUInt[submeshes_h];
        tmpSubMeshesTexcoords_h = new TPureUInt[submeshes_h];
        tmpSubMeshesNormals_h   = new TPureUInt[submeshes_h];

        lines_start = new TPureUInt[submeshes_h];
        lines_end = new TPureUInt[submeshes_h];

        TPureUInt k;
        bool sector = false;
        TPureUInt actsubmesh = 0;
        int numparsed = 0;
    
        // finding starting and ending lines of vertices for every submesh
        _pOwner->getConsole().OLnOI("parsing vertices ...");
        for (TPureUInt i = 0; i < lines_h; i++)
        {
            if ( (strstr((char*) lines[i],"v") > 0) && (strstr((char*) lines[i],"#") == 0) && (strstr((char*) lines[i],"vt") == 0) && (strstr((char*) lines[i],"vn") == 0) && (strstr((char*) lines[i],"g") == 0))
            {
                if ( !sector )
                {
                    sector = true;
                    lines_start[actsubmesh] = i;
                }
            }
            else
            {
                if ( sector )
                {
                    sector = false;
                    lines_end[actsubmesh] = i-1;
                    actsubmesh++;
                }
            }
        }
                    
        // parsing vertices
        for (TPureUInt i = 0; i < submeshes_h; i++)
        {            
            tmpSubMeshesVertices_h[i] = lines_end[i] - lines_start[i] + 1;
            tmpSubMeshesVertices[i] = new TXYZ[tmpSubMeshesVertices_h[i]];
            _pOwner->getConsole().OLn("tmpSubMeshesVertices_h[%d] == %d", i, tmpSubMeshesVertices_h[i]);
            k = 0;
            for (TPureUInt j = lines_start[i]; j <= lines_end[i]; j++)
            {
                numparsed = sscanf((char*)lines[j], "%*s %f %f %f", &(tmpSubMeshesVertices[i][k].x), &(tmpSubMeshesVertices[i][k].y), &(tmpSubMeshesVertices[i][k].z));
                if ( numparsed != 3 )
                {
                    _pOwner->getConsole().EOLn("ERROR: sscanf() failed @ vertices section: numparsed == %d", numparsed);
                    _pOwner->getConsole().OLn("  @ lines[%d]: '%s'", j, lines[j]);
                }    
                k++;
            }
        }
        _pOwner->getConsole().OLnOO("vertices parsed!");

        // finding starting and ending lines of texcoords for every submesh
        _pOwner->getConsole().OLnOI("parsing texcoords ...");
        sector = false;
        actsubmesh = 0;
        for (TPureUInt i = 0; i < lines_h; i++)
        {
            if ( (strstr((char*) lines[i], "vt") > 0) && (strstr((char*) lines[i], "#") == 0) )
            {
                if ( !sector )
                {
                    sector = true;
                    lines_start[actsubmesh] = i;
                }
            }
            else
            {
                if ( sector )
                {
                    sector = false;
                    lines_end[actsubmesh] = i-1;
                    actsubmesh++;
                }
            }
        }
                    
        k = 0;
        // parsing texcoords
        for (TPureUInt i = 0; i < submeshes_h; i++)
        {              
            tmpSubMeshesTexcoords_h[i] = lines_end[i] - lines_start[i] + 1;
            tmpSubMeshesTexcoords[i] = new TUVW[tmpSubMeshesTexcoords_h[i]];
            _pOwner->getConsole().OLn("tmpSubMeshesTexcoords_h[%d] == %d", i, tmpSubMeshesTexcoords_h[i]);
            k = 0;
            for (TPureUInt j = lines_start[i]; j <= lines_end[i]; j++)
            {
                numparsed = sscanf((char*)lines[j], "%*s %f %f %f", &(tmpSubMeshesTexcoords[i][k].u), &(tmpSubMeshesTexcoords[i][k].v), &(tmpSubMeshesTexcoords[i][k].w));
                if ( numparsed != 3 )
                {
                    _pOwner->getConsole().EOLn("ERROR: sscanf() failed @ texcoords section: numparsed == %d", numparsed);
                    _pOwner->getConsole().OLn("  @ lines[%d]: '%s'", j, lines[j]);
                }    
                k++;
            }
        }
        _pOwner->getConsole().OLnOO("texcoords parsed!");

        // finding starting and ending lines of normals for every submesh
        _pOwner->getConsole().OLnOI("parsing normals ...");
        sector = false;
        actsubmesh = 0;
        for (TPureUInt i = 0; i < lines_h; i++)
        {
            if ( (strstr((char*) lines[i], "vn") > 0) && (strstr((char*) lines[i], "#") == 0) )
            {
                if ( !sector )
                {
                    sector = true;
                    lines_start[actsubmesh] = i;
                }
            }
            else
            {
                if ( sector )
                {
                    sector = false;
                    lines_end[actsubmesh] = i-1;
                    actsubmesh++;
                }
            }
        }

        // parsing normals
        for (TPureUInt i = 0; i < submeshes_h; i++)
        {           
            tmpSubMeshesNormals_h[i] = lines_end[i] - lines_start[i] + 1;
            tmpSubMeshesNormals[i] = new TXYZ[tmpSubMeshesNormals_h[i]];
            _pOwner->getConsole().OLn("tmpSubMeshesNormals_h[%d] == %d", i, tmpSubMeshesNormals_h[i]);
            k = 0;
            for (TPureUInt j = lines_start[i]; j <= lines_end[i]; j++)
            {
                numparsed = sscanf((char*)lines[j], "%*s %f %f %f", &(tmpSubMeshesNormals[i][k].x), &(tmpSubMeshesNormals[i][k].y), &(tmpSubMeshesNormals[i][k].z));
                if ( numparsed != 3 )
                {
                    _pOwner->getConsole().EOLn("ERROR: sscanf() failed @ normals section: numparsed == %d", numparsed);
                    _pOwner->getConsole().OLn("  @ lines[%d]: '%s'", j, lines[j]);
                }    
                k++;
            }
        }
        _pOwner->getConsole().OLnOO("normals parsed!");

        // finding starting and ending lines of faces for every submesh
        _pOwner->getConsole().OLnOI("parsing faces ...");
        sector = false;
        actsubmesh = 0;
        for (TPureUInt i = 0; i < lines_h; i++)
        {
            if ( (strstr((char*) lines[i], "f") > 0) && (strstr((char*) lines[i], "#") == 0) && ((strstr((char*) lines[i], "g ") == 0)) )
            {
                if ( !sector )
                {
                    sector = true;
                    lines_start[actsubmesh] = i;
                }
            }
            else
            {
                if ( sector )
                {
                    sector = false;
                    lines_end[actsubmesh] = i-1;
                    actsubmesh++;
                }
            }
        }

        // we have to determine min and max vertex indices here, prior to allocating arrays
        for (TPureUInt i = 0; i < submeshes_h; i++)
        {                    
            PureMesh3D* const currSubObj = (PureMesh3D*) (obj->getAttachedAt(i));
            k = 0;
            // todo: later this loop can be replaced by a simple assignment like: k = lines_end[i] - lines_start[i] +/-1 or sg like that
            for (TPureUInt j = lines_start[i]; j <= lines_end[i]; j++)
            {
                // there will be submesh-local indices in the end, all starting from nMinVertexIndex = 0 to nMaxVertexIndex = k*3+2
                // TODO: since we use submesh-local indices, there is no use for nMinVertexIndex, as it must be always 0 in all submeshes!
                // also, I'm not sure about the use of nMaxVertexIndex, since I guess it always equals to nVertexIndices_h-1.
                // So, I would rather decide nIndicesType based on the value nVertexIndices_h.
                if ( currSubObj->pImpl->nMinVertexIndex > k*3 )
                    currSubObj->pImpl->nMinVertexIndex = k*3;
                if ( currSubObj->pImpl->nMaxVertexIndex < k*3+2 )
                    currSubObj->pImpl->nMaxVertexIndex = k*3+2;
                k++;
            }
            // determining what type of values to be stored in the index arrays ...
            if ( currSubObj->pImpl->nMaxVertexIndex <= UCHAR_MAX )
            {
                currSubObj->pImpl->nIndicesType = GL_UNSIGNED_BYTE;
                _pOwner->getConsole().OLn("submeshes_h[%d] nIndicesType = GL_UNSIGNED_BYTE", i);
            }
            else if ( currSubObj->pImpl->nMaxVertexIndex <= USHRT_MAX )
            {
                currSubObj->pImpl->nIndicesType = GL_UNSIGNED_SHORT;
                _pOwner->getConsole().OLn("submeshes_h[%d] nIndicesType = GL_UNSIGNED_SHORT", i);
            }
            else
            {
                currSubObj->pImpl->nIndicesType = GL_UNSIGNED_INT;
                _pOwner->getConsole().OLn("submeshes_h[%d] nIndicesType = GL_UNSIGNED_INT", i);
            }
            if ( currSubObj->pImpl->nMinVertexIndex != 0 )
            {
                _pOwner->getConsole().EOLn("ERROR: submeshes_h[%d] nMinVertexIndex == %d", i, currSubObj->pImpl->nMinVertexIndex);
                _ASSERT( false );
            }
        }

        // parsing faces
        int prevvertcount = 1;
        int prevnormcount = 1;
        int prevtexcoordcount = 1;
        for (TPureUInt i = 0; i < submeshes_h; i++)
        {                    
            PureMesh3D* const currSubObj = (PureMesh3D*) (obj->getAttachedAt(i));
            currSubObj->pImpl->nFaces_h = lines_end[i] - lines_start[i] + 1;
            // currSubObj->pImpl->pFaces = (TFACE4*) malloc( sizeof(TFACE4) * currSubObj->pImpl->nFaces_h );
            currSubObj->pImpl->nVertexIndices_h = currSubObj->pImpl->nFaces_h * 3;
            currSubObj->pImpl->pVertexIndices = new char[PureGLsnippets::getSizeofIndexType(currSubObj->pImpl->nIndicesType) * currSubObj->pImpl->nVertexIndices_h];
            currSubObj->pImpl->nVertices_h = currSubObj->pImpl->nVertexIndices_h;
            currSubObj->pImpl->pVertices = new TXYZ[currSubObj->pImpl->nVertices_h];
            currSubObj->pImpl->pNormals  = new TXYZ[currSubObj->pImpl->nVertices_h];

            if ( !currSubObj->pImpl->pMaterial->allocateArrays(
                   currSubObj->pImpl->nVertices_h,
                   currSubObj->pImpl->nVertexIndices_h,
                   currSubObj->pImpl->nVertexIndices_h,
                   PureGLsnippets::getSizeofIndexType(currSubObj->pImpl->nIndicesType) ) )
            {
                // not nice but now this is the quickest way of error handling, on the long run allocateArrays() should throw
                _pOwner->getConsole().EOLn("ERROR: currSubObj->pImpl->pMaterial->allocateArrays() failed!");
                throw std::bad_alloc();
            }

            _pOwner->getConsole().OLn("obj->subObjects[%d]->nFaces_h == %d", i, currSubObj->pImpl->nFaces_h);
            k = 0;
            // parsing faces of a submesh
            for (TPureUInt j = lines_start[i]; j <= lines_end[i]; j++)
            {
                TPureUInt tmpParseVertexIndices[3];
                TPureUInt tmpParseTexcoordIndices[3];
                TPureUInt tmpParseNormalIndices[3];
                numparsed = sscanf((char*)lines[j], "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
                                    &(tmpParseVertexIndices[0]),
                                    &(tmpParseTexcoordIndices[0]),
                                    &(tmpParseNormalIndices[0]),
                                    &(tmpParseVertexIndices[1]),
                                    &(tmpParseTexcoordIndices[1]),
                                    &(tmpParseNormalIndices[1]),
                                    &(tmpParseVertexIndices[2]),
                                    &(tmpParseTexcoordIndices[2]),
                                    &(tmpParseNormalIndices[2]));
                if ( numparsed != 9 )
                {
                    _pOwner->getConsole().EOLn("ERROR: sscanf() failed @ faces section: numparsed == %d", numparsed);
                    _pOwner->getConsole().OLn("  @ lines[%d]: '%s'", j, lines[j]);
                }    

                // Putting parsed indices into index arrays.
                // We decrease the parsed index values by the previously accumulated index values because OBJ stores global indices but we need submesh-local indices.
                // setVertexIndex() stores indices with minimal storage size, eg. 1 as byte, 500 as short, so passing submesh-local indices here is CRITICAL to avoid chopping values.
                /* Note: exchange k*3 and k*3+2 for CW-CCW change. This maybe a feature in the future. */
                TPureBool setVertexIndicesOk = true;
                setVertexIndicesOk &= currSubObj->pImpl->setVertexIndex(k*3+2, tmpParseVertexIndices[0] - prevvertcount);
                setVertexIndicesOk &= currSubObj->pImpl->setVertexIndex(k*3+1, tmpParseVertexIndices[1] - prevvertcount);
                setVertexIndicesOk &= currSubObj->pImpl->setVertexIndex(k*3,   tmpParseVertexIndices[2] - prevvertcount);

                // Putting geometry data into arrays. We store geometry data redundantly.
                currSubObj->pImpl->pVertices[k*3]    = tmpSubMeshesVertices[i][ currSubObj->pImpl->getVertexIndex(k*3) ];
                currSubObj->pImpl->pVertices[k*3+1]  = tmpSubMeshesVertices[i][ currSubObj->pImpl->getVertexIndex(k*3+1) ];
                currSubObj->pImpl->pVertices[k*3+2]  = tmpSubMeshesVertices[i][ currSubObj->pImpl->getVertexIndex(k*3+2) ];
                // don't need to fill pVerticesTransf array here, that task is for during rendering!
                currSubObj->pImpl->pMaterial->getTexcoords()[k*3]   = tmpSubMeshesTexcoords[i][ tmpParseTexcoordIndices[2] - prevtexcoordcount ];
                currSubObj->pImpl->pMaterial->getTexcoords()[k*3+1] = tmpSubMeshesTexcoords[i][ tmpParseTexcoordIndices[1] - prevtexcoordcount ];
                currSubObj->pImpl->pMaterial->getTexcoords()[k*3+2] = tmpSubMeshesTexcoords[i][ tmpParseTexcoordIndices[0] - prevtexcoordcount ];

                // Update the index values because we now store geometry data redundantly.
                setVertexIndicesOk &= currSubObj->pImpl->setVertexIndex(k*3,   k*3);
                setVertexIndicesOk &= currSubObj->pImpl->setVertexIndex(k*3+1, k*3+1);
                setVertexIndicesOk &= currSubObj->pImpl->setVertexIndex(k*3+2, k*3+2);

                if ( !setVertexIndicesOk )
                {
                    const std::string sErrMsg = "setVertexIndicesOk false at submesh " + std::to_string(i) + ", line " + std::to_string(j) + ", k " + std::to_string(k);
                    throw std::range_error(sErrMsg);
                }

                k++;
            } 
            prevvertcount += tmpSubMeshesVertices_h[i];
            prevnormcount += tmpSubMeshesNormals_h[i];
            prevtexcoordcount += tmpSubMeshesTexcoords_h[i];
        }
        _pOwner->getConsole().OLnOO("faces parsed!");
    } // try
    catch (const std::exception& e)
    {
        for (TPureUInt i = 0; i < submeshes_h; i++)
        {
            if ( tmpSubMeshesVertices != NULL )
                delete[] tmpSubMeshesVertices[i];

            if ( tmpSubMeshesTexcoords != NULL )
                delete[] tmpSubMeshesTexcoords[i];

            if ( tmpSubMeshesNormals != NULL )
                delete[] tmpSubMeshesNormals[i];
        }
        delete obj; // this takes care of submesh geometry and materials too!
        delete[] tmpSubMeshesVertices;
        delete[] tmpSubMeshesTexcoords;
        delete[] tmpSubMeshesNormals;
        delete[] tmpSubMeshesVertices_h;
        delete[] tmpSubMeshesTexcoords_h;
        delete[] tmpSubMeshesNormals_h;

        delete[] lines_start;
        delete[] lines_end;

        delete[] lines;
        delete[] filebuffer;

        _pOwner->getConsole().EOLnOO("ERROR: submesh geometry arrays buildup issue: %s!", e.what());
        return NULL;
    } // catch

    _pOwner->getConsole().OLnOI("submodel names ...");
    TPureUInt isubmsh = 0;
    bool isname = true;
    for (TPureUInt i = 0; i < lines_h; i++)
    {
        if ( memchr(lines[i],'#', strlen((char*)lines[i])) == 0 )
        {
            if ( strchr((char*) lines[i],'g') > 0  )
            {
                if ( isname )
                {
                    const char* const pSubModelName = lines[i] + 2;
                    PureMesh3D* const pSubObj = (PureMesh3D*)(obj->getAttachedAt(isubmsh));
                    pSubObj->SetName(pSubModelName);
                    _pOwner->getConsole().OLn("subObjects[%d]->getName() = %s", isubmsh, pSubObj->getName().c_str());
                    isubmsh++;
                }
                isname = !isname;         
            }
        }        
    }
    _pOwner->getConsole().OLnOO("submodel names set!");

    _pOwner->getConsole().OLnOI("submodel material settings ...");
    for (TPureUInt i = 0; i < submeshes_h; i++)
    {
        PureMesh3D* const currSubObj = (PureMesh3D*) (obj->getAttachedAt(i));
        for (TPureUInt j = 0; j < currSubObj->pImpl->pMaterial->getColorsCount(); j++)
        {
            currSubObj->pImpl->pMaterial->getColors()[j].red = 1.0f;                                                         
            currSubObj->pImpl->pMaterial->getColors()[j].green = 1.0f;
            currSubObj->pImpl->pMaterial->getColors()[j].blue = 1.0f;
            currSubObj->pImpl->pMaterial->getColors()[j].alpha = 1.0f;
        }
    }
    _pOwner->getConsole().OLnOO("submodel material settings done!");

    _pOwner->getConsole().OLnOI("freeing up temporary buffers ...");
    for (TPureUInt i = 0; i < submeshes_h; i++)
    {
        if ( tmpSubMeshesVertices != NULL )
            delete[] tmpSubMeshesVertices[i];

        if ( tmpSubMeshesTexcoords != NULL )
            delete[] tmpSubMeshesTexcoords[i];

        if ( tmpSubMeshesNormals != NULL )
            delete[] tmpSubMeshesNormals[i];
    }
    delete[] tmpSubMeshesVertices;
    delete[] tmpSubMeshesTexcoords;
    delete[] tmpSubMeshesNormals;
    delete[] tmpSubMeshesVertices_h;
    delete[] tmpSubMeshesTexcoords_h;
    delete[] tmpSubMeshesNormals_h;

    delete[] lines;
    delete[] filebuffer;
    delete[] lines_start;
    delete[] lines_end;
    _pOwner->getConsole().OLnOO("done freeing up temporary buffers!");

    obj->RecalculateSize();

    _pOwner->getConsole().SOLnOO("> OBJ loaded!");

    return obj;
} // loadOBJ()


// ############################### PRIVATE ###############################


PureMesh3DManager::PureMesh3DManagerImpl::PureMesh3DManagerImpl() :
    materialMgr(materialMgr)
{

}


/**
    @param owner  Public class instance owning this pimpl object.
    @param matMgr A MaterialManager instance to be used for creating materials used by Mesh3D instances.
*/
PureMesh3DManager::PureMesh3DManagerImpl::PureMesh3DManagerImpl(PureMesh3DManager* owner, PureMaterialManager& matMgr) :
    materialMgr(matMgr)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PureMesh3DManager() ...");
    bMinimalIndexStorage = true;
    nRunningCounter = 0;
    bInited = true;
    _pOwner->getConsole().SOLnOO("Done!");
} // PureMesh3DManager(...)


PureMesh3DManager::PureMesh3DManagerImpl::PureMesh3DManagerImpl(const PureMesh3DManager::PureMesh3DManagerImpl&) :
    materialMgr(materialMgr)
{

}


PureMesh3DManager::PureMesh3DManagerImpl& PureMesh3DManager::PureMesh3DManagerImpl::operator=(const PureMesh3DManager::PureMesh3DManagerImpl&)
{
    return *this;    
}



/*
   PureMesh3DManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureMesh3DManager::PureMesh3DManager(PureMaterialManager& matMgr)
{
    pImpl = new PureMesh3DManagerImpl(this, matMgr);
} // PureMesh3DManager(...)


PureMesh3DManager::~PureMesh3DManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureMesh3DManager()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureMesh3DManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureMesh3DManager::getLoggerModuleName()
{
    return "PureMesh3DManager";
} // getLoggerModuleName()


/**
    Tells whether the object is correctly initialized or not.
    @return True if the given TextureManager instance was initialized correctly, false otherwise.
*/
TPureBool PureMesh3DManager::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


/**
    Tells whether storage of indices is minimalized or not.
    Minimalized storage of indices means that the software always tries to pack geometry indices into as small storage types as possible.
    This can happen in both system and video memory.
    If enabled, geometry indices generally take less memory and rendering may be faster.
    Enabled by default.
*/
TPureBool PureMesh3DManager::isMinimalIndexStorageEnabled() const
{
    return pImpl->isMinimalIndexStorageEnabled();
}


/**
    Sets whether storage of indices is minimalized or not.
    Minimalized storage of indices means that the software always tries to pack geometry indices into as small storage types as possible.
    This can happen in both system or video memory.
    If enabled, geometry indices generally take less memory and rendering may be faster.
    Enabled by default.

    Note: disabling is unimplemented currently!
*/
void PureMesh3DManager::SetMinimalIndexStorageEnabled(TPureBool state)
{
    pImpl->SetMinimalIndexStorageEnabled(state);
}


/**
    Creates a new plane mesh with the given sizes.

    @param a Length of plane (size on X-axis).
    @param b Height of plane (size on Y-axis).
    
    @return New 3D mesh with 1 face and 4 vertices.
            PGENULL if Mesh3DManager is not yet initialized.
*/
PureMesh3D* PureMesh3DManager::createPlane(TPureFloat a, TPureFloat b)
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PureMesh3DManager::createPlane(%f, %f)", a, b);

    PureMesh3D* mesh = PGENULL;
    PureMesh3D* submesh = PGENULL;
    try
    {
        mesh = new PureMesh3D(PURE_PM_QUADS);
        submesh = new PureMesh3D(PURE_PM_QUADS);

        mesh->Attach(*submesh);
        if ( !pImpl->convertToPlane(*mesh, a, b) )
        {
            throw std::runtime_error("convertToPlane() failed!");
        }
        Attach( *mesh );
    }
    catch (const std::exception& e)
    {
        delete mesh;
        delete submesh;
        getConsole().EOLnOO("ERROR: Failed to instantiate or fill new PureMesh3D: %s!", e.what());
        getConsole().OLn("");
        return PGENULL;
    }

    mesh->SetName("Mesh3D " + std::to_string(pImpl->nRunningCounter++));

    getConsole().SOLnOO("> Plane mesh created successfully, name: %s!", mesh->getName().c_str());
    getConsole().OLn("");

    return mesh;

} // createPlane()


/**
    Creates a new box mesh with the given sizes.
    This box consists of quads instead of triangles and has 24 vertices.
    Every vertex has a normal, a color, and texture uvw-coordinates in separate arrays.
    All faces are defined counter-clockwise starting from the lower left vertex, thus
    the order of texture uvw-coordinates are the same for each face.
    Order of faces is: front face towards the viewer, back, left, right, top, bottom.

    @param a Length of box (size on X-axis).
    @param b Height of box (size on Y-axis).
    @param c Depth of box (size on Z-axis).

    @return New 3D box mesh with 6 faces and 24 vertices.
            PGENULL if Mesh3DManager is not yet initialized.
*/
PureMesh3D* PureMesh3DManager::createBox(TPureFloat a, TPureFloat b, TPureFloat c)
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PureMesh3DManager::createBox(%f, %f, %f)", a, b, c);

    PureMesh3D* mesh = PGENULL;
    PureMesh3D* submesh = PGENULL;
    try
    {
        mesh = new PureMesh3D(PURE_PM_QUADS);
        submesh = new PureMesh3D(PURE_PM_QUADS);
        mesh->Attach(*submesh);
        if ( !pImpl->convertToBox(*mesh, a, b, c) )
        {
            throw std::runtime_error("convertToBox() failed!");
        }
        Attach( *mesh );
    }
    catch (const std::exception& e)
    {
        delete mesh;
        delete submesh;
        getConsole().EOLnOO("ERROR: Failed to instantiate or fill new PureMesh3D: %s!", e.what());
        getConsole().OLn("");
        return PGENULL;
    }

    mesh->SetName("Mesh3D " + std::to_string(pImpl->nRunningCounter++));

    getConsole().SOLnOO("> Box mesh created successfully, name: %s!", mesh->getName().c_str());
    getConsole().OLn("");

    return mesh;

} // createBox()


/**
    Creates a new cube mesh with the given sizes.
    Equivalent to createBox(a, a, a).

    @param a Length of edges.

    @return New 3D cube mesh with 6 faces and 24 vertices.
            PGENULL if PureMesh3DManager is not yet initialized.
*/
PureMesh3D* PureMesh3DManager::createCube(TPureFloat a)
{
    getConsole().OLnOI("PureMesh3DManager::createCube(%f), passing to createBox() ...", a);
    PureMesh3D* const cube = createBox(a, a, a);
    if ( cube != PGENULL )
    {
        getConsole().SOLnOO("> Cube mesh created successfully, name: %s!", cube->getName().c_str());
    }
    else
    {
        getConsole().EOLnOO("ERROR: createBox() returned NULL!");
    }

    getConsole().OLn("");
    return cube;
} // createCube()


/**
    Creates 3D mesh from the given file.

    @param filename The model file to be loaded to be an Object3D instance.

    @return The created mesh.
            PGENULL if PureMesh3DManager is not yet initialized or the specified file does not exist or the specified file format is not known.
*/
PureMesh3D* PureMesh3DManager::createFromFile(const char* filename)
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PureMesh3DManager::createFromFile(\"%s\")", filename);

    if ( filename == NULL )
    {
        getConsole().EOLnOO("ERROR: input was NULL, returning PGENULL");
        getConsole().OLn("");
        return PGENULL;
    }

    PureMesh3D* obj = PGENULL;

    string sFileExt = PFL::getExtension(filename);
    if ( sFileExt == "" )
    {                                              
        getConsole().EOLnOO("ERROR: no file extension!");
        getConsole().OLn("");
        return PGENULL;    
    }
    if ( !PFL::fileExists(filename) )
    {
        getConsole().EOLnOO("ERROR: file doesn't exist!");
        getConsole().OLn("");
        return PGENULL;
    }
    
    #pragma warning(disable:4244)  /* int-char conversion in std::transform */
    transform(sFileExt.begin(), sFileExt.end(), sFileExt.begin(), ::toupper);
    #pragma warning(default:4244)

    getConsole().OLn("ext: .%s", sFileExt.c_str());
    if ( sFileExt == "OBJ" )
    {
        getConsole().OI();
        obj = pImpl->loadOBJ(filename);
        getConsole().OO();
     }    
    else
    {
        getConsole().EOLnOO("ERROR: unsupported extension!");
        getConsole().OLn("");
        return PGENULL;
    }

    if ( obj == PGENULL )
    {
        getConsole().EOLnOO("ERROR: loadXXX() returned PGENULL!");
        getConsole().OLn("");
        return PGENULL;
    }

    obj->SetFilename(filename);
    Attach( *obj );
    obj->SetName("Mesh3D " + std::to_string(pImpl->nRunningCounter++));

    getConsole().SOLnOO("> Mesh loaded successfully, name: %s!", obj->getName().c_str());
    getConsole().OLn("");
    return obj; 
} // createFromFile()


/**
    From PureFiledManager, adding logging mesh data.
*/
void PureMesh3DManager::WriteList() const
{
    getConsole().OLn("PureMesh3DManager::WriteList()");
    getConsole().OLn("===============================");
    getConsole().OLnOI("");
    PureFiledManager::WriteList();
    getConsole().OO();
} // WriteList()


// ############################## PROTECTED ##############################


PureMesh3DManager::PureMesh3DManager()
{

}


PureMesh3DManager::PureMesh3DManager(const PureMesh3DManager&)
{

}


PureMesh3DManager& PureMesh3DManager::operator=(const PureMesh3DManager&)
{
    return *this;    
}


void PureMesh3DManager::WriteListCallback(const PureManaged& mngd) const
{
    PureFiledManager::WriteListCallback(mngd);
    const PureMesh3D& mesh = (PureMesh3D&) mngd;

    getConsole().OIOLnOO("pos: [%f,%f,%f], rel pos: [%f,%f,%f], size: [%f,%f,%f]",
                          mesh.getPosVec().getX(), mesh.getPosVec().getY(), mesh.getPosVec().getZ(),
                          mesh.getRelPosVec().getX(), mesh.getRelPosVec().getY(), mesh.getRelPosVec().getZ(),
                          mesh.getSizeVec().getX(), mesh.getSizeVec().getY(), mesh.getSizeVec().getZ());
    getConsole().OIOLnOO("%d subobjects, %u vertices, %u vertex indices", mesh.getCount(), mesh.getVerticesCount(), mesh.getVertexIndicesCount());
    getConsole().OIOLnOO("primitive format: %d, %u faces, %u triangles", (int)(mesh.getPrimitiveFormat()), mesh.getFaceCount(), mesh.getTriangleCount());
} // WriteListCallback()


// ############################### PRIVATE ###############################


