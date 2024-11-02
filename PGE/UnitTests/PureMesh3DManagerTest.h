#pragma once

/*
    ###################################################################################
    PureMesh3DManagerTest.h
    Unit test for PureMesh3DManager.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Object3D/PureMesh3DManager.h"
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureMesh3DManagerTest :
    public UnitTest
{
public:

    PureMesh3DManagerTest() :
        UnitTest( __FILE__ )
    {
        
    };

    virtual ~PureMesh3DManagerTest()
    {
        
    }

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), true);
        engine = NULL;
        mm = NULL;
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureMesh3DManagerTest::testCtor);
        addSubTest("testIsInitialized", (PFNUNITSUBTEST) &PureMesh3DManagerTest::testIsInitialized);
        addSubTest("testIsMinimalIndexStorageEnabled", (PFNUNITSUBTEST) &PureMesh3DManagerTest::testIsMinimalIndexStorageEnabled);
        addSubTest("testSetMinimalIndexStorageEnabled", (PFNUNITSUBTEST) &PureMesh3DManagerTest::testSetMinimalIndexStorageEnabled);
        
        addSubTest("testCreatePlane", (PFNUNITSUBTEST) &PureMesh3DManagerTest::testCreatePlane);
        addSubTest("testCreateBox", (PFNUNITSUBTEST) &PureMesh3DManagerTest::testCreateBox);
        addSubTest("testCreateCube", (PFNUNITSUBTEST) &PureMesh3DManagerTest::testCreateCube);
        addSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PureMesh3DManagerTest::testCreateFromFile);
        addSubTest("testWriteList", (PFNUNITSUBTEST) &PureMesh3DManagerTest::testWriteList);
    }

    virtual bool setUp() override
    {
        bool ret = true;
        if ( engine == NULL )
        {
            PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
            engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
            ret &= assertEquals((TPureUInt)0, engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0), "engine");  // pretty standard display mode, should work on most systems
            mm = &engine->getMesh3DManager();
            ret &= assertNotNull(mm, "mm null");
        }
        return ret;
    }

    virtual void tearDown() override
    {
        finalize();
        mm = NULL;
    }

    virtual void finalize() override
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), false);
    }


private:
    PR00FsUltimateRenderingEngine* engine;
    PureMesh3DManager* mm;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PureMesh3DManagerTest(const PureMesh3DManagerTest&)
    {};         

    PureMesh3DManagerTest& operator=(const PureMesh3DManagerTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertNotNull(mm);
    }

    bool testIsInitialized()
    {
        return assertTrue( mm->isInitialized() );
    }

    bool testIsMinimalIndexStorageEnabled()
    {
        return assertTrue( mm->isMinimalIndexStorageEnabled() );
    }

    bool testSetMinimalIndexStorageEnabled()
    {
        mm->SetMinimalIndexStorageEnabled(false);
        return assertFalse( mm->isMinimalIndexStorageEnabled() );
    }

    bool testCreatePlane()
    {
        return assertNotNull( mm->createPlane(1.0f, 2.0f) );
    }

    bool testCreateBox()
    {
        return assertNotNull( mm->createBox(1.0f, 2.0f, 3.0f) );
    }

    bool testCreateCube()
    {
        return assertNotNull( mm->createCube(1.0f) );
    }

    bool testCreateFromFile()
    {
        // This snail object is from legacy proofps project, so its subobject names contain texture file names as well,
        // so that related textures can be auto-loaded. We do NOT expect texture increase here, because Mesh stuff
        // doesn't care about textures. Object3DManager may care about autoloading them.
        const TPureInt nTexturesBeforeLoad = engine->getTextureManager().getCount();

        const PureMesh3D* const obj = mm->createFromFile("_res/models/snail_proofps/snail.obj");

        return assertNotNull( obj, "not null" ) &
            assertEquals( nTexturesBeforeLoad, engine->getTextureManager().getCount(), "textures count");
    }

    bool testWriteList()
    {
        mm->WriteList();
        return true;
    }
   
}; // class PureMesh3DManagerTest
