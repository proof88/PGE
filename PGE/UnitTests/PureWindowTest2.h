#pragma once

/*
    ###################################################################################
    PureWindowTest2.h
    Unit test for PureWindow instance which is already initialized.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Display/PureWindow.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 500
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 500
#endif

class PureWindowTest2 :
    public UnitTest
{
public:

    PureWindowTest2() :
        UnitTest(__FILE__, "inited"),
        wnd( PureWindow::createAndGet(cfgProfiles, inputHandler) ),
        inputHandler(PGEInputHandler::createAndGet(cfgProfiles))
    {
        
    }

    virtual ~PureWindowTest2()
    {
        finalize();
    } // ~PureWindowTest2()

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        addSubTest("testInitialize", (PFNUNITSUBTEST) &PureWindowTest2::testInitialize);
        addSubTest("testDeinitialize", (PFNUNITSUBTEST) &PureWindowTest2::testDeinitialize);
        addSubTest("testIsInitialized", (PFNUNITSUBTEST) &PureWindowTest2::testIsInitialized);
        addSubTest("testResetDefaults", (PFNUNITSUBTEST) &PureWindowTest2::testResetDefaults);
        addSubTest("testGetWidth", (PFNUNITSUBTEST) &PureWindowTest2::testGetWidth);
        addSubTest("testGetHeight", (PFNUNITSUBTEST) &PureWindowTest2::testGetHeight);
        addSubTest("testGetClientWidth", (PFNUNITSUBTEST) &PureWindowTest2::testGetClientWidth);
        addSubTest("testGetClientHeight", (PFNUNITSUBTEST) &PureWindowTest2::testGetClientHeight);
        addSubTest("testSetWidth", (PFNUNITSUBTEST) &PureWindowTest2::testSetWidth);
        addSubTest("testSetHeight", (PFNUNITSUBTEST) &PureWindowTest2::testSetHeight);
        addSubTest("testSetClientWidth", (PFNUNITSUBTEST) &PureWindowTest2::testSetClientWidth);
        addSubTest("testSetClientHeight", (PFNUNITSUBTEST) &PureWindowTest2::testSetClientHeight);
        addSubTest("testGetSideFrameWidth", (PFNUNITSUBTEST) &PureWindowTest2::testGetSideFrameWidth);
        addSubTest("testGetBottomFrameHeight", (PFNUNITSUBTEST) &PureWindowTest2::testGetBottomFrameHeight);
        addSubTest("testGetCaptionHeight", (PFNUNITSUBTEST) &PureWindowTest2::testGetCaptionHeight);
        addSubTest("testGetTitleBarHeight", (PFNUNITSUBTEST) &PureWindowTest2::testGetTitleBarHeight);
        addSubTest("testSetX", (PFNUNITSUBTEST) &PureWindowTest2::testSetX);
        addSubTest("testSetY", (PFNUNITSUBTEST) &PureWindowTest2::testSetY);
        addSubTest("testGetWndHandle", (PFNUNITSUBTEST) &PureWindowTest2::testGetWndHandle);
        addSubTest("testGetWndDC", (PFNUNITSUBTEST) &PureWindowTest2::testGetWndDC);
        addSubTest("testSetActive", (PFNUNITSUBTEST) &PureWindowTest2::testSetActive);
        addSubTest("testSetTopMost", (PFNUNITSUBTEST) &PureWindowTest2::testSetTopMost);
        addSubTest("testSetCaption", (PFNUNITSUBTEST) &PureWindowTest2::testSetCaption);
        addSubTest("testSetResizableH", (PFNUNITSUBTEST) &PureWindowTest2::testSetResizableH);
        addSubTest("testSetResizableV", (PFNUNITSUBTEST) &PureWindowTest2::testSetResizableV);
        addSubTest("testSetResizable", (PFNUNITSUBTEST) &PureWindowTest2::testSetResizable);
        addSubTest("testSetBorder", (PFNUNITSUBTEST) &PureWindowTest2::testSetBorder);
        addSubTest("testSetSysMenu", (PFNUNITSUBTEST) &PureWindowTest2::testSetSysMenu);
        addSubTest("testSetMinButton", (PFNUNITSUBTEST) &PureWindowTest2::testSetMinButton);
        addSubTest("testSetMaxButton", (PFNUNITSUBTEST) &PureWindowTest2::testSetMaxButton);
        addSubTest("testSetEnabled", (PFNUNITSUBTEST) &PureWindowTest2::testSetEnabled);
        addSubTest("testSetWindowProc", (PFNUNITSUBTEST) &PureWindowTest2::testSetWindowProc);
        addSubTest("testProcessMessages", (PFNUNITSUBTEST) &PureWindowTest2::testProcessMessages);
        addSubTest("testHasCloseRequest", (PFNUNITSUBTEST) &PureWindowTest2::testHasCloseRequest);
        addSubTest("testSetAutoRedrawAllOnPosChange", (PFNUNITSUBTEST) &PureWindowTest2::testSetAutoRedrawAllOnPosChange);
        addSubTest("testSetAutoCleanupOnQuit", (PFNUNITSUBTEST) &PureWindowTest2::testSetAutoCleanupOnQuit);
        addSubTest("testSetAutoCameraWarningState", (PFNUNITSUBTEST) &PureWindowTest2::testSetAutoCameraWarningState);
        addSubTest("testShow", (PFNUNITSUBTEST) &PureWindowTest2::testShow);
        addSubTest("testShowFull", (PFNUNITSUBTEST) &PureWindowTest2::testShowFull);
        addSubTest("testHide", (PFNUNITSUBTEST) &PureWindowTest2::testHide);
        addSubTest("testMinimize", (PFNUNITSUBTEST) &PureWindowTest2::testMinimize);
        addSubTest("testMaximize", (PFNUNITSUBTEST) &PureWindowTest2::testMaximize);
        addSubTest("testRestore", (PFNUNITSUBTEST) &PureWindowTest2::testRestore);
        addSubTest("testClose", (PFNUNITSUBTEST) &PureWindowTest2::testClose);
        addSubTest("testSetCursorVisible", (PFNUNITSUBTEST) &PureWindowTest2::testSetCursorVisible);
        addSubTest("testWriteSettings", (PFNUNITSUBTEST) &PureWindowTest2::testWriteSettings);
    }

    virtual bool setUp() override
    {
        return assertTrue(wnd.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, false, "alma"), "wnd initialize");
    }

    virtual void tearDown() override
    {
        finalize();
    }

    virtual void finalize() override
    {
        wnd.Deinitialize();
        CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", false);
    }



private:
    PureWindow& wnd;
    PGEcfgProfiles cfgProfiles;
    PGEInputHandler& inputHandler;

    // ---------------------------------------------------------------------------

    PureWindowTest2(const PureWindowTest2&) :
        wnd( PureWindow::createAndGet(cfgProfiles, inputHandler) ),
        inputHandler(PGEInputHandler::createAndGet(cfgProfiles))
    {};         

    PureWindowTest2& operator=(const PureWindowTest2&)
    {
        return *this; 
    };

    bool testInitialize()
    {
        /* Note that we do not unit test anything regarding the last parameter: window, because
           that is used for legacy purposes for projects creating a window on their own. */
        return assertFalse(wnd.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, false, "almafa"));
    }

    bool testDeinitialize()
    {
        wnd.Deinitialize();

        return assertEquals((TPureUInt)0, wnd.getWidth(), "width") &
            assertEquals((TPureUInt)0, wnd.getHeight(), "height") &
            assertEquals((TPureUInt)0, wnd.getClientWidth(), "clwidth") &
            assertEquals((TPureUInt)0, wnd.getClientHeight(), "clheight") &
            assertEquals((TPureUInt)0, wnd.getSideFrameWidth(), "sideFrameWidth") &
            assertEquals((TPureUInt)0, wnd.getBottomFrameHeight(), "bottomFrameHeight") &
            assertEquals((TPureUInt)0, wnd.getCaptionHeight(), "captionHeight") &
            assertEquals((TPureUInt)0, wnd.getTitleBarHeight(), "titleBarHeight") &
            assertEquals("", wnd.getCaption(), "caption") &
            assertEquals((HDC)0, wnd.getWndDC(), "dc") &
            assertEquals((HWND)0, wnd.getWndHandle(), "handle") &
            assertEquals((TPureUInt)0, wnd.getX(), "x") &
            assertEquals((TPureUInt)0, wnd.getY(), "y") &
            assertFalse(wnd.isActive(), "isactive") &
            assertTrue(wnd.isAutoCleanupOnQuitOn(), "autocleanup") &
            assertTrue(wnd.isAutoRedrawAllOnPosChange(), "autoredrawposchng") &
            assertTrue(wnd.getAutoCameraWarningState(), "autocamwarn") &
            assertFalse(wnd.isInitialized(), "isinitialized") &
            assertFalse(wnd.isCursorVisible(), "cursorvisible") &
            assertFalse(wnd.isEnabled(), "enabled") &
            assertFalse(wnd.isResizable(), "resizable") &
            assertFalse(wnd.isResizableV(), "resizableV") &
            assertFalse(wnd.isResizableH(), "resizableH") &
            assertFalse(wnd.isTopMost(), "topmost");
    }

    bool testIsInitialized()
    {
        return assertTrue(wnd.isInitialized());
    }

    bool testResetDefaults()
    {
        wnd.SetAutoCameraWarningState(false);
        wnd.SetAutoCleanupOnQuitOn(false);
        wnd.SetAutoRedrawAllOnPosChange(false);
        wnd.ResetDefaults();

        return assertTrue(wnd.getAutoCameraWarningState(), "autocam") &
            assertTrue(wnd.isAutoCleanupOnQuitOn(), "autocleanup") &
            assertTrue(wnd.isAutoRedrawAllOnPosChange(), "autoredraw");
    }

    bool testGetWidth()
    {
        return assertLequals((TPureUInt)WINDOW_WIDTH, wnd.getWidth());
    }

    bool testGetHeight()
    {
        return assertLequals((TPureUInt)WINDOW_HEIGHT, wnd.getHeight());
    }

    bool testGetClientWidth()
    {
        return assertEquals((TPureUInt)WINDOW_WIDTH, wnd.getClientWidth());
    }

    bool testGetClientHeight()
    {
        return assertEquals((TPureUInt)WINDOW_WIDTH, wnd.getClientHeight());
    }

    bool testSetWidth()
    {
        const unsigned int newWidth = 50;
        wnd.SetWidth(newWidth);
        return assertEquals((TPureUInt)newWidth, wnd.getWidth(), "width") &
            assertGequals((TPureUInt)newWidth, wnd.getClientWidth(), "clientwidth");
    }

    bool testSetHeight()
    {
        const unsigned int newHeight = 50;
        wnd.SetHeight(newHeight);
        return assertEquals((TPureUInt)newHeight, wnd.getHeight(), "height") &
            assertGequals((TPureUInt)newHeight, wnd.getClientHeight(), "clientheight");
    }

    bool testSetClientWidth()
    {
        const unsigned int newClientWidth = 50;
        wnd.SetClientWidth(newClientWidth);
        return assertEquals((TPureUInt)newClientWidth, wnd.getClientWidth(), "clientwidth") &
            assertLequals((TPureUInt)newClientWidth, wnd.getWidth(), "width");
    }

    bool testSetClientHeight()
    {
        const unsigned int newClientHeight = 50;
        wnd.SetClientHeight(newClientHeight);
        return assertEquals((TPureUInt)newClientHeight, wnd.getClientHeight(), "clientheight") &
            assertLequals((TPureUInt)newClientHeight, wnd.getHeight(), "height");
    }

    bool testGetSideFrameWidth()
    {
        return assertEquals((wnd.getWidth() - wnd.getClientWidth()) / 2, wnd.getSideFrameWidth());
    }
    
    bool testGetBottomFrameHeight()
    {
        return assertEquals(
            wnd.getHeight() - wnd.getTitleBarHeight() - wnd.getClientHeight(),
            wnd.getBottomFrameHeight());
    }
    
    bool testGetCaptionHeight()
    {
        return assertBetween((unsigned int) 1, wnd.getTitleBarHeight(), wnd.getCaptionHeight());
    }
    
    bool testGetTitleBarHeight()
    {
        return assertEquals(
            wnd.getHeight() - wnd.getClientHeight() - wnd.getBottomFrameHeight(),
            wnd.getTitleBarHeight());
    }

    bool testSetX()
    {
        wnd.SetX(5);
        return assertEquals((TPureUInt)5, wnd.getX(), "getX") &
            assertLequals((TPureUInt)WINDOW_WIDTH, wnd.getWidth(), "width") &
            assertLequals((TPureUInt)WINDOW_HEIGHT, wnd.getHeight(), "height") &
            assertEquals((TPureUInt)WINDOW_WIDTH, wnd.getClientWidth(), "clientwidth") &
            assertEquals((TPureUInt)WINDOW_HEIGHT, wnd.getClientHeight(), "clientheight");
    }

    bool testSetY()
    {
        wnd.SetY(5);
        return assertEquals((TPureUInt)5, wnd.getY()) &
            assertLequals((TPureUInt)WINDOW_WIDTH, wnd.getWidth(), "width") &
            assertLequals((TPureUInt)WINDOW_HEIGHT, wnd.getHeight(), "height") &
            assertEquals((TPureUInt)WINDOW_WIDTH, wnd.getClientWidth(), "clientwidth") &
            assertEquals((TPureUInt)WINDOW_HEIGHT, wnd.getClientHeight(), "clientheight");
    }

    bool testGetWndHandle()
    {
        return assertNotEquals((HWND)0, wnd.getWndHandle());
    }

    bool testGetWndDC()
    {
        return assertNotEquals((HDC)0, wnd.getWndDC());
    }

    bool testSetActive()
    {
        wnd.SetActive();
        return assertTrue(wnd.isActive());
    }

    bool testSetTopMost()
    {
        wnd.SetTopMost(true);
        bool b = assertTrue(wnd.isTopMost(), "b1");

        wnd.SetTopMost(false);
        b &= assertFalse(wnd.isTopMost(), "b2");

        return b;
    }

    bool testSetCaption()
    {
        wnd.SetCaption("asdasd");
        return assertEquals("asdasd", wnd.getCaption());
    }

    bool testSetResizableH()
    {
        wnd.SetResizableH(true);
        bool b = assertTrue(wnd.isResizableH(), "b1");

        wnd.SetResizableH(false);
        b = assertFalse(wnd.isResizableH(), "b2");

        return b;
    }

    bool testSetResizableV()
    {
        wnd.SetResizableV(true);
        const bool b1 = wnd.isResizableV();

        wnd.SetResizableV(false);
        const bool b2 = wnd.isResizableV();

        return assertTrue(b1, "b1") & assertFalse(b2, "b2");
    }

    bool testSetResizable()
    {
        wnd.SetResizable(true);
        const bool b1 = wnd.isResizable();
        wnd.SetResizable(false);
        const bool b2 = wnd.isResizable();
        return assertTrue(b1, "b1") & assertFalse(b2, "b2");
    }

    bool testSetBorder()
    {
        wnd.SetBorder(true);
        const bool b1 = wnd.hasBorder();

        wnd.SetBorder(false);
        const bool b2 = wnd.hasBorder();

        return assertTrue(b1, "b1") & assertFalse(b2, "b2");
    }

    bool testSetSysMenu()
    {
        wnd.SetSysMenu(true);
        const bool b1 = wnd.hasSysMenu();

        wnd.SetSysMenu(false);
        const bool b2 = wnd.hasSysMenu();

        return assertTrue(b1, "b1") & assertFalse(b2, "b2");
    }

    bool testSetMinButton()
    {
        wnd.SetMinButton(true);
        const bool b1 = wnd.hasMinButton();

        wnd.SetMinButton(false);
        const bool b2 = wnd.hasMinButton();

        return assertTrue(b1, "b1") & assertFalse(b2, "b2");
    }

    bool testSetMaxButton()
    {
        wnd.SetMaxButton(true);
        const bool b1 = wnd.hasMaxButton();

        wnd.SetMaxButton(false);
        const bool b2 = wnd.hasMaxButton();

        return assertTrue(b1, "b1") & assertFalse(b2, "b2");
    }

    bool testSetEnabled()
    {
        wnd.SetEnabled(true);
        const bool b1 = wnd.isEnabled();

        wnd.SetEnabled(false);
        const bool b2 = wnd.isEnabled();

        return assertTrue(b1, "b1") & assertFalse(b2, "b2");
    }

    bool testSetWindowProc()
    {
        wnd.SetWindowProc(NULL);
        return assertNotNull(wnd.getWindowProc());
    }

    bool testProcessMessages()
    {
        wnd.ProcessMessages();
        return true;
    }

    bool testHasCloseRequest()
    {
        const bool b1 = wnd.hasCloseRequest();
        wnd.Close();

        const bool b2 = wnd.hasCloseRequest();

        return assertFalse(b1, "b1") & assertTrue(b2, "b2");
    }

    bool testSetAutoRedrawAllOnPosChange()
    {
        wnd.SetAutoRedrawAllOnPosChange(false);
        return assertFalse(wnd.isAutoRedrawAllOnPosChange());
    }

    bool testSetAutoCleanupOnQuit()
    {
        wnd.SetAutoCleanupOnQuitOn(false);
        return assertFalse(wnd.isAutoCleanupOnQuitOn());
    }

    bool testSetAutoCameraWarningState()
    {
        wnd.SetAutoCameraWarningState(false);
        return assertFalse(wnd.getAutoCameraWarningState());
    }

    bool testShow()
    {
        wnd.Show();
        return true;
    }

    bool testShowFull()
    {
        wnd.ShowFull();
        return true;
    }

    bool testHide()
    {
        wnd.Show();
        return true;
    }

    bool testMinimize()
    {
        wnd.Minimize();
        return true;
    }

    bool testMaximize()
    {
        wnd.Maximize();
        return true;
    }

    bool testRestore()
    {
        wnd.Restore();
        return true;
    }

    bool testClose()
    {
        wnd.Close();
        return true;
    }

    bool testSetCursorVisible()
    {
        wnd.SetCursorVisible(false);
        const bool b1 = wnd.isCursorVisible();

        wnd.SetCursorVisible(true);
        const bool b2 = wnd.isCursorVisible();

        return assertFalse(b1, "b1") & assertTrue(b2, "b2");
    }

    bool testWriteSettings()
    {
        wnd.WriteSettings();
        return true;
    }
    
}; // class PureWindowTest2
