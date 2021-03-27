#pragma once

/*
    ###################################################################################
    PRREMaterialManagerTest.h
    Unit test for PRREMaterialManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGEallHeaders.h"
#include "../PRRE/include/external/Material/PRREMaterialManager.h"
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREMaterialManagerTest :
    public UnitTest
{
public:

    PRREMaterialManagerTest() :
        UnitTest(__FILE__)
    {
        engine = NULL;
        im = NULL;
        tm = NULL;
        mm = NULL;
    }

    virtual ~PRREMaterialManagerTest()
    {
        Finalize();
    } // ~PRREMaterialManagerTest()

protected:

    virtual void Initialize()
    {
        /*
        CConsole::getConsoleInstance().SetLoggingState(PRREMaterial::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PRREMaterialManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PRRETexture::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PRRETextureManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PRREImage::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PRREImageManager::getLoggerModuleName(), true);
        */
        engine = NULL;
        mm = NULL;
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testCtor1);
        AddSubTest("testCtor2", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testCtor2);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testDtor);
        AddSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testCreateFromFile);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testWriteList);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret &= assertEquals((TPRREuint)0, engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0), "engine" );  // pretty standard display mode, should work on most systems
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
        CConsole::getConsoleInstance().SetLoggingState(PRREMaterial::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREMaterialManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRRETexture::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRRETextureManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREImage::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREImageManager::getLoggerModuleName(), false);
    }



private:
    PR00FsReducedRenderingEngine* engine;
    PRREImageManager* im;
    PRRETextureManager* tm;
    PRREMaterialManager* mm;

    // ---------------------------------------------------------------------------

    PRREMaterialManagerTest(const PRREMaterialManagerTest&)
    {};         

    PRREMaterialManagerTest& operator=(const PRREMaterialManagerTest&)
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
        mm = new PRREMaterialManager();
        if ( !assertNotNull(mm, "mm not null"))
        {
            return false;
        }

        return assertNull(mm->createMaterial(), "createMaterial") &
            assertFalse(mm->isInitialized(), "isInit") &
            assertEquals((TPRREuint) 1, mm->getMaximumLayerCount(), "maxLayerCount");
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
    
}; // class PRREMaterialManagerTest
