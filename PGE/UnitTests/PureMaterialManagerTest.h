#pragma once

/*
    ###################################################################################
    PureMaterialManagerTest.h
    Unit test for PureMaterialManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGEallHeaders.h"
#include "../Pure/include/external/Material/PureMaterialManager.h"
#include "../Pure/include/external/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureMaterialManagerTest :
    public UnitTest
{
public:

    PureMaterialManagerTest() :
        UnitTest(__FILE__)
    {
        engine = NULL;
        im = NULL;
        tm = NULL;
        mm = NULL;
    }

    virtual ~PureMaterialManagerTest()
    {
        Finalize();
    } // ~PureMaterialManagerTest()

protected:

    virtual void Initialize()
    {
        /*
        CConsole::getConsoleInstance().SetLoggingState(PureMaterial::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureMaterialManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureImage::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureImageManager::getLoggerModuleName(), true);
        */
        engine = NULL;
        mm = NULL;
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PureMaterialManagerTest::testCtor1);
        AddSubTest("testCtor2", (PFNUNITSUBTEST) &PureMaterialManagerTest::testCtor2);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PureMaterialManagerTest::testDtor);
        AddSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PureMaterialManagerTest::testCreateFromFile);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PureMaterialManagerTest::testWriteList);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret &= assertEquals((TPureUInt)0, engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0), "engine" );  // pretty standard display mode, should work on most systems
            im = &engine->getImageManager();
            tm = &engine->getTextureManager();
            mm = &engine->getMaterialManager();
            ret &= assertNotNull(im, "im");
            ret &= assertNotNull(tm, "tm");
            ret &= assertNotNull(mm, "mm");
        }
        return ret;
    }

    virtual void TearDown()
    {
        Finalize();
        im = NULL;
        tm = NULL;
        mm = NULL;
    }

    virtual void Finalize()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        CConsole::getConsoleInstance().SetLoggingState(PureMaterial::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureMaterialManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureImage::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureImageManager::getLoggerModuleName(), false);
    }



private:
    PR00FsReducedRenderingEngine* engine;
    PureImageManager* im;
    PureTextureManager* tm;
    PureMaterialManager* mm;

    // ---------------------------------------------------------------------------

    PureMaterialManagerTest(const PureMaterialManagerTest&)
    {};         

    PureMaterialManagerTest& operator=(const PureMaterialManagerTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        mm = new PureMaterialManager();
        if ( !assertNotNull(mm, "mm not null"))
        {
            return false;
        }

        return assertNull(mm->createMaterial(), "createMaterial") &
            assertFalse(mm->isInitialized(), "isInit") &
            assertEquals((TPureUInt) 1, mm->getMaximumLayerCount(), "maxLayerCount");
    }

    bool testCtor2()
    {
        // HW specific but should pass on main test machine with at least 2 layers
        return assertNotNull(mm->createMaterial(), "createMaterial") &
            assertTrue(mm->isInitialized(), "isInit") &
            assertGreater(mm->getMaximumLayerCount(), (unsigned int) 1, "maxLayerCount");
    }

    bool testDtor()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return true;
    }

    bool testCreateFromFile()
    {
        return assertNull(mm->createFromFile("asd"));
    }

    bool testWriteList()
    {
        mm->WriteList();
        return true;
    }
    
}; // class PureMaterialManagerTest
