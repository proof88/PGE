#pragma once

/*
    ###################################################################################
    PureMesh3DManagerImpl.h
    This file is part of PURE.
    Internal header.
    PureMesh3DManagerImpl class.
    Made by PR00F88
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../external/Object3D/PureMesh3DManager.h"
#include "../gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PureMesh3DManager::PureMesh3DManagerImpl
{

public:
    virtual ~PureMesh3DManagerImpl();

    TPureBool isInitialized() const;

    TPureBool isMinimalIndexStorageEnabled() const;
    void      SetMinimalIndexStorageEnabled(TPureBool state);

    TPureBool convertToPlane(
        PureMesh3D& mesh, TPureFloat a, TPureFloat b);                            /**< Converts the given Mesh to a plane with the given sizes. */

    TPureBool convertToBox(
        PureMesh3D& mesh, TPureFloat a, TPureFloat b, TPureFloat c);              /**< Converts the given Mesh to a box with the given sizes. */

    PureMaterial* createMaterialForMesh(PureMesh3D& mesh) const;                  /**< Creates a material for the given Mesh if it doesn't yet have one. */

protected:
    PureMesh3D* loadOBJ(const char* filename);   /**< Loads OBJ files, handles the actual file operations, and creates the Mesh3D object. */

private:

    PureMesh3DManager* _pOwner;      /**< The owner public object who creates this pimpl object. */

    PureMaterialManager& materialMgr;           /**< Used to auto-load/create materials for objects. */

    TPureBool            bInited;               /**< True if successfully inited, false if not functional. */
    TPureUInt            nRunningCounter;       /**< Always increased when creating a new level-1 Mesh3D instance. */
    TPureBool            bMinimalIndexStorage;  /**< True if storage of indices is minimalized. */

    // ---------------------------------------------------------------------------

    PureMesh3DManagerImpl();
    PureMesh3DManagerImpl(PureMesh3DManager* owner, PureMaterialManager& matMgr);

    PureMesh3DManagerImpl(const PureMesh3DManagerImpl&);
    PureMesh3DManagerImpl& operator=(const PureMesh3DManagerImpl&);

    friend class PureMesh3DManager;

}; // class PureMesh3DManager::PureMesh3DManagerImpl