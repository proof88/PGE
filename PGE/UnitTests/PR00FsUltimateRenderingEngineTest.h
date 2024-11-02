#pragma once

/*
    ###################################################################################
    PR00FsUltimateRenderingEngineTest.h
    Unit test for PR00FsUltimateRenderingEngine.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"

class PR00FsUltimateRenderingEngineTest :
    public UnitTest
{
public:

    PR00FsUltimateRenderingEngineTest() :
        UnitTest( __FILE__, "not inited" )
    {
        
    }

    virtual ~PR00FsUltimateRenderingEngineTest()
    {
        
    }

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PR00FsUltimateRenderingEngine::getLoggerModuleName(), true);

        engine = NULL;
        
        addSubTest("testInitializeInvalidResolution2", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeInvalidResolution2);
        addSubTest("testInitializeInvalidColorDepth", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeInvalidColorDepth);
        addSubTest("testInitializeInvalidZDepth", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeInvalidZDepth);
        addSubTest("testInitializeInvalidRefreshRate", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeInvalidRefreshRate);
        addSubTest("testInitializeWindowed800x600x32x24x0", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeWindowed800x600x32x24x0);
        addSubTest("testInitializeWindowed800x600x32x24x8", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeWindowed800x600x32x24x8);
        addSubTest("testInitializeFullScreen800x600x32x24x0", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeFullScreen800x600x32x24x0);
        addSubTest("testInitializeFullScreenRefreshRate0", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeFullScreenRefreshRate0);
        addSubTest("testInitializeWindowedCurrentResolution", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeWindowedCurrentResolution);
        addSubTest("testInitializeFullScreenCurrentResolution", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testInitializeFullScreenCurrentResolution);
        addSubTest("testShutdown", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testShutdown);
        addSubTest("testIsInitialized", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testIsInitialized);
        addSubTest("testGetScreen", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetScreen);
        addSubTest("testGetWindow", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetWindow);
        addSubTest("testGetHardwareInfo", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetHardwareInfo);
        addSubTest("testGetImageManager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetImageManager);
        addSubTest("testGetTextureManager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetTextureManager);
        addSubTest("testGetObject3DManager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetObject3DManager);
        addSubTest("testGetCamera", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetCamera);
        addSubTest("testGetUImanager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetUImanager);
        addSubTest("testGetRenderer", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetRenderer);
        addSubTest("testCopyScreenToTexture", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testCopyScreenToTexture);
        addSubTest("testWriteList", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testWriteList);
        addSubTest("testGetAutoWriteStatsAtShutdown", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testGetAutoWriteStatsAtShutdown);
        addSubTest("testSetAutoWriteStatsAtShutdown", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest::testSetAutoWriteStatsAtShutdown);
    }

    virtual bool setUp() override
    {
        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);

        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        return assertNotNull(engine, "engine null");
    }

    virtual void tearDown() override
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
    }

    virtual void finalize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(PR00FsUltimateRenderingEngine::getLoggerModuleName(), false);    
    }

private:

    PR00FsUltimateRenderingEngine* engine;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PR00FsUltimateRenderingEngineTest(const PR00FsUltimateRenderingEngineTest&)
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