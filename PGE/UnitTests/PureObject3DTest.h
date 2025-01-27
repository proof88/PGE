#pragma once

/*
    ###################################################################################
    PureImageTest.h
    Unit test for PureObject3D.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"
#include "../Pure/include/external/Object3D/PureObject3DManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureObject3DTest :
    public UnitTest
{
public:

    PureObject3DTest() :
        UnitTest( __FILE__ )
    {
        engine = NULL;
        obj = NULL;
        objFromFile = NULL;
        objCloned = NULL;
        objFromFileCloned = NULL;
        objPlane = NULL;
        objBox = NULL;
        objCube = NULL;
    }

    virtual ~PureObject3DTest()
    {
        finalize();
    }

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureVertexTransfer::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureObject3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureObject3D::getLoggerModuleName(), true);

        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        om = &engine->getObject3DManager();
        obj = NULL;
        objFromFile = NULL;
        objCloned = NULL;
        objFromFileCloned = NULL;
        objPlane = om->createPlane(1.0f, 2.0f);
        objBox   = om->createBox(1.0f, 2.0f, 3.0f);
        objCube  = om->createCube(1.0f);

        addSubTest("testCtor", (PFNUNITSUBTEST) &PureObject3DTest::testCtor);
        addSubTest("testDtor", (PFNUNITSUBTEST) &PureObject3DTest::testDtor);
        addSubTest("testGetReferredObject", (PFNUNITSUBTEST) &PureObject3DTest::testGetReferredObject_GetReferrerObjects);

        // Object3D contains some slight modifications in these functions compared to the original Mesh3D functions
        addSubTest("testGetPrimitiveFormat", (PFNUNITSUBTEST) &PureObject3DTest::testGetPrimitiveFormat);
        addSubTest("testGetVerticesCount", (PFNUNITSUBTEST) &PureObject3DTest::testGetVerticesCount);
        addSubTest("testGetVertices", (PFNUNITSUBTEST) &PureObject3DTest::testGetVertices);
        addSubTest("testGetVertexIndicesType", (PFNUNITSUBTEST) &PureObject3DTest::testGetVertexIndicesType);
        addSubTest("testGetMinVertexIndex", (PFNUNITSUBTEST) &PureObject3DTest::testGetMinVertexIndex);
        addSubTest("testGetMaxVertexIndex", (PFNUNITSUBTEST) &PureObject3DTest::testGetMaxVertexIndex);
        addSubTest("testGetVertexIndicesCount", (PFNUNITSUBTEST) &PureObject3DTest::testGetVertexIndicesCount);
        addSubTest("testGetVertexIndex", (PFNUNITSUBTEST) &PureObject3DTest::testGetVertexIndex);
        addSubTest("testGetNormals", (PFNUNITSUBTEST) &PureObject3DTest::testGetNormals);
        addSubTest("testGetFaceCount", (PFNUNITSUBTEST) &PureObject3DTest::testGetFaceCount);
        addSubTest("testGetTriangleCount", (PFNUNITSUBTEST) &PureObject3DTest::testGetTriangleCount);
        // Object3D contains some slight modifications in these functions compared to the original VertexTransfer functions
        addSubTest("testGetVertexModifyingHabit", (PFNUNITSUBTEST) &PureObject3DTest::testGetVertexModifyingHabit);
        addSubTest("testSetVertexModifyingHabit", (PFNUNITSUBTEST) &PureObject3DTest::testSetVertexModifyingHabit);
        addSubTest("testGetVertexReferencingMode", (PFNUNITSUBTEST) &PureObject3DTest::testGetVertexReferencingMode);
        addSubTest("testSetVertexReferencingMode", (PFNUNITSUBTEST) &PureObject3DTest::testSetVertexReferencingMode);
        addSubTest("testGetVertexTransferMode", (PFNUNITSUBTEST) &PureObject3DTest::testGetVertexTransferMode);
        addSubTest("testSetVertexTransferMode", (PFNUNITSUBTEST) &PureObject3DTest::testSetVertexTransferMode);
        addSubTest("testGetLastTransferredVertexCount", (PFNUNITSUBTEST) &PureObject3DTest::testGetLastTransferredVertexCount);
        addSubTest("testGetLastTransferredTriangleCount", (PFNUNITSUBTEST) &PureObject3DTest::testGetLastTransferredTriangleCount);

        addSubTest("testGetTransformedVertices", (PFNUNITSUBTEST) &PureObject3DTest::testGetTransformedVertices);
        addSubTest("testGetAngleVec", (PFNUNITSUBTEST) &PureObject3DTest::testGetAngleVec);
        addSubTest("testGetScaledSizeVec", (PFNUNITSUBTEST) &PureObject3DTest::testGetScaledSizeVec);
        addSubTest("testGetScaling", (PFNUNITSUBTEST) &PureObject3DTest::testGetScaling);
        addSubTest("testSetScalingToScalar", (PFNUNITSUBTEST) &PureObject3DTest::testSetScalingToScalar);
        addSubTest("testSetScalingToVector", (PFNUNITSUBTEST) &PureObject3DTest::testSetScalingToVector);
        addSubTest("testScaleByScalar", (PFNUNITSUBTEST) &PureObject3DTest::testScaleByScalar);
        addSubTest("testScaleByVector", (PFNUNITSUBTEST) &PureObject3DTest::testScaleByVector);
        addSubTest("testGetBiggestAreaScaled", (PFNUNITSUBTEST) &PureObject3DTest::testGetBiggestAreaScaled);
        addSubTest("testRecalculateBiggerAreaScaled", (PFNUNITSUBTEST) &PureObject3DTest::testRecalculateBiggerAreaScaled);
        addSubTest("testIsVisible", (PFNUNITSUBTEST) &PureObject3DTest::testIsVisible);
        addSubTest("testSetVisible", (PFNUNITSUBTEST) &PureObject3DTest::testSetVisible);
        addSubTest("testShow", (PFNUNITSUBTEST) &PureObject3DTest::testShow);
        addSubTest("testHide", (PFNUNITSUBTEST) &PureObject3DTest::testHide);
        addSubTest("testIsColliding_TO_BE_REMOVED", (PFNUNITSUBTEST) &PureObject3DTest::testIsColliding_TO_BE_REMOVED);
        addSubTest("testSetColliding_TO_BE_REMOVED", (PFNUNITSUBTEST) &PureObject3DTest::testSetColliding_TO_BE_REMOVED);
        addSubTest("testGetRotationOrder", (PFNUNITSUBTEST) &PureObject3DTest::testGetRotationOrder);
        addSubTest("testSetRotationOrder", (PFNUNITSUBTEST) &PureObject3DTest::testSetRotationOrder);
        addSubTest("testIsLit", (PFNUNITSUBTEST) &PureObject3DTest::testIsLit);
        addSubTest("testSetLit", (PFNUNITSUBTEST) &PureObject3DTest::testSetLit);
        addSubTest("testIsDoubleSided", (PFNUNITSUBTEST) &PureObject3DTest::testIsDoubleSided);
        addSubTest("testSetDoubleSided", (PFNUNITSUBTEST) &PureObject3DTest::testSetDoubleSided);
        addSubTest("testIsWireframed", (PFNUNITSUBTEST) &PureObject3DTest::testIsWireframed);
        addSubTest("testSetWireframed", (PFNUNITSUBTEST) &PureObject3DTest::testSetWireframed);
        addSubTest("testIsWireframedCulled", (PFNUNITSUBTEST) &PureObject3DTest::testIsWireframedCulled);
        addSubTest("testSetWireframedCulled", (PFNUNITSUBTEST) &PureObject3DTest::testSetWireframedCulled);
        addSubTest("testIsAffectingZBuffer", (PFNUNITSUBTEST) &PureObject3DTest::testIsAffectingZBuffer);
        addSubTest("testSetAffectingZBuffer", (PFNUNITSUBTEST) &PureObject3DTest::testSetAffectingZBuffer);
        addSubTest("testIsTestingAgainstZBuffer", (PFNUNITSUBTEST) &PureObject3DTest::testIsTestingAgainstZBuffer);
        addSubTest("testSetTestingAgainstZBuffer", (PFNUNITSUBTEST) &PureObject3DTest::testSetTestingAgainstZBuffer);
        addSubTest("testIsStickedToScreen", (PFNUNITSUBTEST) &PureObject3DTest::testIsStickedToScreen);
        addSubTest("testSetStickedToScreen", (PFNUNITSUBTEST) &PureObject3DTest::testSetStickedToScreen);
        addSubTest("testIsOccluder", (PFNUNITSUBTEST) &PureObject3DTest::testIsOccluder);
        addSubTest("testSetOccluder", (PFNUNITSUBTEST) &PureObject3DTest::testSetOccluder);
        addSubTest("testIsOccluded", (PFNUNITSUBTEST) &PureObject3DTest::testIsOccluded);
        addSubTest("testIsOcclusionTested", (PFNUNITSUBTEST) &PureObject3DTest::testIsOcclusionTested);
        addSubTest("testSetOcclusionTested", (PFNUNITSUBTEST) &PureObject3DTest::testSetOcclusionTested);
        addSubTest("testGetBoundingBoxObject", (PFNUNITSUBTEST) &PureObject3DTest::testGetBoundingBoxObject);
        addSubTest("testDraw", (PFNUNITSUBTEST) &PureObject3DTest::testDraw);
        addSubTest("testDraw_Cloned", (PFNUNITSUBTEST) &PureObject3DTest::testDraw_Cloned);
        addSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PureObject3DTest::testGetUsedSystemMemory);
        addSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PureObject3DTest::testGetUsedVideoMemory);

        // since Object3D became a Manager also, we should test these capabalities here as well for possible changed behavior
        addSubTest("testGetCount", (PFNUNITSUBTEST) &PureObject3DTest::testGetCount);
        addSubTest("testIsEmpty", (PFNUNITSUBTEST) &PureObject3DTest::testIsEmpty);
        addSubTest("testGetSize", (PFNUNITSUBTEST) &PureObject3DTest::testGetSize);
        addSubTest("testGetAttachedAt", (PFNUNITSUBTEST) &PureObject3DTest::testGetAttachedAt);
        addSubTest("testPreAlloc", (PFNUNITSUBTEST) &PureObject3DTest::testPreAlloc);
        addSubTest("testHasAttached2", (PFNUNITSUBTEST) &PureObject3DTest::testHasAttached2);
        addSubTest("testAttach", (PFNUNITSUBTEST) &PureObject3DTest::testAttach);
        addSubTest("testDetach", (PFNUNITSUBTEST) &PureObject3DTest::testDetach);
        addSubTest("testDeleteAttachedInstance", (PFNUNITSUBTEST) &PureObject3DTest::testDeleteAttachedInstance);
        addSubTest("testDeleteAll", (PFNUNITSUBTEST) &PureObject3DTest::testDeleteAll);
        addSubTest("testWriteList", (PFNUNITSUBTEST) &PureObject3DTest::testWriteList);

        // since DetachFrom()'s behavior depends on managers' Detach() behavior, and that is overridden in Object3DManager, we test DetachFrom() too!
        addSubTest("testDetachFrom", (PFNUNITSUBTEST) &PureObject3DTest::testDetachFrom);
    }

    virtual bool setUp() override
    {
        obj = om->createBox(1.0f, 2.0f, 3.0f);
        objFromFile = om->createFromFile("_res/models/snail_proofps/snail.obj");

        if ( !obj )
        {
            return assertNotNull(obj, "obj is NULL");
        }
        if ( !objFromFile )
        {
            return assertNotNull(objFromFile, "objFromFile is NULL");
        }

        objCloned = om->createCloned(*obj);
        objFromFileCloned = om->createCloned(*objFromFile);

        return assertNotNull(obj, "obj null") &
            assertNotNull(objFromFile, "objFromFile null") &
            assertNotNull(objCloned, "objCloned null") &
            assertNotNull(objFromFileCloned, "objFromFileCloned null") &
            assertNotNull(objPlane, "objPlane null") &
            assertNotNull(objCube, "objCube null") &
            assertNotNull(objBox, "objBox null");
    }

    virtual void tearDown() override
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
        if ( objCloned )
        {
            delete objCloned;
            objCloned = NULL;
        }
        if ( objFromFileCloned )
        {
            delete objFromFileCloned;
            objFromFileCloned = NULL;
        }
    }

    virtual void finalize() override
    {
        obj = NULL;
        objFromFile = NULL;
        objCloned = NULL;
        objFromFileCloned = NULL;
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

        CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureVertexTransfer::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureObject3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureObject3D::getLoggerModuleName(), false);
    }

private:
    PR00FsUltimateRenderingEngine* engine;
    PureObject3DManager* om;
    PureObject3D* obj;
    PureObject3D* objFromFile;
    PureObject3D* objCloned;
    PureObject3D* objFromFileCloned;
    const PureObject3D* objPlane,
                      * objBox,
                      * objCube;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PureObject3DTest(const PureObject3DTest&)
    {};         

    PureObject3DTest& operator=(const PureObject3DTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return true;
    }

    bool testDtor()
    {
        const TPureInt objCount = om->getCount();

        delete obj;
        obj = NULL;

        delete objFromFile;
        objFromFile = NULL;

        // dtor of Object3D is also responsible for removing itself from its manager
        return assertEquals(objCount-2, om->getCount(), "getCount 2nd");
    }

    bool testGetReferredObject_GetReferrerObjects()
    {
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
        objCloned = PGENULL;
        b &= assertTrue(obj->getReferrerObjects().empty(), "obj->getReferrerObjects().empty() after deleting cloned");

        delete objFromFile;
        objFromFile = PGENULL;        
        b &= assertEquals((PureObject3D*)PGENULL, objFromFileCloned->getReferredObject(), "objFromFileCloned->getReferredObject() NULL after deleting referred");

        return b;
    }

    bool testGetPrimitiveFormat()
    {
        return assertEquals(PURE_PM_TRIANGLES, objFromFile->getPrimitiveFormat(), "objFromFile") &
            assertEquals(PURE_PM_TRIANGLES, objFromFileCloned->getPrimitiveFormat(), "objFromFileCloned") &
            assertEquals(PURE_PM_QUADS, obj->getPrimitiveFormat(), "obj") &
            assertEquals(PURE_PM_QUADS, objCloned->getPrimitiveFormat(), "objCloned") &
            assertEquals(PURE_PM_QUADS, objPlane->getPrimitiveFormat(), "plane") &
            assertEquals(PURE_PM_QUADS, objBox->getPrimitiveFormat(), "box") &
            assertEquals(PURE_PM_QUADS, objCube->getPrimitiveFormat(), "cube");
    }

    bool testGetVerticesCount()
    {
        // note: not testing cloned with getAttachedAt() and similar stuff is not overridden to handle cloned objects
        TPureUInt nVerticesCount = 0;
        for (TPureInt i = 0; i < objFromFile->getCount(); i++)
        {
            nVerticesCount += ((PureObject3D*)objFromFile->getAttachedAt(i))->getVerticesCount();
        }

        return assertEquals((TPureUInt) 4, objPlane->getVerticesCount(), "plane") &
            assertEquals((TPureUInt) 24, objBox->getVerticesCount(), "box") &
            assertEquals((TPureUInt) 24, objCube->getVerticesCount(), "cube") &
            assertEquals(nVerticesCount, objFromFile->getVerticesCount(), "objFromFile") &
            assertEquals(nVerticesCount, objFromFileCloned->getVerticesCount(), "objFromFileCloned") &
            assertEquals(((PureObject3D*)objPlane->getAttachedAt(0))->getVerticesCount(), objPlane->getVerticesCount(), "plane 2") &
            assertEquals(((PureObject3D*)objBox->getAttachedAt(0))->getVerticesCount(), objBox->getVerticesCount(), "box 2") &
            assertEquals(((PureObject3D*)objCube->getAttachedAt(0))->getVerticesCount(), objCube->getVerticesCount(), "cube 2") &
            assertEquals((TPureUInt)144, ((PureObject3D*)objFromFile->getAttachedAt(0))->getVerticesCount(), "objFromFile 3");
    }

    bool testGetVertices()
    {
        // note: not testing cloned with getAttachedAt() and similar stuff is not overridden to handle cloned objects
        return assertNotNull(objPlane->getVertices(), "plane") &
            assertNotNull(objBox->getVertices(), "box") &
            assertNotNull(objCube->getVertices(), "cube") &
            assertNull(objFromFile->getVertices(), "objFromFile") &
            assertNull(objFromFileCloned->getVertices(), "objFromFileCloned") &
            assertEquals(((PureObject3D*)objPlane->getAttachedAt(0))->getVertices(), objPlane->getVertices(), "plane 2") &
            assertEquals(((PureObject3D*)objBox->getAttachedAt(0))->getVertices(), objBox->getVertices(), "box 2") &
            assertEquals(((PureObject3D*)objCube->getAttachedAt(0))->getVertices(), objCube->getVertices(), "cube 2") &
            assertNotEquals(((PureObject3D*)objFromFile->getAttachedAt(0))->getVertices(), objFromFile->getVertices(), "objFromFile 2") &
            assertNull(objPlane->getVertices(false), "plane noimplicit") &
            assertNull(objBox->getVertices(false), "box noimplicit") &
            assertNull(objCube->getVertices(false), "cube noimplicit") &
            assertNull(objFromFile->getVertices(false), "objFromFile noimplicit") &
            assertNull(objFromFileCloned->getVertices(false), "objFromFileCloned noimplicit");
    }

    bool testGetVertexIndicesType()
    {
        // note: not testing cloned with getAttachedAt() and similar stuff is not overridden to handle cloned objects
        return assertEquals((unsigned int)GL_UNSIGNED_BYTE, objPlane->getVertexIndicesType(), "plane") &
            assertEquals((unsigned int)GL_UNSIGNED_BYTE, objBox->getVertexIndicesType(), "box") &
            assertEquals((unsigned int)GL_UNSIGNED_BYTE, objCube->getVertexIndicesType(), "cube") &
            assertEquals((unsigned int)GL_UNSIGNED_BYTE, objFromFile->getVertexIndicesType(), "objFromFile noimplicit") &
            assertEquals((unsigned int)GL_UNSIGNED_BYTE, objFromFileCloned->getVertexIndicesType(), "objFromFileCloned noimplicit") &
            /* intentionally comparing level-1 parent vs level-2 submesh vertex index types since they must be equal anyway */
            assertEquals(((PureObject3D*)objPlane->getAttachedAt(0))->getVertexIndicesType(), objPlane->getVertexIndicesType(false), "plane noimplicit") &
            assertEquals(((PureObject3D*)objBox->getAttachedAt(0))->getVertexIndicesType(), objBox->getVertexIndicesType(false), "box noimplicit") &
            assertEquals(((PureObject3D*)objCube->getAttachedAt(0))->getVertexIndicesType(), objCube->getVertexIndicesType(false), "cube noimplicit") &
            assertEquals((unsigned int)GL_UNSIGNED_BYTE, ((PureObject3D*)objFromFile->getAttachedAt(0))->getVertexIndicesType(), "objFromFile noimplicit");
    }

    bool testGetMinVertexIndex()
    {
        // note: not testing cloned with getAttachedAt() and similar stuff is not overridden to handle cloned objects
        return assertEquals((TPureUInt)0, objPlane->getMinVertexIndex(), "plane") &
            assertEquals((TPureUInt)0, objBox->getMinVertexIndex(), "box") &
            assertEquals((TPureUInt)0, objCube->getMinVertexIndex(), "cube") &
            assertEquals((TPureUInt)UINT_MAX, objFromFile->getMinVertexIndex(), "objFromFile") &
            assertEquals((TPureUInt)UINT_MAX, objFromFileCloned->getMinVertexIndex(), "objFromFileCloned") &
            /* doesn't matter if implicit access or not, if there is vertex or not, 0 is the expected anyway */
            assertEquals(((PureObject3D*)objPlane->getAttachedAt(0))->getMinVertexIndex(), objPlane->getMinVertexIndex(false), "plane sub noimplicit") &
            assertEquals(((PureObject3D*)objBox->getAttachedAt(0))->getMinVertexIndex(), objBox->getMinVertexIndex(false), "box sub noimplicit") &
            assertEquals(((PureObject3D*)objCube->getAttachedAt(0))->getMinVertexIndex(), objCube->getMinVertexIndex(false), "cube sub noimplicit") &
            assertNotEquals(((PureObject3D*)objFromFile->getAttachedAt(0))->getMinVertexIndex(), objFromFile->getMinVertexIndex(false), "objFromFile sub noimplicit") &
            assertEquals(((PureObject3D*)objPlane->getAttachedAt(0))->getMinVertexIndex(), objPlane->getMinVertexIndex(), "plane sub") &
            assertEquals(((PureObject3D*)objBox->getAttachedAt(0))->getMinVertexIndex(), objBox->getMinVertexIndex(), "box sub") &
            assertEquals(((PureObject3D*)objCube->getAttachedAt(0))->getMinVertexIndex(), objCube->getMinVertexIndex(), "cube sub") &
            assertEquals((TPureUInt) 0, ((PureObject3D*)objFromFile->getAttachedAt(0))->getMinVertexIndex(), "objFromFile sub");
    }

    bool testGetMaxVertexIndex()
    {
        // note: not testing cloned with getAttachedAt() and similar stuff is not overridden to handle cloned objects
        return assertEquals((TPureUInt)3, objPlane->getMaxVertexIndex(), "plane") &
            assertEquals((TPureUInt)23, objBox->getMaxVertexIndex(), "box") &
            assertEquals((TPureUInt)23, objCube->getMaxVertexIndex(), "cube") &
            assertEquals((TPureUInt)0, objFromFile->getMaxVertexIndex(), "objFromFile") &
            assertEquals((TPureUInt)0, objFromFileCloned->getMaxVertexIndex(), "objFromFileCloned") &
            assertEquals((TPureUInt)0, objPlane->getMaxVertexIndex(false), "plane noimplicit") &
            assertEquals((TPureUInt)0, objBox->getMaxVertexIndex(false), "box noimplicit") &
            assertEquals((TPureUInt)0, objCube->getMaxVertexIndex(false), "cube noimplicit") &
            assertEquals((TPureUInt)0, objFromFile->getMaxVertexIndex(false), "objFromFile noimplicit") &
            assertEquals((TPureUInt)0, objFromFileCloned->getMaxVertexIndex(false), "objFromFileCloned noimplicit") &
            assertEquals((TPureUInt)3, ((PureObject3D*)objPlane->getAttachedAt(0))->getMaxVertexIndex(), "plane sub") &
            assertEquals((TPureUInt)23, ((PureObject3D*)objBox->getAttachedAt(0))->getMaxVertexIndex(), "box sub") &
            assertEquals((TPureUInt)23, ((PureObject3D*)objCube->getAttachedAt(0))->getMaxVertexIndex(), "cube sub") &
            assertEquals((TPureUInt)143, ((PureObject3D*)objFromFile->getAttachedAt(0))->getMaxVertexIndex(), "objFromFile sub");
    }

    bool testGetVertexIndicesCount()
    {
        // note: not testing cloned with getAttachedAt() and similar stuff is not overridden to handle cloned objects

        TPureUInt nVertexIndicesCount = 0;
        for (TPureInt i = 0; i < objFromFile->getCount(); i++)
        {
            nVertexIndicesCount += ((PureObject3D*)objFromFile->getAttachedAt(i))->getVertexIndicesCount();
        }

        return assertEquals((TPureUInt) 4, objPlane->getVertexIndicesCount(), "plane") &
            assertEquals((TPureUInt) 24, objBox->getVertexIndicesCount(), "box") &
            assertEquals((TPureUInt) 24, objCube->getVertexIndicesCount(), "cube") &
            assertEquals(nVertexIndicesCount, objFromFile->getVertexIndicesCount(), "objFromFile") &
            assertEquals(nVertexIndicesCount, objFromFileCloned->getVertexIndicesCount(), "objFromFileCloned") &
            assertEquals(((PureObject3D*)objPlane->getAttachedAt(0))->getVertexIndicesCount(), objPlane->getVertexIndicesCount(), "plane 2") &
            assertEquals(((PureObject3D*)objBox->getAttachedAt(0))->getVertexIndicesCount(), objBox->getVertexIndicesCount(), "box 2") &
            assertEquals(((PureObject3D*)objCube->getAttachedAt(0))->getVertexIndicesCount(), objCube->getVertexIndicesCount(), "cube 2") &
            assertEquals((TPureUInt)144, ((PureObject3D*)objFromFile->getAttachedAt(0))->getVertexIndicesCount(), "objFromFile 3");
    }

    bool testGetVertexIndex()
    {
        bool b = true;

        // note: not testing cloned with getAttachedAt() and similar stuff is not overridden to handle cloned objects
        
        const PureObject3D& submeshPlane = * ((PureObject3D*) (objPlane->getAttachedAt(0)));
        for (TPureUInt i = 0; i < submeshPlane.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i, submeshPlane.getVertexIndex(i),
            (std::string("plane sub ") + std::to_string(i)).c_str());
            b &= assertEquals(i, objPlane->getVertexIndex(i),
            (std::string("plane implicit ") + std::to_string(i)).c_str());
            // here explicit should return 0 since objPlane is level-1 obj without own geometry
            b &= assertEquals((TPureUInt)0, objPlane->getVertexIndex(i, false),
            (std::string("plane explicit ") + std::to_string(i)).c_str());
        }

        const PureObject3D& submeshBox = * ((PureObject3D*) (objBox->getAttachedAt(0)));
        for (TPureUInt i = 0; i < submeshBox.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i, submeshBox.getVertexIndex(i),
            (std::string("box sub ") + std::to_string(i)).c_str());
            b &= assertEquals(i, objBox->getVertexIndex(i),
            (std::string("box implicit ") + std::to_string(i)).c_str());
            // here explicit should return 0 since objBox is level-1 obj without own geometry
            b &= assertEquals((TPureUInt)0, objBox->getVertexIndex(i, false),
            (std::string("box explicit ") + std::to_string(i)).c_str());
        }

        const PureObject3D& submeshCube = * ((PureObject3D*) (objCube->getAttachedAt(0)));
        for (TPureUInt i = 0; i < submeshCube.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i, submeshCube.getVertexIndex(i),
            (std::string("cube sub ") + std::to_string(i)).c_str());
            b &= assertEquals(i, objCube->getVertexIndex(i),
            (std::string("cube implicit ") + std::to_string(i)).c_str());
            // here explicit should return 0 since objCube is level-1 obj without own geometry
            b &= assertEquals((TPureUInt)0, objCube->getVertexIndex(i, false),
            (std::string("cube explicit ") + std::to_string(i)).c_str());
        }

        const PureObject3D& submeshFile = * ((PureObject3D*) (objFromFile->getAttachedAt(0)));
        for (TPureUInt i = 0; i < submeshFile.getVertexIndicesCount(); i++)
        {
            b &= assertEquals(i, submeshFile.getVertexIndex(i),
            (std::string("objFromFile sub ") + std::to_string(i)).c_str());
            // here implicit should return 0 since submesh is not selected implicitly for multi-submesh objects!
            b &= assertEquals((TPureUInt)0, objFromFile->getVertexIndex(i),
            (std::string("objFromFile implicit ") + std::to_string(i)).c_str());
            // here explicit should return 0 since objFromFile is level-1 obj without own geometry
            b &= assertEquals((TPureUInt)0, objFromFile->getVertexIndex(i, false),
            (std::string("objFromFile explicit ") + std::to_string(i)).c_str());
        }

        return b;
    }

    bool testGetNormals()
    {
        return assertNotNull(objPlane->getNormals(), "plane") &
            assertNotNull(objBox->getNormals(), "box") &
            assertNotNull(objCube->getNormals(), "cube") &
            assertNull(objFromFile->getNormals(), "objFromFile") &
            assertNull(objFromFileCloned->getNormals(), "objFromFileCloned") &
            assertEquals(((PureObject3D*)objPlane->getAttachedAt(0))->getNormals(), objPlane->getNormals(), "plane 2") &
            assertEquals(((PureObject3D*)objBox->getAttachedAt(0))->getNormals(), objBox->getNormals(), "box 2") &
            assertEquals(((PureObject3D*)objCube->getAttachedAt(0))->getNormals(), objCube->getNormals(), "cube 2") &
            assertNotEquals(((PureObject3D*)objFromFile->getAttachedAt(0))->getNormals(), objFromFile->getNormals(), "objFromFile 2") &
            assertNull(objPlane->getNormals(false), "plane noimplicit") &
            assertNull(objBox->getNormals(false), "box noimplicit") &
            assertNull(objCube->getNormals(false), "cube noimplicit") &
            assertNull(objFromFile->getNormals(false), "objFromFile noimplicit") &
            assertNull(objFromFileCloned->getNormals(false), "objFromFileCloned noimplicit");
    }

    bool testGetFaceCount()
    {
        return assertEquals((TPureUInt)6, obj->getFaceCount(), "obj") &
            assertEquals((TPureUInt)728, objFromFile->getFaceCount(), "objFromFile") &
            assertEquals((TPureUInt)6, objCloned->getFaceCount(), "objCloned") &
            assertEquals((TPureUInt)728, objFromFileCloned->getFaceCount(), "objFromFileCloned");
    }

    bool testGetTriangleCount()
    {
        return assertEquals((TPureUInt)12, obj->getTriangleCount(), "obj") &
            assertEquals((TPureUInt)728, objFromFile->getTriangleCount(), "objFromFile") &
            assertEquals((TPureUInt)12, objCloned->getTriangleCount(), "objCloned") &
            assertEquals((TPureUInt)728, objFromFileCloned->getTriangleCount(), "objFromFileCloned");
    }

    bool testGetVertexModifyingHabit()
    {
        const TPURE_VERTEX_MODIFYING_HABIT originalVertexModifyingHabit = obj->getVertexModifyingHabit();
        // if we change the modifying habit of the referred object, cloned object returns the updated vertex modifying habit of the referred object
        bool b = assertTrue(obj->setVertexModifyingHabit(PURE_VMOD_DYNAMIC), "set");

        b &= assertEquals(PURE_VMOD_STATIC, originalVertexModifyingHabit, "obj original") &
            assertEquals(PURE_VMOD_DYNAMIC, obj->getVertexModifyingHabit(), "obj") &
            assertEquals(PURE_VMOD_DYNAMIC, objCloned->getVertexModifyingHabit(), "objCloned") &
            assertEquals(PURE_VMOD_STATIC, objFromFile->getVertexModifyingHabit(), "objFromFile") &
            assertEquals(PURE_VMOD_STATIC, objPlane->getVertexModifyingHabit(), "plane") &
            assertEquals(PURE_VMOD_STATIC, objBox->getVertexModifyingHabit(), "box") &
            assertEquals(PURE_VMOD_STATIC, objCube->getVertexModifyingHabit(), "cube");
        
        return b;
    }

    bool testSetVertexModifyingHabit()
    {
        bool b = assertTrue(objFromFile->setVertexModifyingHabit(PURE_VMOD_DYNAMIC), "set objFromFile");
        b &= assertTrue(obj->setVertexModifyingHabit(PURE_VMOD_DYNAMIC), "set obj");

        // subobjects must reject this when called by user
        PureObject3D* const subobj1Obj = (PureObject3D*)(obj->getAttachedAt(0));
        b &= assertFalse(subobj1Obj->setVertexModifyingHabit( PURE_VMOD_STATIC ), "set subobj1Obj");

        PureObject3D* const subobj1ObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
        b &= assertFalse(subobj1ObjFromFile->setVertexModifyingHabit( PURE_VMOD_STATIC ), "set subobj1FromFile");

        // cloned object must reject this in any case
        b &= assertFalse(objCloned->setVertexModifyingHabit( PURE_VMOD_STATIC ), "set objCloned");
        b &= assertFalse(objFromFileCloned->setVertexModifyingHabit( PURE_VMOD_STATIC ), "set objFromFileCloned");

        b &= assertTrue( PureVertexTransfer::isVertexModifyingDynamic(obj->getVertexTransferMode()), "obj" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic(objFromFile->getVertexTransferMode()), "objFromFile" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic(subobj1Obj->getVertexTransferMode()), "subobj1Obj" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic(subobj1ObjFromFile->getVertexTransferMode()), "subobj1ObjFromFile" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic(objCloned->getVertexTransferMode()), "objCloned" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic(objFromFileCloned->getVertexTransferMode()), "objFromFileCloned" );
        
        return b;
    }

    bool testGetVertexReferencingMode()
    {
        const TPURE_VERTEX_REFERENCING_MODE originalVertexRefMode = obj->getVertexReferencingMode();
        // if we change the referencing mode of the referred object, cloned object returns the updated vertex referencing mode of the referred object
        bool b = assertTrue(obj->setVertexReferencingMode(PURE_VREF_INDEXED), "set obj");

        b &= assertEquals(PURE_VREF_DIRECT, originalVertexRefMode, "obj original") &
            assertEquals(PURE_VREF_INDEXED, obj->getVertexReferencingMode(), "obj") &
            assertEquals(PURE_VREF_INDEXED, objCloned->getVertexReferencingMode(), "objCloned") &
            assertEquals(PURE_VREF_INDEXED, objFromFile->getVertexReferencingMode(), "objFromFile") &
            assertEquals(PURE_VREF_DIRECT, objPlane->getVertexReferencingMode(), "plane") &
            assertEquals(PURE_VREF_DIRECT, objBox->getVertexReferencingMode(), "box") &
            assertEquals(PURE_VREF_DIRECT, objCube->getVertexReferencingMode(), "cube");

        return b;
    }

    bool testSetVertexReferencingMode()
    {
        bool b = assertTrue(obj->setVertexReferencingMode(PURE_VREF_INDEXED), "set obj");
        b &= assertTrue(objFromFile->setVertexReferencingMode(PURE_VREF_DIRECT), "set objFromFile");

        // subobjects must reject this when called by user
        PureObject3D* const subobj1Obj = (PureObject3D*)(obj->getAttachedAt(0));
        b &= assertFalse(subobj1Obj->setVertexReferencingMode( PURE_VREF_DIRECT ), "set subobj1Obj");

        PureObject3D* const subobj1ObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
        b &= assertFalse(subobj1ObjFromFile->setVertexReferencingMode( PURE_VREF_INDEXED ), "set subobj1ObjFromFile");

        // cloned object must reject this in any case
        b &= assertFalse(objCloned->setVertexReferencingMode( PURE_VREF_DIRECT ), "set objCloned");
        b &= assertFalse(objFromFileCloned->setVertexReferencingMode( PURE_VREF_DIRECT ), "set objFromFileCloned");

        b &= assertTrue( PureVertexTransfer::isVertexReferencingIndexed(obj->getVertexTransferMode()), "obj" ) &
            assertFalse( PureVertexTransfer::isVertexReferencingIndexed(objFromFile->getVertexTransferMode()), "objFromFile" ) &
            assertTrue( PureVertexTransfer::isVertexReferencingIndexed(subobj1Obj->getVertexTransferMode()), "subobj1Obj" ) &
            assertFalse( PureVertexTransfer::isVertexReferencingIndexed(subobj1ObjFromFile->getVertexTransferMode()), "subobj1ObjFromFile" ) &
            assertTrue( PureVertexTransfer::isVertexReferencingIndexed(objCloned->getVertexTransferMode()), "objCloned" ) &
            assertFalse( PureVertexTransfer::isVertexReferencingIndexed(objFromFileCloned->getVertexTransferMode()), "objFromFileCloned" );

        return b;
    }

    bool testGetVertexTransferMode()
    {
        // Generic server-side vertex array should be selected, main test machine supports it
        const TPURE_VERTEX_TRANSFER_MODE vtExpected = PURE_VMOD_STATIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);

        // if we change the transfer mode of the referred object, cloned object returns the updated vertex transfer mode of the referred object
        const TPURE_VERTEX_TRANSFER_MODE vtExpectedForCloned = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT;
        bool b = assertTrue(obj->setVertexTransferMode(vtExpectedForCloned), "set obj");

        b &= assertEquals(vtExpectedForCloned, obj->getVertexTransferMode() & vtExpectedForCloned, "obj 1") &
            assertEquals(0u, BITF_READ(obj->getVertexTransferMode(), PURE_VT_VENDOR_BITS, 3), "obj 2") &
            assertEquals(vtExpectedForCloned, objCloned->getVertexTransferMode() & vtExpectedForCloned, "objCloned 1") &
            assertEquals(0u, BITF_READ(objCloned->getVertexTransferMode(), PURE_VT_VENDOR_BITS, 3), "objCloned 2") &
            assertEquals(vtExpected, objFromFile->getVertexTransferMode() & vtExpected, "objFromFile 1") &
            assertEquals(0u, BITF_READ(objFromFile->getVertexTransferMode(), PURE_VT_VENDOR_BITS, 3), "objFromFile 2") &
            assertEquals(vtExpected, objPlane->getVertexTransferMode() & vtExpected, "plane 1") &
            assertEquals(0u, BITF_READ(objPlane->getVertexTransferMode(), PURE_VT_VENDOR_BITS, 3), "plane 2") &
            assertEquals(vtExpected, objBox->getVertexTransferMode() & vtExpected, "box 1") &
            assertEquals(0u, BITF_READ(objBox->getVertexTransferMode(), PURE_VT_VENDOR_BITS, 3), "box 2") &
            assertEquals(vtExpected, objCube->getVertexTransferMode() & vtExpected, "cube 1") &
            assertEquals(0u, BITF_READ(objCube->getVertexTransferMode(), PURE_VT_VENDOR_BITS, 3), "cube 2");

        return b;
    }

    bool testSetVertexTransferMode()
    {
        const TPURE_VERTEX_TRANSFER_MODE vtExpectedObj = obj->getVertexTransferMode();
        bool b = assertTrue(obj->setVertexTransferMode( obj->getVertexTransferMode() ), "set obj"); // intentionally testing setting to the same
        b &= assertEquals(vtExpectedObj, obj->getVertexTransferMode(), "sva obj 1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedObjFromFile = objFromFile->getVertexTransferMode();
        b &= assertTrue(objFromFile->setVertexTransferMode( objFromFile->getVertexTransferMode() ), "set objFromFile"); // intentionally testing setting to the same
        b &= assertEquals(vtExpectedObjFromFile, objFromFile->getVertexTransferMode(), "sva objFromFile 1");

        // make sure the mode of the 2 objects is not just simple (PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT) (1by1 immediate mode)
        b &= assertNotEquals(PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT, obj->getVertexTransferMode(), "obj not dir 1by1");
        b &= assertNotEquals(PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT, objFromFile->getVertexTransferMode(), "objFromFile not dir 1by1");

        // subobjects must reject this when called by user
        // by default the selected transfer mode is NOT PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT hence we try set that but expect no change!
        PureObject3D* const subobj1Obj = (PureObject3D*)(obj->getAttachedAt(0));
        b &= assertFalse(subobj1Obj->setVertexTransferMode( PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT ), "set subobj1Obj");
        b &= assertEquals(vtExpectedObj, subobj1Obj->getVertexTransferMode(), "dir obj subobject");

        PureObject3D* const subobj1ObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
        b &= assertFalse(subobj1ObjFromFile->setVertexTransferMode( PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT ), "set subobj1ObjFromFile");
        b &= assertEquals(vtExpectedObjFromFile, subobj1ObjFromFile->getVertexTransferMode(), "dir objFromFile subobject");

        // cloned object must reject this in any case
        // by default the selected transfer mode is NOT PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT hence we try set that but expect no change!
        b &= assertFalse(objCloned->setVertexTransferMode( PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT ), "set objCloned");
        b &= assertEquals(vtExpectedObj, objCloned->getVertexTransferMode(), "dir obj cloned");

        b &= assertFalse(objFromFileCloned->setVertexTransferMode( PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT ), "set objFromFileCloned");
        b &= assertEquals(vtExpectedObjFromFile, objFromFileCloned->getVertexTransferMode(), "dir objFromFile cloned");

        // Generic server-side vertex arrays are supported by main test machine

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaIndSVAobj = PURE_VMOD_STATIC  | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedStaIndSVAobj ), "set obj sva");
        b &= assertEquals(vtExpectedStaIndSVAobj, obj->getVertexTransferMode(), "sva obj 2");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaIndSVAobjFromFile = PURE_VMOD_STATIC  | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedStaIndSVAobjFromFile ), "set objFromFile sva");
        b &= assertEquals(vtExpectedStaIndSVAobjFromFile, objFromFile->getVertexTransferMode(), "sva objFromFile 2");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynIndSVAobj = PURE_VMOD_DYNAMIC  | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynIndSVAobj ), "set obj sva 3");
        b &= assertEquals(vtExpectedDynIndSVAobj, obj->getVertexTransferMode(), "sva obj 3");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynIndSVAobjFromFile = PURE_VMOD_DYNAMIC  | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynIndSVAobjFromFile ), "set objFromFile sva 3");
        b &= assertEquals(vtExpectedDynIndSVAobjFromFile, objFromFile->getVertexTransferMode(), "sva objFromFile 3");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDirSVAobj = PURE_VMOD_DYNAMIC  | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynDirSVAobj ), "set obj sva 4");
        b &= assertEquals(vtExpectedDynDirSVAobj, obj->getVertexTransferMode(), "sva obj 4");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDirSVAobjFromFile = PURE_VMOD_DYNAMIC  | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynDirSVAobjFromFile ), "set objFromFile sva 4");
        b &= assertEquals(vtExpectedDynDirSVAobjFromFile, objFromFile->getVertexTransferMode(), "sva objFromFile 4");

        // following modes must be supported on every machine

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDir1by1obj = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT;
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynDir1by1obj ), "set obj 1b1");
        b &= assertEquals(vtExpectedDynDir1by1obj, obj->getVertexTransferMode(), "dir obj 1b1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDir1by1objFromFile = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT;
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynDir1by1objFromFile ), "set objFromFile 1b1");
        b &= assertEquals(vtExpectedDynDir1by1objFromFile, objFromFile->getVertexTransferMode(), "dir objFromFile 1b1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDirVAobj = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynDirVAobj ), "set obj rva");
        b &= assertEquals(vtExpectedDynDirVAobj, obj->getVertexTransferMode(), "dir obj rva");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDirVAobjFromFile = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynDirVAobjFromFile ), "set objFromFile rva");
        b &= assertEquals(vtExpectedDynDirVAobjFromFile, objFromFile->getVertexTransferMode(), "dir objFromFile rva");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynInd1by1obj = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED;
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynInd1by1obj ), "set obj 1b1");
        b &= assertEquals(vtExpectedDynInd1by1obj, obj->getVertexTransferMode(), "ind obj 1b1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynInd1by1objFromFile = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED;
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynInd1by1objFromFile ), "set objFromFile 1b1");
        b &= assertEquals(vtExpectedDynInd1by1objFromFile, objFromFile->getVertexTransferMode(), "ind objFromFile 1b1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynIndVAobj = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT);
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedDynIndVAobj ), "set obj rva");
        b &= assertEquals(vtExpectedDynIndVAobj, obj->getVertexTransferMode(), "ind obj rva");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynIndVAobjFromFile = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT);
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedDynIndVAobjFromFile ), "set objFromFile rv");
        b &= assertEquals(vtExpectedDynIndVAobjFromFile, objFromFile->getVertexTransferMode(), "ind objFromFile rva");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaDirDLobj = PURE_VMOD_STATIC | PURE_VREF_DIRECT;
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedStaDirDLobj ), "set obj DL");
        b &= assertEquals(vtExpectedStaDirDLobj, obj->getVertexTransferMode(), "dir obj DL");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaDirDLobjFromFile = PURE_VMOD_STATIC | PURE_VREF_DIRECT;
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedStaDirDLobjFromFile ), "set objFromFile DL");
        b &= assertEquals(vtExpectedStaDirDLobjFromFile, objFromFile->getVertexTransferMode(), "dir objFromFile DL");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaIndDLobj = PURE_VMOD_STATIC | PURE_VREF_INDEXED;
        b &= assertTrue(obj->setVertexTransferMode( vtExpectedStaIndDLobj ), "set obj DL");
        b &= assertEquals(vtExpectedStaIndDLobj, obj->getVertexTransferMode(), "ind obj DL");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaIndDLobjFromFile = PURE_VMOD_STATIC | PURE_VREF_INDEXED;
        b &= assertTrue(objFromFile->setVertexTransferMode( vtExpectedStaIndDLobjFromFile ), "set objFromFile DL");
        b &= assertEquals(vtExpectedStaIndDLobjFromFile, objFromFile->getVertexTransferMode(), "ind objFromFile DL");

        return b;
    }

    bool testGetLastTransferredVertexCount()
    {
        // non-initial values are checked in testTransferVertices()
        return assertEquals((TPureUInt)0, obj->getLastTransferredVertexCount(), "obj") &
            assertEquals((TPureUInt)0, objFromFile->getLastTransferredVertexCount(), "objFromFile");

    }

    bool testGetLastTransferredTriangleCount()
    {
        // non-initial values are checked in testTransferVertices()
        return assertEquals((TPureUInt)0, obj->getLastTransferredTriangleCount(), "obj") &
            assertEquals((TPureUInt)0, objFromFile->getLastTransferredTriangleCount(), "objFromFile");

    }

    bool testGetTransformedVertices()
    {
        return assertNotNull(objPlane->getTransformedVertices(), "plane") &
            assertNotNull(objBox->getTransformedVertices(), "box") &
            assertNotNull(objCube->getTransformedVertices(), "cube") &
            assertNull(objFromFile->getTransformedVertices(), "objFromFile") &
            assertEquals(((PureObject3D*)objPlane->getAttachedAt(0))->getTransformedVertices(), objPlane->getTransformedVertices(), "plane 2") &
            assertEquals(((PureObject3D*)objBox->getAttachedAt(0))->getTransformedVertices(), objBox->getTransformedVertices(), "box 2") &
            assertEquals(((PureObject3D*)objCube->getAttachedAt(0))->getTransformedVertices(), objCube->getTransformedVertices(), "cube 2") &
            assertNotEquals(((PureObject3D*)objFromFile->getAttachedAt(0))->getTransformedVertices(), objFromFile->getTransformedVertices(), "objFromFile 2") &
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
        const PureVector vUnit(1.0f, 1.0f, 1.0f);

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
        const TPureFloat fOriginalBiggestAreaScaledObj = obj->getBiggestAreaScaled();
        const TPureFloat fOriginalBiggestAreaScaledObjFromFile = objFromFile->getBiggestAreaScaled();

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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
        const PureVector vOriginalSubobjectProperty = subObjFromFile->getScaling();
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
        const PureVector vNewScaling(2.0f, 3.0f, 4.0f);
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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
        const PureVector vOriginalSubobjectProperty = subObjFromFile->getScaling();
        subObjFromFile->SetScaling(PureVector(1.0f, 1.0f, 1.0f));
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
        const TPureFloat fOriginalBiggestAreaScaledObj = obj->getBiggestAreaScaled();
        const TPureFloat fOriginalBiggestAreaScaledObjFromFile = objFromFile->getBiggestAreaScaled();

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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
        const PureVector vOriginalSubobjectProperty = subObjFromFile->getScaling();
        subObjFromFile->Scale(2.0f);
        b &= assertTrue(vOriginalSubobjectProperty == subObjFromFile->getScaling(), "property unchanged for subobject");

        return b;
    }

    bool testScaleByVector()
    {
        const PureVector vScaleBy(2.0f, 3.0f, 4.0f);

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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
        const PureVector vOriginalSubobjectProperty = subObjFromFile->getScaling();
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
        
        TPureFloat fObjFileAreaXY = objFromFile->getSizeVec().getX() * objFromFile->getSizeVec().getY();
        TPureFloat fObjFileAreaXZ = objFromFile->getSizeVec().getX() * objFromFile->getSizeVec().getZ();
        TPureFloat fObjFileAreaYZ = objFromFile->getSizeVec().getY() * objFromFile->getSizeVec().getZ();

        TPureFloat fObjFileExpectedBiggestArea;
        if ( fObjFileAreaXY > fObjFileAreaXZ )
        {
            fObjFileExpectedBiggestArea = fObjFileAreaXY > fObjFileAreaYZ ? fObjFileAreaXY : fObjFileAreaYZ;
        }
        else
        {
            fObjFileExpectedBiggestArea = fObjFileAreaXZ > fObjFileAreaYZ ? fObjFileAreaXZ : fObjFileAreaYZ;
        }

        b &= assertEquals(fObjFileExpectedBiggestArea, objFromFile->getBiggestAreaScaled(), 0.01f, "objFromFile 1");

        const PureVector vScaleBy(2.0f, 3.0f, 4.0f);
        objFromFile->Scale(vScaleBy);

        fObjFileAreaXY *= vScaleBy.getX() * vScaleBy.getY();
        fObjFileAreaXZ *= vScaleBy.getX() * vScaleBy.getZ();
        fObjFileAreaYZ *= vScaleBy.getY() * vScaleBy.getZ();

        TPureFloat fObjFileExpectedBiggestAreaScaled;
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
        const TPureFloat fObjAreaBiggest = obj->getBiggestAreaScaled();
        const TPureFloat fObjFromFileAreaBiggest = objFromFile->getBiggestAreaScaled();

        bool b = assertEquals(fObjAreaBiggest, obj->recalculateBiggestAreaScaled(), E, "obj") &
            assertEquals(fObjFromFileAreaBiggest, objFromFile->recalculateBiggestAreaScaled(), E, "objFromFile");

        // LAZY: we could also test if we change vertex geometry and upload it again to host with setVertexTransferMode()
        // and expect the area to change automatically, but for now we just believe it is also ok ...

        return b;
    }

    bool testIsVisible()
    {
        return assertTrue(objPlane->isRenderingAllowed(), "plane") &
            assertTrue(objFromFile->isRenderingAllowed(), "objFromFile") &
            assertTrue(objBox->isRenderingAllowed(), "box") &
            assertTrue(objCube->isRenderingAllowed(), "cube");
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

        obj->SetRenderingAllowed(false);
        objFromFile->SetRenderingAllowed(false);

        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 2") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) != om->get3dOpaqueOccludees().end(), "objFromFile is in getOpaqueOccludees 2") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) == om->get3dBlendedOccludees().end(), "objFromFile is NOT in get3dBlendedOccludees 2") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 2") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 2");

        return b & assertFalse(obj->isRenderingAllowed(), "obj") & assertFalse(objFromFile->isRenderingAllowed(), "objFromFile");
    }

    bool testShow()
    {
        objFromFile->SetOccluder(true);

        obj->SetRenderingAllowed(false);
        obj->Show();

        objFromFile->SetRenderingAllowed(false);
        objFromFile->Show();

        return assertTrue(obj->isRenderingAllowed(), "obj") & assertTrue(objFromFile->isRenderingAllowed(), "objFromFile") &
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

        return b & assertFalse(obj->isRenderingAllowed(), "obj") & assertFalse(objFromFile->isRenderingAllowed(), "objFromFile");
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
        return assertEquals(PURE_YXZ, obj->getRotationOrder(), "obj") &
            assertEquals(PURE_YXZ, objFromFile->getRotationOrder(), "objFromFile") &
            assertEquals(PURE_YXZ, objPlane->getRotationOrder(), "plane") &
            assertEquals(PURE_YXZ, objBox->getRotationOrder(), "box") &
            assertEquals(PURE_YXZ, objCube->getRotationOrder(), "cube");
    }

    bool testSetRotationOrder()
    {
        obj->SetRotationOrder(PURE_XYZ);
        objFromFile->SetRotationOrder(PURE_XYZ);

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
        const TPURE_ROTATION_ORDER vOriginalSubobjectProperty = subObjFromFile->getRotationOrder();
        subObjFromFile->SetRotationOrder(PURE_XYZ);
        bool b = assertTrue(vOriginalSubobjectProperty == subObjFromFile->getRotationOrder(), "property unchanged for subobject");

        return b & assertEquals(PURE_XYZ, obj->getRotationOrder(), "obj") & assertEquals(PURE_XYZ, objFromFile->getRotationOrder(), "objFromFile");
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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
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

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
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

        objFromFile->getMaterial().setBlendMode(PURE_BM_STANDARD_TRANSPARENCY);
        objFromFile->SetOccluder(true);
        b &= assertFalse(objFromFile->isOccluder(), "objFromFile is not occluder 6") &
            assertTrue(std::find(om->getOccluders().begin(), om->getOccluders().end(), objFromFile) == om->getOccluders().end(), "objFromFile is NOT in getOccluders 6") &
            assertTrue(std::find(om->get3dOpaqueOccludees().begin(), om->get3dOpaqueOccludees().end(), objFromFile) == om->get3dOpaqueOccludees().end(), "objFromFile is NOT in getOpaqueOccludees 6") &
            assertTrue(std::find(om->get3dBlendedOccludees().begin(), om->get3dBlendedOccludees().end(), objFromFile) != om->get3dBlendedOccludees().end(), "objFromFile is in get3dBlendedOccludees 6") &
            assertTrue(std::find(om->get2dOpaqueOccludees().begin(), om->get2dOpaqueOccludees().end(), objFromFile) == om->get2dOpaqueOccludees().end(), "objFromFile is NOT in get2dOpaqueOccludees 6") &
            assertTrue(std::find(om->get2dBlendedOccludees().begin(), om->get2dBlendedOccludees().end(), objFromFile) == om->get2dBlendedOccludees().end(), "objFromFile is NOT in get2dBlendedOccludees 6");
        objFromFile->getMaterial().setBlendMode(PURE_BM_NONE);

        objFromFile->SetOccluder(true);

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
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
        objCloned->SetOcclusionTested(true);

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

        obj->SetOcclusionTested(false);
        objFromFile->SetOcclusionTested(false);

        PureObject3D* const subObjFromFile = (PureObject3D*)(objFromFile->getAttachedAt(0));
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
        // just to be sure
        obj->SetOcclusionTested(false);
        objFromFile->SetOcclusionTested(true);

        // intentionally drawing both twice, to check if draw() properly resets the counters before transferring vertices!
        obj->draw(PURE_RPASS_NORMAL, false, false);
        objFromFile->draw(PURE_RPASS_NORMAL, false, false);
        TPureUInt nObjLastTransferredVertices = obj->draw(PURE_RPASS_NORMAL, false, false);
        TPureUInt nObjFromFileLastTransferredVertices = objFromFile->draw(PURE_RPASS_NORMAL, false, false);

        bool b = assertEquals(obj->getVertexIndicesCount(), obj->getLastTransferredVertexCount(), "obj 1") &
            assertEquals(objFromFile->getVertexIndicesCount(), objFromFile->getLastTransferredVertexCount(), "objFromFile 1") &
            assertEquals(nObjLastTransferredVertices, obj->getLastTransferredVertexCount(), "obj 2") &
            assertEquals(nObjFromFileLastTransferredVertices, objFromFile->getLastTransferredVertexCount(), "objFromFile 2") &
            assertEquals(obj->getTriangleCount(), obj->getLastTransferredTriangleCount(), "obj 3") &
            assertEquals(objFromFile->getTriangleCount(), objFromFile->getLastTransferredTriangleCount(), "objFromFile 3");

        // hide the objects and expect their counters to be zero after draw (so they still reset their counters before deciding not drawing anything)
        obj->SetRenderingAllowed(false);
        objFromFile->SetRenderingAllowed(false);
        nObjLastTransferredVertices = obj->draw(PURE_RPASS_NORMAL, false, false);
        nObjFromFileLastTransferredVertices = objFromFile->draw(PURE_RPASS_NORMAL, false, false);

        b &= assertEquals(0u, obj->getLastTransferredVertexCount(), "obj 1 hidden 1") &
            assertEquals(0u, objFromFile->getLastTransferredVertexCount(), "objFromFile 1 hidden 1") &
            assertEquals(nObjLastTransferredVertices, obj->getLastTransferredVertexCount(), "obj 2 hidden 1") &
            assertEquals(nObjFromFileLastTransferredVertices, objFromFile->getLastTransferredVertexCount(), "objFromFile 2 hidden 1") &
            assertEquals(0u, obj->getLastTransferredTriangleCount(), "obj 3 hidden 1") &
            assertEquals(0u, objFromFile->getLastTransferredTriangleCount(), "objFromFile 3 hidden 1");
        
        // check the same with PURE_RPASS_START_OCCLUSION_QUERY render pass
        nObjLastTransferredVertices = obj->draw(PURE_RPASS_NORMAL, false, false);
        nObjFromFileLastTransferredVertices = objFromFile->draw(PURE_RPASS_NORMAL, false, false);

        b &= assertEquals(0u, obj->getLastTransferredVertexCount(), "obj 1 hidden 2") &
            assertEquals(0u, objFromFile->getLastTransferredVertexCount(), "objFromFile 1 hidden 2") &
            assertEquals(nObjLastTransferredVertices, obj->getLastTransferredVertexCount(), "obj 2 hidden 2") &
            assertEquals(nObjFromFileLastTransferredVertices, objFromFile->getLastTransferredVertexCount(), "objFromFile 2 hidden 2") &
            assertEquals(0u, obj->getLastTransferredTriangleCount(), "obj 3 hidden 2") &
            assertEquals(0u, objFromFile->getLastTransferredTriangleCount(), "objFromFile 3 hidden 2");

        // let objects be rendered again
        obj->SetRenderingAllowed(true);
        objFromFile->SetRenderingAllowed(true);

        // make sure the objects are far behind the camera
        obj->getPosVec().SetZ(-100.f);
        objFromFile->getPosVec().SetZ(-100.f);

        nObjLastTransferredVertices = obj->draw(PURE_RPASS_START_OCCLUSION_QUERY, false, false);
        nObjFromFileLastTransferredVertices = objFromFile->draw(PURE_RPASS_START_OCCLUSION_QUERY, false, false);

        b &= assertEquals((TPureUInt)0, obj->getLastTransferredVertexCount(), "obj 4") &
            assertEquals((TPureUInt)24, objFromFile->getLastTransferredVertexCount(), "objFromFile 4") &
            assertEquals(nObjLastTransferredVertices, obj->getLastTransferredVertexCount(), "obj 5") &
            assertEquals(nObjFromFileLastTransferredVertices, objFromFile->getLastTransferredVertexCount(), "objFromFile 5") &
            assertEquals((TPureUInt)0, obj->getLastTransferredTriangleCount(), "obj 6") &
            assertEquals((TPureUInt)12, objFromFile->getLastTransferredTriangleCount(), "objFromFile 6");
        
        nObjLastTransferredVertices = obj->draw(PURE_RPASS_NORMAL, false, false);
        nObjFromFileLastTransferredVertices = objFromFile->draw(PURE_RPASS_NORMAL, false, false);

        // expecting all zeros for objFromFile due to behind the camera and occlusion test fail, but
        // expecting same positive values as before for obj because it is not occlusion tested!
        b &= assertEquals(obj->getVertexIndicesCount(), obj->getLastTransferredVertexCount(), "obj 7") &
            assertEquals((TPureUInt)0, objFromFile->getLastTransferredVertexCount(), "objFromFile 7") &
            assertEquals(nObjLastTransferredVertices, nObjLastTransferredVertices, "obj 8") &
            assertEquals((TPureUInt)0, nObjFromFileLastTransferredVertices, "objFromFile 8") &
            assertEquals(obj->getTriangleCount(), obj->getLastTransferredTriangleCount(), "obj 9") &
            assertEquals((TPureUInt)0, objFromFile->getLastTransferredTriangleCount(), "objFromFile 9");

        // debug bounding box must be rendered regardless of object occlusion state, but
        // it must not be rendered for an object which is not occlusion tested
        nObjLastTransferredVertices = obj->draw(PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY, false, false);
        nObjFromFileLastTransferredVertices = objFromFile->draw(PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY, false, false);

        b &= assertEquals((TPureUInt)0, obj->getLastTransferredVertexCount(), "obj 10") &
            assertEquals((TPureUInt)24, objFromFile->getLastTransferredVertexCount(), "objFromFile 10") &
            assertEquals(nObjLastTransferredVertices, obj->getLastTransferredVertexCount(), "obj 11") &
            assertEquals(nObjFromFileLastTransferredVertices, objFromFile->getLastTransferredVertexCount(), "objFromFile 11") &
            assertEquals((TPureUInt)0, obj->getLastTransferredTriangleCount(), "obj 12") &
            assertEquals((TPureUInt)12, objFromFile->getLastTransferredTriangleCount(), "objFromFile 12");

        return b;
    } // testDraw()

    bool testDraw_Cloned()
    {
        // make sure the objects are far behind the camera
        objFromFile->getPosVec().SetZ(-100.f);
        objFromFileCloned->getPosVec().SetZ(-100.f);

        // just to be sure
        objFromFile->SetOcclusionTested(true);
        objFromFileCloned->SetOcclusionTested(true);

        TPureUInt nObjFromFileClonedLastTransferredVertices = objFromFileCloned->draw(PURE_RPASS_NORMAL, false, false);
        objFromFileCloned->draw(PURE_RPASS_NORMAL, false, false);

        bool b = assertEquals(objFromFileCloned->getVertexIndicesCount(), objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 1") &
            assertEquals(nObjFromFileClonedLastTransferredVertices, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 2") &
            assertEquals(nObjFromFileClonedLastTransferredVertices, objFromFile->getLastTransferredVertexCount(), "objFromFile 1") &
            assertEquals(objFromFileCloned->getTriangleCount(), objFromFileCloned->getLastTransferredTriangleCount(), "objFromFileCloned 3") &
            assertEquals(objFromFileCloned->getLastTransferredTriangleCount(), objFromFile->getLastTransferredTriangleCount(), "objFromFile 2");

        // check if cloned object also properly resets counters to 0 when trying to draw it when it is not allowed to be drawn
        objFromFileCloned->SetRenderingAllowed(false);
        objFromFileCloned->draw(PURE_RPASS_NORMAL, false, false);

        b &= assertEquals(0u, objFromFile->getLastTransferredVertexCount(), "objFromFile 1 hidden 1") &
            assertEquals(0u, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 1 hidden 1") &
            assertEquals(0u, objFromFile->getLastTransferredVertexCount(), "objFromFile 2 hidden 1") &
            assertEquals(0u, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 2 hidden 1") &
            assertEquals(0u, objFromFile->getLastTransferredTriangleCount(), "objFromFile 3 hidden 1") &
            assertEquals(0u, objFromFileCloned->getLastTransferredTriangleCount(), "objFromFileCloned 3 hidden 1");

        // allow draw again
        objFromFileCloned->SetRenderingAllowed(true);

        nObjFromFileClonedLastTransferredVertices = objFromFileCloned->draw(PURE_RPASS_START_OCCLUSION_QUERY, false, false);
        objFromFileCloned->getLastTransferredVertexCount();

        b &= assertEquals(24u, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 4") &
            assertEquals(nObjFromFileClonedLastTransferredVertices, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 5") &
            assertEquals(24u, objFromFile->getLastTransferredVertexCount(), "objFromFile 3") &
            assertEquals(12u, objFromFileCloned->getLastTransferredTriangleCount(), "objFromFileCloned 6") &
            assertEquals(12u, objFromFile->getLastTransferredTriangleCount(), "objFromFile 4");
        
        nObjFromFileClonedLastTransferredVertices = objFromFileCloned->draw(PURE_RPASS_NORMAL, false, false);

        // expecting all zeros for objFromFileCloned due to behind the camera and occlusion test fail
        b &= assertEquals(0u, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 7") &
            assertEquals(nObjFromFileClonedLastTransferredVertices, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 8") &
            assertEquals(0u, objFromFile->getLastTransferredVertexCount(), "objFromFile 5") &
            assertEquals(0u, objFromFileCloned->getLastTransferredTriangleCount(), "objFromFileCloned 9") &
            assertEquals(0u, objFromFile->getLastTransferredTriangleCount(), "objFromFile 6");

        // debug bounding box must be rendered regardless of object occlusion state
        nObjFromFileClonedLastTransferredVertices = objFromFileCloned->draw(PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY, false, false);

        b &= assertEquals(24u, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 10") &
            assertEquals(nObjFromFileClonedLastTransferredVertices, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 11") &
            assertEquals(24u, objFromFile->getLastTransferredVertexCount(), "objFromFile 7") &
            assertEquals(12u, objFromFileCloned->getLastTransferredTriangleCount(), "objFromFileCloned 12") &
            assertEquals(12u, objFromFile->getLastTransferredTriangleCount(), "objFromFile 8");

        // debug bounding box must NOT be rendered if occlusion test is off
        objFromFileCloned->SetOcclusionTested(false);
        
        nObjFromFileClonedLastTransferredVertices = objFromFileCloned->draw(PURE_RPASS_BOUNDING_BOX_DEBUG_FOR_OCCLUSION_QUERY, false, false);

        b &= assertEquals(0u, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 13") &
            assertEquals(nObjFromFileClonedLastTransferredVertices, objFromFileCloned->getLastTransferredVertexCount(), "objFromFileCloned 14") &
            assertEquals(0u, objFromFile->getLastTransferredVertexCount(), "objFromFile 9") &
            assertEquals(0u, objFromFileCloned->getLastTransferredTriangleCount(), "objFromFileCloned 15") &
            assertEquals(0u, objFromFile->getLastTransferredTriangleCount(), "objFromFile 10");

        return b;
    } // testDraw_Cloned()

    bool testGetUsedSystemMemory()
    {
        return assertGreater(obj->getUsedSystemMemory(),          sizeof(PureObject3D) + obj->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),         "obj") &
               assertGreater(objFromFile->getUsedSystemMemory(),  sizeof(PureObject3D) + objFromFile->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ), "objFromFile") &
               assertGreater(objPlane->getUsedSystemMemory(),     sizeof(PureObject3D) + objPlane->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),    "plane") &
               assertGreater(objBox->getUsedSystemMemory(),       sizeof(PureObject3D) + objBox->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),      "box") &
               assertGreater(objCube->getUsedSystemMemory(),      sizeof(PureObject3D) + objCube->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),     "cube");
    }

    bool testGetUsedVideoMemory()
    {
        TPureUInt objFromFileMinMemUsage = 0;
        for (TPureInt i = 0; i < objFromFile->getCount(); i++)
        {
            const PureObject3D* const submesh = (PureObject3D*) objFromFile->getAttachedAt(i);
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
        const bool b = assertEquals(objCloned->getUsedVideoMemory(), (TPureUInt) 0,          "objCloned") &
            assertGreater(obj->getUsedVideoMemory(),                 (TPureUInt) 0,          "obj") &
            assertGreater(objFromFile->getUsedVideoMemory(),         objFromFileMinMemUsage, "objFromFile") &
            assertGreater(objPlane->getUsedVideoMemory(),            (TPureUInt) 0,          "plane") &
            assertGreater(objBox->getUsedVideoMemory(),              (TPureUInt) 0,          "box") &
            assertGreater(objCube->getUsedVideoMemory(),             (TPureUInt) 0,          "cube");
        
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
        const TPureInt prevSize = obj->getSize();
        obj->PreAlloc(10);

        return assertEquals(prevSize + 10, obj->getSize());
    }

    bool testHasAttached2()
    {
        PureObject3D* const mgd1 = om->createCube(1);
        mgd1->DetachFrom();
        obj->Attach(*mgd1);

        return assertFalse(obj->hasAttached(*objPlane), "objPlane") &
               assertTrue(obj->hasAttached(*mgd1), "mgd1");
    }

    bool testAttach()
    {
        PureObject3D& mgr1 = *(om->createCube(1));
        PureObject3D& mgr2 = *(om->createCube(1));
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
        PureObject3D& mgd = *((PureObject3D*) (obj->getAttachedAt(0)));
        obj->Detach(mgd);
        bool l = assertEquals(0, obj->getCount());
        obj->Attach(mgd);

        return l;
    }

    bool testDeleteAttachedInstance()
    {
        PureObject3D& mgd = *((PureObject3D*) (obj->getAttachedAt(0)));
        obj->DeleteAttachedInstance(mgd);

        return assertEquals(0, obj->getCount());
    }

    bool testDeleteAll()
    {
        PureObject3D* const mgd1 = om->createCube(1);
        PureObject3D* const mgd2 = om->createCube(1);

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

}; // class PureObject3DTest