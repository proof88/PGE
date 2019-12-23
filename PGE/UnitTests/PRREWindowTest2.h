#pragma once

/*
    ###################################################################################
    PRREWindowTest2.h
    Unit test for PRREWindow instance which is already initialized.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGE/PRRE/PRREWindow.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 500
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 500
#endif

class PRREWindowTest2 :
    public UnitTest
{
public:

    PRREWindowTest2() :
        UnitTest(__FILE__, "inited"),
        wnd( PRREWindow::createAndGet() )
    {
    }

    ~PRREWindowTest2()
    {
        Finalize();
    } // ~PRREWindowTest2()

protected:

    virtual void Initialize()
    {
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PRREWindowTest2::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PRREWindowTest2::testDeinitialize);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PRREWindowTest2::testIsInitialized);
        AddSubTest("testResetDefaults", (PFNUNITSUBTEST) &PRREWindowTest2::testResetDefaults);
        AddSubTest("testGetWidth", (PFNUNITSUBTEST) &PRREWindowTest2::testGetWidth);
        AddSubTest("testGetHeight", (PFNUNITSUBTEST) &PRREWindowTest2::testGetHeight);
        AddSubTest("testGetClientWidth", (PFNUNITSUBTEST) &PRREWindowTest2::testGetClientWidth);
        AddSubTest("testGetClientHeight", (PFNUNITSUBTEST) &PRREWindowTest2::testGetClientHeight);
        AddSubTest("testSetWidth", (PFNUNITSUBTEST) &PRREWindowTest2::testSetWidth);
        AddSubTest("testSetHeight", (PFNUNITSUBTEST) &PRREWindowTest2::testSetHeight);
        AddSubTest("testSetClientWidth", (PFNUNITSUBTEST) &PRREWindowTest2::testSetClientWidth);
        AddSubTest("testSetClientHeight", (PFNUNITSUBTEST) &PRREWindowTest2::testSetClientHeight);
        AddSubTest("testGetSideFrameWidth", (PFNUNITSUBTEST) &PRREWindowTest2::testGetSideFrameWidth);
        AddSubTest("testGetBottomFrameHeight", (PFNUNITSUBTEST) &PRREWindowTest2::testGetBottomFrameHeight);
        AddSubTest("testGetCaptionHeight", (PFNUNITSUBTEST) &PRREWindowTest2::testGetCaptionHeight);
        AddSubTest("testGetTitleBarHeight", (PFNUNITSUBTEST) &PRREWindowTest2::testGetTitleBarHeight);
        AddSubTest("testSetX", (PFNUNITSUBTEST) &PRREWindowTest2::testSetX);
        AddSubTest("testSetY", (PFNUNITSUBTEST) &PRREWindowTest2::testSetY);
        AddSubTest("testGetWndHandle", (PFNUNITSUBTEST) &PRREWindowTest2::testGetWndHandle);
        AddSubTest("testGetWndDC", (PFNUNITSUBTEST) &PRREWindowTest2::testGetWndDC);
        AddSubTest("testSetActive", (PFNUNITSUBTEST) &PRREWindowTest2::testSetActive);
        AddSubTest("testSetTopMost", (PFNUNITSUBTEST) &PRREWindowTest2::testSetTopMost);
        AddSubTest("testSetCaption", (PFNUNITSUBTEST) &PRREWindowTest2::testSetCaption);
        AddSubTest("testSetResizableH", (PFNUNITSUBTEST) &PRREWindowTest2::testSetResizableH);
        AddSubTest("testSetResizableV", (PFNUNITSUBTEST) &PRREWindowTest2::testSetResizableV);
        AddSubTest("testSetResizable", (PFNUNITSUBTEST) &PRREWindowTest2::testSetResizable);
        AddSubTest("testSetBorder", (PFNUNITSUBTEST) &PRREWindowTest2::testSetBorder);
        AddSubTest("testSetSysMenu", (PFNUNITSUBTEST) &PRREWindowTest2::testSetSysMenu);
        AddSubTest("testSetMinButton", (PFNUNITSUBTEST) &PRREWindowTest2::testSetMinButton);
        AddSubTest("testSetMaxButton", (PFNUNITSUBTEST) &PRREWindowTest2::testSetMaxButton);
        AddSubTest("testSetEnabled", (PFNUNITSUBTEST) &PRREWindowTest2::testSetEnabled);
        AddSubTest("testSetWindowProc", (PFNUNITSUBTEST) &PRREWindowTest2::testSetWindowProc);
        AddSubTest("testProcessMessages", (PFNUNITSUBTEST) &PRREWindowTest2::testProcessMessages);
        AddSubTest("testHasCloseRequest", (PFNUNITSUBTEST) &PRREWindowTest2::testHasCloseRequest);
        AddSubTest("testSetAutoRedrawAllOnPosChange", (PFNUNITSUBTEST) &PRREWindowTest2::testSetAutoRedrawAllOnPosChange);
        AddSubTest("testSetAutoCleanupOnQuit", (PFNUNITSUBTEST) &PRREWindowTest2::testSetAutoCleanupOnQuit);
        AddSubTest("testSetAutoCameraWarningState", (PFNUNITSUBTEST) &PRREWindowTest2::testSetAutoCameraWarningState);
        AddSubTest("testShow", (PFNUNITSUBTEST) &PRREWindowTest2::testShow);
        AddSubTest("testShowFull", (PFNUNITSUBTEST) &PRREWindowTest2::testShowFull);
        AddSubTest("testHide", (PFNUNITSUBTEST) &PRREWindowTest2::testHide);
        AddSubTest("testMinimize", (PFNUNITSUBTEST) &PRREWindowTest2::testMinimize);
        AddSubTest("testMaximize", (PFNUNITSUBTEST) &PRREWindowTest2::testMaximize);
        AddSubTest("testRestore", (PFNUNITSUBTEST) &PRREWindowTest2::testRestore);
        AddSubTest("testClose", (PFNUNITSUBTEST) &PRREWindowTest2::testClose);
        AddSubTest("testSetCursorVisible", (PFNUNITSUBTEST) &PRREWindowTest2::testSetCursorVisible);
        AddSubTest("testWriteSettings", (PFNUNITSUBTEST) &PRREWindowTest2::testWriteSettings);
    }

    virtual void SetUp()
    {
        wnd.initialize(WINDOW_WIDTH, WINDOW_HEIGHT, false, "alma");    
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

    PRREWindowTest2(const PRREWindowTest2&) :
        wnd( PRREWindow::createAndGet() )
    {};         

    PRREWindowTest2& operator=(const PRREWindowTest2&)
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
        return assertLequals((TPRREuint)WINDOW_WIDTH, wnd.getWidth());
    }

    bool testGetHeight()
    {
        return assertLequals((TPRREuint)WINDOW_HEIGHT, wnd.getHeight());
    }

    bool testGetClientWidth()
    {
        return assertEquals((TPRREuint)WINDOW_WIDTH, wnd.getClientWidth());
    }

    bool testGetClientHeight()
    {
        return assertEquals((TPRREuint)WINDOW_WIDTH, wnd.getClientHeight());
    }

    bool testSetWidth()
    {
        const unsigned int newWidth = 50;
        wnd.SetWidth(newWidth);
        return assertEquals((TPRREuint)newWidth, wnd.getWidth(), "width") &
            assertGequals((TPRREuint)newWidth, wnd.getClientWidth(), "clientwidth");
    }

    bool testSetHeight()
    {
        const unsigned int newHeight = 50;
        wnd.SetHeight(newHeight);
        return assertEquals((TPRREuint)newHeight, wnd.getHeight(), "height") &
            assertGequals((TPRREuint)newHeight, wnd.getClientHeight(), "clientheight");
    }

    bool testSetClientWidth()
    {
        const unsigned int newClientWidth = 50;
        wnd.SetClientWidth(newClientWidth);
        return assertEquals((TPRREuint)newClientWidth, wnd.getClientWidth(), "clientwidth") &
            assertLequals((TPRREuint)newClientWidth, wnd.getWidth(), "width");
    }

    bool testSetClientHeight()
    {
        const unsigned int newClientHeight = 50;
        wnd.SetClientHeight(newClientHeight);
        return assertEquals((TPRREuint)newClientHeight, wnd.getClientHeight(), "clientheight") &
            assertLequals((TPRREuint)newClientHeight, wnd.getHeight(), "height");
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
        return assertEquals((TPRREuint)5, wnd.getX(), "getX") &
            assertLequals((TPRREuint)WINDOW_WIDTH, wnd.getWidth(), "width") &
            assertLequals((TPRREuint)WINDOW_HEIGHT, wnd.getHeight(), "height") &
            assertEquals((TPRREuint)WINDOW_WIDTH, wnd.getClientWidth(), "clientwidth") &
            assertEquals((TPRREuint)WINDOW_HEIGHT, wnd.getClientHeight(), "clientheight");
    }

    bool testSetY()
    {
        wnd.SetY(5);
        return assertEquals((TPRREuint)5, wnd.getY()) &
            assertLequals((TPRREuint)WINDOW_WIDTH, wnd.getWidth(), "width") &
            assertLequals((TPRREuint)WINDOW_HEIGHT, wnd.getHeight(), "height") &
            assertEquals((TPRREuint)WINDOW_WIDTH, wnd.getClientWidth(), "clientwidth") &
            assertEquals((TPRREuint)WINDOW_HEIGHT, wnd.getClientHeight(), "clientheight");
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
    
}; // class PRREWindowTest2
