#pragma once

/*
    ###################################################################################
    PureWindowTest.h
    Unit test for PureWindow instance which is not yet initialized.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Display/PureWindow.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureWindowTest :
    public UnitTest
{
public:

    PureWindowTest() :
        UnitTest(__FILE__, "not inited"),
        wnd( PureWindow::createAndGet(cfgProfiles, inputHandler) ),
        inputHandler(PGEInputHandler::createAndGet(cfgProfiles))
    {
        
    }

    virtual ~PureWindowTest()
    {
        finalize();
    } // ~PureWindowTest()

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureWindowTest::testCtor);
        addSubTest("testInitialize", (PFNUNITSUBTEST) &PureWindowTest::testInitialize);
        addSubTest("testInitializeInvalid", (PFNUNITSUBTEST) &PureWindowTest::testInitializeInvalid);
        addSubTest("testDeinitialize", (PFNUNITSUBTEST) &PureWindowTest::testDeinitialize);
        addSubTest("testIsInitialized", (PFNUNITSUBTEST) &PureWindowTest::testIsInitialized);
        addSubTest("testResetDefaults", (PFNUNITSUBTEST) &PureWindowTest::testResetDefaults);
        addSubTest("testSetWidth", (PFNUNITSUBTEST) &PureWindowTest::testSetWidth);
        addSubTest("testSetHeight", (PFNUNITSUBTEST) &PureWindowTest::testSetHeight);
        addSubTest("testSetClientWidth", (PFNUNITSUBTEST) &PureWindowTest::testSetClientWidth);
        addSubTest("testSetClientHeight", (PFNUNITSUBTEST) &PureWindowTest::testSetClientHeight);
        addSubTest("testGetSideFrameWidth", (PFNUNITSUBTEST) &PureWindowTest::testGetSideFrameWidth);
        addSubTest("testGetBottomFrameHeight", (PFNUNITSUBTEST) &PureWindowTest::testGetBottomFrameHeight);
        addSubTest("testGetCaptionHeight", (PFNUNITSUBTEST) &PureWindowTest::testGetCaptionHeight);
        addSubTest("testGetTitleBarHeight", (PFNUNITSUBTEST) &PureWindowTest::testGetTitleBarHeight);
        addSubTest("testSetX", (PFNUNITSUBTEST) &PureWindowTest::testSetX);
        addSubTest("testSetY", (PFNUNITSUBTEST) &PureWindowTest::testSetY);
        addSubTest("testGetWndHandle", (PFNUNITSUBTEST) &PureWindowTest::testGetWndHandle);
        addSubTest("testGetWndDC", (PFNUNITSUBTEST) &PureWindowTest::testGetWndDC);
        addSubTest("testSetActive", (PFNUNITSUBTEST) &PureWindowTest::testSetActive);
        addSubTest("testSetTopMost", (PFNUNITSUBTEST) &PureWindowTest::testSetTopMost);
        addSubTest("testSetCaption", (PFNUNITSUBTEST) &PureWindowTest::testSetCaption);
        addSubTest("testSetResizableH", (PFNUNITSUBTEST) &PureWindowTest::testSetResizableH);
        addSubTest("testSetResizableV", (PFNUNITSUBTEST) &PureWindowTest::testSetResizableV);
        addSubTest("testSetResizable", (PFNUNITSUBTEST) &PureWindowTest::testSetResizable);
        addSubTest("testSetBorder", (PFNUNITSUBTEST) &PureWindowTest::testSetBorder);
        addSubTest("testSetSysMenu", (PFNUNITSUBTEST) &PureWindowTest::testSetSysMenu);
        addSubTest("testSetMinButton", (PFNUNITSUBTEST) &PureWindowTest::testSetMinButton);
        addSubTest("testSetMaxButton", (PFNUNITSUBTEST) &PureWindowTest::testSetMaxButton);
        addSubTest("testSetEnabled", (PFNUNITSUBTEST) &PureWindowTest::testSetEnabled);
        addSubTest("testSetWindowProc", (PFNUNITSUBTEST) &PureWindowTest::testSetWindowProc);
        addSubTest("testProcessMessages", (PFNUNITSUBTEST) &PureWindowTest::testProcessMessages);
        addSubTest("testHasCloseRequest", (PFNUNITSUBTEST) &PureWindowTest::testHasCloseRequest);
        addSubTest("testSetAutoRedrawAllOnPosChange", (PFNUNITSUBTEST) &PureWindowTest::testSetAutoRedrawAllOnPosChange);
        addSubTest("testSetAutoCleanupOnQuit", (PFNUNITSUBTEST) &PureWindowTest::testSetAutoCleanupOnQuit);
        addSubTest("testSetAutoCameraWarningState", (PFNUNITSUBTEST) &PureWindowTest::testSetAutoCameraWarningState);
        addSubTest("testShow", (PFNUNITSUBTEST) &PureWindowTest::testShow);
        addSubTest("testShowFull", (PFNUNITSUBTEST) &PureWindowTest::testShowFull);
        addSubTest("testHide", (PFNUNITSUBTEST) &PureWindowTest::testHide);
        addSubTest("testMinimize", (PFNUNITSUBTEST) &PureWindowTest::testMinimize);
        addSubTest("testMaximize", (PFNUNITSUBTEST) &PureWindowTest::testMaximize);
        addSubTest("testRestore", (PFNUNITSUBTEST) &PureWindowTest::testRestore);
        addSubTest("testClose", (PFNUNITSUBTEST) &PureWindowTest::testClose);
        addSubTest("testSetCursorVisible", (PFNUNITSUBTEST) &PureWindowTest::testSetCursorVisible);
        addSubTest("testWriteSettings", (PFNUNITSUBTEST) &PureWindowTest::testWriteSettings);

    }

    virtual bool setUp() override
    {
        return true;    
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

    PureWindowTest(const PureWindowTest&) :
        wnd( PureWindow::createAndGet(cfgProfiles, inputHandler) ),
        inputHandler(PGEInputHandler::createAndGet(cfgProfiles))
    {};         

    PureWindowTest& operator=(const PureWindowTest&)
    {
        return *this;
    };

    bool testCtor()
    {
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
            assertFalse(wnd.isCursorVisible(), "cursorvisible") &
            assertFalse(wnd.isEnabled(), "enabled") &
            assertFalse(wnd.isResizable(), "resizable") &
            assertFalse(wnd.isResizableV(), "resizableV") &
            assertFalse(wnd.isResizableH(), "resizableH") &
            assertFalse(wnd.isTopMost(), "topmost") &
            assertNull(wnd.getWindowProc(), "wndproc");
    }

    bool testInitialize()
    {
        /* Note that we do not unit test anything regarding the last parameter: window, because
           that is used for legacy purposes for projects creating a window on their own. */
        return assertTrue(wnd.initialize(500,500, false, "almafa"));
    }

    bool testInitializeInvalid()
    {
        /* Note that we do not unit test anything regarding the last parameter: window, because
           that is used for legacy purposes for projects creating a window on their own. */
        return assertFalse(wnd.initialize(0,0, false, "almafa"));
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
        return assertFalse(wnd.isInitialized());
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

    bool testSetWidth()
    {
        wnd.SetWidth(5);
        return assertEquals((TPureUInt)0, wnd.getWidth());
    }

    bool testSetHeight()
    {
        wnd.SetHeight(5);
        return assertEquals((TPureUInt)0, wnd.getHeight());
    }

    bool testSetClientWidth()
    {
        wnd.SetClientWidth(5);
        return assertEquals((TPureUInt)0, wnd.getClientWidth());
    }

    bool testSetClientHeight()
    {
        wnd.SetClientHeight(5);
        return assertEquals((TPureUInt)0, wnd.getClientHeight());
    }

    bool testGetSideFrameWidth()
    {
        return assertEquals((TPureUInt)0, wnd.getSideFrameWidth());
    }
    
    bool testGetBottomFrameHeight()
    {
        return assertEquals((TPureUInt)0, wnd.getBottomFrameHeight());
    }
    
    bool testGetCaptionHeight()
    {
        return assertEquals((TPureUInt)0, wnd.getCaptionHeight());
    }
    
    bool testGetTitleBarHeight()
    {
        return assertEquals((TPureUInt)0, wnd.getTitleBarHeight());
    }

    bool testSetX()
    {
        wnd.SetX(5);
        return assertEquals((TPureUInt)0, wnd.getX());
    }

    bool testSetY()
    {
        wnd.SetY(5);
        return assertEquals((TPureUInt)0, wnd.getY());
    }

    bool testGetWndHandle()
    {
        return assertEquals((HWND)0, wnd.getWndHandle());
    }

    bool testGetWndDC()
    {
        return assertEquals((HDC)0, wnd.getWndDC());
    }

    bool testSetActive()
    {
        wnd.SetActive();
        return assertFalse(wnd.isActive());
    }

    bool testSetTopMost()
    {
        wnd.SetTopMost(true);
        return assertFalse(wnd.isTopMost());
    }

    bool testSetCaption()
    {
        wnd.SetCaption("almafa");
        return assertEquals("", wnd.getCaption());
    }

    bool testSetResizableH()
    {
        wnd.SetResizableH(true);
        return assertFalse(wnd.isResizableH());
    }

    bool testSetResizableV()
    {
        wnd.SetResizableV(true);
        return assertFalse(wnd.isResizableV());
    }

    bool testSetResizable()
    {
        wnd.SetResizable(true);
        return assertFalse(wnd.isResizable());
    }

    bool testSetBorder()
    {
        wnd.SetBorder(true);
        return assertFalse(wnd.hasBorder());
    }

    bool testSetSysMenu()
    {
        wnd.SetSysMenu(true);
        return assertFalse(wnd.hasSysMenu());
    }

    bool testSetMinButton()
    {
        wnd.SetMinButton(true);
        return assertFalse(wnd.hasMinButton());
    }

    bool testSetMaxButton()
    {
        wnd.SetMaxButton(true);
        return assertFalse(wnd.hasMaxButton());
    }

    bool testSetEnabled()
    {
        wnd.SetEnabled(true);
        return assertFalse(wnd.isEnabled());
    }

    bool testSetWindowProc()
    {
        wnd.SetWindowProc((WNDPROC) 1);
        return assertNull(wnd.getWindowProc());
    }

    bool testProcessMessages()
    {
        wnd.ProcessMessages();
        return true;
    }

    bool testHasCloseRequest()
    {
        return assertFalse(wnd.hasCloseRequest());
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
        wnd.SetCursorVisible(true);
        return assertFalse(wnd.isCursorVisible());
    }

    bool testWriteSettings()
    {
        wnd.WriteSettings();
        return true;
    }
    
}; // class PureWindowTest
