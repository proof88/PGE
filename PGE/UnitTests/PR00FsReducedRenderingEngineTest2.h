#pragma once

/*
    ###################################################################################
    PR00FsReducedRenderingEngineTest2.h
    Unit test for PR00FsReducedRenderingEngine.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"

class PR00FsReducedRenderingEngineTest2 :
    public UnitTest
{
public:

    PR00FsReducedRenderingEngineTest2() :
        UnitTest( __FILE__, "inited" )
    {
        
    }

    virtual ~PR00FsReducedRenderingEngineTest2()
    {
        
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PR00FsReducedRenderingEngine::getLoggerModuleName(), true);
        engine = NULL;
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testInitialize);
        AddSubTest("testShutdown", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testShutdown);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testIsInitialized);
        AddSubTest("testGetScreen", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetScreen);
        AddSubTest("testGetWindow", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetWindow);
        AddSubTest("testGetHardwareInfo", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetHardwareInfo);
        AddSubTest("testGetImageManager", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetImageManager);
        AddSubTest("testGetTextureManager", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetTextureManager);
        AddSubTest("testGetObject3DManager", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetObject3DManager);
        AddSubTest("testGetCamera", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetCamera);
        AddSubTest("testGetUImanager", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetUImanager);
        AddSubTest("testGetRenderer", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetRenderer);
        AddSubTest("testCopyScreenToTexture", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testCopyScreenToTexture);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testWriteList);
        AddSubTest("testGetAutoWriteStatsAtShutdown", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testGetAutoWriteStatsAtShutdown);
        AddSubTest("testSetAutoWriteStatsAtShutdown", (PFNUNITSUBTEST) &PR00FsReducedRenderingEngineTest2::testSetAutoWriteStatsAtShutdown);
    }

    virtual bool setUp()
    {
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        return assertEquals((TPRREuint) 0, engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0), "engine");  // pretty standard display mode, should work on most systems
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
        CConsole::getConsoleInstance().SetLoggingState(PR00FsReducedRenderingEngine::getLoggerModuleName(), false);    
    }

private:

    PR00FsReducedRenderingEngine* engine;

    // ---------------------------------------------------------------------------

    PR00FsReducedRenderingEngineTest2(const PR00FsReducedRenderingEngineTest2&)
    {};         

    PR00FsReducedRenderingEngineTest2& operator=(const PR00FsReducedRenderingEngineTest2&)
    {
        return *this;
    };

    bool testInitialize()
    {
        return assertFalse(engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0) == 0, "initialize()") &
            assertTrue(engine->isInitialized(), "isInitialized()");
    }

    bool testShutdown()
    {
        return assertTrue( engine->shutdown() );
    }

    bool testIsInitialized()
    {
        return assertTrue( engine->isInitialized() );
    }

    bool testGetScreen()
    {
        return assertTrue( engine->getScreen().isInitialized() );
    }

    bool testGetWindow()
    {
        return assertTrue( engine->getWindow().isInitialized() );
    }

    bool testGetHardwareInfo()
    {
        return assertTrue( engine->getHardwareInfo().isInitialized() );
    }

    bool testGetImageManager()
    {
        return assertNotNull( &engine->getImageManager() );
    }

    bool testGetTextureManager()
    {
        return assertNotNull( &engine->getTextureManager() );
    }

    bool testGetObject3DManager()
    {
        return assertNotNull( &engine->getObject3DManager() );
    }

    bool testGetCamera()
    {
        return assertNotNull( &engine->getCamera() );
    }

    bool testGetUImanager()
    {
        return assertTrue( engine->getUImanager().isInitialized() );
    }

    bool testGetRenderer()
    {
        return assertNotNull( engine->getRenderer() );
    }

    bool testCopyScreenToTexture()
    {
        PRREImage*    img = engine->getImageManager().createBlank(1024, 768, 24);
        PRRETexture * tex = engine->getTextureManager().createTextureFromImage( *img );

        // we just simply call this and expect nothing bad to happen
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
   
}; // class PR00FsReducedRenderingEngineTest2