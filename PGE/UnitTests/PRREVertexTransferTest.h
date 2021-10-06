#pragma once

/*
    ###################################################################################
    PRREVertexTransferTest.h
    Unit test for PRREVertexTransfer.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include <cassert>
#include "../PRRE/include/external/Object3D/PRREVertexTransferMode.h"
#include "../PRRE/include/external/Object3D/PRREMesh3DManager.h"
#include "../PRRE/include/internal/Object3D/PRREMesh3DImpl.h"
#include "../PRRE/include/internal/Object3D/PRREMesh3DManagerImpl.h"
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class SampleDescendantFromVertexTransfer : public PRREVertexTransfer
{
public:
    SampleDescendantFromVertexTransfer(
        PRREMaterialManager& matMgr,
        TPRRE_VERTEX_MODIFYING_HABIT vmod = PRRE_VMOD_STATIC,
        TPRRE_VERTEX_REFERENCING_MODE vref = PRRE_VREF_DIRECT,
        TPRREbool bForceUseClientMemory = false) : PRREVertexTransfer(matMgr, vmod, vref, bForceUseClientMemory)
    {};

    virtual ~SampleDescendantFromVertexTransfer()
    {};

protected:

    SampleDescendantFromVertexTransfer() : PRREVertexTransfer()
    {};

    SampleDescendantFromVertexTransfer(const SampleDescendantFromVertexTransfer&)
    {};

    SampleDescendantFromVertexTransfer& operator=(const SampleDescendantFromVertexTransfer&)
    {
        return *this;
    };

}; // SampleDescendantFromVertexTransfer

class SampleManagerForDescendantFromVertexTransfer : public PRREMesh3DManager
{
public:
    SampleManagerForDescendantFromVertexTransfer(PRREMaterialManager& matMgr) :
        PRREMesh3DManager(matMgr),
        materialMgr(matMgr)
    {
    }
  
    SampleDescendantFromVertexTransfer* createBox(
        TPRREfloat a, TPRREfloat b, TPRREfloat c,
        TPRRE_VERTEX_MODIFYING_HABIT vmod = PRRE_VMOD_STATIC,
        TPRRE_VERTEX_REFERENCING_MODE vref = PRRE_VREF_DIRECT,
        TPRREbool bForceUseClientMemory = false)
    {
        getConsole().OLnOI("PRREObject3DManager::createBox(%f, %f, %f)", a, b, c);

        SampleDescendantFromVertexTransfer* obj = new SampleDescendantFromVertexTransfer(materialMgr, vmod, vref, bForceUseClientMemory);
        assert(obj);
        Attach( *obj );
        SampleDescendantFromVertexTransfer* const subobj = new SampleDescendantFromVertexTransfer(materialMgr, vmod, vref, bForceUseClientMemory);
        assert(subobj);
        obj->Attach( *subobj );
        PRREMesh3DManager* const me = this;
        bool res = me->pImpl->convertToBox(*obj, a, b, c);
        assert(res);

        // although SampleDescendantFromVertexTransfer ctor has already selected the vtransmode, we set it again
        // to actually allocate the needed resources for the geometry
        getConsole().OI();
        if ( !obj->setVertexTransferMode( obj->getVertexTransferMode() ) )
        {
            getConsole().OO();
            DeleteAttachedInstance( *obj );
            getConsole().EOLnOO("SampleManagerForDescendantFromVertexTransfer::createFromFile() returning NULL now!");
            return PGENULL;
        }
        getConsole().OO();

        getConsole().SOLnOO("> Box created successfully!");
        getConsole().OLn("");

        return obj;
    };

    SampleDescendantFromVertexTransfer* createFromFile(
        const char* filename,
        TPRRE_VERTEX_MODIFYING_HABIT vmod,
        TPRRE_VERTEX_REFERENCING_MODE vref,
        TPRREbool bForceUseClientMemory = false)
    {
        getConsole().OLnOI("SampleManagerForDescendantFromVertexTransfer::createFromFile(\"%s\")", filename);

        if ( !PFL::fileExists(filename) )
        {
            getConsole().EOLnOO("ERROR: file doesn't exist!");
            getConsole().OLn("");
            return PGENULL;
        }

        getConsole().OI();
        PRREMesh3D* const tmpMesh = PRREMesh3DManager::createFromFile(filename);
        assert(tmpMesh);
        SampleDescendantFromVertexTransfer* obj = new SampleDescendantFromVertexTransfer(materialMgr, vmod, vref, bForceUseClientMemory);
        assert(obj);
        bool b = ((PRREMesh3D*)obj)->pImpl->cannibalize(*tmpMesh);
        assert(b);

        for (TPRREint i = 0; i < tmpMesh->getCount(); i++) 
        {
            SampleDescendantFromVertexTransfer* const subobject = new SampleDescendantFromVertexTransfer(materialMgr, vmod, vref, bForceUseClientMemory);
            assert(subobject);
            obj->Attach( *subobject );
            b &= ((PRREMesh3D*)subobject)->pImpl->cannibalize(*(PRREMesh3D*)(tmpMesh->getAttachedAt(i)));
            assert(b);
        }
        this->DeleteAttachedInstance(*tmpMesh);

        obj->SetFilename(filename);
        getConsole().OO();
        Attach( *obj );

        // although SampleDescendantFromVertexTransfer ctor has already selected the vtransmode, we set it again
        // to actually allocate the needed resources for the geometry
        getConsole().OI();
        if ( !obj->setVertexTransferMode( obj->getVertexTransferMode() ) )
        {
            getConsole().OO();
            DeleteAttachedInstance( *obj );
            getConsole().EOLnOO("SampleManagerForDescendantFromVertexTransfer::createFromFile() returning NULL now!");
            return PGENULL;
        }
        getConsole().OO();

        getConsole().SOLnOO("> Object loaded successfully!");
        getConsole().OLn("");
        return obj; 
    };

protected:

    SampleManagerForDescendantFromVertexTransfer() : materialMgr(materialMgr)
    {};

    SampleManagerForDescendantFromVertexTransfer(const SampleManagerForDescendantFromVertexTransfer&) : materialMgr(materialMgr)
    {};

    SampleManagerForDescendantFromVertexTransfer& operator=(const SampleManagerForDescendantFromVertexTransfer&)
    {
        return *this;
    };

private:
    PRREMaterialManager& materialMgr;

}; // SampleManagerForDescendantFromVertexTransfer

class PRREVertexTransferTest :
    public UnitTest
{
public:

    PRREVertexTransferTest() :
        UnitTest( __FILE__ )
    {
        engine = NULL;
        vtm = NULL;
        mesh = NULL;
        meshFromFile = NULL;
        
    };

    virtual ~PRREVertexTransferTest()
    {
        Finalize();
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PRREMesh3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREMesh3D::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREVertexTransfer::getLoggerModuleName(), true);
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        vtm = new SampleManagerForDescendantFromVertexTransfer(engine->getMaterialManager()); 
        mesh = NULL;
        meshFromFile = NULL;
        AddSubTest("testIsVertexTransferModeSelectable", (PFNUNITSUBTEST) &PRREVertexTransferTest::testIsVertexTransferModeSelectable);
        AddSubTest("testIsVertexReferencingIndexed", (PFNUNITSUBTEST) &PRREVertexTransferTest::testIsVertexReferencingIndexed);
        AddSubTest("testIsVertexModifyingDynamic", (PFNUNITSUBTEST) &PRREVertexTransferTest::testIsVertexModifyingDynamic);
        AddSubTest("testSelectVertexTransferMode", (PFNUNITSUBTEST) &PRREVertexTransferTest::testSelectVertexTransferMode);
        AddSubTest("testIsVideoMemoryUsed", (PFNUNITSUBTEST) &PRREVertexTransferTest::testIsVideoMemoryUsed);
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREVertexTransferTest::testCtor);
        AddSubTest("testGetVertexModifyingHabit", (PFNUNITSUBTEST) &PRREVertexTransferTest::testGetVertexModifyingHabit);
        AddSubTest("testSetVertexModifyingHabit", (PFNUNITSUBTEST) &PRREVertexTransferTest::testSetVertexModifyingHabit);
        AddSubTest("testGetVertexReferencingMode", (PFNUNITSUBTEST) &PRREVertexTransferTest::testGetVertexReferencingMode);
        AddSubTest("testSetVertexReferencingMode", (PFNUNITSUBTEST) &PRREVertexTransferTest::testSetVertexReferencingMode);
        AddSubTest("testGetVertexTransferMode", (PFNUNITSUBTEST) &PRREVertexTransferTest::testGetVertexTransferMode);
        AddSubTest("testSetVertexTransferMode", (PFNUNITSUBTEST) &PRREVertexTransferTest::testSetVertexTransferMode);
        AddSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PRREVertexTransferTest::testGetUsedSystemMemory);
        AddSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PRREVertexTransferTest::testGetUsedVideoMemory);
    }

    virtual bool setUp()
    {
        mesh = vtm->createBox(1.0f, 2.0f, 3.0f);
        meshFromFile = vtm->createFromFile("_res/models/snail_proofps/snail.obj", PRRE_VMOD_STATIC, PRRE_VREF_DIRECT);
        return assertNotNull(mesh, "mesh null") &
            assertNotNull(meshFromFile, "meshFromFile null");
    }

    virtual void TearDown()
    {
        if ( mesh )
        {
            delete mesh;
            mesh = NULL;
        }
        if ( meshFromFile )
        {
            delete meshFromFile;
            meshFromFile = NULL;
        }
    }

    virtual void Finalize()
    {
        mesh = NULL;
        meshFromFile = NULL;

        if ( vtm )
        {
            vtm->DeleteAll();
            delete vtm;
            vtm = NULL;
        }

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }

        CConsole::getConsoleInstance().SetLoggingState(PRREMesh3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREMesh3D::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREVertexTransfer::getLoggerModuleName(), false);
    }


private:
    PR00FsReducedRenderingEngine* engine;
    SampleManagerForDescendantFromVertexTransfer* vtm;
    SampleDescendantFromVertexTransfer* mesh;
    SampleDescendantFromVertexTransfer* meshFromFile;

    // ---------------------------------------------------------------------------

    PRREVertexTransferTest(const PRREVertexTransferTest&)
    {};         

    PRREVertexTransferTest& operator=(const PRREVertexTransferTest&)
    {
        return *this;
    };

    bool testIsVertexTransferModeSelectable()
    {
        // only testing HW-independent values
        return assertTrue( PRREVertexTransfer::isVertexTransferModeSelectable(PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT), "PRRE_VT_DYN_DIR_1_BY_1" ) &
               assertTrue( PRREVertexTransfer::isVertexTransferModeSelectable(PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT)), "PRRE_VT_DYN_DIR_RVA" ) &
               assertTrue( PRREVertexTransfer::isVertexTransferModeSelectable(PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED), "PRRE_VT_DYN_IND_1_BY_1" ) &
               assertTrue( PRREVertexTransfer::isVertexTransferModeSelectable(PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT)), "PRRE_VT_DYN_IND_RVA" ) &
               assertTrue( PRREVertexTransfer::isVertexTransferModeSelectable(PRRE_VMOD_STATIC  | PRRE_VREF_DIRECT), "PRRE_VT_STA_DIR_DL" ) &
               assertTrue( PRREVertexTransfer::isVertexTransferModeSelectable(PRRE_VMOD_STATIC  | PRRE_VREF_INDEXED), "PRRE_VT_STA_IND_DL" );
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
        const TPRRE_VERTEX_TRANSFER_MODE vtransStaDirHost = PRREVertexTransfer::selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_DIRECT, false);
        const TPRRE_VERTEX_TRANSFER_MODE vtransStaIndHost = PRREVertexTransfer::selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_INDEXED, false);
        const TPRRE_VERTEX_TRANSFER_MODE vtransDynDirHost = PRREVertexTransfer::selectVertexTransferMode(PRRE_VMOD_DYNAMIC, PRRE_VREF_DIRECT, false);
        const TPRRE_VERTEX_TRANSFER_MODE vtransDynIndHost = PRREVertexTransfer::selectVertexTransferMode(PRRE_VMOD_DYNAMIC, PRRE_VREF_INDEXED, false);

        const TPRRE_VERTEX_TRANSFER_MODE vtransStaDirClient = PRREVertexTransfer::selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_DIRECT, true);
        const TPRRE_VERTEX_TRANSFER_MODE vtransStaIndClient = PRREVertexTransfer::selectVertexTransferMode(PRRE_VMOD_STATIC, PRRE_VREF_INDEXED, true);
        const TPRRE_VERTEX_TRANSFER_MODE vtransDynDirClient = PRREVertexTransfer::selectVertexTransferMode(PRRE_VMOD_DYNAMIC, PRRE_VREF_DIRECT, true);
        const TPRRE_VERTEX_TRANSFER_MODE vtransDynIndClient = PRREVertexTransfer::selectVertexTransferMode(PRRE_VMOD_DYNAMIC, PRRE_VREF_INDEXED, true);

        
        // vtransDynDirClient and vtransDynIndClient should be clientside since habit is dynamic and bForceUseClientMemory is true

        const bool bVTransDynDirClientIsClientside = assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(vtransDynDirClient), "vtransDynDirClient dyn" ) &
            assertFalse( PRREVertexTransfer::isVertexReferencingIndexed(vtransDynDirClient), "vtransDynDirClient ind" ) &
            assertTrue(BIT_READ(vtransDynDirClient,PRRE_VT_VA_BIT), "vtransDynDirClient VA") &
            assertFalse(BIT_READ(vtransDynDirClient,PRRE_VT_SVA_BIT), "vtransDynDirClient RVA"); // CVA and RNG bits are not checked here

        const bool bVTransDynIndClientIsClientside = assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(vtransDynIndClient), "vtransDynIndClient dyn" ) &
            assertTrue( PRREVertexTransfer::isVertexReferencingIndexed(vtransDynIndClient), "vtransDynIndClient ind" ) &
            assertTrue(BIT_READ(vtransDynIndClient,PRRE_VT_VA_BIT), "vtransDynIndClient VA") &
            assertFalse(BIT_READ(vtransDynIndClient,PRRE_VT_SVA_BIT), "vtransDynIndClient RVA"); // CVA and RNG bits are not checked here

        
        // vtransStaDirClient and vtransStaIndClient should be serverside since habit is static hence bForceUseClientMemory is ignored

        const bool bVTransStaDirClientIsServerside = assertFalse( PRREVertexTransfer::isVertexModifyingDynamic(vtransStaDirHost), "vtransStaDirHost dyn" ) &
            assertFalse( PRREVertexTransfer::isVertexReferencingIndexed(vtransStaDirHost), "vtransStaDirHost ind" ) &
            assertTrue(BIT_READ(vtransStaDirHost,PRRE_VT_VA_BIT), "vtransStaDirHost VA") &
            assertTrue(BIT_READ(vtransStaDirHost,PRRE_VT_SVA_BIT), "vtransStaDirHost SVA"); // CVA and RNG bits are not checked here

        const bool bVTransStaIndClientIsServerside = assertFalse( PRREVertexTransfer::isVertexModifyingDynamic(vtransStaIndHost), "vtransStaIndHost dyn" ) &
            assertTrue( PRREVertexTransfer::isVertexReferencingIndexed(vtransStaIndHost), "vtransStaIndHost ind" ) &
            assertTrue(BIT_READ(vtransStaIndHost,PRRE_VT_VA_BIT), "vtransStaIndHost VA") &
            assertTrue(BIT_READ(vtransStaIndHost,PRRE_VT_SVA_BIT), "vtransStaIndHost SVA"); // CVA and RNG bits are not checked here

        return 
            assertFalse( PRREVertexTransfer::isVertexReferencingIndexed( vtransDynDirHost ), "indexed ? vtransDynDirHost" ) &
            assertTrue( PRREVertexTransfer::isVertexReferencingIndexed( vtransDynIndHost ), "indexed ? vtransDynIndHost" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic( vtransDynDirHost ), "dynamic ? vtransDynDirHost" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic( vtransDynIndHost ), "dynamic ? vtransDynIndHost" ) &
            assertFalse( PRREVertexTransfer::isVertexReferencingIndexed( vtransStaDirClient ), "indexed ? vtransStaDirClient" ) &
            assertTrue( PRREVertexTransfer::isVertexReferencingIndexed( vtransStaIndClient ), "indexed ? vtransStaIndClient" ) &
            assertFalse( PRREVertexTransfer::isVertexModifyingDynamic( vtransStaDirClient ), "dynamic ? vtransStaDirClient" ) &
            assertFalse( PRREVertexTransfer::isVertexModifyingDynamic( vtransStaIndClient ), "dynamic ? vtransStaIndClient" ) &
            bVTransDynDirClientIsClientside &
            bVTransDynIndClientIsClientside &
            bVTransStaDirClientIsServerside &
            bVTransStaIndClientIsServerside;
    }

    bool testIsVideoMemoryUsed()
    {
        return assertFalse(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_DIR_1_BY_1    ), "PRRE_VT_DYN_DIR_1_BY_1") &
        assertFalse(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_DIR_RVA              ), "PRRE_VT_DYN_DIR_RVA") &
        assertFalse(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_DIR_RVA_CVA          ), "PRRE_VT_DYN_DIR_RVA_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_DIR_SVA_GEN           ), "PRRE_VT_DYN_DIR_SVA_GEN") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_DIR_SVA_ATI           ), "PRRE_VT_DYN_DIR_SVA_ATI") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_DIR_SVA_ATI_CVA       ), "PRRE_VT_DYN_DIR_SVA_ATI_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_DIR_SVA_NV            ), "PRRE_VT_DYN_DIR_SVA_NV") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_DIR_SVA_NV_CVA        ), "PRRE_VT_DYN_DIR_SVA_NV_CVA") &
        assertFalse(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_1_BY_1           ), "PRRE_VT_DYN_IND_1_BY_1") &
        assertFalse(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_RVA              ), "PRRE_VT_DYN_IND_RVA") &
        assertFalse(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_RVA_RNG          ), "PRRE_VT_DYN_IND_RVA_RNG") &
        assertFalse(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_RVA_CVA          ), "PRRE_VT_DYN_IND_RVA_CVA") &
        assertFalse(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_RVA_CVA_RNG      ), "PRRE_VT_DYN_IND_RVA_CVA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_GEN           ), "PRRE_VT_DYN_IND_SVA_GEN") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_ATI           ), "PRRE_VT_DYN_IND_SVA_ATI") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_ATI_RNG       ), "PRRE_VT_DYN_IND_SVA_ATI_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_ATI_CVA       ), "PRRE_VT_DYN_IND_SVA_ATI_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_ATI_CVA_RNG   ), "PRRE_VT_DYN_IND_SVA_ATI_CVA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_ATI_EA        ), "PRRE_VT_DYN_IND_SVA_ATI_EA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_ATI_EA_RNG    ), "PRRE_VT_DYN_IND_SVA_ATI_EA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_ATI_EA_CVA    ), "PRRE_VT_DYN_IND_SVA_ATI_EA_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_ATI_EA_CVA_RNG), "PRRE_VT_DYN_IND_SVA_ATI_EA_CVA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_NV            ), "PRRE_VT_DYN_IND_SVA_NV") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_NV_RNG        ), "PRRE_VT_DYN_IND_SVA_NV_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_NV_CVA        ), "PRRE_VT_DYN_IND_SVA_NV_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_NV_CVA_RNG    ), "PRRE_VT_DYN_IND_SVA_NV_CVA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_NV_EA         ), "PRRE_VT_DYN_IND_SVA_NV_EA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_NV_EA_RNG     ), "PRRE_VT_DYN_IND_SVA_NV_EA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_NV_EA_CVA     ), "PRRE_VT_DYN_IND_SVA_NV_EA_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_DYN_IND_SVA_NV_EA_CVA_RNG ), "PRRE_VT_DYN_IND_SVA_NV_EA_CVA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_DIR_DL                ), "PRRE_VT_STA_DIR_DL") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_DIR_SVA_GEN           ), "PRRE_VT_STA_DIR_SVA_GEN") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_DIR_SVA_ATI           ), "PRRE_VT_STA_DIR_SVA_ATI") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_DIR_SVA_ATI_CVA       ), "PRRE_VT_STA_DIR_SVA_ATI_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_DIR_SVA_NV            ), "PRRE_VT_STA_DIR_SVA_NV") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_DIR_SVA_NV_CVA        ), "PRRE_VT_STA_DIR_SVA_NV_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_DL                ), "PRRE_VT_STA_IND_DL") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_GEN           ), "PRRE_VT_STA_IND_SVA_GEN") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_ATI           ), "PRRE_VT_STA_IND_SVA_ATI") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_ATI_RNG       ), "PRRE_VT_STA_IND_SVA_ATI_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_ATI_CVA       ), "PRRE_VT_STA_IND_SVA_ATI_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_ATI_CVA_RNG   ), "PRRE_VT_STA_IND_SVA_ATI_CVA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_ATI_EA        ), "PRRE_VT_STA_IND_SVA_ATI_EA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_ATI_EA_RNG    ), "PRRE_VT_STA_IND_SVA_ATI_EA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_ATI_EA_CVA    ), "PRRE_VT_STA_IND_SVA_ATI_EA_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_ATI_EA_CVA_RNG), "PRRE_VT_STA_IND_SVA_ATI_EA_CVA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_NV            ), "PRRE_VT_STA_IND_SVA_NV") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_NV_RNG        ), "PRRE_VT_STA_IND_SVA_NV_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_NV_CVA        ), "PRRE_VT_STA_IND_SVA_NV_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_NV_CVA_RNG    ), "PRRE_VT_STA_IND_SVA_NV_CVA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_NV_EA         ), "PRRE_VT_STA_IND_SVA_NV_EA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_NV_EA_RNG     ), "PRRE_VT_STA_IND_SVA_NV_EA_RNG") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_NV_EA_CVA     ), "PRRE_VT_STA_IND_SVA_NV_EA_CVA") &
        assertTrue(PRREVertexTransfer::isVideoMemoryUsed(PRRE_VT_STA_IND_SVA_NV_EA_CVA_RNG ), "PRRE_VT_STA_IND_SVA_NV_EA_CVA_RNG");
    }

    bool testCtor()
    {
        SampleDescendantFromVertexTransfer* vtJust1Param = new SampleDescendantFromVertexTransfer(engine->getMaterialManager());
        SampleDescendantFromVertexTransfer* vtVmod = new SampleDescendantFromVertexTransfer(engine->getMaterialManager(), PRRE_VMOD_DYNAMIC);
        SampleDescendantFromVertexTransfer* vtVref = new SampleDescendantFromVertexTransfer(engine->getMaterialManager(), PRRE_VMOD_DYNAMIC, PRRE_VREF_INDEXED);
        SampleDescendantFromVertexTransfer* vtForceClientMemory = new SampleDescendantFromVertexTransfer(engine->getMaterialManager(), PRRE_VMOD_DYNAMIC, PRRE_VREF_INDEXED, true);

        if ( assertNotNull(vtJust1Param, "null vtJust1Param") &
            assertNotNull(vtVmod, "null vtVmod") &
            assertNotNull(vtVref, "null vtVref") &
            assertNotNull(vtForceClientMemory, "null vtForceClientMemory") )
        {
            // TODO: also add a check for the ForceUseClientMemory ... that could be public function of VertexTransfer, tells is using client or server memory!
            return assertEquals(PRRE_VMOD_STATIC, vtJust1Param->getVertexModifyingHabit() & PRRE_VMOD_STATIC, "vtJust1Param vmod") &
                assertEquals(PRRE_VREF_DIRECT, vtJust1Param->getVertexReferencingMode() & PRRE_VREF_DIRECT, "vtJust1Param vref") &
                assertEquals(PRRE_VMOD_DYNAMIC, vtVmod->getVertexModifyingHabit() & PRRE_VMOD_DYNAMIC, "vtVmod vmod") &
                assertEquals(PRRE_VREF_DIRECT, vtVmod->getVertexReferencingMode() & PRRE_VREF_DIRECT, "vtVmod vref") &
                assertEquals(PRRE_VMOD_DYNAMIC, vtVref->getVertexModifyingHabit() & PRRE_VMOD_DYNAMIC, "vtVref vmod") &
                assertEquals(PRRE_VREF_INDEXED, vtVref->getVertexReferencingMode() & PRRE_VREF_INDEXED, "vtVref vref");
        }
        else
            return false;
    }

     bool testGetVertexModifyingHabit()
    {
        return 
            assertEquals(PRRE_VMOD_STATIC, meshFromFile->getVertexModifyingHabit(), "meshFromFile") &
            assertEquals(PRRE_VMOD_STATIC, mesh->getVertexModifyingHabit(), "box");
    }

    bool testSetVertexModifyingHabit()
    {
        bool b = assertTrue(meshFromFile->setVertexModifyingHabit(PRRE_VMOD_DYNAMIC), "set meshFromFile");
        b &= assertTrue(mesh->setVertexModifyingHabit(PRRE_VMOD_DYNAMIC), "set mesh");

        // subobjects must reject this when called by user
        PRREVertexTransfer* const submesh1Mesh = (PRREVertexTransfer*)(mesh->getAttachedAt(0));
        b &= assertFalse(submesh1Mesh->setVertexModifyingHabit( PRRE_VMOD_STATIC ), "set submesh1Mesh");

        PRREVertexTransfer* const submesh1MeshFromFile = (PRREVertexTransfer*)(meshFromFile->getAttachedAt(0));
        b &= assertFalse(submesh1MeshFromFile->setVertexModifyingHabit( PRRE_VMOD_STATIC ), "set submesh1MeshFromFile");

        return b &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(mesh->getVertexTransferMode()), "mesh" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(meshFromFile->getVertexTransferMode()), "meshFromFile" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(submesh1Mesh->getVertexTransferMode()), "submesh1Mesh" ) &
            assertTrue( PRREVertexTransfer::isVertexModifyingDynamic(submesh1MeshFromFile->getVertexTransferMode()), "submesh1MeshFromFile" );
    }

    bool testGetVertexReferencingMode()
    {
        return assertEquals(PRRE_VREF_DIRECT, meshFromFile->getVertexReferencingMode(), "meshFromFile") &
            assertEquals(PRRE_VREF_DIRECT, mesh->getVertexReferencingMode(), "box");
    }

    bool testSetVertexReferencingMode()
    {
        bool b = assertTrue(mesh->setVertexReferencingMode(PRRE_VREF_INDEXED), "mesh");
        b &= assertTrue(meshFromFile->setVertexReferencingMode(PRRE_VREF_DIRECT), "meshFromFile");

        // subobjects must reject this when called by user
        PRREVertexTransfer* const submesh1Mesh = (PRREVertexTransfer*)(mesh->getAttachedAt(0));
        b &= assertFalse(submesh1Mesh->setVertexReferencingMode( PRRE_VREF_DIRECT ), "submesh1Mesh");

        PRREVertexTransfer* const submesh1MeshFromFile = (PRREVertexTransfer*)(meshFromFile->getAttachedAt(0));
        b &= assertFalse(submesh1MeshFromFile->setVertexReferencingMode( PRRE_VREF_INDEXED ), "submesh1MeshFromFile");

        return b &
            assertTrue( PRREVertexTransfer::isVertexReferencingIndexed(mesh->getVertexTransferMode()), "mesh" ) &
            assertFalse( PRREVertexTransfer::isVertexReferencingIndexed(meshFromFile->getVertexTransferMode()), "meshFromFile" ) &
            assertTrue( PRREVertexTransfer::isVertexReferencingIndexed(submesh1Mesh->getVertexTransferMode()), "submesh1Mesh" ) &
            assertFalse( PRREVertexTransfer::isVertexReferencingIndexed(submesh1MeshFromFile->getVertexTransferMode()), "submesh1MeshFromFile" );
    }

    bool testGetVertexTransferMode()
    {
        // Generic server-side vertex array should be selected, main test machine supports it
        const TPRRE_VERTEX_TRANSFER_MODE vtExpected = PRRE_VMOD_STATIC | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);

        return assertEquals(vtExpected, meshFromFile->getVertexTransferMode() & vtExpected, "meshFromFile 1") &
            assertEquals(0u, BITF_READ(meshFromFile->getVertexTransferMode(), PRRE_VT_VENDOR_BITS, 3), "meshFromFile 2") &
            assertEquals(vtExpected, mesh->getVertexTransferMode() & vtExpected, "box 1") &
            assertEquals(0u, BITF_READ(mesh->getVertexTransferMode(), PRRE_VT_VENDOR_BITS, 3), "box 2");
    }

    bool testSetVertexTransferMode()
    {
        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedObj = mesh->getVertexTransferMode();
        bool l = assertTrue(mesh->setVertexTransferMode( mesh->getVertexTransferMode() ), "set sva mesh 1"); // intentionally testing setting to the same
        l &= assertEquals(vtExpectedObj, mesh->getVertexTransferMode(), "sva mesh 1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedObjFromFile = meshFromFile->getVertexTransferMode();
        l &= assertTrue(meshFromFile->setVertexTransferMode( meshFromFile->getVertexTransferMode() ), "set sva meshFromFile 1"); // intentionally testing setting to the same
        l &= assertEquals(vtExpectedObjFromFile, meshFromFile->getVertexTransferMode(), "sva meshFromFile 1");

        // make sure the mode of the 2 meshes is not just basic (PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT) (1by1 immediate mode)
        l &= assertNotEquals(PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT, mesh->getVertexTransferMode(), "mesh not dir 1by1");
        l &= assertNotEquals(PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT, meshFromFile->getVertexTransferMode(), "meshFromFile not dir 1by1");

        // subobjects must reject this when called by user
        // by default the selected transfer mode is NOT PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT hence we try set that but expect no change!
        PRREVertexTransfer* const submesh1Mesh = (PRREVertexTransfer*)(mesh->getAttachedAt(0));
        l &= assertFalse(submesh1Mesh->setVertexTransferMode( PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT ), "set dir mesh subobject");
        l &= assertEquals(vtExpectedObj, submesh1Mesh->getVertexTransferMode(), "dir mesh subobject");

        PRREVertexTransfer* const submesh1MeshFromFile = (PRREVertexTransfer*)(meshFromFile->getAttachedAt(0));
        l &= assertFalse(submesh1MeshFromFile->setVertexTransferMode( PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT ), "set dir meshFromFile subobject");
        l &= assertEquals(vtExpectedObjFromFile, submesh1MeshFromFile->getVertexTransferMode(), "dir meshFromFile subobject");

        // Generic server-side vertex arrays are supported by main test machine

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaIndSVAobj = PRRE_VMOD_STATIC  | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedStaIndSVAobj ), "set sva mesh 2");
        l &= assertEquals(vtExpectedStaIndSVAobj, mesh->getVertexTransferMode(), "sva mesh 2");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaIndSVAobjFromFile = PRRE_VMOD_STATIC  | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedStaIndSVAobjFromFile ), "set sva meshFromFile 2");
        l &= assertEquals(vtExpectedStaIndSVAobjFromFile, meshFromFile->getVertexTransferMode(), "sva meshFromFile 2");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynIndSVAobj = PRRE_VMOD_DYNAMIC  | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynIndSVAobj ), "set sva mesh 3");
        l &= assertEquals(vtExpectedDynIndSVAobj, mesh->getVertexTransferMode(), "sva mesh 3");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynIndSVAobjFromFile = PRRE_VMOD_DYNAMIC  | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynIndSVAobjFromFile ), "set sva meshFromFile 3");
        l &= assertEquals(vtExpectedDynIndSVAobjFromFile, meshFromFile->getVertexTransferMode(), "sva meshFromFile 3");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDirSVAobj = PRRE_VMOD_DYNAMIC  | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynDirSVAobj ), "set sva mesh 4");
        l &= assertEquals(vtExpectedDynDirSVAobj, mesh->getVertexTransferMode(), "sva mesh 4");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDirSVAobjFromFile = PRRE_VMOD_DYNAMIC  | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT) | BIT(PRRE_VT_SVA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynDirSVAobjFromFile ), "set sva meshFromFile 4");
        l &= assertEquals(vtExpectedDynDirSVAobjFromFile, meshFromFile->getVertexTransferMode(), "sva meshFromFile 4");

        // following modes must be supported on every machine

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDir1by1obj = PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT;
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynDir1by1obj ), "set dir mesh 1b1");
        l &= assertEquals(vtExpectedDynDir1by1obj, mesh->getVertexTransferMode(), "dir mesh 1b1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDir1by1objFromFile = PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT;
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynDir1by1objFromFile ), "set dir meshFromFile 1b1");
        l &= assertEquals(vtExpectedDynDir1by1objFromFile, meshFromFile->getVertexTransferMode(), "dir meshFromFile 1b1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDirVAobj = PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynDirVAobj ), "set dir mesh rva");
        l &= assertEquals(vtExpectedDynDirVAobj, mesh->getVertexTransferMode(), "dir mesh rva");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynDirVAobjFromFile = PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT | BIT(PRRE_VT_VA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynDirVAobjFromFile ), "set dir meshFromFile rva");
        l &= assertEquals(vtExpectedDynDirVAobjFromFile, meshFromFile->getVertexTransferMode(), "dir meshFromFile rva");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynInd1by1obj = PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED;
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynInd1by1obj ), "set ind mesh 1b1");
        l &= assertEquals(vtExpectedDynInd1by1obj, mesh->getVertexTransferMode(), "ind mesh 1b1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynInd1by1objFromFile = PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED;
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynInd1by1objFromFile ), "set ind meshFromFile 1b1");
        l &= assertEquals(vtExpectedDynInd1by1objFromFile, meshFromFile->getVertexTransferMode(), "ind meshFromFile 1b1");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynIndVAobj = PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynIndVAobj ), "set ind mesh rva");
        l &= assertEquals(vtExpectedDynIndVAobj, mesh->getVertexTransferMode(), "ind mesh rva");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedDynIndVAobjFromFile = PRRE_VMOD_DYNAMIC | PRRE_VREF_INDEXED | BIT(PRRE_VT_VA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynIndVAobjFromFile ), "set ind meshFromFile rva");
        l &= assertEquals(vtExpectedDynIndVAobjFromFile, meshFromFile->getVertexTransferMode(), "ind meshFromFile rva");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaDirDLobj = PRRE_VMOD_STATIC | PRRE_VREF_DIRECT;
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedStaDirDLobj ), "set dir mesh DL");
        l &= assertEquals(vtExpectedStaDirDLobj, mesh->getVertexTransferMode(), "dir mesh DL");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaDirDLobjFromFile = PRRE_VMOD_STATIC | PRRE_VREF_DIRECT;
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedStaDirDLobjFromFile ), "set dir meshFromFile DL");
        l &= assertEquals(vtExpectedStaDirDLobjFromFile, meshFromFile->getVertexTransferMode(), "dir meshFromFile DL");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaIndDLobj = PRRE_VMOD_STATIC | PRRE_VREF_INDEXED;
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedStaIndDLobj ), "set ind mesh DL");
        l &= assertEquals(vtExpectedStaIndDLobj, mesh->getVertexTransferMode(), "ind mesh DL");

        const TPRRE_VERTEX_TRANSFER_MODE vtExpectedStaIndDLobjFromFile = PRRE_VMOD_STATIC | PRRE_VREF_INDEXED;
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedStaIndDLobjFromFile ), "set ind meshFromFile DL");
        l &= assertEquals(vtExpectedStaIndDLobjFromFile, meshFromFile->getVertexTransferMode(), "ind meshFromFile DL");

        return l;
    }

    bool testGetUsedSystemMemory()
    {
        return assertGreater(mesh->getUsedSystemMemory(),          sizeof(SampleDescendantFromVertexTransfer) + mesh->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),         "mesh") &
               assertGreater(meshFromFile->getUsedSystemMemory(),  sizeof(SampleDescendantFromVertexTransfer) + meshFromFile->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ), "meshFromFile");
    }

    bool testGetUsedVideoMemory()
    {
        TPRREuint meshMinMemUsage = 0;
        for (TPRREint i = 0; i < mesh->getCount(); i++)
        {
            const PRREVertexTransfer* const submesh = (PRREVertexTransfer*) mesh->getAttachedAt(i);
            meshMinMemUsage += submesh->getVerticesCount() * sizeof(TXYZ) * 2 /* *2 because of we have normals too */ +
                submesh->getMaterial(false).getColorsCount() * sizeof(TRGBAFLOAT) +
                submesh->getMaterial(false).getTexcoordsCount() * sizeof(TUVW);
        }

        TPRREuint meshFromFileMinMemUsage = 0;
        for (TPRREint i = 0; i < meshFromFile->getCount(); i++)
        {
            const PRREVertexTransfer* const submesh = (PRREVertexTransfer*) meshFromFile->getAttachedAt(i);
            meshFromFileMinMemUsage += submesh->getVerticesCount() * sizeof(TXYZ) * 2 /* *2 because of we have normals too */ +
                submesh->getMaterial(false).getColorsCount() * sizeof(TRGBAFLOAT) +
                submesh->getMaterial(false).getTexcoordsCount() * sizeof(TUVW);
        }

        // by default the created objects should eat video memory
        bool l = assertGreater(mesh->getUsedVideoMemory(),    meshMinMemUsage, "mesh") &
            assertGreater(meshFromFile->getUsedVideoMemory(), meshFromFileMinMemUsage, "meshFromFile");
        
        l &= assertTrue(mesh->setVertexTransferMode( PRRE_VMOD_DYNAMIC | PRRE_VREF_DIRECT ), "set mesh basic");
        l &= assertEquals(mesh->getUsedVideoMemory(), (TPRREuint) 0, "mesh after set");

        return l;
    }
   
}; // class PRREVertexTransferTest
