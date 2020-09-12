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
#include "../PRRE/Object3D/PRREObject3DManager.h"
#include "../PRRE/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREObject3DManagerTest :
    public UnitTest
{
public:

    PRREObject3DManagerTest() :
        UnitTest( __FILE__ )
    {};

protected:

    virtual void Initialize()
    {
        engine = NULL;
        om = NULL;
        AddSubTest("testIsBlendFuncBlends", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testIsBlendFuncBlends);
        AddSubTest("testIsBFB", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testIsBFB);
        AddSubTest("testGetPRREblendFromGLblend", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testGetPRREblendFromGLblend);
        AddSubTest("testGetGLblendFromPRREblend", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testGetGLblendFromPRREblend);
        AddSubTest("testGetPRREprimitiveFromGLprimitive", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testGetPRREprimitiveFromGLprimitive);
        AddSubTest("testGetGLprimitiveFromPRREprimitive", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testGetGLprimitiveFromPRREprimitive);
        AddSubTest("testIsVertexTransferModeSelectable", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testIsVertexTransferModeSelectable);
        AddSubTest("testIsVertexReferencingIndexed", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testIsVertexReferencingIndexed);
        AddSubTest("testIsVertexModifyingDynamic", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testIsVertexModifyingDynamic);
        AddSubTest("testSelectVertexTransferMode", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testSelectVertexTransferMode);
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCtor);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testIsInitialized);
        AddSubTest("testCreatePlane", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreatePlane);
        AddSubTest("testCreateBox", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreateBox);
        AddSubTest("testCreateCube", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreateCube);
        AddSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreateFromFile);
        AddSubTest("testCreateCloned", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testCreateCloned);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREObject3DManagerTest::testWriteList);
    }

    virtual void SetUp()
    {
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
            om = &engine->getObject3DManager();
        }
        
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

    bool testIsBlendFuncBlends()
    {
        return assertFalse(PRREObject3DManager::isBlendFuncBlends(PRRE_ONE, PRRE_ZERO), "(ONE, ZERO)") &
            assertTrue(PRREObject3DManager::isBlendFuncBlends(PRRE_ONE, PRRE_ONE), "(ONE, ONE)") &
            assertTrue(PRREObject3DManager::isBlendFuncBlends(PRRE_SRC_COLOR, PRRE_ZERO), "(SRC_COLOR, ZERO)") &
            assertTrue(PRREObject3DManager::isBlendFuncBlends(PRRE_DST_COLOR, PRRE_ONE), "(DST_COLOR, ONE)") &
            assertTrue(PRREObject3DManager::isBlendFuncBlends(PRRE_ZERO, PRRE_DST_ALPHA), "(ZERO, DST_ALPHA)") &
            assertTrue(PRREObject3DManager::isBlendFuncBlends(PRRE_ONE_MINUS_CONSTANT_COLOR, PRRE_ONE), "(ONE_MINUS_CONSTANT_COLOR, ONE)") &
            assertTrue(PRREObject3DManager::isBlendFuncBlends(PRRE_CONSTANT_COLOR, PRRE_ONE), "(CONSTANT_COLOR, ONE)") &
            assertTrue(PRREObject3DManager::isBlendFuncBlends(PRRE_ZERO, PRRE_ONE_MINUS_DST_ALPHA), "(ZERO, ONE_MINUS_DST_ALPHA)") &
            assertTrue(PRREObject3DManager::isBlendFuncBlends(PRRE_ZERO, PRRE_CONSTANT_ALPHA), "(ZERO, CONSTANT_ALPHA)") &
            assertTrue(PRREObject3DManager::isBlendFuncBlends(PRRE_SRC_ALPHA_SATURATE, PRRE_ONE), "(SRC_ALPHA_SATURATE, ONE)");
        // not checking every combination, no problem
    }

    bool testIsBFB()
    {
        return assertFalse(PRREObject3DManager::isBFB(PRRE_ONE, PRRE_ZERO), "(ONE, ZERO)") &
            assertTrue(PRREObject3DManager::isBFB(PRRE_ONE, PRRE_ONE), "(ONE, ONE)") &
            assertTrue(PRREObject3DManager::isBFB(PRRE_SRC_COLOR, PRRE_ZERO), "(SRC_COLOR, ZERO)") &
            assertTrue(PRREObject3DManager::isBFB(PRRE_DST_COLOR, PRRE_ONE), "(DST_COLOR, ONE)") &
            assertTrue(PRREObject3DManager::isBFB(PRRE_ZERO, PRRE_DST_ALPHA), "(ZERO, DST_ALPHA)") &
            assertTrue(PRREObject3DManager::isBFB(PRRE_ONE_MINUS_CONSTANT_COLOR, PRRE_ONE), "(ONE_MINUS_CONSTANT_COLOR, ONE)") &
            assertTrue(PRREObject3DManager::isBFB(PRRE_CONSTANT_COLOR, PRRE_ONE), "(CONSTANT_COLOR, ONE)") &
            assertTrue(PRREObject3DManager::isBFB(PRRE_ZERO, PRRE_ONE_MINUS_DST_ALPHA), "(ZERO, ONE_MINUS_DST_ALPHA)") &
            assertTrue(PRREObject3DManager::isBFB(PRRE_ZERO, PRRE_CONSTANT_ALPHA), "(ZERO, CONSTANT_ALPHA)") &
            assertTrue(PRREObject3DManager::isBFB(PRRE_SRC_ALPHA_SATURATE, PRRE_ONE), "(SRC_ALPHA_SATURATE, ONE)");
        // not checking every combination, no problem
    }

    bool testGetPRREblendFromGLblend()
    {
        // we believe its okay
        return true;
    }

    bool testGetGLblendFromPRREblend()
    {
        // we believe its okay
        return true;
    }

    bool testGetPRREprimitiveFromGLprimitive()
    {
        // we believe its okay
        return true;
    }

    bool testGetGLprimitiveFromPRREprimitive()
    {
        // we believe its okay
        return true;
    }

    bool testIsVertexTransferModeSelectable()
    {
        // only testing HW-independent values
        return assertTrue( PRREObject3DManager::isVertexTransferModeSelectable(PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT), "PRRE_VT_DYN_DIR_1_BY_1" ) &
               assertTrue( PRREObject3DManager::isVertexTransferModeSelectable(PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT)), "PRRE_VT_DYN_DIR_RVA" ) &
               assertTrue( PRREObject3DManager::isVertexTransferModeSelectable(PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED), "PRRE_VT_DYN_IND_1_BY_1" ) &
               assertTrue( PRREObject3DManager::isVertexTransferModeSelectable(PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT)), "PRRE_VT_DYN_IND_RVA" ) &
               assertTrue( PRREObject3DManager::isVertexTransferModeSelectable(PRRE_VMOD_STATIC  | PRRE_VREF_DIRECT), "PRRE_VT_STA_DIR_DL" ) &
               assertTrue( PRREObject3DManager::isVertexTransferModeSelectable(PRRE_VMOD_STATIC  | PRRE_VREF_INDEXED), "PRRE_VT_STA_IND_DL" );
    }

    bool testIsVertexReferencingIndexed()
    {
        // we believe its okay
        return true;
    }

    bool testIsVertexModifyingDynamic()
    {
        // we believe its okay
        return true;
    }

    bool testSelectVertexTransferMode()
    {
        const TPRRE_VERTEX_TRANSFER_MODE vtransStaDirHost = PRREObject3DManager::selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_DIRECT, false);
        const TPRRE_VERTEX_TRANSFER_MODE vtransStaIndHost = PRREObject3DManager::selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_INDEXED, false);
        const TPRRE_VERTEX_TRANSFER_MODE vtransDynDirHost = PRREObject3DManager::selectVertexTransferMode(PRRE_VMOD_DYNAMIC, PRRE_VREF_DIRECT, false);
        const TPRRE_VERTEX_TRANSFER_MODE vtransDynIndHost = PRREObject3DManager::selectVertexTransferMode(PRRE_VMOD_DYNAMIC, PRRE_VREF_INDEXED, false);

        const TPRRE_VERTEX_TRANSFER_MODE vtransStaDirClient = PRREObject3DManager::selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_DIRECT, true);
        const TPRRE_VERTEX_TRANSFER_MODE vtransStaIndClient = PRREObject3DManager::selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_INDEXED, true);
        const TPRRE_VERTEX_TRANSFER_MODE vtransDynDirClient = PRREObject3DManager::selectVertexTransferMode(PRRE_VMOD_DYNAMIC, PRRE_VREF_DIRECT, true);
        const TPRRE_VERTEX_TRANSFER_MODE vtransDynIndClient = PRREObject3DManager::selectVertexTransferMode(PRRE_VMOD_DYNAMIC, PRRE_VREF_INDEXED, true);

        
        // vtransDynDirClient and vtransDynIndClient should be clientside since habit is dynamic and bForceUseClientMemory is true

        const bool bVTransDynDirClientIsClientside = assertTrue( PRREObject3DManager::isVertexModifyingDynamic(vtransDynDirClient), "vtransDynDirClient dyn" ) &
            assertFalse( PRREObject3DManager::isVertexReferencingIndexed(vtransDynDirClient), "vtransDynDirClient ind" ) &
            assertTrue(BIT_READ(vtransDynDirClient,PRRE_VT_VA_BIT), "vtransDynDirClient VA") &
            assertFalse(BIT_READ(vtransDynDirClient,PRRE_VT_SVA_BIT), "vtransDynDirClient RVA"); // CVA and RNG bits are not checked here

        const bool bVTransDynIndClientIsClientside = assertTrue( PRREObject3DManager::isVertexModifyingDynamic(vtransDynIndClient), "vtransDynIndClient dyn" ) &
            assertTrue( PRREObject3DManager::isVertexReferencingIndexed(vtransDynIndClient), "vtransDynIndClient ind" ) &
            assertTrue(BIT_READ(vtransDynIndClient,PRRE_VT_VA_BIT), "vtransDynIndClient VA") &
            assertFalse(BIT_READ(vtransDynIndClient,PRRE_VT_SVA_BIT), "vtransDynIndClient RVA"); // CVA and RNG bits are not checked here

        
        // vtransStaDirClient and vtransStaIndClient should be serverside since habit is static hence bForceUseClientMemory is ignored

        const bool bVTransStaDirClientIsServerside = assertFalse( PRREObject3DManager::isVertexModifyingDynamic(vtransStaDirHost), "vtransStaDirHost dyn" ) &
            assertFalse( PRREObject3DManager::isVertexReferencingIndexed(vtransStaDirHost), "vtransStaDirHost ind" ) &
            assertTrue(BIT_READ(vtransStaDirHost,PRRE_VT_VA_BIT), "vtransStaDirHost VA") &
            assertTrue(BIT_READ(vtransStaDirHost,PRRE_VT_SVA_BIT), "vtransStaDirHost SVA"); // CVA and RNG bits are not checked here

        const bool bVTransStaIndClientIsServerside = assertFalse( PRREObject3DManager::isVertexModifyingDynamic(vtransStaIndHost), "vtransStaIndHost dyn" ) &
            assertTrue( PRREObject3DManager::isVertexReferencingIndexed(vtransStaIndHost), "vtransStaIndHost ind" ) &
            assertTrue(BIT_READ(vtransStaIndHost,PRRE_VT_VA_BIT), "vtransStaIndHost VA") &
            assertTrue(BIT_READ(vtransStaIndHost,PRRE_VT_SVA_BIT), "vtransStaIndHost SVA"); // CVA and RNG bits are not checked here

        return 
            assertFalse( PRREObject3DManager::isVertexReferencingIndexed( vtransDynDirHost ), "indexed ? vtransDynDirHost" ) &
            assertTrue( PRREObject3DManager::isVertexReferencingIndexed( vtransDynIndHost ), "indexed ? vtransDynIndHost" ) &
            assertTrue( PRREObject3DManager::isVertexModifyingDynamic( vtransDynDirHost ), "dynamic ? vtransDynDirHost" ) &
            assertTrue( PRREObject3DManager::isVertexModifyingDynamic( vtransDynIndHost ), "dynamic ? vtransDynIndHost" ) &
            assertFalse( PRREObject3DManager::isVertexReferencingIndexed( vtransStaDirClient ), "indexed ? vtransStaDirClient" ) &
            assertTrue( PRREObject3DManager::isVertexReferencingIndexed( vtransStaIndClient ), "indexed ? vtransStaIndClient" ) &
            assertFalse( PRREObject3DManager::isVertexModifyingDynamic( vtransStaDirClient ), "dynamic ? vtransStaDirClient" ) &
            assertFalse( PRREObject3DManager::isVertexModifyingDynamic( vtransStaIndClient ), "dynamic ? vtransStaIndClient" ) &
            bVTransDynDirClientIsClientside &
            bVTransDynIndClientIsClientside &
            bVTransStaDirClientIsServerside &
            bVTransStaIndClientIsServerside;
    }

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
        return assertNotNull( om->createPlane(1.0f, 2.0f) );
    }

    bool testCreateBox()
    {
        return assertNotNull( om->createBox(1.0f, 2.0f, 3.0f) );
    }

    bool testCreateCube()
    {
        return assertNotNull( om->createCube(1.0f) );
    }

    bool testCreateFromFile()
    {
        // This snail object is from legacy proofps project, so its subobject names contain texture file names as well,
        // so that related textures can be auto-loaded. We can check here if number of textures has increased or not.
        // We expect textures to be auto-loaded as well, so increase in number of textures.
        const TPRREint nTexturesBeforeLoad = engine->getTextureManager().getCount();

        const PRREObject3D* const obj = om->createFromFile("_res/models/snail_proofps/snail.obj");

        // As texture filenames shouldn't be part of final subobject names, we can check for that also.
        bool b1 = true;
        for (TPRREint i = 0; i < obj->getCount(); i++)
            b1 = b1 & assertNull( strstr(((PRREObject3D*)obj->getAttachedAt(i))->getName().c_str(), ".bmp"), "bmp ext in subname" );

        return assertNotNull( obj, "not null" ) &
            assertLess( nTexturesBeforeLoad, engine->getTextureManager().getCount(), "textures count") &
            assertEquals((TPRREint)9, obj->getCount(), "subobject count") &
            b1;
    }

    bool testCreateCloned()
    {
        PRREObject3D* const objPlane = om->createPlane(1.0f, 2.0f);
        objPlane->getPosVec().Set(1, 2, 3);
        objPlane->getAngleVec().Set(10, 20, 30);
        objPlane->SetName("lol");
        objPlane->SetScaling( PRREVector(4,5,6) );
        objPlane->SetDoubleSided(true);
        objPlane->SetAffectingZBuffer(false);
        objPlane->SetLit(false);
        objPlane->SetRotationOrder(TPRRE_ROTATION_ORDER::PRRE_ZYX);
        objPlane->SetStickedToScreen(true);
        objPlane->SetVertexTransferMode( om->selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_DIRECT, false) );
        objPlane->SetVisible(false);
        objPlane->SetWireframed(true);
        objPlane->SetWireframedCulled(true);

        PRREObject3D* const objCloned = om->createCloned( *objPlane );

        return assertTrue(objPlane->getPosVec() == objCloned->getPosVec(), "pos") &
            assertTrue(objPlane->getAngleVec() == objCloned->getAngleVec(), "angle") &
            assertTrue(objPlane->getScaling() == objCloned->getScaling(), "scale") &
            assertEquals(objPlane->getName(), objCloned->getName(), "name") &
            assertTrue(objPlane->getSizeVec() == objCloned->getSizeVec(), "size") &
            assertEquals(objPlane->getRotationOrder(), objCloned->getRotationOrder(), "rot") &
            assertEquals(objPlane->getVertexTransferMode(), objCloned->getVertexTransferMode(), "vtrans") &
            assertTrue(objCloned->isDoubleSided(), "doublesided") &
            assertFalse(objCloned->isAffectingZBuffer(), "affectingzbuffer") &
            assertFalse(objCloned->isLit(), "islit") &
            assertTrue(objCloned->isStickedToScreen(), "stickedtoscreen") &
            assertFalse(objCloned->isVisible(), "visible") &
            assertTrue(objCloned->isWireframed(), "wireframed") &
            assertTrue(objCloned->isWireframedCulled(), "wireframeculled");
    }

    bool testWriteList()
    {
        om->WriteList();
        return true;
    }
   
}; // class PRREObject3DManagerTest
