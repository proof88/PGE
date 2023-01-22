/*
    ###################################################################################
    PureObject3DManager.cpp
    This file is part of Pure.
    PureObject3DManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################                                                
*/

#include "PurebaseIncludes.h"  // PCH
#include "../../include/external/Object3D/PureObject3DManager.h"
#include "../../include/internal/Object3D/PureObject3DImpl.h"
#include "../../include/internal/Object3D/PureMesh3DImpl.h"
#include "../../include/internal/Object3D/PureMesh3DManagerImpl.h"
#include "../../include/external/Hardware/PureHwInfo.h"
#include "../../include/internal/PureGLextensionFuncs.h"
#include "../../include/internal/PureGLsafeFuncs.h"
#include "../../include/internal/PureGLsnippets.h"
#include "../../include/internal/Purepragmas.h"

using namespace std;

/*
   PureObject3DManager::PureObject3DManagerImpl
   ###########################################################################
*/


class PureObject3DManager::PureObject3DManagerImpl
{

public:
    virtual ~PureObject3DManagerImpl();

    TPurebool isInitialized() const;

          std::deque<PureObject3D*>& getOccluders();
    const std::deque<PureObject3D*>& getOccluders() const;
          std::deque<PureObject3D*>& get3dOpaqueOccludees();
    const std::deque<PureObject3D*>& get3dOpaqueOccludees() const;
          std::deque<PureObject3D*>& get3dBlendedOccludees();
    const std::deque<PureObject3D*>& get3dBlendedOccludees() const;
          std::deque<PureObject3D*>& get2dOpaqueOccludees();
    const std::deque<PureObject3D*>& get2dOpaqueOccludees() const;
          std::deque<PureObject3D*>& get2dBlendedOccludees();
    const std::deque<PureObject3D*>& get2dBlendedOccludees() const;

protected:

private:

    static TPurebool isEligibleForOcclusionQuery(TPureuint nTotalVertexIndices);  /**< Decides if an object is eligible for automatically turn on occlusion query on it. */

    PureObject3DManager* _pOwner;        /**< The owner public object who creates this pimpl object. */

    TPureuint            nRunningCounter;        /**< Always increased when creating a new level-1 Object3D instance. */
    TPurebool            bInited;                /**< True if successfully inited, false if not functional. */
    TPurebool            bMinimalIndexStorage;   /**< True if storage of indices is minimalized. */
    PureTextureManager&  textureMgr;             /**< Used to auto-load textures for OBJ files. */
    PureMaterialManager& materialMgr;            /**< Used to auto-load/create materials for objects. */
    TPurefloat           fOccluderSelectionBias; /**< The biggest area of an object should be at least this times bigger than the average biggest area of all objects to be selected as an occluder. */
    TPureuint            nMaxOccluderCount;      /**< How many occluders are allowed to be selected by UpdateOccluderStates(). */

    // ---------------------------------------------------------------------------

    PureObject3DManagerImpl();
    PureObject3DManagerImpl(
        PureObject3DManager* owner,
        PureTextureManager& texMgr,
        PureMaterialManager& matMgr );

    PureObject3DManagerImpl(const PureObject3DManagerImpl&);
    PureObject3DManagerImpl& operator=(const PureObject3DManagerImpl&);

    friend class PureObject3DManager;

};


// ############################### PUBLIC ################################


PureObject3DManager::PureObject3DManagerImpl::~PureObject3DManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PureObject3DManagerImpl() ...");
    _pOwner->DeleteAll();
    _pOwner->getConsole().OO();    
    _pOwner = NULL;
} // ~PureObject3DManager()


TPurebool PureObject3DManager::PureObject3DManagerImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::getOccluders()
{
    return PureObject3D::PureObject3DImpl::occluders;
} // getOccluders()


const std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::getOccluders() const
{
    return PureObject3D::PureObject3DImpl::occluders;
} // getOccluders()


std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::get3dOpaqueOccludees()
{
    return PureObject3D::PureObject3DImpl::occludees_opaque;
} // get3dOpaqueOccludees()


const std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::get3dOpaqueOccludees() const
{
    return PureObject3D::PureObject3DImpl::occludees_opaque;
} // get3dOpaqueOccludees()


std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::get3dBlendedOccludees()
{
    return PureObject3D::PureObject3DImpl::occludees_blended;
} // get3dBlendedOccludees()


const std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::get3dBlendedOccludees() const
{
    return PureObject3D::PureObject3DImpl::occludees_blended;
} // get3dBlendedOccludees()


std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::get2dOpaqueOccludees()
{
    return PureObject3D::PureObject3DImpl::occludees_2d_opaque;
} // get2dOpaqueOccludees()


const std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::get2dOpaqueOccludees() const
{
    return PureObject3D::PureObject3DImpl::occludees_2d_opaque;
} // get2dOpaqueOccludees()


std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::get2dBlendedOccludees()
{
    return PureObject3D::PureObject3DImpl::occludees_2d_blended;
} // get2dBlendedOccludees()


const std::deque<PureObject3D*>& PureObject3DManager::PureObject3DManagerImpl::get2dBlendedOccludees() const
{
    return PureObject3D::PureObject3DImpl::occludees_2d_blended;
} // get2dBlendedOccludees()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    Decides if an object is eligible for automatically turn on occlusion query on it.
    @return True if object should be prepared for occlusion querying, false otherwise.
*/
TPurebool PureObject3DManager::PureObject3DManagerImpl::isEligibleForOcclusionQuery(TPureuint nTotalVertexIndices)
{
    return nTotalVertexIndices >= 100;
} // isEligibleForOcclusionQuery()


PureObject3DManager::PureObject3DManagerImpl::PureObject3DManagerImpl() :
    textureMgr(textureMgr),
    materialMgr(materialMgr)
{

}


/**
    @param owner  Public class instance owning this pimpl object.
    @param texMgr A TextureManager instance to be used for loading textures used by Object3D instances.
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.
*/
PureObject3DManager::PureObject3DManagerImpl::PureObject3DManagerImpl(PureObject3DManager* owner, PureTextureManager& texMgr, PureMaterialManager& matMgr) :
    textureMgr(texMgr),
    materialMgr(matMgr)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PureObject3DManagerImpl() ...");
    bInited = false;
    nRunningCounter = 0;
    bMinimalIndexStorage = true;
    fOccluderSelectionBias = 1.0f;
    nMaxOccluderCount = 4;
    PureObject3D::PureObject3DImpl::stats.push_back( PureObject3D::PureObject3DImpl::CurrentStats() );
    // we rely on texture manager initialized state since that also relies on initialized state of hwinfo
    bInited = texMgr.isInitialized();
    _pOwner->getConsole().SOLnOO("Done!");
} // PureObject3DManager(...)


PureObject3DManager::PureObject3DManagerImpl::PureObject3DManagerImpl(const PureObject3DManager::PureObject3DManagerImpl&) :
    textureMgr(textureMgr),
    materialMgr(materialMgr)
{

}


PureObject3DManager::PureObject3DManagerImpl& PureObject3DManager::PureObject3DManagerImpl::operator=(const PureObject3DManager::PureObject3DManagerImpl&)
{
    return *this;    
}


/*
   PureObject3DManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @param texMgr A TextureManager instance to be used for loading textures used by Object3D instances.
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureObject3DManager::PureObject3DManager(PureTextureManager& texMgr, PureMaterialManager& matMgr)
    : PureMesh3DManager(matMgr)
{
    pImpl = new PureObject3DManagerImpl(this, texMgr, matMgr);
} // PureObject3DManager(...)


PureObject3DManager::~PureObject3DManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PureObject3DManager()


/**
    Adds the given managed to the manager, if the managed has no manager yet.
    No effect if given managed already has a manager.
    The object will be present in get3dOpaqueOccludees() or get2dOpaqueOccludees() list based on its sticked-to-screen property since it will be attached as non-blended.
*/
void PureObject3DManager::Attach(PureManaged& m)
{
    if ( m.getManager() != PGENULL )
        return;

    PureMesh3DManager::Attach(m);
    try
    {
        PureObject3D& obj = dynamic_cast<PureObject3D&>(m);

        // it will be an occludee for sure, since:
        // - either this is a brand new object attached first time to its manager,
        // - or Detach() had previously removed it from occludees_* and occluders lists, and without manager it cannot be set as occluder so it stayed unchanged for sure!
        obj.SetOccluder(false);
    }
    catch (const std::exception&)
    {
        getConsole().OLn("WARNING: PureObject3DManager::Attach() param is NOT Object3D ref!");
    }
} // Attach()


/**
    Removes the given managed from the manager, so the managed will have no manager.
    The object will not be occlusion tested, and won't be an occluder, and actually it will NOT be present in any of the following lists:
    getOccluders(), get3dOpaqueOccludees(), get3dBlendedOccludees(), get2dOpaqueOccludees() and get2dBlendedOccludees().
*/
void PureObject3DManager::Detach(PureManaged& m)
{
    if ( m.getManager() == PGENULL )
        return;

    try
    {
        PureObject3D& obj = dynamic_cast<PureObject3D&>(m);
        obj.SetOcclusionTested(false);
        obj.getMaterial(false).setBlendMode(Pure_BM_NONE); // removes from occludees_blended and occludees_2d_blended container
        obj.SetOccluder(false); // removes from occluders container

        auto occ_it = std::find(PureObject3D::PureObject3DImpl::occludees_opaque.begin(), PureObject3D::PureObject3DImpl::occludees_opaque.end(), &obj);
        if ( occ_it != PureObject3D::PureObject3DImpl::occludees_opaque.end() )
        {
            PureObject3D::PureObject3DImpl::occludees_opaque.erase(occ_it);
        }
        else
        {
            occ_it = std::find(PureObject3D::PureObject3DImpl::occludees_2d_opaque.begin(), PureObject3D::PureObject3DImpl::occludees_2d_opaque.end(), &obj);
            if ( occ_it != PureObject3D::PureObject3DImpl::occludees_2d_opaque.end() )
            {
                PureObject3D::PureObject3DImpl::occludees_2d_opaque.erase(occ_it);
            }
        }
    }
    catch (const std::exception&)
    {
        // no need to log
        //getConsole().OLn("WARNING: PureObject3DManager::Detach() param is NOT Object3D ref!");
    }

    PureMesh3DManager::Detach(m);
} // Detach()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureObject3DManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureObject3DManager::getLoggerModuleName()
{
    return "PureObject3DManager";
} // getLoggerModuleName()


/**
    Tells whether the manager is correctly initialized or not.
    @return True if the given Object3DManager instance was initialized correctly, false otherwise.
*/
TPurebool PureObject3DManager::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


/**
    Get a list of occluders.
    If you want to get all the occluders, this way is faster than iterating over all objects and checking if they are occluders.

    @return List of occluders.
*/
std::deque<PureObject3D*>& PureObject3DManager::getOccluders()
{
    return pImpl->getOccluders();
} // getOccluders()


/**
    Get a list of occluders.
    If you want to get all the occluders, this way is faster than iterating over all objects and checking if they are occluders.

    @return List of occluders.
*/
const std::deque<PureObject3D*>& PureObject3DManager::getOccluders() const
{
    return pImpl->getOccluders();
} // getOccluders()


/**
    Get a list of 3D (non-sticked) opaque (non-blended) occludees.
    If you want to get all the 3D (non-sticked) opaque occludees, this way is faster than iterating over all objects and checking their properties.

    @return List of 3D opaque occludees.
*/
std::deque<PureObject3D*>& PureObject3DManager::get3dOpaqueOccludees()
{
    return pImpl->get3dOpaqueOccludees();
} // get3dOpaqueOccludees()


/**
    Get a list of 3D (non-sticked) opaque (non-blended) occludees.
    If you want to get all the 3D (non-sticked) opaque occludees, this way is faster than iterating over all objects and checking their properties.

    @return List of 3D opaque occludees.
*/
const std::deque<PureObject3D*>& PureObject3DManager::get3dOpaqueOccludees() const
{
    return pImpl->get3dOpaqueOccludees();
} // get3dOpaqueOccludees()


/**
    Get a list of 3D (non-sticked) blended occludees.
    If you want to get all the 3D (non-sticked) blended occludees, this way is faster than iterating over all objects and checking their properties.

    @return List of 3D blended occludees.
*/
std::deque<PureObject3D*>& PureObject3DManager::get3dBlendedOccludees()
{
    return pImpl->get3dBlendedOccludees();
} // get3dBlendedOccludees()


/**
    Get a list of 3D (non-sticked) blended occludees.
    If you want to get all the 3D (non-sticked) blended occludees, this way is faster than iterating over all objects and checking their properties.

    @return List of 3D blended occludees.
*/
const std::deque<PureObject3D*>& PureObject3DManager::get3dBlendedOccludees() const
{
    return pImpl->get3dBlendedOccludees();
} // get3dBlendedOccludees()


/**
    Get a list of 2D (sticked) opaque (non-blended) occludees.
    If you want to get all the 2D (sticked) opaque occludees, this way is faster than iterating over all objects and checking their properties.

    @return List of 2D opaque occludees.
*/
std::deque<PureObject3D*>& PureObject3DManager::get2dOpaqueOccludees()
{
    return pImpl->get2dOpaqueOccludees();
} // get2dOpaqueOccludees()


/**
    Get a list of 2D (sticked) opaque (non-blended) occludees.
    If you want to get all the 2D (sticked) opaque occludees, this way is faster than iterating over all objects and checking their properties.

    @return List of 2D opaque occludees.
*/
const std::deque<PureObject3D*>& PureObject3DManager::get2dOpaqueOccludees() const
{
    return pImpl->get2dOpaqueOccludees();
} // get2dOpaqueOccludees()


/**
    Get a list of 2D (sticked) blended occludees.
    If you want to get all the 2D (sticked) blended occludees, this way is faster than iterating over all objects and checking their properties.

    @return List of 2D blended occludees.
*/
std::deque<PureObject3D*>& PureObject3DManager::get2dBlendedOccludees()
{
    return pImpl->get2dBlendedOccludees();
} // get2dBlendedOccludees()


/**
    Get a list of 2D (sticked) blended occludees.
    If you want to get all the 2D (sticked) blended occludees, this way is faster than iterating over all objects and checking their properties.

    @return List of 2D blended occludees.
*/
const std::deque<PureObject3D*>& PureObject3DManager::get2dBlendedOccludees() const
{
    return pImpl->get2dBlendedOccludees();
} // get2dBlendedOccludees()


/**
    Creates a new plane with the given sizes.

    @param a                     Length of plane (size on X-axis).
    @param b                     Height of plane (size on Y-axis).
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.
    
    @return New 3D object with 1 face and 4 vertices.
            PGENULL if Object3DManager is not yet initialized.
*/
PureObject3D* PureObject3DManager::createPlane(
    TPurefloat a, TPurefloat b,
    TPure_VERTEX_MODIFYING_HABIT vmod,
    TPure_VERTEX_REFERENCING_MODE vref,
    TPurebool bForceUseClientMemory)
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PureObject3DManager::createPlane(%f, %f)", a, b);

    PureObject3D* obj = PGENULL;
    PureObject3D* subobj = PGENULL;
    try
    {
        obj = new PureObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
        // do not attach obj to object3dmanager yet, we need its material created first!

        subobj = new PureObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
        obj->Attach( *subobj );

        PureMesh3DManager* const me = this;
        if ( !(me->pImpl->convertToPlane(*obj, a, b)) )
        {
            throw std::runtime_error("convertToPlane() failed!");
        }

        // now we have material, it can be attached to object3dmanager!
        Attach( *obj );

        subobj->pImpl->pVerticesTransf = new TPure_TRANSFORMED_VERTEX[subobj->getVerticesCount()];
    }
    catch (const std::exception& e)
    {
        delete subobj; // will invoke obj->Detach()
        delete obj;    // would delete subobj, except if it is not yet attached, so we explicitly delete subobj above
        getConsole().EOLnOO("ERROR: PureObject3DManager::createPlane() failed to instantiate or fill PureObject3D: %s!", e.what());
        getConsole().OLn("");
        return PGENULL;
    }

    // although PPP has already selected the vtransmode, we set it again
    // to actually allocate the needed resources for the geometry
    getConsole().OI();
    if ( !obj->setVertexTransferMode( obj->getVertexTransferMode() ) )
    {
        getConsole().OO();
        DeleteAttachedInstance( *obj );
        getConsole().EOLnOO("ERROR: setVertexTransferMode() failed, returning NULL now!");
        getConsole().OLn("");
        return PGENULL;
    }
    getConsole().OO();

    obj->SetName("Object3D " + std::to_string(pImpl->nRunningCounter++));
    obj->SetOccluder(false);

    getConsole().SOLnOO("> Plane created successfully, name: %s!", obj->getName().c_str());
    getConsole().OLn("");

    return obj;

} // createPlane()


/**
    Creates a new box with the given sizes.
    This box consists of quads instead of triangles and has 24 vertices.
    Every vertex has a normal, a color, and texture uvw-coordinates in separate arrays.
    All faces are defined counter-clockwise starting from the lower left vertex, thus
    the order of texture uvw-coordinates are the same for each face.
    Order of faces is: front face towards the viewer, back, left, right, top, bottom.

    @param a                     Length of box (size on X-axis).
    @param b                     Height of box (size on Y-axis).
    @param c                     Depth of box (size on Z-axis).
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.

    @return New 3D box with 6 faces and 24 vertices.
            PGENULL if Object3DManager is not yet initialized.
*/
PureObject3D* PureObject3DManager::createBox(
    TPurefloat a, TPurefloat b, TPurefloat c,
    TPure_VERTEX_MODIFYING_HABIT vmod,
    TPure_VERTEX_REFERENCING_MODE vref,
    TPurebool bForceUseClientMemory )
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PureObject3DManager::createBox(%f, %f, %f)", a, b, c);

    PureObject3D* obj = PGENULL;
    PureObject3D* subobj = PGENULL;
    try
    {
        obj = new PureObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
        // do not attach obj to object3dmanager yet, we need its material created first!

        subobj = new PureObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
        obj->Attach( *subobj );

        PureMesh3DManager* const me = this;
        if ( !me->pImpl->convertToBox(*obj, a, b, c) )
        {
            throw std::runtime_error("convertToBox() failed!");
        }

        // now we have material, it can be attached to object3dmanager!
        Attach( *obj );

        subobj->pImpl->pVerticesTransf = new TPure_TRANSFORMED_VERTEX[subobj->getVerticesCount()];
    }
    catch (const std::exception& e)
    {
        delete subobj; // will invoke obj->Detach()
        delete obj;    // would delete subobj, except if it is not yet attached, so we explicitly delete subobj above
        getConsole().EOLn("ERROR: PureObject3DManager::createPlane() failed to instantiate or fill PureObject3D: %s!", e.what());
        getConsole().OLn("");
        return PGENULL;
    }

    // although PPP has already selected the vtransmode, we set it again
    // to actually allocate the needed resources for the geometry
    getConsole().OI();
    if ( !obj->setVertexTransferMode( obj->getVertexTransferMode() ) )
    {
        getConsole().OO();
        DeleteAttachedInstance( *obj );
        getConsole().EOLnOO("ERROR: setVertexTransferMode() failed, returning NULL now!");
        getConsole().OLn("");
        return PGENULL;
    }
    getConsole().OO();

    obj->SetName("Object3D " + std::to_string(pImpl->nRunningCounter++));
    obj->SetOccluder(false);

    getConsole().SOLnOO("> Box created successfully, name: %s!", obj->getName().c_str());
    getConsole().OLn("");

    return obj;

} // createBox()


/**
    Creates a new cube with the given sizes.
    Equivalent to createBox(a, a, a).

    @param a                     Length of edges.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.

    @return New 3D cube with 6 faces and 24 vertices.
            PGENULL if Object3DManager is not yet initialized.
*/
PureObject3D* PureObject3DManager::createCube(
    TPurefloat a,
    TPure_VERTEX_MODIFYING_HABIT vmod,
    TPure_VERTEX_REFERENCING_MODE vref,
    TPurebool bForceUseClientMemory )
{
    getConsole().OLnOI("PureObject3DManager::createCube(%f), passing to createBox() ...", a);
    PureObject3D* const cube = createBox(a, a, a, vmod, vref, bForceUseClientMemory);
    if ( PGENULL == cube )
    {
        getConsole().EOLnOO("ERROR: PureObject3DManager::createBox() failed!");
    }
    else
    {
        getConsole().SOLnOO("> Cube created successfully, name: %s!", cube->getName().c_str());
    }

    getConsole().OLn("");
    return cube;
} // createCube()


/**
    Creates object from the given file.

    @param filename              The model file to be loaded to be an Object3D instance.
    @param vmod                  What vertex modifying habit to be set for the new Object3D instance.
    @param vref                  What vertex referencing mode to be set for the new Object3D instance.
    @param bForceUseClientMemory Force-select a vertex transfer mode storing geometry in client memory instead of server memory.
                                 Please note that this is considered only if dynamic modifying habit is specified.
                                 Specifying static modifying habit will always select a mode which places geometry data into server memory.

    @return The created object.
            PGENULL if Object3DManager is not yet initialized or the specified file does not exist or the specified file format is not known.
*/
PureObject3D* PureObject3DManager::createFromFile(
    const char* filename,
    TPure_VERTEX_MODIFYING_HABIT vmod,
    TPure_VERTEX_REFERENCING_MODE vref,
    TPurebool bForceUseClientMemory )
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PureObject3DManager::createFromFile(\"%s\")", filename);

    if ( filename == NULL )
    {
        getConsole().EOLnOO("ERROR: input was NULL, returning PGENULL");
        getConsole().OLn("");
        return PGENULL;
    }

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

    PureObject3D* obj = PGENULL;
    PureObject3D* subobject = PGENULL;
    PureMesh3D* tmpMesh = PGENULL;
    #pragma warning(disable:4244)  /* int-char conversion in std::transform */
    transform(sFileExt.begin(), sFileExt.end(), sFileExt.begin(), ::toupper);
    #pragma warning(default:4244)
    getConsole().OLn("ext: .%s", sFileExt.c_str());
    if ( sFileExt == "OBJ" )
    {
        try
        {
            getConsole().OI();
            tmpMesh = PureMesh3DManager::createFromFile(filename);
            if ( !tmpMesh )
            {
                getConsole().OO();
                getConsole().EOLnOO("ERROR: failed to create tmpMesh!");
                getConsole().OLn("");
                return PGENULL;
            }
            obj = new PureObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
            if ( !((PureMesh3D*)obj)->pImpl->cannibalize(*tmpMesh) )
            {
                const std::string sErrMsg = "cannibalize() failed for level-1 obj!";
                throw std::runtime_error(sErrMsg);
            }
            obj->SetName("Object3D " + std::to_string(pImpl->nRunningCounter++));

            TPureuint nVerticesTotalTmpMesh = 0;
            for (TPureint i = 0; i < tmpMesh->getCount(); i++) 
            {
                subobject = new PureObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
                obj->Attach( *subobject );
                if ( !((PureMesh3D*)subobject)->pImpl->cannibalize(*(PureMesh3D*)(tmpMesh->getAttachedAt(i))) )
                {
                    const std::string sErrMsg = "cannibalize() failed for level-2 subobj " + std::to_string(i) + "!";
                    throw std::runtime_error(sErrMsg);
                }
                subobject->pImpl->pVerticesTransf = new TPure_TRANSFORMED_VERTEX[subobject->getVerticesCount()];
                nVerticesTotalTmpMesh += subobject->getVertexIndicesCount();
                
                // Legacy tmcsgfxlib behavior: auto-load textures for subobjects where pipe character is present in name
                const std::string::size_type nPipePos = subobject->getName().find('|');
                if ( nPipePos != std::string::npos )
                {
                    const std::string sTexName = subobject->getName().substr(nPipePos+1);
                    getConsole().OLn("Legacy: loading texture implicitly by submodel name: %s", sTexName.c_str());
                    PureTexture* const tex = pImpl->textureMgr.createFromFile( PFL::getDirectory(filename).append(sTexName).c_str() );
                    if ( tex )
                    {
                        if ( !subobject->getMaterial().setTexture(tex) )
                        {
                            getConsole().EOLn("ERROR: failed to set texture for subobject! Continuing ...");
                        }
                    }
                    else
                    {
                        getConsole().EOLn("ERROR: PureObject3DManager::createFromFile() failed to load texture: %s! Continuing ...", sTexName.c_str());
                    }
                    // since we have loaded texture from submodelname, we can get rid of the texture filename part of it
                    subobject->SetName( subobject->getName().substr(0, nPipePos) ); 
                }
            }
            this->DeleteAttachedInstance(*tmpMesh);
            tmpMesh = PGENULL;

            obj->SetFilename(filename);
            getConsole().OO();
            Attach( *obj );

            obj->SetOccluder(false);
            if ( PureHwInfo::get().getVideo().isOcclusionQuerySupported() && pImpl->isEligibleForOcclusionQuery(nVerticesTotalTmpMesh) )
            {
                obj->SetOcclusionTested(true);
            }

            // although PPP has already selected the vtransmode, we set it again
            // to actually allocate the needed resources for the geometry
            getConsole().OI();
            if ( !obj->setVertexTransferMode( obj->getVertexTransferMode() ) )
            {
                getConsole().OO();
                DeleteAttachedInstance( *obj );
                getConsole().EOLnOO("ERROR: setVertexTransferMode() failed, returning NULL now!");
                getConsole().OLn("");
                return PGENULL;
            }
            getConsole().OO();
        } // try
        catch (const std::exception& e)
        {
            // note: textures won't be deallocated cause we don't track what textures were loaded for this object!
            delete tmpMesh;
            delete subobject;  // will invoke obj->Detach()
            delete obj;     // would delete subobj, except if it is not yet attached, so we explicitly delete subobj above
            getConsole().OO();
            getConsole().EOLnOO("ERROR: failed to instantiate: %s!", e.what());
            getConsole().OLn("");
            return PGENULL;
        } // catch
    }    
    else
    {
        getConsole().EOLnOO("ERROR: unsupported extension!");
        getConsole().OLn("");
        return PGENULL;
    }

    getConsole().SOLnOO("> Object loaded successfully, name: %s!", obj->getName().c_str());
    getConsole().OLn("");
    return obj; 
} // createFromFile()


/**
    Creates object from the given file.
    Equals to calling createFromFile(filename, Pure_VMOD_STATIC, Pure_VREF_INDEXED, false).

    @param filename The model file to be loaded to be an Object3D instance.

    @return The created object.
            PGENULL if Object3DManager is not yet initialized or the specified file does not exist or the specified file format is not known.
*/
PureObject3D* PureObject3DManager::createFromFile(const char* filename)
{
    return createFromFile(filename, Pure_VMOD_STATIC, Pure_VREF_INDEXED, false);    
} // createFromFile()


/**
    Creates a new object by cloning an already existing object.
    The clone will have its own properties such as position, visibility, etc. but the geometry will not be cloned, just referenced.
    Useful for saving memory on geometry data on both client and server side.
    Note that it is the user's responsibility to delete all clones of an object before trying to delete the original object.

    @return A clone instance.
            PGENULL if Object3DManager is not yet initialized, or if memory allocation issue happens, or when vertex transfer mode cannot be set.
*/
PureObject3D* PureObject3DManager::createCloned(PureObject3D& referredobj)
{
    // referredobj cannot be const ref, since we need to store to non-const ptr, so later non-const Draw() can be invoked on it.
    // so the reason of referredobj not const is because Draw() is non-const.

    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI(
        "PureObject3DManager::createCloned(\"%s\")",
        (referredobj.getFilename().length() > 0) ? referredobj.getFilename().c_str() : referredobj.getName().c_str() );

    PureObject3D* obj = PGENULL;
    try
    {
        obj = new PureObject3D(pImpl->materialMgr);
        // do not attach obj to object3dmanager yet, we need material to be created first!

        // SetName() invoked later below ...
        obj->SetFilename( referredobj.getFilename() );
        
        obj->pImpl->pRefersto = &referredobj;
        referredobj.pImpl->referrers.insert(obj);

        obj->getPosVec() = referredobj.getPosVec();
        // we dont need to copy vRelPos, since getRelPosVec() is overridden in Object3D to work for cloned objects too!
        obj->pImpl->vAngle = referredobj.getAngleVec();
        obj->pImpl->bAffectedByLights = referredobj.isLit();
        obj->pImpl->bDoubleSided = referredobj.isDoubleSided();
        obj->pImpl->bWireframe = referredobj.isWireframed();
        obj->pImpl->bWireframedCull = referredobj.isWireframedCulled();
        obj->pImpl->bAffectZBuffer = referredobj.isAffectingZBuffer();
        obj->pImpl->vScaling = referredobj.getScaling();
        obj->pImpl->fBiggestAreaScaled = referredobj.getBiggestAreaScaled();
        obj->pImpl->bVisible = referredobj.isRenderingAllowed();
        obj->pImpl->rotation = referredobj.getRotationOrder();
        obj->pImpl->bStickedToScreen = referredobj.isStickedToScreen();

        PureMesh3DManager* const me = this;
        if ( me->pImpl->createMaterialForMesh(*obj) == PGENULL )
        {
            // this should not be like this, createMaterialForMesh() should throw.
            throw std::bad_alloc();
        }

        // since we have material now, we can attach it to object3dmanager!
        Attach( *obj );

        // for the material, no need to copy texcoords, etc ...
        // however we need to set blendfunc and envcolor, because renderer would not get referredobj blendfunc ...
        // probably this should be treated as bug of renderer, however I fix it from objectmanager side here.
        if ( !obj->getMaterial(false).setBlendFuncs( referredobj.getMaterial(false).getSourceBlendFunc(), referredobj.getMaterial(false).getDestinationBlendFunc() ) )
        {
            throw std::logic_error("setBlendFuncs() failed with referredobj's values!");
        }
        obj->getMaterial(false).getTextureEnvColor().Set(
            referredobj.getMaterial(false).getTextureEnvColor().getRed(),
            referredobj.getMaterial(false).getTextureEnvColor().getGreen(),
            referredobj.getMaterial(false).getTextureEnvColor().getBlue(),
            referredobj.getMaterial(false).getTextureEnvColor().getAlpha() );

        obj->SetName("Object3D " + std::to_string(pImpl->nRunningCounter++) + " (clone of " + referredobj.getName() + ")");

        obj->SetOccluder(referredobj.isOccluder());
        if ( referredobj.getBoundingBoxObject() )
        {
            obj->SetOcclusionTested(true);
        }
        
        getConsole().SOLnOO("> Object cloned successfully, name: %s!", obj->getName().c_str());
    } // try
    catch (const std::exception& e)
    {
        delete obj; // if new PureObject3D() fails, obj is null so this has no effect, but if createMaterialForMesh() fails, this will properly get rid of obj
        // PrepareObjectForOcclusionQuery() could also throw, in such case obj's dtor will also take care of any leftovers made by PrepareObjectForOcclusionQuery()
        obj = PGENULL;
        getConsole().EOLnOO("ERROR: failed to instantiate PureObject3D: %s!", e.what());
    }

    getConsole().OLn("");
    return obj;
} // createCloned()


/**
    Iterates over its manageds and updates their occluder states.
    It uses their getBiggestAreaScaled() values to determine which should be occluder.
    It sets states on and off, based on how their getBiggestAreaScaled() compare to others'.
    Objects having any of the following properties are ignored during calculation since they cannot even be occluders:
     - hidden by user;
     - wireframed;
     - having blended material;
     - not affecting z-buffer;
     - sticked-to-screen.

    A renderer might invoke this function periodically based on its configuration.
*/
void PureObject3DManager::UpdateOccluderStates()
{
    //getConsole().SetLoggingState(PureObject3DManager::getLoggerModuleName(), true);

    TPurefloat fAverageBiggestAreaScaled = 0.0f;
    TPureuint nCount = 0;
    for (TPureint i = 0; i < getSize(); i++)
    {
        const PureObject3D* const pMngd = (PureObject3D*) getAttachedAt(i);
        if ( pMngd != PGENULL )
        {
            if ( !pMngd->isRenderingAllowed() )
                continue;

            const TPurebool bOpaque = !PureMaterial::isBlendFuncReallyBlending(pMngd->getMaterial(false).getSourceBlendFunc(), pMngd->getMaterial(false).getDestinationBlendFunc() );

            if ( !bOpaque )
                continue;

            if ( pMngd->isStickedToScreen() )
                continue;

            if ( !pMngd->isAffectingZBuffer() )
                continue;

            if ( pMngd->isWireframed() )
                continue;

            nCount++;
            fAverageBiggestAreaScaled += pMngd->getBiggestAreaScaled();
        }
    }
    fAverageBiggestAreaScaled /= (TPurefloat)nCount;

    //getConsole().OLnOI("%s() START: avg area: %f", __FUNCTION__, fAverageBiggestAreaScaled);
    //getConsole().OLn("area, bAreaBiggerThanAvg, name, filename");
    std::deque<PureObject3D*> occluderCandidates;
    for (TPureint i = 0; i < getSize(); i++)
    {
        PureObject3D* const pMngd = (PureObject3D*) getAttachedAt(i);
        if ( pMngd != PGENULL )
        {
            if ( !pMngd->isRenderingAllowed() )
                continue;

            const TPurebool bOpaque = !PureMaterial::isBlendFuncReallyBlending(pMngd->getMaterial(false).getSourceBlendFunc(), pMngd->getMaterial(false).getDestinationBlendFunc() );

            if ( !bOpaque )
                continue;

            if ( pMngd->isStickedToScreen() )
                continue;

            if ( !pMngd->isAffectingZBuffer() )
                continue;

            if ( pMngd->isWireframed() )
                continue;

            const TPurebool bAreaBiggerThanAvg = (pMngd->getBiggestAreaScaled() > (fAverageBiggestAreaScaled * pImpl->fOccluderSelectionBias));

            //getConsole().OLn("%f, %b, %s, %s", pMngd->getBiggestAreaScaled(), bAreaBiggerThanAvg, pMngd->getName().c_str(), pMngd->getFilename().c_str());            
            
            if ( bAreaBiggerThanAvg )
            {
                // dont let it be occluder yet at this point, just put in the candidates list, order by biggest scaled area, from bigger to smaller
                auto it = occluderCandidates.begin();
                while ( it != occluderCandidates.end() )
                {
                    if ( (*it)->getBiggestAreaScaled() > pMngd->getBiggestAreaScaled() )
                    {
                        it++;
                    }
                    else
                    {
                        break;
                    }
                }
                occluderCandidates.insert(it, pMngd);
            }
            else
            {
                pMngd->SetOccluder(false);
            }
        }
    }
    //getConsole().OOOLn("%s() END", __FUNCTION__);

    // now we really set occluder state for the first nMaxOccluderCount element of the candidates list
    TPureuint occCntr = 0;
    auto it = occluderCandidates.begin();
    while ( it != occluderCandidates.end() )
    {
        if ( occCntr < pImpl->nMaxOccluderCount )
        {
            //getConsole().OLn("%f, %s, %s", (*it)->getBiggestAreaScaled(), (*it)->getName().c_str(), (*it)->getFilename().c_str());
            (*it)->SetOccluder(true);
        }
        else
        {
            (*it)->SetOccluder(false);
        }
        occCntr++;
        it++;
    }

    //getConsole().SetLoggingState(PureObject3DManager::getLoggerModuleName(), false);
} // UpdateOccluderStates()


/**
    Gets the occluder selection bias.
    See details at SetOccluderSelectionBias().
*/
TPurefloat PureObject3DManager::getOccluderSelectionBias() const
{
    return pImpl->fOccluderSelectionBias;
} // getOccluderSelectionBias()


/**
    Sets the occluder selection bias.
    By default objects are selected as occluders if their biggest scaled area size is bigger than the average scaled area size of all objects.
    If for any reason this is not suitable for your scene, you can set a bias value with this function.
    This bias will be multiplied by the average scaled area size of all objects when comparison will happen.
    So, if you want fewer objects to be selected as occluder, set a bigger bias value.
    If you want more objects to be selected as occluder, set a lower bias value.
    The default value is 1.0f.

    @param fBias The new occluder selection bias.
*/
void PureObject3DManager::SetOccluderSelectionBias(TPurefloat fBias)
{
    if ( fBias < 0.f )
        return;

    pImpl->fOccluderSelectionBias = fBias;
} // SetOccluderSelectionBias()


/**
    Gets the maximum number of occluders.
    See details at SetMaxOccluderCount().
*/
TPureuint PureObject3DManager::getMaxOccluderCount() const
{
    return pImpl->nMaxOccluderCount;
} // getMaxOccluderCount()


/**
    Sets the maximum number of occluders.
    In some circumstances you might change the maximum number of occluders because for different scenes you might need different number of occluders
    to achieve higher rendering performance. Without this limit, UpdateOccluderStates() might choose too many objects as occluder based on their sizes,
    however those objects won't be checked for occlusion, but you might want them to act as occludees instead of occluders.
    Objects with biggest scaled area will be set for occluders. This means that if you set this maximum number to 2, then the 2 objects having the 2 biggest
    scaled area will be selected as occluder.
    Setting this value to 0 means there won't be any occluder selected.
    Default value is 4.
*/
void PureObject3DManager::SetMaxOccluderCount(TPureuint nMax)
{
    pImpl->nMaxOccluderCount = nMax;
} // SetMaxOccluderCount()


/**
    Should be invoked when a managed's property got changed from a different kind of manager or managed.
    PureObject3DManager is interested in some property changes related to its managed PureObject3D instances, e.g. blend mode that might change in a PureMaterial instance.
*/
void PureObject3DManager::HandleManagedPropertyChanged(PureManaged& m)
{
    try
    {
        PureObject3D& obj = dynamic_cast<PureObject3D&>(m);
        
        // originally we invoked SetOccluder(false) only when bBlended was true, as seen below, however
        // the problem with that is we need to invoke that also when bBlended became false, because
        // SetOccluder() is the only function which properly removes the object and inserts it from/to
        // the containers.
        // So now we rather not check blending, but always invoke it, to have effect also when blended
        // is turned off.
        // This approach doesn't do any harm, but probably unnecessarily slow.
        // Would be good if we knew WHICH property has been changed and from what to what value.
        //const TPurebool bBlended = PureMaterial::isBlendFuncReallyBlending(obj.getMaterial(false).getSourceBlendFunc(), obj.getMaterial(false).getDestinationBlendFunc());
        //
        //if ( bBlended )
        //{
        //    obj.SetOccluder(false);    
        //}
        obj.SetOccluder(false);
        // Note that we just cannot set occluder state to true here because only UpdateOccluderStates() is aware of Object3Ds' average biggestarea!
    }
    catch (const std::exception&)
    {
        getConsole().EOLn("WARNING: PureObject3DManager::HandleManagedPropertyChanged() param is NOT Object3D ref!");
    }

    PureMesh3DManager::HandleManagedPropertyChanged(m);
} // HandleManagedPropertyChanged()


/**
    Saves and resets current statistics.
    Useful if you want to restart measurements.
    The saved statistics can be logged logged by WriteList() anytime later.
*/
void PureObject3DManager::ResetStatistics()
{
    getConsole().OLnOI("PureObject3DManager::ResetStatistics()");
    getConsole().OLn("Following statistics are being reset:");

    getConsole().OLn("Sync: timeLongestGlobalWaitForSyncQueryFinish: %u s %u us",
        PureObject3D::PureObject3DImpl::stats[PureObject3D::PureObject3DImpl::stats.size()-1].timeLongestGlobalWaitForSyncQueryFinish.tv_sec,
        PureObject3D::PureObject3DImpl::stats[PureObject3D::PureObject3DImpl::stats.size()-1].timeLongestGlobalWaitForSyncQueryFinish.tv_usec);
    getConsole().OLn("ASync: nFramesWaited4OcclTestResGlobal: min: %u, max: %u;",
        PureObject3D::PureObject3DImpl::stats[PureObject3D::PureObject3DImpl::stats.size()-1].nFramesWaitedForOcclusionTestResultGlobalMin,
        PureObject3D::PureObject3DImpl::stats[PureObject3D::PureObject3DImpl::stats.size()-1].nFramesWaitedForOcclusionTestResultGlobalMax); 
    getConsole().OLn("");
    getConsole().OO();

    PureObject3D::PureObject3DImpl::stats.push_back( PureObject3D::PureObject3DImpl::CurrentStats() );
    
    // object-local occlusion query stats
    for (TPureint i = 0; i < getSize(); i++)
    {
        const PureObject3D* const pMngd = (PureObject3D*) getAttachedAt(i);
        if ( pMngd != PGENULL )
        {
            pMngd->pImpl->timeLongestWaitForSyncQueryFinish.tv_sec = LONG_MIN;
            pMngd->pImpl->timeLongestWaitForSyncQueryFinish.tv_usec = LONG_MIN;
            pMngd->pImpl->nFramesWaitedForOcclusionTestResultMin = UINT_MAX;
            pMngd->pImpl->nFramesWaitedForOcclusionTestResultMax = 0;
        }
    }

} // ResetStatistics()


/**
    Gets the amount of allocated video memory for all objects owned by this manager.

    @return The summarized allocated video memory for all objects owned by this manager.
*/
TPureuint PureObject3DManager::getUsedVideoMemory() const
{
    TPureuint nVRAMtotal = 0;
    for (TPureint i = 0; i < getSize(); i++)
    {
        const PureObject3D* const pMngd = (PureObject3D*) getAttachedAt(i);
        if ( pMngd != PGENULL )
            nVRAMtotal += pMngd->getUsedVideoMemory();
    }
    return nVRAMtotal;
} // getUsedVideoMemory()


/**
    From PureMesh3DManager, adding logging vertex transfer and object data, including some occlusion query stats.
*/
void PureObject3DManager::WriteList() const
{
    getConsole().OLn("PureObject3DManager::WriteList()");
    getConsole().OLn("===============================");
    getConsole().OLnOI("");
    PureMesh3DManager::WriteList();
    getConsole().OO();

    TPureuint nVRAMtotal = getUsedVideoMemory();

    getConsole().OI();

    getConsole().OLn("> total used video memory = %d Bytes <= %d kBytes <= %d MBytes", nVRAMtotal, (int)(ceil(nVRAMtotal/1024.0f)), (int)(ceil(nVRAMtotal/1024.0f/1024.0f)));
    getConsole().OLn("");

    getConsole().OLn("Collected statistics:");
    for (TPureuint i = 0; i < PureObject3D::PureObject3DImpl::stats.size(); i++)
    {
        const PureObject3D::PureObject3DImpl::CurrentStats& iStat = PureObject3D::PureObject3DImpl::stats[i]; 
        getConsole().OLn("Stats %u:", i+1);
        getConsole().OLn("Sync: timeLongestGlobalWaitForSyncQueryFinish: %u s %u us",
            iStat.timeLongestGlobalWaitForSyncQueryFinish.tv_sec,
            iStat.timeLongestGlobalWaitForSyncQueryFinish.tv_usec);
        getConsole().OLn("ASync: nFramesWaited4OcclTestResGlobal: min: %u, max: %u;",
            iStat.nFramesWaitedForOcclusionTestResultGlobalMin,
            iStat.nFramesWaitedForOcclusionTestResultGlobalMax); 
        getConsole().OLn("");
    }

    getConsole().OO();
    getConsole().OLn("");
} // WriteList()


// ############################## PROTECTED ##############################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PureObject3DManager::PureObject3DManager()
{

}


PureObject3DManager::PureObject3DManager(const PureObject3DManager&)
{

}


PureObject3DManager& PureObject3DManager::operator=(const PureObject3DManager&)
{
    return *this;    
}


void PureObject3DManager::WriteListCallback(const PureManaged& mngd) const
{
    PureMesh3DManager::WriteListCallback(mngd);
    const PureObject3D& obj = (PureObject3D&) mngd;
    getConsole().OIOLnOO("scaled size: [%f, %f, %f]", obj.getScaledSizeVec().getX(), obj.getScaledSizeVec().getY(), obj.getScaledSizeVec().getZ());
    getConsole().OIOLnOO("biggest area: %f", obj.getBiggestAreaScaled());
    getConsole().OIOLnOO("show: %b, xfer: %d, indexed: %b, vid mem: %d kB;",
        obj.isRenderingAllowed(),
        obj.getVertexTransferMode(),
        PureVertexTransfer::isVertexReferencingIndexed(obj.getVertexTransferMode()),
        (int)ceil(obj.getUsedVideoMemory()/1024.0f));
    getConsole().OIOLnOO("dblSided: %b, wirefr: %b, writeZ: %b, testZ: %b;", obj.isDoubleSided(), obj.isWireframed(), obj.isAffectingZBuffer(), obj.isTestingAgainstZBuffer());
    getConsole().OIOLnOO("occlTest: %b, qId: %d, occludee: %b, occluded: %b;", obj.isOcclusionTested(), obj.pImpl->nOcclusionQuery, !(obj.isOccluder()), obj.isOccluded());
    if ( obj.isOcclusionTested() )
    {
        getConsole().OIOLnOO("Sync: timeLongestWaitForSyncQueryFinish: %u s %u us;", obj.pImpl->timeLongestWaitForSyncQueryFinish.tv_sec, obj.pImpl->timeLongestWaitForSyncQueryFinish.tv_usec); 
        getConsole().OIOLnOO("ASync: nFramesWaited4OcclTestRes: min: %u, max: %u;", obj.pImpl->nFramesWaitedForOcclusionTestResultMin, obj.pImpl->nFramesWaitedForOcclusionTestResultMax); 
    }
    
    if ( obj.pImpl->pBoundingBox )
    {
        getConsole().OIOLnOO("It has bounding box for occlusion query:");
        getConsole().Indent();
        WriteListCallback( *(obj.pImpl->pBoundingBox) );
        getConsole().Outdent();
    }
    else
    {
        getConsole().OLn("");
    }
    
} // WriteListCallback()


// ############################### PRIVATE ###############################


