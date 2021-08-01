/*
    ###################################################################################
    PRRERendererSWincremental.cpp
    This file is part of PRRE.
    Incremental SW Renderer class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PRREbaseIncludes.h"  // PCH
#include "../../include/external/Render/PRRERendererSWincremental.h"
#include "../../include/external/Render/PRREIIncrementalRenderPipeline.h"
#include "../../include/external/Math/PRREVector.h"
#include "../../include/external/Math/PRRETransformMatrix.h"
#include "../../include/external/Object3D/PRREObject3DManager.h"
#include "../../include/internal/PRREGLsnippets.h"
#include "../../include/internal/PRREpragmas.h"
#include "../../include/internal/Math/PUREScissor.h"
#include "../../../../../PFL/PFL/PFL.h"

using namespace std;


/*
   PRRERendererSWincrementalImpl
   ###########################################################################
*/


class PRRERendererSWincrementalImpl :
    public PRRERendererSWincremental,
    public PRREIIncrementalRenderPipeline
{
public:
    virtual ~PRRERendererSWincrementalImpl();       /**< Calls shutdown(). */

    TPRREuint initialize(
        TPRREuint width, TPRREuint height,
        TPRRE_DISPLAY_MODES dmode,
        TPRREuint freq, TPRREint cdepth,
        TPRREint zdepth, TPRREint stencil,
        TPRREint samples,
        HWND window = NULL);         
    TPRREbool shutdown();            
    TPRREbool isInitialized() const;    

    void SetManagers(
        PRREObject3DManager* _objmgr,
        PRREuiManager* _uimgr,
        PRRECamera* _cam );

    void RenderObject(PRREObject3D& object);

    void RenderScene();       
    
    void WriteStats() const;       
    void CheckConsistency() const;  

private:
    TPRREbool            bInited;
    PRREWindow&          wnd;          /**< Our window, where we draw to, singleton. */
    PRREhwInfo&          hwInfo;       /**< Hardware infos, singleton. */
    PRREScreen&          screen;       /**< Our screen, singleton. */
    PRREObject3DManager* pObject3DMgr; /**< ObjectManager, at least 1 instance. */
    PRRECamera*          pCamera;      /**< Camera, at least 1 instance. */
    PRREuiManager*       pUImgr;       /**< UI manager, singleton. */

    TPRREbool            bLighting;

    HDC        hColorBufferDC;         /**< Memory Device Context for Color Buffer, compatible with wnd's DC. */
	BITMAPINFO biColorBufferDDB;	   /**< Attributes of Color Buffer DIB. */
	HBITMAP    hColorBufferDDB;		   /**< Handle to Color Buffer DIB. */
    BYTE*      pColorBufferDIB;        /**< Color buffer. */
	float*     pZBuffer;               /**< Depth buffer. */

    // ---------------------------------------------------------------------------

    PRRERendererSWincrementalImpl();                /**< NULLs members only. */
    PRRERendererSWincrementalImpl(
        PRREWindow& _wnd,
        PRREScreen& _scr,
        PRREhwInfo& _hwinfo );
    PRRERendererSWincrementalImpl(const PRRERendererSWincrementalImpl&);
    PRRERendererSWincrementalImpl& operator=(const PRRERendererSWincrementalImpl&);

    void VertexProcessing(
        const TXYZ* pVertices,
        TPRREuint nVertices_h,
        bool bIndexed,
        const void* pVertexIndices,
        TPRREuint nVertexIndices_h,
        unsigned int nIndicesType,
        TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
        const TXYZ& pos,
        const TXYZ& rot,
        const TXYZ& scaling,
        const TXYZ& campos,
        const TXYZ& camrot );

    void PrimitiveAssembly(
        TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
        TPRREuint nVertexIndices_h,
        const TXYZ& pos,
        const TXYZ& rot,
        const TXYZ& campos,
        const TXYZ& camrot,
        TPRREint vp_x,
        TPRREint vp_y,
        TPRREuint vp_w,
        TPRREuint vp_h);

    void GeometricStage(
        const TXYZ* pVertices,
        TPRREuint nVertices_h,
        bool bIndexed,
        const void* pVertexIndices,
        TPRREuint nVertexIndices_h,
        unsigned int nIndicesType,
        TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
        const TXYZ& pos,
        const TXYZ& rot,
        const TXYZ& scaling,
        const TXYZ& campos,
        const TXYZ& camrot,
        TPRREint vp_x,
        TPRREint vp_y,
        TPRREuint vp_w,
        TPRREuint vp_h);
    void RasterStage();
    void SetPixel(
        int x,
        int y,
        TRGBAUBYTE color,
        TPRREint vp_x,
        TPRREint vp_y,
        TPRREuint vp_w,
        TPRREuint vp_h);
    void Line(
        int x1, int y1,
        int x2, int y2,
        TRGBAUBYTE color,
        TPRREint vp_x, TPRREint vp_y,
        TPRREuint vp_w, TPRREuint vp_h
    );
    void RenderStage(
        TPRREuint nVertexIndices_h,
        TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
        TPRREint vp_x,
        TPRREint vp_y,
        TPRREuint vp_w,
        TPRREuint vp_h);

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

    friend class PRRERendererSWincremental;

}; // PRRERendererSWincrementalImpl


// ############################### PUBLIC ################################


/**
    Calls shutdown().
*/
PRRERendererSWincrementalImpl::~PRRERendererSWincrementalImpl()
{
    shutdown();
} // ~PRRERendererSWincrementalImpl()


/**
    Documentation is at public PRRERendererSWincremental class definition.
*/
TPRREuint PRRERendererSWincrementalImpl::initialize(
    TPRREuint width, TPRREuint height,
    TPRRE_DISPLAY_MODES dmode, TPRREuint freq,
    TPRREint cdepth, TPRREint zdepth, TPRREint stencil,
    TPRREint samples,
    HWND window)
{
    TPRREint st = stencil;

    getConsole().OLn("PRRERendererSWincremental::initialize(%d, %d, dmode, %d, %d, %d, %d, %d)",
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
        screen.SetFullScreened( dmode != PRRE_WINDOWED );  // == PRRE_FULLSCREEN nem jó, mert lesz 3. FS mód is ...
        screen.SetFreq( freq );
        screen.SetColorBits( cdepth );
        screen.SetDepthBits( zdepth );
      
        getConsole().OLn("new PRREWindow...");
        if ( !wnd.initialize(screen.getResWidth(), screen.getResHeight(), dmode != PRRE_WINDOWED, "Window", window) )
        {
            getConsole().EOLnOO("ERROR: wnd.initialize() failed!");
            shutdown();
            return 1;
        }
        getConsole().SOLn("done!");

        if ( !screen.applyDisplaySettings(wnd.getWndDC(), PRRE_SCREEN_PF_NONE) )
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


TPRREbool PRRERendererSWincrementalImpl::shutdown()
{
    getConsole().OLn("PRRERendererSWincremental::shutdown()");
    
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


TPRREbool PRRERendererSWincrementalImpl::isInitialized() const
{
    return bInited;
} // isInitialized()


void PRRERendererSWincrementalImpl::SetManagers(
    PRREObject3DManager* _objmgr,
    PRREuiManager* _uimgr,
    PRRECamera* _cam )
{
    pObject3DMgr = _objmgr;
    pUImgr = _uimgr;
    pCamera = _cam;
}


void PRRERendererSWincrementalImpl::RenderObject(PRREObject3D& object)
{
    // TODO
}


void PRRERendererSWincrementalImpl::RenderScene()
{
    if ( !isInitialized() )
    {
        return;
    }

    BeginRendering();

    Draw3DObjects();

    FinishRendering();

} // RenderScene()


void PRRERendererSWincrementalImpl::WriteStats() const
{
    getConsole().OLn("PRRERendererSWincremental::WriteStats()");
    getConsole().L();
} // WriteStats()


/**
    Runs an internal consistency test, just for debug.
*/
void PRRERendererSWincrementalImpl::CheckConsistency() const
{
    getConsole().OLn("PRRERendererSWincremental::CheckConsistency()");

    getConsole().OLn("END of consistency check!");
} // CheckConsistency()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    NULLs members only.
*/                                                                         
PRRERendererSWincrementalImpl::PRRERendererSWincrementalImpl() :
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
} // PRRERendererSWincrementalImpl(...)


PRRERendererSWincrementalImpl::PRRERendererSWincrementalImpl(
    PRREWindow& _wnd,
    PRREScreen& _scr,
    PRREhwInfo& _hwinfo ) :
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
} // PRRERendererSWincrementalImpl(...)


PRRERendererSWincrementalImpl::PRRERendererSWincrementalImpl(const PRRERendererSWincrementalImpl&) :
    wnd( PRREWindow::createAndGet() ),
    hwInfo( PRREhwInfo::get() ),
    screen( PRREScreen::createAndGet() ),
    pUImgr( NULL ),
    pObject3DMgr( NULL ),
    pCamera( NULL ),
    hColorBufferDC( NULL ),
    hColorBufferDDB( NULL ),
    pColorBufferDIB( NULL ),
    pZBuffer( NULL )
{
}


PRRERendererSWincrementalImpl& PRRERendererSWincrementalImpl::operator=(const PRRERendererSWincrementalImpl&)
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
void PRRERendererSWincrementalImpl::VertexProcessing(
    const TXYZ* pVertices,
    TPRREuint nVertices_h,
    bool bIndexed,
    const void* pVertexIndices,
    TPRREuint nVertexIndices_h,
    unsigned int nIndicesType,
    TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
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

    for (TPRREuint i = 0; i < nVertexIndices_h; i++)
    {
        const TXYZ& vertex = bIndexed ? pVertices[ PRREGLsnippets::getVertexIndex(pVertexIndices, i, nIndicesType) ] : pVertices[i];
        PRREVector transfdVertex(vertex.x, vertex.y, vertex.z);

        /* Model -> World */
        PRRETransformMatrix mModelTr;
        mModelTr.SetTranslation(pos.x, pos.y, pos.z);

        PRRETransformMatrix mScalingTr;
        mScalingTr.SetScale(scaling.x, scaling.y, scaling.z);
        transfdVertex = mScalingTr * transfdVertex;
        // Note that we need to detect negative scaling factor and change front face winding order on-the-fly, see details in PRREObject3D::PRREObject3DImpl::Draw_ApplyTransformations()
        
        PRRETransformMatrix mRotation;
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
        PRRETransformMatrix mViewTr;
        mViewTr.SetLookAt(pCamera->getPosVec(), pCamera->getTargetVec(), pCamera->getUpVec());
        transfdVertex = mViewTr * transfdVertex;

        /* View/Eye -> Clip */
        /* The pyramidal frustum in eye space becomes a cube, therefore, the clipping plane equations in clip space are: X + Wc = 0, Y + Wc = 0, Z + Wc = 0, etc. */
        PRRETransformMatrix mProjTr;
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
void PRRERendererSWincrementalImpl::PrimitiveAssembly(
    TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
    TPRREuint nVertexIndices_h,
    const TXYZ& pos,
    const TXYZ& rot,
    const TXYZ& campos,
    const TXYZ& camrot,
    TPRREint vp_x,
    TPRREint vp_y,
    TPRREuint vp_w,
    TPRREuint vp_h
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

    for (TPRREuint i = 0; i < (nVertexIndices_h / 3); i++)
    {
        for (TPRREuint j = 0; j < 3; j++)
        {
            const TPRREuint vi = i*3 + j;
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
    for (TPRREuint i = 0; i < nVertexIndices_h; i++)
    {
        pVerticesTransf[i].coords.x = pVerticesTransf[i].coords.x / pVerticesTransf[i].coords.w;
        pVerticesTransf[i].coords.y = pVerticesTransf[i].coords.y / pVerticesTransf[i].coords.w;
        pVerticesTransf[i].coords.z = pVerticesTransf[i].coords.z / pVerticesTransf[i].coords.w;
    }

    const float DEPTH_RANGE_MIN = 0.0f;
    const float DEPTH_RANGE_MAX = 1.0f;

    /* viewport transf: input normalized device coordinates, output screen coordinates */
    for (TPRREuint i = 0; i < nVertexIndices_h; i++)
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
void PRRERendererSWincrementalImpl::GeometricStage(
    const TXYZ* pVertices,
    TPRREuint nVertices_h,
    bool bIndexed,
    const void* pVertexIndices,
    TPRREuint nVertexIndices_h,
    unsigned int nIndicesType,
    TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
    const TXYZ& pos,
    const TXYZ& rot,
    const TXYZ& scaling,
    const TXYZ& campos,
    const TXYZ& camrot,
    TPRREint vp_x,
    TPRREint vp_y,
    TPRREuint vp_w,
    TPRREuint vp_h
)
{
    VertexProcessing(pVertices, nVertices_h, bIndexed, pVertexIndices, nVertexIndices_h, nIndicesType, pVerticesTransf, pos, rot, scaling, campos, camrot);
    PrimitiveAssembly(pVerticesTransf, nVertexIndices_h, pos, rot, campos, camrot, vp_x, vp_y, vp_w, vp_h);
}


void PRRERendererSWincrementalImpl::SetPixel(
    int x,
    int y,
    TRGBAUBYTE color,
    TPRREint vp_x,
    TPRREint vp_y,
    TPRREuint vp_w,
    TPRREuint vp_h
)
{
	if ( (y > -1) && (y < (TPRREint) vp_h) && (x > -1) && (x < (TPRREint) vp_w) )
	{
		pColorBufferDIB[(y*vp_w+x)*3]     = (BYTE) color.blue;
		pColorBufferDIB[(y*vp_w+x)*3 + 1] = (BYTE) color.green;
		pColorBufferDIB[(y*vp_w+x)*3 + 2] = (BYTE) color.red;
	}
}


void PRRERendererSWincrementalImpl::Line(
    int x1, int y1,
    int x2, int y2,
    TRGBAUBYTE color,
    TPRREint vp_x, TPRREint vp_y,
    TPRREuint vp_w, TPRREuint vp_h
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
void PRRERendererSWincrementalImpl::RenderStage(
    TPRREuint nVertexIndices_h,
    TPRRE_TRANSFORMED_VERTEX* pVerticesTransf,
    TPRREint vp_x,
    TPRREint vp_y,
    TPRREuint vp_w,
    TPRREuint vp_h
)
{
    //TriangleSetup();
    // not sure about how to implement this part, as there are overlaying tasks here
    //FragmentProcessing();
    //PerSampleProcessing();

    TRGBAUBYTE color;
    color.red = color.green = color.blue = 255;

    /*    
    for (TPRREuint i = 0; i < nVertexIndices_h; i++)
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
    
    for (TPRREuint i = 0; i < (nVertexIndices_h / 3); i++)
    {
        const TPRREuint vistart = i*3;
        // draw triangle only if all 3 vertices are inside the frustum
        const TPRREbool bDrawTriangle = pVerticesTransf[vistart].draw && pVerticesTransf[vistart+1].draw && pVerticesTransf[vistart+2].draw;
        // draw triangle if at least 1 vertex is inside the frustum
        //const TPRREbool bDrawTriangle = pVerticesTransf[vistart].draw || pVerticesTransf[vistart+1].draw || pVerticesTransf[vistart+2].draw;
        if ( bDrawTriangle )
        {
            for (TPRREuint j = 0; j < 3; j++)
            {
                const TPRREuint vi1 = vistart + j;
                const TPRREuint vi2 = ( (j == 2) ? vistart : vi1+1 );
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



void PRRERendererSWincrementalImpl::ClearColorBuffer()
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
void PRRERendererSWincrementalImpl::BeginRendering()
{
    ClearColorBuffer();
}


/**
    Sets perspective projection.
*/
void PRRERendererSWincrementalImpl::SwitchToPerspectiveProjection()
{

    
}


/**
    Sets orthographic projection.
*/
void PRRERendererSWincrementalImpl::SwitchToOrtographicProjection()
{


}


/**
    Draws 3D objects.
*/
void PRRERendererSWincrementalImpl::Draw3DObjects()
{
    for (int i = 0; i < pObject3DMgr->getSize(); i++)
    {

        PRREObject3D* const obj = (PRREObject3D*) pObject3DMgr->getAttachedAt(i);
        if ( obj == PGENULL )
            continue;

        if ( obj->isVisible() && !obj->isStickedToScreen() )
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
            PRREObject3D* pWhichParent = obj->getReferredObject() ? obj->getReferredObject() : obj;

            for (TPRREint j = 0; j < pWhichParent->getCount(); j++)
            {

                PRREObject3D* const subobj = (PRREObject3D*) pWhichParent->getAttachedAt(j);
                if ( subobj == PGENULL )
                    continue;

                if ( !subobj->isVisible() )
                    continue;

                GeometricStage(
                    subobj->getVertices(),
                    subobj->getVerticesCount(),
                    PRREVertexTransfer::isVertexReferencingIndexed( subobj->getVertexTransferMode() ),
                    subobj->getVertexIndices(),
                    subobj->getVertexIndicesCount(),
                    subobj->getVertexIndicesType(),
                    subobj->getTransformedVertices(),
                    pos,
                    rot,
                    scaling,
                    camPos,
                    camRot,
                    (TPRREint) pCamera->getViewport().pos.x,
                    (TPRREint) pCamera->getViewport().pos.y,
                    (TPRREuint) pCamera->getViewport().size.width,
                    (TPRREuint) pCamera->getViewport().size.height
                );
            
                RenderStage(
                    subobj->getVertexIndicesCount(),
                    subobj->getTransformedVertices(),
                    (TPRREint) pCamera->getViewport().pos.x,
                    (TPRREint) pCamera->getViewport().pos.y,
                    (TPRREuint) pCamera->getViewport().size.width,
                    (TPRREuint) pCamera->getViewport().size.height
                );

            }

        }

    } // for i
}


/**
    Draws 2D objects.
*/
void PRRERendererSWincrementalImpl::Draw2DObjects()
{

}

void PRRERendererSWincrementalImpl::SyncToScreen()
{
    HGDIOBJ prevgdiobject = SelectObject(hColorBufferDC, hColorBufferDDB);
	SetDIBits(wnd.getWndDC(), hColorBufferDDB, 0, biColorBufferDDB.bmiHeader.biHeight, pColorBufferDIB, &biColorBufferDDB, DIB_RGB_COLORS);
	BitBlt(wnd.getWndDC(), 0, 0, biColorBufferDDB.bmiHeader.biWidth, biColorBufferDDB.bmiHeader.biHeight, hColorBufferDC, 0, 0, SRCCOPY);
	SelectObject(hColorBufferDC, prevgdiobject); 
}


/**
    Forces pending tasks to be finished and displays the rendered picture.
*/
void PRRERendererSWincrementalImpl::FinishRendering()
{
    SyncToScreen();
}


/*
   PRRERendererSWincremental
   ###########################################################################
*/


// ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PRRERendererSWincremental& PRRERendererSWincremental::createAndGet(
    PRREWindow& _wnd,
    PRREScreen& _scr,
    PRREhwInfo& _hwinfo )
{
    static PRRERendererSWincrementalImpl inst(_wnd, _scr, _hwinfo);
    return inst;
} // createAndGet()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PRRERendererSWincremental::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PRRERendererSWincremental::getLoggerModuleName()
{
    return "PRRERendererSWincremental";
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


