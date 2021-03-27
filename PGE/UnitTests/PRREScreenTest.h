#pragma once

/*
    ###################################################################################
    PRREScreenTest.h
    Unit test for PRREScreen.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/Display/PRREScreen.h"
#include "../PRRE/include/external/Display/PRREWindow.h"
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREScreenTest :
    public UnitTest
{
public:

    PRREScreenTest() :
        UnitTest( __FILE__ ),
        scr( PRREScreen::createAndGet() ),
        wnd( PRREWindow::createAndGet() )
    {
        engine = NULL;
        screensvrOrig = false;
        monitorpwrOrig = false;
        standbyOrig = false;
        vsyncOrig = false;
        
    }

    virtual ~PRREScreenTest()
    {
        
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        screensvrOrig = scr.isScreensaverEnabled();
        monitorpwrOrig = scr.isMonitorPowersaveEnabled();
        standbyOrig = scr.isStandbyEnabled();
        vsyncOrig = scr.isVSyncEnabled();
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREScreenTest::testCtor);
        AddSubTest("testApplyDisplaySettings", (PFNUNITSUBTEST) &PRREScreenTest::testApplyDisplaySettings);
        AddSubTest("testResetDisplaySettings", (PFNUNITSUBTEST) &PRREScreenTest::testResetDisplaySettings);
        AddSubTest("testIsInitialized1", (PFNUNITSUBTEST) &PRREScreenTest::testIsInitialized1);
        AddSubTest("testSetResolution1", (PFNUNITSUBTEST) &PRREScreenTest::testSetResolution1);
        AddSubTest("testSetFullScreened1", (PFNUNITSUBTEST) &PRREScreenTest::testSetFullScreened1);
        AddSubTest("testSetFreq1", (PFNUNITSUBTEST) &PRREScreenTest::testSetFreq1);
        AddSubTest("testSetColorBits1", (PFNUNITSUBTEST) &PRREScreenTest::testSetColorBits1);
        AddSubTest("testSetDepthBit1s", (PFNUNITSUBTEST) &PRREScreenTest::testSetDepthBits1);
        AddSubTest("testSetStencilBits1", (PFNUNITSUBTEST) &PRREScreenTest::testSetStencilBits1);
        AddSubTest("testIsInitialized2", (PFNUNITSUBTEST) &PRREScreenTest::testIsInitialized2);
        AddSubTest("testSetResolution2", (PFNUNITSUBTEST) &PRREScreenTest::testSetResolution2);
        AddSubTest("testSetFullscreened2", (PFNUNITSUBTEST) &PRREScreenTest::testSetFullscreened2);
        AddSubTest("testSetFreq2", (PFNUNITSUBTEST) &PRREScreenTest::testSetFreq2);
        AddSubTest("testSetColorBits2", (PFNUNITSUBTEST) &PRREScreenTest::testSetColorBits2);
        AddSubTest("testSetDepthBits2", (PFNUNITSUBTEST) &PRREScreenTest::testSetDepthBits2);
        AddSubTest("testSetStencilBits2", (PFNUNITSUBTEST) &PRREScreenTest::testSetStencilBits2);
        AddSubTest("testSetResolution3", (PFNUNITSUBTEST) &PRREScreenTest::testSetResolution3);
        AddSubTest("testSetScreensaver", (PFNUNITSUBTEST) &PRREScreenTest::testSetScreensaver);
        AddSubTest("testSetMonitorPowersaveEnabled", (PFNUNITSUBTEST) &PRREScreenTest::testSetMonitorPowersaveEnabled);
        AddSubTest("testSetStandbyEnabled", (PFNUNITSUBTEST) &PRREScreenTest::testSetStandbyEnabled);
        AddSubTest("testSetVSyncEnabled1", (PFNUNITSUBTEST) &PRREScreenTest::testSetVSyncEnabled1);
        AddSubTest("testSetVSyncEnabled2", (PFNUNITSUBTEST) &PRREScreenTest::testSetVSyncEnabled2);
        AddSubTest("testSetFSAAlevel1", (PFNUNITSUBTEST) &PRREScreenTest::testSetFSAAlevel1);
        AddSubTest("testSetFSAAlevel2", (PFNUNITSUBTEST) &PRREScreenTest::testSetFSAAlevel2);
        AddSubTest("testSetFSAAlevel3", (PFNUNITSUBTEST) &PRREScreenTest::testSetFSAAlevel3);
    }

    virtual void Finalize()
    {
        scr.SetScreensaverEnabled(screensvrOrig);
        scr.SetMonitorPowersaveEnabled(monitorpwrOrig);
        scr.SetStandbyEnabled(standbyOrig);
        scr.SetVSyncEnabled(vsyncOrig);
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", false);
    }


private:
    PRREScreen& scr;
    PRREWindow& wnd;
    PR00FsReducedRenderingEngine* engine;
    bool screensvrOrig;
    bool monitorpwrOrig;
    bool standbyOrig;
    bool vsyncOrig;

    // ---------------------------------------------------------------------------

    PRREScreenTest(const PRREScreenTest&) :
        scr( PRREScreen::createAndGet() ),
        wnd( PRREWindow::createAndGet() )
    {};         

    PRREScreenTest& operator=(const PRREScreenTest&)
    {
        return *this;
    };

    bool initDisp(const bool bFSAA = false)
    {
        wnd.initialize(800, 600, false, "alma");

        scr.SetFullScreened(true);
        scr.SetColorBits(32);
        scr.SetDepthBits(24);
        scr.SetStencilBits(0);
        scr.SetFreq(0);
        scr.SetFSAAlevel(bFSAA ? 2 : 0);
        return scr.applyDisplaySettings(wnd.getWndDC());
    }

    void StopDisp()
    {
        scr.ResetDisplaySettings();
        wnd.Deinitialize();
    }

    bool testCtor()
    {
        return assertEquals(0, scr.getColorBits(), "colorBits") &
            assertEquals(0, scr.getDepthBits(), "depthBits") &
            assertEquals((TPRREuint) 0, scr.getFreq(), "freq") &
            assertEquals(0, scr.getFSAAlevel(), "FSAAcurrent") &
            assertFalse(scr.isFullScreened(), "fullscreen") &
            assertEquals((TPRREuint) 0, scr.getResWidth(), "resWidth") &
            assertEquals((TPRREuint) 0, scr.getResHeight(), "resHeight") &
            assertEquals(0, scr.getStencilBits(), "stencilBits") &
            assertFalse(scr.isVSyncEnabled(), "vsync") &
            assertFalse(scr.isFSAAready(), "FSAAready") &
            assertTrue(scr.isMonitorPowersaveEnabled(), "monitorpwrsave") &
            assertFalse(scr.isScreensaverEnabled(), "screensvr") &
            assertFalse(scr.isStandbyEnabled(), "standby");
    }

    bool testApplyDisplaySettings()
    {
        wnd.initialize(800, 600, false, "alma");

        const bool b1 = scr.applyDisplaySettings(wnd.getWndDC());
        scr.SetColorBits(32);
        const bool b2 = scr.applyDisplaySettings(wnd.getWndDC());
        scr.SetDepthBits(24);
        const bool b3 = scr.applyDisplaySettings(wnd.getWndDC());
        scr.SetResolution(800, 600);
        scr.SetDepthBits(0);
        const bool b4 = scr.applyDisplaySettings(wnd.getWndDC());
        scr.SetDepthBits(24);
        const bool b5 = scr.applyDisplaySettings(wnd.getWndDC());
        const bool b6 = scr.applyDisplaySettings(wnd.getWndDC());

        scr.ResetDisplaySettings();
        wnd.Deinitialize();

        wnd.initialize(800, 600, true, "alma");
        scr.SetFullScreened(true);
        const bool b7 = scr.applyDisplaySettings(wnd.getWndDC());

        scr.ResetDisplaySettings();
        wnd.Deinitialize();

        wnd.initialize(200, 200, false, "alma");
        scr.SetFullScreened(false);
        scr.SetResolution(0, 0);
        const bool b8 = scr.applyDisplaySettings(wnd.getWndDC());
        const bool b9 = (scr.getResWidth() > 0) && (scr.getResHeight() != 200);

        scr.ResetDisplaySettings();
        wnd.Deinitialize();

        return assertFalse(b1, "b1") & assertFalse(b2, "b2") & assertFalse(b3, "b3") &
            assertFalse(b4, "b4") & assertTrue(b5, "b5") & assertFalse(b6, "b6") &
            assertTrue(b7, "b7") & assertTrue(b8, "b8") & assertTrue(b9, "b9");
    }

    bool testResetDisplaySettings()
    {
        const bool bA1 = initDisp();

        const TPRREuint desktopResX = scr.getResWidth();
        const TPRREuint desktopResY = scr.getResHeight();
        const TPRREint  desktopCBits = scr.getColorBits();
        
        scr.ResetDisplaySettings();
        
        scr.SetFullScreened(true);
        const bool bA2 = scr.applyDisplaySettings(wnd.getWndDC());

        scr.ResetDisplaySettings();
        scr.SetFullScreened(false);
        bool bA3 = scr.applyDisplaySettings(wnd.getWndDC());

        const bool b1 = (desktopResX == scr.getResWidth());
        const bool b2 = (desktopResY == scr.getResHeight());
        const bool b3 = (desktopCBits == scr.getColorBits());

        StopDisp();

        return assertTrue(b1, "b1") & assertTrue(b2, "b2") & assertTrue(b3, "b3") &
            assertTrue(bA1, "bA1") & assertTrue(bA2, "bA2") & assertTrue(bA3, "bA3");
    }

    bool testIsInitialized1()
    {
        return assertFalse(scr.isInitialized());
    }

    bool testSetResolution1()
    {
        scr.SetResolution(800, 600);
        return assertEquals((TPRREuint) 800, scr.getResWidth(), "width") &
            assertEquals((TPRREuint) 600, scr.getResHeight(), "height"); 
    }

    bool testSetFullScreened1()
    {
        scr.SetFullScreened(true);
        const bool b1 = scr.isFullScreened();

        scr.SetFullScreened(false);
        const bool b2 = scr.isFullScreened();

        return assertTrue(b1, "b1") & assertFalse(b2, "b2");
    }

    bool testSetFreq1()
    {
        scr.SetFreq(30);
        return assertEquals((TPRREuint)30, scr.getFreq());
    }

    bool testSetColorBits1()
    {
        scr.SetColorBits(30);
        return assertEquals(30, scr.getColorBits());
    }

    bool testSetDepthBits1()
    {
        scr.SetDepthBits(30);
        return assertEquals(30, scr.getDepthBits());
    }

    bool testSetStencilBits1()
    {
        scr.SetStencilBits(30);
        return assertEquals(30, scr.getStencilBits());
    }

    bool testIsInitialized2()
    {
        const bool bA1 = initDisp();
        
        const bool b1 = scr.isInitialized();

        StopDisp();

        return assertTrue(bA1, "bA1") & assertTrue(b1, "b1");
    }

    bool testSetResolution2()
    {
        const bool bA1 = initDisp();

        scr.SetResolution(640, 480);
        const bool b1 = (scr.getResWidth() == (TPRREuint) 800);
        const bool b2 = (scr.getResHeight() == (TPRREuint) 600);

        StopDisp();

        return assertTrue(bA1, "bA1") & assertTrue(b1, "b1") & assertTrue(b2, "b2");
    }

    bool testSetFullscreened2()
    {
        const bool bA1 = initDisp();

        scr.SetFullScreened(false);
        const bool b1 = scr.isFullScreened();

        StopDisp();

        return assertTrue(bA1, "bA1") & assertTrue(b1, "b1");
    }

    bool testSetFreq2()
    {
        const bool bA1 = initDisp();

        scr.SetFreq(30);
        const bool b1 = ((TPRREuint)30 == scr.getFreq());

        StopDisp();

        return assertTrue(bA1, "bA1") & assertFalse(b1, "b1");
    }

    bool testSetColorBits2()
    {
        const bool bA1 = initDisp();

        scr.SetColorBits(30);
        const bool b1 = (30 == scr.getColorBits());

        StopDisp();

        return assertTrue(bA1, "bA1") & assertFalse(b1, "b1");
    }

    bool testSetDepthBits2()
    {
        const bool bA1 = initDisp();

        scr.SetDepthBits(30);
        const bool b1 = (30 == scr.getDepthBits());

        StopDisp();

        return assertTrue(bA1, "bA1") & assertFalse(b1, "b1");
    }

    bool testSetStencilBits2()
    {
        const bool bA1 = initDisp();

        scr.SetStencilBits(30);
        const bool b1 = (30 == scr.getStencilBits());

        StopDisp();

        return assertTrue(bA1, "bA1") & assertFalse(b1, "b1");
    }

    bool testSetResolution3()
    {
        scr.SetResolution(0, 0);
        return assertNotEquals((TPRREuint) 0, scr.getResWidth(), "width") &
            assertNotEquals((TPRREuint) 0, scr.getResHeight(), "height"); 
    }

    bool testSetScreensaver()
    {
        scr.SetScreensaverEnabled(true);
        return assertTrue(scr.isScreensaverEnabled());
    }

    bool testSetMonitorPowersaveEnabled()
    {
        scr.SetMonitorPowersaveEnabled(false);
        return assertFalse(scr.isMonitorPowersaveEnabled());
    }

    bool testSetStandbyEnabled()
    {
        scr.SetStandbyEnabled(true);
        return assertTrue(scr.isStandbyEnabled());
    }

    bool testSetVSyncEnabled1()
    {
        scr.SetVSyncEnabled(true);
        return assertFalse(scr.isVSyncEnabled());
    }

    bool testSetVSyncEnabled2()
    {
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        
        scr.SetVSyncEnabled(true);
        const bool b1 = scr.isVSyncEnabled();

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return assertTrue(b1);
    }

    bool testSetFSAAlevel1()
    {
        scr.SetFSAAlevel(2);
        return assertEquals(0, scr.getFSAAlevel());
    }

    bool testSetFSAAlevel2()
    {
        const bool bA1 = initDisp(true);

        scr.SetFSAAlevel(4);
        const bool b1 = (4 == scr.getFSAAlevel()); // shouldn't be altered after initDisp()
        const bool b2 = (0 == scr.getFSAAlevel()); // should be 0 because there was no FSAA available without GL init

        StopDisp();

        return assertTrue(bA1, "bA1") & assertFalse(b1, "b1") & assertTrue(b2, "b2");
    }

    bool testSetFSAAlevel3()
    {
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 2); // FSAA 2x

        const bool b1 = scr.isFSAAready();
        const bool b2 = (scr.getFSAAlevel() == 2);

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return assertTrue(b1, "b1") & assertTrue(b2, "b2");
    }

    
}; // class PRREScreenTest
