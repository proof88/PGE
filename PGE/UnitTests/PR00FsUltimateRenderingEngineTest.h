#pragma once

/*
    ###################################################################################
    PR00FsUltimateRenderingEngineTest.h
    Unit test for PR00FsUltimateRenderingEngine.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"

class PR00FsUltimateRenderingEngineTest :
    public UnitTest
{
public:

    PR00FsUltimateRenderingEngineTest() :
        UnitTest( __FILE__, "not inited" ),
        cfgProfiles("")
    {
        
    }

    virtual ~PR00FsUltimateRenderingEngineTest()
    {
        
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PR00FsUltimateRenderingEngine::getLoggerModuleName(), true);

        engine = NULL;
        
        AddSubTest("testInitializeInvalidResolution2", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeInvalidResolution2);
        AddSubTest("testInitializeInvalidColorDepth", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeInvalidColorDepth);
        AddSubTest("testInitializeInvalidZDepth", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeInvalidZDepth);
        AddSubTest("testInitializeInvalidRefreshRate", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeInvalidRefreshRate);
        AddSubTest("testInitializeWindowed800x600x32x24x0", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeWindowed800x600x32x24x0);
        AddSubTest("testInitializeWindowed800x600x32x24x8", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeWindowed800x600x32x24x8);
        AddSubTest("testInitializeFullScreen800x600x32x24x0", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeFullScreen800x600x32x24x0);
        AddSubTest("testInitializeFullScreenRefreshRate0", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeFullScreenRefreshRate0);
        AddSubTest("testInitializeWindowedCurrentResolution", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeWindowedCurrentResolution);
        AddSubTest("testInitializeFullScreenCurrentResolution", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeFullScreenCurrentResolution);
        AddSubTest("testShutdown", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testShutdown);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testIsInitialized);
        AddSubTest("testGetScreen", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetScreen);
        AddSubTest("testGetWindow", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetWindow);
        AddSubTest("testGetHardwareInfo", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetHardwareInfo);
        AddSubTest("testGetImageManager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetImageManager);
        AddSubTest("testGetTextureManager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetTextureManager);
        AddSubTest("testGetObject3DManager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetObject3DManager);
        AddSubTest("testGetCamera", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetCamera);
        AddSubTest("testGetUImanager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetUImanager);
        AddSubTest("testGetRenderer", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetRenderer);
        AddSubTest("testCopyScreenToTexture", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testCopyScreenToTexture);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testWriteList);
        AddSubTest("testGetAutoWriteStatsAtShutdown", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetAutoWriteStatsAtShutdown);
        AddSubTest("testSetAutoWriteStatsAtShutdown", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testSetAutoWriteStatsAtShutdown);
    }

    virtual bool setUp()
    {
        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);

        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        return assertNotNull(engine, "engine null");
    }

    virtual void TearDown()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
    }

    virtual void Finalize()
    {
        CConsole::getConsoleInstance().SetLoggingState(PR00FsUltimateRenderingEngine::getLoggerModuleName(), false);    
    }

private:

    PR00FsUltimateRenderingEngine* engine;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PR00FsUltimateRenderingEngineTest(const PR00FsUltimateRenderingEngineTest&) :
        cfgProfiles("")
    {};         

    PR00FsUltimateRenderingEngineTest& operator=(const PR00FsUltimateRenderingEngineTest&)
    {
        return *this;
    };

    bool testInitializeInvalidResolution2()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 400, 400, PURE_FULLSCREEN, 0, 32, 24, 0, 0) > 0, "initialize()") &
            assertFalse(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeInvalidColorDepth()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 0, 24, 0, 0) > 0, "initialize()") &
            assertFalse(engine->isInitialized(), "isInitialized()"); 
    }

    bool testInitializeInvalidZDepth()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 0, 0, 0) > 0, "initialize()") &
            assertFalse(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeInvalidRefreshRate()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_FULLSCREEN, 74554, 32, 24, 0, 0) > 0, "initialize()") &
            assertFalse(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeWindowed800x600x32x24x0()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeWindowed800x600x32x24x8()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 8, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeFullScreen800x600x32x24x0()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_FULLSCREEN, 0, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeFullScreenRefreshRate0()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_FULLSCREEN, 60, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeWindowedCurrentResolution()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 0, 0, PURE_WINDOWED, 0, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeFullScreenCurrentResolution()
    {
        return assertTrue(engine->initialize(PURE_RENDERER_HW_FP, 0, 0, PURE_FULLSCREEN, 0, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testShutdown()
    {
        return assertTrue( engine->shutdown() );
    }

    bool testIsInitialized()
    {
        return assertFalse( engine->isInitialized() );
    }

    bool testGetScreen()
    {
        return assertFalse( engine->getScreen().isInitialized() );
    }

    bool testGetWindow()
    {
        return assertFalse( engine->getWindow().isInitialized() );
    }

    bool testGetHardwareInfo()
    {
        return assertFalse( engine->getHardwareInfo().isInitialized() );
    }

    bool testGetImageManager()
    {
        return assertNull( &engine->getImageManager() );
    }

    bool testGetTextureManager()
    {
        return assertNull( &engine->getTextureManager() );
    }

    bool testGetObject3DManager()
    {
        return assertNull( &engine->getObject3DManager() );
    }

    bool testGetCamera()
    {
        return assertNull( &engine->getCamera() );
    }

    bool testGetUImanager()
    {
        return assertFalse( engine->getUImanager().isInitialized() );
    }

    bool testGetRenderer()
    {
        return assertNull( engine->getRenderer() );
    }

    bool testCopyScreenToTexture()
    {
        // we just simply call this and expect nothing bad to happen
        PureTexture* tex = NULL;
        engine->CopyScreenToTexture(*tex);

        return true;
    }

    bool testWriteList()
    {
        engine->WriteList();

        return true;
    }

    bool testGetAutoWriteStatsAtShutdown()
    {
        return assertFalse(engine->getAutoWriteStatsAtShutdown());
    }

    bool testSetAutoWriteStatsAtShutdown()
    {
        bool b = assertFalse(engine->getAutoWriteStatsAtShutdown(), "get 1");

        engine->SetAutoWriteStatsAtShutdown(true);
        b &= assertTrue(engine->getAutoWriteStatsAtShutdown(), "get 2");

        engine->SetAutoWriteStatsAtShutdown(false);
        b &= assertFalse(engine->getAutoWriteStatsAtShutdown(), "get 3");

        return b;
    }
    
}; // class PR00FsUltimateRenderingEngineTest