/*
    ###################################################################################
    PureWindow.cpp
    This file is part of PURE.
    Pure window.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "PureBaseIncludes.h"  // PCH

#include "../../include/internal/GUI/imgui-1.88/imgui.h"
#include "../../include/internal/GUI/imgui-1.88/backends/imgui_impl_opengl2.h"
#include "../../include/internal/GUI/imgui-1.88/backends/imgui_impl_win32.h"

#include "../../include/external/Display/PureWindow.h"
#include "../../include/internal/PurePragmas.h"
#include "../../include/internal/Display/PureSharedSettings.h"
#include "../../../PGEInputHandler.h"

// For now, do not depend on g3t. In the future we might re-enable it.
//#include "../../../../../g3t/g3t/g3t.h"

using namespace std;

#define WM_REDRAW_REQUIRED WM_USER+0        /**< Redrawing needed, i.e. window borders changed, etc, but size remains the same. */


LRESULT __stdcall PuredefaultWindowProc(HWND hWindow, UINT wndMsg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


class PureWindowImpl :
    public PureWindow
{

public:

    static const char* getLoggerModuleName();          /**< Returns the logger module name of this class. */

    // ---------------------------------------------------------------------------

    virtual ~PureWindowImpl();

    CConsole&  getConsole() const;                    /**< Returns access to console preset with logger module name as this class. */

    TPureBool initialize(
        TPureUInt width, TPureUInt height,
        TPureBool bFullscreen,
        const char* caption,
        HWND window = NULL);                        
    void      Deinitialize();                        
    TPureBool isInitialized() const;                 
    void      ResetDefaults();                       
    TPureUInt getWidth() const;                      
    void      SetWidth(TPureUInt newwidth);          
    TPureUInt getClientWidth() const;                
    void      SetClientWidth(TPureUInt newwidth);    
    TPureUInt getHeight() const;                     
    void      SetHeight(TPureUInt newheight);        
    TPureUInt getClientHeight() const;               
    void      SetClientHeight(TPureUInt newheight);  
    TPureUInt getSideFrameWidth() const;             
    TPureUInt getBottomFrameHeight() const;          
    TPureUInt getCaptionHeight() const;              
    TPureUInt getTitleBarHeight() const;             
    
    TPureUInt getX() const;             
    void      SetX(TPureUInt newx);     
    TPureUInt getY() const;             
    void      SetY(TPureUInt newy);     
    
    HWND      getWndHandle() const;     
    HDC       getWndDC() const;         
    
    TPureBool isActive() const;             
    void      SetActive();                  
    TPureBool isTopMost() const;            
    void      SetTopMost(TPureBool state);  

    const std::string& getCaption() const;                          
    void               SetCaption(const std::string& newcaption);   
    
    TPureBool isResizableH() const;            
    void      SetResizableH(TPureBool state);  
    TPureBool isResizableV() const;            
    void      SetResizableV(TPureBool state);  
    TPureBool isResizable() const;            
    void      SetResizable(TPureBool state);  
    
    TPureBool hasBorder() const;             
    void      SetBorder(TPureBool state);   
    TPureBool hasSysMenu() const;           
    void      SetSysMenu(TPureBool state);   
    TPureBool hasMinButton() const;           
    void      SetMinButton(TPureBool state); 
    TPureBool hasMaxButton() const;         
    void      SetMaxButton(TPureBool state);  
    
    TPureBool isEnabled() const;           
    void      SetEnabled(TPureBool state); 
    
    WNDPROC   getWindowProc() const;                  
    void      SetWindowProc(const WNDPROC windowproc);  
    void      ProcessMessages();                      
    
    TPureBool hasCloseRequest() const;                     
    TPureBool isAutoRedrawAllOnPosChange() const;            
    void      SetAutoRedrawAllOnPosChange(TPureBool state);  
    TPureBool isAutoCleanupOnQuitOn() const;                
    void      SetAutoCleanupOnQuitOn(TPureBool state);      
    TPureBool getAutoCameraWarningState() const;            
    void      SetAutoCameraWarningState(TPureBool state);   
    
    void      Show();    
    void      ShowFull();   
    void      Hide();   
    void      Minimize();   
    void      Maximize();   
    void      Restore();    
    void      Close();      
    
    TPureBool isCursorVisible() const;           
    void      SetCursorVisible(TPureBool state); 
    
    void WriteSettings();

protected:

    // ---------------------------------------------------------------------------

private:

    static const TPureBool DEF_AUTOCLEANUP          = true;    /**< Default value of automatic cleanup on close request. */
    static const TPureBool DEF_AUTOCAMWARN          = true;    /**< Default value of automatic camera warning on resizing. */
    static const TPureBool DEF_REDRAWALL_WNDPOSCHNG = true;    /**< Default value of automatic redrawing of window content on position change. */

    // ---------------------------------------------------------------------------

    const PureSharedSettings& sharedSettings;  /**< Pointer to shared settings, const because we just reading it. */

    PGEcfgProfiles&  m_cfgProfiles;
    PGEInputHandler& input;         /**< Our source of input. */
    RAWINPUT         rawInput;      /**< For raw mouse input. */
    UINT             rawInputSize;  /**< Size of RAWINPUT. */

    HWND        hWindow;                 /**< Window Handle. */
    TPureBool   bExternalOwner;          /**< If true, then an existing window was specified at init, thus we neither create nor delete the window. */
    DWORD       dwStyle, dwStyleEx;      /**< Window styles. */
    WNDCLASS    clWindow;                /**< Window class. */
    TITLEBARINFOEX infoTitlebar;         /**< Extended title bar info, for Vista and newer. */
    RECT        rectWindowFull;          /**< Position and full size of the window. */
    RECT        rectWindowClient;        /**< Position and size of window client area. */
    TPureUInt   nClientWidthOrig;        /**< Original width of the window's client area. */
    TPureUInt   nClientHeightOrig;       /**< Original height of the window's client area. */
    TPureUInt   nSideFrameWidth;         /**< Side frame width. */
    TPureUInt   nBottomFrameHeight;      /**< Bottom frame height. */
    TPureUInt   nCaptionHeight;          /**< Caption area height. */
    TPureUInt   nTitleBarHeight;         /**< Title bar height. */
    TPureBool   m_bFullscreen;           /**< Is it a fullscreen window or not. */
    std::string sCaption;                /**< Window title. */
    TPureBool   enabled;                 /**< Is the window enabled or not. */
    TPureBool   bVisibleCursor;          /**< Is the mouse cursor visible or not. */
    TPureBool   bBtnMin;                 /**< Does the window have a minimize button or not. */
    TPureBool   bBtnMax;                 /**< Does the window have a maximize button or not. */
    TPureBool   bBorder;                 /**< Does the window have border or not. */
    TPureBool   bSysMenu;                /**< Does the window have a sysmenu or not. */
    TPureBool   bResizingH;              /**< Is horizontal resizing enabled or not. */
    TPureBool   bResizingV;              /**< Is vertical resizing enabled or not. */
    TPureBool   bCloseRequested;         /**< True if a close request has been captured by the window. */
    TPureBool   bActive;                 /**< Is the window active or not. */
    TPureBool   bTopMost;                /**< Is the window topmost or not. */
    TPureBool   bAutoCleanupOnQuit;      /**< If true, the window will close itself on close request. */
    TPureBool   bAutoCameraWarn;         /**< If true, the window will notify cameras about resizing. */
    TPureBool   bAutoRedrawOnPosChange;  /**< If true, the window will redraw its content on position change. */

    // ---------------------------------------------------------------------------

    explicit PureWindowImpl(
        PGEcfgProfiles& cfgProfiles,
        PGEInputHandler& inputHandler);
    PureWindowImpl(const PureWindowImpl&);
    PureWindowImpl& operator=(const PureWindowImpl&);

    void      SetPropertiesBeforeInitialize();    /**< Sets default values to properties, used by ctor and Deinitialize(). */
    TPureBool setupWindowClass(WNDCLASS& clWnd);  /**< Fills the given WNDCLASS structure and registers it. */
    TPureBool actualCreateWindow(
        TPureUInt width,
        TPureUInt height,
        TPureBool bFullscreen,
        const char* caption,
        const WNDCLASS& clWnd );                  /**< This creates the window actually. */

    /* events, called from PuredefaultWindowProc() */
    LRESULT onActivate(TPureBool isActive);
    LRESULT onSysCommand(WPARAM wParam, LPARAM lParam);
    LRESULT onClose();
    LRESULT onDestroy();
    LRESULT onInput(HRAWINPUT handle);
    LRESULT onKeyUp(TPureInt key);
    LRESULT onKeyDown(TPureInt key);
    LRESULT onMouseMove(TPureInt x, TPureInt y);
    LRESULT onMouseWheel(TPureShort nWheel);
    LRESULT onMouseUp(PGEInputMouse::MouseButton btn, WPARAM wParam, TPureInt x, TPureInt y);
    LRESULT onMouseDown(PGEInputMouse::MouseButton btn, WPARAM wParam, TPureInt x, TPureInt y);
    LRESULT onRedrawRequired();
    LRESULT onPosChanged(const LPWINDOWPOS newPos);
    //LRESULT onG3tCommand(UINT wmsg, WPARAM wParam, LPARAM lParam);

    // ---------------------------------------------------------------------------

    /* Our message handler callback function fully accesses it. */
    friend LRESULT __stdcall PuredefaultWindowProc(HWND, UINT, WPARAM, LPARAM); 

    friend class PureWindow;

};


/**
    This function is our message-handling callback function.
    Some message is handled by this instead of the default DefWindowProc().

    @return 0 if the given message has been handled by this function, otherwise the result of DefWindowProc().
*/
LRESULT __stdcall PuredefaultWindowProc(HWND hWindow, UINT wndMsg, WPARAM wParam, LPARAM lParam)
{
    PureWindowImpl* const wnd = (PureWindowImpl*) GetWindowLongA(hWindow, GWL_USERDATA);
    
    if ( wnd == NULL )
        return DefWindowProc(hWindow, wndMsg, wParam, lParam);

    if (ImGui_ImplWin32_WndProcHandler(hWindow, wndMsg, wParam, lParam))
        return true;

    switch ( wndMsg )
    {
    case WM_ACTIVATE   : return wnd->onActivate((LOWORD(wParam) == WA_ACTIVE) || (LOWORD(wParam) == WA_CLICKACTIVE));
    case WM_SYSCOMMAND : return wnd->onSysCommand(wParam, lParam);
    case WM_CLOSE      : return wnd->onClose();
    case WM_DESTROY    : return wnd->onDestroy();
    case WM_INPUT      : return wnd->onInput( (HRAWINPUT) lParam);
    case WM_KEYDOWN    : return wnd->onKeyDown(wParam);
    case WM_KEYUP      : return wnd->onKeyUp(wParam);
    case WM_MOUSEMOVE  : return wnd->onMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    case WM_MOUSEWHEEL : return wnd->onMouseWheel( HIWORD(wParam) );
    // Note: Quake3::win_wndproc.c states that Win32 may accumulate multiple events into wParam ... I havent experienced this yet though ...
    case WM_LBUTTONDOWN: return wnd->onMouseDown(PGEInputMouse::MouseButton::MBTN_LEFT, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    case WM_LBUTTONUP  : return wnd->onMouseUp(PGEInputMouse::MouseButton::MBTN_LEFT, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    case WM_MBUTTONDOWN: return wnd->onMouseDown(PGEInputMouse::MouseButton::MBTN_MIDDLE, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    case WM_MBUTTONUP  : return wnd->onMouseUp(PGEInputMouse::MouseButton::MBTN_MIDDLE, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    case WM_RBUTTONDOWN: return wnd->onMouseDown(PGEInputMouse::MouseButton::MBTN_RIGHT, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    case WM_RBUTTONUP  : return wnd->onMouseUp(PGEInputMouse::MouseButton::MBTN_RIGHT, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    case WM_REDRAW_REQUIRED:  return wnd->onRedrawRequired();
    case WM_WINDOWPOSCHANGED: return wnd->onPosChanged( (LPWINDOWPOS) lParam );
    //case WM_G3T_PING: return wnd->onG3tCommand(wndMsg, wParam, lParam);
    default: return DefWindowProc(hWindow, wndMsg, wParam, lParam);
    } // switch()

} // PuredefaultWindowProc()


// ############################### PUBLIC ################################


PureWindowImpl::~PureWindowImpl()
{
    Deinitialize();
} // ~PureWindow()


/**
    Returns access to console preset with logger module name as this class.
    Intentionally not virtual, so the getConsole() in derived class should hide this instead of overriding.

    @return Console instance used by this class.
*/
CConsole& PureWindowImpl::getConsole() const
{
    return CConsole::getConsoleInstance(getLoggerModuleName());
} // getConsole()


/**
    Returns the logger module name of this class.
    Intentionally not virtual, so derived class should hide this instead of overriding.
    Not even private, so user can also access this from outside, for any reason like controlling log filtering per logger module name.

    @return The logger module name of this class.
*/
const char* PureWindowImpl::getLoggerModuleName()
{
    return "PureWindow";
} // getLoggerModuleName()


/**
    This creates the window actually.
    Only has effect if the window is not already initialized.
    Fails if either width or height is 0.

    @param width       Width of the window to be created. Ignored if an already existing window is specified.
    @param height      Height of the window to be created. Ignored if an already existing window is specified.
    @param bFullscreen Should it be a fullscreen window? Ignored if an already existing window is specified.
    @param caption     Caption of the window. Ignored if an already existing window is specified.
    @param window      If a target window is already created for rendering purpose, it can be specified here. To be used for legacy reasons only.
                       If specified, no new window will be actually created but the given window will be treated as it was created by this class instance.
                       Properties of this window should be managed by the owner.

    @return True on success, false on fail.
*/
TPureBool PureWindowImpl::initialize(TPureUInt width, TPureUInt height, TPureBool bFullscreen, const char* caption, HWND window)
{
    if ( hWindow != NULL )
        return false;

    if ( width == 0 || height == 0 )
        return false;

    getConsole().OIOLn("PureWindow(%d, %d, %b, \"%s\", con) ...", width, height, bFullscreen, caption);
    getConsole().OI();

    SetPropertiesBeforeInitialize();
    ResetDefaults();

    if ( window == NULL )
    {
        if ( !setupWindowClass(clWindow) )
        {
            getConsole().OO();
            getConsole().OO();
            return false;
        }

        if ( !actualCreateWindow(width, height, bFullscreen, caption, clWindow) )
        {
            getConsole().OO();
            getConsole().OO();
            UnregisterClass(clWindow.lpszClassName, clWindow.hInstance);
            SetPropertiesBeforeInitialize();
            return false;
        }
    }
    else
    {
        getConsole().OLn("Skipped creating new window as an already existing window has been specified!");
        hWindow = window;
        bExternalOwner = true;
    }
    
    GetClientRect(hWindow, &rectWindowClient);
    getConsole().OLn("GetClientRect(): %d x %d",
                      rectWindowClient.right - rectWindowClient.left,
                      rectWindowClient.bottom - rectWindowClient.top);

    dwStyle = GetWindowLong(hWindow, GWL_STYLE);
    dwStyleEx = GetWindowLong(hWindow, GWL_EXSTYLE);
                    
    if ( !bExternalOwner )
    {
        SetWindowLong(hWindow, GWL_USERDATA, (LONG) this);
        SetCaption(caption);
    }

    ProcessMessages(); // let the window keep up the pace

    // triggering an onPosChanged() event to retrieve additional geometric details
    tagWINDOWPOS tmpStuff;
    tmpStuff.cx = rectWindowFull.right - rectWindowFull.left;
    tmpStuff.cy = rectWindowFull.bottom - rectWindowFull.top;
    tmpStuff.x = rectWindowFull.left;
    tmpStuff.y = rectWindowFull.top;
    onPosChanged(&tmpStuff);

    getConsole().OOSOLn("> done!");
    getConsole().OO();

    return true;
} // Initialize()


/**
    This deletes the window, then initialize() can be called again later.
*/
void PureWindowImpl::Deinitialize()
{
    getConsole().OLn("PureWindow::Deinitialize()");
    if ( hWindow == NULL )
        return;

    getConsole().OI();

    if ( !bExternalOwner )
    {
        if ( DestroyWindow( hWindow ) )
            getConsole().SOLn("DestroyWindow() OK!");
        else
            getConsole().EOLn("ERROR: DestroyWindow() failed!");

        if ( UnregisterClass(clWindow.lpszClassName, clWindow.hInstance) )
            getConsole().SOLn("UnregisterClass() OK!");
        else
            getConsole().EOLn("ERROR: UnregisterClass() failed!");
    }
    else
    {
        getConsole().OLn("Skipped destroying the window as it was already created for us at init!");
    }

    SetPropertiesBeforeInitialize();

    getConsole().OO();
} // Deinitialize()


/**
    Gets whether the window is initialized.
*/
TPureBool PureWindowImpl::isInitialized() const
{
    return ( hWindow != NULL );
} // isInitialized()


/**
    Resets the default settings.
    Equivalent to calling these functions:
     - SetAutoCameraWarningState( true )
     - SetAutoCleanupOnQuit( true )
     - SetAutoRedrawOnPosChange( true ).
*/
void PureWindowImpl::ResetDefaults()
{
    this->SetAutoCameraWarningState( DEF_AUTOCAMWARN );
    this->SetAutoCleanupOnQuitOn( DEF_AUTOCLEANUP );
    this->SetAutoRedrawAllOnPosChange( DEF_REDRAWALL_WNDPOSCHNG );
} // ResetDefaults()


/**
    Gets window width.
    @return Window width in pixels.
*/
TPureUInt PureWindowImpl::getWidth() const
{
    return rectWindowFull.right - rectWindowFull.left;
} // getWidth()


/**
    Sets window width.
    Has no effect before initialization and in fullscreen mode.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param newwidth Requested window width in pixels.
*/
void PureWindowImpl::SetWidth(TPureUInt newwidth)
{
    if ( bExternalOwner )
        return;

    if ( !m_bFullscreen && (hWindow != NULL) )
    {
        rectWindowFull.right = rectWindowFull.left + newwidth;
        SetWindowPos(hWindow, PGENULL, 0,0,
                     newwidth, rectWindowFull.bottom - rectWindowFull.top,
                     SWP_NOMOVE | SWP_NOZORDER); 
        GetClientRect(hWindow, &rectWindowClient);
        SendMessage(hWindow, WM_REDRAW_REQUIRED, 0, 0);
    }
} // SetWidth()


/**
    Gets window client width.
    This area is where we render to.

    @return Window client width in pixels.
*/
TPureUInt PureWindowImpl::getClientWidth() const
{
    return rectWindowClient.right - rectWindowClient.left;
} // getClientWidth()


/**
    Sets window client width.
    This area is where we render to.
    Has no effect before initialization and in fullscreen mode.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param newwidth Requested window client width in pixels.
*/
void PureWindowImpl::SetClientWidth(TPureUInt newwidth)
{
    if ( bExternalOwner )
        return;

    if ( !m_bFullscreen && (hWindow != NULL) )
    {
        rectWindowFull.right = rectWindowFull.left + newwidth;
        const RECT old = rectWindowFull;
        rectWindowFull.bottom = rectWindowFull.top + getClientHeight();
        AdjustWindowRectEx(&rectWindowFull, dwStyle, false, dwStyleEx);        
        rectWindowFull.top = old.top;
        rectWindowFull.bottom = old.bottom;
        const LONG diff = old.left - rectWindowFull.left;
        rectWindowFull.left = old.left;
        rectWindowFull.right += diff;
        SetWindowPos(hWindow, NULL, rectWindowFull.left, rectWindowFull.top,
                     rectWindowFull.right - rectWindowFull.left, rectWindowFull.bottom - rectWindowFull.top,
                     SWP_NOZORDER);
        SendMessage(hWindow, WM_REDRAW_REQUIRED, 0, 0);
    }
} // SetClientWidth()


/**
    Gets window height.
    @return Window height in pixels.
*/
TPureUInt PureWindowImpl::getHeight() const
{
    return rectWindowFull.bottom - rectWindowFull.top;
} // getHeight()


/**
    Sets window height.
    Has no effect before initialization and in fullscreen mode.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param newheight Requested window height in pixels.
*/
void PureWindowImpl::SetHeight(TPureUInt newheight)
{
    if ( bExternalOwner )
        return;

    if ( !m_bFullscreen && (hWindow != NULL) )
    {
        rectWindowFull.bottom = rectWindowFull.top + newheight;
        SetWindowPos(hWindow, PGENULL, 0,0, 
                     rectWindowFull.right - rectWindowFull.left, rectWindowFull.bottom - rectWindowFull.top,
                     SWP_NOMOVE | SWP_NOZORDER); 
        GetClientRect(hWindow, &rectWindowClient);
        SendMessage(hWindow, WM_REDRAW_REQUIRED, 0, 0);
    }
} // SetHeight()


/**
    Gets window client height.
    This area is where we render to.

    @return Window client height in pixels.
*/
TPureUInt PureWindowImpl::getClientHeight() const
{
    return rectWindowClient.bottom - rectWindowClient.top;
} // getClientHeight()


/**
    Sets window client width.
    This area is where we render to.
    Has no effect before initialization and in fullscreen mode.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param newheight Requested window client height in pixels.
*/
void PureWindowImpl::SetClientHeight(TPureUInt newheight)
{
    if ( bExternalOwner )
        return;

    if ( !m_bFullscreen && (hWindow != NULL) )
    {
        rectWindowFull.bottom = rectWindowFull.top + newheight;
        const RECT old = rectWindowFull;
        rectWindowFull.right = rectWindowFull.left + getClientWidth();
        AdjustWindowRectEx(&rectWindowFull, dwStyle, false, dwStyleEx);
        rectWindowFull.left = old.left;
        rectWindowFull.right = old.right;
        const LONG diff = old.top - rectWindowFull.top;
        rectWindowFull.top = old.top;
        rectWindowFull.bottom += diff;
        SetWindowPos(hWindow, NULL, rectWindowFull.left, rectWindowFull.top,
                     rectWindowFull.right - rectWindowFull.left, rectWindowFull.bottom - rectWindowFull.top,
                     SWP_NOZORDER);
        SendMessage(hWindow, WM_REDRAW_REQUIRED, 0, 0);
    }
} // SetClientHeight()


/**
    Gets window side frame width.
    The window has 2 side frames, a left and a right frame. The client are is between the 2 side frames.
    You can resize the window horizontally by dragging the side frames with the mouse if horizontal resizing is enabled.

    @return Window side frame width in pixels.
*/
TPureUInt PureWindowImpl::getSideFrameWidth() const
{
    return nSideFrameWidth;
} // getSideFrameWidth()


/**
    Gets window bottom frame height.
    The window has a bottom frame, right under the client area.
    You can resize the window vertically by dragging the bottom frame with the mouse if vertical resizing is enabled.

    @return Window bottom frame width in pixels.
*/
TPureUInt PureWindowImpl::getBottomFrameHeight() const
{
    return nBottomFrameHeight;
} // getBottomFrameHeight()


/**
    Gets window caption area height.
    The caption area is within the title bar area and its height is smaller than the title bar height.

    @return Window caption area height in pixels.
*/
TPureUInt PureWindowImpl::getCaptionHeight() const
{
    return nCaptionHeight;
} // getCaptionHeight()


/**
    Gets window title bar height.
    The title bar is the bar containing the caption and the upper frame of the window. Its height is bigger than the caption height.

    @return Window title bar height in pixels.
*/
TPureUInt PureWindowImpl::getTitleBarHeight() const
{
    return nTitleBarHeight;
} // getTitleBarHeight()


/**
    Gets window X-coordinate.
    @return X-coordinate of the window in screen pixels.
*/
TPureUInt PureWindowImpl::getX() const
{
    return rectWindowFull.left;
} // getX()


/**
    Sets window X-coordinate.
    Has no effect before initialization and in fullscreen mode.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param newx X-coordinate of window in screen pixels.
*/
void PureWindowImpl::SetX(TPureUInt newx)
{
    if ( bExternalOwner )
        return;

    if ( !m_bFullscreen && (hWindow != NULL) )
    {
        rectWindowFull.left = newx;
        rectWindowFull.right = newx + (rectWindowFull.right-rectWindowFull.left);
        SetWindowPos(hWindow, NULL, newx, rectWindowFull.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER); 
    }
} // SetX()


/**
    Gets window Y-coordinate.
    @return Y-coordinate of the window  in screen pixels.
*/
TPureUInt PureWindowImpl::getY() const
{
    return rectWindowFull.top;
} // getY()


/**
    Sets window Y-coordinate.
    Has no effect before initialization and in fullscreen mode.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param newy Y-coordinate of window in screen pixels.
*/
void PureWindowImpl::SetY(TPureUInt newy)
{
    if ( bExternalOwner )
        return;

    if ( !m_bFullscreen && (hWindow != NULL) )
    {
        rectWindowFull.top = newy;
        rectWindowFull.bottom = newy + (rectWindowFull.bottom-rectWindowFull.top);
        SetWindowPos(hWindow, NULL, rectWindowFull.left, newy, 0, 0, SWP_NOSIZE | SWP_NOZORDER); 
    }
} // SetY()


/**
    Gets window handle.
    @return Handle of window.
*/
HWND PureWindowImpl::getWndHandle() const
{
    return hWindow;
} // getWndHandle()


/**
    Gets window Device Context.
    @return Device context of window, or NULL before initialization.
*/
HDC PureWindowImpl::getWndDC() const
{
    return hWindow ? GetDC( hWindow ) : NULL;
} // getWndDC()


/**
    Gets whether window is active or not.
    If window is active, it receives keyboard input.
    @return True if window is active, false otherwise.
*/
TPureBool PureWindowImpl::isActive() const
{
    return bActive;
} // isActive()


/**
    Sets the window active.
    If window is active, it receives keyboard input.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.
*/
void PureWindowImpl::SetActive()
{
    if ( bExternalOwner )
        return;

    if ( hWindow )
        SetActiveWindow( hWindow );
} // SetActive()


/**
    Gets whether window is topmost or not.
    Topmost property does not necessarily bring the window above all other windows, but it will be above all non-topmost windows. Read MSDN documentation for details.
    @return True if window is topmost, false otherwise.
*/
TPureBool PureWindowImpl::isTopMost() const
{
    return bTopMost;
} // isTopMost()


/**
    Sets the window topmost property.
    Topmost property does not necessarily bring the window above all other windows, but it will be above all non-topmost windows. Read MSDN documentation for details.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Window topmost property to be this.
*/
void PureWindowImpl::SetTopMost(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    bTopMost = state;
    SetWindowPos(hWindow, state ? HWND_TOPMOST : HWND_NOTOPMOST,
                 0,0, 0,0, SWP_NOSIZE | SWP_NOMOVE);
} // SetTopMost()


/**
    Gets the window caption.
    @return Caption of the window.
*/
const string& PureWindowImpl::getCaption() const 
{
    return sCaption;
} // getVideocardOGLName()


/**
    Sets the window caption.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param newcaption Caption to be set for the window.
*/
void PureWindowImpl::SetCaption(const string& newcaption)
{                
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    sCaption = newcaption;
    SetWindowText(hWindow, sCaption.c_str());
} // SetCaption()


/**
    Gets whether window is resizable horizontally.
    @return True if window is resizable horizontally, false otherwise.
*/
TPureBool PureWindowImpl::isResizableH() const
{
    return bResizingH;
} // isResizableH()


/**
    Sets whether window is resizable horizontally.
    No effect before initialization. 
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Should window be resizable horizontally or not.
*/
void PureWindowImpl::SetResizableH(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    bResizingH = state;
} // SetResizableH()


/**
    Gets whether window is resizable vertically.
    @return True if window is resizable vertically, false otherwise.
*/
TPureBool PureWindowImpl::isResizableV() const
{
    return bResizingV;
} // isResizableV()


/**
    Sets whether window is resizable vertically.
    No effect before initialization. 
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Should window be resizable vertically or not.
*/
void PureWindowImpl::SetResizableV(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    bResizingV = state;
} // SetResizableV()


/**
    Gets whether window is fully resizable.
    @return True if the window is resizable both horizontally and vertically, false otherwise.
*/
TPureBool PureWindowImpl::isResizable() const
{
    return bResizingH && bResizingV;
} // isResizable()


/**
    Sets whether window is fully resizable.
    If state is true, the window will be resizable both horizontally and vertically.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Should window be resizable both horizontally and vertically or not.
*/
void PureWindowImpl::SetResizable(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    bResizingH = bResizingV = state;
    dwStyle = GetWindowLong(hWindow, GWL_STYLE);
    dwStyle = state ? (dwStyle | WS_THICKFRAME) : (dwStyle & ~WS_THICKFRAME);
    SetWindowLong(hWindow, GWL_STYLE, dwStyle);
    rectWindowFull.right = rectWindowFull.left + getClientWidth();
    rectWindowFull.bottom = rectWindowFull.top + getClientHeight();
    AdjustWindowRectEx(&rectWindowFull, dwStyle, false, dwStyleEx);
    SetWindowPos(hWindow, NULL,
                 rectWindowFull.left, rectWindowFull.top,
                 rectWindowFull.right - rectWindowFull.left,
                 rectWindowFull.bottom - rectWindowFull.top,
                 SWP_NOZORDER | SWP_FRAMECHANGED);
} // SetResizable()


/**
    Gets whether window has border or not.
*/
TPureBool PureWindowImpl::hasBorder() const
{
    return bBorder;
} // hasBorder()


/**
    Sets whether window has border or not.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Should window has border or not.
*/
void PureWindowImpl::SetBorder(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    bBorder = state;
    dwStyle = GetWindowLong(hWindow, GWL_STYLE);
    dwStyle = bBorder ? (dwStyle | WS_BORDER) : (dwStyle & ~WS_BORDER);
    SetWindowLong(hWindow, GWL_STYLE, dwStyle);
    SetWindowPos(hWindow, NULL, 0,0,0,0,
                 SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
    SendMessage(hWindow, WM_REDRAW_REQUIRED, 0, 0);
} // SetBorder()


/**
    Gets whether window has sysmenu or not.
    Sysmenu is the area where the caption of the window can be found.

    @return True if window has sysmenu, false otherwise.
*/
TPureBool PureWindowImpl::hasSysMenu() const
{
    return bSysMenu;
} // hasSysMenu()


/**
    Sets whether window has sysmenu or not.
    Sysmenu is the area where the caption of the window can be found.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Should window has sysmenu or not.
*/
void PureWindowImpl::SetSysMenu(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    bSysMenu = state;
    dwStyle = GetWindowLong(hWindow, GWL_STYLE);
    dwStyle = bSysMenu ? (dwStyle | WS_SYSMENU | WS_CAPTION) : (dwStyle & ~WS_SYSMENU);
    SetWindowLong(hWindow, GWL_STYLE, dwStyle);
    SetWindowPos(hWindow, NULL, 0,0,0,0,
                 SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
} // SetSysMenu()


/**
    Gets whether window has minimize button or not.
    This button minimizes the window to the Taskbar.

    @return True if the window has minimize button, false otherwise.
*/
TPureBool PureWindowImpl::hasMinButton() const 
{
    return bBtnMin;
} // hasMinButton()


/**
    Sets whether window has minimize button or not.
    This button minimizes the window to the Taskbar.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Should window has minimize button or not.
*/
void PureWindowImpl::SetMinButton(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    bBtnMin = state;
    dwStyle = GetWindowLong(hWindow, GWL_STYLE);
    dwStyle = state ? (dwStyle | WS_MINIMIZEBOX) : (dwStyle & ~WS_MINIMIZEBOX); 
    SetWindowLong(hWindow, GWL_STYLE, dwStyle);
    SetWindowPos(hWindow, NULL, 0,0,0,0,
                 SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
} // SetMinButton()


/**
    Gets whether window has maximize/restore button or not.
    This button maximizes the window or restores to original size.

    @return True if the window has maximize/restore button, false otherwise.
*/
TPureBool PureWindowImpl::hasMaxButton() const 
{
    return bBtnMax;
} // hasMaxButton()


/**
    Sets whether window has maximize/restore button or not.
    This button maximizes the window or restores to original size.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Should window has maximize/restore button or not.
*/
void PureWindowImpl::SetMaxButton(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    bBtnMax = state;
    dwStyle = GetWindowLong(hWindow, GWL_STYLE);
    dwStyle = state ? (dwStyle | WS_MAXIMIZEBOX) : (dwStyle & ~WS_MAXIMIZEBOX);
    SetWindowLong(hWindow, GWL_STYLE, dwStyle);
    SetWindowPos(hWindow, NULL, 0,0,0,0,
                 SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
} // SetMaxButton()


/**
    Gets whether window is enabled or not.
    If not enabled, it doesn't receive any input neither from keyboard nor mouse.

    @return True if window is enabled, false otherwise.
*/
TPureBool PureWindowImpl::isEnabled() const
{
  return enabled;
} // isEnabled()


/**
    Sets whether window is enabled or not.
    If not enabled, it doesn't receive any input neither from keyboard nor mouse.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Window should be enabled or not.
*/
void PureWindowImpl::SetEnabled(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    enabled = state;
    EnableWindow(hWindow, enabled);
} // SetEnabled()


/**
    Gets the message handling window procedure of this window.
    @return The message handling procedure of this window.
*/
WNDPROC PureWindowImpl::getWindowProc() const
{
    return clWindow.lpfnWndProc;
} // getWindowProc()


/**
    Sets the message handling window procedure of this window.
    The engine provides a message handling procedure by default, so it's not compulsory to set a custom procedure.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param windowproc A custom message handling window procedure for this window. If NULL, the engine's default message handling procedure will be set again.
*/
void PureWindowImpl::SetWindowProc(const WNDPROC windowproc)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    clWindow.lpfnWndProc = (windowproc == NULL) ? PuredefaultWindowProc : windowproc;
    SetWindowLong(hWindow, GWL_WNDPROC, (LONG) clWindow.lpfnWndProc);
} // SetWindowProc()


/**
    Processes the queued window messages.
    Calls Deinitialize() if detects a close request (WM_CLOSE) AND "automatic cleanup on close request" is enabled.
    Calling this often is recommended (i.e. once somewhere in a main loop) to make the window react to user activity without delay.
    No effect before initialization.
*/
void PureWindowImpl::ProcessMessages()
{
    if ( hWindow == NULL )
        return;

    MSG mssg;
    if ( bCloseRequested && bAutoCleanupOnQuit )
        this->Deinitialize();
    while ( PeekMessage(&mssg, hWindow, 0, 0, PM_REMOVE) != 0 )
    {
        TranslateMessage(&mssg);
        DispatchMessage(&mssg);
    };
} // ProcessMessages()


/**
    Gets whether the window has a close request or not.
    @return True if the window has detected a close request (WM_CLOSE) during message processing, false otherwise.
*/
TPureBool PureWindowImpl::hasCloseRequest() const
{
    return bCloseRequested;
} // hasCloseRequest()


/**
    Gets whether automatic redraw of window contents on position change is enabled or not.
    Enabled by default.
    @return True if automatic redraw of window contents on position change is enabled, false otherwise.
*/
TPureBool PureWindowImpl::isAutoRedrawAllOnPosChange() const
{
    return bAutoRedrawOnPosChange;
} // isAutoRedrawAllOnPosChange()


/**
    Sets whether automatic redraw of window contents on position change is enabled or not.
    If state is true, window content will be redrawed on moving or resizing the window.
    Enabled by default.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Automatic redraw of window contents to be enabled or not.
*/
void PureWindowImpl::SetAutoRedrawAllOnPosChange(TPureBool state)
{
    if ( bExternalOwner )
        return;

    bAutoRedrawOnPosChange = state;
} // SetAutoRedrawAllOnPosChange()


/**
    Gets whether automatic cleanup of window on close request is enabled or not.
    Enabled by default.
    @return True if automatic cleanup of window on close request is enabled, false otherwise.
*/
TPureBool PureWindowImpl::isAutoCleanupOnQuitOn() const
{
    return bAutoCleanupOnQuit;
} // isAutoCleanupOnQuitOn()


/**
    Sets whether automatic cleanup of window on close request is enabled or not.
    If state is true, the window will automatically destroy itself on close request in the next ProcessMessages().
    Enabled by default.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Automatic cleanup of window to be enabled or not.
*/
void PureWindowImpl::SetAutoCleanupOnQuitOn(TPureBool state)
{
    if ( bExternalOwner )
        return;

    bAutoCleanupOnQuit = state;
} // SetAutoCleanupOnQuitOn()


/**
    Gets whether automatic camera warning on resize is enabled or not.
    Enabled by default.
    @return True if automatic camera warning on resize is enabled, false otherwise.
*/
TPureBool PureWindowImpl::getAutoCameraWarningState() const
{
    return bAutoCameraWarn;
} // getAutoCameraWarningState()


/**
    Sets whether automatic camera warning on resize is enabled or not.
    If state is true, the cameras will be automatically warned about a relevant property change, i.e. window resizing so they can readjust their properties.
    Enabled by default.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Automatic camera warning to be enabled or not.
*/
void PureWindowImpl::SetAutoCameraWarningState(TPureBool state)
{
    if ( bExternalOwner )
        return;

    bAutoCameraWarn = state;
} // SetAutoCameraWarningState()


/**
    Shows the window.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.
*/
void PureWindowImpl::Show()
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    ShowWindow(hWindow, SW_SHOW);
} // Show()


/**
    Shows and brings the window to front and sets the focus to it.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.
*/
void PureWindowImpl::ShowFull()
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    ShowWindow(hWindow, SW_SHOW);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);
} // ShowFull()


/**
    Hides the window.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.
*/
void PureWindowImpl::Hide()
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    ShowWindow(hWindow, SW_HIDE);
} // Hide()


/**
    Minimizes the window, so it will be visible only on the Taskbar.
    Equivalent to clicking the minimize button in the sysmenu.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.
*/
void PureWindowImpl::Minimize()
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    ShowWindow(hWindow, SW_MINIMIZE);
} // Minimize()


/**
    Maximizes the window.
    Equivalent to clicking the maximize button in the sysmenu.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.
*/
void PureWindowImpl::Maximize()
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    ShowWindow(hWindow, SW_MAXIMIZE);
} // Maximize()


/**
    Restores the window to the previous state
    Equivalent to clicking the restore button in the sysmenu.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.
*/
void PureWindowImpl::Restore()
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    ShowWindow(hWindow, SW_RESTORE);
} // Restore()


/**
    Closes the window.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.
*/
void PureWindowImpl::Close()
{
    if ( bExternalOwner )
        return;

    SendMessage(hWindow, WM_CLOSE, 0, 0);
} // Close()


/**
    Gets whether the mouse cursor is visible or not within the window area.
    @return True if the mouse cursor is visible within the window area, false otherwise.
*/
TPureBool PureWindowImpl::isCursorVisible() const
{
    return bVisibleCursor;
} // isCursorVisible()


/**
    Sets whether the mouse cursor is visible or not within the window area.
    No effect before initialization.
    No effect if an already created window was specified for initialize(), in such case the owner of the window is responsible for setting its properties.

    @param state Mouse cursor should be visible within the window area or not.
*/
void PureWindowImpl::SetCursorVisible(TPureBool state)
{
    if ( bExternalOwner )
        return;

    if ( hWindow == NULL )
        return;

    if ( bVisibleCursor = state )
        while ( ShowCursor(TRUE) < 0 ) ;
    else
        while ( ShowCursor(FALSE) > -1 ) ;
} // SetCursorVisible()


/**
    Writes the window properties to the console window.
    No effect before initialization.
*/
void PureWindowImpl::WriteSettings()
{    
    if ( hWindow == NULL )
        return;

    getConsole().OLn("PureWindow::WriteSettings()");
    if ( bExternalOwner )
        getConsole().OLn("  Owner is external, info below may be invalid!");
    else
        getConsole().OLn("  No external owner.");
    getConsole().OLn("  left = %d, right = %d, top = %d, bottom = %d",
                      rectWindowFull.left, rectWindowFull.right,
                      rectWindowFull.top, rectWindowFull.bottom);
    getConsole().OLn("  right-left = %d, bottom-top = %d",
                      rectWindowFull.right - rectWindowFull.left,
                      rectWindowFull.bottom - rectWindowFull.top);
    getConsole().OLn("  nClientWidth = %d, nClientHeight = %d",
                      getClientWidth(), getClientHeight());
    getConsole().OLn("  caption = %s", sCaption.c_str());
    getConsole().OLn("  fs = %b, autoclup = %b", m_bFullscreen, bAutoCleanupOnQuit);
    getConsole().OLn("  minbtn = %b, maxbtn = %b", bBtnMin, bBtnMax);
    getConsole().OLn("  res_h = %b, res_v = %b", bResizingH, bResizingV);
    getConsole().OLn("  camwarn = %b, closereq = %b, bTopMost = %b",
                      bAutoCameraWarn, bCloseRequested, bTopMost);
    getConsole().OLn("");
} // WriteSettings()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################


PureWindowImpl::PureWindowImpl(
    PGEcfgProfiles& cfgProfiles,
    PGEInputHandler& inputHandler) :
    sharedSettings(PureSharedSettings::createAndGet()),
    m_cfgProfiles(cfgProfiles),
    input(inputHandler)
{
    SetPropertiesBeforeInitialize();
} // PureWindow()


PureWindowImpl::PureWindowImpl(const PureWindowImpl& other) :
    sharedSettings(PureSharedSettings::createAndGet()),
    m_cfgProfiles(other.m_cfgProfiles),
    input( other.input )
{
}


PureWindowImpl& PureWindowImpl::operator=(const PureWindowImpl&)
{
    return *this;
}


/**
    Sets default values to properties, used by ctor and Deinitialize().
*/
void PureWindowImpl::SetPropertiesBeforeInitialize()                         
{
    hWindow = NULL;
    bExternalOwner = false;
    dwStyle   = 0;
    dwStyleEx = 0;
    memset(&clWindow, 0, sizeof(clWindow));
    memset(&rectWindowFull, 0, sizeof(rectWindowFull));
    memset(&rectWindowClient, 0, sizeof(rectWindowClient));
    memset(&infoTitlebar, 0, sizeof(infoTitlebar));
    infoTitlebar.cbSize = sizeof(infoTitlebar);
    nClientWidthOrig = nClientHeightOrig = 0;
    nSideFrameWidth = nBottomFrameHeight = 0;
    nCaptionHeight = nTitleBarHeight = 0;
    m_bFullscreen = enabled = bVisibleCursor = false;
    bBtnMin = bBtnMax = bBorder = bSysMenu = false;
    bResizingH = bResizingV = false;
    bCloseRequested = bActive = bTopMost = false;
    sCaption = "";
    bAutoCleanupOnQuit     = DEF_AUTOCLEANUP;
    bAutoCameraWarn        = DEF_AUTOCAMWARN;
    bAutoRedrawOnPosChange = DEF_REDRAWALL_WNDPOSCHNG;
} // SetPropertiesBeforeInitialize()


/**
    Fills the given WNDCLASS structure and registers it.

    @param clWnd The WNDCLASS structure to be filled and registered.

    @return True on success, false on failure.
*/
TPureBool PureWindowImpl::setupWindowClass(WNDCLASS& clWnd)
{
    if ( (clWnd.hInstance = GetModuleHandle(NULL)) == NULL )
    {
        getConsole().EOLn("GetModuleHandle() failed!");
        return false;
    }
    getConsole().SOLn("GetModuleHandle() ok!");
    
    clWnd.style = CS_OWNDC | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
    clWnd.lpfnWndProc = PuredefaultWindowProc;
    clWnd.hCursor = LoadCursor(0, IDC_ARROW);
    clWnd.lpszClassName = "PureWNDCLSS";

    if ( RegisterClass(&clWnd) == 0 )
    {
        getConsole().EOLn("RegisterClass() failed!");
        return false;
    }
    getConsole().SOLn("RegisterClass() ok!");
    return true;
} // setupWindowClass()


/**
    This creates the windows actually.
    Updates the following members: sCaption, bBtnMin, bBorder, bSysMenu, bVisibleCursor, enabled, dwStyle, dwStyleEx, rectWindow,
    nClientWidthOrig, nClientHeightOrig, nClientWidth, nClientHeight, hWindow.

    @param width       Width of the window to be created.
    @param height      Height of the window to be created.
    @param bFullscreen Should it be a fullscreen window?
    @param caption     Caption of the window.
    @param clWnd       A properly filled and registered WNDCLASS. You can use setupWindowClass() to have such.

    @return True on success, false on failure.
*/
TPureBool PureWindowImpl::actualCreateWindow(TPureUInt width, TPureUInt height, TPureBool bFullscreen, const char* caption, const WNDCLASS& clWnd)
{
    sCaption = caption;
    m_bFullscreen = bFullscreen;
    bBtnMin = !bFullscreen;   
    bBorder = bSysMenu = bVisibleCursor = enabled = true;
    /*
        "WS_CLIPCHILDREN affects the drawing of the parent window.
        More specifically, it prevents it from drawing in the areas occupied by its children.
        So if you're re-drawing the parent on a regular basis and WS_CLIPCHILDREN isn't set, you'll end up stomping on the child window's display..."
        https://stackoverflow.com/questions/2399216/why-might-ws-clipchildren-be-necessary-for-the-display-of-directshow-window-on-x

        AFAIK WS_CLIPSIBLINGS is only for child windows, but specifying here doesn't cause any issues anyway so I'm using it.
    */
    dwStyle = bFullscreen ? WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS : WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    dwStyleEx = bFullscreen ? WS_EX_APPWINDOW : WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    nClientWidthOrig = width;
    nClientHeightOrig = height;
    rectWindowFull.left = bFullscreen ? 0 : GetSystemMetrics(SM_CXSCREEN) / 2 - nClientWidthOrig / 2;
    rectWindowFull.top = bFullscreen ? 0 : GetSystemMetrics(SM_CYSCREEN) / 2 - nClientHeightOrig / 2;
    rectWindowFull.right = rectWindowFull.left + nClientWidthOrig;
    rectWindowFull.bottom = rectWindowFull.top + nClientHeightOrig;
    if ( !bFullscreen )
        AdjustWindowRectEx(&rectWindowFull, dwStyle, false, dwStyleEx);

    hWindow = CreateWindowEx(dwStyleEx, clWnd.lpszClassName, caption, dwStyle,
                             rectWindowFull.left, rectWindowFull.top,
                             rectWindowFull.right - rectWindowFull.left, rectWindowFull.bottom - rectWindowFull.top,
                             HWND_DESKTOP, NULL, clWnd.hInstance, NULL);

    if ( hWindow == NULL )
    {
        getConsole().EOLn("CreateWindowEx() failed!");
        return false;
    }
    getConsole().SOLn("CreateWindowEx() ok!");

    return true;
} // createActualWindow()


LRESULT PureWindowImpl::onActivate(TPureBool isActive)
{
    bActive = isActive;
    //getConsole().OLn("PuredefaultWindowProc() isActive: %b", bActive);
    return 0;
} // onActivate()


LRESULT PureWindowImpl::onSysCommand(WPARAM wParam, LPARAM lParam)
{
    if ( ( (wParam == SC_SCREENSAVE) && (!sharedSettings.get(PURE_SSET_SCREENSAVER_ALLOWED)) ) ||
         ( (wParam == SC_MONITORPOWER) && (!sharedSettings.get(PURE_SSET_MONITORPOWERSAVE_ALLOWED)) ) )
    {   
        // If screensaver is about to start or monitor is going to be turned off, and
        // engine setting disallows this, we dont let it happen.
        return 0;
    }
    // Returning DefWindowProc() here means the subject of the message was
    // neither screensaver nor monitorpoweroff OR we let it happen.
    return DefWindowProc(hWindow, WM_SYSCOMMAND, wParam, lParam);
} // onSysCommand()


/*
    User clicks on close button
       |
    WM_CLOSE
       |
    really?
     /    \
    NO    YES
    ret(0)   DestroyWindow()
                |
             WM_DESTROY
             free up resources
                |
             PostQuitMessage(0)

    The WM_QUIT message is not associated with a window and therefore will never be
    received through a window's window procedure.
    It is retrieved only by the GetMessage() or PeekMessage() functions.
    Do not post the WM_QUIT message using PostMessage(); use PostQuitMessage().
*/
LRESULT PureWindowImpl::onClose()
{
    bCloseRequested = true;
    getConsole().OLn("PureWindow::onClose()");
    return 0;
} // onClose()


LRESULT PureWindowImpl::onDestroy()
{
    getConsole().OLn("PureWindow::onDestroy()");
    return 0;
} // onDestroy()


LRESULT PureWindowImpl::onInput(HRAWINPUT handle)
{
    rawInputSize = 40;
    GetRawInputData(handle, RID_INPUT, &rawInput, &rawInputSize, sizeof(RAWINPUTHEADER));
    
    if (rawInput.header.dwType == RIM_TYPEMOUSE) 
    {
        int xPosRelative = rawInput.data.mouse.lLastX;
        int yPosRelative = rawInput.data.mouse.lLastY;
        input.getMouse().ReceiveRelativeInput(xPosRelative, yPosRelative);
    }
    
    return 0;
} // onInput()


LRESULT PureWindowImpl::onKeyUp(TPureInt key)
{
    // For extended keys, lParam should be also checked:
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
    input.getKeyboard().SetKeyPressed((unsigned char) key, false);
    return 0;
} // onKeyUp()


LRESULT PureWindowImpl::onKeyDown(TPureInt key)
{
    // For extended keys, lParam should be also checked:
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
    input.getKeyboard().SetKeyPressed((unsigned char) key, true);
    return 0;
} // onKeyDown()


LRESULT PureWindowImpl::onMouseMove(TPureInt x, TPureInt y)
{
    const int correctedX = x + getX() + getSideFrameWidth();
    const int correctedY = y + getY() + getTitleBarHeight();

    input.getMouse().ReceiveAbsoluteInput(correctedX, correctedY);
    return 0;
} // onMouseMove()


LRESULT PureWindowImpl::onMouseWheel(TPureShort nWheel)
{   // it's IMPORTANT to be signed although a WORD will be stored in it
    input.getMouse().ReceiveWheel(nWheel);
    return 0;
} // onMouseWheel()


LRESULT PureWindowImpl::onMouseUp(PGEInputMouse::MouseButton btn, WPARAM wParam, TPureInt x, TPureInt y)
{
    input.getMouse().SetButtonPressed(btn, false);
    return 0;
} // onMouseUp()


LRESULT PureWindowImpl::onMouseDown(PGEInputMouse::MouseButton btn, WPARAM wParam, TPureInt x, TPureInt y)
{
    input.getMouse().SetButtonPressed(btn, true);
    return 0;
} // onMouseDown()


LRESULT PureWindowImpl::onRedrawRequired()
{
    getConsole().OLn("PuredefaultWindowProc() WM_REDRAW_REQUIRED");
    getConsole().OLn("");
    RECT tmpRect;
    GetClientRect(hWindow, &tmpRect);

    //pParentMgr->pParentMgr->WarnCameraManagersAboutWindowChange(nClientWidth, nClientHeight, nClientWidthOrig, nClientHeightOrig);
    if ( bAutoRedrawOnPosChange )
    {
        //pParentMgr->pParentMgr->RenderAll();
    }
    return 0;
} // onRedrawRequired()


LRESULT PureWindowImpl::onPosChanged(const LPWINDOWPOS newPos)
{
    const int newWidth = newPos->cx;
    const int newHeight = newPos->cy;
    const int newX = newPos->x;
    const int newY = newPos->y;
    /*TPureBool warncammgrs = false;
    TPureUInt prevclientwidth = 0;
    TPureUInt prevclientheight = 0;*/

    if ( !m_bFullscreen)
    {
        if ( bAutoCameraWarn )
        {
            if ( ( (rectWindowFull.right-rectWindowFull.left) != newWidth ) ||
                 ( (rectWindowFull.bottom-rectWindowFull.top) != newHeight ) )
            {
                /*if ( pParentMgr != PGENULL )
                {
                    if ( pParentMgr->pParentMgr != PGENULL )
                    {
                        pParentMgr->WriteLn("autocamerawarn");
                        prevclientwidth = nClientWidth;
                        prevclientheight = nClientHeight;
                        warncammgrs = true;
                    }
                    }*/  
            }
        } // bAutoCameraWarn
    } // !m_bFullscreen
    rectWindowFull.left = newX;
    rectWindowFull.top  = newY;
    rectWindowFull.right  = newX + newWidth;
    rectWindowFull.bottom = newY + newHeight;
    GetClientRect(hWindow, &rectWindowClient);

    SendMessage(hWindow, WM_GETTITLEBARINFOEX, 0, (LPARAM) &infoTitlebar);
    nCaptionHeight = infoTitlebar.rcTitleBar.bottom == 0 ? GetSystemMetrics(SM_CYCAPTION) : 
                                                           infoTitlebar.rcTitleBar.bottom - infoTitlebar.rcTitleBar.top; 
    
    nSideFrameWidth = (rectWindowFull.right - rectWindowFull.left - rectWindowClient.right) / 2;
    nBottomFrameHeight = nSideFrameWidth /*isResizableV() ? GetSystemMetrics(SM_CYFRAME) : GetSystemMetrics(SM_CYFIXEDFRAME)*/;
    nTitleBarHeight = rectWindowFull.bottom - rectWindowFull.top - rectWindowClient.bottom - nBottomFrameHeight; 

    //WriteSettings();
    //if ( warncammgrs )
        // pParentMgr->pParentMgr->WarnCameraManagersAboutWindowChange(nClientWidth, nClientHeight, nClientWidthOrig, nClientHeightOrig);

    //if ( bAutoRedrawOnPosChange )
        // pParentMgr->pParentMgr->RenderAll();

    return 0;
} // onPosChanged()


// commented out for now due to g3t.h include is commented out
/*
LRESULT PureWindowImpl::onG3tCommand(UINT wmsg, WPARAM wParam, LPARAM lParam)
{    
    switch ( wmsg )
    {
    case WM_G3T_PING:
        getConsole().OLn("PuredefaultWindowProc() WM_G3T_PING");
        PostMessage((HWND) wParam, WM_G3T_PONG, (WPARAM) this->hWindow, 0);
        break;
    case WM_G3T_PONG: break;
    case WM_G3T_PAUSE: break;
    case WM_G3T_RESUME: break;
    }

    getConsole().OLn("");
    return 0;
}
*/


/*
   PureWindow
   ###########################################################################
*/


 // ############################### PUBLIC ################################


/**
    Creates and gets the singleton instance.
    @return Window instance.
*/
PureWindow& PureWindow::createAndGet(
    PGEcfgProfiles& cfgProfiles,
    PGEInputHandler& inputHandler)
{
    static PureWindowImpl windowInstance(cfgProfiles, inputHandler);
    return windowInstance;
} // get()


// ############################## PROTECTED ##############################


// ############################### PRIVATE ###############################