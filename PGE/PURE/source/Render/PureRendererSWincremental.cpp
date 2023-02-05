/*
    ###################################################################################
    PureRendererSWincremental.cpp
    This file is part of PURE.
    Incremental SW Renderer class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Render/PureRendererSWincremental.h"
#include "../../include/external/Render/PureIIncrementalRenderPipeline.h"
#include "../../include/external/Math/PureVector.h"
#include "../../include/external/Math/PureTransformMatrix.h"
#include "../../include/external/Object3D/PureObject3DManager.h"
#include "../../include/internal/PureGLsnippets.h"
#include "../../include/internal/PurePragmas.h"
#include "../../include/internal/Math/PUREScissor.h"
#include "../../../../../PFL/PFL/PFL.h"

using namespace std;


/*
   PureRendererSWincrementalImpl
   ###########################################################################
*/


class PureRendererSWincrementalImpl :
    public PureRendererSWincremental,
    public PureIIncrementalRenderPipeline
{
public:
    virtual ~PureRendererSWincrementalImpl();       /**< Calls shutdown(). */

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

    void RenderObject(PureObject3D& object);

    void RenderScene(); 

    void ShowGuiDemo(TPureBool state);

    const TPURE_RENDER_HINT& getRenderHints();
    void  SetRenderHints(const TPURE_RENDER_HINT& hints);

    void ResetStatistics();
    
    void WriteStats() const;       
    void CheckConsistency() const;  

private:
    TPURE_RENDER_HINT    renderHints;  /**< Render hints. */
    TPureBool            bInited;
    PGEcfgProfiles&      m_cfgProfiles;
    PureWindow&          wnd;          /**< Our window, where we draw to, singleton. */
    PureHwInfo&          hwInfo;       /**< Hardware infos, singleton. */
    PureScreen&          screen;       /**< Our screen, singleton. */
    PureObject3DManager* pObject3DMgr; /**< ObjectManager, at least 1 instance. */
    PureCamera*          pCamera;      /**< Camera, at least 1 instance. */
    PureUiManager*       pUImgr;       /**< UI manager, singleton. */

    TPureBool            bLighting;

    HDC        hColorBufferDC;         /**< Memory Device Context for Color Buffer, compatible with wnd's DC. */
	BITMAPINFO biColorBufferDDB;	   /**< Attributes of Color Buffer DIB. */
	HBITMAP    hColorBufferDDB;		   /**< Handle to Color Buffer DIB. */
    BYTE*      pColorBufferDIB;        /**< Color buffer. */
	float*     pZBuffer;               /**< Depth buffer. */

    // ---------------------------------------------------------------------------

    PureRendererSWincrementalImpl();                /**< NULLs members only. */
    PureRendererSWincrementalImpl(
        PGEcfgProfiles& cfgProfiles,
        PureWindow& _wnd,
        PureScreen& _scr,
        PureHwInfo& _hwinfo );
    PureRendererSWincrementalImpl(const PureRendererSWincrementalImpl&);
    PureRendererSWincrementalImpl& operator=(const PureRendererSWincrementalImpl&);

    void VertexProcessing(
        const TXYZ* pVertices,
        TPureUInt nVertices_h,
        bool bIndexed,
        const void* pVertexIndices,
        TPureUInt nVertexIndices_h,
        unsigned int nIndicesType,
        TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
        const TXYZ& pos,
        const TXYZ& rot,
        const TXYZ& scaling,
        const TXYZ& campos,
        const TXYZ& camrot );

    void PrimitiveAssembly(
        TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
        TPureUInt nVertexIndices_h,
        const TXYZ& pos,
        const TXYZ& rot,
        const TXYZ& campos,
        const TXYZ& camrot,
        TPureInt vp_x,
        TPureInt vp_y,
        TPureUInt vp_w,
        TPureUInt vp_h);

    void GeometricStage(
        const TXYZ* pVertices,
        TPureUInt nVertices_h,
        bool bIndexed,
        const void* pVertexIndices,
        TPureUInt nVertexIndices_h,
        unsigned int nIndicesType,
        TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
        const TXYZ& pos,
        const TXYZ& rot,
        const TXYZ& scaling,
        const TXYZ& campos,
        const TXYZ& camrot,
        TPureInt vp_x,
        TPureInt vp_y,
        TPureUInt vp_w,
        TPureUInt vp_h);
    void RasterStage();
    void SetPixel(
        int x,
        int y,
        TRGBAUBYTE color,
        TPureInt vp_x,
        TPureInt vp_y,
        TPureUInt vp_w,
        TPureUInt vp_h);
    void Line(
        int x1, int y1,
        int x2, int y2,
        TRGBAUBYTE color,
        TPureInt vp_x, TPureInt vp_y,
        TPureUInt vp_w, TPureUInt vp_h
    );
    void RenderStage(
        TPureUInt nVertexIndices_h,
        TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
        TPureInt vp_x,
        TPureInt vp_y,
        TPureUInt vp_w,
        TPureUInt vp_h);

    void ClearColorBuffer();
    void BeginRendering();                 /**< Sets viewport size and clears buffers. */
    void SwitchToPerspectiveProjection();  /**< Sets perspective projection. */
    void SwitchToOrtographicProjection();  /**< Sets orthographic projection. */
    void Draw3DObjects();                  /**< Draws 3D objects. */
    void Draw2DObjects();                  /**< Draws 2D objects. */
    void SyncToScreen();
    void FinishRendering();                /**< Forces pending tasks to be finished and displays the rendered picture. */

    void printFeatureList(
        const std::string& features,
         const char* endstring);       /**< Writes the given list to console. */           
    void printOGLfeatures();           /**< Writes OGL feature list to console. */
    void printWGLfeatures();           /**< Writes WGL feature list to console. */

    friend class PureRendererSWincremental;

}; // PureRendererSWincrementalImpl


// ############################### PUBLIC ################################


/**
    Calls shutdown().
*/
PureRendererSWincrementalImpl::~PureRendererSWincrementalImpl()
{
    shutdown();
} // ~PureRendererSWincrementalImpl()


/**
    Documentation is at public PureRendererSWincremental class definition.
*/
TPureUInt PureRendererSWincrementalImpl::initialize(
    TPureUInt width, TPureUInt height,
    TPURE_DISPLAY_MODES dmode, TPureUInt freq,
    TPureInt cdepth, TPureInt zdepth, TPureInt stencil,
    TPureInt samples,
    HWND window)
{
    TPureInt st = stencil;

    getConsole().OLn("PureRendererSWincremental::initialize(%d, %d, dmode, %d, %d, %d, %d, %d)",
                      width, height, freq, cdepth, zdepth, st, samples);
    if ( isInitialized() )
    {
        getConsole().SOLn("  > already initialized!");
        return 1;
    }

    getConsole().OI();

    // we ignore stencil bits
    if ( st != 0 )
    {
        st = 0;
        getConsole().OLn("WARNING: ignoring non-0 stencil!");
    }

    // we ignore FSAA level as well ...
    if ( samples != 0 )
    {
        samples = 0;
        getConsole().OLn("WARNING: ignoring non-0 FSAA samples!");
    }

    getConsole().OLn("");
    getConsole().OLn("#### STAGE 1/3 ####");

        getConsole().OLn("screen settings ...");
        screen.SetResolution(width, height);
        screen.SetFullScreened( dmode != PURE_WINDOWED );  // == PURE_FULLSCREEN nem jó, mert lesz 3. FS mód is ...
        screen.SetFreq( freq );
        screen.SetColorBits( cdepth );
        screen.SetDepthBits( zdepth );
      
        getConsole().OLn("new PureWindow...");
        if ( !wnd.initialize(screen.getResWidth(), screen.getResHeight(), dmode != PURE_WINDOWED, "Window", window) )
        {
            getConsole().EOLnOO("ERROR: wnd.initialize() failed!");
            shutdown();
            return 1;
        }
        getConsole().SOLn("done!");

        if ( !screen.applyDisplaySettings(wnd.getWndDC(), PURE_SCREEN_PF_NONE) )
        {
            getConsole().EOLnOO("ERROR: applyDisplaySettings() failed!");
            shutdown();
            return 1;
        }

        getConsole().OLn("Allocating frame buffer ...");

            hColorBufferDC = CreateCompatibleDC( wnd.getWndDC() );
            if ( hColorBufferDC == NULL )
	        {
                getConsole().EOLnOO("ERROR: CreateCompatibleDC() failed!");
                shutdown();
                return 1;
            }

	        memset(&biColorBufferDDB, 0, sizeof(biColorBufferDDB));
	        biColorBufferDDB.bmiHeader.biSize = sizeof(biColorBufferDDB);
	        biColorBufferDDB.bmiHeader.biPlanes   =  1;
	        biColorBufferDDB.bmiHeader.biBitCount = 24;
	        biColorBufferDDB.bmiHeader.biCompression = BI_RGB;
	        biColorBufferDDB.bmiHeader.biWidth     = width;
	        biColorBufferDDB.bmiHeader.biHeight    = height;
	        biColorBufferDDB.bmiHeader.biSizeImage = width * height * 3;
	        hColorBufferDDB = CreateDIBSection(wnd.getWndDC(), &biColorBufferDDB, DIB_RGB_COLORS, (void**)&pColorBufferDIB, NULL, 0);
            if ( hColorBufferDDB == NULL )
            {
                getConsole().EOLnOO("ERROR: CreateDIBSection() failed!");
                shutdown();
                return 1;
            }

            // TODO: GdiFlush() maybe needed when renderer renders the picture later.

            try
            {
                pZBuffer = new float[width * height];
            }
            catch (const std::bad_alloc&)
            {
                getConsole().EOLnOO("ERROR: failed to allocate Z-Buffer!");
                shutdown();
                return 1;
            }

        getConsole().SOLn("done!");

    getConsole().OLn("");
    getConsole().OLn("#### STAGE 2/3 ####");

        hwInfo.Initialize(
            NULL, wnd.getWndDC(),
            screen.getResWidth(), screen.getResHeight(),
            screen.getColorBits(), screen.getDepthBits(),
            screen.getStencilBits(), screen.getFSAAlevel() );

    getConsole().OLn("");
    getConsole().OLn("#### STAGE 3/3 ####");
    getConsole().OLn("");

        // GFX card drivers' default setting in 2015: off (if undefined by application), so we also set it to false initially
        screen.SetVSyncEnabled(false);
        bInited = true;

    getConsole().OLn("");
    getConsole().SOLn("> Incremental SW Renderer initialized");
    getConsole().OLnOO("");

    return 0;

} // initialize()


TPureBool PureRendererSWincrementalImpl::shutdown()
{
    getConsole().OLn("PureRendererSWincremental::shutdown()");
    
    getConsole().OIOLn("Shutting down renderer ...");
    getConsole().OI();

    delete[] pZBuffer;
    pZBuffer = NULL;

    DeleteObject(hColorBufferDDB);
    hColorBufferDDB = NULL;

    DeleteDC(hColorBufferDC);
    hColorBufferDC = NULL;

    wnd.Deinitialize();
    screen.ResetDisplaySettings();
    hwInfo.Deinitialize();

    getConsole().OO();

    getConsole().OLnOO("Renderer is shut down!");

    return true;
} // shutdown()


TPureBool PureRendererSWincrementalImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


void PureRendererSWincrementalImpl::SetManagers(
    PureObject3DManager* _objmgr,
    PureUiManager* _uimgr,
    PureCamera* _cam )
{
    pObject3DMgr = _objmgr;
    pUImgr = _uimgr;
    pCamera = _cam;
}


void PureRendererSWincrementalImpl::RenderObject(PureObject3D& object)
{
    // TODO
}


void PureRendererSWincrementalImpl::RenderScene()
{
    if ( !isInitialized() )
    {
        return;
    }

    BeginRendering();

    Draw3DObjects();

    FinishRendering();

} // RenderScene()

void PureRendererSWincrementalImpl::ShowGuiDemo(TPureBool)
{
    // this is doing nothing intentionally since we dont have SW implementation for GUI
}


const TPURE_RENDER_HINT& PureRendererSWincrementalImpl::getRenderHints()
{
    return renderHints;
} // getRenderHints()


void PureRendererSWincrementalImpl::SetRenderHints(const TPURE_RENDER_HINT& hints)
{
    renderHints = hints;
} // SetRenderHints()


void PureRendererSWincrementalImpl::ResetStatistics()
{
} // ResetStatistics()


void PureRendererSWincrementalImpl::WriteStats() const
{
    getConsole().OLn("PureRendererSWincremental::WriteStats()");
    getConsole().L();
} // WriteStats()


/**
    Runs an internal consistency test, just for debug.
*/
void PureRendererSWincrementalImpl::CheckConsistency() const
{
    getConsole().OLn("PureRendererSWincremental::CheckConsistency()");

    getConsole().OLn("END of consistency check!");
} // CheckConsistency()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    NULLs members only.
*/                                                                         
PureRendererSWincrementalImpl::PureRendererSWincrementalImpl() :
    renderHints(DefaultHints),
    m_cfgProfiles(m_cfgProfiles),
    wnd( wnd ),
    hwInfo( hwInfo ),
    screen( screen ),
    pObject3DMgr( NULL ),
    pCamera( NULL ),
    pUImgr( NULL ),
    hColorBufferDC( NULL ),
    hColorBufferDDB( NULL ),
    pColorBufferDIB( NULL ),
    pZBuffer( NULL )
{
    bInited = false;
} // PureRendererSWincrementalImpl(...)


PureRendererSWincrementalImpl::PureRendererSWincrementalImpl(
    PGEcfgProfiles& cfgProfiles,
    PureWindow& _wnd,
    PureScreen& _scr,
    PureHwInfo& _hwinfo ) :
    renderHints(DefaultHints),
    m_cfgProfiles(cfgProfiles),
    wnd( _wnd ),
    hwInfo( _hwinfo ),
    screen( _scr ),
    pObject3DMgr( NULL ),
    pCamera( NULL ),
    pUImgr( NULL ),
    hColorBufferDC( NULL ),
    hColorBufferDDB( NULL ),
    pColorBufferDIB( NULL ),
    pZBuffer( NULL )
{
    bInited = false;
} // PureRendererSWincrementalImpl(...)


PureRendererSWincrementalImpl::PureRendererSWincrementalImpl(const PureRendererSWincrementalImpl& other) :
    renderHints(DefaultHints),
    m_cfgProfiles(other.m_cfgProfiles),
    wnd( other.wnd ),
    hwInfo( PureHwInfo::get() ),
    screen( PureScreen::createAndGet() ),
    pUImgr( NULL ),
    pObject3DMgr( NULL ),
    pCamera( NULL ),
    hColorBufferDC( NULL ),
    hColorBufferDDB( NULL ),
    pColorBufferDIB( NULL ),
    pZBuffer( NULL )
{
}


PureRendererSWincrementalImpl& PureRendererSWincrementalImpl::operator=(const PureRendererSWincrementalImpl&)
{
    return *this;
}


/**
    Loop over the input vertices and transform them into pVerticesTransf array.
    Number of input vertices is nVertices_h.
    If bIndexed is true:
        - pVertexIndices != NULL;
        - nVertexIndices_h > 0;
        - number of output vertices is nVertexIndices_h;
        - nVertices_h <= nVertexIndices_h;
    else
        - pVertexIndices = NULL;
        - nVertexIndices_h = 0;
        - number of output vertices is nVertices_h.

    Transformation is: Model -> World -> View -> Clip coordinate systems.
*/
void PureRendererSWincrementalImpl::VertexProcessing(
    const TXYZ* pVertices,
    TPureUInt nVertices_h,
    bool bIndexed,
    const void* pVertexIndices,
    TPureUInt nVertexIndices_h,
    unsigned int nIndicesType,
    TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
    const TXYZ& pos,
    const TXYZ& rot,
    const TXYZ& scaling,
    const TXYZ& campos,
    const TXYZ& camrot
) 
{
    if ( pVertices == NULL || nVertices_h == 0 )
        return;

    if ( bIndexed )
    {
        if ( pVertexIndices == NULL || nVertexIndices_h == 0 )
            return;
    }

    if ( pVerticesTransf == NULL )
        return;

    /* we need completely defined triangles */
    if ( nVertexIndices_h % 3 != 0 )
        return;

    for (TPureUInt i = 0; i < nVertexIndices_h; i++)
    {
        const TXYZ& vertex = bIndexed ? pVertices[ PureGLsnippets::getVertexIndex(pVertexIndices, i, nIndicesType) ] : pVertices[i];
        PureVector transfdVertex(vertex.x, vertex.y, vertex.z);

        /* Model -> World */
        PureTransformMatrix mModelTr;
        mModelTr.SetTranslation(pos.x, pos.y, pos.z);

        PureTransformMatrix mScalingTr;
        mScalingTr.SetScale(scaling.x, scaling.y, scaling.z);
        transfdVertex = mScalingTr * transfdVertex;
        // Note that we need to detect negative scaling factor and change front face winding order on-the-fly, see details in PureObject3D::PureObject3DImpl::Draw_ApplyTransformations()
        
        PureTransformMatrix mRotation;
        // TODO: we should set rotation on all axes with 1 matrix but unfortunately currently we just have separate setrotation methods for each axes
        // so we need 3 different rotation matrices for each axis, and this also introduces the rotation order problem.
       
        
        mRotation.SetRotationX(rot.x);
        transfdVertex = mRotation * transfdVertex;
        mRotation.SetRotationY(abs(rot.y-360));   // TODO: in proofps, weapon is rotating in other direction ... probably my SetRotationY() is bad from LH/RH point of view ???
        transfdVertex = mRotation * transfdVertex;
        mRotation.SetRotationZ(rot.z);
        transfdVertex = mRotation * transfdVertex;

        transfdVertex = mModelTr * transfdVertex;

        /* World -> View/Eye */
        PureTransformMatrix mViewTr;
        mViewTr.SetLookAt(pCamera->getPosVec(), pCamera->getTargetVec(), pCamera->getUpVec());
        transfdVertex = mViewTr * transfdVertex;

        /* View/Eye -> Clip */
        /* The pyramidal frustum in eye space becomes a cube, therefore, the clipping plane equations in clip space are: X + Wc = 0, Y + Wc = 0, Z + Wc = 0, etc. */
        PureTransformMatrix mProjTr;
        mProjTr.SetFrustumByFovY(pCamera->getFieldOfView(), pCamera->getAspectRatio(), pCamera->getNearPlane(), pCamera->getFarPlane());
        transfdVertex = mProjTr * transfdVertex;

        pVerticesTransf[i].coords = transfdVertex.getXYZW();
    }
}


/**
    Loop over the input triangles and throw away those which are outside of the view frustum.
    Transform the rest to screen coordinates.

    Transformation is: Clip -> NDC -> Screen coordinate systems.
*/
void PureRendererSWincrementalImpl::PrimitiveAssembly(
    TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
    TPureUInt nVertexIndices_h,
    const TXYZ& pos,
    const TXYZ& rot,
    const TXYZ& campos,
    const TXYZ& camrot,
    TPureInt vp_x,
    TPureInt vp_y,
    TPureUInt vp_w,
    TPureUInt vp_h
)
{
    if ( pVerticesTransf == NULL || nVertexIndices_h == 0 )
        return;

    /* we need completely defined triangles */
    if ( nVertexIndices_h % 3 != 0 )
        return;

    /* We are in clip space now, meaning that clip planes are now parallel with axes, so frustum culling / clipping is very easy to do. */
    /* The clip coordinates, Xc, Yc and Zc are tested by comparing with Wc. If any clip coordinate is less than -Wc, or greater than Wc, then the vertex is outside of the view frustum. */
    /* Proposed WoW: for each triangle, check the location of all 3 vertices to the view frustum:
       - if all vertices are inside, we are fine;
       - if all vertices are outside, throw away the triangle somehow at this stage to decrease the workload on Render Stage;
       - if the triangle is partially inside and outside the frustum, it needs to be clipped, that will be taken care in Render Stage with some 2D-scissoring. */
    /* Current WoW: regardless of to which triangle a vertex is related, we mark each vertex if it is inside the view frustum or not (draw bool variable).
       Then Render Stage checks if all vertices of a triangle is outside: if so, triangle will be dropped there. */
    // TODO: Note that clipping could be also done in 3D-space now, maybe sometime in future I implement it.
    //       It would be nicer because I think responsibility of Primitive Assembly is to output proper triangles for the rasterizer and if needed, insert extra triangles. */

    for (TPureUInt i = 0; i < (nVertexIndices_h / 3); i++)
    {
        for (TPureUInt j = 0; j < 3; j++)
        {
            const TPureUInt vi = i*3 + j;
            if ( (pVerticesTransf[vi].coords.x >= -pVerticesTransf[vi].coords.w) && (pVerticesTransf[vi].coords.x <= pVerticesTransf[vi].coords.w)
                 &&
                 (pVerticesTransf[vi].coords.y >= -pVerticesTransf[vi].coords.w) && (pVerticesTransf[vi].coords.y <= pVerticesTransf[vi].coords.w)
                 &&
                 (pVerticesTransf[vi].coords.z >= -pVerticesTransf[vi].coords.w) && (pVerticesTransf[vi].coords.z <= pVerticesTransf[vi].coords.w) )
            {
                /* triangle is partially inside the view frustum, do not throw it away */
                pVerticesTransf[vi].draw = true;
            }
            else
            {
                pVerticesTransf[vi].draw = false;
            }
        }
    }

    /* perspective divide: input clip coordinates, output normalized device coordinates */
    for (TPureUInt i = 0; i < nVertexIndices_h; i++)
    {
        pVerticesTransf[i].coords.x = pVerticesTransf[i].coords.x / pVerticesTransf[i].coords.w;
        pVerticesTransf[i].coords.y = pVerticesTransf[i].coords.y / pVerticesTransf[i].coords.w;
        pVerticesTransf[i].coords.z = pVerticesTransf[i].coords.z / pVerticesTransf[i].coords.w;
    }

    const float DEPTH_RANGE_MIN = 0.0f;
    const float DEPTH_RANGE_MAX = 1.0f;

    /* viewport transf: input normalized device coordinates, output screen coordinates */
    for (TPureUInt i = 0; i < nVertexIndices_h; i++)
    {
        pVerticesTransf[i].coords.x = vp_w / 2 * pVerticesTransf[i].coords.x + vp_x + vp_w / 2;
        pVerticesTransf[i].coords.y = vp_h / 2 * pVerticesTransf[i].coords.y + vp_y + vp_h / 2;
        pVerticesTransf[i].coords.z = (DEPTH_RANGE_MAX - DEPTH_RANGE_MIN) / 2 * pVerticesTransf[i].coords.z + (DEPTH_RANGE_MAX + DEPTH_RANGE_MIN) / 2;
    }

    // TODO: face culling (here or sometime earlier in view space)
}


/**
    This should be called for every renderable object.
    Renderer cannot access private parts of objects.
    So objects have to provide interface for their input and output data.
    Input data in this case is the object's base vertices / triangle list selected by LOD.
    Output data are the vertices / triangle list after vertex processing and primitive assembly.
    Input data -> VS -> VP -> PA -> Output data.
*/
void PureRendererSWincrementalImpl::GeometricStage(
    const TXYZ* pVertices,
    TPureUInt nVertices_h,
    bool bIndexed,
    const void* pVertexIndices,
    TPureUInt nVertexIndices_h,
    unsigned int nIndicesType,
    TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
    const TXYZ& pos,
    const TXYZ& rot,
    const TXYZ& scaling,
    const TXYZ& campos,
    const TXYZ& camrot,
    TPureInt vp_x,
    TPureInt vp_y,
    TPureUInt vp_w,
    TPureUInt vp_h
)
{
    VertexProcessing(pVertices, nVertices_h, bIndexed, pVertexIndices, nVertexIndices_h, nIndicesType, pVerticesTransf, pos, rot, scaling, campos, camrot);
    PrimitiveAssembly(pVerticesTransf, nVertexIndices_h, pos, rot, campos, camrot, vp_x, vp_y, vp_w, vp_h);
}


void PureRendererSWincrementalImpl::SetPixel(
    int x,
    int y,
    TRGBAUBYTE color,
    TPureInt vp_x,
    TPureInt vp_y,
    TPureUInt vp_w,
    TPureUInt vp_h
)
{
	if ( (y > -1) && (y < (TPureInt) vp_h) && (x > -1) && (x < (TPureInt) vp_w) )
	{
		pColorBufferDIB[(y*vp_w+x)*3]     = (BYTE) color.blue;
		pColorBufferDIB[(y*vp_w+x)*3 + 1] = (BYTE) color.green;
		pColorBufferDIB[(y*vp_w+x)*3 + 2] = (BYTE) color.red;
	}
}


void PureRendererSWincrementalImpl::Line(
    int x1, int y1,
    int x2, int y2,
    TRGBAUBYTE color,
    TPureInt vp_x, TPureInt vp_y,
    TPureUInt vp_w, TPureUInt vp_h
)
{
    bool candraw = PUREScissor::DoScissor(x1, y1, x2, y2, vp_x, vp_w-1, vp_y, vp_h-1);

	if ( candraw )
	{
		if ( x1 == x2 )
		{ // vertical line
			if ( y2 < y1 )
			{
				for (int yi = y2; yi <= y1; yi++)
					SetPixel(x1, yi, color, vp_x, vp_y, vp_w, vp_h);
			}
			else
			{
				for (int yi = y1; yi <= y2; yi++)
					SetPixel(x1, yi, color, vp_x, vp_y, vp_w, vp_h);
			}
		}
		else
		{
			int db;
			float x,y, lx,ly;
			if ( abs(x2-x1) > abs(y2-y1) )
			{
				db = abs( x2 - x1 )+1;
				if ( x2 < x1 ) lx = -1; else lx = 1;
				ly = (float)(y2-y1) / (float)(abs(x2-x1));
			}
			else
			{
				db = abs( y2 - y1 )+1;
				lx = (float)(x2-x1) / (float)(abs(y2-y1));
				ly = (float)db / (float)(y2-y1);
			}
			x = (float) x1;
			y = (float) y1;
			for (int i = 1; i <= db; i++)
			{
				SetPixel(PFL::roundi(x), PFL::roundi(y), color, vp_x, vp_y, vp_w, vp_h);
				x += lx;
				y += ly;
			}
		}
	} // candraw
}


/**
    This should be called for every renderable object.
    Renderer cannot access private parts of objects.
    So objects have to provide interface for their input and output data.
    Input data in this case are the fragments after GeometricStage.
    Output data in this case is null, since we render the output to framebuffer here.
    Input data -> TS -> FP -> PSP -> Framebuffer.
*/
void PureRendererSWincrementalImpl::RenderStage(
    TPureUInt nVertexIndices_h,
    TPURE_TRANSFORMED_VERTEX* pVerticesTransf,
    TPureInt vp_x,
    TPureInt vp_y,
    TPureUInt vp_w,
    TPureUInt vp_h
)
{
    //TriangleSetup();
    // not sure about how to implement this part, as there are overlaying tasks here
    //FragmentProcessing();
    //PerSampleProcessing();

    TRGBAUBYTE color;
    color.red = color.green = color.blue = 255;

    /*    
    for (TPureUInt i = 0; i < nVertexIndices_h; i++)
    {
        if ( pVerticesTransf[i].draw )
        {
            SetPixel(
                pVerticesTransf[i].coords.x, pVerticesTransf[i].coords.y,
                color,
                vp_x, vp_y,
                vp_w, vp_h
            );
        }
    }  */
    
    for (TPureUInt i = 0; i < (nVertexIndices_h / 3); i++)
    {
        const TPureUInt vistart = i*3;
        // draw triangle only if all 3 vertices are inside the frustum
        const TPureBool bDrawTriangle = pVerticesTransf[vistart].draw && pVerticesTransf[vistart+1].draw && pVerticesTransf[vistart+2].draw;
        // draw triangle if at least 1 vertex is inside the frustum
        //const TPureBool bDrawTriangle = pVerticesTransf[vistart].draw || pVerticesTransf[vistart+1].draw || pVerticesTransf[vistart+2].draw;
        if ( bDrawTriangle )
        {
            for (TPureUInt j = 0; j < 3; j++)
            {
                const TPureUInt vi1 = vistart + j;
                const TPureUInt vi2 = ( (j == 2) ? vistart : vi1+1 );
                Line(
                    (int) pVerticesTransf[vi1].coords.x, (int) pVerticesTransf[vi1].coords.y,
                    (int) pVerticesTransf[vi2].coords.x, (int) pVerticesTransf[vi2].coords.y,
                    color,
                    vp_x, vp_y,
                    vp_w, vp_h);
            }
        }
    }


}



void PureRendererSWincrementalImpl::ClearColorBuffer()
{
	DWORD size = biColorBufferDDB.bmiHeader.biSizeImage;
    if ( (pCamera->getBackgroundColor().getBlue() == pCamera->getBackgroundColor().getGreen()) &&
         (pCamera->getBackgroundColor().getGreen() == pCamera->getBackgroundColor().getRed())
       )
	{
		memset((void*)(pColorBufferDIB), pCamera->getBackgroundColor().getRed(), size);
	}
	else
	{
        struct 
        {
	        BYTE red;
	        BYTE green;
	        BYTE blue;
        } tmp;
        tmp.red   = pCamera->getBackgroundColor().getBlue();
        tmp.green = pCamera->getBackgroundColor().getGreen();
        tmp.blue  = pCamera->getBackgroundColor().getRed();

		for (DWORD i = 0; i < size; i += 3)
		{
			memcpy((void*)(pColorBufferDIB+i), &tmp, sizeof(tmp));
		}
	}	
}


/**
    Sets viewport size and clears buffers.
*/
void PureRendererSWincrementalImpl::BeginRendering()
{
    ClearColorBuffer();
}


/**
    Sets perspective projection.
*/
void PureRendererSWincrementalImpl::SwitchToPerspectiveProjection()
{

    
}


/**
    Sets orthographic projection.
*/
void PureRendererSWincrementalImpl::SwitchToOrtographicProjection()
{


}


/**
    Draws 3D objects.
*/
void PureRendererSWincrementalImpl::Draw3DObjects()
{
    for (int i = 0; i < pObject3DMgr->getSize(); i++)
    {

        PureObject3D* const obj = (PureObject3D*) pObject3DMgr->getAttachedAt(i);
        if ( obj == PGENULL )
            continue;

        if ( obj->isRenderingAllowed() && !obj->isStickedToScreen() )
        {
            
            TXYZ pos, rot, camPos, camRot, scaling;

            pos.x = obj->getPosVec().getX();
            pos.y = obj->getPosVec().getY();
            pos.z = obj->getPosVec().getZ();
            rot.x = obj->getAngleVec().getX();
            rot.y = obj->getAngleVec().getY();
            rot.z = obj->getAngleVec().getZ();
            scaling.x = obj->getScaling().getX();
            scaling.y = obj->getScaling().getY();
            scaling.z = obj->getScaling().getZ();

            camPos.x = pCamera->getPosVec().getX();
            camPos.y = pCamera->getPosVec().getY();
            camPos.z = pCamera->getPosVec().getZ();

            /* HACK: this is NOT cam rotation, this definitely to be changed to real rotation */
            camRot.x = pCamera->getTargetVec().getX();
            camRot.y = pCamera->getTargetVec().getY();
            camRot.z = pCamera->getTargetVec().getZ();
            
            // take care of attached objects (subobjects)
            // note that either we have our own subobjects, OR we are just a cloned object and we need to draw original object's subobjects
            PureObject3D* pWhichParent = obj->getReferredObject() ? obj->getReferredObject() : obj;

            for (TPureInt j = 0; j < pWhichParent->getCount(); j++)
            {

                PureObject3D* const subobj = (PureObject3D*) pWhichParent->getAttachedAt(j);
                if ( subobj == PGENULL )
                    continue;

                if ( !subobj->isRenderingAllowed() )
                    continue;

                GeometricStage(
                    subobj->getVertices(),
                    subobj->getVerticesCount(),
                    PureVertexTransfer::isVertexReferencingIndexed( subobj->getVertexTransferMode() ),
                    subobj->getVertexIndices(),
                    subobj->getVertexIndicesCount(),
                    subobj->getVertexIndicesType(),
                    subobj->getTransformedVertices(),
                    pos,
                    rot,
                    scaling,
                    camPos,
                    camRot,
                    (TPureInt) pCamera->getViewport().pos.x,
                    (TPureInt) pCamera->getViewport().pos.y,
                    (TPureUInt) pCamera->getViewport().size.width,
                    (TPureUInt) pCamera->getViewport().size.height
                );
            
                RenderStage(
                    subobj->getVertexIndicesCount(),
                    subobj->getTransformedVertices(),
                    (TPureInt) pCamera->getViewport().pos.x,
                    (TPureInt) pCamera->getViewport().pos.y,
                    (TPureUInt) pCamera->getViewport().size.width,
                    (TPureUInt) pCamera->getViewport().size.height
                );

            }

        }

    } // for i
}


/**
    Draws 2D objects.
*/
void PureRendererSWincrementalImpl::Draw2DObjects()
{

}

void PureRendererSWincrementalImpl::SyncToScreen()
{
    HGDIOBJ prevgdiobject = SelectObject(hColorBufferDC, hColorBufferDDB);
	SetDIBits(wnd.getWndDC(), hColorBufferDDB, 0, biColorBufferDDB.bmiHeader.biHeight, pColorBufferDIB, &biColorBufferDDB, DIB_RGB_COLORS);
	BitBlt(wnd.getWndDC(), 0, 0, biColorBufferDDB.bmiHeader.biWidth, biColorBufferDDB.bmiHeader.biHeight, hColorBufferDC, 0, 0, SRCCOPY);
	SelectObject(hColorBufferDC, prevgdiobject); 
}


/**
    Forces pending tasks to be finished and displays the rendered picture.
*/
void PureRendererSWincrementalImpl::FinishRendering()
{
    SyncToScreen();
}


/*
   PureRendererSWincremental
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PureRendererSWincremental& PureRendererSWincremental::createAndGet(
    PGEcfgProfiles& cfgProfiles,
    PureWindow& _wnd,
    PureScreen& _scr,
    PureHwInfo& _hwinfo )
{
    static PureRendererSWincrementalImpl inst(cfgProfiles, _wnd, _scr, _hwinfo);
    return inst;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureRendererSWincremental::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureRendererSWincremental::getLoggerModuleName()
{
    return "PureRendererSWincremental";
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


