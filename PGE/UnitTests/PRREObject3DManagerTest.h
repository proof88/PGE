#pragma once

/*
    ###################################################################################
    PRREObject3DManagerTest.h
    Unit test for PRREObject3DManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/Object3D/PRREVertexTransferMode.h"
#include "../PRRE/include/external/Object3D/PRREObject3DManager.h"
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREObject3DManagerTest :
    public UnitTest
{
public:

    PRREObject3DManagerTest() :
        UnitTest( __FILE__ )
    {
    };

    virtual ~PRREObject3DManagerTest()
    {
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PRREMesh3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREMesh3D::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREVertexTransfer::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREObject3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREObject3D::getLoggerModuleName(), true);
        engine = NULL;
        om = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCtor);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testIsInitialized);
        AddSubTest("testCreatePlane", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreatePlane);
        AddSubTest("testCreateBox", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreateBox);
        AddSubTest("testCreateCube", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreateCube);
        AddSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreateFromFile);
        AddSubTest("testCreateCloned", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreateCloned);
        AddSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testGetUsedVideoMemory);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testWriteList);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret &= assertEquals((TPRREuint)0, engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0), "engine");  // pretty standard display mode, should work on most systems
            om = &engine->getObject3DManager();
            ret &= assertNotNull(om, "om null");
        }
        return ret;
    }

    virtual void TearDown()
    {
        Finalize();
        om = NULL;
    }

    virtual void Finalize()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        CConsole::getConsoleInstance().SetLoggingState(PRREMesh3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREMesh3D::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREVertexTransfer::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREObject3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREObject3D::getLoggerModuleName(), false);
    }


private:
    PR00FsReducedRenderingEngine* engine;
    PRREObject3DManager* om;

    // ---------------------------------------------------------------------------

    PRREObject3DManagerTest(const PRREObject3DManagerTest&)
    {};         

    PRREObject3DManagerTest& operator=(const PRREObject3DManagerTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertNotNull(om);
    }

    bool testIsInitialized()
    {
        return assertTrue( om->isInitialized() );
    }

    bool testCreatePlane()
    {
        // we could check for other default properties such as vertex transfer mode, etc but for some reason we check them in Object3DTest instead
        const PRREObject3D* const obj = om->createPlane(1.0f, 2.0f);

        if ( !assertNotNull(obj, "obj not null"))
        {
            return false;
        }

        return assertNotEquals(std::string::npos, obj->getName().find("Object3D "), "name substr") &
            assertEquals((TPRREuint)4, obj->getVerticesCount(), "vertices count") ;
    }

    bool testCreateBox()
    {
        // we could check for other default properties such as vertex transfer mode, etc but for some reason we check them in Object3DTest instead
        const PRREObject3D* const obj = om->createBox(1.0f, 2.0f, 3.0f);

        if ( !assertNotNull(obj, "obj not null"))
        {
            return false;
        }

        return assertNotEquals(std::string::npos, obj->getName().find("Object3D "), "name substr") &
            assertEquals((TPRREuint)24, obj->getVerticesCount(), "vertices count") ;
    }

    bool testCreateCube()
    {
        // we could check for other default properties such as vertex transfer mode, etc but for some reason we check them in Object3DTest instead
        const PRREObject3D* const obj = om->createCube(1.0f);

        if ( !assertNotNull(obj, "obj not null") )
        {
            return false;
        }

        return assertNotEquals(std::string::npos, obj->getName().find("Object3D "), "name substr") &
            assertEquals((TPRREuint)24, obj->getVerticesCount(), "vertices count") ;
    }

    bool testCreateFromFile()
    {
        // This snail object is from legacy proofps project, so its subobject names contain texture file names as well,
        // so that related textures can be auto-loaded. We can check here if number of textures has increased or not.
        // We expect textures to be auto-loaded as well, so increase in number of textures.
        const TPRREint nTexturesBeforeLoad = engine->getTextureManager().getCount();

        const PRREObject3D* const obj = om->createFromFile("_res/models/snail_proofps/snail.obj");

        if ( !assertNotNull(obj, "obj not null"))
        {
            return false;
        }

        // As texture filenames shouldn't be part of final subobject names, we can check for that also.
        bool b1 = true;
        for (TPRREint i = 0; i < obj->getCount(); i++)
            b1 = b1 & assertNull( strstr(((PRREObject3D*)obj->getAttachedAt(i))->getName().c_str(), ".bmp"), "bmp ext in subname" );

        return assertLess( nTexturesBeforeLoad, engine->getTextureManager().getCount(), "textures count") &
            assertNotEquals(std::string::npos, obj->getName().find("Object3D "), "name substr") &
            assertEquals((TPRREint)9, obj->getCount(), "subobject count") &
            b1;
    }

    bool testCreateCloned()
    {
        PRREObject3D* const objPlane = om->createPlane(1.0f, 2.0f);
        objPlane->getPosVec().Set(1, 2, 3);
        objPlane->getAngleVec().Set(10, 20, 30);
        objPlane->SetName("stgsertseesg");
        objPlane->SetScaling( PRREVector(4,5,6) );
        objPlane->SetDoubleSided(true);
        objPlane->SetAffectingZBuffer(false);
        objPlane->SetLit(false);
        objPlane->SetRotationOrder(TPRRE_ROTATION_ORDER::PRRE_ZYX);
        objPlane->SetStickedToScreen(true);
        objPlane->setVertexTransferMode( PRREVertexTransfer::selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_DIRECT, false) );
        objPlane->SetVisible(false);
        objPlane->SetWireframed(true);
        objPlane->SetWireframedCulled(true);

        PRREObject3D* const objCloned = om->createCloned( *objPlane );
        if ( !assertNotNull(objCloned, "objCloned not null"))
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }

        PRREObject3D* const objFromFile = om->createFromFile("_res/models/robot_proofps/robot.obj");
        if ( !assertNotNull(objFromFile, "objFromFile not null"))
        {
            return assertNotNull(objFromFile, "objFromFile is NULL");
        }

        PRREObject3D* const objFromFileCloned = om->createCloned(*objFromFile);
        if ( !objFromFileCloned )
        {
            return assertNotNull(objFromFileCloned, "objFromFileCloned is NULL");
        }

        const bool b = assertNotEquals(objPlane->getName(), objCloned->getName(), "name") &
            assertNotEquals(std::string::npos, objCloned->getName().find("clone of " + objPlane->getName()), "name substr");

        return assertTrue(objPlane->getPosVec() == objCloned->getPosVec(), "pos") &
            assertTrue(objPlane->getRelPosVec() == objCloned->getRelPosVec(), "rel pos") &
            assertTrue(objPlane->getAngleVec() == objCloned->getAngleVec(), "angle") &
            assertTrue(objPlane->getScaling() == objCloned->getScaling(), "scale") &
            b &
            assertTrue(objPlane->getSizeVec() == objCloned->getSizeVec(), "size") &
            assertEquals(objPlane->getRotationOrder(), objCloned->getRotationOrder(), "rot") &
            assertEquals(objPlane->getVertexTransferMode(), objCloned->getVertexTransferMode(), "vtrans") &
            assertTrue(objCloned->isDoubleSided(), "doublesided") &
            assertFalse(objCloned->isAffectingZBuffer(), "affectingzbuffer") &
            assertFalse(objCloned->isLit(), "islit") &
            assertTrue(objCloned->isStickedToScreen(), "stickedtoscreen") &
            assertFalse(objCloned->isVisible(), "visible") &
            assertTrue(objCloned->isWireframed(), "wireframed") &
            assertTrue(objCloned->isWireframedCulled(), "wireframeculled") &
            assertNull(objCloned->getBoundingBoxObject(), "objCloned bounding box") &
            assertNotNull(objFromFileCloned->getBoundingBoxObject(), "objCloned bounding box") &
            assertNotEquals(objFromFileCloned->getBoundingBoxObject(), objFromFile->getBoundingBoxObject(), "different bounding boxes") &
            assertTrue(objFromFileCloned->getRelPosVec() == objFromFile->getRelPosVec(), "objFromFile rel pos") &
            assertTrue(objFromFileCloned->isOccluded() == false, "objFromFile isOccluded") &
            assertTrue(objFromFileCloned->isOccluder() == false, "objFromFile isOccluder") &
            assertTrue(objFromFileCloned->isOcclusionTested() == true, "objFromFile isOcclusionTested");
    }

    bool testGetUsedVideoMemory()
    {
        const PRREObject3D* const plane = om->createPlane(1.0f, 2.0f);
        const PRREObject3D* const box = om->createBox(1.0f, 2.0f, 3.0f);

        if ( !assertNotNull(plane, "plane not null"))
        {
            return false;
        }

        if ( !assertNotNull(box, "box not null"))
        {
            return false;
        }

        return assertEquals(om->getUsedVideoMemory(), plane->getUsedVideoMemory() + box->getUsedVideoMemory());
    }

    bool testWriteList()
    {
        om->WriteList();
        return true;
    }
   
}; // class PRREObject3DManagerTest
