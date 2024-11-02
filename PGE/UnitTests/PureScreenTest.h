#pragma once

/*
    ###################################################################################
    PureScreenTest.h
    Unit test for PureScreen.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Display/PureScreen.h"
#include "../Pure/include/external/Display/PureWindow.h"
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureScreenTest :
    public UnitTest
{
public:

    PureScreenTest() :
        UnitTest( __FILE__ ),
        scr( PureScreen::createAndGet() ),
        wnd( PureWindow::createAndGet(cfgProfiles, inputHandler) ),
        inputHandler(PGEInputHandler::createAndGet(cfgProfiles))
    {
        engine = NULL;
        screensvrOrig = false;
        monitorpwrOrig = false;
        standbyOrig = false;
        vsyncOrig = false;
        
    }

    virtual ~PureScreenTest()
    {
        
    }

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        screensvrOrig = scr.isScreensaverEnabled();
        monitorpwrOrig = scr.isMonitorPowersaveEnabled();
        standbyOrig = scr.isStandbyEnabled();
        vsyncOrig = scr.isVSyncEnabled();
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureScreenTest::testCtor);
        addSubTest("testApplyDisplaySettings", (PFNUNITSUBTEST) &PureScreenTest::testApplyDisplaySettings);
        addSubTest("testResetDisplaySettings", (PFNUNITSUBTEST) &PureScreenTest::testResetDisplaySettings);
        addSubTest("testIsInitialized1", (PFNUNITSUBTEST) &PureScreenTest::testIsInitialized1);
        addSubTest("testSetResolution1", (PFNUNITSUBTEST) &PureScreenTest::testSetResolution1);
        addSubTest("testSetFullScreened1", (PFNUNITSUBTEST) &PureScreenTest::testSetFullScreened1);
        addSubTest("testSetFreq1", (PFNUNITSUBTEST) &PureScreenTest::testSetFreq1);
        addSubTest("testSetColorBits1", (PFNUNITSUBTEST) &PureScreenTest::testSetColorBits1);
        addSubTest("testSetDepthBit1s", (PFNUNITSUBTEST) &PureScreenTest::testSetDepthBits1);
        addSubTest("testSetStencilBits1", (PFNUNITSUBTEST) &PureScreenTest::testSetStencilBits1);
        addSubTest("testIsInitialized2", (PFNUNITSUBTEST) &PureScreenTest::testIsInitialized2);
        addSubTest("testSetResolution2", (PFNUNITSUBTEST) &PureScreenTest::testSetResolution2);
        addSubTest("testSetFullscreened2", (PFNUNITSUBTEST) &PureScreenTest::testSetFullscreened2);
        addSubTest("testSetFreq2", (PFNUNITSUBTEST) &PureScreenTest::testSetFreq2);
        addSubTest("testSetColorBits2", (PFNUNITSUBTEST) &PureScreenTest::testSetColorBits2);
        addSubTest("testSetDepthBits2", (PFNUNITSUBTEST) &PureScreenTest::testSetDepthBits2);
        addSubTest("testSetStencilBits2", (PFNUNITSUBTEST) &PureScreenTest::testSetStencilBits2);
        addSubTest("testSetResolution3", (PFNUNITSUBTEST) &PureScreenTest::testSetResolution3);
        addSubTest("testSetScreensaver", (PFNUNITSUBTEST) &PureScreenTest::testSetScreensaver);
        addSubTest("testSetMonitorPowersaveEnabled", (PFNUNITSUBTEST) &PureScreenTest::testSetMonitorPowersaveEnabled);
        addSubTest("testSetStandbyEnabled", (PFNUNITSUBTEST) &PureScreenTest::testSetStandbyEnabled);
        addSubTest("testSetVSyncEnabled1", (PFNUNITSUBTEST) &PureScreenTest::testSetVSyncEnabled1);
        addSubTest("testSetVSyncEnabled2", (PFNUNITSUBTEST) &PureScreenTest::testSetVSyncEnabled2);
        addSubTest("testSetFSAAlevel1", (PFNUNITSUBTEST) &PureScreenTest::testSetFSAAlevel1);
        addSubTest("testSetFSAAlevel2", (PFNUNITSUBTEST) &PureScreenTest::testSetFSAAlevel2);
        addSubTest("testSetFSAAlevel3", (PFNUNITSUBTEST) &PureScreenTest::testSetFSAAlevel3);
    }

    virtual void finalize() override
    {
        scr.SetScreensaverEnabled(screensvrOrig);
        scr.SetMonitorPowersaveEnabled(monitorpwrOrig);
        scr.SetStandbyEnabled(standbyOrig);
        scr.setVSyncEnabled(vsyncOrig);
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", false);
    }


private:
    PureScreen& scr;
    PureWindow& wnd;
    PR00FsUltimateRenderingEngine* engine;
    bool screensvrOrig;
    bool monitorpwrOrig;
    bool standbyOrig;
    bool vsyncOrig;
    PGEcfgProfiles cfgProfiles;
    PGEInputHandler& inputHandler;

    // ---------------------------------------------------------------------------

    PureScreenTest(const PureScreenTest&) :
        scr( PureScreen::createAndGet() ),
        wnd( PureWindow::createAndGet(cfgProfiles, inputHandler) ),
        inputHandler(PGEInputHandler::createAndGet(cfgProfiles))
    {};         

    PureScreenTest& operator=(const PureScreenTest&)
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
        // settings like colorbits, depthbits, etc might have stayed as there were set previously in another test case,
        // and that is an expected behavior that ResetDisplaySettings() is not resetting them, so the only thing we
        // can expect is the initialized state
        return assertFalse(scr.isInitialized(), "initialized");
    }

    bool testApplyDisplaySettings()
    {
        wnd.initialize(800, 600, false, "alma");

        // ctor is not assuring us about default values of these, and some other unit test might have changed these before,
        // so lets reset these manually
        scr.SetFullScreened(false);
        scr.SetColorBits(0);
        scr.SetDepthBits(0);
        // cannot use SetResolution(0, 0) as it has special behavior, so we set 0 separately
        scr.SetResolution(0, 1);
        scr.SetResolution(1, 0);
        scr.SetDepthBits(0);

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

        const TPureUInt desktopResX = scr.getResWidth();
        const TPureUInt desktopResY = scr.getResHeight();
        const TPureInt  desktopCBits = scr.getColorBits();
        
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
        return assertEquals((TPureUInt) 800, scr.getResWidth(), "width") &
            assertEquals((TPureUInt) 600, scr.getResHeight(), "height"); 
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
        return assertEquals((TPureUInt)30, scr.getFreq());
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
        const bool b1 = (scr.getResWidth() == (TPureUInt) 800);
        const bool b2 = (scr.getResHeight() == (TPureUInt) 600);

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
        const bool b1 = ((TPureUInt)30 == scr.getFreq());

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
        return assertNotEquals((TPureUInt) 0, scr.getResWidth(), "width") &
            assertNotEquals((TPureUInt) 0, scr.getResHeight(), "height"); 
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
        const bool bRet = scr.setVSyncEnabled(true);
        return assertFalse(scr.isVSyncEnabled(), "isVSyncEnabled") &
            assertFalse(bRet, "returned");
    }

    bool testSetVSyncEnabled2()
    {
        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        
        const bool bRet = scr.setVSyncEnabled(true);
        const bool b1 = scr.isVSyncEnabled();

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return assertTrue(b1, "isVSyncEnabled") &
            assertTrue(bRet, "returned");;
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
        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 2); // FSAA 2x

        const bool b1 = scr.isFSAAready();
        const bool b2 = (scr.getFSAAlevel() == 2);

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return assertTrue(b1, "b1") & assertTrue(b2, "b2");
    }

    
}; // class PureScreenTest
