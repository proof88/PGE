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
#include "../PRREpragmas.h"
#include "PRREObject3DManager.h"
#include "PRREObject3DImpl.h"
#include "../PRREGLextensionFuncs.h"
#include "../PRREGLsnippets.h"
#include "../Hardware/PRREhwInfo.h"

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
    _pOwner->getConsole().OLnOI("~PRREObject3DManager() ...");
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
    _pOwner->getConsole().OLnOI("PRREObject3DManager() ...");
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
    Tells whether the given Vertex Transfer Mode is available on the current hardware.
    The following modes are always available with accelerated renderer:
    - dynamic modifying:
      - direct referencing:  PRRE_VT_DYN_DIR_1_BY_1, PRRE_VT_DYN_DIR_RVA
      - indexed referencing: PRRE_VT_DYN_IND_1_BY_1, PRRE_VT_DYN_IND_RVA
    - static modifying:
      - direct referencing:  PRRE_VT_STA_DIR_DL
      - indexed referencing: PRRE_VT_STA_IND_DL

    The following modes are always available with non-accelerated renderer:
    - dynamic modifying:
      - direct referencing:  PRRE_VT_DYN_DIR_1_BY_1
      - indexed referencing: PRRE_VT_DYN_IND_1_BY_1
    - static modifying:
      - direct referencing:  same as for dynamic modifying above
      - indexed referencing: same as for dynamic modifying above

    @return True if the given transfer mode is available on the current hardware, false otherwise.
*/
TPRREbool PRREObject3DManager::isVertexTransferModeSelectable(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
    {   /* only these 2 modes are supported in non-accelerated mode */
        if ( (vtrans == (PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT)) ||
             (vtrans == (PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED)) )
           return true;
        else
           return false;
    }

    /* at this point we are accelerated */

    /* currently not supporting any vendor-specific mode */
    if ( BITF_READ(vtrans,PRRE_VT_VENDOR_BITS,3) != 0 )
        return false;

    /* first need to check for CVA and RNG capability, as both may apply to VBO */

    if ( BIT_READ(vtrans, PRRE_VT_RNG_BIT) && !PRREhwInfo::get().getVideo().isDrawRangeElementsSupported()  )
        return false;

    if ( BIT_READ(vtrans, PRRE_VT_CVA_BIT) && !PRREhwInfo::get().getVideo().isCompiledVertexArraySupported()  )
        return false;

    if ( BIT_READ(vtrans, PRRE_VT_SVA_BIT) && !PRREhwInfo::get().getVideo().isVertexBufferObjectSupported()  )
        return false;

    return true;
} // isVertexTransferModeSelectable()


/**
    Tells whether the given Vertex Transfer Mode references vertices by indexing.
    @return True if the given mode references vertices by indexing.
*/
TPRREbool PRREObject3DManager::isVertexReferencingIndexed(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    return BIT_READ(vtrans, PRRE_VT_VREF_BIT) == 1u;
} // isVertexReferencingIndexed()


/**
    Tells whether the given Vertex Transfer Mode means dynamic modifying habit.
    @return True if the given mode means dynamic modifying habit.
*/
TPRREbool PRREObject3DManager::isVertexModifyingDynamic(TPRRE_VERTEX_TRANSFER_MODE vtrans)
{
    return BIT_READ(vtrans, PRRE_VT_VMOD_BIT) == 1u;
} // isVertexModifyingDynamic()


/**
    Selects a suitable vertex transfer mode.
    The selected mode is compatible with the current hardware and complies with the given arguments.
    Please note that bForceUseClientMemory = true is considered only if dynamic modifying habit is specified.
    Specifying static modifying habit will always select a mode which places geometry data into server memory.
    
    @return A suitable vertex transfer mode selected by PPP based on the given arguments and
            the current hardware capabilities.
*/
TPRRE_VERTEX_TRANSFER_MODE PRREObject3DManager::selectVertexTransferMode(
    TPRRE_VERTEX_MODIFYING_HABIT vmod,
    TPRRE_VERTEX_REFERENCING_MODE vref,
    TPRREbool bForceUseClientMemory)
{
    TPRRE_VERTEX_TRANSFER_MODE ret = 0;

    if ( vref == PRRE_VREF_INDEXED )
        ret = ret | PRRE_VREF_INDEXED;

    // dont go further if CPU is rendering
    if ( !PRREhwInfo::get().getVideo().isAcceleratorDetected() )
        return ret | PRRE_VMOD_DYNAMIC;

    if ( vmod == PRRE_VMOD_DYNAMIC )
        ret = ret | PRRE_VMOD_DYNAMIC;

    if ( PRREhwInfo::get().getVideo().isDrawRangeElementsSupported() )
        ret = ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_RNG_BIT);

    if ( PRREhwInfo::get().getVideo().isCompiledVertexArraySupported() )
        ret = ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_CVA_BIT);

    // bForceUseClientMemory is considered only if modifying habit is dynamic
    if ( bForceUseClientMemory && (vmod == PRRE_VMOD_DYNAMIC) )
        return ret | BIT(PRRE_VT_VA_BIT); // client-side should use an array, even if none of RNG and CVA bits are available, to avoid selecting immediate mode

    // At this point we want to use host/server memory, but it could be still either client- or server-side array, or display list or immediate mode, so
    // we should set PRRE_VT_VA_BIT only in case of some arraying.

    // if general VBO is supported, return with that immediately as we don't want to stick to any vendor-specific magical stuff.
    if ( PRREhwInfo::get().getVideo().isVertexBufferObjectSupported() )
        return ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);

    if ( PRREhwInfo::get().getVideo().isATIVertexArrayObjectSupported() )
    {
        if ( PRREhwInfo::get().getVideo().isATIElementArraySupported() )
            ret = ret | BIT(PRRE_VT_RNG_BIT) | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT) | BITF_PREP(PRRE_VT_VENDOR_ATI_2,PRRE_VT_VENDOR_BITS,3);
        else
            ret = ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT) | BITF_PREP(PRRE_VT_VENDOR_ATI_1,PRRE_VT_VENDOR_BITS,3);
    }
    else
    {
        if ( PRREhwInfo::get().getVideo().isNVVertexArrayRangeSupported() )
        {
            if ( PRREhwInfo::get().getVideo().isNVElementArraySupported() )
                ret = ret | BIT(PRRE_VT_RNG_BIT) | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT) | BITF_PREP(PRRE_VT_VENDOR_NVIDIA_2,PRRE_VT_VENDOR_BITS,3);
            else
                ret = ret | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT) | BITF_PREP(PRRE_VT_VENDOR_NVIDIA_1,PRRE_VT_VENDOR_BITS,3);
        }
    }

    // at this point, if VA_BIT is not set, the selected mode is either display list or immediate mode, depending on vmod

    return ret;
} // selectVertexTransferMode()


/**
    @param texMgr A TextureManager instance to be used for loading textures used by Object3D instances.
    @param matMgr A MaterialManager instance to be used for creating materials used by Object3D instances.
*/
PRREObject3DManager::PRREObject3DManager(PRRETextureManager& texMgr, PRREMaterialManager& matMgr)
    : PRREMesh3DManager(matMgr)
{
    p = new PRREObject3DManagerImpl(this, texMgr, matMgr);
} // PRREObject3DManager(...)


PRREObject3DManager::~PRREObject3DManager()
{
    delete p;
    p = NULL;
} // ~PRREObject3DManager()


/**
    Tells whether the object is correctly initialized or not.
    @return True if the given TextureManager instance was initialized correctly, false otherwise.
*/
TPRREbool PRREObject3DManager::isInitialized() const
{
    return p->isInitialized();
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
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREObject3DManager::createPlane(%f, %f)", a, b);

    PRREObject3D* const obj = new PRREObject3D(vmod, vref, bForceUseClientMemory);
    Attach( *obj );
    PRREObject3D* const subobj = new PRREObject3D(vmod, vref, bForceUseClientMemory);
    obj->Attach( *subobj );
    ConvertToPlane(*obj, a, b);
    obj->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
    subobj->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
    subobj->p->pVerticesTransf = (TPRRE_TRANSFORMED_VERTEX*) malloc( sizeof(TPRRE_TRANSFORMED_VERTEX) * subobj->getVerticesCount() );

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
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI("PRREObject3DManager::createBox(%f, %f, %f)", a, b, c);

    PRREObject3D* const obj = new PRREObject3D(vmod, vref, bForceUseClientMemory);
    Attach( *obj );
    PRREObject3D* const subobj = new PRREObject3D(vmod, vref, bForceUseClientMemory);
    obj->Attach( *subobj );
    ConvertToBox(*obj, a, b, c);
    obj->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
    subobj->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
    subobj->p->pVerticesTransf = (TPRRE_TRANSFORMED_VERTEX*) malloc( sizeof(TPRRE_TRANSFORMED_VERTEX) * subobj->getVerticesCount() );

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
    if ( !p->isInitialized() )
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
        obj = new PRREObject3D(vmod, vref, bForceUseClientMemory);
        obj->Cannibalize(*tmpMesh);
        obj->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );

        for (TPRREint i = 0; i < tmpMesh->getCount(); i++) 
        {
            PRREObject3D* const subobject = new PRREObject3D(vmod, vref, bForceUseClientMemory);
            obj->Attach( *subobject );
            subobject->Cannibalize(*(PRREMesh3D*)(tmpMesh->getAttachedAt(i)));
            subobject->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );
            subobject->p->pVerticesTransf = (TPRRE_TRANSFORMED_VERTEX*) malloc( sizeof(TPRRE_TRANSFORMED_VERTEX) * subobject->getVerticesCount() );
            
            // Legacy tmcsgfxlib behavior: auto-load textures for subobjects where pipe character is present in name
            const std::string::size_type nPipePos = subobject->getName().find('|');
            if ( nPipePos != std::string::npos )
            {
                const std::string sTexName = subobject->getName().substr(nPipePos+1);
                getConsole().OLn("Legacy: loading texture implicitly by submodel name: %s", sTexName.c_str());
                PRRETexture* const tex = p->textureMgr.createFromFile( PFL::getDirectory(filename).append(sTexName).c_str() );
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
    if ( !p->isInitialized() )
    {
        return PGENULL;
    }

    getConsole().OLnOI(
        "PRREObject3DManager::createCloned(\"%s\")",
        (referredobj.getFilename().length() > 0) ? referredobj.getFilename().c_str() : referredobj.getName().c_str() );

    PRREObject3D* obj = new PRREObject3D();

    obj->SetName( referredobj.getName() );
    obj->SetFilename( referredobj.getFilename() );
    
    obj->p->vertexTransferMode = referredobj.getVertexTransferMode();
    obj->p->pRefersto = &referredobj;
    obj->getPosVec() = referredobj.getPosVec();
    obj->p->vAngle = referredobj.getAngleVec();

    obj->p->bAffectedByLights = referredobj.isLit();
    obj->p->bDoubleSided = referredobj.isDoubleSided();
    obj->p->bWireframe = referredobj.isWireframed();
    obj->p->bWireframedCull = referredobj.isWireframedCulled();
    obj->p->bAffectZBuffer = referredobj.isAffectingZBuffer();
    obj->p->vScaling = referredobj.getScaling();
    obj->p->bVisible = referredobj.isVisible();
    obj->p->rotation = referredobj.getRotationOrder();
    obj->p->bStickedToScreen = referredobj.isStickedToScreen();
    obj->p->vertexTransferMode = referredobj.getVertexTransferMode();

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

    obj->p->nTexcoordsVBO.resize( p->materialMgr.getMaximumLayerCount() );

    Attach( *obj );

    getConsole().SOLnOO("> Object cloned successfully!");
    getConsole().OLn("");
    return obj;
}


// ############################## PROTECTED ##############################


PRREObject3DManager::PRREObject3DManager() :
    PRREMesh3DManager()
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


