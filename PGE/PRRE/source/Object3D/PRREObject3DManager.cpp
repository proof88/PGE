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
#include "../../include/external/Hardware/PRREhwInfo.h"
#include "../../include/internal/PRREGLextensionFuncs.h"
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

protected:

private:

    PRREObject3DManager* _pOwner;      /**< The owner public object who creates this pimpl object. */

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


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


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
    Gets whether the given source and destination blend factors really mean blending or not.
    If source factor is PRRE_ONE and destination factor is PRRE_ZERO, there's no blending even if blending is enabled.

    @return True if the given blend factors really mean blending, false otherwise.
*/
TPRREbool PRREObject3DManager::isBlendFuncBlends(TPRRE_BLENDFACTORS sfactor, TPRRE_BLENDFACTORS dfactor)
{
    return !( (sfactor == PRRE_ONE) && (dfactor == PRRE_ZERO) );
} // isBlendFuncBlends()


/**
    Same as isBlendFuncBlends().
    @return True if the given blend factors really mean blending, false otherwise.
*/
TPRREbool PRREObject3DManager::isBFB(TPRRE_BLENDFACTORS sfactor, TPRRE_BLENDFACTORS dfactor)
{
    return isBlendFuncBlends(sfactor, dfactor);
} // isBFB()


/**
    @param texMgr A TextureManager instance to be used for loading textures used by Object3D instances.
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.
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
    Tells whether the object is correctly initialized or not.
    @return True if the given TextureManager instance was initialized correctly, false otherwise.
*/
TPRREbool PRREObject3DManager::isInitialized() const
{
    return pImpl->isInitialized();
} // isInitialized()


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

    PRREObject3D* const obj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
    Attach( *obj );
    PRREObject3D* const subobj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
    obj->Attach( *subobj );
    ConvertToPlane(*obj, a, b);
    subobj->pImpl->pVerticesTransf = (TPRRE_TRANSFORMED_VERTEX*) malloc( sizeof(TPRRE_TRANSFORMED_VERTEX) * subobj->getVerticesCount() );

    // although PPP has already selected the vtransmode, we set it again
    // to actually allocate the needed resources for the geometry
    getConsole().OI();
    obj->SetVertexTransferMode( obj->getVertexTransferMode() );
    getConsole().OO();

    getConsole().SOLnOO("> Plane created successfully!");
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

    PRREObject3D* const obj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
    Attach( *obj );
    PRREObject3D* const subobj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
    obj->Attach( *subobj );
    ConvertToBox(*obj, a, b, c);
    subobj->pImpl->pVerticesTransf = (TPRRE_TRANSFORMED_VERTEX*) malloc( sizeof(TPRRE_TRANSFORMED_VERTEX) * subobj->getVerticesCount() );

    // although PPP has already selected the vtransmode, we set it again
    // to actually allocate the needed resources for the geometry
    getConsole().OI();
    obj->SetVertexTransferMode( obj->getVertexTransferMode() );
    getConsole().OO();

    getConsole().SOLnOO("> Box created successfully!");
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
    getConsole().SOLnOO("> Cube created successfully!");
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

    PRREObject3D* obj = PGENULL;

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

    transform(sFileExt.begin(), sFileExt.end(), sFileExt.begin(), ::toupper);
    getConsole().OLn("ext: .%s", sFileExt.c_str());
    if ( sFileExt == "OBJ" )
    {
        getConsole().OI();
        PRREMesh3D* const tmpMesh = PRREMesh3DManager::createFromFile(filename);
        obj = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
        obj->Cannibalize(*tmpMesh);

        for (TPRREint i = 0; i < tmpMesh->getCount(); i++) 
        {
            PRREObject3D* const subobject = new PRREObject3D(pImpl->materialMgr, vmod, vref, bForceUseClientMemory);
            obj->Attach( *subobject );
            subobject->Cannibalize(*(PRREMesh3D*)(tmpMesh->getAttachedAt(i)));
            subobject->pImpl->pVerticesTransf = (TPRRE_TRANSFORMED_VERTEX*) malloc( sizeof(TPRRE_TRANSFORMED_VERTEX) * subobject->getVerticesCount() );
            
            // Legacy tmcsgfxlib behavior: auto-load textures for subobjects where pipe character is present in name
            const std::string::size_type nPipePos = subobject->getName().find('|');
            if ( nPipePos != std::string::npos )
            {
                const std::string sTexName = subobject->getName().substr(nPipePos+1);
                getConsole().OLn("Legacy: loading texture implicitly by submodel name: %s", sTexName.c_str());
                PRRETexture* const tex = pImpl->textureMgr.createFromFile( PFL::getDirectory(filename).append(sTexName).c_str() );
                if ( tex )
                {
                    subobject->getMaterial().SetTexture(tex);
                }
                // since we have loaded texture from submodelname, we can get rid of the texture filename part of it
                subobject->SetName( subobject->getName().substr(0, nPipePos) ); 
            }
        }
        this->DeleteAttachedInstance(*tmpMesh);

        obj->SetFilename(filename);
        getConsole().OO();
        Attach( *obj );

        // although PPP has already selected the vtransmode, we set it again
        // to actually allocate the needed resources for the geometry
        getConsole().OI();
        obj->SetVertexTransferMode( obj->getVertexTransferMode() );
        getConsole().OO();
    }    
    else
    {
        getConsole().EOLnOO("ERROR: unsupported extension!");
        getConsole().OLn("");
        return PGENULL;
    }

    getConsole().SOLnOO("> Object loaded successfully!");
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
            PGENULL if Object3DManager is not yet initialized.
*/
PRREObject3D* PRREObject3DManager::createCloned(PRREObject3D& referredobj)
{
    if ( !pImpl->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI(
        "PRREObject3DManager::createCloned(\"%s\")",
        (referredobj.getFilename().length() > 0) ? referredobj.getFilename().c_str() : referredobj.getName().c_str() );

    PRREObject3D* obj = new PRREObject3D(pImpl->materialMgr);

    obj->SetName( referredobj.getName() );
    obj->SetFilename( referredobj.getFilename() );
    
    // attach right now, otherwise SetVertexTransferMode() would fail
    Attach( *obj );
    obj->SetVertexTransferMode(referredobj.getVertexTransferMode());
    // SetVertexTransferMode() would fail if referred object would be set earlier
    obj->pImpl->pRefersto = &referredobj;

    obj->getPosVec() = referredobj.getPosVec();
    obj->pImpl->vAngle = referredobj.getAngleVec();
    obj->pImpl->bAffectedByLights = referredobj.isLit();
    obj->pImpl->bDoubleSided = referredobj.isDoubleSided();
    obj->pImpl->bWireframe = referredobj.isWireframed();
    obj->pImpl->bWireframedCull = referredobj.isWireframedCulled();
    obj->pImpl->bAffectZBuffer = referredobj.isAffectingZBuffer();
    obj->pImpl->vScaling = referredobj.getScaling();
    obj->pImpl->bVisible = referredobj.isVisible();
    obj->pImpl->rotation = referredobj.getRotationOrder();
    obj->pImpl->bStickedToScreen = referredobj.isStickedToScreen();

    CreateMaterialForMesh(*obj);
    // for the material, no need to copy texcoords, etc ...
    // however we need to set blendfunc and envcolor, because renderer would not get referredobj blendfunc ...
    // probably this should be treated as bug of renderer, however I fix it from objectmanager side here.
    obj->getMaterial().SetBlendFuncs( referredobj.getMaterial().getSourceBlendFunc(), referredobj.getMaterial().getDestinationBlendFunc() );
    obj->getMaterial().getTextureEnvColor().Set(
        referredobj.getMaterial().getTextureEnvColor().getRed(),
        referredobj.getMaterial().getTextureEnvColor().getGreen(),
        referredobj.getMaterial().getTextureEnvColor().getBlue(),
        referredobj.getMaterial().getTextureEnvColor().getAlpha() );

    

    getConsole().SOLnOO("> Object cloned successfully!");
    getConsole().OLn("");
    return obj;
}


// ############################## PROTECTED ##############################


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


// ############################### PRIVATE ###############################


