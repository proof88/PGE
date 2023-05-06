#pragma once

/*
    ###################################################################################
    PR00FsUltimateRenderingEngineTest2.h
    Unit test for PR00FsUltimateRenderingEngine.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"

class PR00FsUltimateRenderingEngineTest2 :
    public UnitTest
{
public:

    PR00FsUltimateRenderingEngineTest2() :
        UnitTest( __FILE__, "inited" ),
        cfgProfiles("")
    {
        
    }

    virtual ~PR00FsUltimateRenderingEngineTest2()
    {
        
    }

protected:

    virtual void Initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PR00FsUltimateRenderingEngine::getLoggerModuleName(), true);
        engine = NULL;
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testInitialize);
        AddSubTest("testShutdown", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testShutdown);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testIsInitialized);
        AddSubTest("testGetScreen", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetScreen);
        AddSubTest("testGetWindow", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetWindow);
        AddSubTest("testGetHardwareInfo", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetHardwareInfo);
        AddSubTest("testGetImageManager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetImageManager);
        AddSubTest("testGetTextureManager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetTextureManager);
        AddSubTest("testGetObject3DManager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetObject3DManager);
        AddSubTest("testGetCamera", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetCamera);
        AddSubTest("testGetUImanager", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetUImanager);
        AddSubTest("testGetRenderer", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetRenderer);
        AddSubTest("testCopyScreenToTexture", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testCopyScreenToTexture);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testWriteList);
        AddSubTest("testGetAutoWriteStatsAtShutdown", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testGetAutoWriteStatsAtShutdown);
        AddSubTest("testSetAutoWriteStatsAtShutdown", (PFNUNITSUBTEST) &PR00FsUltimateRenderingEngineTest2::testSetAutoWriteStatsAtShutdown);
    }

    virtual bool setUp() override
    {
        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);

        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        return assertEquals((TPureUInt) 0, engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0), "engine");  // pretty standard display mode, should work on most systems
    }

    virtual void TearDown() override
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
    }

    virtual void Finalize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(PR00FsUltimateRenderingEngine::getLoggerModuleName(), false);    
    }

private:

    PR00FsUltimateRenderingEngine* engine;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PR00FsUltimateRenderingEngineTest2(const PR00FsUltimateRenderingEngineTest2&) :
        cfgProfiles("")
    {};         

    PR00FsUltimateRenderingEngineTest2& operator=(const PR00FsUltimateRenderingEngineTest2&)
    {
        return *this;
    };

    bool testInitialize()
    {
        return assertFalse(engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0) == 0, "initialize()") &
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
        PureImage*    img = engine->getImageManager().createBlank(1024, 768, 24);
        PureTexture * tex = engine->getTextureManager().createTextureFromImage( *img );

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
   
}; // class PR00FsUltimateRenderingEngineTest2