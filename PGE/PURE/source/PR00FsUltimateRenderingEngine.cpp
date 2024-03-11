/*
    ###################################################################################
    PR00FsUltimateRenderingEngine.cpp
    This file is part of PURE.
    PR00F's Ultimate Rendering Engine
    Made by PR00F88
    ###################################################################################
*/


#include "PureBaseIncludes.h"  // PCH

#include "../include/external/PR00FsUltimateRenderingEngine.h"
#include "../include/external/Render/PureRendererHWfixedPipe.h"
#include "../include/external/Render/PureRendererSWincremental.h"
#include "../include/internal/PureGLsnippets.h"
#include "../include/internal/PureGLextensionFuncs.h"
#include "../include/internal/PurePragmas.h"
#include "../include/internal/Display/PureSharedSettings.h"


using namespace std;


/*
   PR00FsUltimateRenderingEngineImpl
   ###########################################################################
*/

class PR00FsUltimateRenderingEngineImpl :
    public PR00FsUltimateRenderingEngine
{
public:
    virtual ~PR00FsUltimateRenderingEngineImpl();       /**< Calls shutdown(). */

    TPureUInt initialize(
        TPURE_RENDERER rndr,
        TPureUInt width, TPureUInt height,
        TPURE_DISPLAY_MODES dmode,
        TPureUInt freq, TPureInt cdepth,
        TPureInt zdepth, TPureInt stencil,
        TPureInt samples,
        HWND window = NULL);         
    TPureBool shutdown();            
    TPureBool isInitialized() const;         
    
    PureScreen&           getScreen() const;         
    PureWindow&           getWindow() const;        
    PureHwInfo&           getHardwareInfo() const;     
    PureImageManager&     getImageManager() const;  
    PureTextureManager&   getTextureManager() const;  
    PureMaterialManager&  getMaterialManager() const;
    PureMesh3DManager&    getMesh3DManager() const; 
    PureObject3DManager&  getObject3DManager() const; 
    PureCamera&           getCamera() const;      
    PureUiManager&        getUImanager() const;
    PureIRenderer*        getRenderer() const;
    void                  setGuiDrawCallback(const std::function<void()>& cb);

    void                  WriteList() const;

    void                  CopyScreenToTexture(PureTexture& tex);
    
    void PrintTexturesUnusedByMaterials() const;
    void PrintMaterialsUnusedByMeshes() const;
    void PrintMaterialsUnusedByObjects() const;
    void WriteStats() const;     
    TPureBool getAutoWriteStatsAtShutdown() const;
    void SetAutoWriteStatsAtShutdown(TPureBool state);

private:
    PGEcfgProfiles&      m_cfgProfiles;
    PGEInputHandler&     m_inputHandler;
    PureSharedSettings&  ssets;        /**< Shared settings, singleton. */
    PureWindow&          wnd;          /**< Our window, where we draw to, singleton. */
    PureHwInfo&          hwInfo;       /**< Hardware infos, singleton. */
    PureScreen&          screen;       /**< Our screen, singleton. */
    PureImageManager*    pImageMgr;    /**< ImageManager, at least 1 instance. */
    PureTextureManager*  pTextureMgr;  /**< TextureManager, at least 1 instance. */
    PureMaterialManager* pMaterialMgr; /**< MaterialManager, at least 1 instance. */
    PureMesh3DManager*   pMesh3DMgr;   /**< MeshManager, at least 1 instance. */
    PureObject3DManager* pObject3DMgr; /**< ObjectManager, at least 1 instance. */
    PureCamera*          pCamera;      /**< Camera, at least 1 instance. */
    PureUiManager&       uiMgr;        /**< UI manager, singleton. */
    PureIRenderer*       pRenderer;    /**< Renderer, singleton. */
    TPureBool            bAutoWriteStatsAtShutdown;  /** If true, will invoke WriteStats() at the beginning of shutdown(). Default false. */

    // ---------------------------------------------------------------------------

    explicit PR00FsUltimateRenderingEngineImpl(
        PGEcfgProfiles& cfgProfiles,
        PGEInputHandler& inputHandler);                /**< NULLs members only. */
    PR00FsUltimateRenderingEngineImpl(const PR00FsUltimateRenderingEngineImpl&);
    PR00FsUltimateRenderingEngineImpl& operator=(const PR00FsUltimateRenderingEngineImpl&);

    friend class PR00FsUltimateRenderingEngine;

};


// ############################### PUBLIC ################################


/**
    Calls shutdown().
*/
PR00FsUltimateRenderingEngineImpl::~PR00FsUltimateRenderingEngineImpl()
{
    shutdown();
} // ~PR00FsUltimateRenderingEngine()


/**
    Initializes the engine.
    Tries to set the given display mode, then initializes OpenGL.
 
    @param rndr    Which renderer to use.
    @param width   See initialize() of the selected renderer.
    @param height  See initialize() of the selected renderer.
    @param dmode   See initialize() of the selected renderer.
    @param freq    See initialize() of the selected renderer.
    @param cdepth  See initialize() of the selected renderer.
    @param zdepth  See initialize() of the selected renderer.
    @param stencil See initialize() of the selected renderer.
    @param samples See initialize() of the selected renderer.
    @param window  See initialize() of the selected renderer.

    @return The result of the initialization. 0 on success, positive value otherwise. Currently 1 for any error.
*/
TPureUInt PR00FsUltimateRenderingEngineImpl::initialize(
    TPURE_RENDERER rndr,
    TPureUInt width, TPureUInt height,
    TPURE_DISPLAY_MODES dmode, TPureUInt freq,
    TPureInt cdepth, TPureInt zdepth, TPureInt stencil,
    TPureInt samples,
    HWND window)
{
    TPureInt st = stencil;

    getConsole().OLn("Pure::initialize(%d, %d, %d, dmode, %d, %d, %d, %d, %d)",
                      rndr, width, height, freq, cdepth, zdepth, st, samples);
    if ( isInitialized() )
    {
        getConsole().SOLn("  > already initialized!");
        return 1;
    }

    getConsole().OIOLn( PURE_NAME );
    getConsole().OLn( PURE_VERSION );

    switch (rndr)
    {
    case PURE_RENDERER_SW:
        pRenderer = &PureRendererSWincremental::createAndGet(m_cfgProfiles, wnd, screen, hwInfo);
        break;
    case PURE_RENDERER_HW_FP:
        pRenderer = &PureRendererHWfixedPipe::createAndGet(m_cfgProfiles, wnd, screen, hwInfo);
        break;
    default:
        getConsole().EOLnOO("ERROR: invalid renderer specified!");
        return 1;
    }

    if ( pRenderer->initialize(width, height, dmode, freq, cdepth, zdepth, stencil, samples, window) != 0 )
    {
        getConsole().EOLnOO("ERROR: failed to initialize renderer!");
        return 1;
    }

    // OpenGL is fully operational at this point so managers can be instantiated
    // Note in case of SW rendering there is no OpenGL, but managers are able to detect that condition so we are safe here.
    try
    {
        // I know this error handling is a joke!
        // the try is added to catch the exceptions might be thrown by new,
        // but since the ctors are not throwing YET, I need to check isInitialized() after each other.
        // obviously I will add exception throwing to ctors and then this code will be much cleaner.

        pImageMgr = new PureImageManager();

        pTextureMgr  = new PureTextureManager();
        if ( !pTextureMgr->isInitialized() )
        {
            getConsole().EOLn("ERROR: failed to initialize PureTextureManager!");
            shutdown();
            getConsole().OO();
            return 1;
        }

        pMaterialMgr = new PureMaterialManager();
        if ( !pMaterialMgr->isInitialized() )
        {
            getConsole().EOLn("ERROR: failed to initialize PureMaterialManager!");
            shutdown();
            getConsole().OO();
            return 1;
        }

        pMesh3DMgr = new PureMesh3DManager(*pMaterialMgr);
        if ( !pMesh3DMgr->isInitialized() )
        {
            getConsole().EOLn("ERROR: failed to initialize PureMesh3DManager!");
            shutdown();
            getConsole().OO();
            return 1;
        }

        pObject3DMgr = new PureObject3DManager(*pTextureMgr, *pMaterialMgr);
        if ( !pObject3DMgr->isInitialized() )
        {
            getConsole().EOLn("ERROR: failed to initialize PureObject3DManager!");
            shutdown();
            getConsole().OO();
            return 1;
        }

        pCamera = new PureCamera();
        // initialize camera viewport to window client area
        // we can trust window clientrect size even if this is an external window
        pCamera->SetViewport(0, 0, wnd.getClientWidth(), wnd.getClientHeight());
        pCamera->SetAspectRatio(wnd.getClientWidth() / (TPureFloat) wnd.getClientHeight());
    } // try
    catch (const std::bad_alloc&)
    {
        getConsole().EOLn("ERROR: failed to instantiate a manager!");
        shutdown();
        getConsole().OO();
        return 1;
    }

    uiMgr.Initialize( wnd.getWndHandle(), wnd.getWndDC());

    pRenderer->SetManagers(pObject3DMgr, &uiMgr, pCamera);

    // PURE is fully operational at this point
    getConsole().OLn("");
    getConsole().SOLn("> PURE initialized");
    getConsole().OLnOO("");

    return 0;

} // initialize()


/**
    This stops the engine.
    Deletes everything created by the engine (window, textures, models, etc...).
    No effect before initialization.
    After shutdown, initialize() can be called again.

    @return True on successful shutdown, false otherwise.
*/
TPureBool PR00FsUltimateRenderingEngineImpl::shutdown()
{
    getConsole().OLn("PR00FsUltimateRenderingEngine::shutdown()");
    getConsole().OLn("");

    if ( getAutoWriteStatsAtShutdown() )
    {
        getConsole().SetLoggingState("4LLM0DUL3S", true);
        getConsole().OIOLn("Dumping states ...");
        getConsole().OLn("");
        
        WriteList();
        getConsole().OLnOO("");
        
        getConsole().SetLoggingState("4LLM0DUL3S", false);
    }

    getConsole().OIOLn("Shutting down subsystems ...");
    getConsole().OLn("");
    getConsole().OI();

    uiMgr.Deinitialize();

    // managed objects like objects, textures, etc are implicitly deleted by calling dtor of their managers below
    delete pCamera;
    pCamera = PGENULL;

    delete pObject3DMgr;
    pObject3DMgr = PGENULL;

    delete pMesh3DMgr;
    pMesh3DMgr = PGENULL;

    delete pMaterialMgr;
    pMaterialMgr = PGENULL;

    delete pTextureMgr;
    pTextureMgr = PGENULL;

    delete pImageMgr;
    pImageMgr = PGENULL;

    bool success = true;

    // since renderer owns GL context, it must be shut down last, so managers still have the chance above to properly
    // delete any GL resources
    if ( pRenderer )
    {
        if (!( success = pRenderer->shutdown() ))
            getConsole().EOLnOO("ERROR: failed to shut down renderer!");
        pRenderer = PGENULL;
    }   

    // theoretically window, screen, and hwinfo are already deinitialized by renderer, however we need to take care of proper housekeeping
    wnd.Deinitialize();
    screen.ResetDisplaySettings();
    hwInfo.Deinitialize();

    getConsole().OO();

    getConsole().OLnOO("END OF SHUTDOWN!");

    return success;
} // shutdown()


/**
    Gets the state of the engine.
    @return True if the engine is successfully initialized, false before initialization or after shutdown.
*/
TPureBool PR00FsUltimateRenderingEngineImpl::isInitialized() const
{
    return ( pRenderer && pRenderer->isInitialized() );
} // isInitialized()

 
/** 
    Get access to screen manipulation functions.

    @return Reference to PureScreen object if engine is initialized, invalid reference otherwise.
*/
PureScreen& PR00FsUltimateRenderingEngineImpl::getScreen() const
{
    return screen;
} // getScreen()


/** 
    Get access to window manipulation functions.

    @return Reference to PureWindow object if engine is initialized, invalid reference otherwise.
*/
PureWindow& PR00FsUltimateRenderingEngineImpl::getWindow() const
{
    return wnd;
} // getWindow()


/**
    Get access to hardware information.

    @return Reference to PureHwInfo object if engine is initialized, invalid reference otherwise.
*/
PureHwInfo& PR00FsUltimateRenderingEngineImpl::getHardwareInfo() const
{
    return hwInfo;
} // getHardwareInfo()


/** 
    Get access to image handler functions.

    @return Reference to PureImageManager object if engine is initialized, invalid reference otherwise.
*/
PureImageManager& PR00FsUltimateRenderingEngineImpl::getImageManager() const
{
    return *pImageMgr;
} // getImageManager()


/**
    Get access to texture handler functions.

    @return Reference to PureTextureManager object if engine is initialized, invalid reference otherwise.
*/
PureTextureManager& PR00FsUltimateRenderingEngineImpl::getTextureManager() const
{
    return *pTextureMgr;
} // getTextureManager()


/**
    Get access to material handler functions.

    @return Reference to PureMaterialManager object if engine is initialized, invalid reference otherwise.
*/
PureMaterialManager& PR00FsUltimateRenderingEngineImpl::getMaterialManager() const
{
    return *pMaterialMgr;
}


PureMesh3DManager& PR00FsUltimateRenderingEngineImpl::getMesh3DManager() const
{
    return *pMesh3DMgr;
}


/** 
    Get access to Object3D handler functions.

    @return Reference to PureObject3DManager object if engine is initialized, invalid reference otherwise.
*/
PureObject3DManager& PR00FsUltimateRenderingEngineImpl::getObject3DManager() const
{
    return *pObject3DMgr;
} // getObject3DManager()


/**
    Get access to camera handler functions.

    @return Reference to PureCamera object if engine is initialized, invalid reference otherwise.
*/
PureCamera& PR00FsUltimateRenderingEngineImpl::getCamera() const
{
    return *pCamera;
} // getCamera()


/**
    Get access to user interface functions.

    @return Reference to PureUiManager object if engine is initialized, invalid reference otherwise.
*/
PureUiManager& PR00FsUltimateRenderingEngineImpl::getUImanager() const
{
    return uiMgr;
} // getUImanager()


/**
    Get access to the selected renderer.

    @return Pointer to a renderer object implementing PureIRenderer interface if engine is initialized, NULL otherwise.
*/
PureIRenderer* PR00FsUltimateRenderingEngineImpl::getRenderer() const
{
    return pRenderer;
}


void PR00FsUltimateRenderingEngineImpl::PrintTexturesUnusedByMaterials() const
{
    if ( !isInitialized() )
        return;

    const PureTextureManager& texMgr = getTextureManager();
    const PureMaterialManager& matMgr = getMaterialManager();
    
    getConsole().OLn("List of Textures Unused by Materials");
    getConsole().OLn("====================================");

    TPureUInt nUnusedTextures = 0;

    for (TPureInt texi = 0; texi < texMgr.getCount(); texi++)
    {
        PureTexture* tex = (PureTexture*) texMgr.getAttachedAt(texi);
        if ( !tex )
            continue;

        bool textureUsedByMaterial = false;
        for (TPureInt mati = 0; !textureUsedByMaterial && (mati < matMgr.getCount()); mati++)
        {
            PureMaterial* mat = (PureMaterial*) matMgr.getAttachedAt(mati);
            if ( !mat)
                continue;

            for (TPureUInt layeri = 0; !textureUsedByMaterial && (layeri < matMgr.getMaximumLayerCount()); layeri++)
            {
                if ( mat->getTexture(layeri) == tex )
                    textureUsedByMaterial = true;
            }
        } // for mati
        if ( !textureUsedByMaterial )
        {
            ++nUnusedTextures;
            getConsole().OLn("Internal num.: %d, file name: %s", tex->getInternalNum(), tex->getFilename().c_str());
        }
    } // for texi

    getConsole().OLn("%d Textures Found to be Unused by Checking %d Materials.", nUnusedTextures, matMgr.getCount());
    getConsole().OLn("");
} // PrintUnusedTextures()


void PR00FsUltimateRenderingEngineImpl::PrintMaterialsUnusedByMeshes() const
{
    if ( !isInitialized() )
        return;

    const PureMesh3DManager& meshMgr = getMesh3DManager();
    const PureMaterialManager& matMgr = getMaterialManager();
    
    getConsole().OLn("List of Materials Unused by Meshes");
    getConsole().OLn("==================================");

    TPureUInt nUnusedMaterials = 0;

    for (TPureInt mati = 0; mati < matMgr.getCount(); mati++)
    {
        PureMaterial* mat = (PureMaterial*) matMgr.getAttachedAt(mati);
        if ( !mat )
            continue;

        bool matUsedByMesh = false;
        for (TPureInt meshi = 0; !matUsedByMesh && (meshi < meshMgr.getCount()); meshi++)
        {
            PureMesh3D* mesh = (PureMesh3D*) meshMgr.getAttachedAt(meshi);
            if ( !mesh)
                continue;

            if ( &(mesh->getMaterial(false)) == mat )
                matUsedByMesh = true;
            
            for (TPureInt submeshi = 0; !matUsedByMesh && (submeshi < mesh->getCount()); submeshi++)
            {
                PureMesh3D* submesh = (PureMesh3D*) mesh->getAttachedAt(submeshi);
                if ( !submesh)
                    continue;

                if ( &(submesh->getMaterial()) == mat )
                    matUsedByMesh = true;
            }
        } // for meshi
        if ( !matUsedByMesh )
        {
            ++nUnusedMaterials;
            getConsole().OLn("Name: %s", mat->getName().c_str());
        }
    } // for mati

    getConsole().OLn("%d Materials Found to be Unused by checking %d Mesh3Ds.", nUnusedMaterials, meshMgr.getCount());
    getConsole().OLn("");
} // PrintMaterialsUnusedByMeshes()


void PR00FsUltimateRenderingEngineImpl::PrintMaterialsUnusedByObjects() const
{
    if ( !isInitialized() )
        return;

    const PureObject3DManager& objMgr = getObject3DManager();
    const PureMaterialManager& matMgr = getMaterialManager();
    
    getConsole().OLn("List of Materials Unused by Object3Ds");
    getConsole().OLn("=====================================");

    TPureUInt nUnusedMaterials = 0;

    for (TPureInt mati = 0; mati < matMgr.getCount(); mati++)
    {
        PureMaterial* mat = (PureMaterial*) matMgr.getAttachedAt(mati);
        if ( !mat )
            continue;

        bool matUsedByObject3D = false;
        for (TPureInt obji = 0; !matUsedByObject3D && (obji < objMgr.getCount()); obji++)
        {
            PureObject3D* obj = (PureObject3D*) objMgr.getAttachedAt(obji);
            if ( !obj)
                continue;

            if ( &(obj->getMaterial(false)) == mat )
                    matUsedByObject3D = true;
            
            if ( obj->getBoundingBoxObject() )
            {
                if ( &(obj->getBoundingBoxObject()->getMaterial(false)) == mat )
                    matUsedByObject3D = true;

                for (TPureInt subobji = 0; !matUsedByObject3D && (subobji < obj->getBoundingBoxObject()->getCount()); subobji++)
                {
                    PureObject3D* subobj = (PureObject3D*) obj->getBoundingBoxObject()->getAttachedAt(subobji);
                    if ( !subobj)
                        continue;

                    if ( &(subobj->getMaterial()) == mat )
                        matUsedByObject3D = true;
                }
            } // bounding box

            for (TPureInt subobji = 0; !matUsedByObject3D && (subobji < obj->getCount()); subobji++)
            {
                PureObject3D* subobj = (PureObject3D*) obj->getAttachedAt(subobji);
                if ( !subobj)
                    continue;

                if ( &(subobj->getMaterial()) == mat )
                    matUsedByObject3D = true;
            }
        } // for obji
        if ( !matUsedByObject3D )
        {
            ++nUnusedMaterials;
            getConsole().OLn("Name: %s", mat->getName().c_str());
        }
    } // for mati

    getConsole().OLn("%d Materials Found to be Unused by checking %d Object3Ds.", nUnusedMaterials, objMgr.getCount());
    getConsole().OLn("");
} // PrintMaterialsUnusedByObjects()


/**
    Invoke WriteList() of all children instances.
    This will give you a verbose list of all created/loaded managed and manager instances with information about resource usage.
*/
void PR00FsUltimateRenderingEngineImpl::WriteList() const
{
    getConsole().OLnOI("PR00FsUltimateRenderingEngine::WriteList() start");
    getConsole().OLnOI("");
    if ( isInitialized() )
    {
        // renderer is 1st so the Conclude() inside will calculate with valid time
        getRenderer()->WriteStats();
        // getScreen() doesnt have such ...
        getWindow().WriteSettings();
        getHardwareInfo().WriteStats();
        getImageManager().WriteList();
        getTextureManager().WriteList();
        PrintTexturesUnusedByMaterials();
        getMaterialManager().WriteList();
        PrintMaterialsUnusedByMeshes();
        PrintMaterialsUnusedByObjects();
        getMesh3DManager().WriteList();
        getObject3DManager().WriteList();
        // getCamera() doesnt have such ...
        // getUImanager() doesnt have such ...

        const TPureUInt nSumSysMemUsed =
            sizeof(wnd) + 
            sizeof(hwInfo) + 
            sizeof(*pCamera) + 
            sizeof(uiMgr) +
            sizeof(*pRenderer) +
            sizeof(getImageManager()) + 
            getImageManager().getUsedSystemMemory() +
            sizeof(getTextureManager()) + 
            getTextureManager().getUsedSystemMemory() +
            sizeof(getMaterialManager()) + 
            getMaterialManager().getUsedSystemMemory() +
            sizeof(getMesh3DManager()) + 
            getMesh3DManager().getUsedSystemMemory() +
            sizeof(getObject3DManager()) + 
            getObject3DManager().getUsedSystemMemory();
    
        const TPureUInt nSumVidMemUsed =
            getTextureManager().getUsedTextureMemory() +
            getObject3DManager().getUsedVideoMemory();
            /* todo: add screen vram usage i.e. framebuffer, copy this from old JointFuture engine */

        getConsole().OLn("Engine-Summarized Memory Usage:");
        getConsole().OLn("> total used system memory = %d kBytes <= %d MBytes", (int)(ceil(nSumSysMemUsed/1024.0f)), (int)(ceil(nSumSysMemUsed/1024.0f/1024.0f)));
        getConsole().OLn("> total used video  memory = %d kBytes <= %d MBytes", (int)(ceil(nSumVidMemUsed/1024.0f)), (int)(ceil(nSumVidMemUsed/1024.0f/1024.0f)));
        getConsole().OLn("");
    }
    else
    {
        getConsole().OLn("PR00FsUltimateRenderingEngine is NOT initialized!");
    }

    getConsole().OLnOO("PR00FsUltimateRenderingEngine::WriteList() end");
    getConsole().OLn("");
} // WriteList()


/**
    Checks if invoke of WriteList() would happen at the beginning of shutdown().
    Default false.

    @return True if WriteList() would be invoked automatically at the beginning of shutdown(), false otherwise.
*/
TPureBool PR00FsUltimateRenderingEngineImpl::getAutoWriteStatsAtShutdown() const
{
    return bAutoWriteStatsAtShutdown;
} // getAutoWriteStatsAtShutdown()


/**
    Sets if invoke of WriteList() should happen at the beginning of shutdown().
    Default false.

    @param state If true WriteList() will be invoked automatically at the beginning of shutdown(), otherwise it won't be invoked.
*/
void PR00FsUltimateRenderingEngineImpl::SetAutoWriteStatsAtShutdown(TPureBool state)
{
    bAutoWriteStatsAtShutdown = state;
} // SetAutoWriteStatsAtShutdown()


/**
    Copies screen content to texture.

    @param tex The target texture to where we save content of the screen. Width of the texture must be same as viewport width of the camera.
*/
void PR00FsUltimateRenderingEngineImpl::CopyScreenToTexture(PureTexture& tex)
{
    // This function should be rather part of PureScreen(), however it is easier to implement here.
    // One challenge of implementing this in PureScreen() is that that class needs to know:
    //  - HWInfo to check if multitexturing is supported, although singleton instance is easily get with getHardwareInfo();
    //  - current camera because that is not singleton and cannot be queried by that class, this is the real challenge.
    //    Camera is needed to get viewport size. It is not ok to check current resolution because:
    //    - it does not equal to expected image size when engine runs in windowed mode;
    //    - in future we may want to have multiple cameras with their own viewports, and CopyToTexture() should copy only 1 viewport content.

    if ( !isInitialized() )
        return;

    // we don't expect texture dimension to be rectangular (w != h), but
    // its width must be same as our viewport's width
    if ( tex.getWidth() != getCamera().getViewport().size.width )
        return;

    if ( tex.getHeight() == 0 )
        return;

    if ( tex.getInternalNum() == 0 )
        return;

    if ( getHardwareInfo().getVideo().isMultiTexturingSupported() )
    {
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glDisable(GL_TEXTURE_2D);
        glActiveTextureARB(GL_TEXTURE0_ARB);
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex.getInternalNum());
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, tex.getWidth(), tex.getHeight(), 0);
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    NULLs members only.
*/                                                                         
PR00FsUltimateRenderingEngineImpl::PR00FsUltimateRenderingEngineImpl(PGEcfgProfiles& cfgProfiles, PGEInputHandler& inputHandler) :
    m_cfgProfiles(cfgProfiles),
    m_inputHandler(inputHandler),
    ssets( PureSharedSettings::createAndGet() ),
    wnd( PureWindow::createAndGet(cfgProfiles, inputHandler) ),
    hwInfo( PureHwInfo::get() ),
    screen( PureScreen::createAndGet() ),
    uiMgr( PureUiManager::createAndGet() ),
    bAutoWriteStatsAtShutdown(false)
{
    pImageMgr = PGENULL;
    pTextureMgr = PGENULL;
    pCamera = PGENULL;
    pRenderer = PGENULL;
} // PR00FsUltimateRenderingEngineImpl(...)


PR00FsUltimateRenderingEngineImpl::PR00FsUltimateRenderingEngineImpl(const PR00FsUltimateRenderingEngineImpl& other) :
    m_cfgProfiles(other.m_cfgProfiles),
    m_inputHandler(other.m_inputHandler),
    ssets( PureSharedSettings::createAndGet() ),
    wnd( PureWindow::createAndGet(other.m_cfgProfiles, other.m_inputHandler) ),
    hwInfo( PureHwInfo::get() ),
    screen( PureScreen::createAndGet() ),
    uiMgr( PureUiManager::createAndGet() ),
    bAutoWriteStatsAtShutdown(false)
{
}


PR00FsUltimateRenderingEngineImpl& PR00FsUltimateRenderingEngineImpl::operator=(const PR00FsUltimateRenderingEngineImpl&)
{
    return *this;
}


/*
   PR00FsUltimateRenderingEngine
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PR00FsUltimateRenderingEngine& PR00FsUltimateRenderingEngine::createAndGet(PGEcfgProfiles& cfgProfiles, PGEInputHandler& inputHandler)
{
    static PR00FsUltimateRenderingEngineImpl pureInstance(cfgProfiles, inputHandler);
    return pureInstance;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PR00FsUltimateRenderingEngine::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PR00FsUltimateRenderingEngine::getLoggerModuleName()
{
    return "PR00FsUltimateRenderingEngine";
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################
