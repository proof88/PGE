/*
    ###################################################################################
    PR00FsReducedRenderingEngine.cpp
    This file is part of PRRE.
    PR00F's Reduced Rendering Engine
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/


#include "PRREbaseIncludes.h"  // PCH
#include "PRREpragmas.h"
#include "PR00FsReducedRenderingEngine.h"
#include "PRREBaseClass.h"
#include "PRRESharedSettings.h"
#include "PRREGLsnippets.h"
#include "PRREGLextensionFuncs.h"
#include "Render/PRRERendererHWfixedPipe.h"
#include "Render/PRRERendererSWincremental.h"

using namespace std;


/*
   PR00FsReducedRenderingEngineImpl
   ###########################################################################
*/

class PR00FsReducedRenderingEngineImpl :
    public PR00FsReducedRenderingEngine
{
public:
    virtual ~PR00FsReducedRenderingEngineImpl();       /**< Calls shutdown(). */

    TPRREuint initialize(
        TPRRE_RENDERER rndr,
        TPRREuint width, TPRREuint height,
        TPRRE_DISPLAY_MODES dmode,
        TPRREuint freq, TPRREint cdepth,
        TPRREint zdepth, TPRREint stencil,
        TPRREint samples,
        HWND window = NULL);         
    TPRREbool shutdown();            
    TPRREbool isInitialized() const;         
    
    PRREScreen&           getScreen() const;         
    PRREWindow&           getWindow() const;        
    PRREhwInfo&           getHardwareInfo() const;     
    PRREImageManager&     getImageManager() const;  
    PRRETextureManager&   getTextureManager() const;  
    PRREMaterialManager&  getMaterialManager() const;
    PRREObject3DManager&  getObject3DManager() const; 
    PRRECamera&           getCamera() const;      
    PRREuiManager&        getUImanager() const;
    PRREIRenderer*        getRenderer() const;

    void                  CopyScreenToTexture(PRRETexture& tex);
    
    void WriteStats() const;       
    void CheckConsistency() const;  

private:
    PRRESharedSettings&  ssets;        /**< Shared settings, singleton. */
    PRREWindow&          wnd;          /**< Our window, where we draw to, singleton. */
    PRREhwInfo&          hwInfo;       /**< Hardware infos, singleton. */
    PRREScreen&          screen;       /**< Our screen, singleton. */
    PRREImageManager*    pImageMgr;    /**< ImageManager, at least 1 instance. */
    PRRETextureManager*  pTextureMgr;  /**< TextureManager, at least 1 instance. */
    PRREMaterialManager* pMaterialMgr; /**< MaterialManager, at least 1 instance. */
    PRREObject3DManager* pObject3DMgr; /**< ObjectManager, at least 1 instance. */
    PRRECamera*          pCamera;      /**< Camera, at least 1 instance. */
    PRREuiManager&       uiMgr;        /**< UI manager, singleton. */
    PRREIRenderer*       pRenderer;    /**< Renderer, singleton. */

    // ---------------------------------------------------------------------------

    PR00FsReducedRenderingEngineImpl();                /**< NULLs members only. */
    PR00FsReducedRenderingEngineImpl(const PR00FsReducedRenderingEngineImpl&);
    PR00FsReducedRenderingEngineImpl& operator=(const PR00FsReducedRenderingEngineImpl&);

    friend class PR00FsReducedRenderingEngine;

};


// ############################### PUBLIC ################################


/**
    Calls shutdown().
*/
PR00FsReducedRenderingEngineImpl::~PR00FsReducedRenderingEngineImpl()
{
    shutdown();
} // ~PR00FsReducedRenderingEngine()


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
TPRREuint PR00FsReducedRenderingEngineImpl::initialize(
    TPRRE_RENDERER rndr,
    TPRREuint width, TPRREuint height,
    TPRRE_DISPLAY_MODES dmode, TPRREuint freq,
    TPRREint cdepth, TPRREint zdepth, TPRREint stencil,
    TPRREint samples,
    HWND window)
{
    TPRREint st = stencil;

    getConsole().OLn("PRRE::initialize(%d, %d, %d, dmode, %d, %d, %d, %d, %d)",
                      rndr, width, height, freq, cdepth, zdepth, st, samples);
    if ( isInitialized() )
    {
        getConsole().SOLn("  > already initialized!");
        return 1;
    }

    getConsole().OIOLn( PRRE_NAME );
    getConsole().OLn( PRRE_VERSION );

    switch (rndr)
    {
    case PRRE_RENDERER_SW:
        pRenderer = &PRRERendererSWincremental::createAndGet(wnd, screen, hwInfo);
        break;
    case PRRE_RENDERER_HW_FP:
        pRenderer = &PRRERendererHWfixedPipe::createAndGet(wnd, screen, hwInfo);
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
    pImageMgr = new PRREImageManager();

    pTextureMgr  = new PRRETextureManager();
    if ( !pTextureMgr->isInitialized() )
    {
        getConsole().EOLn("ERROR: failed to initialize PRRETextureManager!");
        shutdown();
        getConsole().OO();
        return 1;
    }

    pMaterialMgr = new PRREMaterialManager();
    if ( !pMaterialMgr->isInitialized() )
    {
        getConsole().EOLn("ERROR: failed to initialize PRREMaterialManager!");
        shutdown();
        getConsole().OO();
        return 1;
    }

    pObject3DMgr = new PRREObject3DManager(*pTextureMgr, *pMaterialMgr);
    if ( !pObject3DMgr->isInitialized() )
    {
        getConsole().EOLn("ERROR: failed to initialize PRREObject3DManager!");
        shutdown();
        getConsole().OO();
        return 1;
    }

    pCamera = new PRRECamera();
    // initialize camera viewport to window client area
    // we can trust window clientrect size even if this is an external window
    pCamera->SetViewport(0, 0, wnd.getClientWidth(), wnd.getClientHeight());
    pCamera->SetAspectRatio(wnd.getClientWidth() / (TPRREfloat) wnd.getClientHeight());

    uiMgr.Initialize( wnd.getWndDC() );

    pRenderer->SetManagers(pObject3DMgr, &uiMgr, pCamera);

    // PRRE is fully operational at this point
    getConsole().OLn("");
    getConsole().SOLn("> PRRE initialized");
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
TPRREbool PR00FsReducedRenderingEngineImpl::shutdown()
{
    getConsole().OLn("PR00FsReducedRenderingEngine::shutdown()");
    
    getConsole().OIOLn("Shutting down subsystems ...");
    getConsole().OI();

    bool success = true;
    // first the renderer must be shut down, then the managers
    if ( pRenderer )
    {
        if (!( success = pRenderer->shutdown() ))
            getConsole().EOLnOO("ERROR: failed to shut down renderer!");
        pRenderer = PGENULL;
    }   

    uiMgr.Deinitialize();

    // managed objects like objects, textures, etc are implicitly deleted by calling dtor of their managers below
    delete pCamera;
    pCamera = PGENULL;

    delete pObject3DMgr;
    pObject3DMgr = PGENULL;

    delete pMaterialMgr;
    pMaterialMgr = PGENULL;

    delete pTextureMgr;
    pTextureMgr = PGENULL;

    delete pImageMgr;
    pImageMgr = PGENULL;

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
TPRREbool PR00FsReducedRenderingEngineImpl::isInitialized() const
{
    return ( pRenderer && pRenderer->isInitialized() );
} // isInitialized()

 
/** 
    Get access to screen manipulation functions.

    @return Reference to PRREScreen object if engine is initialized, invalid reference otherwise.
*/
PRREScreen& PR00FsReducedRenderingEngineImpl::getScreen() const
{
    return screen;
} // getScreen()


/** 
    Get access to window manipulation functions.

    @return Reference to PRREWindow object if engine is initialized, invalid reference otherwise.
*/
PRREWindow& PR00FsReducedRenderingEngineImpl::getWindow() const
{
    return wnd;
} // getWindow()


/**
    Get access to hardware information.

    @return Reference to PRREhwInfo object if engine is initialized, invalid reference otherwise.
*/
PRREhwInfo& PR00FsReducedRenderingEngineImpl::getHardwareInfo() const
{
    return hwInfo;
} // getHardwareInfo()


/** 
    Get access to image handler functions.

    @return Reference to PRREImageManager object if engine is initialized, invalid reference otherwise.
*/
PRREImageManager& PR00FsReducedRenderingEngineImpl::getImageManager() const
{
    return *pImageMgr;
} // getImageManager()


/**
    Get access to texture handler functions.

    @return Reference to PRRETextureManager object if engine is initialized, invalid reference otherwise.
*/
PRRETextureManager& PR00FsReducedRenderingEngineImpl::getTextureManager() const
{
    return *pTextureMgr;
} // getTextureManager()


/**
    Get access to material handler functions.

    @return Reference to PRREMaterialManager object if engine is initialized, invalid reference otherwise.
*/
PRREMaterialManager& PR00FsReducedRenderingEngineImpl::getMaterialManager() const
{
    return *pMaterialMgr;
}


/** 
    Get access to Object3D handler functions.

    @return Reference to PRREObject3DManager object if engine is initialized, invalid reference otherwise.
*/
PRREObject3DManager& PR00FsReducedRenderingEngineImpl::getObject3DManager() const
{
    return *pObject3DMgr;
} // getObject3DManager()


/**
    Get access to camera handler functions.

    @return Reference to PRRECamera object if engine is initialized, invalid reference otherwise.
*/
PRRECamera& PR00FsReducedRenderingEngineImpl::getCamera() const
{
    return *pCamera;
} // getCamera()


/**
    Get access to user interface functions.

    @return Reference to PRREuiManager object if engine is initialized, invalid reference otherwise.
*/
PRREuiManager& PR00FsReducedRenderingEngineImpl::getUImanager() const
{
    return uiMgr;
} // getUImanager()


/**
    Get access to the selected renderer.

    @return Pointer to a renderer object implementing PRREIRenderer interface if engine is initialized, NULL otherwise.
*/
PRREIRenderer* PR00FsReducedRenderingEngineImpl::getRenderer() const
{
    return pRenderer;
}


/**
    Copies screen content to texture.

    @param tex The target texture to where we save content of the screen. Width of the texture must be same as viewport width of the camera.
*/
void PR00FsReducedRenderingEngineImpl::CopyScreenToTexture(PRRETexture& tex)
{
    // This function should be rather part of PRREScreen(), however it is easier to implement here.
    // One challenge of implementing this in PRREScreen() is that that class needs to know:
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
PR00FsReducedRenderingEngineImpl::PR00FsReducedRenderingEngineImpl() :
    ssets( PRRESharedSettings::createAndGet() ),
    wnd( PRREWindow::createAndGet() ),
    hwInfo( PRREhwInfo::get() ),
    screen( PRREScreen::createAndGet() ),
    uiMgr( PRREuiManager::createAndGet() )
{
    pImageMgr = PGENULL;
    pTextureMgr = PGENULL;
    pCamera = PGENULL;
    pRenderer = PGENULL;
} // PR00FsReducedRenderingEngineImpl(...)


PR00FsReducedRenderingEngineImpl::PR00FsReducedRenderingEngineImpl(const PR00FsReducedRenderingEngineImpl&) :
    ssets( PRRESharedSettings::createAndGet() ),
    wnd( PRREWindow::createAndGet() ),
    hwInfo( PRREhwInfo::get() ),
    screen( PRREScreen::createAndGet() ),
    uiMgr( PRREuiManager::createAndGet() )
{
}


PR00FsReducedRenderingEngineImpl& PR00FsReducedRenderingEngineImpl::operator=(const PR00FsReducedRenderingEngineImpl&)
{
    return *this;
}


/*
   PR00FsReducedRenderingEngine
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PR00FsReducedRenderingEngine& PR00FsReducedRenderingEngine::createAndGet()
{
    static PR00FsReducedRenderingEngineImpl PRREinstance;
    return PRREinstance;
} // createAndGet()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################
