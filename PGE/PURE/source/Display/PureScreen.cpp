#pragma once

/*
    ###################################################################################
    PureScreen.h
    This file is part of PURE.
    Screen handler class.
    Made by PR00F88
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH
#include "../../include/external/Display/PureScreen.h"
#include "../../include/internal/PureGLextensionFuncs.h"
#include "../../include/internal/PurePragmas.h"
#include "../../include/internal/Display/PureSharedSettings.h"


/*
   PureScreenImpl
   ###########################################################################
*/


class PureScreenImpl :
    public PureScreen
{

public:

    static const char* getLoggerModuleName();

    // ---------------------------------------------------------------------------

    virtual ~PureScreenImpl();

    CConsole&  getConsole() const;

    TPureBool applyDisplaySettings(HDC dc, TPURE_SCREEN_PF pixelFormat = PURE_SCREEN_PF_SIMPLE);
    void ResetDisplaySettings();

    TPureBool isInitialized() const;

    TPureUInt getResWidth() const;
    TPureUInt getResHeight() const;
    void SetResolution(TPureUInt w, TPureUInt h);

    TPureBool isFullScreened() const;
    void SetFullScreened(TPureBool fs);

    TPureUInt getFreq() const;
    void SetFreq(TPureUInt f);

    TPureInt getColorBits() const;
    void SetColorBits(TPureInt c);

    TPureInt getDepthBits() const;
    void SetDepthBits(TPureInt d);

    TPureInt getStencilBits() const;
    void SetStencilBits(TPureInt s);

    TPureBool isScreensaverEnabled() const;
    void SetScreensaverEnabled(TPureBool state);

    TPureBool isMonitorPowersaveEnabled() const;
    void SetMonitorPowersaveEnabled(TPureBool state);

    TPureBool isStandbyEnabled() const;
    void SetStandbyEnabled(TPureBool state);

    TPureBool isFSAAready() const;
    TPureInt getFSAAlevel() const;
    void SetFSAAlevel(TPureInt level);

    TPureBool isVSyncEnabled() const;
    TPureBool setVSyncEnabled(TPureBool state);

private:

    // ---------------------------------------------------------------------------

    PureSharedSettings& sharedSettings;  /**< Pointer to shared settings. */

    PIXELFORMATDESCRIPTOR m_pfd;         /**< Pixelformat descriptor. */
    TPureBool bDisplaySettingsApplied;   /**< Are the set display settings applied? */
    TPureBool bFullScreen;               /**< Are we in fullscreen mode? */
    int       iSimplePixelFormat;        /**< Best pixel format found by Win32. */
    int       m_iAdvancedPixelFormat;    /**< Best pixel format found by OpenGL. */
    int       m_iAppliedPixelFormat;     /**< Currently applied pixel format, iSimple or iAdvanced. */
    TPureUInt nResX;                     /**< Horizontal screen resolution. */
    TPureUInt nResY;                     /**< Vertical screen resolution. */
    TPureUInt nRefreshRate;              /**< Screen refresh rate. */
    TPureInt  nBitsColor;                /**< Screen color depth. */
    TPureInt  nBitsDepth;                /**< ZBuffer depth (32/24/16). */
    TPureInt  nBitsStencil;              /**< Stencil buffer depth (8/0). */
    TPureBool bFSAA_ready;               /**< Is FSAA initialized? */
    GLint     iFSAA_state;               /**< Current FSAA-level (0x/1x/2x/3x/...). */
    GLint     iFSAA_req;                 /**< Requested FSAA-level. */
    TPureBool bVSyncState;               /**< Is VSync enabled? */

    // ---------------------------------------------------------------------------

    /**
        Fills up members with default values.
    */
    PureScreenImpl();
    PureScreenImpl(const PureScreenImpl&);
    PureScreenImpl& operator=(const PureScreenImpl&);


    void FindSimplePixelFormat(HDC dc);
    void FindAdvancedPixelFormat(HDC dc);
    void ReadAdvancedPixelFormatInfo(HDC dc, int iAppliedPixelFormat);
    TPureBool applyPixelFormat(HDC dc, int iPixelFormat, PIXELFORMATDESCRIPTOR* pfd);

    void ReadCurrentDisplaySettings(TPureUInt& nWidth, TPureUInt& nHeight, TPureInt& nCDepth, TPureUInt& nRefRate);

    TPureUInt findMaximumRefreshRate(TPureUInt nWidth, TPureUInt nHeight, TPureInt nCDepth);

    bool switchToFullscreenMode(TPureUInt nWidth, TPureUInt nHeight, TPureInt nCDepth, TPureUInt nRefRate);

    friend class PureScreen;

};


// ############################### PUBLIC ################################



PureScreenImpl::~PureScreenImpl()
{

} // ~PureScreenImpl()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureScreenImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureScreenImpl::getLoggerModuleName()
{
    return "PureScreen";
} // getLoggerModuleName()


TPureBool PureScreenImpl::applyDisplaySettings(HDC dc, TPURE_SCREEN_PF pixelFormat)
{
    getConsole().OLn("PureScreen::applyDisplaySettings()");

    if ( bDisplaySettingsApplied )
    {
        getConsole().OLn("  Already applied!");
        if (pixelFormat != PURE_SCREEN_PF_NONE)
        {
            FindAdvancedPixelFormat(dc);
        }

        return false;
    }

    if ( (nBitsColor == 0) || (nBitsDepth == 0) || (nResX == 0) || (nResY == 0) )
    {
        getConsole().EOLn("  ERROR: rr == %d, clr == %d, z == %d, %d x %d",
                            nRefreshRate, nBitsColor, nBitsDepth, nResX, nResY);
        return false;
    }
        
    bool canproceed = true;
        
    if ( bFullScreen )
    {
        if ( nRefreshRate == 0 )
        {   // maximum Hz
            nRefreshRate = findMaximumRefreshRate(nResX, nResY, nBitsColor);
            getConsole().OLn("  selected refresh rate = %d Hz", nRefreshRate);
        }
        canproceed = switchToFullscreenMode(nResX, nResY, nBitsColor, nRefreshRate);
    } // bFullScreen

    if ( canproceed )
    {
        getConsole().SOn(); getConsole().OLn("  can proceed"); getConsole().SOff();
        ReadCurrentDisplaySettings(nResX, nResY, nBitsColor, nRefreshRate);
        getConsole().OLn("  Current display settings: %dx%dx%d @ %d Hz",
                            nResX, nResY, nBitsColor, nRefreshRate);
        FindSimplePixelFormat(dc);
        switch (pixelFormat)
        {
        case PURE_SCREEN_PF_SIMPLE:
            bDisplaySettingsApplied = applyPixelFormat(dc, iSimplePixelFormat, &m_pfd);
            break;
        case PURE_SCREEN_PF_ADVANCED:
            bDisplaySettingsApplied = applyPixelFormat(dc, m_iAdvancedPixelFormat, &m_pfd);
            break;
        case PURE_SCREEN_PF_NONE:
            // TODO: create DIB in RAM to render to as in CSWRenderer ctor!
            bDisplaySettingsApplied = true;
            break;
        default:
            bDisplaySettingsApplied = false;
            getConsole().EOLn("ERROR: invalid pixelFormat specified!");
        }
                                                        
        bFSAA_ready = (m_iAdvancedPixelFormat == m_iAppliedPixelFormat);

        return bDisplaySettingsApplied;
    } // canproceed

    return false;

} // applyDisplaySettings()


void PureScreenImpl::ResetDisplaySettings()
{
    getConsole().OLn("PureScreen::ResetDisplaySettings()");
       
    if ( !bDisplaySettingsApplied || !bFullScreen )
    {
        getConsole().OLn("  nothing to reset.");
        getConsole().OLn("  bDisplaySettingsApplied == %b", bDisplaySettingsApplied);
        getConsole().OLn("  bFullScreen == %b", bFullScreen);
        bDisplaySettingsApplied = false;
        return;
    }

    getConsole().OI();
    LONG cdsret;
    if ((cdsret = ChangeDisplaySettings(NULL, 0)) == DISP_CHANGE_SUCCESSFUL)
    {
        getConsole().SOLn("ChangeDisplaySettings() okay.");
    }
    else
    {
        getConsole().EOLn("ChangeDisplaySettings(NULL, 0) failed: %d", cdsret);
    }
    getConsole().OO();
    bDisplaySettingsApplied = false;
} // ResetDisplaySettings()


TPureBool PureScreenImpl::isInitialized() const
{
    return bDisplaySettingsApplied;
} // isInitialized()


TPureUInt PureScreenImpl::getResWidth() const
{
    return nResX;
} // getWidth()

TPureUInt PureScreenImpl::getResHeight() const
{ 
    return nResY; 
} // getHeight()


void PureScreenImpl::SetResolution(TPureUInt w, TPureUInt h)
{
    if ( bDisplaySettingsApplied )
        return;

    if ( (w == 0) && (h == 0) )
    {
        DEVMODE dm;
        memset(&dm, 0, sizeof(DEVMODE));
        dm.dmSize = sizeof(DEVMODE);
        if ( EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) )
        {
            nResX = dm.dmPelsWidth;
            nResY = dm.dmPelsHeight;
        }
        else
        {
            nResX = 800;
            nResY = 600;
        }
    }
    else
    {
        nResX = w;
        nResY = h;
    }
} // SetWidth()


TPureBool PureScreenImpl::isFullScreened() const
{
    return bFullScreen;
} // isFullScreened()

void PureScreenImpl::SetFullScreened(TPureBool fs)
{
    if ( bDisplaySettingsApplied )
        return;
    bFullScreen = fs;
} // SetFullScreened()


TPureUInt PureScreenImpl::getFreq() const
{ 
    return nRefreshRate; 
} // getFreq()

void PureScreenImpl::SetFreq(TPureUInt f)
{
    if ( bDisplaySettingsApplied )
        return;

    nRefreshRate = f;
} // SetFreq()


TPureInt PureScreenImpl::getColorBits() const
{ 
    return nBitsColor;
} // getColorBits()

void PureScreenImpl::SetColorBits(TPureInt c)
{
    if ( bDisplaySettingsApplied )
        return;

    nBitsColor = c;
} // SetColorBits()


TPureInt PureScreenImpl::getDepthBits() const
{ 
    return nBitsDepth; 
} // getDepthBits()

void PureScreenImpl::SetDepthBits(TPureInt d)
{
    if ( bDisplaySettingsApplied )
        return;

    nBitsDepth = d;
} // SetDepthBits()


TPureInt PureScreenImpl::getStencilBits() const
{ 
    return nBitsStencil; 
} // getStencilBits()

void PureScreenImpl::SetStencilBits(TPureInt s)
{
    if ( bDisplaySettingsApplied )
        return;

    nBitsStencil = s;
} // SetStencilBits()


TPureBool PureScreenImpl::isScreensaverEnabled() const
{ 
    return sharedSettings.get(PURE_SSET_SCREENSAVER_ALLOWED);
} // isScreensaverEnabled()

void PureScreenImpl::SetScreensaverEnabled(TPureBool state) 
{ 
    sharedSettings.Set(PURE_SSET_SCREENSAVER_ALLOWED, state); 
} // SetScreensaverEnabled()


TPureBool PureScreenImpl::isMonitorPowersaveEnabled() const
{ 
    return sharedSettings.get(PURE_SSET_MONITORPOWERSAVE_ALLOWED);
} // isMonitorPowersaveEnabled()

void PureScreenImpl::SetMonitorPowersaveEnabled(TPureBool state) 
{ 
    sharedSettings.Set(PURE_SSET_MONITORPOWERSAVE_ALLOWED, state);
} // SetMonitorPowersaveEnabled()


TPureBool PureScreenImpl::isStandbyEnabled() const
{ 
    return sharedSettings.get(PURE_SSET_STANDBY_ALLOWED); 
} // isStandbyEnabled()

void PureScreenImpl::SetStandbyEnabled(TPureBool state) 
{ 
    sharedSettings.Set(PURE_SSET_STANDBY_ALLOWED, state);
} // SetStandbyEnabled()


TPureBool PureScreenImpl::isFSAAready() const
{ 
    return bFSAA_ready;
} // isFSAAready()

TPureInt PureScreenImpl::getFSAAlevel() const
{ 
    return iFSAA_state; 
} // getFSAACurrentLevel()

void PureScreenImpl::SetFSAAlevel(TPureInt level)
{ 
    if ( bDisplaySettingsApplied )
        return;

    iFSAA_req = (GLint) level;
} // getFSAACurrentLevel()


TPureBool PureScreenImpl::isVSyncEnabled() const
{
    return bVSyncState;
} // isVSyncEnabled()

TPureBool PureScreenImpl::setVSyncEnabled(TPureBool state)
{
    if ( sharedSettings.get(PURE_SSET_VSYNC_SUPPORTED) )
    {
        if ( TRUE == wglSwapIntervalEXT( state ? 1 : 0 ) )
        {
            bVSyncState = state;
            getConsole().SOLn("setVSyncEnabled(%b) OK!", state);
        }
        else
        {
            bVSyncState = false;
            getConsole().EOLn("setVSyncEnabled(%b) ERROR: wglSwapIntervalEXT() failed!", state);
        }
    }
    else
    {
        bVSyncState = false;
        if (state)
        {
            getConsole().EOLn("setVSyncEnabled(%b) ERROR: VSync is not supported!", state);
        }
    }

    return bVSyncState;
} // setVSyncEnabled()



// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


/**
    Fills up members with default values.
*/
PureScreenImpl::PureScreenImpl() :
    sharedSettings( PureSharedSettings::createAndGet() )
{
    bDisplaySettingsApplied = false;
    nResX = 0;
    nResY = 0;
    bFullScreen = false;
    nRefreshRate = 0;
    nBitsColor = 0;
    nBitsDepth = 0;
    nBitsStencil = 0;
    iFSAA_state = 0; 
    iFSAA_req = 0;
    bFSAA_ready = false;
    bVSyncState = false;
    memset(&m_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    iSimplePixelFormat = 0;
    m_iAdvancedPixelFormat = 0;
    m_iAppliedPixelFormat = 0;
} // PureScreenImpl(...)


PureScreenImpl::PureScreenImpl(const PureScreenImpl&) :
    sharedSettings( PureSharedSettings::createAndGet() )
{}   


PureScreenImpl& PureScreenImpl::operator=(const PureScreenImpl&)
{
    return *this;
}


/**
    Finds a pixel format for the given HDC based on the previously set values like color bits.
    This is the legacy way of selecting a pfd and this does not support setting up FSAA.
    Tries to lower Z-depth bits to 24 or 16 to ensure success.
    Updates iSimplePixelFormat.
*/
void PureScreenImpl::FindSimplePixelFormat(HDC dc)
{
    getConsole().OLn("  FindSimplePixelFormat() ...");
    if ( iSimplePixelFormat > 0 )
    {
        getConsole().OLn("    already found!");
        return;
    }

    memset(&m_pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    m_pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
    m_pfd.nVersion     = (WORD) 1;
    m_pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED;
    m_pfd.iPixelType   = PFD_TYPE_RGBA;        
    m_pfd.iLayerType   = PFD_MAIN_PLANE;
    m_pfd.cColorBits   = (BYTE) nBitsColor;
    m_pfd.cDepthBits   = (BYTE) nBitsDepth;
    m_pfd.cStencilBits = (BYTE) nBitsStencil;

    int iformat = ChoosePixelFormat(dc, &m_pfd);
    while ( iformat == 0 && (m_pfd.cDepthBits > 16) )
    {
        getConsole().EOLn("  WARNING: %d-bit Z-Buffer failed", m_pfd.cDepthBits);
        m_pfd.cDepthBits -= 8;
        iformat = ChoosePixelFormat(dc, &m_pfd);
    }

    if ( iformat == 0 )
    {
        getConsole().EOLn("  ERROR: not found pixel format!");
        return;
    }

    nBitsDepth = (TPureUInt) m_pfd.cDepthBits;
    iSimplePixelFormat = iformat;
    getConsole().SOLn("  > Simple pixel format collected!");
} // FindSimplePixelFormat()


/**
    Finds a pixel format for the given HDC based on the previously set values like color bits.
    Advanced means the pfd will be selected by OpenGL so this method requires OpenGL to be initialized.
    This method can be used if an FSAA-ready pfd is needed.
    Updates m_iAdvancedPixelFormat.
*/
void PureScreenImpl::FindAdvancedPixelFormat(HDC dc)
{
    getConsole().OLn("  FindAdvancedPixelFormat() ...");
    if ( m_iAdvancedPixelFormat > 0 )
    {
        getConsole().OLn("    already found!");
        return;
    }

    if ( wglChoosePixelFormatARB == PGENULL )
    {
        getConsole().EOLn("  ERROR: wglChoosePixelFormatARB() is NULL!");
        return;
    }

    int pixelformat = 0;
    UINT numformats = 0;
    float fAttributes[] = {0,0};
    int iAttributes[] =
    {
	    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
	    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	    WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
	    WGL_COLOR_BITS_ARB,     nBitsColor,
	    WGL_ALPHA_BITS_ARB,     8,
	    WGL_DEPTH_BITS_ARB,     nBitsDepth,
	    WGL_STENCIL_BITS_ARB,   nBitsStencil,
	    WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
	    WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
	    WGL_SAMPLES_ARB,        (int) iFSAA_req,
	    0, 0
    };
    int valid = wglChoosePixelFormatARB(dc, iAttributes, fAttributes, (UINT) 1, &pixelformat, &numformats);

    if ( (valid == FALSE) || (numformats < 1) || (pixelformat < 1) )
    {
        getConsole().EOLn("  ERROR: wglChoosePixelFormatARB() failed!");
        return;
    }
    m_iAdvancedPixelFormat = pixelformat;
    getConsole().SOLn("  > Advanced pixel format collected!");
} // FindAdvancedPixelFormat()


/**
    Reads advanced pixel format about the given pixel format using wglGetPixelFormatAttrib...().
*/
void PureScreenImpl::ReadAdvancedPixelFormatInfo(HDC dc, int iAppliedPixelFormat)
{
    if ( wglGetPixelFormatAttribivARB == PGENULL )
        return;

    const UINT nParams = 13;
    int iAttributes[nParams] =
    {
        WGL_ACCELERATION_ARB,
        WGL_SWAP_LAYER_BUFFERS_ARB,
        WGL_SWAP_METHOD_ARB,
        WGL_DOUBLE_BUFFER_ARB,
        WGL_COLOR_BITS_ARB,
        WGL_RED_BITS_ARB,
        WGL_GREEN_BITS_ARB,
        WGL_BLUE_BITS_ARB,
        WGL_ALPHA_BITS_ARB,
        WGL_DEPTH_BITS_ARB,
        WGL_STENCIL_BITS_ARB,
        WGL_SAMPLE_BUFFERS_ARB,
        WGL_SAMPLES_ARB
    };
    int nValues[nParams];

    if ( !wglGetPixelFormatAttribivARB(dc, iAppliedPixelFormat, 0, nParams, iAttributes, nValues) )
    {
        getConsole().EOLn("  WARNING: wglGetPixelFormatAttribivARB() not null but failed!");
        return;
    }

    getConsole().OLn("  clr: %d R%dG%dB%dA%d Z%d S%d", nValues[4], nValues[5], nValues[6], nValues[7],
                                                        nValues[8], nValues[9], nValues[10]);
    switch ( nValues[0] )
    {
    case WGL_FULL_ACCELERATION_ARB:    getConsole().OLn("  Accelerated by ICD."); break;
    case WGL_GENERIC_ACCELERATION_ARB: getConsole().EOLn("  Accelerated by MCD."); break;
    case WGL_NO_ACCELERATION_ARB:      getConsole().EOLn("  Software rasterization."); break;
    } // switch

    /*if ( nValues[1] )
        getConsole().OLn("  BackBuffer is present.");
    else
        getConsole().EOLn("  WARNING: no backbuffer!");
    */ 

    switch ( nValues[2] )
    {                    
    case WGL_SWAP_EXCHANGE_ARB:  getConsole().OLn("  Swap mode: exchange."); break;
    case WGL_SWAP_COPY_ARB:      getConsole().OLn("  Swap mode: copy."); break;
    case WGL_SWAP_UNDEFINED_ARB: getConsole().OLn("  Swap mode: copy w/ undefined."); break;
    } // switch

    if ( nValues[3] )
        getConsole().OLn("  DoubleBuffering.");
    else
        getConsole().EOLn("  WARNING: no DoubleBuffering!");

    getConsole().OLn("  Number of sample buffers: %d.", nValues[11]);
    iFSAA_state = nValues[12];
    getConsole().OLn("  FSAA is ready @ %dX mode.", iFSAA_state);
} // ReadAdvancedPixelFormatInfo()


/**
    Sets the given pixel format for the given HDC.
    Updates m_iAppliedPixelFormat.

    @return True on success, false otherwise.
*/
TPureBool PureScreenImpl::applyPixelFormat(HDC dc, int iPixelFormat, PIXELFORMATDESCRIPTOR* pfd)
{
    if ( SetPixelFormat(dc, iPixelFormat, pfd) )
    {
        getConsole().SOLn("  SetPixelFormat() passed");
        m_iAppliedPixelFormat = GetPixelFormat(dc);
        if (m_iAppliedPixelFormat != iPixelFormat)
        {
            getConsole().EOLn("  WARNING: GetPixelFormat() did not return the expected pixel format!");
        }
        DescribePixelFormat(dc, m_iAppliedPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), pfd);
        getConsole().OLn("  clr: %d R%dG%dB%dA%d Z%d S%d ver: %d",
                            pfd->cColorBits, pfd->cRedBits, pfd->cGreenBits, pfd->cBlueBits,
                            pfd->cAlphaBits, pfd->cDepthBits, pfd->cStencilBits, pfd->nVersion);
        ReadAdvancedPixelFormatInfo(dc, m_iAppliedPixelFormat);
        return true;
    } // SetPixelFormat()

    getConsole().EOLn("  ERROR: SetPixelFormat() failed!");
    return false;
} // applyPixelFormat()


/**
    Reads and stores current display resolution width, height, color depth and refresh rate.
*/
void PureScreenImpl::ReadCurrentDisplaySettings(TPureUInt& nWidth, TPureUInt& nHeight, TPureInt& nCDepth, TPureUInt& nRefRate)
{
    DEVMODE dm;
    memset(&dm, 0, sizeof(DEVMODE));
    dm.dmSize = sizeof(DEVMODE);
    if ( !EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) )
    {
        getConsole().EOLn("  ERROR: EnumDisplaySettings(...) failed!");
        return;
    }
    nWidth   = (TPureUInt) dm.dmPelsWidth;
    nHeight  = (TPureUInt) dm.dmPelsHeight;
    nCDepth  = (TPureInt)  dm.dmBitsPerPel;
    nRefRate = (TPureInt)  dm.dmDisplayFrequency;
} // ReadCurrentDisplaySettings()


/**
    Finds the maximum available refresh rate for the given display resolution width, height and color depth.
*/
TPureUInt PureScreenImpl::findMaximumRefreshRate(TPureUInt nWidth, TPureUInt nHeight, TPureInt nCDepth)
{
    TPureUInt nFoundMaxRefreshRate = 0;
    DEVMODE dm;
    memset(&dm, 0, sizeof(DEVMODE));
    dm.dmSize = sizeof(DEVMODE);
    for (DWORD i = 0; EnumDisplaySettings(NULL, i, &dm); i++)
    {
        if ( (dm.dmPelsWidth == (DWORD) nWidth) && (dm.dmPelsHeight == (DWORD) nHeight) && (dm.dmBitsPerPel == (DWORD) nCDepth) )
        {
            if ( dm.dmDisplayFrequency > (DWORD) nFoundMaxRefreshRate )
                nFoundMaxRefreshRate = (TPureUInt) dm.dmDisplayFrequency;
        }
    }
    return nFoundMaxRefreshRate;
} // FindMaximumRefreshRate()


/**
    Switches display mode to fullscreen at the given resolution width, height, color depth with the given refresh rate.

    @return True on success, false otherwise.
*/
bool PureScreenImpl::switchToFullscreenMode(TPureUInt nWidth, TPureUInt nHeight, TPureInt nCDepth, TPureUInt nRefRate)
{
    DEVMODE dm;
    memset(&dm, 0, sizeof(DEVMODE));
    dm.dmSize = sizeof(DEVMODE);
    dm.dmPelsWidth  = (DWORD) nWidth;
    dm.dmPelsHeight = (DWORD) nHeight;
    dm.dmBitsPerPel = (DWORD) nCDepth;
    dm.dmDisplayFrequency = (DWORD) nRefRate;
    dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

    return (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
} // switchToFullscreenMode()


/*
   PureScreen
   ###########################################################################
*/


 // ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
*/
PureScreen& PureScreen::createAndGet()
{
    static PureScreenImpl screenInstance;
    return (PureScreen&) screenInstance;
} // createAndGet()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureScreen::getLoggerModuleName()
{
    return PureScreenImpl::getLoggerModuleName();
} // getLoggerModuleName()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


