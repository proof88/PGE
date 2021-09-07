/*
    ###################################################################################
    PRREObject3DManager.cpp
    This file is part of PRRE.
    PRREObject3DManager class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################                                                
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Object3D/PRREObject3DManager.h"
#include "../../include/internal/Object3D/PRREObject3DImpl.h"
#include "../../include/internal/Object3D/PRREMesh3DImpl.h"
#include "../../include/internal/Object3D/PRREMesh3DManagerImpl.h"
#include "../../include/external/Hardware/PRREhwInfo.h"
#include "../../include/internal/PRREGLextensionFuncs.h"
#include "../../include/internal/PRREGLsafeFuncs.h"
#include "../../include/internal/PRREGLsnippets.h"
#include "../../include/internal/PRREpragmas.h"

using namespace std;

/*
   PRREObject3DManager::PRREObject3DManagerImpl
   ###########################################################################
*/


class PRREObject3DManager::PRREObject3DManagerImpl
{

public:
    virtual ~PRREObject3DManagerImpl();

    TPRREbool isInitialized() const;

    const std::set<PRREObject3D*>& getOccluders() const;
    const std::set<PRREObject3D*>& getOpaqueOccludees() const;
    const std::set<PRREObject3D*>& getBlendedOccludees() const;

protected:

private:

    static TPRREuint nRunningCounter;    /**< Always increased when creating a new level-1 Object3D instance. */

    static TPRREbool isEligibleForOcclusionQuery(TPRREuint nTotalVerticesCount);  /**< Decides if an object is eligible for automatically turn on occlusion query on it. */

    PRREObject3DManager* _pOwner;        /**< The owner public object who creates this pimpl object. */

    TPRREbool            bInited;               /**< True if successfully inited, false if not functional. */
    TPRREbool            bMinimalIndexStorage;  /**< True if storage of indices is minimalized. */
    PRRETextureManager&  textureMgr;            /**< Used to auto-load textures for OBJ files. */
    PRREMaterialManager& materialMgr;           /**< Used to auto-load/create materials for objects. */

    // ---------------------------------------------------------------------------

    PRREObject3DManagerImpl();
    PRREObject3DManagerImpl(
        PRREObject3DManager* owner,
        PRRETextureManager& texMgr,
        PRREMaterialManager& matMgr );

    PRREObject3DManagerImpl(const PRREObject3DManagerImpl&);
    PRREObject3DManagerImpl& operator=(const PRREObject3DManagerImpl&);

    friend class PRREObject3DManager;

};


// ############################### PUBLIC ################################


PRREObject3DManager::PRREObject3DManagerImpl::~PRREObject3DManagerImpl()
{
    _pOwner->getConsole().OLnOI("~PRREObject3DManagerImpl() ...");
    _pOwner->DeleteAll();
    _pOwner->getConsole().OO();    
    _pOwner = NULL;
} // ~PRREObject3DManager()


TPRREbool PRREObject3DManager::PRREObject3DManagerImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


const std::set<PRREObject3D*>& PRREObject3DManager::PRREObject3DManagerImpl::getOccluders() const
{
    return PRREObject3D::PRREObject3DImpl::occluders;
} // getOccluders()


const std::set<PRREObject3D*>& PRREObject3DManager::PRREObject3DManagerImpl::getOpaqueOccludees() const
{
    return PRREObject3D::PRREObject3DImpl::occludees_opaque;
} // getOpaqueOccludees()


const std::set<PRREObject3D*>& PRREObject3DManager::PRREObject3DManagerImpl::getBlendedOccludees() const
{
    return PRREObject3D::PRREObject3DImpl::occludees_blended;
} // getBlendedOccludees()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


TPRREuint PRREObject3DManager::PRREObject3DManagerImpl::nRunningCounter = 0;


/**
    Decides if an object is eligible for automatically turn on occlusion query on it.
    @return True if object should be prepared for occlusion querying, false otherwise.
*/
TPRREbool PRREObject3DManager::PRREObject3DManagerImpl::isEligibleForOcclusionQuery(TPRREuint nTotalVerticesCount)
{
    return nTotalVerticesCount >= 100;
} // isEligibleForOcclusionQuery()


PRREObject3DManager::PRREObject3DManagerImpl::PRREObject3DManagerImpl() :
    textureMgr(textureMgr),
    materialMgr(materialMgr)
{

}


/**
    @param owner  Public class instance owning this pimpl object.
    @param texMgr A TextureManager instance to be used for loading textures used by Object3D instances.
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.
*/
PRREObject3DManager::PRREObject3DManagerImpl::PRREObject3DManagerImpl(PRREObject3DManager* owner, PRRETextureManager& texMgr, PRREMaterialManager& matMgr) :
    textureMgr(texMgr),
    materialMgr(matMgr)
{
    _pOwner = owner;
    _pOwner->getConsole().OLnOI("PRREObject3DManagerImpl() ...");
    bInited = false;
    bMinimalIndexStorage = true;
    // we rely on texture manager initialized state since that also relies on initialized state of hwinfo
    bInited = texMgr.isInitialized();
    _pOwner->getConsole().SOLnOO("Done!");
} // PRREObject3DManager(...)


PRREObject3DManager::PRREObject3DManagerImpl::PRREObject3DManagerImpl(const PRREObject3DManager::PRREObject3DManagerImpl&) :
    textureMgr(textureMgr),
    materialMgr(materialMgr)
{

}


PRREObject3DManager::PRREObject3DManagerImpl& PRREObject3DManager::PRREObject3DManagerImpl::operator=(const PRREObject3DManager::PRREObject3DManagerImpl&)
{
    return *this;    
}


/*
   PRREObject3DManager
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    @param texMgr A TextureManager instance to be used for loading textures used by Object3D instances.
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.

    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRREObject3DManager::PRREObject3DManager(PRRETextureManager& texMgr, PRREMaterialManager& matMgr)
    : PRREMesh3DManager(matMgr)
{
    pImpl = new PRREObject3DManagerImpl(this, texMgr, matMgr);
} // PRREObject3DManager(...)


PRREObject3DManager::~PRREObject3DManager()
{
    delete pImpl;
    pImpl = NULL;
} // ~PRREObject3DManager()


/**
    Adds the given managed to the manager, if the managed has no manager yet.
    No effect if given managed already has a manager.
    The object will be present in getOpaqueOccludees() list since it will be attached as non-blended.
*/
void PRREObject3DManager::Attach(PRREManaged& m)
{
    if ( m.getManager() != PGENULL )
        return;

    PRREMesh3DManager::Attach(m);
    try
    {
        PRREObject3D& obj = dynamic_cast<PRREObject3D&>(m);

        // it will be an occludee for sure, since:
        // - either this is a brand new object attached first time to its manager,
        // - or Detach() had previously removed it from occludees_* and occluders lists, and without manager it cannot be set as occluder so it stayed unchanged for sure!
        obj.SetOccluder(false);
    }
    catch (const std::exception&)
    {
        getConsole().OLn("WARNING: PRREObject3DManager::Attach() param is NOT Object3D ref!");
    }
} // Attach()


/**
    Removes the given managed from the manager, so the managed will have no manager.
    The object will not be occlusion tested, and won't be an occluder, and actually it will NOT be present in any of the following lists:
    getOccluders(), getOpaqueOccludees() and getBlendedOccludees().
*/
void PRREObject3DManager::Detach(PRREManaged& m)
{
    if ( m.getManager() == PGENULL )
        return;

    try
    {
        PRREObject3D& obj = dynamic_cast<PRREObject3D&>(m);
        obj.SetOcclusionTested(false);
        obj.SetOccluder(false);
        obj.getMaterial(false).setBlendMode(PRRE_BM_NONE);

        auto occ_it = std::find(PRREObject3D::PRREObject3DImpl::occludees_opaque.begin(), PRREObject3D::PRREObject3DImpl::occludees_opaque.end(), &obj);
        if ( occ_it != PRREObject3D::PRREObject3DImpl::occludees_opaque.end() )
        {
            PRREObject3D::PRREObject3DImpl::occludees_opaque.erase(occ_it);
        }
        else
        {
            occ_it = std::find(PRREObject3D::PRREObject3DImpl::occludees_blended.begin(), PRREObject3D::PRREObject3DImpl::occludees_blended.end(), &obj);
            if ( occ_it != PRREObject3D::PRREObject3DImpl::occludees_blended.end() )
            {
                PRREObject3D::PRREObject3DImpl::occludees_blended.erase(occ_it);
            }
        }
    }
    catch (const std::exception&)
    {
        // no need to log
        //getConsole().OLn("WARNING: PRREObject3DManager::Detach() param is NOT Object3D ref!");
    }

    PRREMesh3DManager::Detach(m);
} // Detach()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRREObject3DManager::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRREObject3DManager::getLoggerModuleName()
{
    return "PRREObject3DManager";
} // getLoggerModuleName()


/**
    Tells whether the manager is correctly initialized or not.
    @return True if the given Object3DManager instance was initialized correctly, false otherwise.
*/
TPRREbool PRREObject3DManager::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


/**
    Get a list of occluders.
    If you want to get all the occluders, this way is faster than iterating over all objects and checking if they are occluders.

    @return List of occluders.
*/
const std::set<PRREObject3D*>& PRREObject3DManager::getOccluders() const
{
    return pImpl->getOccluders();
} // getOccluders()


/**
    Get a list of opaque (non-blended) occludees.
    If you want to get all the opaque occludees, this way is faster than iterating over all objects and checking if they are non-blended and occludees.

    @return List of opaque occludees.
*/
const std::set<PRREObject3D*>& PRREObject3DManager::getOpaqueOccludees() const
{
    return pImpl->getOpaqueOccludees();
} // getOpaqueOccludees()


/**
    Get a list of blended occludees.
    If you want to get all the blended occludees, this way is faster than iterating over all objects and checking if they are blended and occludees.

    @return List of blended occludees.
*/
const std::set<PRREObject3D*>& PRREObject3DManager::getBlendedOccludees() const
{
    return pImpl->getBlendedOccludees();
} // getBlendedOccludees()


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
PRREObject3D* PRREObject3DManager::createPlane(
    TPRREfloat a, TPRREfloat b,
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory)
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREObject3DManager::createPlane(%f, %f)", a, b);

    PRREObject3D* obj = PGENULL;
    PRREObject3D* subobj = PGENULL;
    try
    {
        obj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
        // do not attach obj to object3dmanager yet, we need its material created first!

        subobj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
        obj->Attach( *subobj );

        PRREMesh3DManager* const me = this;
        if ( !(me->pImpl->convertToPlane(*obj, a, b)) )
        {
            throw std::runtime_error("convertToPlane() failed!");
        }

        // now we have material, it can be attached to object3dmanager!
        Attach( *obj );

        subobj->pImpl->pVerticesTransf = new TPRRE_TRANSFORMED_VERTEX[subobj->getVerticesCount()];
    }
    catch (const std::exception& e)
    {
        delete subobj; // will invoke obj->Detach()
        delete obj;    // would delete subobj, except if it is not yet attached, so we explicitly delete subobj above
        getConsole().EOLnOO("ERROR: PRREObject3DManager::createPlane() failed to instantiate or fill PRREObject3D: %s!", e.what());
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
PRREObject3D* PRREObject3DManager::createBox(
    TPRREfloat a, TPRREfloat b, TPRREfloat c,
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory )
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREObject3DManager::createBox(%f, %f, %f)", a, b, c);

    PRREObject3D* obj = PGENULL;
    PRREObject3D* subobj = PGENULL;
    try
    {
        obj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
        // do not attach obj to object3dmanager yet, we need its material created first!

        subobj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
        obj->Attach( *subobj );

        PRREMesh3DManager* const me = this;
        if ( !me->pImpl->convertToBox(*obj, a, b, c) )
        {
            throw std::runtime_error("convertToBox() failed!");
        }

        // now we have material, it can be attached to object3dmanager!
        Attach( *obj );

        subobj->pImpl->pVerticesTransf = new TPRRE_TRANSFORMED_VERTEX[subobj->getVerticesCount()];
    }
    catch (const std::exception& e)
    {
        delete subobj; // will invoke obj->Detach()
        delete obj;    // would delete subobj, except if it is not yet attached, so we explicitly delete subobj above
        getConsole().EOLn("ERROR: PRREObject3DManager::createPlane() failed to instantiate or fill PRREObject3D: %s!", e.what());
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
PRREObject3D* PRREObject3DManager::createCube(
    TPRREfloat a,
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory )
{
    getConsole().OLnOI("PRREObject3DManager::createCube(%f), passing to createBox() ...", a);
    PRREObject3D* const cube = createBox(a, a, a, vmod, vref, bForceUseClientMemory);
    if ( PGENULL == cube )
    {
        getConsole().EOLnOO("ERROR: PRREObject3DManager::createBox() failed!");
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
PRREObject3D* PRREObject3DManager::createFromFile(
    const char* filename,
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory )
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREObject3DManager::createFromFile(\"%s\")", filename);

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

    PRREObject3D* obj = PGENULL;
    PRREObject3D* subobject = PGENULL;
    PRREMesh3D* tmpMesh = PGENULL;
    transform(sFileExt.begin(), sFileExt.end(), sFileExt.begin(), ::toupper);
    getConsole().OLn("ext: .%s", sFileExt.c_str());
    if ( sFileExt == "OBJ" )
    {
        try
        {
            getConsole().OI();
            tmpMesh = PRREMesh3DManager::createFromFile(filename);
            if ( !tmpMesh )
            {
                getConsole().OO();
                getConsole().EOLnOO("ERROR: failed to create tmpMesh!");
                getConsole().OLn("");
                return PGENULL;
            }
            obj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
            if ( !((PRREMesh3D*)obj)->pImpl->cannibalize(*tmpMesh) )
            {
                const std::string sErrMsg = "cannibalize() failed for level-1 obj!";
                throw std::runtime_error(sErrMsg);
            }
            obj->SetName("Object3D " + std::to_string(pImpl->nRunningCounter++));

            TPRREuint nVerticesTotalTmpMesh = 0;
            for (TPRREint i = 0; i < tmpMesh->getCount(); i++) 
            {
                subobject = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
                obj->Attach( *subobject );
                if ( !((PRREMesh3D*)subobject)->pImpl->cannibalize(*(PRREMesh3D*)(tmpMesh->getAttachedAt(i))) )
                {
                    const std::string sErrMsg = "cannibalize() failed for level-2 subobj " + std::to_string(i) + "!";
                    throw std::runtime_error(sErrMsg);
                }
                subobject->pImpl->pVerticesTransf = new TPRRE_TRANSFORMED_VERTEX[subobject->getVerticesCount()];
                nVerticesTotalTmpMesh += subobject->getVerticesCount();
                
                // Legacy tmcsgfxlib behavior: auto-load textures for subobjects where pipe character is present in name
                const std::string::size_type nPipePos = subobject->getName().find('|');
                if ( nPipePos != std::string::npos )
                {
                    const std::string sTexName = subobject->getName().substr(nPipePos+1);
                    getConsole().OLn("Legacy: loading texture implicitly by submodel name: %s", sTexName.c_str());
                    PRRETexture* const tex = pImpl->textureMgr.createFromFile( PFL::getDirectory(filename).append(sTexName).c_str() );
                    if ( tex )
                    {
                        if ( !subobject->getMaterial().setTexture(tex) )
                        {
                            getConsole().EOLn("ERROR: failed to set texture for subobject! Continuing ...");
                        }
                    }
                    else
                    {
                        getConsole().EOLn("ERROR: PRREObject3DManager::createFromFile() failed to load texture: %s! Continuing ...", sTexName.c_str());
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
            if ( PRREhwInfo::get().getVideo().isOcclusionQuerySupported() && pImpl->isEligibleForOcclusionQuery(nVerticesTotalTmpMesh) )
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
    Equals to calling createFromFile(filename, PRRE_VMOD_STATIC, PRRE_VREF_INDEXED, false).

    @param filename The model file to be loaded to be an Object3D instance.

    @return The created object.
            PGENULL if Object3DManager is not yet initialized or the specified file does not exist or the specified file format is not known.
*/
PRREObject3D* PRREObject3DManager::createFromFile(const char* filename)
{
    return createFromFile(filename, PRRE_VMOD_STATIC, PRRE_VREF_INDEXED, false);    
} // createFromFile()


/**
    Creates a new object by cloning an already existing object.
    The clone will have its own properties such as position, visibility, etc. but the geometry will not be cloned, just referenced.
    Useful for saving memory on geometry data on both client and server side.

    @return A clone instance.
            PGENULL if Object3DManager is not yet initialized, or if memory allocation issue happens, or when vertex transfer mode cannot be set.
*/
PRREObject3D* PRREObject3DManager::createCloned(PRREObject3D& referredobj)
{
    // referredobj cannot be const ref, since we need to store to non-const ptr, so later non-const Draw() can be invoked on it.
    // so the reason of referredobj not const is because Draw() is non-const.

    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI(
        "PRREObject3DManager::createCloned(\"%s\")",
        (referredobj.getFilename().length() > 0) ? referredobj.getFilename().c_str() : referredobj.getName().c_str() );

    PRREObject3D* obj = PGENULL;
    try
    {
        obj = new PRREObject3D(pImpl->materialMgr);
        // do not attach obj to object3dmanager yet, we need material to be created first!

        // SetName() invoked later below ...
        obj->SetFilename( referredobj.getFilename() );
        
        obj->pImpl->pRefersto = &referredobj;

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
        obj->pImpl->bVisible = referredobj.isVisible();
        obj->pImpl->rotation = referredobj.getRotationOrder();
        obj->pImpl->bStickedToScreen = referredobj.isStickedToScreen();

        PRREMesh3DManager* const me = this;
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
        obj->getMaterial().getTextureEnvColor().Set(
            referredobj.getMaterial().getTextureEnvColor().getRed(),
            referredobj.getMaterial().getTextureEnvColor().getGreen(),
            referredobj.getMaterial().getTextureEnvColor().getBlue(),
            referredobj.getMaterial().getTextureEnvColor().getAlpha() );

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
        delete obj; // if new PRREObject3D() fails, obj is null so this has no effect, but if createMaterialForMesh() fails, this will properly get rid of obj
        // PrepareObjectForOcclusionQuery() could also throw, in such case obj's dtor will also take care of any leftovers made by PrepareObjectForOcclusionQuery()
        obj = PGENULL;
        getConsole().EOLnOO("ERROR: failed to instantiate PRREObject3D: %s!", e.what());
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
void PRREObject3DManager::UpdateOccluderStates()
{
    //getConsole().SetLoggingState(PRREObject3DManager::getLoggerModuleName(), true);

    TPRREfloat fAverageBiggestAreaScaled = 0.0f;
    TPRREuint nCount = 0;
    for (TPRREint i = 0; i < getSize(); i++)
    {
        const PRREObject3D* const pMngd = (PRREObject3D*) getAttachedAt(i);
        if ( pMngd != PGENULL )
        {
            if ( !pMngd->isVisible() )
                continue;

            const TPRREbool bOpaque = !PRREMaterial::isBlendFuncReallyBlending(pMngd->getMaterial(false).getSourceBlendFunc(), pMngd->getMaterial(false).getDestinationBlendFunc() );

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
    fAverageBiggestAreaScaled /= (TPRREfloat)nCount;

    //getConsole().OLnOI("%s() START: avg area: %f", __FUNCTION__, fAverageBiggestAreaScaled);
    //getConsole().OLn("area, bAreaBiggerThanAvg, name, filename");
    for (TPRREint i = 0; i < getSize(); i++)
    {
        PRREObject3D* const pMngd = (PRREObject3D*) getAttachedAt(i);
        if ( pMngd != PGENULL )
        {
            if ( !pMngd->isVisible() )
                continue;

            const TPRREbool bOpaque = !PRREMaterial::isBlendFuncReallyBlending(pMngd->getMaterial(false).getSourceBlendFunc(), pMngd->getMaterial(false).getDestinationBlendFunc() );

            if ( !bOpaque )
                continue;

            if ( pMngd->isStickedToScreen() )
                continue;

            if ( !pMngd->isAffectingZBuffer() )
                continue;

            if ( pMngd->isWireframed() )
                continue;

            const TPRREbool bAreaBiggerThanAvg = (pMngd->getBiggestAreaScaled() > fAverageBiggestAreaScaled);

            //getConsole().OLn("%f, %b, %s, %s", pMngd->getBiggestAreaScaled(), bAreaBiggerThanAvg, pMngd->getName().c_str(), pMngd->getFilename().c_str());
            
            pMngd->SetOccluder(bAreaBiggerThanAvg);
        }
    }
    //getConsole().OOOLn("%s() END", __FUNCTION__);

    //getConsole().SetLoggingState(PRREObject3DManager::getLoggerModuleName(), false);
} // UpdateOccluderStates()


/**
    Should be invoked when a managed's property got changed from a different kind of manager or managed.
    PRREObject3DManager is interested in some property changes related to its managed PRREObject3D instances, e.g. blend mode that might change in a PRREMaterial instance.
*/
void PRREObject3DManager::HandleManagedPropertyChanged(PRREManaged& m)
{
    try
    {
        PRREObject3D& obj = dynamic_cast<PRREObject3D&>(m);
        
        const TPRREbool bBlended = PRREMaterial::isBlendFuncReallyBlending(obj.getMaterial(false).getSourceBlendFunc(), obj.getMaterial(false).getDestinationBlendFunc());

        if ( bBlended )
        {
            obj.SetOccluder(false);
        }
        // obj.SetOccluder(true) in different cases will be handled by UpdateOccluderStates() when invoked periodically by renderer, we just cannot
        // set occluder state to true here because only UpdateOccluderStates() is aware of Object3Ds' average biggestarea!
    }
    catch (const std::exception&)
    {
        getConsole().OLn("WARNING: PRREObject3DManager::HandleManagedPropertyChanged() param is NOT Object3D ref!");
    }

    PRREMesh3DManager::HandleManagedPropertyChanged(m);
} // HandleManagedPropertyChanged()


/**
    Gets the amount of allocated video memory for all objects owned by this manager.

    @return The summarized allocated video memory for all objects owned by this manager.
*/
TPRREuint PRREObject3DManager::getUsedVideoMemory() const
{
    TPRREuint nVRAMtotal = 0;
    for (TPRREint i = 0; i < getSize(); i++)
    {
        const PRREObject3D* const pMngd = (PRREObject3D*) getAttachedAt(i);
        if ( pMngd != PGENULL )
            nVRAMtotal += pMngd->getUsedVideoMemory();
    }
    return nVRAMtotal;
} // getUsedVideoMemory()


void PRREObject3DManager::WriteList() const
{
    getConsole().OLn("PRREObject3DManager::WriteList()");
    getConsole().OLn("===============================");
    getConsole().OLnOI("");
    PRREMesh3DManager::WriteList();
    getConsole().OO();

    TPRREuint nVRAMtotal = getUsedVideoMemory();

    getConsole().OIOLnOO("> total used video memory = %d Bytes <= %d kBytes <= %d MBytes", nVRAMtotal, (int)(ceil(nVRAMtotal/1024.0f)), (int)(ceil(nVRAMtotal/1024.0f/1024.0f)));
    getConsole().OLn("");
} // WriteList()


// ############################## PROTECTED ##############################


/**
    @exception std::bad_alloc - This class or its ancestor dynamically allocates memory with operator new, in case of failure the exception is not handled but propagated to caller.
*/
PRREObject3DManager::PRREObject3DManager()
{

}


PRREObject3DManager::PRREObject3DManager(const PRREObject3DManager&)
{

}


PRREObject3DManager& PRREObject3DManager::operator=(const PRREObject3DManager&)
{
    return *this;    
}


void PRREObject3DManager::WriteListCallback(const PRREManaged& mngd) const
{
    PRREMesh3DManager::WriteListCallback(mngd);
    const PRREObject3D& obj = (PRREObject3D&) mngd;
    getConsole().OIOLnOO("scaled size: [%f, %f, %f]", obj.getScaledSizeVec().getX(), obj.getScaledSizeVec().getY(), obj.getScaledSizeVec().getZ());
    getConsole().OIOLnOO("biggest area: %f", obj.getBiggestAreaScaled());
    getConsole().OIOLnOO("show: %b, xfer: %d, indexed: %b, vid mem: %d kB;",
        obj.isVisible(),
        obj.getVertexTransferMode(),
        PRREVertexTransfer::isVertexReferencingIndexed(obj.getVertexTransferMode()),
        (int)ceil(obj.getUsedVideoMemory()/1024.0f));
    getConsole().OIOLnOO("dblSided: %b, wirefr: %b, writeZ: %b, testZ: %b;", obj.isDoubleSided(), obj.isWireframed(), obj.isAffectingZBuffer(), obj.isTestingAgainstZBuffer());
    getConsole().OIOLnOO("occlTest: %b, qId: %d, occludee: %b, occluded: %b;", obj.isOcclusionTested(), obj.pImpl->nOcclusionQuery, !(obj.isOccluder()), obj.isOccluded());
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


