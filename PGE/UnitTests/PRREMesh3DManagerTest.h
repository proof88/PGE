#pragma once

/*
    ###################################################################################
    PRREMesh3DManagerTest.h
    Unit test for PRREMesh3DManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/Object3D/PRREMesh3DManager.h"
#include "../PRRE/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREMesh3DManagerTest :
    public UnitTest
{
public:

    PRREMesh3DManagerTest() :
        UnitTest( __FILE__ )
    {};

protected:

    virtual void Initialize()
    {
        engine = NULL;
        mm = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREMesh3DManagerTest::testCtor);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PRREMesh3DManagerTest::testIsInitialized);
        AddSubTest("testIsMinimalIndexStorageEnabled", (PFNUNITSUBTEST) &PRREMesh3DManagerTest::testIsMinimalIndexStorageEnabled);
        AddSubTest("testSetMinimalIndexStorageEnabled", (PFNUNITSUBTEST) &PRREMesh3DManagerTest::testSetMinimalIndexStorageEnabled);
        
        AddSubTest("testCreatePlane", (PFNUNITSUBTEST) &PRREMesh3DManagerTest::testCreatePlane);
        AddSubTest("testCreateBox", (PFNUNITSUBTEST) &PRREMesh3DManagerTest::testCreateBox);
        AddSubTest("testCreateCube", (PFNUNITSUBTEST) &PRREMesh3DManagerTest::testCreateCube);
        AddSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PRREMesh3DManagerTest::testCreateFromFile);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREMesh3DManagerTest::testWriteList);
    }

    virtual void SetUp()
    {
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
            mm = &engine->getMesh3DManager();
        }
        
    }

    virtual void TearDown()
    {
        Finalize();
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
    PRREMesh3DManager* mm;

    // ---------------------------------------------------------------------------

    PRREMesh3DManagerTest(const PRREMesh3DManagerTest&)
    {};         

    PRREMesh3DManagerTest& operator=(const PRREMesh3DManagerTest&)
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
        const TPRREint nTexturesBeforeLoad = engine->getTextureManager().getCount();

        const PRREMesh3D* const obj = mm->createFromFile("_res/models/snail_proofps/snail.obj");

        return assertNotNull( obj, "not null" ) &
            assertEquals( nTexturesBeforeLoad, engine->getTextureManager().getCount(), "textures count");
    }

    bool testWriteList()
    {
        mm->WriteList();
        return true;
    }
   
}; // class PRREMesh3DManagerTest
