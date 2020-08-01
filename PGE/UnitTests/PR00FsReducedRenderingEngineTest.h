#pragma once

/*
    ###################################################################################
    PR00FsReducedRenderingEngineTest.h
    Unit test for PR00FsReducedRenderingEngine.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/PR00FsReducedRenderingEngine.h"

class PR00FsReducedRenderingEngineTest :
    public UnitTest
{
public:

    PR00FsReducedRenderingEngineTest() :
        UnitTest( __FILE__, "not inited" )
    {}

protected:

    virtual void Initialize()
    {
        engine = NULL;
        
        AddSubTest("testInitializeInvalidResolution2", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeInvalidResolution2);
        AddSubTest("testInitializeInvalidColorDepth", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeInvalidColorDepth);
        AddSubTest("testInitializeInvalidZDepth", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeInvalidZDepth);
        AddSubTest("testInitializeInvalidRefreshRate", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeInvalidRefreshRate);
        AddSubTest("testInitializeWindowed800x600x32x24x0", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeWindowed800x600x32x24x0);
        AddSubTest("testInitializeWindowed800x600x32x24x8", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeWindowed800x600x32x24x8);
        AddSubTest("testInitializeFullScreen800x600x32x24x0", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeFullScreen800x600x32x24x0);
        AddSubTest("testInitializeFullScreenRefreshRate0", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeFullScreenRefreshRate0);
        AddSubTest("testInitializeWindowedCurrentResolution", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeWindowedCurrentResolution);
        AddSubTest("testInitializeFullScreenCurrentResolution", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testInitializeFullScreenCurrentResolution);
        AddSubTest("testShutdown", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testShutdown);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testIsInitialized);
        AddSubTest("testGetScreen", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testGetScreen);
        AddSubTest("testGetWindow", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testGetWindow);
        AddSubTest("testGetHardwareInfo", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testGetHardwareInfo);
        AddSubTest("testGetImageManager", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testGetImageManager);
        AddSubTest("testGetTextureManager", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testGetTextureManager);
        AddSubTest("testGetObject3DManager", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testGetObject3DManager);
        AddSubTest("testGetCamera", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testGetCamera);
        AddSubTest("testGetUImanager", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testGetUImanager);
        AddSubTest("testGetRenderer", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testGetRenderer);
        AddSubTest("testCopyScreenToTexture", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest::testCopyScreenToTexture);
    }

    virtual void SetUp()
    {
        engine = &PR00FsReducedRenderingEngine::createAndGet();
    }

    virtual void TearDown()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
    }

private:

    PR00FsReducedRenderingEngine* engine;

    // ---------------------------------------------------------------------------

    PR00FsReducedRenderingEngineTest(const PR00FsReducedRenderingEngineTest&)
    {};         

    PR00FsReducedRenderingEngineTest& operator=(const PR00FsReducedRenderingEngineTest&)
    {
        return *this;
    };

    bool testInitializeInvalidResolution2()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 400, 400, PRRE_FULLSCREEN, 0, 32, 24, 0, 0) > 0, "initialize()") &
            assertFalse(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeInvalidColorDepth()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 0, 24, 0, 0) > 0, "initialize()") &
            assertFalse(engine->isInitialized(), "isInitialized()"); 
    }

    bool testInitializeInvalidZDepth()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 0, 0, 0) > 0, "initialize()") &
            assertFalse(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeInvalidRefreshRate()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_FULLSCREEN, 74554, 32, 24, 0, 0) > 0, "initialize()") &
            assertFalse(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeWindowed800x600x32x24x0()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeWindowed800x600x32x24x8()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 8, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeFullScreen800x600x32x24x0()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_FULLSCREEN, 0, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeFullScreenRefreshRate0()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_FULLSCREEN, 60, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeWindowedCurrentResolution()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 0, 0, PRRE_WINDOWED, 0, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testInitializeFullScreenCurrentResolution()
    {
        return assertTrue(engine->initialize(PRRE_RENDERER_HW_FP, 0, 0, PRRE_FULLSCREEN, 0, 32, 24, 0, 0) == 0, "initialize()") &
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
        PRRETexture* tex = NULL;
        engine->CopyScreenToTexture(*tex);

        return true;
    }
    
}; // class PR00FsReducedRenderingEngineTest