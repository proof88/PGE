#pragma once

/*
    ###################################################################################
    PRREMesh3DManagerImpl.h
    This file is part of PRRE.
    Internal header.
    PRREMesh3DManagerImpl class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "../../external/PRREallHeaders.h"
#include "../../external/Object3D/PRREMesh3DManager.h"
#include "../gl/gl.h"  // for GLenum and similar, which should be removed from here soon ...

class PRREMesh3DManager::PRREMesh3DManagerImpl
{

public:
    virtual ~PRREMesh3DManagerImpl();

    TPRREbool isInitialized() const;

    TPRREbool isMinimalIndexStorageEnabled() const;
    void      SetMinimalIndexStorageEnabled(TPRREbool state);

    TPRREbool convertToPlane(
        PRREMesh3D& mesh, TPRREfloat a, TPRREfloat b);                            /**< Converts the given Mesh to a plane with the given sizes. */

    TPRREbool convertToBox(
        PRREMesh3D& mesh, TPRREfloat a, TPRREfloat b, TPRREfloat c);              /**< Converts the given Mesh to a box with the given sizes. */

    PRREMaterial* createMaterialForMesh(PRREMesh3D& mesh) const;                  /**< Creates a material for the given Mesh if it doesn't yet have one. */

protected:
    PRREMesh3D* loadOBJ(const char* filename);   /**< Loads OBJ files, handles the actual file operations, and creates the Object3D object. */

private:

    PRREMesh3DManager* _pOwner;      /**< The owner public object who creates this pimpl object. */

    PRREMaterialManager& materialMgr;           /**< Used to auto-load/create materials for objects. */

    TPRREbool            bInited;               /**< True if successfully inited, false if not functional. */
    TPRREbool            bMinimalIndexStorage;  /**< True if storage of indices is minimalized. */

    // ---------------------------------------------------------------------------

    PRREMesh3DManagerImpl();
    PRREMesh3DManagerImpl(PRREMesh3DManager* owner, PRREMaterialManager& matMgr);

    PRREMesh3DManagerImpl(const PRREMesh3DManagerImpl&);
    PRREMesh3DManagerImpl& operator=(const PRREMesh3DManagerImpl&);

    friend class PRREMesh3DManager;

}; // class PRREMesh3DManager::PRREMesh3DManagerImpl