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
#include "../PRRE/Material/PRREMaterialManager.h"
#include "../PRRE/PR00FsReducedRenderingEngine.h"

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

    ~PRREMaterialManagerTest()
    {
        Finalize();
    } // ~PRREMaterialManagerTest()

protected:

    virtual void Initialize()
    {
        engine = NULL;
        mm = NULL;
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testCtor1);
        AddSubTest("testCtor2", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testCtor2);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testDtor);
        AddSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testCreateFromFile);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREMaterialManagerTest::testWriteList);
    }

    virtual void SetUp()
    {
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
            im = &engine->getImageManager();
            tm = &engine->getTextureManager();
            mm = &engine->getMaterialManager();
        }
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

        return assertNotNull(mm, "notNull") &
            assertNull(mm->createMaterial(), "createMaterial") &
            assertFalse(mm->isInitialized(), "isInit") &
            assertEquals((TPRREuint) 1, mm->getMaximumLayerCount(), "maxLayerCount");
    }

    bool testCtor2()
    {
        // HW specific but should pass on main test machine with at least 2 layers
        return assertNotNull(mm, "notNull") &
            assertNotNull(mm->createMaterial(), "createMaterial") &
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
        tm->WriteList();
        return true;
    }
    
}; // class PRREMaterialManagerTest
