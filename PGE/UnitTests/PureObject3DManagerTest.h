#pragma once

/*
    ###################################################################################
    PureObject3DManagerTest.h
    Unit test for PureObject3DManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Object3D/PureVertexTransferMode.h"
#include "../Pure/include/external/Object3D/PureObject3DManager.h"
#include "../Pure/include/external/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureObject3DManagerTest :
    public UnitTest
{
public:

    PureObject3DManagerTest() :
        UnitTest( __FILE__ )
    {
    };

    virtual ~PureObject3DManagerTest()
    {
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureVertexTransfer::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureObject3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureObject3D::getLoggerModuleName(), true);
        engine = NULL;
        om = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureObject3DManagerTest::testCtor);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PureObject3DManagerTest::testIsInitialized);
        
        // some functions from PureManager are overrid here
        AddSubTest("testAttachAndDetach", (PFNUNITSUBTEST) &PureObject3DManagerTest::testAttachAndDetach);
        AddSubTest("testDeleteAttachedInstance", (PFNUNITSUBTEST) &PureObject3DManagerTest::testDeleteAttachedInstance);

        // getOccluders() , get3dOpaqueOccludees(), etc. don't have their own test cases since they are
        // tested in almost all testcases here and in PureObject3DTest::testSetOccluder() too.
        AddSubTest("testCreatePlane", (PFNUNITSUBTEST) &PureObject3DManagerTest::testCreatePlane);
        AddSubTest("testCreateBox", (PFNUNITSUBTEST) &PureObject3DManagerTest::testCreateBox);
        AddSubTest("testCreateCube", (PFNUNITSUBTEST) &PureObject3DManagerTest::testCreateCube);
        AddSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PureObject3DManagerTest::testCreateFromFile);
        AddSubTest("testCreateCloned", (PFNUNITSUBTEST) &PureObject3DManagerTest::testCreateCloned);
        AddSubTest("testUpdateOccluderStates", (PFNUNITSUBTEST) &PureObject3DManagerTest::testUpdateOccluderStates);
        AddSubTest("testGetOccluderSelectionBias", (PFNUNITSUBTEST) &PureObject3DManagerTest::testGetOccluderSelectionBias);
        AddSubTest("testSetOccluderSelectionBias", (PFNUNITSUBTEST) &PureObject3DManagerTest::testSetOccluderSelectionBias);
        AddSubTest("testGetMaxOccluderCount", (PFNUNITSUBTEST) &PureObject3DManagerTest::testGetMaxOccluderCount);
        AddSubTest("testSetMaxOccluderCount", (PFNUNITSUBTEST) &PureObject3DManagerTest::testSetMaxOccluderCount);
        AddSubTest("testHandleManagedPropertyChanged", (PFNUNITSUBTEST) &PureObject3DManagerTest::testHandleManagedPropertyChanged);
        AddSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PureObject3DManagerTest::testGetUsedVideoMemory);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PureObject3DManagerTest::testWriteList);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret &= assertEquals((TPureuint)0, engine->initialize(Pure_RENDERER_HW_FP, 800, 600, Pure_WINDOWED, 0, 32, 24, 0, 0), "engine");  // pretty standard display mode, should work on most systems
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
        CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureVertexTransfer::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureObject3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureObject3D::getLoggerModuleName(), false);
    }


private:
    PR00FsReducedRenderingEngine* engine;
    PureObject3DManager* om;

    // ---------------------------------------------------------------------------

    PureObject3DManagerTest(const PureObject3DManagerTest&)
    {};         

    PureObject3DManagerTest& operator=(const PureObject3DManagerTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertNotNull(om) & assertTrue(om->getOccluders().empty(), "getOccluders empty") & assertTrue(om->get3dOpaqueOccludees().empty(), "getOpaqueOccludees empty");
    }

    bool testIsInitialized()
    {
        return assertTrue( om->isInitialized() );
    }

    bool testAttachAndDetach()
    {
        PureObject3D* const objBox = om->createBox(1.0f, 2.0f, 3.0f);
        PureObject3D* const objPlane = om->createPlane(1.0f, 2.0f);

        if ( !assertNotNull(objBox, "objBox not null") || !assertNotNull(objPlane, "objPlane not null"))
        {
            return false;
        }

        objBox->SetOcclusionTested(true);
        objPlane->SetOccluder(true);

        bool b = assertEquals((std::size_t)1, om->getOccluders().size(), "getOccluders not empty 1") & assertEquals((std::size_t)1, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees not empty 1")
            & assertEquals((std::size_t)0, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees empty 1") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 1")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 1");
        b &= assertTrue(objBox->isOcclusionTested(), "objBox occlusiontested 1") & assertTrue(objPlane->isOccluder(), "objPlane occluder 1");

        om->Detach(*objPlane);
        om->Detach(*objBox);

        b &= assertEquals((std::size_t)0, om->getOccluders().size(), "getOccluders empty 1") & assertEquals((std::size_t)0, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees empty 1")
            & assertEquals((std::size_t)0, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees empty 2") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 2")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 2");
        b &= assertFalse(objBox->isOcclusionTested(), "objBox not occlusiontested 1") & assertFalse(objPlane->isOccluder(), "objPlane not occluder 1");

        om->Attach(*objPlane);
        om->Attach(*objBox);

        b &= assertEquals((std::size_t)0, om->getOccluders().size(), "getOccluders empty 2") & assertEquals((std::size_t)2, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees not empty 2")
            & assertEquals((std::size_t)0, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees empty 3") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 3")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 3");
        b &= assertFalse(objBox->isOcclusionTested(), "objBox not occlusiontested 2") & assertFalse(objPlane->isOccluder(), "objPlane not occluder 2");

        objPlane->getMaterial(false).setBlendMode(Pure_BM_STANDARD_TRANSPARENCY);

        b &= assertEquals((std::size_t)0, om->getOccluders().size(), "getOccluders empty 3") & assertEquals((std::size_t)1, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees not empty 3")
            & assertEquals((std::size_t)1, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees not empty 1") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 4")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 4");

        om->Detach(*objPlane);

        b &= assertEquals((std::size_t)0, om->getOccluders().size(), "getOccluders empty 4") & assertEquals((std::size_t)1, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees not empty 4")
            & assertEquals((std::size_t)0, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees empty 4")
            & assertEquals(Pure_BM_NONE, objPlane->getMaterial(false).getBlendMode(), "setBlendMode is nonblend 1") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 5")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 5");

        om->Attach(*objPlane);

        b &= assertEquals((std::size_t)0, om->getOccluders().size(), "getOccluders empty 5") & assertEquals((std::size_t)2, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees not empty 5")
            & assertEquals((std::size_t)0, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees empty 5")
            & assertEquals(Pure_BM_NONE, objPlane->getMaterial(false).getBlendMode(), "setBlendMode is nonblend 2") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 6")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 6");

        objPlane->SetStickedToScreen(true);

        b &= assertEquals((std::size_t)0, om->getOccluders().size(), "getOccluders empty 6") & assertEquals((std::size_t)1, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees not empty 6")
            & assertEquals((std::size_t)0, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees empty 6")
            & assertEquals((std::size_t)1, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees NOT empty 7")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 7");

        om->Detach(*objPlane);

        b &= assertEquals((std::size_t)0, om->getOccluders().size(), "getOccluders empty 7") & assertEquals((std::size_t)1, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees not empty 7")
            & assertEquals((std::size_t)0, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees empty 7")
            & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 8")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 8");

        om->Attach(*objPlane);

        b &= assertEquals((std::size_t)0, om->getOccluders().size(), "getOccluders empty 8") & assertEquals((std::size_t)1, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees not empty 8")
            & assertEquals((std::size_t)0, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees empty 8")
            & assertEquals((std::size_t)1, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees NOT empty 9")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 9");

        return b;
    }

    bool testDeleteAttachedInstance()
    {
        PureObject3D* const objBox = om->createBox(1.0f, 2.0f, 3.0f);
        PureObject3D* const objPlane = om->createPlane(1.0f, 2.0f);
        PureObject3D* const objCube = om->createCube(1.0f);

        if ( !assertNotNull(objBox, "objBox not null") || !assertNotNull(objPlane, "objPlane not null") || !assertNotNull(objCube, "objCube not null"))
        {
            return false;
        }

        objBox->SetOcclusionTested(true);
        objPlane->SetOccluder(true);
        objCube->getMaterial(false).setBlendMode(Pure_BM_STANDARD_TRANSPARENCY);

        bool b = assertEquals((std::size_t)1, om->getOccluders().size(), "getOccluders not empty 1") & assertEquals((std::size_t)1, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees not empty 1")
            & assertEquals((std::size_t)1, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees not empty 1") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 1")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 1");
        b &= assertTrue(objBox->isOcclusionTested(), "objBox occlusiontested 1") & assertTrue(objPlane->isOccluder(), "objPlane occluder 1");

        om->DeleteAttachedInstance(*objBox);
        om->DeleteAttachedInstance(*objPlane);
        om->DeleteAttachedInstance(*objCube);

        b &= assertEquals((std::size_t)0, om->getOccluders().size(), "getOccluders empty 1") & assertEquals((std::size_t)0, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees empty 1") &
            assertEquals((std::size_t)0, om->get3dBlendedOccludees().size(), "get3dBlendedOccludees empty 1") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 2")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 2");

        return b;
    }

    bool testCreatePlane()
    {
        // we could check for other default properties such as vertex transfer mode, etc but for some reason we check them in Object3DTest instead
        const PureObject3D* const obj = om->createPlane(1.0f, 2.0f);

        if ( !assertNotNull(obj, "obj not null"))
        {
            return false;
        }

        return assertNotEquals(std::string::npos, obj->getName().find("Object3D "), "name substr") &
            assertEquals((TPureuint)4, obj->getVerticesCount(), "vertices count") &
            assertTrue(om->getOccluders().empty(), "getOccluders empty") & assertFalse(om->get3dOpaqueOccludees().empty(), "getOpaqueOccludees not empty") &
            assertTrue(om->get3dBlendedOccludees().empty(), "get3dBlendedOccludees empty") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 1")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 1");
    }

    bool testCreateBox()
    {
        // we could check for other default properties such as vertex transfer mode, etc but for some reason we check them in Object3DTest instead
        const PureObject3D* const obj = om->createBox(1.0f, 2.0f, 3.0f);

        if ( !assertNotNull(obj, "obj not null"))
        {
            return false;
        }

        return assertNotEquals(std::string::npos, obj->getName().find("Object3D "), "name substr") &
            assertEquals((TPureuint)24, obj->getVerticesCount(), "vertices count") &
            assertTrue(om->getOccluders().empty(), "getOccluders empty") & assertFalse(om->get3dOpaqueOccludees().empty(), "getOpaqueOccludees not empty") &
            assertTrue(om->get3dBlendedOccludees().empty(), "get3dBlendedOccludees empty") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 1")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 1");
    }

    bool testCreateCube()
    {
        // we could check for other default properties such as vertex transfer mode, etc but for some reason we check them in Object3DTest instead
        const PureObject3D* const obj = om->createCube(1.0f);

        if ( !assertNotNull(obj, "obj not null") )
        {
            return false;
        }

        return assertNotEquals(std::string::npos, obj->getName().find("Object3D "), "name substr") &
            assertEquals((TPureuint)24, obj->getVerticesCount(), "vertices count") &
            assertTrue(om->getOccluders().empty(), "getOccluders empty") & assertFalse(om->get3dOpaqueOccludees().empty(), "getOpaqueOccludees not empty") &
            assertTrue(om->get3dBlendedOccludees().empty(), "get3dBlendedOccludees empty") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 1")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 1");
    }

    bool testCreateFromFile()
    {
        // This snail object is from legacy proofps project, so its subobject names contain texture file names as well,
        // so that related textures can be auto-loaded. We can check here if number of textures has increased or not.
        // We expect textures to be auto-loaded as well, so increase in number of textures.
        const TPureint nTexturesBeforeLoad = engine->getTextureManager().getCount();

        const PureObject3D* const objFromFile = om->createFromFile("_res/models/snail_proofps/snail.obj");

        if ( !assertNotNull(objFromFile, "obj not null"))
        {
            return false;
        }

        // As texture filenames shouldn't be part of final subobject names, we can check for that also.
        bool b1 = true;
        for (TPureint i = 0; i < objFromFile->getCount(); i++)
            b1 = b1 & assertNull( strstr(((PureObject3D*)objFromFile->getAttachedAt(i))->getName().c_str(), ".bmp"), "bmp ext in subname" );

        return assertLess( nTexturesBeforeLoad, engine->getTextureManager().getCount(), "textures count") &
            assertNotEquals(std::string::npos, objFromFile->getName().find("Object3D "), "name substr") &
            assertEquals((TPureint)9, objFromFile->getCount(), "subobject count") &
            b1 &
            assertTrue(om->getOccluders().empty(), "getOccluders empty") & assertFalse(om->get3dOpaqueOccludees().empty(), "getOpaqueOccludees not empty") &
            assertTrue(om->get3dBlendedOccludees().empty(), "get3dBlendedOccludees empty") & assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 1")
            & assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 1");
    }

    bool testCreateCloned()
    {
        PureObject3D* const objPlane = om->createPlane(1.0f, 2.0f);
        objPlane->getPosVec().Set(1, 2, 3);
        objPlane->getAngleVec().Set(10, 20, 30);
        objPlane->SetName("stgsertseesg");
        objPlane->SetScaling( PureVector(4,5,6) );
        objPlane->SetDoubleSided(true);
        objPlane->SetAffectingZBuffer(false);
        objPlane->SetLit(false);
        objPlane->SetRotationOrder(TPure_ROTATION_ORDER::Pure_ZYX);
        objPlane->SetStickedToScreen(true);
        objPlane->setVertexTransferMode( PureVertexTransfer::selectVertexTransferMode(Pure_VMOD_STATIC, Pure_VREF_DIRECT, false) );
        objPlane->SetRenderingAllowed(false);
        objPlane->SetWireframed(true);
        objPlane->SetWireframedCulled(true);

        PureObject3D* const objCloned = om->createCloned( *objPlane );
        if ( !assertNotNull(objCloned, "objCloned not null"))
        {
            return assertNotNull(objCloned, "objCloned not NULL");
        }

        PureObject3D* const objFromFile = om->createFromFile("_res/models/robot_proofps/robot.obj");
        if ( !assertNotNull(objFromFile, "objFromFile not null"))
        {
            return assertNotNull(objFromFile, "objFromFile is NULL");
        }

        PureObject3D* const objFromFileCloned = om->createCloned(*objFromFile);
        if ( !objFromFileCloned )
        {
            return assertNotNull(objFromFileCloned, "objFromFileCloned is NULL");
        }

        PureObject3D* const objFromFileOther = om->createFromFile("_res/models/snail_proofps/snail.obj");
        if ( !assertNotNull(objFromFileOther, "objFromFileOther not null"))
        {
            return assertNotNull(objFromFileOther, "objFromFileOther is NULL");
        }

        // for this occludee, turn occlusion test off, so its cloned should create bounding box for its own!
        objFromFileOther->SetOcclusionTested(false);

        PureObject3D* const objFromFileOtherCloned = om->createCloned(*objFromFileOther);
        if ( !objFromFileOtherCloned )
        {
            return assertNotNull(objFromFileOtherCloned, "objFromFileOtherCloned is NULL");
        }

        // we need to set occlusion testing turned on explicitly since its referred is not tested for occlusion,
        // we expect here an implicit bounding box creation too
        objFromFileOtherCloned->SetOcclusionTested(true);

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
            assertFalse(objCloned->isRenderingAllowed(), "visible") &
            assertTrue(objCloned->isWireframed(), "wireframed") &
            assertTrue(objCloned->isWireframedCulled(), "wireframeculled") &
            assertNull(objCloned->getBoundingBoxObject(), "objCloned bounding box") &
            assertNull(objFromFileCloned->getBoundingBoxObject(), "objFromFileCloned bounding box, as it should use referred's box") &
            assertNotNull(objFromFileOtherCloned->getBoundingBoxObject(), "objFromFileOtherCloned bounding box, as it should have its own") &
            assertNotEquals(objFromFile->getBoundingBoxObject(), objFromFileCloned->getBoundingBoxObject(), "different bounding boxes") &
            assertTrue(objFromFileCloned->getRelPosVec() == objFromFile->getRelPosVec(), "objFromFileCloned rel pos") &
            assertEquals(objCloned->getBiggestAreaScaled(), objPlane->getBiggestAreaScaled(), "objCloned biggest area scaled") &
            assertEquals(objFromFileCloned->getBiggestAreaScaled(), objFromFile->getBiggestAreaScaled(), "objFromFileCloned biggest area scaled") &
            assertFalse(objFromFileCloned->isOccluded(), "objFromFileCloned isOccluded") &
            assertFalse(objFromFileCloned->isOccluder(), "objFromFileCloned isOccluder") &
            assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFileCloned) != om->get3dOpaqueOccludees().end(), "objFromFileCloned is in get3dOpaqueOccludees") &
            assertTrue( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFileCloned) == om->get3dBlendedOccludees().end(), "objFromFileCloned is NOT in get3dBlendedOccludees") &
            assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFileCloned) == om->getOccluders().end(), "objFromFileCloned is NOT in getOccluders") &
            assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFileCloned) == om->get2dOpaqueOccludees().end(), "objFromFileCloned is NOT in get2dOpaqueOccludees") &
            assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFileCloned) == om->get2dBlendedOccludees().end(), "objFromFileCloned is NOT in get2dBlendedOccludees") &
            assertTrue(objFromFileCloned->isOcclusionTested() == true, "objFromFileCloned isOcclusionTested") &
            assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objPlane) == om->get3dOpaqueOccludees().end(), "objPlane is NOT in get3dOpaqueOccludees") &
            assertTrue( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objPlane) == om->get3dBlendedOccludees().end(), "objPlane is NOT in get3dBlendedOccludees") &
            assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), objPlane) == om->getOccluders().end(), "objPlane is NOT in getOccluders") &
            assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objPlane) != om->get2dOpaqueOccludees().end(), "objPlane is in get2dOpaqueOccludees") &
            assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objPlane) == om->get2dBlendedOccludees().end(), "objPlane is NOT in get2dBlendedOccludees") &
            assertTrue(objCloned->isOcclusionTested() == false, "objPlane NOT isOcclusionTested") &
            assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objCloned) == om->get3dOpaqueOccludees().end(), "objCloned is NOT in get3dOpaqueOccludees") &
            assertTrue( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objCloned) == om->get3dBlendedOccludees().end(), "objCloned is NOT in get3dBlendedOccludees") &
            assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), objCloned) == om->getOccluders().end(), "objCloned is NOT in getOccluders") &
            assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objCloned) != om->get2dOpaqueOccludees().end(), "objCloned is in get2dOpaqueOccludees") &
            assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objCloned) == om->get2dBlendedOccludees().end(), "objCloned is NOT in get2dBlendedOccludees") &
            assertTrue(objCloned->isOcclusionTested() == false, "objCloned NOT isOcclusionTested");
    }

    bool testUpdateOccluderStates()
    {
        PureObject3D* const objFromFile = om->createFromFile("_res/models/snail_proofps/snail.obj");

        if ( !assertNotNull(objFromFile, "obj not null"))
        {
            return false;
        }

        std::vector<PureObject3D*> boxes;
        for (int i = 0; i < 100; i++)
        {
            boxes.push_back( om->createBox(1.0f, 1.0f, 1.0f) );
        }
        
        om->UpdateOccluderStates();
        
        bool b = true;
        for (std::size_t i = 0; i < boxes.size(); i++)
        {
            b &= assertFalse( boxes[i]->isOccluder(), (std::string("boxes[") + std::to_string(i) + "] isOccluder()").c_str() ) &
                 assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), boxes[i]) != om->get3dOpaqueOccludees().end(), (std::string("boxes[") + std::to_string(i) + "] is in getOpaqueOccludees").c_str()) &
                 assertTrue( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), boxes[i]) == om->get3dBlendedOccludees().end(), (std::string("boxes[") + std::to_string(i) + "] is NOT in get3dBlendedOccludees").c_str()) &
                 assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), boxes[i]) == om->getOccluders().end(), (std::string("boxes[") + std::to_string(i) + "] is NOT in getOccluders").c_str()) &
                 assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), boxes[i]) == om->get2dOpaqueOccludees().end(), (std::string("boxes[") + std::to_string(i) + "] is NOT in get2dOpaqueOccludees").c_str()) &
                 assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), boxes[i]) == om->get2dBlendedOccludees().end(), (std::string("boxes[") + std::to_string(i) + "] is NOT in get2dBlendedOccludees").c_str());
        } 

        // size of snail with default scaling is somewhere around [67, 47, 68] or something
        b &= assertTrue(objFromFile->isOccluder(), "objFromFile is occluder 1") &
             assertFalse( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 1") &
             assertFalse( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) != om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 1") &
             assertFalse( std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is in getOccluders 1") &
             assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 2") &
             assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 2") &
             assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 1") &
             assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 1");

        objFromFile->SetStickedToScreen(true);
        om->UpdateOccluderStates();
        b &= assertFalse(objFromFile->isOccluder(), "objFromFile not occluder 1") &
             assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 2") &
             assertFalse( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) != om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 2") &
             assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 2") &
             assertEquals((std::size_t)1, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees NOT empty 3") &
             assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 3") &
             assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) != om->get2dOpaqueOccludees().end(), "objFromFile is in get2dOpaqueOccludees 2") &
             assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 2");

        objFromFile->SetStickedToScreen(false);
        om->UpdateOccluderStates();
        b &= assertTrue(objFromFile->isOccluder(), "objFromFile is occluder 2") &
             assertFalse( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 3") &
             assertFalse( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) != om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 3") &
             assertFalse( std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is in getOccluders 3") &
             assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 4") &
             assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 4") &
             assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 3") &
             assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 3");

        objFromFile->getMaterial().setBlendMode(Pure_BM_STANDARD_TRANSPARENCY);
        om->UpdateOccluderStates();
        b &= assertFalse(objFromFile->isOccluder(), "objFromFile not occluder 2") &
             assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 4") &
             assertTrue( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) != om->get3dBlendedOccludees().end(), "objFromFile is in get3dBlendedOccludees 1") &
             assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 4") &
             assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 5") &
             assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 5") &
             assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 4") &
             assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 4");

        objFromFile->getMaterial().setBlendMode(Pure_BM_NONE);
        om->UpdateOccluderStates();
        b &= assertTrue(objFromFile->isOccluder(), "objFromFile is occluder 3") &
             assertFalse( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 5") &
             assertFalse( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) != om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 4") &
             assertFalse( std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is in getOccluders 5") &
             assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 6") &
             assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 6") &
             assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 5") &
             assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 5");

        // check if limiting max occluder count to 0 actually maked objFromFile as an occludee
        om->SetMaxOccluderCount(0);
        om->UpdateOccluderStates();

        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is occluder 4") &
             assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 6") &
             assertFalse( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) != om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 5") &
             assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 6") &
             assertEquals((std::size_t)0, om->get2dOpaqueOccludees().size(), "get2dOpaqueOccludees empty 7") &
             assertEquals((std::size_t)0, om->get2dBlendedOccludees().size(), "get2dBlendedOccludees empty 7") &
             assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 6") &
             assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 6");

        return b;
    }

    bool testGetOccluderSelectionBias()
    {
        return assertEquals(1.0f, om->getOccluderSelectionBias(), E);
    }

    bool testSetOccluderSelectionBias()
    {
        om->SetOccluderSelectionBias(20.0f);
        return assertEquals(20.0f, om->getOccluderSelectionBias(), E);
    }

    bool testGetMaxOccluderCount()
    {
        return assertEquals(4u, om->getMaxOccluderCount());
    }

    bool testSetMaxOccluderCount()
    {
        om->SetMaxOccluderCount(1);
        return assertEquals(1u, om->getMaxOccluderCount());
    }

    bool testHandleManagedPropertyChanged()
    {
        PureObject3D* const plane = om->createPlane(1.0f, 2.0f);
        if ( !assertNotNull(plane, "plane not null"))
        {
            return false;
        }

        plane->SetOccluder(true);

        bool b = assertTrue(plane->isOccluder(), "plane is occluder 1") &
             assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), plane) == om->get3dOpaqueOccludees().end(), "plane is NOT in getOpaqueOccludees 1") &
             assertTrue( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), plane) == om->get3dBlendedOccludees().end(), "plane is NOT in get3dBlendedOccludees 1") &
             assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), plane) != om->getOccluders().end(), "plane is in getOccluders 1") &
             assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), plane) == om->get2dOpaqueOccludees().end(), "plane is NOT in get2dOpaqueOccludees 1") &
             assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), plane) == om->get2dBlendedOccludees().end(), "plane is NOT in get2dBlendedOccludees 1");

        // remember we dont need the explicit subobject access, we want parent object's blending to be changed!
        plane->getMaterial(false).setBlendMode(Pure_BM_STANDARD_TRANSPARENCY);

        // Material's responsibility to invoke Object3DManager's HandleManagedPropertyChanged() when blendmode is change, however
        // it is Object3DManager's responsibility to set plane as its Material's utiliser. If that is set correctly, we assume
        // Material would invoke it. So, in this test we don't invoke HandleManagedPropertyChanged() explicitly but expect it
        // to be invoked my Material, and as a result, we expect plane not to be occluder anymore!

        b &= assertFalse(plane->isOccluder(), "plane is NOT occluder 2") &
             assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), plane) == om->get3dOpaqueOccludees().end(), "plane is NOT in getOpaqueOccludees 2") &
             assertTrue( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), plane) != om->get3dBlendedOccludees().end(), "plane is in get3dBlendedOccludees 2") &
             assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), plane) == om->getOccluders().end(), "plane is NOT in getOccluders 2") &
             assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), plane) == om->get2dOpaqueOccludees().end(), "plane is NOT in get2dOpaqueOccludees 2") &
             assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), plane) == om->get2dBlendedOccludees().end(), "plane is NOT in get2dBlendedOccludees 2");

        plane->getMaterial(false).setBlendMode(Pure_BM_NONE);

        // plane is not occluder even though it is opaque again, because only UpdateOccluderStates() would set it to occluder again, by calculating and comparing biggest area sizes!
        b &= assertFalse(plane->isOccluder(), "plane is NOT occluder 3") &
             assertTrue( std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), plane) != om->get3dOpaqueOccludees().end(), "plane is in getOpaqueOccludees 3") &
             assertTrue( std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), plane) == om->get3dBlendedOccludees().end(), "plane is NOT in get3dBlendedOccludees 3") &
             assertTrue( std::find(om->getOccluders().begin(), om->getOccluders().end(), plane) == om->getOccluders().end(), "plane is NOT in getOccluders 3") &
             assertTrue( std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), plane) == om->get2dOpaqueOccludees().end(), "plane is NOT in get2dOpaqueOccludees 3") &
             assertTrue( std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), plane) == om->get2dBlendedOccludees().end(), "plane is NOT in get2dBlendedOccludees 3");

        return b;
    }

    bool testGetUsedVideoMemory()
    {
        const PureObject3D* const plane = om->createPlane(1.0f, 2.0f);
        const PureObject3D* const box = om->createBox(1.0f, 2.0f, 3.0f);

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
   
}; // class PureObject3DManagerTest
