/*
    ###################################################################################
    PureRendererHWfixedPipe.cpp
    This file is part of Pure.
    Fixed-Pipeline HW Renderer class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PurebaseIncludes.h"  // PCH

#include "../../include/internal/GUI/imgui-1.88/imgui.h"
#include "../../include/internal/GUI/imgui-1.88/backends/imgui_impl_opengl2.h"
#include "../../include/internal/GUI/imgui-1.88/backends/imgui_impl_win32.h"

#include "../../include/external/Render/PureRendererHWfixedPipe.h"
#include <cassert>
#include "../../include/external/Math/PureVector.h"
#include "../../include/external/Math/PureTransformMatrix.h"
#include "../../include/external/Object3D/PureObject3DManager.h"
#include "../../include/internal/PureGLsafeFuncs.h"
#include "../../include/internal/PureGLsnippets.h"
#include "../../include/internal/PureGLextensionFuncs.h"
#include "../../include/internal/Purepragmas.h"

using namespace std;

/*
   PureRendererHWfixedPipeImpl
   ###########################################################################
*/

// TODO: this should also implement PureIIncrementalRenderPipeline
class PureRendererHWfixedPipeImpl :
    public PureRendererHWfixedPipe
{
public:
    /**
        Renderer statistics collected until next ResetStatistics().
    */
    struct CurrentStats
    {
        TPURE_RENDER_HINT renderHints;  /**< Render hints. */
        TPureUInt         nRenderedFrames;
        TPureFloat        fAvgFrameTime;
        TPureFloat        fAvgFps;
        TPureFloat        fDurationSecs;
        PFL::timeval      timeMinFrameTime;
        PFL::timeval      timeMaxFrameTime;
        CurrentStats();
        void ResetStatistics();
        void Conclude(const PFL::timeval& timeDurationStart);
    };

    /**
        Last frame statistics collected until end of frame, then reset at the beginning of next frame.
    */
    class LastFrameStats
    {
    public:
        LastFrameStats();
        void Reset();
        void Update(const PureObject3D& obj, const TPURE_RENDER_PASS& iRenderPass);
        TPureUInt getObjectsVisible() const;
        TPureUInt getObjectsOccluders() const;
        TPureUInt getOccludeesNonOcclusionTested() const;
        TPureUInt getOccludeesOcclusionTested() const;
        TPureUInt getOccludeesOcclusionTestedAndOccluded() const;
        TPureUInt getOccludeesOcclusionTestedAndNonOccluded() const;
        TPureUInt getOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway() const;
        TPureUInt getTransferredVertices() const;
        TPureUInt getTransferredTriangles() const;
    private:
        TPureUInt nLastFrameObjectsVisible;
        TPureUInt nLastFrameObjectsOccluders;
        TPureUInt nLastFrameObjectsNonOcclusionTested;
        TPureUInt nLastFrameObjectsOcclusionTested;
        TPureUInt nLastFrameObjectsOccluded;
        TPureUInt nLastFrameObjectsNonOccluded;
        TPureUInt nLastFrameObjectsNonOccludedButNotVisible;
        TPureUInt nLastFrameVertices;
        TPureUInt nLastFrameTriangles;
    };

    static TPureBool OQ_AUTO_UPDATE_OCCLUDER_STATES;
    static TPureBool OQ_ZPASS_FOR_OCCLUDERS;

    static std::map<TPureUInt, std::string> mapRenderPaths2String;
    static std::map<TPureUInt, std::string> mapOcclusionQueryMethod2String;
    static std::vector<CurrentStats> stats;
    static LastFrameStats lastFrameStats;

    // ---------------------------------------------------------------------------

    virtual ~PureRendererHWfixedPipeImpl();       /**< Calls shutdown(). */

    TPureUInt initialize(
        TPureUInt width, TPureUInt height,
        TPURE_DISPLAY_MODES dmode,
        TPureUInt freq, TPureInt cdepth,
        TPureInt zdepth, TPureInt stencil,
        TPureInt samples,
        HWND window = NULL);         
    TPureBool shutdown();            
    TPureBool isInitialized() const;    

    void SetManagers(
        PureObject3DManager* _objmgr,
        PureUiManager* _uimgr,
        PureCamera* _cam );

    void RenderScene();  

    void ShowGuiDemo(TPureBool state);

    const TPURE_RENDER_HINT& getRenderHints();
    void  SetRenderHints(const TPURE_RENDER_HINT& hints);

    void ResetStatistics();
    
    void WriteStats() const;       
    void CheckConsistency() const;  

    TPureUInt getLastFrameObjectsVisible() const;
    TPureUInt getLastFrameOccluders() const;
    TPureUInt getLastFrameOccludeesNonOcclusionTested() const;
    TPureUInt getLastFrameOccludeesOcclusionTested() const;
    TPureUInt getLastFrameOccludeesOcclusionTestedAndOccluded() const;
    TPureUInt getLastFrameOccludeesOcclusionTestedAndNonOccluded() const;
    TPureUInt getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway() const;
    TPureUInt getLastFrameTransferredVertices() const;
    TPureUInt getLastFrameTransferredTriangles() const;

private:
    PureWindow&          wnd;          /**< Our window, where we draw to, singleton. */
    PureHwInfo&          hwInfo;       /**< Hardware infos, singleton. */
    PureScreen&          screen;       /**< Our screen, singleton. */
    PureObject3DManager* pObject3DMgr; /**< ObjectManager, at least 1 instance. */
    PureCamera*          pCamera;      /**< Camera, at least 1 instance. */
    PureUiManager*       pUImgr;       /**< UI manager, singleton. */

    HGLRC                rc;           /**< Rendering context, initially NULL. Determines whether the renderer is initialized or not. */
    GLfloat              mat4x4Identity[4][4];
    PFL::timeval         timeDurationStart;

    TPureBool            bShowGuiDemo;

    // ---------------------------------------------------------------------------

    PureRendererHWfixedPipeImpl();                /**< NULLs members only. */
    PureRendererHWfixedPipeImpl(
        PureWindow& _wnd,
        PureScreen& _scr,
        PureHwInfo& _hwinfo );
    PureRendererHWfixedPipeImpl(const PureRendererHWfixedPipeImpl&);
    PureRendererHWfixedPipeImpl& operator=(const PureRendererHWfixedPipeImpl&);

    void printOGLerrorBrief();           /**< Writes OpenGL error to console only if there is really an error. */
    void printOGLerrorFull();            /**< Writes OpenGL error to console even the error is no error. */
    void LogFullRenderHints(const TPURE_RENDER_HINT& hints) const;     /**< Logs given render hints with full descriptive text. */
    void LogLastFrameStats() const;                                    /**< Logs last frame stats. */
    void LogCollectedStats(TPureUInt i) const;
    void LogLastCollectedStats() const;

    TPureBool initializeOpenGL(HDC dc);  /**< Initializes OpenGL. */
    TPureBool shutdownOpenGL();          /**< Shuts down OpenGL. */
    void      SetBasicThingsInOpenGL();  /**< Sets some basic things in OpenGL. */

    void RenderObject(PureObject3D& object);

    void BeginRendering();                                               /**< Sets viewport size and clears buffers. */
    void SwitchToPerspectiveProjection();                                /**< Sets perspective projection. */
    void SwitchToOrtographicProjection();                                /**< Sets orthographic projection. */
    void OrderObjectContainersByZdistance();                             /**< Orders objects by Z-distance relative to camera view. */
    void Draw3DObjects_Legacy(PureIRenderer& renderer);                  /**< Draws 3D objects, the legacy PR00FPS way. */
    void Draw3DObjects_Ordered_Containers(PureIRenderer& renderer);      /**< Draws 3D objects using separate Object3D containers. */
    void Draw3DObjects_OcclusionQuery(
        PureIRenderer& renderer,
        TPureBool bASyncQuery);                                          /**< Draws 3D objects with occlusion query. */
    void Draw2DObjects(PureIRenderer& renderer);                         /**< Draws 2D objects. */
    void FinishRendering();                /**< Forces pending tasks to be finished and displays the rendered picture. */

    friend class PureRendererHWfixedPipe;

}; // PureRendererHWfixedPipeImpl


// ############################### PUBLIC ################################

TPureBool PureRendererHWfixedPipeImpl::OQ_AUTO_UPDATE_OCCLUDER_STATES = true;
TPureBool PureRendererHWfixedPipeImpl::OQ_ZPASS_FOR_OCCLUDERS = false;

std::map<TPureUInt, std::string> PureRendererHWfixedPipeImpl::mapRenderPaths2String;
std::map<TPureUInt, std::string> PureRendererHWfixedPipeImpl::mapOcclusionQueryMethod2String;
std::vector<PureRendererHWfixedPipeImpl::CurrentStats> PureRendererHWfixedPipeImpl::stats;
PureRendererHWfixedPipeImpl::LastFrameStats PureRendererHWfixedPipeImpl::lastFrameStats;

PureRendererHWfixedPipeImpl::CurrentStats::CurrentStats()
{
    ResetStatistics();
} // CurrentStats()

void PureRendererHWfixedPipeImpl::CurrentStats::ResetStatistics()
{
    renderHints = 0;   
    nRenderedFrames = 0;
    fAvgFrameTime = 0.f;
    fAvgFps = 0.f;
    fDurationSecs = 0.f;
    timeMinFrameTime.tv_sec  = LONG_MAX;
    timeMinFrameTime.tv_usec = LONG_MAX;
    timeMaxFrameTime.tv_sec  = LONG_MIN;
    timeMaxFrameTime.tv_usec = LONG_MIN;
}  // ResetStatistics()

void PureRendererHWfixedPipeImpl::CurrentStats::Conclude(const PFL::timeval& timeDurationStart)
{
    PFL::timeval timeReset;
    PFL::gettimeofday(&timeReset, 0);
    
    fDurationSecs = PFL::getTimeDiffInUs(timeReset, timeDurationStart) / 1000000.f;
    fAvgFrameTime = fDurationSecs / nRenderedFrames;
    fAvgFps = 1 / fAvgFrameTime;
} // Conclude()


PureRendererHWfixedPipeImpl::LastFrameStats::LastFrameStats()
{
    Reset();
} // LastFrameStats()

void PureRendererHWfixedPipeImpl::LastFrameStats::Reset()
{
    nLastFrameObjectsVisible = 0;
    nLastFrameObjectsOccluders = 0;
    nLastFrameObjectsNonOcclusionTested = 0;
    nLastFrameObjectsOcclusionTested = 0;
    nLastFrameObjectsOccluded = 0;
    nLastFrameObjectsNonOccluded = 0;
    nLastFrameObjectsNonOccludedButNotVisible = 0;
    nLastFrameVertices = 0;
    nLastFrameTriangles = 0;
} // Reset()

/**
    Updates the per-frame stats with data from the given object.
    Only PURE_RPASS_NORMAL renderpass changes the following counters:
     - nLastFrameObjectsVisible;
     - nLastFrameObjectsOcclusionTested;
     - nLastFrameObjectsNonOccluded;
     - nLastFrameObjectsOccluded.
    
    Vertex and triangle counters are updated by every renderpass by and kind of object.

    Occluders do not update nLastFrameObjectsOcclusionTested even if occlusion testing is enabled for them because the renderer
    just doesn't test occlusion for occluders. Thus they dont update nLastFrameObjectsNonOccluded and nLastFrameObjectsOccluded either.
*/
void PureRendererHWfixedPipeImpl::LastFrameStats::Update(const PureObject3D& obj, const TPURE_RENDER_PASS& iRenderPass)
{
    switch (iRenderPass)
    {
        case PURE_RPASS_NORMAL:
        {
            if ( obj.isRenderingAllowed() )
            {
                nLastFrameObjectsVisible++;
            }
            
            if ( obj.isOccluder() )
            {
                nLastFrameObjectsOccluders++;
            }
            else
            {
                if ( obj.isOcclusionTested() )
                {
                    nLastFrameObjectsOcclusionTested++;
                    if ( obj.isOccluded() )
                    {
                        nLastFrameObjectsOccluded++;
                    }
                    else
                    {
                        nLastFrameObjectsNonOccluded++;
                        if ( !obj.isRenderingAllowed() )
                        {
                            nLastFrameObjectsNonOccludedButNotVisible++;
                        }
                    }
                }
                else
                {
                    nLastFrameObjectsNonOcclusionTested++;
                }
            }
        } // case PURE_RPASS_NORMAL
        // fall-through
        case PURE_RPASS_START_OCCLUSION_QUERY:
        case PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY:
        case PURE_RPASS_Z_ONLY:
        default:
            nLastFrameVertices += obj.getLastTransferredVertexCount();
            nLastFrameTriangles += obj.getLastTransferredTriangleCount(); 
    } // switch
} // Update()

TPureUInt PureRendererHWfixedPipeImpl::LastFrameStats::getObjectsVisible() const
{
    return nLastFrameObjectsVisible;
} // getObjectsVisible()

TPureUInt PureRendererHWfixedPipeImpl::LastFrameStats::getObjectsOccluders() const
{
    return nLastFrameObjectsOccluders;
} // getObjectsOccluders()

TPureUInt PureRendererHWfixedPipeImpl::LastFrameStats::getOccludeesNonOcclusionTested() const
{
    return nLastFrameObjectsNonOcclusionTested;
} // getOccludeesNonOcclusionTested()

TPureUInt PureRendererHWfixedPipeImpl::LastFrameStats::getOccludeesOcclusionTested() const
{
    return nLastFrameObjectsOcclusionTested;
} // getOccludeesOcclusionTested()

TPureUInt PureRendererHWfixedPipeImpl::LastFrameStats::getOccludeesOcclusionTestedAndOccluded() const
{
    return nLastFrameObjectsOccluded;
} // getOccludeesOcclusionTestedAndOccluded()

TPureUInt PureRendererHWfixedPipeImpl::LastFrameStats::getOccludeesOcclusionTestedAndNonOccluded() const
{
    return nLastFrameObjectsNonOccluded;
} // getOccludeesOcclusionTestedAndNonOccluded()

TPureUInt PureRendererHWfixedPipeImpl::LastFrameStats::getOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway() const
{
    return nLastFrameObjectsNonOccludedButNotVisible;
} // getOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway()

TPureUInt PureRendererHWfixedPipeImpl::LastFrameStats::getTransferredVertices() const
{
    return nLastFrameVertices;
}

TPureUInt PureRendererHWfixedPipeImpl::LastFrameStats::getTransferredTriangles() const
{
    return nLastFrameTriangles;
}


/**
    Calls shutdown().
*/
PureRendererHWfixedPipeImpl::~PureRendererHWfixedPipeImpl()
{
    shutdown();
} // ~PureRendererHWfixedPipeImpl()


TPureUInt PureRendererHWfixedPipeImpl::initialize(
    TPureUInt width, TPureUInt height,
    TPURE_DISPLAY_MODES dmode, TPureUInt freq,
    TPureInt cdepth, TPureInt zdepth, TPureInt stencil,
    TPureInt samples,
    HWND window)
{
    TPureInt st = stencil;

    getConsole().OLn("PureRendererHWfixedPipe::initialize(%d, %d, dmode, %d, %d, %d, %d, %d)",
                      width, height, freq, cdepth, zdepth, st, samples);
    if ( isInitialized() )
    {
        getConsole().SOLn("  > already initialized!");
        return 1;
    }

    getConsole().OI();

    // we accept only 0 or 8 for stencil bits
    if ( st != 0 )
    {
        st = 8;
        getConsole().OLn("WARNING: forcing non-0 stencil to %d !", st);
    }

    TPureBool bRequestAdvancedPixelFormat = false;  // will be set to true in the loop AFTER 1st GL init if antialiasing is requested AND if advanced formats available
    TPureBool bLoopNeeded = true;  // will be set to false in the loop AFTER 1st GL init if antialiasing is NOT requested OR advanced formats are NOT available

    /*
        Initialize OpenGL once or twice.
        2nd init is needed if advanced pixel format is needed, eg. antialiasing is requested. That's the reason of the loop here.
        This comes from the nature of OpenGL: it must be initialized to query available advanced pixel formats, then it may be reinitialized again with an
        advanced pixel format, thus a renderer must handle screen, window and hwinfo instances init during its init.
    */
    while ( bLoopNeeded )
    {
        getConsole().OLn("");
        getConsole().OLn("#### STAGE 1/3 ####");

            getConsole().OLn("screen settings ...");
            screen.SetResolution(width, height);
            screen.SetFullScreened( dmode != PURE_WINDOWED );  // == PURE_FULLSCREEN nem jó, mert lesz 3. FS mód is ...
            screen.SetFreq( freq );
            screen.SetColorBits( cdepth );
            screen.SetDepthBits( zdepth );
            screen.SetStencilBits( st );
            screen.SetFSAAlevel( samples );
      
            getConsole().OLn("new PureWindow...");
            /* 
                A window may be created only once, even if advanced pixel format is requested, since a DC is required to be created only once.
		        This would mean that window instance can be created before renderer initialization. But based on SetPixelFormat() documentation, a
		        pixel format shouldn't be set to a window multiple times. This means that we actually have to recreate a window if an advanced pixel
		        format is to be set!
            */
            if ( !wnd.initialize(screen.getResWidth(), screen.getResHeight(), dmode != PURE_WINDOWED, "Window", window) )
            {
                getConsole().EOLnOO("ERROR: wnd.initialize() failed!");
                shutdown();
                return 1;
            }
            getConsole().SOLn("done!");

            if ( !screen.applyDisplaySettings(wnd.getWndDC(), bRequestAdvancedPixelFormat ? PURE_SCREEN_PF_ADVANCED : PURE_SCREEN_PF_SIMPLE) )
            {
                getConsole().EOLnOO("ERROR: applyDisplaySettings() failed!");
                shutdown();
                return 1;
            }

        getConsole().OLn("");
        getConsole().OLn("#### STAGE 2/3 ####");
            if ( !initializeOpenGL(wnd.getWndDC()) )
            {
                getConsole().OO();
                shutdown();
                return 1;
            }

            hwInfo.Initialize(
                rc, wnd.getWndDC(),
                screen.getResWidth(), screen.getResHeight(),
                screen.getColorBits(), screen.getDepthBits(),
                screen.getStencilBits(), screen.getFSAAlevel() );

            if ( hwInfo.getVideo().isAdvancedPixelFormatSetSupported() )
            {
                if ( (samples > 0) && !bRequestAdvancedPixelFormat )
                {
                    getConsole().OLn("Advanced pixel format requested, trying ...");
                    // following call can be removed after screen.applyDisplaySettings() is modified to automatically
                    // query advanced pixel formats at first call ... see PGE-History.txt's relevant RFR task!
                    screen.applyDisplaySettings( wnd.getWndDC() );
                    bRequestAdvancedPixelFormat = true;
                    shutdown();
                }
                else
                {
                    bLoopNeeded = false;
                    getConsole().OLn("Could set advanced pixel format but not requested or already set.");
                }
            }
            else
            {
                getConsole().OLn("Advanced pixel formats not available.");
                bLoopNeeded = false;
            }
    } // while bLoopNeeded

    getConsole().OLn("");
    getConsole().OLn("#### STAGE 3/3 ####");
    getConsole().OLn("");

        printOGLerrorFull();
        SetBasicThingsInOpenGL();
        printOGLerrorFull();
        PureGLsnippets::Init();

        timeDurationStart.tv_sec = 0;
        timeDurationStart.tv_usec = 0;

        // GFX card drivers' default setting in 2015: off (if undefined by application), so we also set it to false initially
        screen.SetVSyncEnabled(false);

        IMGUI_CHECKVERSION();
        if (!ImGui::CreateContext())
        {
            getConsole().EOLnOO("ERROR: ImGui::CreateContext() failed!");
            shutdown();
            return 1;
        }

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = nullptr;  // by default it is imgui.ini in current work dir, but I dont want this auto-config behavior now ...
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        if (!ImGui_ImplWin32_Init(wnd.getWndHandle()))
        {
            getConsole().EOLnOO("ERROR: ImGui::ImGui_ImplWin32_Init() failed!");
            shutdown();
            return 1;
        }

        if (!ImGui_ImplOpenGL2_Init())
        {
            getConsole().EOLnOO("ERROR: ImGui::ImGui_ImplOpenGL2_Init() failed!");
            shutdown();
            return 1;
        }

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != NULL);

    getConsole().OLn("");
    getConsole().SOLn("> Fixed Pipeline HW Renderer initialized");
    getConsole().OLnOO("");

    return 0;

} // initialize()


TPureBool PureRendererHWfixedPipeImpl::shutdown()
{
    getConsole().OLn("PureRendererHWfixedPipe::shutdown()");

    if (!isInitialized())
    {
        getConsole().SOLn("  > already shut down!");
        return true;
    }
    
    getConsole().OIOLn("Shutting down renderer ...");
    getConsole().OI();

    if (ImGui_ImplOpenGL2_Initialized())
    {
        ImGui_ImplOpenGL2_Shutdown();
    }
    if (ImGui_ImplWin32_Initialized())
    {
        ImGui_ImplWin32_Shutdown();
    }
    if (ImGui::GetCurrentContext())
    {
        ImGui::DestroyContext();
    }

    if ( !shutdownOpenGL() )
    {
        getConsole().EOLn("WARNING: shutdownOpenGL() failed!");
    }
    lastFrameStats.Reset();

    // only window, scren, hwinfo needs to be deinitialized,
    // other managers set by SetManagers() are taken care by who specified it at the beginning.
    wnd.Deinitialize();
    screen.ResetDisplaySettings();
    hwInfo.Deinitialize();

    getConsole().OO();

    getConsole().OLnOO("Renderer is shut down!");

    return true;
} // shutdown()


TPureBool PureRendererHWfixedPipeImpl::isInitialized() const
{
    return ( rc != NULL );
} // isInitialized()


void PureRendererHWfixedPipeImpl::SetManagers(
    PureObject3DManager* _objmgr,
    PureUiManager* _uimgr,
    PureCamera* _cam )
{
    pObject3DMgr = _objmgr;
    pUImgr = _uimgr;
    pCamera = _cam;
}


void PureRendererHWfixedPipeImpl::RenderObject(PureObject3D& object)
{
    // TODO
}


void PureRendererHWfixedPipeImpl::RenderScene()
{
    if ( !isInitialized() )
    {
        return;
    }

    PFL::timeval timeFrameStart, timeFrameEnd;
    PFL::gettimeofday(&timeFrameStart, 0);

    if ( (timeDurationStart.tv_sec == 0) && (timeDurationStart.tv_usec == 0) )
    { // mark beginning of new stats duration
        PFL::gettimeofday(&timeDurationStart, 0);
    }

    lastFrameStats.Reset();

    BeginRendering();
                                                                                                
    SwitchToPerspectiveProjection();
    const TPureUInt iRenderPath = BITF_READ(stats[stats.size()-1].renderHints, PURE_RH_RENDER_PATH_BITS, 3);
    switch (iRenderPath)
    {
    case PURE_RH_RP_LEGACY_PR00FPS:
        Draw3DObjects_Legacy(*this);
        break;
    case PURE_RH_RP_DISTANCE_ORDERED:
        Draw3DObjects_Ordered_Containers(*this);
        break;
    case PURE_RH_RP_OCCLUSION_CULLED:
        {
            const TPureUInt iOcclusionQueryMethod = BITF_READ(stats[stats.size()-1].renderHints, PURE_RH_OQ_METHOD_BITS, 2);
            switch (iOcclusionQueryMethod)
            {
            case PURE_RH_OQ_METHOD_SYNC:
                Draw3DObjects_OcclusionQuery(*this, false);
                break;
            case PURE_RH_OQ_METHOD_ASYNC:
                Draw3DObjects_OcclusionQuery(*this, true);
                break;
            default:
                // later more OQ methods will come ...
                getConsole().EOLn("%s() ERROR: unhandled iOcclusionQueryMethod!", __FUNCTION__);
                assert(false);
            } // switch iOcclusionQueryMethod
        }
        break;
    default:
        // later more renderpaths will come ...
        getConsole().EOLn("%s() ERROR: unhandled iRenderPath!", __FUNCTION__);
        assert(false);
    } // switch iRenderPath
    
    SwitchToOrtographicProjection();
    Draw2DObjects(*this);

    FinishRendering();

    // we don't really check for GL errors in above functions, so check them here
    printOGLerrorBrief();

    stats[stats.size()-1].nRenderedFrames++;
    PFL::gettimeofday(&timeFrameEnd, 0);
    PFL::updateForMinDuration(stats[stats.size()-1].timeMinFrameTime, timeFrameStart, timeFrameEnd); 
    PFL::updateForMaxDuration(stats[stats.size()-1].timeMaxFrameTime, timeFrameStart, timeFrameEnd);

} // RenderScene()

void PureRendererHWfixedPipeImpl::ShowGuiDemo(TPureBool state)
{
    bShowGuiDemo = state;
}


const TPURE_RENDER_HINT& PureRendererHWfixedPipeImpl::getRenderHints()
{
    return stats[stats.size()-1].renderHints;
} // getRenderHints()


void PureRendererHWfixedPipeImpl::SetRenderHints(const TPURE_RENDER_HINT& hints)
{
    const TPureUInt prevRenderPath = BITF_READ(stats[stats.size()-1].renderHints, PURE_RH_RENDER_PATH_BITS, 3);
    const TPureUInt newRenderPath = BITF_READ(hints, PURE_RH_RENDER_PATH_BITS, 3);

    const TPureUInt prevOcclusionQueryMethod = BITF_READ(stats[stats.size()-1].renderHints, PURE_RH_OQ_METHOD_BITS, 2);
    const TPureUInt newOcclusionQueryMethod = BITF_READ(hints, PURE_RH_OQ_METHOD_BITS, 2);

    const TPureBool bPrevOcclusionDrawBoundingBoxes = ( BIT_READ(stats[stats.size()-1].renderHints, PURE_RH_OQ_DRAW_BOUNDING_BOXES_BIT) == 1u );
    const TPureBool bNewOcclusionDrawBoundingBoxes = ( BIT_READ(hints, PURE_RH_OQ_DRAW_BOUNDING_BOXES_BIT) == 1u );

    const TPureBool bPrevOcclusionDrawIfQueryPending = ( BIT_READ(stats[stats.size()-1].renderHints, PURE_RH_OQ_DRAW_IF_QUERY_PENDING_BIT) == 1u );
    const TPureBool bNewOcclusionDrawIfQueryPending = ( BIT_READ(hints, PURE_RH_OQ_DRAW_IF_QUERY_PENDING_BIT) == 1u );

    const TPureBool bPrevOrderByDistance = ( BIT_READ(stats[stats.size()-1].renderHints, PURE_RH_ORDERING_BY_DISTANCE_BIT) == 1u );
    const TPureBool bNewOrderByDistance = ( BIT_READ(hints, PURE_RH_ORDERING_BY_DISTANCE_BIT) == 1u );

    getConsole().OLn("PureRendererHWfixedPipe::SetRenderHints(%u)", hints);
    getConsole().OLn("  prev renderHints: %u", stats[stats.size()-1].renderHints);
    if ( prevRenderPath != newRenderPath )
    {
        getConsole().OLn("  prevRenderPath: %u - %s", prevRenderPath, mapRenderPaths2String[prevRenderPath].c_str());
        getConsole().OLn("  newRenderPath : %u - %s", newRenderPath, mapRenderPaths2String[newRenderPath].c_str());
    }

    if ( prevOcclusionQueryMethod != newOcclusionQueryMethod )
    {
        getConsole().OLn("  prevOcclusionQueryMethod: %u - %s", prevOcclusionQueryMethod, mapOcclusionQueryMethod2String[prevOcclusionQueryMethod].c_str());
        getConsole().OLn("  newOcclusionQueryMethod : %u - %s", newOcclusionQueryMethod, mapOcclusionQueryMethod2String[newOcclusionQueryMethod].c_str());
    }

    if ( bPrevOcclusionDrawBoundingBoxes != bNewOcclusionDrawBoundingBoxes )
    {
        getConsole().OLn("  bPrevOcclusionDrawBoundingBoxes: %b", bPrevOcclusionDrawBoundingBoxes);
        getConsole().OLn("  bNewOcclusionDrawBoundingBoxes : %b", bNewOcclusionDrawBoundingBoxes);
    }

    if ( bPrevOcclusionDrawIfQueryPending != bNewOcclusionDrawIfQueryPending )
    {
        getConsole().OLn("  bPrevOcclusionDrawIfQueryPending: %b", bPrevOcclusionDrawIfQueryPending);
        getConsole().OLn("  bNewOcclusionDrawIfQueryPending : %b", bNewOcclusionDrawIfQueryPending);
    }

    if ( bPrevOrderByDistance != bNewOrderByDistance )
    {
        getConsole().OLn("  bPrevOrderByDistance: %b", bPrevOrderByDistance);
        getConsole().OLn("  bNewOrderByDistance : %b", bNewOrderByDistance);
    }

    if ( ( (prevRenderPath == PURE_RH_RP_OCCLUSION_CULLED) && (newRenderPath != PURE_RH_RP_OCCLUSION_CULLED) )
        ||
        ( (prevOcclusionQueryMethod == PURE_RH_OQ_METHOD_ASYNC) && (newOcclusionQueryMethod != PURE_RH_OQ_METHOD_ASYNC) ) )
    {
        // we are switching either render path or occlusion query method when there still might be pending queries,
        // so let's wait them to finish, otherwise some assert() may fail and/or some states might be inconsistent at next rendering!
        getConsole().OLn("");
        getConsole().OLn("  Force-finishing ongoing occlusion queries");
        for (int i = 0; i < pObject3DMgr->getSize(); i++)
        {
            PureObject3D* const obj = (PureObject3D*) pObject3DMgr->getAttachedAt(i);
        
            if ( obj == PGENULL )
                continue;

            obj->ForceFinishOcclusionTest();
        }
    }
    getConsole().OLn("");

    stats[stats.size()-1].renderHints = hints;
} // SetRenderHints()


void PureRendererHWfixedPipeImpl::ResetStatistics()
{
    stats[stats.size()-1].Conclude(timeDurationStart);
    // RenderScene will mark beginning of new stats duration
    timeDurationStart.tv_sec = 0;
    timeDurationStart.tv_usec = 0;

    getConsole().OLnOI("PureRendererHWfixedPipe::ResetStatistics()");

    LogLastFrameStats();

    getConsole().OLn("Following render hints are saved and stats are reset:");
    getConsole().OI();
    LogLastCollectedStats();
    getConsole().OO();

    stats.push_back( CurrentStats() );
    // current renderHints will be the latest
    stats[stats.size()-1].renderHints = stats[stats.size()-2].renderHints;

    getConsole().OI();
    pObject3DMgr->ResetStatistics(); 
    getConsole().OO();

    getConsole().OO();
} // ResetStatistics()


void PureRendererHWfixedPipeImpl::WriteStats() const
{
    getConsole().OLn("PureRendererHWfixedPipe::WriteStats()");
    getConsole().L();

    getConsole().OI();

    LogFullRenderHints(stats[stats.size()-1].renderHints);
    LogLastFrameStats();

    stats[stats.size()-1].Conclude(timeDurationStart);
    getConsole().OLn("Collected Statistics:");
    for (TPureUInt i = 0; i < stats.size(); i++)
    {
        getConsole().OLn("Stats %u:", i+1);
        getConsole().OI();
        LogFullRenderHints(stats[i].renderHints);
        LogCollectedStats(i);
        getConsole().OO();
    }

    getConsole().OO();
} // WriteStats()


/**
    Runs an internal consistency test, just for debug.
*/
void PureRendererHWfixedPipeImpl::CheckConsistency() const
{
    getConsole().OLn("PureRendererHWfixedPipe::CheckConsistency()");

    getConsole().OLn("END of consistency check!");
} // CheckConsistency()


/**
    Returns number of objects allowed to be visible in last frame.
*/
TPureUInt PureRendererHWfixedPipeImpl::getLastFrameObjectsVisible() const
{
    return lastFrameStats.getObjectsVisible();
}

/**
    Returns number of objects that were occluders in last frame.
*/
TPureUInt PureRendererHWfixedPipeImpl::getLastFrameOccluders() const
{
    return lastFrameStats.getObjectsOccluders();
}

/**
    Returns number of occludees that had occlusion test disabled in last frame.
*/
TPureUInt PureRendererHWfixedPipeImpl::getLastFrameOccludeesNonOcclusionTested() const
{
    return lastFrameStats.getOccludeesNonOcclusionTested();
}

/**
    Returns number of occludees that had occlusion test enabled (not necessarily ongoing) in last frame.
*/
TPureUInt PureRendererHWfixedPipeImpl::getLastFrameOccludeesOcclusionTested() const
{
    return lastFrameStats.getOccludeesOcclusionTested();
}

/**
    Returns number of occluded occludees in last frame (only counted if occlusion test was also enabled).
*/
TPureUInt PureRendererHWfixedPipeImpl::getLastFrameOccludeesOcclusionTestedAndOccluded() const
{
    return lastFrameStats.getOccludeesOcclusionTestedAndOccluded();
}

/**
    Returns number of non-occluded occludees in last frame (only counted if occlusion test was also enabled).
*/
TPureUInt PureRendererHWfixedPipeImpl::getLastFrameOccludeesOcclusionTestedAndNonOccluded() const
{
    return lastFrameStats.getOccludeesOcclusionTestedAndNonOccluded();
}

/**
    Returns number of non-occluded occludees in last frame (only counted if occlusion test was also enabled but visibility was not).
*/
TPureUInt PureRendererHWfixedPipeImpl::getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway() const
{
    return lastFrameStats.getOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway();
}


TPureUInt PureRendererHWfixedPipeImpl::getLastFrameTransferredVertices() const
{
    return lastFrameStats.getTransferredVertices();
}

TPureUInt PureRendererHWfixedPipeImpl::getLastFrameTransferredTriangles() const
{
    return lastFrameStats.getTransferredTriangles();
}


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    NULLs members only.
*/                                                                         
PureRendererHWfixedPipeImpl::PureRendererHWfixedPipeImpl() :
    wnd( wnd ),
    hwInfo( hwInfo ),
    screen( screen ),
    pObject3DMgr( NULL ),
    pCamera( NULL ),
    pUImgr( NULL ),
    bShowGuiDemo( false )
{
    rc = NULL;
    stats.push_back( PureRendererHWfixedPipeImpl::CurrentStats() );
    stats[0].renderHints = DefaultHints;
    timeDurationStart.tv_sec = 0;
    timeDurationStart.tv_usec = 0;

    // CPP11 initializer list!!!
    mapRenderPaths2String[PURE_RH_RP_LEGACY_PR00FPS]   = std::string("Legacy PR00FPS");
    mapRenderPaths2String[PURE_RH_RP_DISTANCE_ORDERED] = std::string("Distance Ordered");
    mapRenderPaths2String[PURE_RH_RP_OCCLUSION_CULLED] = std::string("Occlusion Culled");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_1]          = std::string("Reserved");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_2]          = std::string("Reserved");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_3]          = std::string("Reserved");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_4]          = std::string("Reserved");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_5]          = std::string("Reserved");

    mapOcclusionQueryMethod2String[PURE_RH_OQ_METHOD_SYNC] = std::string("Sync");
    mapOcclusionQueryMethod2String[PURE_RH_OQ_METHOD_ASYNC] = std::string("Async");
    mapOcclusionQueryMethod2String[PURE_RH_OQ_METHOD_RSRVD_1] = std::string("Reserved");
    mapOcclusionQueryMethod2String[PURE_RH_OQ_METHOD_RSRVD_2] = std::string("Reserved");
} // PureRendererHWfixedPipeImpl(...)


PureRendererHWfixedPipeImpl::PureRendererHWfixedPipeImpl(
    PureWindow& _wnd,
    PureScreen& _scr,
    PureHwInfo& _hwinfo ) :
    wnd( _wnd ),
    hwInfo( _hwinfo ),
    screen( _scr ),
    pObject3DMgr( NULL ),
    pCamera( NULL ),
    pUImgr( NULL ),
    bShowGuiDemo(false)
{
    rc = NULL;
    stats.push_back( PureRendererHWfixedPipeImpl::CurrentStats() );
    stats[0].renderHints = DefaultHints;
    timeDurationStart.tv_sec = 0;
    timeDurationStart.tv_usec = 0;

    // CPP11 initializer list!!!
    mapRenderPaths2String[PURE_RH_RP_LEGACY_PR00FPS]   = std::string("Legacy PR00FPS");
    mapRenderPaths2String[PURE_RH_RP_DISTANCE_ORDERED] = std::string("Distance Ordered");
    mapRenderPaths2String[PURE_RH_RP_OCCLUSION_CULLED] = std::string("Occlusion Culled");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_1]          = std::string("Reserved");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_2]          = std::string("Reserved");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_3]          = std::string("Reserved");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_4]          = std::string("Reserved");
    mapRenderPaths2String[PURE_RH_RP_RSRVD_5]          = std::string("Reserved");

    mapOcclusionQueryMethod2String[PURE_RH_OQ_METHOD_SYNC] = std::string("Sync");
    mapOcclusionQueryMethod2String[PURE_RH_OQ_METHOD_ASYNC] = std::string("Async");
    mapOcclusionQueryMethod2String[PURE_RH_OQ_METHOD_RSRVD_1] = std::string("Reserved");
    mapOcclusionQueryMethod2String[PURE_RH_OQ_METHOD_RSRVD_2] = std::string("Reserved");
} // PureRendererHWfixedPipeImpl(...)


PureRendererHWfixedPipeImpl::PureRendererHWfixedPipeImpl(const PureRendererHWfixedPipeImpl&) :
    wnd( PureWindow::createAndGet() ),
    hwInfo( PureHwInfo::get() ),
    screen( PureScreen::createAndGet() ),
    pUImgr( NULL ),
    pObject3DMgr( NULL ),
    pCamera( NULL ),
    bShowGuiDemo(false)
{
}


PureRendererHWfixedPipeImpl& PureRendererHWfixedPipeImpl::operator=(const PureRendererHWfixedPipeImpl&)
{
    return *this;
}


/**
    Writes OpenGL error to console only if there is really an error.
*/
void PureRendererHWfixedPipeImpl::printOGLerrorBrief()
{
    while ( PureGLsnippets::isGLErrorPresent() )
        getConsole().EOLn("Last error: %s",
                           PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));   
} // printOGLerrorBrief()


/**
    Writes OpenGL error to console even the error is no error.
*/
void PureRendererHWfixedPipeImpl::printOGLerrorFull()
{
    if ( PureGLsnippets::isGLErrorPresent() )
        getConsole().EOLn("Last error: %s",
                           PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
    else
        getConsole().SOLn("Last error: %s",
                           PureGLsnippets::getGLErrorTextFromEnum( PureGLsnippets::getLastSavedGLError() ));
} // printOGLerrorFull()


/**
    Logs given render hints with full descriptive text.
*/
void PureRendererHWfixedPipeImpl::LogFullRenderHints(const TPURE_RENDER_HINT& hints) const
{
    const TPureUInt iRenderPath = BITF_READ(hints, PURE_RH_RENDER_PATH_BITS, 3);
    const TPureUInt iOcclusionQueryMethod = BITF_READ(hints, PURE_RH_OQ_METHOD_BITS, 2);
    const TPureBool bOcclusionDrawBoundingBoxes = ( BIT_READ(hints, PURE_RH_OQ_DRAW_BOUNDING_BOXES_BIT) == 1u );
    const TPureBool bOcclusionDrawIfQueryPending = ( BIT_READ(hints, PURE_RH_OQ_DRAW_IF_QUERY_PENDING_BIT) == 1u );
    const TPureBool bOrderByDistance = ( BIT_READ(hints, PURE_RH_ORDERING_BY_DISTANCE_BIT) == 1u );

    const std::string sRenderPath = mapRenderPaths2String[iRenderPath];
    const std::string sOcclusionQueryMethod = mapOcclusionQueryMethod2String[iOcclusionQueryMethod];

    getConsole().OLn("iRenderPath: %u - %s", iRenderPath, sRenderPath.c_str());
    getConsole().OLn("iOcclusionQueryMethod: %u - %s", iOcclusionQueryMethod, sOcclusionQueryMethod.c_str());
    getConsole().OLn("bOcclusionDrawBoundingBoxes: %b", bOcclusionDrawBoundingBoxes);
    getConsole().OLn("bOcclusionDrawIfQueryPending: %b", bOcclusionDrawIfQueryPending);
    getConsole().OLn("bOrderByDistance: %b", bOrderByDistance);
    getConsole().OLn("");
} // LogFullRenderHints()


/**
    Logs last frame stats.
*/
void PureRendererHWfixedPipeImpl::LogLastFrameStats() const
{
    getConsole().OLnOI("Last Frame Statistics:");
    getConsole().OLn("Number of TOTAL objects: %u", pObject3DMgr->getOccluders().size() + pObject3DMgr->get3dOpaqueOccludees().size() + pObject3DMgr->get3dBlendedOccludees().size());
    getConsole().OLn(" - visibility enabled  : %u", lastFrameStats.getObjectsVisible());
    getConsole().OLn("Number of Occluders: %u", lastFrameStats.getObjectsOccluders() );
    getConsole().OLn("Number of Occludees w occlusion-TEST disabled: %u", lastFrameStats.getOccludeesNonOcclusionTested());
    getConsole().OLn("Number of Occludees w occlusion-TEST enabled : %u", lastFrameStats.getOccludeesOcclusionTested());
    getConsole().OLn(" - occluded    : %u", lastFrameStats.getOccludeesOcclusionTestedAndOccluded());
    getConsole().OLn(" - not occluded: %u", lastFrameStats.getOccludeesOcclusionTestedAndNonOccluded());
    getConsole().OLn("   - but visibility disabled anyway: %u", lastFrameStats.getOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway());
    getConsole().OLn("Number of transferred VERTICES : %u", lastFrameStats.getTransferredVertices());
    getConsole().OLn("Number of transferred TRIANGLES: %u", lastFrameStats.getTransferredTriangles());
    getConsole().OLnOO("");
} // LogLastFrameStats()


void PureRendererHWfixedPipeImpl::LogCollectedStats(TPureUInt i) const
{
    if ( i >= stats.size() )
    {
        getConsole().EOLn("%s(%u) invalid i", __FUNCTION__, i);
        assert(false);
        return;
    }

    getConsole().OLn("Total Frames Rendered: %u",   stats[i].nRenderedFrames);
    getConsole().OLn("Duration             : %f s", stats[i].fDurationSecs);
    getConsole().OLn("Average FPS          : %f",   stats[i].fAvgFps);
    getConsole().OLn("Average Frame Time   : %f s", stats[i].fAvgFrameTime);
    getConsole().OLn("Min Frame Time       : %u s %u us", stats[i].timeMinFrameTime.tv_sec, stats[i].timeMinFrameTime.tv_usec);
    getConsole().OLn("Max Frame Time       : %u s %u us", stats[i].timeMaxFrameTime.tv_sec, stats[i].timeMaxFrameTime.tv_usec);
    getConsole().OLn("");
} // LogCollectedStats()


void PureRendererHWfixedPipeImpl::LogLastCollectedStats() const
{
    LogFullRenderHints(stats[stats.size()-1].renderHints);
    LogCollectedStats(stats.size()-1);
} // LogLastCollectedStats()


/**
    Initializes OpenGL.
    Updates member rc.
*/
TPureBool PureRendererHWfixedPipeImpl::initializeOpenGL(HDC dc)
{
    getConsole().OLn("initializing OpenGL subsystem ...");

    GLint row, col;
    for (row = 0; row < 4; row++)
        for (col = 0; col < 4 ; col++)
            mat4x4Identity [row][col] = (row == col);

    if ( !( rc = wglCreateContext(dc) ) )
    {
        getConsole().EOLn("ERROR: wglCreateContext() failed!");
        return false;
    }
    getConsole().SOLn("wglCreateContext() passed");

    if ( !wglMakeCurrent(dc, rc) )
    {
        getConsole().EOLn("ERROR: wglMakeCurrent() failed!");
        shutdownOpenGL();
        return false;
    }
    getConsole().SOLn("wglMakeCurrent() passed");

    printOGLerrorFull();
 
    getConsole().SOLn("done!");
    return true;
} // initializeOpenGL()


/**
    Shuts down OpenGL.
    Updates member rc.
*/
TPureBool PureRendererHWfixedPipeImpl::shutdownOpenGL()
{
    if ( !rc )
        return true;

    getConsole().OLnOI("Shutting down OpenGL ...");
    if ( !wglMakeCurrent(NULL, NULL) )
    {
        getConsole().EOLnOO("ERROR: wglMakeCurrent(NULL, NULL) failed!");
        return false;
    }
    getConsole().SOLn("wglMakeCurrent(NULL, NULL) okay.");

    bool b;
    if ( b = (wglDeleteContext(rc) == TRUE) )
        getConsole().SOLn("wglDeleteContext(rc) okay."); 
    else
        getConsole().EOLn("ERROR: wglDeleteContext(rc) failed.");
        
    rc = NULL;
    getConsole().OO();
    return b;
} // shutdownOpenGL()


/**
    Sets some basic things in OpenGL.
*/
void PureRendererHWfixedPipeImpl::SetBasicThingsInOpenGL()
{
    glEnable(GL_DEPTH_TEST);  // some objects like sticked objects or 2-sided AND wireframed objects may temporarily modify this
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);  // permanent setting, don't change in middle of rendering to get use of depth buffer optimisations
    glDepthRange(0, 1);    // permanent
    glCullFace(GL_BACK);      // 2-sided objects may temporarily modify this
    glFrontFace(GL_CCW);      // usually this is true, however some negative-scaled objects may temporarily modify this
    glEnable(GL_SCISSOR_TEST);
    pglHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // permanent setting
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // instead of the default 4-byte boundary, we use 1 to support textures like 2x2
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat white[4] = {1.0f, 1.0f, 1.0f, 1.0};
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 127);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glFlush();
    glFinish();
} // SetBasicThingsInOpenGL()


/**
    Sets viewport size and clears buffers.
*/
void PureRendererHWfixedPipeImpl::BeginRendering()
{
    const GLint vpX = (GLint) pCamera->getViewport().pos.x;
    const GLint vpY = (GLint) ( wnd.getClientHeight() - (pCamera->getViewport().size.height + pCamera->getViewport().pos.y) );
    const GLint vpW = (GLint) pCamera->getViewport().size.width;
    const GLint vpH = (GLint) pCamera->getViewport().size.height;
    
    glViewport(vpX, vpY, vpW, vpH);
    glScissor(vpX, vpY, vpW, vpH);

    /*
        "A common error when using glDepthMask(GL_FALSE) is that you forget to enable it again before the next glClear;
         if it's still disabled, you will have last frame's depth information still in the buffer."
        https://www.reddit.com/r/opengl/comments/66bnsn/strange_behavior_with_depth_test_and_mask/
    */
    glDepthMask(GL_TRUE);

    switch (pCamera->getClearMode())
    {
    case PURE_CLEAR_ZBUFFER_COLORBUFFER:
        glClearColor(pCamera->getBackgroundColor().getRedAsFloat(),
            pCamera->getBackgroundColor().getGreenAsFloat(),
            pCamera->getBackgroundColor().getBlueAsFloat(),
            pCamera->getBackgroundColor().getAlphaAsFloat() );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        break;
    case PURE_CLEAR_ZBUFFER:
        glClear(GL_DEPTH_BUFFER_BIT);
        break;
    }
} // BeginRendering()


/**
    Sets perspective projection.
*/
void PureRendererHWfixedPipeImpl::SwitchToPerspectiveProjection()
{
    PureTransformMatrix mProj;
    mProj.SetFrustumByFovY(pCamera->getFieldOfView(), pCamera->getAspectRatio(), pCamera->getNearPlane(), pCamera->getFarPlane());

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(mProj.getTransposed());

    /*switch (pCamera->getAspectRatioMode())
    {
    case PURE_AM_FIX:     gluPerspective(pCamera->getFieldOfView(),
                                         pCamera->getAspectRatio(),
                                         pCamera->getNearPlane(), pCamera->getFarPlane());
                          break;
    case PURE_AM_DYNAMIC: gluPerspective(pCamera->getFieldOfView(),
                                         pCamera->getViewport().size.width / pCamera->getViewport().size.height,
                                         pCamera->getNearPlane(), pCamera->getFarPlane());
                          break;
    } */

    
    PureTransformMatrix mView;
    mView.SetLookAt(pCamera->getPosVec(), pCamera->getTargetVec(), pCamera->getUpVec());

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mView.getTransposed());
} // SwitchToPerspectiveProjection()


/**
    Sets orthographic projection.
*/
void PureRendererHWfixedPipeImpl::SwitchToOrtographicProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(*mat4x4Identity);
    //glLoadIdentity(); 
    glOrtho(
        0, pCamera->getViewport().size.width,
        0, pCamera->getViewport().size.height,
       -1, 1);
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    glLoadMatrixf(*mat4x4Identity);
    // initial translation so 2D objects will be placed correctly on screen
    glTranslatef(pCamera->getViewport().size.width/2.f, pCamera->getViewport().size.height/2.f, 0.f);
} // SwitchToOrtographicProjection()


struct CompareByZdistance
{
    enum Order
    {
        NEAREST_TO_FAREST, /* front-to-back */
        FAREST_TO_NEAREST  /* back-to-front */
    };
    
    PureVector vPosCam;
    PureVector vFwdCam;
    Order order;
    bool log;

    // std::sort() uses this to determine if a is less than b
    bool operator()(PureObject3D* a, PureObject3D* b) const
    {
        const PureVector vDistanceToCam_a = a->getPosVec() - vPosCam;
        const PureVector vDistanceToCam_b = b->getPosVec() - vPosCam;
        const TPureFloat fZdistanceToCam_a = vDistanceToCam_a.getDotProduct(vFwdCam);
        const TPureFloat fZdistanceToCam_b = vDistanceToCam_b.getDotProduct(vFwdCam);
        if ( log )
        {
            CConsole::getConsoleInstance().OLn("Comparator(%s, %s)", a->getName().c_str(), b->getName().c_str());
            CConsole::getConsoleInstance().OLn("  pos a: [%f, %f, %f], pos b: [%f, %f, %f], zdist a: %f, zdist b: %f",
                a->getPosVec().getX(), a->getPosVec().getY(), a->getPosVec().getZ(),
                b->getPosVec().getX(), b->getPosVec().getY(), b->getPosVec().getZ(),
                fZdistanceToCam_a, fZdistanceToCam_b);
        }
        // std::sort() sorts in non-descending order, if comparator also returns true if element 'a' is less than element 'b',
        // so in our case it is nearest to farest ordering
        return (order == NEAREST_TO_FAREST) ? (fZdistanceToCam_a < fZdistanceToCam_b) : (fZdistanceToCam_a > fZdistanceToCam_b);
    }
}; // struct CompareByZdistance


/**
    Orders objects by Z-distance relative to camera view.
    No effect if PURE_RH_ORDERING_BY_DISTANCE_BIT is not set.

    This approach is based on a comment on this page:
    https://community.khronos.org/t/sorting-objects-by-distance-for-correct-blending/49579/3
    From: k_szczech - Senior Member - May 2007
    "Few words about sorting:
     - Do not compute distance - compute dot product along camera’s Z axis - not only faster but also more accurate (z-buffer is not “spherical”).
     - After you have sorted by Z component you can perform additional sort. Instead of comparing Z coordinates of polygon’s center, compare two polygons by testing
       if one is on the clock wise or counter clock wise side of another. You can even use bubble sort this time since most polyons are in proper order anyway.
     - Some polygons can be merged into groups that never occlude each other. Imagine car windows - if you first draw all windows from the inside and then all windows
       from the outside then you get corret result. So instead of sorting all polygons you only have 2 groups which additionally are in constant relation with each other."

    For more about transparent object rendering:
     - https://stackoverflow.com/questions/37780345/opengl-how-to-create-order-independent-transparency/37783085#37783085
     - https://github.com/PixelClear/Order-Independent-Trasparency
*/
void PureRendererHWfixedPipeImpl::OrderObjectContainersByZdistance()
{
    if ( BIT_READ(stats[stats.size()-1].renderHints, PURE_RH_ORDERING_BY_DISTANCE_BIT) == 1u )
    {
        static CompareByZdistance comparator;
        comparator.vPosCam = pCamera->getPosVec();
        // cam forward calculation could be actually added as method to camera class!
        PureVector vForwardFromTargetAndPos = pCamera->getTargetVec() - pCamera->getPosVec();
        vForwardFromTargetAndPos.Normalize();
        comparator.vFwdCam = vForwardFromTargetAndPos;
        comparator.log = false;

        // this would be less complicated with C++11 lambda ...
        comparator.order = CompareByZdistance::Order::NEAREST_TO_FAREST;
        std::sort(pObject3DMgr->getOccluders().begin(), pObject3DMgr->getOccluders().end(), comparator);
        std::sort(pObject3DMgr->get3dOpaqueOccludees().begin(), pObject3DMgr->get3dOpaqueOccludees().end(), comparator);
        
        comparator.order = CompareByZdistance::Order::FAREST_TO_NEAREST;
            // following logging are useful to debug unit test: PureRendererHWfixedPipeTest::testRenderByZdistanceOrder
            // can be tied to a counter so it is logging only in every nth frame to avoid flooding
            /*CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
            getConsole().OLn("%s", __FUNCTION__);
            getConsole().OLn("Before sort:");
            for (auto obj : pObject3DMgr->get3dBlendedOccludees())
            {
                getConsole().OLn(" %s, pos: [%f, %f, %f]", obj->getName().c_str(), obj->getPosVec().getX(), obj->getPosVec().getY(), obj->getPosVec().getZ());
            }
            getConsole().OI();
            getConsole().OLn("Camera pos: [%f, %f, %f]", comparator.vPosCam.getX(), comparator.vPosCam.getY(), comparator.vPosCam.getZ());
            getConsole().OLn("Camera fwd: [%f, %f, %f]", comparator.vFwdCam.getX(), comparator.vFwdCam.getY(), comparator.vFwdCam.getZ());*/

        std::sort(pObject3DMgr->get3dBlendedOccludees().begin(), pObject3DMgr->get3dBlendedOccludees().end(), comparator);

            /*getConsole().OO();
            getConsole().OLn("After sort:");
            for (auto obj : pObject3DMgr->get3dBlendedOccludees())
            {
                getConsole().OLn(" %s, pos: [%f, %f, %f]", obj->getName().c_str(), obj->getPosVec().getX(), obj->getPosVec().getY(), obj->getPosVec().getZ());
            }
            getConsole().OLn("");
            CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", false);*/

    }
} // OrderObjectContainersByZdistance()


/**
    Draws 3D objects, the legacy PR00FPS way.
*/
void PureRendererHWfixedPipeImpl::Draw3DObjects_Legacy(PureIRenderer& renderer)
{    
    bool blended = true;
    for (int iBlend = 1; iBlend < 3; iBlend++)
    {
        blended = !blended;

        for (int i = 0; i < pObject3DMgr->getSize(); i++)
        {
            PureObject3D* const obj = (PureObject3D*) pObject3DMgr->getAttachedAt(i);
        
            if ( obj == PGENULL )
                continue;
        
            if ( (blended == PureMaterial::isBlendFuncReallyBlending(obj->getMaterial(false).getSourceBlendFunc(), obj->getMaterial(false).getDestinationBlendFunc()))
                 &&
                 ( obj->isRenderingAllowed() )
                 &&
                 ( !obj->isStickedToScreen() )
               )
            {
                glPushMatrix();
                obj->draw(PURE_RPASS_NORMAL, false, false);
                glPopMatrix();
                lastFrameStats.Update(*obj, PURE_RPASS_NORMAL);
            }
        } // for i    
    } // for iBlend
    
} // Draw3DObjects_Legacy


/**
    Draws 3D objects using separate Object3D containers.
    The difference compared to Draw3DObjects_Legacy():
     - iterating separate Object3D containers instead of Object3DManager's base manageds container;
     - these separate containers are ordered by Z-distance from camera, so hopefully it makes less overdraw.
*/
void PureRendererHWfixedPipeImpl::Draw3DObjects_Ordered_Containers(PureIRenderer& renderer)
{    
    static TPureUInt frameCntr = 0;

    OrderObjectContainersByZdistance();

    for (auto it = pObject3DMgr->getOccluders().begin(); it != pObject3DMgr->getOccluders().end(); it++)
    {
        glPushMatrix();
        (*it)->draw(PURE_RPASS_NORMAL, false, false);
        glPopMatrix();
        lastFrameStats.Update(*(*it), PURE_RPASS_NORMAL);
    }

    for (auto it = pObject3DMgr->get3dOpaqueOccludees().begin(); it != pObject3DMgr->get3dOpaqueOccludees().end(); it++)
    {
        glPushMatrix();
        (*it)->draw(PURE_RPASS_NORMAL, false, false);
        glPopMatrix();
        lastFrameStats.Update(*(*it), PURE_RPASS_NORMAL);
    }

    for (auto it = pObject3DMgr->get3dBlendedOccludees().begin(); it != pObject3DMgr->get3dBlendedOccludees().end(); it++)
    {
        glPushMatrix();
        (*it)->draw(PURE_RPASS_NORMAL, false, false);
        glPopMatrix();
        lastFrameStats.Update(*(*it), PURE_RPASS_NORMAL);
    }

    frameCntr++;

    if ( OQ_AUTO_UPDATE_OCCLUDER_STATES )
    {
        if ( (frameCntr % 100) == 0 )
        {
           pObject3DMgr->UpdateOccluderStates();
        }
    }   
} // Draw3DObjects_Ordered_Containers


/**
    Draws 3D objects with occlusion query.
    The difference compared to Draw3DObjects_Ordered_Containers():
     - occlusion query is started for occludees, to decide if they should be really rendered.

    @param bASyncQuery If true, the result of occlusion queries are not waited for in the same frame, instead they will be
                       evaulated in a future frame whenever they are available.
                       If false, all occlusion queries must finish in the frame, to decide if we should really render the occludee.
*/
void PureRendererHWfixedPipeImpl::Draw3DObjects_OcclusionQuery(PureIRenderer& renderer, TPureBool bASyncQuery)
{
    static TPureUInt frameCntr = 0;

    const TPureBool bRenderIfQueryPending = ( BIT_READ(stats[stats.size()-1].renderHints, PURE_RH_OQ_DRAW_IF_QUERY_PENDING_BIT) == 1u );

    OrderObjectContainersByZdistance();

    if ( OQ_ZPASS_FOR_OCCLUDERS )
    {
        // first render the occluders into Z-buffer only
        PureGLsnippets::SetZPassRendering(true);

        for (auto it = pObject3DMgr->getOccluders().begin(); it != pObject3DMgr->getOccluders().end(); it++)
        {
            glPushMatrix();
            (*it)->draw(PURE_RPASS_Z_ONLY, bASyncQuery, bRenderIfQueryPending);
            glPopMatrix();
            lastFrameStats.Update(*(*it), PURE_RPASS_Z_ONLY);
        }

        PureGLsnippets::SetZPassRendering(false);
    } // zpass for occluders

    // first render the occluders normally
    for (auto it = pObject3DMgr->getOccluders().begin(); it != pObject3DMgr->getOccluders().end(); it++)
    {
        glPushMatrix();
        (*it)->draw(PURE_RPASS_NORMAL, bASyncQuery, bRenderIfQueryPending);
        glPopMatrix();
        lastFrameStats.Update(*(*it), PURE_RPASS_NORMAL);
    }

    // then render the occludees with occlusion query
    for (/*TPURE_RENDER_PASS*/ int iRenderPass = PURE_RPASS_START_OCCLUSION_QUERY; iRenderPass <= PURE_RPASS_NORMAL; iRenderPass++)
    {
        PureGLsnippets::SetGLBoundingBoxRendering(iRenderPass == PURE_RPASS_START_OCCLUSION_QUERY);

        for (auto it = pObject3DMgr->get3dOpaqueOccludees().begin(); it != pObject3DMgr->get3dOpaqueOccludees().end(); it++)
        {
            glPushMatrix();
            (*it)->draw((TPURE_RENDER_PASS)iRenderPass, bASyncQuery, bRenderIfQueryPending);
            glPopMatrix();
            lastFrameStats.Update(*(*it), (TPURE_RENDER_PASS)iRenderPass);
        }

        for (auto it = pObject3DMgr->get3dBlendedOccludees().begin(); it != pObject3DMgr->get3dBlendedOccludees().end(); it++)
        {
            glPushMatrix();
            (*it)->draw((TPURE_RENDER_PASS)iRenderPass, bASyncQuery, bRenderIfQueryPending);
            glPopMatrix();
            lastFrameStats.Update(*(*it), (TPURE_RENDER_PASS)iRenderPass);
        }

    } // occludess iRenderPass

    if ( BIT_READ(stats[stats.size()-1].renderHints, PURE_RH_OQ_DRAW_BOUNDING_BOXES_BIT) == 1u )
    {
        PureGLsnippets::glPrepareBeforeDrawBoundingBox();

        for (auto it = pObject3DMgr->getOccluders().begin(); it != pObject3DMgr->getOccluders().end(); it++)
        {
            glPushMatrix();
            (*it)->draw(PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY, bASyncQuery, bRenderIfQueryPending);
            glPopMatrix();
            lastFrameStats.Update(*(*it), PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY);
        }

        for (auto it = pObject3DMgr->get3dOpaqueOccludees().begin(); it != pObject3DMgr->get3dOpaqueOccludees().end(); it++)
        {
            glPushMatrix();
            (*it)->draw(PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY, bASyncQuery, bRenderIfQueryPending);
            glPopMatrix();
            lastFrameStats.Update(*(*it), PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY);
        }

        for (auto it = pObject3DMgr->get3dBlendedOccludees().begin(); it != pObject3DMgr->get3dBlendedOccludees().end(); it++)
        {
            glPushMatrix();
            (*it)->draw(PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY, bASyncQuery, bRenderIfQueryPending);
            glPopMatrix();
            lastFrameStats.Update(*(*it), PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY);
        }
    } // OQ_RENDER_BOUNDING_BOXES

    frameCntr++;

    if ( OQ_AUTO_UPDATE_OCCLUDER_STATES )
    {
        if ( (frameCntr % 100) == 0 )
        {
           pObject3DMgr->UpdateOccluderStates();
        }
    }

    if ( (frameCntr % 1000) == 0 )
    {
        frameCntr = 0;
        
        /*getConsole().SetLoggingState(PureRendererHWfixedPipe::getLoggerModuleName(), true);
        getConsole().OLn("");
        getConsole().SetLoggingState(PureRendererHWfixedPipe::getLoggerModuleName(), false);*/
    } // frameCntr
    
} // Draw3DObjects_OcclusionQuery


/**
    Draws 2D objects (stickedToScreen and UI).
*/
void PureRendererHWfixedPipeImpl::Draw2DObjects(PureIRenderer& renderer)
{
    /* legacy 2D code begins */

    for (int i = 0; i < pObject3DMgr->getSize(); i++)
    {
        PureObject3D* const obj = (PureObject3D*) pObject3DMgr->getAttachedAt(i);
        if ( obj == PGENULL )
            continue;

        if ( ( obj->isRenderingAllowed() )
                &&
             ( obj->isStickedToScreen() )
            )
        {
            glPushMatrix();
            obj->draw(PURE_RPASS_NORMAL, false, false);
            glPopMatrix();
        }
    } // for i

    /* legacy 2D code ends */

    glTranslatef( -pCamera->getViewport().size.width / 2.0f, -pCamera->getViewport().size.height / 2.0f, 0.0f );
    pUImgr->Render();

    glLoadMatrixf(*mat4x4Identity);
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (bShowGuiDemo)
    {
        ImGui::ShowDemoWindow(&bShowGuiDemo);
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
} // Draw2DObjects()


/**
    Forces pending tasks to be finished and displays the rendered picture.
*/
void PureRendererHWfixedPipeImpl::FinishRendering()
{
    glFlush(); // async: doesn't wait, just sends out previously buffered commands for execution
    SwapBuffers( wnd.getWndDC() ); // async: doesn't wait, just requests buffer swap to be done in near future
    //glFinish(); // sync: waits for all previously sent commands to be executed
} // FinishRendering()


/*
   PureRendererHWfixedPipe
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PureRendererHWfixedPipe& PureRendererHWfixedPipe::createAndGet(
    PureWindow& _wnd,
    PureScreen& _scr,
    PureHwInfo& _hwinfo )
{
    static PureRendererHWfixedPipeImpl inst(_wnd, _scr, _hwinfo);
    return inst;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureRendererHWfixedPipe::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureRendererHWfixedPipe::getLoggerModuleName()
{
    return "PureRendererHWfixedPipe";
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


