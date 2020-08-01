#pragma once

/*
    ###################################################################################
    PRREWindowTest.h
    Unit test for PRREWindow instance which is not yet initialized.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/Display/PRREWindow.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREWindowTest :
    public UnitTest
{
public:

    PRREWindowTest() :
        UnitTest(__FILE__, "not inited"),
        wnd( PRREWindow::createAndGet() )
    {}

    ~PRREWindowTest()
    {
        Finalize();
    } // ~PRREWindowTest()

protected:

    virtual void Initialize()
    {
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREWindowTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PRREWindowTest::testInitialize);
        AddSubTest("testInitializeInvalid", (PFNUNITSUBTEST) &PRREWindowTest::testInitializeInvalid);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PRREWindowTest::testDeinitialize);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PRREWindowTest::testIsInitialized);
        AddSubTest("testResetDefaults", (PFNUNITSUBTEST) &PRREWindowTest::testResetDefaults);
        AddSubTest("testSetWidth", (PFNUNITSUBTEST) &PRREWindowTest::testSetWidth);
        AddSubTest("testSetHeight", (PFNUNITSUBTEST) &PRREWindowTest::testSetHeight);
        AddSubTest("testSetClientWidth", (PFNUNITSUBTEST) &PRREWindowTest::testSetClientWidth);
        AddSubTest("testSetClientHeight", (PFNUNITSUBTEST) &PRREWindowTest::testSetClientHeight);
        AddSubTest("testGetSideFrameWidth", (PFNUNITSUBTEST) &PRREWindowTest::testGetSideFrameWidth);
        AddSubTest("testGetBottomFrameHeight", (PFNUNITSUBTEST) &PRREWindowTest::testGetBottomFrameHeight);
        AddSubTest("testGetCaptionHeight", (PFNUNITSUBTEST) &PRREWindowTest::testGetCaptionHeight);
        AddSubTest("testGetTitleBarHeight", (PFNUNITSUBTEST) &PRREWindowTest::testGetTitleBarHeight);
        AddSubTest("testSetX", (PFNUNITSUBTEST) &PRREWindowTest::testSetX);
        AddSubTest("testSetY", (PFNUNITSUBTEST) &PRREWindowTest::testSetY);
        AddSubTest("testGetWndHandle", (PFNUNITSUBTEST) &PRREWindowTest::testGetWndHandle);
        AddSubTest("testGetWndDC", (PFNUNITSUBTEST) &PRREWindowTest::testGetWndDC);
        AddSubTest("testSetActive", (PFNUNITSUBTEST) &PRREWindowTest::testSetActive);
        AddSubTest("testSetTopMost", (PFNUNITSUBTEST) &PRREWindowTest::testSetTopMost);
        AddSubTest("testSetCaption", (PFNUNITSUBTEST) &PRREWindowTest::testSetCaption);
        AddSubTest("testSetResizableH", (PFNUNITSUBTEST) &PRREWindowTest::testSetResizableH);
        AddSubTest("testSetResizableV", (PFNUNITSUBTEST) &PRREWindowTest::testSetResizableV);
        AddSubTest("testSetResizable", (PFNUNITSUBTEST) &PRREWindowTest::testSetResizable);
        AddSubTest("testSetBorder", (PFNUNITSUBTEST) &PRREWindowTest::testSetBorder);
        AddSubTest("testSetSysMenu", (PFNUNITSUBTEST) &PRREWindowTest::testSetSysMenu);
        AddSubTest("testSetMinButton", (PFNUNITSUBTEST) &PRREWindowTest::testSetMinButton);
        AddSubTest("testSetMaxButton", (PFNUNITSUBTEST) &PRREWindowTest::testSetMaxButton);
        AddSubTest("testSetEnabled", (PFNUNITSUBTEST) &PRREWindowTest::testSetEnabled);
        AddSubTest("testSetWindowProc", (PFNUNITSUBTEST) &PRREWindowTest::testSetWindowProc);
        AddSubTest("testProcessMessages", (PFNUNITSUBTEST) &PRREWindowTest::testProcessMessages);
        AddSubTest("testHasCloseRequest", (PFNUNITSUBTEST) &PRREWindowTest::testHasCloseRequest);
        AddSubTest("testSetAutoRedrawAllOnPosChange", (PFNUNITSUBTEST) &PRREWindowTest::testSetAutoRedrawAllOnPosChange);
        AddSubTest("testSetAutoCleanupOnQuit", (PFNUNITSUBTEST) &PRREWindowTest::testSetAutoCleanupOnQuit);
        AddSubTest("testSetAutoCameraWarningState", (PFNUNITSUBTEST) &PRREWindowTest::testSetAutoCameraWarningState);
        AddSubTest("testShow", (PFNUNITSUBTEST) &PRREWindowTest::testShow);
        AddSubTest("testShowFull", (PFNUNITSUBTEST) &PRREWindowTest::testShowFull);
        AddSubTest("testHide", (PFNUNITSUBTEST) &PRREWindowTest::testHide);
        AddSubTest("testMinimize", (PFNUNITSUBTEST) &PRREWindowTest::testMinimize);
        AddSubTest("testMaximize", (PFNUNITSUBTEST) &PRREWindowTest::testMaximize);
        AddSubTest("testRestore", (PFNUNITSUBTEST) &PRREWindowTest::testRestore);
        AddSubTest("testClose", (PFNUNITSUBTEST) &PRREWindowTest::testClose);
        AddSubTest("testSetCursorVisible", (PFNUNITSUBTEST) &PRREWindowTest::testSetCursorVisible);
        AddSubTest("testWriteSettings", (PFNUNITSUBTEST) &PRREWindowTest::testWriteSettings);

    }

    virtual void SetUp()
    {
        
    }

    virtual void TearDown()
    {
        Finalize();
    }

    virtual void Finalize()
    {
        wnd.Deinitialize();
    }



private:
    PRREWindow& wnd;

    // ---------------------------------------------------------------------------

    PRREWindowTest(const PRREWindowTest&) :
        wnd( PRREWindow::createAndGet() )
    {};         

    PRREWindowTest& operator=(const PRREWindowTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertEquals((TPRREuint)0, wnd.getWidth(), "width") &
            assertEquals((TPRREuint)0, wnd.getHeight(), "height") &
            assertEquals((TPRREuint)0, wnd.getClientWidth(), "clwidth") &
            assertEquals((TPRREuint)0, wnd.getClientHeight(), "clheight") &
            assertEquals((TPRREuint)0, wnd.getSideFrameWidth(), "sideFrameWidth") &
            assertEquals((TPRREuint)0, wnd.getBottomFrameHeight(), "bottomFrameHeight") &
            assertEquals((TPRREuint)0, wnd.getCaptionHeight(), "captionHeight") &
            assertEquals((TPRREuint)0, wnd.getTitleBarHeight(), "titleBarHeight") &
            assertEquals("", wnd.getCaption(), "caption") &
            assertEquals((HDC)0, wnd.getWndDC(), "dc") &
            assertEquals((HWND)0, wnd.getWndHandle(), "handle") &
            assertEquals((TPRREuint)0, wnd.getX(), "x") &
            assertEquals((TPRREuint)0, wnd.getY(), "y") &
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

        return assertEquals((TPRREuint)0, wnd.getWidth(), "width") &
            assertEquals((TPRREuint)0, wnd.getHeight(), "height") &
            assertEquals((TPRREuint)0, wnd.getClientWidth(), "clwidth") &
            assertEquals((TPRREuint)0, wnd.getClientHeight(), "clheight") &
            assertEquals((TPRREuint)0, wnd.getSideFrameWidth(), "sideFrameWidth") &
            assertEquals((TPRREuint)0, wnd.getBottomFrameHeight(), "bottomFrameHeight") &
            assertEquals((TPRREuint)0, wnd.getCaptionHeight(), "captionHeight") &
            assertEquals((TPRREuint)0, wnd.getTitleBarHeight(), "titleBarHeight") &
            assertEquals("", wnd.getCaption(), "caption") &
            assertEquals((HDC)0, wnd.getWndDC(), "dc") &
            assertEquals((HWND)0, wnd.getWndHandle(), "handle") &
            assertEquals((TPRREuint)0, wnd.getX(), "x") &
            assertEquals((TPRREuint)0, wnd.getY(), "y") &
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
        return assertEquals((TPRREuint)0, wnd.getWidth());
    }

    bool testSetHeight()
    {
        wnd.SetHeight(5);
        return assertEquals((TPRREuint)0, wnd.getHeight());
    }

    bool testSetClientWidth()
    {
        wnd.SetClientWidth(5);
        return assertEquals((TPRREuint)0, wnd.getClientWidth());
    }

    bool testSetClientHeight()
    {
        wnd.SetClientHeight(5);
        return assertEquals((TPRREuint)0, wnd.getClientHeight());
    }

    bool testGetSideFrameWidth()
    {
        return assertEquals((TPRREuint)0, wnd.getSideFrameWidth());
    }
    
    bool testGetBottomFrameHeight()
    {
        return assertEquals((TPRREuint)0, wnd.getBottomFrameHeight());
    }
    
    bool testGetCaptionHeight()
    {
        return assertEquals((TPRREuint)0, wnd.getCaptionHeight());
    }
    
    bool testGetTitleBarHeight()
    {
        return assertEquals((TPRREuint)0, wnd.getTitleBarHeight());
    }

    bool testSetX()
    {
        wnd.SetX(5);
        return assertEquals((TPRREuint)0, wnd.getX());
    }

    bool testSetY()
    {
        wnd.SetY(5);
        return assertEquals((TPRREuint)0, wnd.getY());
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
    
}; // class PRREWindowTest
