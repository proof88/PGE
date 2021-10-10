#pragma once

/*
    ###################################################################################
    PRREImageTest.h
    Unit test for PRREObject3D.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"
#include "../PRRE/include/external/Object3D/PRREObject3DManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREObject3DTest :
    public UnitTest
{
public:

    PRREObject3DTest() :
        UnitTest( __FILE__ )
    {
        engine = NULL;
        obj = NULL;
        objFromFile = NULL;
        objPlane = NULL;
        objBox = NULL;
        objCube = NULL;
    }

    virtual ~PRREObject3DTest()
    {
        Finalize();
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PRREMesh3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREMesh3D::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREVertexTransfer::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREObject3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREObject3D::getLoggerModuleName(), true);
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        om = &engine->getObject3DManager();
        obj = NULL;
        objFromFile = NULL;
        objPlane = om->createPlane(1.0f, 2.0f);
        objBox   = om->createBox(1.0f, 2.0f, 3.0f);
        objCube  = om->createCube(1.0f);

        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREObject3DTest::testCtor);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PRREObject3DTest::testDtor);
        AddSubTest("testGetReferredObject", (PFNUNITSUBTEST) &PRREObject3DTest::testGetReferredObject_GetReferrerObjects);

        // Object3D contains some slight modifications in these functions compared to the original VertexTransfer functions
        AddSubTest("testGetVertexModifyingHabit", (PFNUNITSUBTEST) &PRREObject3DTest::testGetVertexModifyingHabit);
        AddSubTest("testSetVertexModifyingHabit", (PFNUNITSUBTEST) &PRREObject3DTest::testSetVertexModifyingHabit);
        AddSubTest("testGetVertexReferencingMode", (PFNUNITSUBTEST) &PRREObject3DTest::testGetVertexReferencingMode);
        AddSubTest("testSetVertexReferencingMode", (PFNUNITSUBTEST) &PRREObject3DTest::testSetVertexReferencingMode);
        AddSubTest("testGetVertexTransferMode", (PFNUNITSUBTEST) &PRREObject3DTest::testGetVertexTransferMode);
        AddSubTest("testSetVertexTransferMode", (PFNUNITSUBTEST) &PRREObject3DTest::testSetVertexTransferMode);

        AddSubTest("testGetTransformedVertices", (PFNUNITSUBTEST) &PRREObject3DTest::testGetTransformedVertices);
        AddSubTest("testGetAngleVec", (PFNUNITSUBTEST) &PRREObject3DTest::testGetAngleVec);
        AddSubTest("testGetScaledSizeVec", (PFNUNITSUBTEST) &PRREObject3DTest::testGetScaledSizeVec);
        AddSubTest("testGetScaling", (PFNUNITSUBTEST) &PRREObject3DTest::testGetScaling);
        AddSubTest("testSetScalingToScalar", (PFNUNITSUBTEST) &PRREObject3DTest::testSetScalingToScalar);
        AddSubTest("testSetScalingToVector", (PFNUNITSUBTEST) &PRREObject3DTest::testSetScalingToVector);
        AddSubTest("testScaleByScalar", (PFNUNITSUBTEST) &PRREObject3DTest::testScaleByScalar);
        AddSubTest("testScaleByVector", (PFNUNITSUBTEST) &PRREObject3DTest::testScaleByVector);
        AddSubTest("testGetBiggestAreaScaled", (PFNUNITSUBTEST) &PRREObject3DTest::testGetBiggestAreaScaled);
        AddSubTest("testRecalculateBiggerAreaScaled", (PFNUNITSUBTEST) &PRREObject3DTest::testRecalculateBiggerAreaScaled);
        AddSubTest("testIsVisible", (PFNUNITSUBTEST) &PRREObject3DTest::testIsVisible);
        AddSubTest("testSetVisible", (PFNUNITSUBTEST) &PRREObject3DTest::testSetVisible);
        AddSubTest("testShow", (PFNUNITSUBTEST) &PRREObject3DTest::testShow);
        AddSubTest("testHide", (PFNUNITSUBTEST) &PRREObject3DTest::testHide);
        AddSubTest("testIsColliding_TO_BE_REMOVED", (PFNUNITSUBTEST) &PRREObject3DTest::testIsColliding_TO_BE_REMOVED);
        AddSubTest("testSetColliding_TO_BE_REMOVED", (PFNUNITSUBTEST) &PRREObject3DTest::testSetColliding_TO_BE_REMOVED);
        AddSubTest("testGetRotationOrder", (PFNUNITSUBTEST) &PRREObject3DTest::testGetRotationOrder);
        AddSubTest("testSetRotationOrder", (PFNUNITSUBTEST) &PRREObject3DTest::testSetRotationOrder);
        AddSubTest("testIsLit", (PFNUNITSUBTEST) &PRREObject3DTest::testIsLit);
        AddSubTest("testSetLit", (PFNUNITSUBTEST) &PRREObject3DTest::testSetLit);
        AddSubTest("testIsDoubleSided", (PFNUNITSUBTEST) &PRREObject3DTest::testIsDoubleSided);
        AddSubTest("testSetDoubleSided", (PFNUNITSUBTEST) &PRREObject3DTest::testSetDoubleSided);
        AddSubTest("testIsWireframed", (PFNUNITSUBTEST) &PRREObject3DTest::testIsWireframed);
        AddSubTest("testSetWireframed", (PFNUNITSUBTEST) &PRREObject3DTest::testSetWireframed);
        AddSubTest("testIsWireframedCulled", (PFNUNITSUBTEST) &PRREObject3DTest::testIsWireframedCulled);
        AddSubTest("testSetWireframedCulled", (PFNUNITSUBTEST) &PRREObject3DTest::testSetWireframedCulled);
        AddSubTest("testIsAffectingZBuffer", (PFNUNITSUBTEST) &PRREObject3DTest::testIsAffectingZBuffer);
        AddSubTest("testSetAffectingZBuffer", (PFNUNITSUBTEST) &PRREObject3DTest::testSetAffectingZBuffer);
        AddSubTest("testIsTestingAgainstZBuffer", (PFNUNITSUBTEST) &PRREObject3DTest::testIsTestingAgainstZBuffer);
        AddSubTest("testSetTestingAgainstZBuffer", (PFNUNITSUBTEST) &PRREObject3DTest::testSetTestingAgainstZBuffer);
        AddSubTest("testIsStickedToScreen", (PFNUNITSUBTEST) &PRREObject3DTest::testIsStickedToScreen);
        AddSubTest("testSetStickedToScreen", (PFNUNITSUBTEST) &PRREObject3DTest::testSetStickedToScreen);
        AddSubTest("testIsOccluder", (PFNUNITSUBTEST) &PRREObject3DTest::testIsOccluder);
        AddSubTest("testSetOccluder", (PFNUNITSUBTEST) &PRREObject3DTest::testSetOccluder);
        AddSubTest("testIsOccluded", (PFNUNITSUBTEST) &PRREObject3DTest::testIsOccluded);
        AddSubTest("testIsOcclusionTested", (PFNUNITSUBTEST) &PRREObject3DTest::testIsOcclusionTested);
        AddSubTest("testSetOcclusionTested", (PFNUNITSUBTEST) &PRREObject3DTest::testSetOcclusionTested);
        AddSubTest("testGetBoundingBoxObject", (PFNUNITSUBTEST) &PRREObject3DTest::testGetBoundingBoxObject);
        AddSubTest("testDraw", (PFNUNITSUBTEST) &PRREObject3DTest::testDraw);
        AddSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PRREObject3DTest::testGetUsedSystemMemory);
        AddSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PRREObject3DTest::testGetUsedVideoMemory);

        // since Object3D became a Manager also, we should test these capabalities here as well for possible changed behavior
        AddSubTest("testGetCount", (PFNUNITSUBTEST) &PRREObject3DTest::testGetCount);
        AddSubTest("testIsEmpty", (PFNUNITSUBTEST) &PRREObject3DTest::testIsEmpty);
        AddSubTest("testGetSize", (PFNUNITSUBTEST) &PRREObject3DTest::testGetSize);
        AddSubTest("testGetAttachedAt", (PFNUNITSUBTEST) &PRREObject3DTest::testGetAttachedAt);
        AddSubTest("testPreAlloc", (PFNUNITSUBTEST) &PRREObject3DTest::testPreAlloc);
        AddSubTest("testHasAttached2", (PFNUNITSUBTEST) &PRREObject3DTest::testHasAttached2);
        AddSubTest("testAttach", (PFNUNITSUBTEST) &PRREObject3DTest::testAttach);
        AddSubTest("testDetach", (PFNUNITSUBTEST) &PRREObject3DTest::testDetach);
        AddSubTest("testDeleteAttachedInstance", (PFNUNITSUBTEST) &PRREObject3DTest::testDeleteAttachedInstance);
        AddSubTest("testDeleteAll", (PFNUNITSUBTEST) &PRREObject3DTest::testDeleteAll);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREObject3DTest::testWriteList);

        // since DetachFrom()'s behavior depends on managers' Detach() behavior, and that is overridden in Object3DManager, we test DetachFrom() too!
        AddSubTest("testDetachFrom", (PFNUNITSUBTEST) &PRREObject3DTest::testDetachFrom);
    }

    virtual bool setUp()
    {
        obj = om->createBox(1.0f, 2.0f, 3.0f);
        objFromFile = om->createFromFile("_res/models/snail_proofps/snail.obj");
        return assertNotNull(obj, "obj null") &
            assertNotNull(objFromFile, "objFromFile null") &
            assertNotNull(objPlane, "objPlane null") &
            assertNotNull(objCube, "objCube null") &
            assertNotNull(objBox, "objBox null");
    }

    virtual void TearDown()
    {
        if ( obj )
        {
            delete obj;
            obj = NULL;
        }
        if ( objFromFile )
        {
            delete objFromFile;
            objFromFile = NULL;
        }
    }

    virtual void Finalize()
    {
        obj = NULL;
        objFromFile = NULL;
        om = NULL;

        // engine shutdown will take care of these
        objBox = NULL;
        objPlane = NULL;
        objCube = NULL;

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
    PRREObject3D* obj;
    PRREObject3D* objFromFile;
    const PRREObject3D* objPlane,
                      * objBox,
                      * objCube;

    // ---------------------------------------------------------------------------

    PRREObject3DTest(const PRREObject3DTest&)
    {};         

    PRREObject3DTest& operator=(const PRREObject3DTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return true;
    }

    bool testDtor()
    {
        const TPRREint objCount = om->getCount();

        delete obj;
        obj = NULL;

        delete objFromFile;
        objFromFile = NULL;

        // dtor of Object3D is also responsible for removing itself from its manager
        return assertEquals(objCount-2, om->getCount(), "getCount 2nd");
    }

    bool testGetReferredObject_GetReferrerObjects()
    {
        PRREObject3D* const objCloned = om->createCloned(*obj);
        if ( !objCloned )
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }

        PRREObject3D* const objFromFileCloned = om->createCloned(*objFromFile);
        if ( !objFromFileCloned )
        {
            return assertNotNull(objFromFileCloned, "objFromFileCloned is NULL");
        }

        bool b = true;
        if ( obj->getReferrerObjects().empty() )
        {
            return assertTrue(false, "obj->getReferrerObjects().empty()");
        }
        else
        {
            b &= assertEquals(objCloned, *(obj->getReferrerObjects().begin()), "objCloned is on obj->getReferrerObjects()");
        }

        if ( objFromFile->getReferrerObjects().empty() )
        {
            return assertTrue(false, "objFromFile->getReferrerObjects().empty()");
        }
        else
        {
            b &= assertEquals(objFromFileCloned, *(objFromFile->getReferrerObjects().begin()), "objFromFileCloned is on objFromFile->getReferrerObjects()");
        }

        b &= assertNull(obj->getReferredObject(), "obj") &
            assertNull(objFromFile->getReferredObject(), "obj") &
            assertNull(objPlane->getReferredObject(), "plane") &
            assertNull(objBox->getReferredObject(), "box") &
            assertNull(objCube->getReferredObject(), "cube") &
            assertEquals(obj, objCloned->getReferredObject(), "refobjEquals") &
            assertEquals(objFromFile, objFromFileCloned->getReferredObject(), "refobjFromFileEquals");

        delete objCloned;
        b &= assertTrue(obj->getReferrerObjects().empty(), "obj->getReferrerObjects().empty() after deleting cloned");

        delete objFromFile;
        objFromFile = PGENULL;        
        b &= assertEquals((PRREObject3D*)PGENULL, objFromFileCloned->getReferredObject(), "objFromFileCloned->getReferredObject() NULL after deleting referred");

        return b;
    }

    bool testGetVertexModifyingHabit()
    {
        const TPRRE_VERTEX_MODIFYING_HABIT originalVertexModifyingHabit = obj->getVertexModifyingHabit();
        // if we change the modifying habit of the referred object, cloned object returns the updated vertex modifying habit of the referred object
        const PRREObject3D* const objCloned = om->createCloned( *obj );
        if ( !objCloned )
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }
        bool b = assertTrue(obj->setVertexModifyingHabit(PRRE_VMOD_DYNAMIC), "set");

        b &= assertEquals(PRRE_VMOD_STATIC, originalVertexModifyingHabit, "obj original") &
            assertEquals(PRRE_VMOD_DYNAMIC, obj->getVertexModifyingHabit(), "obj") &
            assertEquals(PRRE_VMOD_DYNAMIC, objCloned->getVertexModifyingHabit(), "objCloned") &
            assertEquals(PRRE_VMOD_STATIC, objFromFile->getVertexModifyingHabit(), "objFromFile") &
            assertEquals(PRRE_VMOD_STATIC, objPlane->getVertexModifyingHabit(), "plane") &
            assertEquals(PRRE_VMOD_STATIC, objBox->getVertexModifyingHabit(), "box") &
            assertEquals(PRRE_VMOD_STATIC, objCube->getVertexModifyingHabit(), "cube");

        delete objCloned;
        
        return b;
    }

    bool testSetVertexModifyingHabit()
    {
        bool b = assertTrue(objFromFile->setVertexModifyingHabit(PRRE_VMOD_DYNAMIC), "set objFromFile");
        b &= assertTrue(obj->setVertexModifyingHabit(PRRE_VMOD_DYNAMIC), "set obj");

        // subobjects must reject this when called by user
        PRREObject3D* const subobj1Obj = (PRREObject3D*)(obj->getAttachedAt(0));
        b &= assertFalse(subobj1Obj->setVertexModifyingHabit( PRRE_VMOD_STATIC ), "set subobj1Obj");

        PRREObject3D* const subobj1ObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        b &= assertFalse(subobj1ObjFromFile->setVertexModifyingHabit( PRRE_VMOD_STATIC ), "set subobj1FromFile");

        // cloned object must reject this in any case
        PRREObject3D* const objCloned = om->createCloned( *obj );
        if ( !objCloned )
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }
        b &= assertFalse(objCloned->setVertexModifyingHabit( PRRE_VMOD_STATIC ), "set objCloned");

        PRREObject3D* const objFromFileCloned = om->createCloned( *objFromFile );
        if ( !objFromFileCloned )
        {
            return assertNotNull(objFromFileCloned, "objFromFileCloned is NULL");
        }
        b &= assertFalse(objFromFileCloned->setVertexModifyingHabit( PRRE_VMOD_STATIC ), "set objFromFileCloned");

        b &= assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(obj->getVertexTransferMode()), "obj" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(objFromFile->getVertexTransferMode()), "objFromFile" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(subobj1Obj->getVertexTransferMode()), "subobj1Obj" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(subobj1ObjFromFile->getVertexTransferMode()), "subobj1ObjFromFile" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(objCloned->getVertexTransferMode()), "objCloned" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(objFromFileCloned->getVertexTransferMode()), "objFromFileCloned" );

        delete objCloned;
        delete objFromFileCloned;
        
        return b;
    }

    bool testGetVertexReferencingMode()
    {
        const TPRRE_VERTEX_REFERENCING_MODE originalVertexRefMode = obj->getVertexReferencingMode();
        // if we change the referencing mode of the referred object, cloned object returns the updated vertex referencing mode of the referred object
        const PRREObject3D* const objCloned = om->createCloned( *obj );
        if ( !objCloned )
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }
        bool b = assertTrue(obj->setVertexReferencingMode(PRRE_VREF_INDEXED), "set obj");

        b &= assertEquals(PRRE_VREF_DIRECT, originalVertexRefMode, "obj original") &
            assertEquals(PRRE_VREF_INDEXED, obj->getVertexReferencingMode(), "obj") &
            assertEquals(PRRE_VREF_INDEXED, objCloned->getVertexReferencingMode(), "objCloned") &
            assertEquals(PRRE_VREF_INDEXED, objFromFile->getVertexReferencingMode(), "objFromFile") &
            assertEquals(PRRE_VREF_DIRECT, objPlane->getVertexReferencingMode(), "plane") &
            assertEquals(PRRE_VREF_DIRECT, objBox->getVertexReferencingMode(), "box") &
            assertEquals(PRRE_VREF_DIRECT, objCube->getVertexReferencingMode(), "cube");

        delete objCloned;

        return b;
    }

    bool testSetVertexReferencingMode()
    {
        bool b = assertTrue(obj->setVertexReferencingMode(PRRE_VREF_INDEXED), "set obj");
        b &= assertTrue(objFromFile->setVertexReferencingMode(PRRE_VREF_DIRECT), "set objFromFile");

        // subobjects must reject this when called by user
        PRREObject3D* const subobj1Obj = (PRREObject3D*)(obj->getAttachedAt(0));
        b &= assertFalse(subobj1Obj->setVertexReferencingMode( PRRE_VREF_DIRECT ), "set subobj1Obj");

        PRREObject3D* const subobj1ObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        b &= assertFalse(subobj1ObjFromFile->setVertexReferencingMode( PRRE_VREF_INDEXED ), "set subobj1ObjFromFile");

        // cloned object must reject this in any case
        PRREObject3D* const objCloned = om->createCloned( *obj );
        if ( !objCloned )
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }
        b &= assertFalse(objCloned->setVertexReferencingMode( PRRE_VREF_DIRECT ), "set objCloned");

        PRREObject3D* const objFromFileCloned = om->createCloned( *objFromFile );
        if ( !objFromFileCloned )
        {
            return assertNotNull(objFromFileCloned, "objFromFileCloned is NULL");
        }
        b &= assertFalse(objFromFileCloned->setVertexReferencingMode( PRRE_VREF_DIRECT ), "set objFromFileCloned");

        b &= assertTrue( PRREVertexTransfer::isVertexReferencingIndexed(obj->getVertexTransferMode()), "obj" ) &
            assertFalse( PRREVertexTransfer::isVertexReferencingIndexed(objFromFile->getVertexTransferMode()), "objFromFile" ) &
            assertTrue( PRREVertexTransfer::isVertexReferencingIndexed(subobj1Obj->getVertexTransferMode()), "subobj1Obj" ) &
            assertFalse( PRREVertexTransfer::isVertexReferencingIndexed(subobj1ObjFromFile->getVertexTransferMode()), "subobj1ObjFromFile" ) &
            assertTrue( PRREVertexTransfer::isVertexReferencingIndexed(objCloned->getVertexTransferMode()), "objCloned" ) &
            assertFalse( PRREVertexTransfer::isVertexReferencingIndexed(objFromFileCloned->getVertexTransferMode()), "objFromFileCloned" );

        delete objCloned;
        delete objFromFileCloned;

        return b;
    }

    bool testGetVertexTransferMode()
    {
        // Generic server-side vertex array should be selected, main test machine supports it
        const TPRRE_VERTEX_TRANSFER_MODE vtExpected = PRRE_VMOD_STATIC | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);

        // if we change the transfer mode of the referred object, cloned object returns the updated vertex transfer mode of the referred object
        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedForCloned = PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT;
        const PRREObject3D* const objCloned = om->createCloned( *obj );
        if ( !objCloned )
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }
        bool b = assertTrue(obj->setVertexTransferMode(vtExpectedForCloned), "set obj");

        b &= assertEquals(vtExpectedForCloned, obj->getVertexTransferMode() & vtExpectedForCloned, "obj 1") &
            assertEquals(0u, BITF_READ(obj->getVertexTransferMode(), PRRE_VT_VENDOR_BITS, 3), "obj 2") &
            assertEquals(vtExpectedForCloned, objCloned->getVertexTransferMode() & vtExpectedForCloned, "objCloned 1") &
            assertEquals(0u, BITF_READ(objCloned->getVertexTransferMode(), PRRE_VT_VENDOR_BITS, 3), "objCloned 2") &
            assertEquals(vtExpected, objFromFile->getVertexTransferMode() & vtExpected, "objFromFile 1") &
            assertEquals(0u, BITF_READ(objFromFile->getVertexTransferMode(), PRRE_VT_VENDOR_BITS, 3), "objFromFile 2") &
            assertEquals(vtExpected, objPlane->getVertexTransferMode() & vtExpected, "plane 1") &
            assertEquals(0u, BITF_READ(objPlane->getVertexTransferMode(), PRRE_VT_VENDOR_BITS, 3), "plane 2") &
            assertEquals(vtExpected, objBox->getVertexTransferMode() & vtExpected, "box 1") &
            assertEquals(0u, BITF_READ(objBox->getVertexTransferMode(), PRRE_VT_VENDOR_BITS, 3), "box 2") &
            assertEquals(vtExpected, objCube->getVertexTransferMode() & vtExpected, "cube 1") &
            assertEquals(0u, BITF_READ(objCube->getVertexTransferMode(), PRRE_VT_VENDOR_BITS, 3), "cube 2");

        delete objCloned;

        return b;
    }

    bool testSetVertexTransferMode()
    {
        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedObj = obj->getVertexTransferMode();
        bool b = assertTrue(obj->setVertexTransferMode( obj->getVertexTransferMode() ), "set obj"); // intentionally testing setting to the same
        b &= assertEquals(vtExpectedObj, obj->getVertexTransferMode(), "sva obj 1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedObjFromFile = objFromFile->getVertexTransferMode();
        b &= assertTrue(objFromFile->setVertexTransferMode( objFromFile->getVertexTransferMode() ), "set objFromFile"); // intentionally testing setting to the same
        b &= assertEquals(vtExpectedObjFromFile, objFromFile->getVertexTransferMode(), "sva objFromFile 1");

        // make sure the mode of the 2 objects is not just simple (PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT) (1by1 immediate mode)
        b &= assertNotEquals(PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT, obj->getVertexTransferMode(), "obj not dir 1by1");
        b &= assertNotEquals(PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT, objFromFile->getVertexTransferMode(), "objFromFile not dir 1by1");

        // subobjects must reject this when called by user
        // by default the selected transfer mode is NOT PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT hence we try set that but expect no change!
        PRREObject3D* const subobj1Obj = (PRREObject3D*)(obj->getAttachedAt(0));
        b &= assertFalse(subobj1Obj->setVertexTransferMode( PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT ), "set subobj1Obj");
        b &= assertEquals(vtExpectedObj, subobj1Obj->getVertexTransferMode(), "dir obj subobject");

        PRREObject3D* const subobj1ObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        b &= assertFalse(subobj1ObjFromFile->setVertexTransferMode( PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT ), "set subobj1ObjFromFile");
        b &= assertEquals(vtExpectedObjFromFile, subobj1ObjFromFile->getVertexTransferMode(), "dir objFromFile subobject");

        // cloned object must reject this in any case
        // by default the selected transfer mode is NOT PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT hence we try set that but expect no change!
        PRREObject3D* const objCloned = om->createCloned( *obj );
        if ( !objCloned )
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }
        b &= assertFalse(objCloned->setVertexTransferMode( PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT ), "set objCloned");
        b &= assertEquals(vtExpectedObj, objCloned->getVertexTransferMode(), "dir obj cloned");

        PRREObject3D* const objFromFileCloned = om->createCloned( *objFromFile );
        if ( !objFromFileCloned )
        {
            return assertNotNull(objFromFileCloned, "objFromFileCloned is NULL");
        }
        b &= assertFalse(objFromFileCloned->setVertexTransferMode( PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT ), "set objFromFileCloned");
        b &= assertEquals(vtExpectedObjFromFile, objFromFileCloned->getVertexTransferMode(), "dir objFromFile cloned");

        // Generic server-side vertex arrays are supported by main test machine

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaIndSVAobj = PRRE_VMOD_STATIC  | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedStaIndSVAobj ), "set obj sva");
        b &= assertEquals(vtExpectedStaIndSVAobj, obj->getVertexTransferMode(), "sva obj 2");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaIndSVAobjFromFile = PRRE_VMOD_STATIC  | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedStaIndSVAobjFromFile ), "set objFromFile sva");
        b &= assertEquals(vtExpectedStaIndSVAobjFromFile, objFromFile->getVertexTransferMode(), "sva objFromFile 2");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynIndSVAobj = PRRE_VMOD_DYNAMIC  | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynIndSVAobj ), "set obj sva 3");
        b &= assertEquals(vtExpectedDynIndSVAobj, obj->getVertexTransferMode(), "sva obj 3");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynIndSVAobjFromFile = PRRE_VMOD_DYNAMIC  | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynIndSVAobjFromFile ), "set objFromFile sva 3");
        b &= assertEquals(vtExpectedDynIndSVAobjFromFile, objFromFile->getVertexTransferMode(), "sva objFromFile 3");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDirSVAobj = PRRE_VMOD_DYNAMIC  | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynDirSVAobj ), "set obj sva 4");
        b &= assertEquals(vtExpectedDynDirSVAobj, obj->getVertexTransferMode(), "sva obj 4");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDirSVAobjFromFile = PRRE_VMOD_DYNAMIC  | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynDirSVAobjFromFile ), "set objFromFile sva 4");
        b &= assertEquals(vtExpectedDynDirSVAobjFromFile, objFromFile->getVertexTransferMode(), "sva objFromFile 4");

        // following modes must be supported on every machine

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDir1by1obj = PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT;
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynDir1by1obj ), "set obj 1b1");
        b &= assertEquals(vtExpectedDynDir1by1obj, obj->getVertexTransferMode(), "dir obj 1b1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDir1by1objFromFile = PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT;
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynDir1by1objFromFile ), "set objFromFile 1b1");
        b &= assertEquals(vtExpectedDynDir1by1objFromFile, objFromFile->getVertexTransferMode(), "dir objFromFile 1b1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDirVAobj = PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynDirVAobj ), "set obj rva");
        b &= assertEquals(vtExpectedDynDirVAobj, obj->getVertexTransferMode(), "dir obj rva");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDirVAobjFromFile = PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynDirVAobjFromFile ), "set objFromFile rva");
        b &= assertEquals(vtExpectedDynDirVAobjFromFile, objFromFile->getVertexTransferMode(), "dir objFromFile rva");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynInd1by1obj = PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED;
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynInd1by1obj ), "set obj 1b1");
        b &= assertEquals(vtExpectedDynInd1by1obj, obj->getVertexTransferMode(), "ind obj 1b1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynInd1by1objFromFile = PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED;
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynInd1by1objFromFile ), "set objFromFile 1b1");
        b &= assertEquals(vtExpectedDynInd1by1objFromFile, objFromFile->getVertexTransferMode(), "ind objFromFile 1b1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynIndVAobj = PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynIndVAobj ), "set obj rva");
        b &= assertEquals(vtExpectedDynIndVAobj, obj->getVertexTransferMode(), "ind obj rva");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynIndVAobjFromFile = PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynIndVAobjFromFile ), "set objFromFile rv");
        b &= assertEquals(vtExpectedDynIndVAobjFromFile, objFromFile->getVertexTransferMode(), "ind objFromFile rva");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaDirDLobj = PRRE_VMOD_STATIC | PRRE_VREF_DIRECT;
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedStaDirDLobj ), "set obj DL");
        b &= assertEquals(vtExpectedStaDirDLobj, obj->getVertexTransferMode(), "dir obj DL");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaDirDLobjFromFile = PRRE_VMOD_STATIC | PRRE_VREF_DIRECT;
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedStaDirDLobjFromFile ), "set objFromFile DL");
        b &= assertEquals(vtExpectedStaDirDLobjFromFile, objFromFile->getVertexTransferMode(), "dir objFromFile DL");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaIndDLobj = PRRE_VMOD_STATIC | PRRE_VREF_INDEXED;
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedStaIndDLobj ), "set obj DL");
        b &= assertEquals(vtExpectedStaIndDLobj, obj->getVertexTransferMode(), "ind obj DL");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaIndDLobjFromFile = PRRE_VMOD_STATIC | PRRE_VREF_INDEXED;
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedStaIndDLobjFromFile ), "set objFromFile DL");
        b &= assertEquals(vtExpectedStaIndDLobjFromFile, objFromFile->getVertexTransferMode(), "ind objFromFile DL");

        delete objCloned;
        delete objFromFileCloned;

        return b;
    }

    bool testGetTransformedVertices()
    {
        return assertNotNull(objPlane->getTransformedVertices(), "plane") &
            assertNotNull(objBox->getTransformedVertices(), "box") &
            assertNotNull(objCube->getTransformedVertices(), "cube") &
            assertNull(objFromFile->getTransformedVertices(), "objFromFile") &
            assertEquals(((PRREObject3D*)objPlane->getAttachedAt(0))->getTransformedVertices(), objPlane->getTransformedVertices(), "plane 2") &
            assertEquals(((PRREObject3D*)objBox->getAttachedAt(0))->getTransformedVertices(), objBox->getTransformedVertices(), "box 2") &
            assertEquals(((PRREObject3D*)objCube->getAttachedAt(0))->getTransformedVertices(), objCube->getTransformedVertices(), "cube 2") &
            assertNotEquals(((PRREObject3D*)objFromFile->getAttachedAt(0))->getTransformedVertices(), objFromFile->getTransformedVertices(), "objFromFile 2") &
            assertNull(objPlane->getTransformedVertices(false), "plane noimplicit") &
            assertNull(objBox->getTransformedVertices(false), "box noimplicit") &
            assertNull(objCube->getTransformedVertices(false), "cube noimplicit") &
            assertNull(objFromFile->getTransformedVertices(false), "objFromFile noimplicit");
    }

    bool testGetAngleVec()
    {
        return assertEquals(0.0f, objPlane->getAngleVec().getX(), E, "plane x") &        
            assertEquals(0.0f, objPlane->getAngleVec().getY(), E, "plane y") &
            assertEquals(0.0f, objPlane->getAngleVec().getZ(), E, "plane z") &
            assertEquals(0.0f, objBox->getAngleVec().getX(), E, "box x") &        
            assertEquals(0.0f, objBox->getAngleVec().getY(), E, "box y") &
            assertEquals(0.0f, objBox->getAngleVec().getZ(), E, "box z") &
            assertEquals(0.0f, objCube->getAngleVec().getX(), E, "cube x") &        
            assertEquals(0.0f, objCube->getAngleVec().getY(), E, "cube y") &
            assertEquals(0.0f, objCube->getAngleVec().getZ(), E, "cube z") &
            assertEquals(0.0f, objFromFile->getAngleVec().getX(), E, "objFromFile x") &        
            assertEquals(0.0f, objFromFile->getAngleVec().getY(), E, "objFromFile y") &
            assertEquals(0.0f, objFromFile->getAngleVec().getZ(), E, "objFromFile z");
    }

    bool testGetScaledSizeVec()
    {
        obj->SetScaling(2.0f);
        objFromFile->SetScaling(3.0f);

        return assertEquals(1.0f, objPlane->getScaledSizeVec().getX(), E, "plane x") &        
            assertEquals(2.0f, objPlane->getScaledSizeVec().getY(), E, "plane y") &
            assertEquals(0.0f, objPlane->getScaledSizeVec().getZ(), E, "plane z") &
            assertEquals(1.0f, objBox->getScaledSizeVec().getX(), E, "box x") &        
            assertEquals(2.0f, objBox->getScaledSizeVec().getY(), E, "box y") &
            assertEquals(3.0f, objBox->getScaledSizeVec().getZ(), E, "box z") &
            assertEquals(1.0f, objCube->getScaledSizeVec().getX(), E, "cube x") &        
            assertEquals(1.0f, objCube->getScaledSizeVec().getY(), E, "cube y") &
            assertEquals(1.0f, objCube->getScaledSizeVec().getZ(), E, "cube z") &
            assertEquals(2.0f, obj->getScaledSizeVec().getX(), E, "obj x") &        
            assertEquals(4.0f, obj->getScaledSizeVec().getY(), E, "obj y") &
            assertEquals(6.0f, obj->getScaledSizeVec().getZ(), E, "obj z") &
            assertEquals(objFromFile->getSizeVec().getX() * objFromFile->getScaling().getX(), objFromFile->getScaledSizeVec().getX(), E, "objFromFile x") &        
            assertEquals(objFromFile->getSizeVec().getY() * objFromFile->getScaling().getY(), objFromFile->getScaledSizeVec().getY(), E, "objFromFile y") &
            assertEquals(objFromFile->getSizeVec().getZ() * objFromFile->getScaling().getZ(), objFromFile->getScaledSizeVec().getZ(), E, "objFromFile z");
    }

    bool testGetScaling()
    {
        const PRREVector vUnit(1.0f, 1.0f, 1.0f);

        const bool b1 = ( vUnit == objPlane->getScaling() );
        const bool b2 = ( vUnit == objBox->getScaling() );
        const bool b3 = ( vUnit == objCube->getScaling() );
        const bool b4 = ( vUnit == objFromFile->getScaling() );
        const bool b5 = objFromFile->getBoundingBoxObject() ? ( vUnit == objFromFile->getBoundingBoxObject()->getScaling() ) : false;

        return assertTrue(b1, "plane") &
            assertTrue(b2, "box") &
            assertTrue(b3, "cube") &
            assertTrue(b4, "objFromFile") &
            assertTrue(b5, "objFromFile bounding box");
    }

    bool testSetScalingToScalar()
    {
        const TPRREfloat fOriginalBiggestAreaScaledObj = obj->getBiggestAreaScaled();
        const TPRREfloat fOriginalBiggestAreaScaledObjFromFile = objFromFile->getBiggestAreaScaled();

        obj->SetScaling(3.0f);
        objFromFile->SetScaling(3.0f);

        bool b = true;
        if ( objFromFile->getBoundingBoxObject() )
        {
            b &= assertEquals(3.0f, objFromFile->getBoundingBoxObject()->getScaling().getX(), E, "X file bounding") &
            assertEquals(3.0f, objFromFile->getBoundingBoxObject()->getScaling().getY(), E, "Y file bounding") &
            assertEquals(3.0f, objFromFile->getBoundingBoxObject()->getScaling().getZ(), E, "Z file bounding");
        }
        else
        {
            b = false;
        }

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const PRREVector vOriginalSubobjectProperty = subObjFromFile->getScaling();
        subObjFromFile->SetScaling(2.0f);
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->getScaling(), "property unchanged for subobject");

        return assertEquals(3.0f, obj->getScaling().getX(), E, "X") &
            assertEquals(3.0f, obj->getScaling().getY(), E, "Y") &
            assertEquals(3.0f, obj->getScaling().getZ(), E, "Z") &
            assertEquals(3.0f, objFromFile->getScaling().getX(), E, "X file") &
            assertEquals(3.0f, objFromFile->getScaling().getY(), E, "Y file") &
            assertEquals(3.0f, objFromFile->getScaling().getZ(), E, "Z file") &
            assertEquals(fOriginalBiggestAreaScaledObj * 3.0f*3.0f, obj->getBiggestAreaScaled(), 0.01f, "area") &
            assertEquals(fOriginalBiggestAreaScaledObjFromFile * 3.0f*3.0f, objFromFile->getBiggestAreaScaled(), 0.01f, "area file") &
            assertTrue(b, "b");
    }

    bool testSetScalingToVector()
    {
        const PRREVector vNewScaling(2.0f, 3.0f, 4.0f);
        obj->SetScaling(vNewScaling);
        objFromFile->SetScaling(vNewScaling);

        bool b = true;
        if ( objFromFile->getBoundingBoxObject() )
        {
            b &= assertEquals(2.0f, objFromFile->getBoundingBoxObject()->getScaling().getX(), E, "X file bounding") &
            assertEquals(3.0f, objFromFile->getBoundingBoxObject()->getScaling().getY(), E, "Y file bounding") &
            assertEquals(4.0f, objFromFile->getBoundingBoxObject()->getScaling().getZ(), E, "Z file bounding");
        }
        else
        {
            b = false;
        }

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const PRREVector vOriginalSubobjectProperty = subObjFromFile->getScaling();
        subObjFromFile->SetScaling(PRREVector(1.0f, 1.0f, 1.0f));
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->getScaling(), "property unchanged for subobject");

        return assertEquals(2.0f, obj->getScaling().getX(), E, "X") &
            assertEquals(3.0f, obj->getScaling().getY(), E, "Y") &
            assertEquals(4.0f, obj->getScaling().getZ(), E, "Z") &
            assertEquals(2.0f, objFromFile->getScaling().getX(), E, "X file") &
            assertEquals(3.0f, objFromFile->getScaling().getY(), E, "Y file") &
            assertEquals(4.0f, objFromFile->getScaling().getZ(), E, "Z file") &
            /* because obj has size [1,2,3] and with scaling vector its YZ area will be biggest */
            assertEquals(obj->getSizeVec().getY() * 3.0f * obj->getSizeVec().getZ() * 4.0f, obj->getBiggestAreaScaled(), 0.01f, "area") &
            /* because objFromFile has size around ~[67, 47, 68] and with scaling vector its YZ area will be biggest, not its XZ */
            assertEquals(objFromFile->getSizeVec().getY() * 3.0f * objFromFile->getSizeVec().getZ() * 4.0f, objFromFile->getBiggestAreaScaled(), 0.01f, "area file") &
            assertTrue(b, "b");
    }

    bool testScaleByScalar()
    {
        const TPRREfloat fOriginalBiggestAreaScaledObj = obj->getBiggestAreaScaled();
        const TPRREfloat fOriginalBiggestAreaScaledObjFromFile = objFromFile->getBiggestAreaScaled();

        obj->Scale(3.0f);
        bool b = assertEquals(3.0f, obj->getScaling().getX(), E, "b X") &
            assertEquals(3.0f, obj->getScaling().getY(), E, "b Y") &
            assertEquals(3.0f, obj->getScaling().getZ(), E, "b Z") &
            assertEquals(fOriginalBiggestAreaScaledObj * 3.0f*3.0f, obj->getBiggestAreaScaled(), 0.01f, "area");

        obj->Scale(2.5f);
        b &= assertEquals(3.0f*2.5f, obj->getScaling().getX(), E, "b2 X") &
            assertEquals(3.0f*2.5f, obj->getScaling().getY(), E, "b2 Y") &
            assertEquals(3.0f*2.5f, obj->getScaling().getZ(), E, "b2 Z") &
            assertEquals(fOriginalBiggestAreaScaledObj * 3.0f*3.0f * 2.5f*2.5f, obj->getBiggestAreaScaled(), 0.1f, "area");;
        
        objFromFile->Scale(3.0f);
        b &= assertEquals(3.0f, objFromFile->getScaling().getX(), E, "b file X") &
            assertEquals(3.0f, objFromFile->getScaling().getY(), E, "b file Y") &
            assertEquals(3.0f, objFromFile->getScaling().getZ(), E, "b file Z") &
            assertEquals(fOriginalBiggestAreaScaledObjFromFile * 3.0f*3.0f, objFromFile->getBiggestAreaScaled(), 0.01f, "area file");

        objFromFile->Scale(2.5f);
        b &= assertEquals(3.0f*2.5f, objFromFile->getScaling().getX(), E, "b2 file X") &
            assertEquals(3.0f*2.5f, objFromFile->getScaling().getY(), E, "b2 file Y") &
            assertEquals(3.0f*2.5f, objFromFile->getScaling().getZ(), E, "b2 file Z") &
            assertEquals(fOriginalBiggestAreaScaledObjFromFile * 3.0f*3.0f * 2.5f*2.5f, objFromFile->getBiggestAreaScaled(), 0.1f, "area file");

        if ( objFromFile->getBoundingBoxObject() )
        {
            b &= assertEquals(3.0f*2.5f, objFromFile->getBoundingBoxObject()->getScaling().getX(), E, "X file bounding") &
            assertEquals(3.0f*2.5f, objFromFile->getBoundingBoxObject()->getScaling().getY(), E, "Y file bounding") &
            assertEquals(3.0f*2.5f, objFromFile->getBoundingBoxObject()->getScaling().getZ(), E, "Z file bounding");
        }
        else
        {
            b = false;
        }

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const PRREVector vOriginalSubobjectProperty = subObjFromFile->getScaling();
        subObjFromFile->Scale(2.0f);
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->getScaling(), "property unchanged for subobject");

        return b;
    }

    bool testScaleByVector()
    {
        const PRREVector vScaleBy(2.0f, 3.0f, 4.0f);

        obj->Scale(vScaleBy);
        bool b = assertEquals(2.0f, obj->getScaling().getX(), E, "b X") &
            assertEquals(3.0f, obj->getScaling().getY(), E, "b Y") &
            assertEquals(4.0f, obj->getScaling().getZ(), E, "b Z");

        obj->Scale(vScaleBy);
        b &= assertEquals(2.0f*2.0f, obj->getScaling().getX(), E, "b2 X") &
            assertEquals(3.0f*3.0f, obj->getScaling().getY(), E, "b2 Y") &
            assertEquals(4.0f*4.0f, obj->getScaling().getZ(), E, "b2 Z") &
            /* because obj has size [1,2,3] and with scaling vector its YZ area will be biggest */
            assertEquals(obj->getSizeVec().getY() * 3.0f * 3.0f * obj->getSizeVec().getZ() * 4.0f * 4.0f, obj->getBiggestAreaScaled(), 0.01f, "area");

        objFromFile->Scale(vScaleBy);
        b &= assertEquals(2.0f, objFromFile->getScaling().getX(), E, "b file X") &
            assertEquals(3.0f, objFromFile->getScaling().getY(), E, "b file Y") &
            assertEquals(4.0f, objFromFile->getScaling().getZ(), E, "b file Z");

        objFromFile->Scale(vScaleBy);
        b &= assertEquals(2.0f*2.0f, objFromFile->getScaling().getX(), E, "b2 file X") &
            assertEquals(3.0f*3.0f, objFromFile->getScaling().getY(), E, "b2 file Y") &
            assertEquals(4.0f*4.0f, objFromFile->getScaling().getZ(), E, "b2 file Z") &
            /* because objFromFile has size around ~[67, 47, 68] and with scaling vector its YZ area will be biggest, not its XZ */
            assertEquals(objFromFile->getSizeVec().getY() * 3.0f * 3.0f * objFromFile->getSizeVec().getZ() * 4.0f * 4.0f, objFromFile->getBiggestAreaScaled(), 0.1f, "area file");

        if ( objFromFile->getBoundingBoxObject() )
        {
            b &= assertEquals(2.0f*2.0f, objFromFile->getBoundingBoxObject()->getScaling().getX(), E, "X file bounding") &
            assertEquals(3.0f*3.0f, objFromFile->getBoundingBoxObject()->getScaling().getY(), E, "Y file bounding") &
            assertEquals(4.0f*4.0f, objFromFile->getBoundingBoxObject()->getScaling().getZ(), E, "Z file bounding");
        }
        else
        {
            b = false;
        }

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const PRREVector vOriginalSubobjectProperty = subObjFromFile->getScaling();
        subObjFromFile->Scale(vScaleBy);
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->getScaling(), "property unchanged for subobject");

        return b;
    }

    bool testGetBiggestAreaScaled()
    {
        bool b = assertEquals(6.0f, obj->getBiggestAreaScaled(), E, "obj") &
            assertEquals(2.0f, objPlane->getBiggestAreaScaled(), E, "objPlane") &
            assertEquals(6.0f, objBox->getBiggestAreaScaled(), E, "objBox") &
            assertEquals(1.0f, objCube->getBiggestAreaScaled(), E, "objCube");
        
        TPRREfloat fObjFileAreaXY = objFromFile->getSizeVec().getX() * objFromFile->getSizeVec().getY();
        TPRREfloat fObjFileAreaXZ = objFromFile->getSizeVec().getX() * objFromFile->getSizeVec().getZ();
        TPRREfloat fObjFileAreaYZ = objFromFile->getSizeVec().getY() * objFromFile->getSizeVec().getZ();

        TPRREfloat fObjFileExpectedBiggestArea;
        if ( fObjFileAreaXY > fObjFileAreaXZ )
        {
            fObjFileExpectedBiggestArea = fObjFileAreaXY > fObjFileAreaYZ ? fObjFileAreaXY : fObjFileAreaYZ;
        }
        else
        {
            fObjFileExpectedBiggestArea = fObjFileAreaXZ > fObjFileAreaYZ ? fObjFileAreaXZ : fObjFileAreaYZ;
        }

        b &= assertEquals(fObjFileExpectedBiggestArea, objFromFile->getBiggestAreaScaled(), 0.01f, "objFromFile 1");

        const PRREVector vScaleBy(2.0f, 3.0f, 4.0f);
        objFromFile->Scale(vScaleBy);

        fObjFileAreaXY *= vScaleBy.getX() * vScaleBy.getY();
        fObjFileAreaXZ *= vScaleBy.getX() * vScaleBy.getZ();
        fObjFileAreaYZ *= vScaleBy.getY() * vScaleBy.getZ();

        TPRREfloat fObjFileExpectedBiggestAreaScaled;
        if ( fObjFileAreaXY > fObjFileAreaXZ )
        {
            fObjFileExpectedBiggestAreaScaled = fObjFileAreaXY > fObjFileAreaYZ ? fObjFileAreaXY : fObjFileAreaYZ;
        }
        else
        {
            fObjFileExpectedBiggestAreaScaled = fObjFileAreaXZ > fObjFileAreaYZ ? fObjFileAreaXZ : fObjFileAreaYZ;
        }

        b &= assertEquals(fObjFileExpectedBiggestAreaScaled, objFromFile->getBiggestAreaScaled(), 0.01f, "objFromFile 2");

        return b;
    }

    bool testRecalculateBiggerAreaScaled()
    {
        const TPRREfloat fObjAreaBiggest = obj->getBiggestAreaScaled();
        const TPRREfloat fObjFromFileAreaBiggest = objFromFile->getBiggestAreaScaled();

        bool b = assertEquals(fObjAreaBiggest, obj->recalculateBiggestAreaScaled(), E, "obj") &
            assertEquals(fObjFromFileAreaBiggest, objFromFile->recalculateBiggestAreaScaled(), E, "objFromFile");

        // LAZY: we could also test if we change vertex geometry and upload it again to host with setVertexTransferMode()
        // and expect the area to change automatically, but for now we just believe it is also ok ...

        return b;
    }

    bool testIsVisible()
    {
        return assertTrue(objPlane->isVisible(), "plane") &
            assertTrue(objFromFile->isVisible(), "objFromFile") &
            assertTrue(objBox->isVisible(), "box") &
            assertTrue(objCube->isVisible(), "cube");
    }

    bool testSetVisible()
    {
        objFromFile->SetOccluder(true);

        bool b = assertTrue(objFromFile->isOccluder(), "objFromFile is occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) != om->getOccluders().end(), "objFromFile is in getOccluders 1") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 1") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 1") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 1") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 1");

        obj->SetVisible(false);
        objFromFile->SetVisible(false);

        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 2") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 2") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 2") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 2") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 2");

        return b & assertFalse(obj->isVisible(), "obj") & assertFalse(objFromFile->isVisible(), "objFromFile");
    }

    bool testShow()
    {
        objFromFile->SetOccluder(true);

        obj->SetVisible(false);
        obj->Show();

        objFromFile->SetVisible(false);
        objFromFile->Show();

        return assertTrue(obj->isVisible(), "obj") & assertTrue(objFromFile->isVisible(), "objFromFile") &
            assertFalse(objFromFile->isOccluder(), "objFromFile is NOT occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 1") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 1") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 1") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 1") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 1");
    }

    bool testHide()
    {
        objFromFile->SetOccluder(true);

        bool b = assertTrue(objFromFile->isOccluder(), "objFromFile is occluder");

        obj->Hide();
        objFromFile->Hide();

        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 1") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 1") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 1") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 1") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 1");

        return b & assertFalse(obj->isVisible(), "obj") & assertFalse(objFromFile->isVisible(), "objFromFile");
    }

    bool testIsColliding_TO_BE_REMOVED()
    {
        return assertFalse(obj->isColliding_TO_BE_REMOVED(), "obj") &
            assertFalse(objFromFile->isColliding_TO_BE_REMOVED(), "objFromFile") &
            assertFalse(objPlane->isColliding_TO_BE_REMOVED(), "plane") &
            assertFalse(objBox->isColliding_TO_BE_REMOVED(), "box") &
            assertFalse(objCube->isColliding_TO_BE_REMOVED(), "cube");
    }

    bool testSetColliding_TO_BE_REMOVED()
    {
        obj->SetColliding_TO_BE_REMOVED(true);
        objFromFile->SetColliding_TO_BE_REMOVED(true);

        return assertTrue(obj->isColliding_TO_BE_REMOVED(), "obj") & assertTrue(objFromFile->isColliding_TO_BE_REMOVED(), "objFromFile");
    }

    bool testGetRotationOrder()
    {
        return assertEquals(PRRE_YXZ, obj->getRotationOrder(), "obj") &
            assertEquals(PRRE_YXZ, objFromFile->getRotationOrder(), "objFromFile") &
            assertEquals(PRRE_YXZ, objPlane->getRotationOrder(), "plane") &
            assertEquals(PRRE_YXZ, objBox->getRotationOrder(), "box") &
            assertEquals(PRRE_YXZ, objCube->getRotationOrder(), "cube");
    }

    bool testSetRotationOrder()
    {
        obj->SetRotationOrder(PRRE_XYZ);
        objFromFile->SetRotationOrder(PRRE_XYZ);

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const TPRRE_ROTATION_ORDER vOriginalSubobjectProperty = subObjFromFile->getRotationOrder();
        subObjFromFile->SetRotationOrder(PRRE_XYZ);
        bool b = assertTrue(vOriginalSubobjectProperty == subObjFromFile->getRotationOrder(), "property unchanged for subobject");

        return b & assertEquals(PRRE_XYZ, obj->getRotationOrder(), "obj") & assertEquals(PRRE_XYZ, objFromFile->getRotationOrder(), "objFromFile");
    }

    bool testIsLit()
    {
        return assertTrue(obj->isLit(), "obj") &
            assertTrue(objFromFile->isLit(), "objFromFile") &
            assertTrue(objPlane->isLit(), "plane") &
            assertTrue(objBox->isLit(), "box") &
            assertTrue(objCube->isLit(), "cube");       
    }

    bool testSetLit()
    {
        obj->SetLit(false);
        objFromFile->SetLit(false);

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const bool vOriginalSubobjectProperty = subObjFromFile->isLit();
        subObjFromFile->SetLit(false);
        bool b = assertTrue(vOriginalSubobjectProperty == subObjFromFile->isLit(), "property unchanged for subobject");

        return b & assertFalse(obj->isLit(), "obj") & assertFalse(objFromFile->isLit(), "objFromFile");
    }

    bool testIsDoubleSided()
    {
        return assertFalse(obj->isDoubleSided(), "obj") &
            assertFalse(objFromFile->isDoubleSided(), "objFromFile") &
            assertFalse(objPlane->isDoubleSided(), "plane") &
            assertFalse(objBox->isDoubleSided(), "box") &
            assertFalse(objCube->isDoubleSided(), "cube");
    }

    bool testSetDoubleSided()
    {
        obj->SetDoubleSided(true);
        objFromFile->SetDoubleSided(true);

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const bool vOriginalSubobjectProperty = subObjFromFile->isDoubleSided();
        subObjFromFile->SetDoubleSided(true);
        bool b = assertTrue(vOriginalSubobjectProperty == subObjFromFile->isDoubleSided(), "property unchanged for subobject");

        return b & assertTrue(obj->isDoubleSided(), "obj") & assertTrue(objFromFile->isDoubleSided(), "objFromFile");
    }

    bool testIsWireframed()
    {
        return assertFalse(obj->isWireframed(), "obj") &
            assertFalse(objFromFile->isWireframed(), "objFromFile") &
            assertFalse(objPlane->isWireframed(), "plane") &
            assertFalse(objBox->isWireframed(), "box") &
            assertFalse(objCube->isWireframed(), "cube");
    }

    bool testSetWireframed()
    {
        objFromFile->SetOccluder(true);

        bool b = true;
        b &= assertTrue(objFromFile->isOccluder(), "objFromFile is occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) != om->getOccluders().end(), "objFromFile is in getOccluders 1") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 1") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 1") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 1") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 1");

        obj->SetWireframed(true);
        objFromFile->SetWireframed(true);

        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 2") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 2") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 2") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 2") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 2");

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const bool vOriginalSubobjectProperty = subObjFromFile->isWireframed();
        subObjFromFile->SetWireframed(true);
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->isWireframed(), "property unchanged for subobject");

        return b & assertTrue(obj->isWireframed(), "obj") & assertTrue(objFromFile->isWireframed(), "objFromFile");
    }

    bool testIsWireframedCulled()
    {
        return assertFalse(obj->isWireframedCulled(), "obj") &
            assertFalse(objFromFile->isWireframedCulled(), "objFromFile") &
            assertFalse(objPlane->isWireframedCulled(), "plane") &
            assertFalse(objBox->isWireframedCulled(), "box") &
            assertFalse(objCube->isWireframedCulled(), "cube");
    }

    bool testSetWireframedCulled()
    {
        obj->SetWireframedCulled(true);
        objFromFile->SetWireframedCulled(true);

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const bool vOriginalSubobjectProperty = subObjFromFile->isWireframedCulled();
        subObjFromFile->SetWireframedCulled(true);
        bool b = assertTrue(vOriginalSubobjectProperty == subObjFromFile->isWireframedCulled(), "property unchanged for subobject");

        return b & assertTrue(obj->isWireframedCulled(), "obj") & assertTrue(objFromFile->isWireframedCulled(), "objFromFile");
    }

    bool testIsAffectingZBuffer()
    {
        return assertTrue(obj->isAffectingZBuffer(), "obj") &
            assertTrue(objFromFile->isAffectingZBuffer(), "objFromFile") &
            assertTrue(objPlane->isAffectingZBuffer(), "plane") &
            assertTrue(objBox->isAffectingZBuffer(), "box") &
            assertTrue(objCube->isAffectingZBuffer(), "cube"); 
    }

    bool testSetAffectingZBuffer()
    {
        objFromFile->SetOccluder(true);

        bool b = true;
        b &= assertTrue(objFromFile->isOccluder(), "objFromFile is occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) != om->getOccluders().end(), "objFromFile is in getOccluders 1") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 1") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 1") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 1") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 1");

        obj->SetAffectingZBuffer(false);
        objFromFile->SetAffectingZBuffer(false);

        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 2") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 2") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 2") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 2") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 2");

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const bool vOriginalSubobjectProperty = subObjFromFile->isAffectingZBuffer();
        subObjFromFile->SetAffectingZBuffer(false);
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->isAffectingZBuffer(), "property unchanged for subobject");

        return b & assertFalse(obj->isAffectingZBuffer(), "obj") & assertFalse(objFromFile->isAffectingZBuffer(), "objFromFile");
    }

    bool testIsTestingAgainstZBuffer()
    {
        return assertTrue(obj->isTestingAgainstZBuffer(), "obj") &
            assertTrue(objFromFile->isTestingAgainstZBuffer(), "objFromFile") &
            assertTrue(objPlane->isTestingAgainstZBuffer(), "plane") &
            assertTrue(objBox->isTestingAgainstZBuffer(), "box") &
            assertTrue(objCube->isTestingAgainstZBuffer(), "cube");
    }

    bool testSetTestingAgainstZBuffer()
    {
        obj->SetTestingAgainstZBuffer(false);
        objFromFile->SetTestingAgainstZBuffer(false);

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const bool vOriginalSubobjectProperty = subObjFromFile->isTestingAgainstZBuffer();
        subObjFromFile->SetTestingAgainstZBuffer(false);
        bool b = assertTrue(vOriginalSubobjectProperty == subObjFromFile->isTestingAgainstZBuffer(), "property unchanged for subobject");

        return b & assertFalse(obj->isTestingAgainstZBuffer(), "obj") & assertFalse(objFromFile->isTestingAgainstZBuffer(), "objFromFile");
    }

    bool testIsStickedToScreen()
    {
        return assertFalse(obj->isStickedToScreen(), "obj") &
            assertFalse(objFromFile->isStickedToScreen(), "objFromFile") &
            assertFalse(objPlane->isStickedToScreen(), "plane") &
            assertFalse(objBox->isStickedToScreen(), "box") &
            assertFalse(objCube->isStickedToScreen(), "cube");
    }

    bool testSetStickedToScreen()
    {
        objFromFile->SetOccluder(true);

        bool b = true;
        b &= assertTrue(objFromFile->isOccluder(), "objFromFile is occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) != om->getOccluders().end(), "objFromFile is in getOccluders 1") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 1") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 1") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 1") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 1");

        obj->SetStickedToScreen(true);
        objFromFile->SetStickedToScreen(true);

        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 2") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 2") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 2") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) != om->get2dOpaqueOccludees().end(), "objFromFile is in get2dOpaqueOccludees 2") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 2");

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const bool vOriginalSubobjectProperty = subObjFromFile->isStickedToScreen();
        subObjFromFile->SetStickedToScreen(true);
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->isStickedToScreen(), "property unchanged for subobject");

        return b & assertTrue(obj->isStickedToScreen(), "obj") & assertTrue(objFromFile->isStickedToScreen(), "objFromFile");
    }

    bool testIsOccluder()
    {
        return assertFalse(obj->isOccluder(), "obj") &
            assertFalse(objFromFile->isOccluder(), "objFromFile") &
            assertFalse(objPlane->isOccluder(), "plane") &
            assertFalse(objBox->isOccluder(), "box") &
            assertFalse(objCube->isOccluder(), "cube");
    }

    bool testSetOccluder()
    {
        obj->SetOccluder(true);
        objFromFile->SetOccluder(false);

        bool b = true;
        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder 1") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 1") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 1") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 1") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 1") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 1");

        objFromFile->SetStickedToScreen(true);
        objFromFile->SetOccluder(true);
        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder 2") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 2") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 2") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 2") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) != om->get2dOpaqueOccludees().end(), "objFromFile is in get2dOpaqueOccludees 2") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 2");
        objFromFile->SetStickedToScreen(false);

        objFromFile->SetWireframed(true);
        objFromFile->SetOccluder(true);
        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder 3") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 3") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 3") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 3") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 3") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 3");
        objFromFile->SetWireframed(false);

        objFromFile->SetAffectingZBuffer(false);
        objFromFile->SetOccluder(true);
        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder 4") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 4") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 4") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 4") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 4") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 4");
        objFromFile->SetAffectingZBuffer(true);

        objFromFile->Hide();
        objFromFile->SetOccluder(true);
        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder 5") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 5") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 5") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 5") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 5") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 5");
        objFromFile->Show();

        objFromFile->getMaterial().setBlendMode(PRRE_BM_STANDARD_TRANSPARENCY);
        objFromFile->SetOccluder(true);
        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder 6") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 6") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 6") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) != om->get3dBlendedOccludees().end(), "objFromFile is in get3dBlendedOccludees 6") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 6") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 6");
        objFromFile->getMaterial().setBlendMode(PRRE_BM_NONE);

        objFromFile->SetOccluder(true);

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const bool vOriginalSubobjectProperty = subObjFromFile->isOccluder();
        subObjFromFile->SetOccluder(true);
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->isOccluder(), "property unchanged for subobject");

        return b & assertTrue(obj->isOccluder(), "obj is occluder") & assertTrue(objFromFile->isOccluder(), "objFromFile is occluder 7") &
            assertFalse(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is in getOccluders 7") &
            assertFalse(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 7") &
            assertFalse(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) != om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 7") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 7") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 7");
    }

    bool testIsOccluded()
    {
        return assertFalse(obj->isOccluded(), "obj") &
            assertFalse(objFromFile->isOccluded(), "objFromFile") &
            assertFalse(objPlane->isOccluded(), "plane") &
            assertFalse(objBox->isOccluded(), "box") &
            assertFalse(objCube->isOccluded(), "cube");
    }

    bool testIsOcclusionTested()
    {
        return assertFalse(obj->isOcclusionTested(), "obj") &
            assertTrue(objFromFile->isOcclusionTested(), "objFromFile") &
            assertFalse(objPlane->isOcclusionTested(), "plane") &
            assertFalse(objBox->isOcclusionTested(), "box") &
            assertFalse(objCube->isOcclusionTested(), "cube");
    }

    bool testSetOcclusionTested()
    {
        obj->SetOcclusionTested(true);                      
        objFromFile->SetOcclusionTested(true);

        PRREObject3D* const objCloned = om->createCloned(*obj);
        if ( !objCloned )
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }

        bool b = assertTrue(obj->isOcclusionTested(), "obj is tested 1") &
            assertTrue(objCloned->isOcclusionTested(), "objCloned is tested 1") &
            assertTrue(objFromFile->isOcclusionTested(), "objFromFile is tested 1") &
            assertNotNull(obj->getBoundingBoxObject(), "obj bounding 1") &
            assertNull(objCloned->getBoundingBoxObject(), "objCloned bounding 1") &
            assertNotNull(objFromFile->getBoundingBoxObject(), "objFromFile bounding 1");

        // expecting cloned object to also have no occlusion test anymore
        obj->SetOcclusionTested(false);
        b &= assertFalse(obj->isOcclusionTested(), "obj is tested 1") &
            assertFalse(objCloned->isOcclusionTested(), "objCloned is tested 1") &
            assertNull(obj->getBoundingBoxObject(), "obj bounding 1");

        delete objCloned;

        obj->SetOcclusionTested(false);
        objFromFile->SetOcclusionTested(false);

        PRREObject3D* const subObjFromFile = (PRREObject3D*)(objFromFile->getAttachedAt(0));
        const bool vOriginalSubobjectProperty = subObjFromFile->isOcclusionTested();
        subObjFromFile->SetOcclusionTested(false);
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->isOcclusionTested(), "property unchanged for subobject");

        b &= assertFalse(obj->isOcclusionTested(), "obj is tested 2") &
            assertFalse(objFromFile->isOcclusionTested(), "objFromFile is tested 2") &
            assertNull(obj->getBoundingBoxObject(), "obj bounding 2") &
            assertNull(objFromFile->getBoundingBoxObject(), "objFromFile bounding 2");

        return b;
    }

    bool testGetBoundingBoxObject()
    {
        bool b = assertNull(obj->getBoundingBoxObject(), "obj") &
            assertNotNull(objFromFile->getBoundingBoxObject(), "objFromFile") &
            assertNull(objPlane->getBoundingBoxObject(), "plane") &
            assertNull(objBox->getBoundingBoxObject(), "box") &
            assertNull(objCube->getBoundingBoxObject(), "cube");

        if ( b )
        {
            b &= assertNull(objFromFile->getBoundingBoxObject()->getBoundingBoxObject(), "objFromFile no recursive bounding boxes");
        }

        return b;
    }

    bool testDraw()
    {
        TPRREuint nObjLastTransferredVertices = obj->draw(PRRE_RPASS_NORMAL, false, false);
        TPRREuint nObjFromFileLastTransferredVertices = objFromFile->draw(PRRE_RPASS_NORMAL, false, false);

        bool b = assertEquals(obj->getVertexIndicesCount(), obj->getLastTransferredVertexCount(), "obj 1") &
            assertEquals(objFromFile->getVertexIndicesCount(), objFromFile->getLastTransferredVertexCount(), "objFromFile 1") &
            assertEquals(nObjLastTransferredVertices, obj->getLastTransferredVertexCount(), "obj 2") &
            assertEquals(nObjFromFileLastTransferredVertices, objFromFile->getLastTransferredVertexCount(), "objFromFile 2") &
            assertEquals(obj->getTriangleCount(), obj->getLastTransferredTriangleCount(), "obj 3") &
            assertEquals(objFromFile->getTriangleCount(), objFromFile->getLastTransferredTriangleCount(), "objFromFile 3");

        // make sure the objects are far behind the camera
        obj->getPosVec().SetZ(-100.f);
        objFromFile->getPosVec().SetZ(-100.f);

        // just to be sure
        obj->SetOcclusionTested(false);
        objFromFile->SetOcclusionTested(true);

        nObjLastTransferredVertices = obj->draw(PRRE_RPASS_START_OCCLUSION_QUERY, false, false);
        nObjFromFileLastTransferredVertices = objFromFile->draw(PRRE_RPASS_START_OCCLUSION_QUERY, false, false);

        b &= assertEquals((TPRREuint)0, obj->getLastTransferredVertexCount(), "obj 4") &
            assertEquals((TPRREuint)24, objFromFile->getLastTransferredVertexCount(), "objFromFile 4") &
            assertEquals(nObjLastTransferredVertices, obj->getLastTransferredVertexCount(), "obj 5") &
            assertEquals(nObjFromFileLastTransferredVertices, objFromFile->getLastTransferredVertexCount(), "objFromFile 5") &
            assertEquals((TPRREuint)0, obj->getLastTransferredTriangleCount(), "obj 6") &
            assertEquals((TPRREuint)12, objFromFile->getLastTransferredTriangleCount(), "objFromFile 6");
        
        nObjLastTransferredVertices = obj->draw(PRRE_RPASS_NORMAL, false, false);
        nObjFromFileLastTransferredVertices = objFromFile->draw(PRRE_RPASS_NORMAL, false, false);

        // expecting all zeros for objFromFile due to behind the camera and occlusion test fail, but
        // expecting same positive values as before for obj because it is not occlusion tested!
        b &= assertEquals(obj->getVertexIndicesCount(), obj->getLastTransferredVertexCount(), "obj 7") &
            assertEquals((TPRREuint)0, objFromFile->getLastTransferredVertexCount(), "objFromFile 7") &
            assertEquals(nObjLastTransferredVertices, nObjLastTransferredVertices, "obj 8") &
            assertEquals((TPRREuint)0, nObjFromFileLastTransferredVertices, "objFromFile 8") &
            assertEquals(obj->getTriangleCount(), obj->getLastTransferredTriangleCount(), "obj 9") &
            assertEquals((TPRREuint)0, objFromFile->getLastTransferredTriangleCount(), "objFromFile 9");

        // debug bounding box must be rendered regardless of object occlusion state, but
        // it must not be rendered for an object which is not occlusion tested
        nObjLastTransferredVertices = obj->draw(PRRE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY, false, false);
        nObjFromFileLastTransferredVertices = objFromFile->draw(PRRE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY, false, false);

        b &= assertEquals((TPRREuint)0, obj->getLastTransferredVertexCount(), "obj 10") &
            assertEquals((TPRREuint)24, objFromFile->getLastTransferredVertexCount(), "objFromFile 10") &
            assertEquals(nObjLastTransferredVertices, obj->getLastTransferredVertexCount(), "obj 11") &
            assertEquals(nObjFromFileLastTransferredVertices, objFromFile->getLastTransferredVertexCount(), "objFromFile 11") &
            assertEquals((TPRREuint)0, obj->getLastTransferredTriangleCount(), "obj 12") &
            assertEquals((TPRREuint)12, objFromFile->getLastTransferredTriangleCount(), "objFromFile 12");

        return b;
    }

    bool testGetUsedSystemMemory()
    {
        return assertGreater(obj->getUsedSystemMemory(),          sizeof(PRREObject3D) + obj->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),         "obj") &
               assertGreater(objFromFile->getUsedSystemMemory(),  sizeof(PRREObject3D) + objFromFile->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ), "objFromFile") &
               assertGreater(objPlane->getUsedSystemMemory(),     sizeof(PRREObject3D) + objPlane->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),    "plane") &
               assertGreater(objBox->getUsedSystemMemory(),       sizeof(PRREObject3D) + objBox->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),      "box") &
               assertGreater(objCube->getUsedSystemMemory(),      sizeof(PRREObject3D) + objCube->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),     "cube");
    }

    bool testGetUsedVideoMemory()
    {
        PRREObject3D* const objCloned = om->createCloned( *obj );
        if ( !objCloned )
        {
            return assertNotNull(objCloned, "objCloned is NULL");
        }

        TPRREuint objFromFileMinMemUsage = 0;
        for (TPRREint i = 0; i < objFromFile->getCount(); i++)
        {
            const PRREObject3D* const submesh = (PRREObject3D*) objFromFile->getAttachedAt(i);
            objFromFileMinMemUsage += submesh->getVerticesCount() * sizeof(TXYZ) * 2 /* *2 because of we have normals too */ +
                submesh->getMaterial(false).getColorsCount() * sizeof(TRGBAFLOAT) +
                submesh->getMaterial(false).getTexcoordsCount() * sizeof(TUVW);
        }
        if ( objFromFile->getBoundingBoxObject() )
        {
            objFromFileMinMemUsage += objFromFile->getBoundingBoxObject()->getUsedVideoMemory();
        }

        // by default the created objects should eat video memory, except the cloned object
        // since it doesn't have underlying geometry, or bounding box object in this case!
        const bool b = assertEquals(objCloned->getUsedVideoMemory(), (TPRREuint) 0,          "objCloned") &
            assertGreater(obj->getUsedVideoMemory(),                 (TPRREuint) 0,          "obj") &
            assertGreater(objFromFile->getUsedVideoMemory(),         objFromFileMinMemUsage, "objFromFile") &
            assertGreater(objPlane->getUsedVideoMemory(),            (TPRREuint) 0,          "plane") &
            assertGreater(objBox->getUsedVideoMemory(),              (TPRREuint) 0,          "box") &
            assertGreater(objCube->getUsedVideoMemory(),             (TPRREuint) 0,          "cube");

        delete objCloned;
        
        return b;
    }

    bool testGetCount()
    {
        return assertEquals(1, obj->getCount(), "obj") &
            assertEquals(9, objFromFile->getCount(), "objFromFile") &
            assertEquals(1, objPlane->getCount(), "plane") &
            assertEquals(1, objBox->getCount(), "box") &
            assertEquals(1, objCube->getCount(), "cube");
    }

    bool testIsEmpty()
    {
        return assertFalse(obj->isEmpty(), "obj") &
            assertFalse(objFromFile->isEmpty(), "objFromFile") &
            assertFalse(objPlane->isEmpty(), "plane") &
            assertFalse(objBox->isEmpty(), "box") &
            assertFalse(objCube->isEmpty(), "cube");
    }

    bool testGetSize()
    {
        return assertGequals(obj->getSize(),      obj->getCount(),         "obj") &
            assertGequals(objFromFile->getSize(), objFromFile->getCount(), "objFromFile") &
            assertGequals(objPlane->getSize(),    objPlane->getCount(),    "plane") &
            assertGequals(objBox->getSize(),      objBox->getCount(),      "box") &
            assertGequals(objCube->getSize(),     objCube->getCount(),     "cube");
    }

    bool testGetAttachedAt()
    {
        return assertNotNull(obj->getAttachedAt(0),   "obj") &
            assertNotNull(objFromFile->getAttachedAt(0),   "objFromFile") &
            assertNotNull(objPlane->getAttachedAt(0), "plane") &
            assertNotNull(objBox->getAttachedAt(0),   "box") &
            assertNotNull(objCube->getAttachedAt(0),  "cube");
    }

    bool testPreAlloc()
    {
        const TPRREint prevSize = obj->getSize();
        obj->PreAlloc(10);

        return assertEquals(prevSize + 10, obj->getSize());
    }

    bool testHasAttached2()
    {
        PRREObject3D* const mgd1 = om->createCube(1);
        mgd1->DetachFrom();
        obj->Attach(*mgd1);

        return assertFalse(obj->hasAttached(*objPlane), "objPlane") &
               assertTrue(obj->hasAttached(*mgd1), "mgd1");
    }

    bool testAttach()
    {
        PRREObject3D& mgr1 = *(om->createCube(1));
        PRREObject3D& mgr2 = *(om->createCube(1));
        obj->DetachFrom();
        mgr1.Attach(*obj);
        mgr2.Attach(*obj);

        bool b = assertTrue(mgr1.hasAttached(*obj), "mgr1.hasAttached()") &
               assertEquals(2, mgr1.getCount(), "mgr1.getCount()") &
               assertFalse(mgr2.hasAttached(*obj), "mgr2.hasAttached()") &
               assertEquals(1, mgr2.getCount(), "mgr2.getCount()");

        obj->DetachFrom();
        delete &mgr1;
        delete &mgr2;

        return b;
    }

    bool testDetach()
    {
        PRREObject3D& mgd = *((PRREObject3D*) (obj->getAttachedAt(0)));
        obj->Detach(mgd);
        bool l = assertEquals(0, obj->getCount());
        obj->Attach(mgd);

        return l;
    }

    bool testDeleteAttachedInstance()
    {
        PRREObject3D& mgd = *((PRREObject3D*) (obj->getAttachedAt(0)));
        obj->DeleteAttachedInstance(mgd);

        return assertEquals(0, obj->getCount());
    }

    bool testDeleteAll()
    {
        PRREObject3D* const mgd1 = om->createCube(1);
        PRREObject3D* const mgd2 = om->createCube(1);

        obj->DeleteAll();
        bool l = assertEquals(0, obj->getCount(), "getCount 1");

        obj->Attach(*mgd1);
        obj->Attach(*mgd2);
        obj->DeleteAll();
        l &= assertEquals(0, obj->getCount(), "getCount 2");

        return l;
    }

    bool testWriteList()
    {
        obj->WriteList();

        return true;
    }

    bool testDetachFrom()
    {
        objFromFile->SetOcclusionTested(true);
        obj->SetOccluder(true);

        const std::size_t nOriginalOccludersCount = om->getOccluders().size();
        const std::size_t nOriginalOccludeesCount = om->get3dOpaqueOccludees().size() + om->get2dOpaqueOccludees().size() + om->get3dBlendedOccludees().size() + om->get2dBlendedOccludees().size();

        bool b = assertTrue(objFromFile->isOcclusionTested(), "objFromFile occlusiontested 1") &
            assertTrue(obj->isOccluder(), "obj occluder 1") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), obj) != om->getOccluders().end(), "obj is in getOccluders 1") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), obj) == om->get3dOpaqueOccludees().end(), "obj is NOT in getOpaqueOccludees 1") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), obj) == om->get3dBlendedOccludees().end(), "obj is NOT in get3dBlendedOccludees 1") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), obj) == om->get2dOpaqueOccludees().end(), "obj is NOT in get2dOpaqueOccludees 1") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), obj) == om->get2dBlendedOccludees().end(), "obj is NOT in get2dBlendedOccludees 1");

        objFromFile->DetachFrom();
        obj->DetachFrom();

        b &= assertEquals(nOriginalOccludersCount-1, om->getOccluders().size(), "getOccluders count 2") & assertEquals(nOriginalOccludeesCount-1, om->get3dOpaqueOccludees().size(), "getOpaqueOccludees count 2");
        b &= assertFalse(objFromFile->isOcclusionTested(), "objFromFile not occlusiontested 1") &
            assertFalse(obj->isOccluder(), "obj not occluder 1") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), obj) == om->getOccluders().end(), "obj is NOT in getOccluders 2") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), obj) == om->get3dOpaqueOccludees().end(), "obj is NOT in getOpaqueOccludees 2") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), obj) == om->get3dBlendedOccludees().end(), "obj is NOT in get3dBlendedOccludees 2") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), obj) == om->get2dOpaqueOccludees().end(), "obj is NOT in get2dOpaqueOccludees 2") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), obj) == om->get2dBlendedOccludees().end(), "obj is NOT in get2dBlendedOccludees 2");

        return b;
    }

}; // class PRREObject3DTest