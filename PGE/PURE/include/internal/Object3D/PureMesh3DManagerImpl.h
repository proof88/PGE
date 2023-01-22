#pragma once

/*
    ###################################################################################
    PureMesh3DManagerImpl.h
    This file is part of PURE.
    Internal header.
    PureMesh3DManagerImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PureAllHeaders.h"
#include "../../external/Object3D/PureMesh3DManager.h"
#include "../gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PureMesh3DManager::PureMesh3DManagerImpl
{

public:
    virtual ~PureMesh3DManagerImpl();

    TPurebool isInitialized() const;

    TPurebool isMinimalIndexStorageEnabled() const;
    void      SetMinimalIndexStorageEnabled(TPurebool state);

    TPurebool convertToPlane(
        PureMesh3D& mesh, TPurefloat a, TPurefloat b);                            /**< Converts the given Mesh to a plane with the given sizes. */

    TPurebool convertToBox(
        PureMesh3D& mesh, TPurefloat a, TPurefloat b, TPurefloat c);              /**< Converts the given Mesh to a box with the given sizes. */

    PureMaterial* createMaterialForMesh(PureMesh3D& mesh) const;                  /**< Creates a material for the given Mesh if it doesn't yet have one. */

protected:
    PureMesh3D* loadOBJ(const char* filename);   /**< Loads OBJ files, handles the actual file operations, and creates the Mesh3D object. */

private:

    PureMesh3DManager* _pOwner;      /**< The owner public object who creates this pimpl object. */

    PureMaterialManager& materialMgr;           /**< Used to auto-load/create materials for objects. */

    TPurebool            bInited;               /**< True if successfully inited, false if not functional. */
    TPureuint            nRunningCounter;       /**< Always increased when creating a new level-1 Mesh3D instance. */
    TPurebool            bMinimalIndexStorage;  /**< True if storage of indices is minimalized. */

    // ---------------------------------------------------------------------------

    PureMesh3DManagerImpl();
    PureMesh3DManagerImpl(PureMesh3DManager* owner, PureMaterialManager& matMgr);

    PureMesh3DManagerImpl(const PureMesh3DManagerImpl&);
    PureMesh3DManagerImpl& operator=(const PureMesh3DManagerImpl&);

    friend class PureMesh3DManager;

}; // class PureMesh3DManager::PureMesh3DManagerImpl