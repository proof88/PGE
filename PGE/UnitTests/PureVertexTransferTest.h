#pragma once

/*
    ###################################################################################
    PureVertexTransferTest.h
    Unit test for PureVertexTransfer.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include <cassert>
#include "../Pure/include/external/Object3D/PureVertexTransferMode.h"
#include "../Pure/include/external/Object3D/PureMesh3DManager.h"
#include "../Pure/include/internal/Object3D/PureMesh3DImpl.h"
#include "../Pure/include/internal/Object3D/PureMesh3DManagerImpl.h"
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class SampleDescendantFromVertexTransfer : public PureVertexTransfer
{
public:
    SampleDescendantFromVertexTransfer(
        PureMaterialManager& matMgr,
        TPURE_VERTEX_MODIFYING_HABIT vmod = PURE_VMOD_STATIC,
        TPURE_VERTEX_REFERENCING_MODE vref = PURE_VREF_DIRECT,
        TPureBool bForceUseClientMemory = false) : PureVertexTransfer(matMgr, vmod, vref, bForceUseClientMemory)
    {};

    virtual ~SampleDescendantFromVertexTransfer()
    {};

    /*
        This is a sample draw logic for a drawable level-1 derived object, just for unit test.
        A real implementation is obviously the PureObject3D::draw() function.
    */
    TPureUInt dummyDraw()
    {
        assert(isLevel1());
        ResetLastTransferredCounts();

        TPureUInt nRetSum = 0;
        for (TPureInt i = 0; i < getCount(); i++)
        {
            nRetSum += ((SampleDescendantFromVertexTransfer*)(getAttachedAt(i)))->transferVertices();
        }
        return nRetSum;
    };

protected:

    SampleDescendantFromVertexTransfer() : PureVertexTransfer()
    {};

    SampleDescendantFromVertexTransfer(const SampleDescendantFromVertexTransfer&)
    {};

    SampleDescendantFromVertexTransfer& operator=(const SampleDescendantFromVertexTransfer&)
    {
        return *this;
    };

}; // SampleDescendantFromVertexTransfer

class SampleManagerForDescendantFromVertexTransfer : public PureMesh3DManager
{
public:
    SampleManagerForDescendantFromVertexTransfer(PureMaterialManager& matMgr) :
        PureMesh3DManager(matMgr),
        materialMgr(matMgr)
    {
    }
  
    SampleDescendantFromVertexTransfer* createBox(
        TPureFloat a, TPureFloat b, TPureFloat c,
        TPURE_VERTEX_MODIFYING_HABIT vmod = PURE_VMOD_STATIC,
        TPURE_VERTEX_REFERENCING_MODE vref = PURE_VREF_DIRECT,
        TPureBool bForceUseClientMemory = false)
    {
        getConsole().OLnOI("PureObject3DManager::createBox(%f, %f, %f)", a, b, c);

        SampleDescendantFromVertexTransfer* obj = new SampleDescendantFromVertexTransfer(materialMgr, vmod, vref, bForceUseClientMemory);
        assert(obj);
        Attach( *obj );
        SampleDescendantFromVertexTransfer* const subobj = new SampleDescendantFromVertexTransfer(materialMgr, vmod, vref, bForceUseClientMemory);
        assert(subobj);
        obj->Attach( *subobj );
        PureMesh3DManager* const me = this;
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
        TPURE_VERTEX_MODIFYING_HABIT vmod,
        TPURE_VERTEX_REFERENCING_MODE vref,
        TPureBool bForceUseClientMemory = false)
    {
        getConsole().OLnOI("SampleManagerForDescendantFromVertexTransfer::createFromFile(\"%s\")", filename);

        if ( !PFL::fileExists(filename) )
        {
            getConsole().EOLnOO("ERROR: file doesn't exist!");
            getConsole().OLn("");
            return PGENULL;
        }

        getConsole().OI();
        PureMesh3D* const tmpMesh = PureMesh3DManager::createFromFile(filename);
        assert(tmpMesh);
        SampleDescendantFromVertexTransfer* obj = new SampleDescendantFromVertexTransfer(materialMgr, vmod, vref, bForceUseClientMemory);
        assert(obj);
        bool b = ((PureMesh3D*)obj)->pImpl->cannibalize(*tmpMesh);
        assert(b);

        for (TPureInt i = 0; i < tmpMesh->getCount(); i++) 
        {
            SampleDescendantFromVertexTransfer* const subobject = new SampleDescendantFromVertexTransfer(materialMgr, vmod, vref, bForceUseClientMemory);
            assert(subobject);
            obj->Attach( *subobject );
            b &= ((PureMesh3D*)subobject)->pImpl->cannibalize(*(PureMesh3D*)(tmpMesh->getAttachedAt(i)));
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
    PureMaterialManager& materialMgr;

}; // SampleManagerForDescendantFromVertexTransfer

class PureVertexTransferTest :
    public UnitTest
{
public:

    PureVertexTransferTest() :
        UnitTest( __FILE__ )
    {
        engine = NULL;
        vtm = NULL;
        mesh = NULL;
        meshFromFile = NULL;
        
    };

    virtual ~PureVertexTransferTest()
    {
        finalize();
    }

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureVertexTransfer::getLoggerModuleName(), true);

        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        vtm = new SampleManagerForDescendantFromVertexTransfer(engine->getMaterialManager()); 
        mesh = NULL;
        meshFromFile = NULL;
        addSubTest("testIsVertexTransferModeSelectable", (PFNUNITSUBTEST) &PureVertexTransferTest::testIsVertexTransferModeSelectable);
        addSubTest("testIsVertexReferencingIndexed", (PFNUNITSUBTEST) &PureVertexTransferTest::testIsVertexReferencingIndexed);
        addSubTest("testIsVertexModifyingDynamic", (PFNUNITSUBTEST) &PureVertexTransferTest::testIsVertexModifyingDynamic);
        addSubTest("testSelectVertexTransferMode", (PFNUNITSUBTEST) &PureVertexTransferTest::testSelectVertexTransferMode);
        addSubTest("testIsVideoMemoryUsed", (PFNUNITSUBTEST) &PureVertexTransferTest::testIsVideoMemoryUsed);
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureVertexTransferTest::testCtor);
        addSubTest("testGetVertexModifyingHabit", (PFNUNITSUBTEST) &PureVertexTransferTest::testGetVertexModifyingHabit);
        addSubTest("testSetVertexModifyingHabit", (PFNUNITSUBTEST) &PureVertexTransferTest::testSetVertexModifyingHabit);
        addSubTest("testGetVertexReferencingMode", (PFNUNITSUBTEST) &PureVertexTransferTest::testGetVertexReferencingMode);
        addSubTest("testSetVertexReferencingMode", (PFNUNITSUBTEST) &PureVertexTransferTest::testSetVertexReferencingMode);
        addSubTest("testGetVertexTransferMode", (PFNUNITSUBTEST) &PureVertexTransferTest::testGetVertexTransferMode);
        addSubTest("testSetVertexTransferMode", (PFNUNITSUBTEST) &PureVertexTransferTest::testSetVertexTransferMode);
        addSubTest("testTransferVertices", (PFNUNITSUBTEST) &PureVertexTransferTest::testTransferVertices);
        addSubTest("testGetLastTransferredVertexCount", (PFNUNITSUBTEST) &PureVertexTransferTest::testGetLastTransferredVertexCount);
        addSubTest("testGetLastTransferredTriangleCount", (PFNUNITSUBTEST) &PureVertexTransferTest::testGetLastTransferredTriangleCount);
        addSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PureVertexTransferTest::testGetUsedSystemMemory);
        addSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PureVertexTransferTest::testGetUsedVideoMemory);
    }

    virtual bool setUp() override
    {
        mesh = vtm->createBox(1.0f, 2.0f, 3.0f);
        meshFromFile = vtm->createFromFile("_res/models/snail_proofps/snail.obj", PURE_VMOD_STATIC, PURE_VREF_DIRECT);
        return assertNotNull(mesh, "mesh null") &
            assertNotNull(meshFromFile, "meshFromFile null");
    }

    virtual void tearDown() override
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

    virtual void finalize() override
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

        CConsole::getConsoleInstance().SetLoggingState(PureMesh3DManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureMesh3D::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureVertexTransfer::getLoggerModuleName(), false);
    }


private:
    PR00FsUltimateRenderingEngine* engine;
    SampleManagerForDescendantFromVertexTransfer* vtm;
    SampleDescendantFromVertexTransfer* mesh;
    SampleDescendantFromVertexTransfer* meshFromFile;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PureVertexTransferTest(const PureVertexTransferTest&)
    {};         

    PureVertexTransferTest& operator=(const PureVertexTransferTest&)
    {
        return *this;
    };

    bool testIsVertexTransferModeSelectable()
    {
        // only testing HW-independent values
        return assertTrue( PureVertexTransfer::isVertexTransferModeSelectable(PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT), "PURE_VT_DYN_DIR_1_BY_1" ) &
               assertTrue( PureVertexTransfer::isVertexTransferModeSelectable(PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT)), "PURE_VT_DYN_DIR_RVA" ) &
               assertTrue( PureVertexTransfer::isVertexTransferModeSelectable(PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED), "PURE_VT_DYN_IND_1_BY_1" ) &
               assertTrue( PureVertexTransfer::isVertexTransferModeSelectable(PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT)), "PURE_VT_DYN_IND_RVA" ) &
               assertTrue( PureVertexTransfer::isVertexTransferModeSelectable(PURE_VMOD_STATIC  | PURE_VREF_DIRECT), "PURE_VT_STA_DIR_DL" ) &
               assertTrue( PureVertexTransfer::isVertexTransferModeSelectable(PURE_VMOD_STATIC  | PURE_VREF_INDEXED), "PURE_VT_STA_IND_DL" );
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
        const TPURE_VERTEX_TRANSFER_MODE vtransStaDirHost = PureVertexTransfer::selectVertexTransferMode(PURE_VMOD_STATIC, PURE_VREF_DIRECT, false);
        const TPURE_VERTEX_TRANSFER_MODE vtransStaIndHost = PureVertexTransfer::selectVertexTransferMode(PURE_VMOD_STATIC, PURE_VREF_INDEXED, false);
        const TPURE_VERTEX_TRANSFER_MODE vtransDynDirHost = PureVertexTransfer::selectVertexTransferMode(PURE_VMOD_DYNAMIC, PURE_VREF_DIRECT, false);
        const TPURE_VERTEX_TRANSFER_MODE vtransDynIndHost = PureVertexTransfer::selectVertexTransferMode(PURE_VMOD_DYNAMIC, PURE_VREF_INDEXED, false);

        const TPURE_VERTEX_TRANSFER_MODE vtransStaDirClient = PureVertexTransfer::selectVertexTransferMode(PURE_VMOD_STATIC, PURE_VREF_DIRECT, true);
        const TPURE_VERTEX_TRANSFER_MODE vtransStaIndClient = PureVertexTransfer::selectVertexTransferMode(PURE_VMOD_STATIC, PURE_VREF_INDEXED, true);
        const TPURE_VERTEX_TRANSFER_MODE vtransDynDirClient = PureVertexTransfer::selectVertexTransferMode(PURE_VMOD_DYNAMIC, PURE_VREF_DIRECT, true);
        const TPURE_VERTEX_TRANSFER_MODE vtransDynIndClient = PureVertexTransfer::selectVertexTransferMode(PURE_VMOD_DYNAMIC, PURE_VREF_INDEXED, true);

        
        // vtransDynDirClient and vtransDynIndClient should be clientside since habit is dynamic and bForceUseClientMemory is true

        const bool bVTransDynDirClientIsClientside = assertTrue( PureVertexTransfer::isVertexModifyingDynamic(vtransDynDirClient), "vtransDynDirClient dyn" ) &
            assertFalse( PureVertexTransfer::isVertexReferencingIndexed(vtransDynDirClient), "vtransDynDirClient ind" ) &
            assertTrue(BIT_READ(vtransDynDirClient,PURE_VT_VA_BIT), "vtransDynDirClient VA") &
            assertFalse(BIT_READ(vtransDynDirClient,PURE_VT_SVA_BIT), "vtransDynDirClient RVA"); // CVA and RNG bits are not checked here

        const bool bVTransDynIndClientIsClientside = assertTrue( PureVertexTransfer::isVertexModifyingDynamic(vtransDynIndClient), "vtransDynIndClient dyn" ) &
            assertTrue( PureVertexTransfer::isVertexReferencingIndexed(vtransDynIndClient), "vtransDynIndClient ind" ) &
            assertTrue(BIT_READ(vtransDynIndClient,PURE_VT_VA_BIT), "vtransDynIndClient VA") &
            assertFalse(BIT_READ(vtransDynIndClient,PURE_VT_SVA_BIT), "vtransDynIndClient RVA"); // CVA and RNG bits are not checked here

        
        // vtransStaDirClient and vtransStaIndClient should be serverside since habit is static hence bForceUseClientMemory is ignored

        const bool bVTransStaDirClientIsServerside = assertFalse( PureVertexTransfer::isVertexModifyingDynamic(vtransStaDirHost), "vtransStaDirHost dyn" ) &
            assertFalse( PureVertexTransfer::isVertexReferencingIndexed(vtransStaDirHost), "vtransStaDirHost ind" ) &
            assertTrue(BIT_READ(vtransStaDirHost,PURE_VT_VA_BIT), "vtransStaDirHost VA") &
            assertTrue(BIT_READ(vtransStaDirHost,PURE_VT_SVA_BIT), "vtransStaDirHost SVA"); // CVA and RNG bits are not checked here

        const bool bVTransStaIndClientIsServerside = assertFalse( PureVertexTransfer::isVertexModifyingDynamic(vtransStaIndHost), "vtransStaIndHost dyn" ) &
            assertTrue( PureVertexTransfer::isVertexReferencingIndexed(vtransStaIndHost), "vtransStaIndHost ind" ) &
            assertTrue(BIT_READ(vtransStaIndHost,PURE_VT_VA_BIT), "vtransStaIndHost VA") &
            assertTrue(BIT_READ(vtransStaIndHost,PURE_VT_SVA_BIT), "vtransStaIndHost SVA"); // CVA and RNG bits are not checked here

        return 
            assertFalse( PureVertexTransfer::isVertexReferencingIndexed( vtransDynDirHost ), "indexed ? vtransDynDirHost" ) &
            assertTrue( PureVertexTransfer::isVertexReferencingIndexed( vtransDynIndHost ), "indexed ? vtransDynIndHost" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic( vtransDynDirHost ), "dynamic ? vtransDynDirHost" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic( vtransDynIndHost ), "dynamic ? vtransDynIndHost" ) &
            assertFalse( PureVertexTransfer::isVertexReferencingIndexed( vtransStaDirClient ), "indexed ? vtransStaDirClient" ) &
            assertTrue( PureVertexTransfer::isVertexReferencingIndexed( vtransStaIndClient ), "indexed ? vtransStaIndClient" ) &
            assertFalse( PureVertexTransfer::isVertexModifyingDynamic( vtransStaDirClient ), "dynamic ? vtransStaDirClient" ) &
            assertFalse( PureVertexTransfer::isVertexModifyingDynamic( vtransStaIndClient ), "dynamic ? vtransStaIndClient" ) &
            bVTransDynDirClientIsClientside &
            bVTransDynIndClientIsClientside &
            bVTransStaDirClientIsServerside &
            bVTransStaIndClientIsServerside;
    }

    bool testIsVideoMemoryUsed()
    {
        return assertFalse(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_DIR_1_BY_1    ), "PURE_VT_DYN_DIR_1_BY_1") &
        assertFalse(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_DIR_RVA              ), "PURE_VT_DYN_DIR_RVA") &
        assertFalse(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_DIR_RVA_CVA          ), "PURE_VT_DYN_DIR_RVA_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_DIR_SVA_GEN           ), "PURE_VT_DYN_DIR_SVA_GEN") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_DIR_SVA_ATI           ), "PURE_VT_DYN_DIR_SVA_ATI") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_DIR_SVA_ATI_CVA       ), "PURE_VT_DYN_DIR_SVA_ATI_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_DIR_SVA_NV            ), "PURE_VT_DYN_DIR_SVA_NV") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_DIR_SVA_NV_CVA        ), "PURE_VT_DYN_DIR_SVA_NV_CVA") &
        assertFalse(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_1_BY_1           ), "PURE_VT_DYN_IND_1_BY_1") &
        assertFalse(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_RVA              ), "PURE_VT_DYN_IND_RVA") &
        assertFalse(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_RVA_RNG          ), "PURE_VT_DYN_IND_RVA_RNG") &
        assertFalse(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_RVA_CVA          ), "PURE_VT_DYN_IND_RVA_CVA") &
        assertFalse(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_RVA_CVA_RNG      ), "PURE_VT_DYN_IND_RVA_CVA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_GEN           ), "PURE_VT_DYN_IND_SVA_GEN") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_ATI           ), "PURE_VT_DYN_IND_SVA_ATI") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_ATI_RNG       ), "PURE_VT_DYN_IND_SVA_ATI_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_ATI_CVA       ), "PURE_VT_DYN_IND_SVA_ATI_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_ATI_CVA_RNG   ), "PURE_VT_DYN_IND_SVA_ATI_CVA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_ATI_EA        ), "PURE_VT_DYN_IND_SVA_ATI_EA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_ATI_EA_RNG    ), "PURE_VT_DYN_IND_SVA_ATI_EA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_ATI_EA_CVA    ), "PURE_VT_DYN_IND_SVA_ATI_EA_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_ATI_EA_CVA_RNG), "PURE_VT_DYN_IND_SVA_ATI_EA_CVA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_NV            ), "PURE_VT_DYN_IND_SVA_NV") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_NV_RNG        ), "PURE_VT_DYN_IND_SVA_NV_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_NV_CVA        ), "PURE_VT_DYN_IND_SVA_NV_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_NV_CVA_RNG    ), "PURE_VT_DYN_IND_SVA_NV_CVA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_NV_EA         ), "PURE_VT_DYN_IND_SVA_NV_EA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_NV_EA_RNG     ), "PURE_VT_DYN_IND_SVA_NV_EA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_NV_EA_CVA     ), "PURE_VT_DYN_IND_SVA_NV_EA_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_DYN_IND_SVA_NV_EA_CVA_RNG ), "PURE_VT_DYN_IND_SVA_NV_EA_CVA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_DIR_DL                ), "PURE_VT_STA_DIR_DL") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_DIR_SVA_GEN           ), "PURE_VT_STA_DIR_SVA_GEN") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_DIR_SVA_ATI           ), "PURE_VT_STA_DIR_SVA_ATI") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_DIR_SVA_ATI_CVA       ), "PURE_VT_STA_DIR_SVA_ATI_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_DIR_SVA_NV            ), "PURE_VT_STA_DIR_SVA_NV") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_DIR_SVA_NV_CVA        ), "PURE_VT_STA_DIR_SVA_NV_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_DL                ), "PURE_VT_STA_IND_DL") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_GEN           ), "PURE_VT_STA_IND_SVA_GEN") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_ATI           ), "PURE_VT_STA_IND_SVA_ATI") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_ATI_RNG       ), "PURE_VT_STA_IND_SVA_ATI_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_ATI_CVA       ), "PURE_VT_STA_IND_SVA_ATI_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_ATI_CVA_RNG   ), "PURE_VT_STA_IND_SVA_ATI_CVA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_ATI_EA        ), "PURE_VT_STA_IND_SVA_ATI_EA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_ATI_EA_RNG    ), "PURE_VT_STA_IND_SVA_ATI_EA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_ATI_EA_CVA    ), "PURE_VT_STA_IND_SVA_ATI_EA_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_ATI_EA_CVA_RNG), "PURE_VT_STA_IND_SVA_ATI_EA_CVA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_NV            ), "PURE_VT_STA_IND_SVA_NV") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_NV_RNG        ), "PURE_VT_STA_IND_SVA_NV_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_NV_CVA        ), "PURE_VT_STA_IND_SVA_NV_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_NV_CVA_RNG    ), "PURE_VT_STA_IND_SVA_NV_CVA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_NV_EA         ), "PURE_VT_STA_IND_SVA_NV_EA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_NV_EA_RNG     ), "PURE_VT_STA_IND_SVA_NV_EA_RNG") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_NV_EA_CVA     ), "PURE_VT_STA_IND_SVA_NV_EA_CVA") &
        assertTrue(PureVertexTransfer::isVideoMemoryUsed(PURE_VT_STA_IND_SVA_NV_EA_CVA_RNG ), "PURE_VT_STA_IND_SVA_NV_EA_CVA_RNG");
    }

    bool testCtor()
    {
        SampleDescendantFromVertexTransfer* vtJust1Param = new SampleDescendantFromVertexTransfer(engine->getMaterialManager());
        SampleDescendantFromVertexTransfer* vtVmod = new SampleDescendantFromVertexTransfer(engine->getMaterialManager(), PURE_VMOD_DYNAMIC);
        SampleDescendantFromVertexTransfer* vtVref = new SampleDescendantFromVertexTransfer(engine->getMaterialManager(), PURE_VMOD_DYNAMIC, PURE_VREF_INDEXED);
        SampleDescendantFromVertexTransfer* vtForceClientMemory = new SampleDescendantFromVertexTransfer(engine->getMaterialManager(), PURE_VMOD_DYNAMIC, PURE_VREF_INDEXED, true);

        if ( assertNotNull(vtJust1Param, "null vtJust1Param") &
            assertNotNull(vtVmod, "null vtVmod") &
            assertNotNull(vtVref, "null vtVref") &
            assertNotNull(vtForceClientMemory, "null vtForceClientMemory") )
        {
            // TODO: also add a check for the ForceUseClientMemory ... that could be public function of VertexTransfer, tells is using client or server memory!
            return assertEquals(PURE_VMOD_STATIC, vtJust1Param->getVertexModifyingHabit() & PURE_VMOD_STATIC, "vtJust1Param vmod") &
                assertEquals(PURE_VREF_DIRECT, vtJust1Param->getVertexReferencingMode() & PURE_VREF_DIRECT, "vtJust1Param vref") &
                assertEquals(PURE_VMOD_DYNAMIC, vtVmod->getVertexModifyingHabit() & PURE_VMOD_DYNAMIC, "vtVmod vmod") &
                assertEquals(PURE_VREF_DIRECT, vtVmod->getVertexReferencingMode() & PURE_VREF_DIRECT, "vtVmod vref") &
                assertEquals(PURE_VMOD_DYNAMIC, vtVref->getVertexModifyingHabit() & PURE_VMOD_DYNAMIC, "vtVref vmod") &
                assertEquals(PURE_VREF_INDEXED, vtVref->getVertexReferencingMode() & PURE_VREF_INDEXED, "vtVref vref");
        }
        else
            return false;
    }

     bool testGetVertexModifyingHabit()
    {
        return 
            assertEquals(PURE_VMOD_STATIC, meshFromFile->getVertexModifyingHabit(), "meshFromFile") &
            assertEquals(PURE_VMOD_STATIC, mesh->getVertexModifyingHabit(), "box");
    }

    bool testSetVertexModifyingHabit()
    {
        bool b = assertTrue(meshFromFile->setVertexModifyingHabit(PURE_VMOD_DYNAMIC), "set meshFromFile");
        b &= assertTrue(mesh->setVertexModifyingHabit(PURE_VMOD_DYNAMIC), "set mesh");

        // subobjects must reject this when called by user
        PureVertexTransfer* const submesh1Mesh = (PureVertexTransfer*)(mesh->getAttachedAt(0));
        b &= assertFalse(submesh1Mesh->setVertexModifyingHabit( PURE_VMOD_STATIC ), "set submesh1Mesh");

        PureVertexTransfer* const submesh1MeshFromFile = (PureVertexTransfer*)(meshFromFile->getAttachedAt(0));
        b &= assertFalse(submesh1MeshFromFile->setVertexModifyingHabit( PURE_VMOD_STATIC ), "set submesh1MeshFromFile");

        return b &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic(mesh->getVertexTransferMode()), "mesh" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic(meshFromFile->getVertexTransferMode()), "meshFromFile" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic(submesh1Mesh->getVertexTransferMode()), "submesh1Mesh" ) &
            assertTrue( PureVertexTransfer::isVertexModifyingDynamic(submesh1MeshFromFile->getVertexTransferMode()), "submesh1MeshFromFile" );
    }

    bool testGetVertexReferencingMode()
    {
        return assertEquals(PURE_VREF_DIRECT, meshFromFile->getVertexReferencingMode(), "meshFromFile") &
            assertEquals(PURE_VREF_DIRECT, mesh->getVertexReferencingMode(), "box");
    }

    bool testSetVertexReferencingMode()
    {
        bool b = assertTrue(mesh->setVertexReferencingMode(PURE_VREF_INDEXED), "mesh");
        b &= assertTrue(meshFromFile->setVertexReferencingMode(PURE_VREF_DIRECT), "meshFromFile");

        // subobjects must reject this when called by user
        PureVertexTransfer* const submesh1Mesh = (PureVertexTransfer*)(mesh->getAttachedAt(0));
        b &= assertFalse(submesh1Mesh->setVertexReferencingMode( PURE_VREF_DIRECT ), "submesh1Mesh");

        PureVertexTransfer* const submesh1MeshFromFile = (PureVertexTransfer*)(meshFromFile->getAttachedAt(0));
        b &= assertFalse(submesh1MeshFromFile->setVertexReferencingMode( PURE_VREF_INDEXED ), "submesh1MeshFromFile");

        return b &
            assertTrue( PureVertexTransfer::isVertexReferencingIndexed(mesh->getVertexTransferMode()), "mesh" ) &
            assertFalse( PureVertexTransfer::isVertexReferencingIndexed(meshFromFile->getVertexTransferMode()), "meshFromFile" ) &
            assertTrue( PureVertexTransfer::isVertexReferencingIndexed(submesh1Mesh->getVertexTransferMode()), "submesh1Mesh" ) &
            assertFalse( PureVertexTransfer::isVertexReferencingIndexed(submesh1MeshFromFile->getVertexTransferMode()), "submesh1MeshFromFile" );
    }

    bool testGetVertexTransferMode()
    {
        // Generic server-side vertex array should be selected, main test machine supports it
        const TPURE_VERTEX_TRANSFER_MODE vtExpected = PURE_VMOD_STATIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);

        return assertEquals(vtExpected, meshFromFile->getVertexTransferMode() & vtExpected, "meshFromFile 1") &
            assertEquals(0u, BITF_READ(meshFromFile->getVertexTransferMode(), PURE_VT_VENDOR_BITS, 3), "meshFromFile 2") &
            assertEquals(vtExpected, mesh->getVertexTransferMode() & vtExpected, "box 1") &
            assertEquals(0u, BITF_READ(mesh->getVertexTransferMode(), PURE_VT_VENDOR_BITS, 3), "box 2");
    }

    bool testSetVertexTransferMode()
    {
        const TPURE_VERTEX_TRANSFER_MODE vtExpectedObj = mesh->getVertexTransferMode();
        bool l = assertTrue(mesh->setVertexTransferMode( mesh->getVertexTransferMode() ), "set sva mesh 1"); // intentionally testing setting to the same
        l &= assertEquals(vtExpectedObj, mesh->getVertexTransferMode(), "sva mesh 1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedObjFromFile = meshFromFile->getVertexTransferMode();
        l &= assertTrue(meshFromFile->setVertexTransferMode( meshFromFile->getVertexTransferMode() ), "set sva meshFromFile 1"); // intentionally testing setting to the same
        l &= assertEquals(vtExpectedObjFromFile, meshFromFile->getVertexTransferMode(), "sva meshFromFile 1");

        // make sure the mode of the 2 meshes is not just basic (PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT) (1by1 immediate mode)
        l &= assertNotEquals(PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT, mesh->getVertexTransferMode(), "mesh not dir 1by1");
        l &= assertNotEquals(PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT, meshFromFile->getVertexTransferMode(), "meshFromFile not dir 1by1");

        // subobjects must reject this when called by user
        // by default the selected transfer mode is NOT PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT hence we try set that but expect no change!
        PureVertexTransfer* const submesh1Mesh = (PureVertexTransfer*)(mesh->getAttachedAt(0));
        l &= assertFalse(submesh1Mesh->setVertexTransferMode( PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT ), "set dir mesh subobject");
        l &= assertEquals(vtExpectedObj, submesh1Mesh->getVertexTransferMode(), "dir mesh subobject");

        PureVertexTransfer* const submesh1MeshFromFile = (PureVertexTransfer*)(meshFromFile->getAttachedAt(0));
        l &= assertFalse(submesh1MeshFromFile->setVertexTransferMode( PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT ), "set dir meshFromFile subobject");
        l &= assertEquals(vtExpectedObjFromFile, submesh1MeshFromFile->getVertexTransferMode(), "dir meshFromFile subobject");

        // Generic server-side vertex arrays are supported by main test machine

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaIndSVAobj = PURE_VMOD_STATIC  | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedStaIndSVAobj ), "set sva mesh 2");
        l &= assertEquals(vtExpectedStaIndSVAobj, mesh->getVertexTransferMode(), "sva mesh 2");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaIndSVAobjFromFile = PURE_VMOD_STATIC  | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedStaIndSVAobjFromFile ), "set sva meshFromFile 2");
        l &= assertEquals(vtExpectedStaIndSVAobjFromFile, meshFromFile->getVertexTransferMode(), "sva meshFromFile 2");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynIndSVAobj = PURE_VMOD_DYNAMIC  | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynIndSVAobj ), "set sva mesh 3");
        l &= assertEquals(vtExpectedDynIndSVAobj, mesh->getVertexTransferMode(), "sva mesh 3");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynIndSVAobjFromFile = PURE_VMOD_DYNAMIC  | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynIndSVAobjFromFile ), "set sva meshFromFile 3");
        l &= assertEquals(vtExpectedDynIndSVAobjFromFile, meshFromFile->getVertexTransferMode(), "sva meshFromFile 3");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDirSVAobj = PURE_VMOD_DYNAMIC  | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynDirSVAobj ), "set sva mesh 4");
        l &= assertEquals(vtExpectedDynDirSVAobj, mesh->getVertexTransferMode(), "sva mesh 4");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDirSVAobjFromFile = PURE_VMOD_DYNAMIC  | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT) | BIT(PURE_VT_SVA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynDirSVAobjFromFile ), "set sva meshFromFile 4");
        l &= assertEquals(vtExpectedDynDirSVAobjFromFile, meshFromFile->getVertexTransferMode(), "sva meshFromFile 4");

        // following modes must be supported on every machine

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDir1by1obj = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT;
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynDir1by1obj ), "set dir mesh 1b1");
        l &= assertEquals(vtExpectedDynDir1by1obj, mesh->getVertexTransferMode(), "dir mesh 1b1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDir1by1objFromFile = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT;
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynDir1by1objFromFile ), "set dir meshFromFile 1b1");
        l &= assertEquals(vtExpectedDynDir1by1objFromFile, meshFromFile->getVertexTransferMode(), "dir meshFromFile 1b1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDirVAobj = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynDirVAobj ), "set dir mesh rva");
        l &= assertEquals(vtExpectedDynDirVAobj, mesh->getVertexTransferMode(), "dir mesh rva");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynDirVAobjFromFile = PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT | BIT(PURE_VT_VA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynDirVAobjFromFile ), "set dir meshFromFile rva");
        l &= assertEquals(vtExpectedDynDirVAobjFromFile, meshFromFile->getVertexTransferMode(), "dir meshFromFile rva");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynInd1by1obj = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED;
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynInd1by1obj ), "set ind mesh 1b1");
        l &= assertEquals(vtExpectedDynInd1by1obj, mesh->getVertexTransferMode(), "ind mesh 1b1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynInd1by1objFromFile = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED;
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynInd1by1objFromFile ), "set ind meshFromFile 1b1");
        l &= assertEquals(vtExpectedDynInd1by1objFromFile, meshFromFile->getVertexTransferMode(), "ind meshFromFile 1b1");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynIndVAobj = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT);
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedDynIndVAobj ), "set ind mesh rva");
        l &= assertEquals(vtExpectedDynIndVAobj, mesh->getVertexTransferMode(), "ind mesh rva");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedDynIndVAobjFromFile = PURE_VMOD_DYNAMIC | PURE_VREF_INDEXED | BIT(PURE_VT_VA_BIT);
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedDynIndVAobjFromFile ), "set ind meshFromFile rva");
        l &= assertEquals(vtExpectedDynIndVAobjFromFile, meshFromFile->getVertexTransferMode(), "ind meshFromFile rva");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaDirDLobj = PURE_VMOD_STATIC | PURE_VREF_DIRECT;
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedStaDirDLobj ), "set dir mesh DL");
        l &= assertEquals(vtExpectedStaDirDLobj, mesh->getVertexTransferMode(), "dir mesh DL");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaDirDLobjFromFile = PURE_VMOD_STATIC | PURE_VREF_DIRECT;
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedStaDirDLobjFromFile ), "set dir meshFromFile DL");
        l &= assertEquals(vtExpectedStaDirDLobjFromFile, meshFromFile->getVertexTransferMode(), "dir meshFromFile DL");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaIndDLobj = PURE_VMOD_STATIC | PURE_VREF_INDEXED;
        l &= assertTrue(mesh->setVertexTransferMode( vtExpectedStaIndDLobj ), "set ind mesh DL");
        l &= assertEquals(vtExpectedStaIndDLobj, mesh->getVertexTransferMode(), "ind mesh DL");

        const TPURE_VERTEX_TRANSFER_MODE vtExpectedStaIndDLobjFromFile = PURE_VMOD_STATIC | PURE_VREF_INDEXED;
        l &= assertTrue(meshFromFile->setVertexTransferMode( vtExpectedStaIndDLobjFromFile ), "set ind meshFromFile DL");
        l &= assertEquals(vtExpectedStaIndDLobjFromFile, meshFromFile->getVertexTransferMode(), "ind meshFromFile DL");

        return l;
    }

    bool testTransferVertices()
    {
        // public dummyDraw() invokes protected transferVertices() straightforward for us to test it

        TPureUInt nMeshLastTransferredVertices = mesh->dummyDraw();
        TPureUInt nMeshFromFileLastTransferredVertices = meshFromFile->dummyDraw();

        return assertEquals(mesh->getVertexIndicesCount(), mesh->getLastTransferredVertexCount(), "mesh 1") &
            assertEquals(meshFromFile->getVertexIndicesCount(), meshFromFile->getLastTransferredVertexCount(), "meshFromFile 1") &
            assertEquals(nMeshLastTransferredVertices, mesh->getLastTransferredVertexCount(), "mesh 2") &
            assertEquals(nMeshFromFileLastTransferredVertices, meshFromFile->getLastTransferredVertexCount(), "meshFromFile 2") &
            assertEquals(mesh->getTriangleCount(), mesh->getLastTransferredTriangleCount(), "mesh 3") &
            assertEquals(meshFromFile->getTriangleCount(), meshFromFile->getLastTransferredTriangleCount(), "meshFromFile 3");
    }

    bool testGetLastTransferredVertexCount()
    {
        // non-initial values are checked in testTransferVertices()
        return assertEquals((TPureUInt)0, mesh->getLastTransferredVertexCount(), "mesh") &
            assertEquals((TPureUInt)0, meshFromFile->getLastTransferredVertexCount(), "meshFromFile");
    }

    bool testGetLastTransferredTriangleCount()
    {
        // non-initial values are checked in testTransferVertices()
        return assertEquals((TPureUInt)0, mesh->getLastTransferredTriangleCount(), "mesh") &
            assertEquals((TPureUInt)0, meshFromFile->getLastTransferredTriangleCount(), "meshFromFile");
    }

    bool testGetUsedSystemMemory()
    {
        return assertGreater(mesh->getUsedSystemMemory(),          sizeof(SampleDescendantFromVertexTransfer) + mesh->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ),         "mesh") &
               assertGreater(meshFromFile->getUsedSystemMemory(),  sizeof(SampleDescendantFromVertexTransfer) + meshFromFile->getVerticesCount() * 2 /* normals too */ * sizeof(TXYZ), "meshFromFile");
    }

    bool testGetUsedVideoMemory()
    {
        TPureUInt meshMinMemUsage = 0;
        for (TPureInt i = 0; i < mesh->getCount(); i++)
        {
            const PureVertexTransfer* const submesh = (PureVertexTransfer*) mesh->getAttachedAt(i);
            meshMinMemUsage += submesh->getVerticesCount() * sizeof(TXYZ) * 2 /* *2 because of we have normals too */ +
                submesh->getMaterial(false).getColorsCount() * sizeof(TRGBAFLOAT) +
                submesh->getMaterial(false).getTexcoordsCount() * sizeof(TUVW);
        }

        TPureUInt meshFromFileMinMemUsage = 0;
        for (TPureInt i = 0; i < meshFromFile->getCount(); i++)
        {
            const PureVertexTransfer* const submesh = (PureVertexTransfer*) meshFromFile->getAttachedAt(i);
            meshFromFileMinMemUsage += submesh->getVerticesCount() * sizeof(TXYZ) * 2 /* *2 because of we have normals too */ +
                submesh->getMaterial(false).getColorsCount() * sizeof(TRGBAFLOAT) +
                submesh->getMaterial(false).getTexcoordsCount() * sizeof(TUVW);
        }

        // by default the created objects should eat video memory
        bool l = assertGreater(mesh->getUsedVideoMemory(),    meshMinMemUsage, "mesh") &
            assertGreater(meshFromFile->getUsedVideoMemory(), meshFromFileMinMemUsage, "meshFromFile");
        
        l &= assertTrue(mesh->setVertexTransferMode( PURE_VMOD_DYNAMIC | PURE_VREF_DIRECT ), "set mesh basic");
        l &= assertEquals(mesh->getUsedVideoMemory(), (TPureUInt) 0, "mesh after set");

        return l;
    }
   
}; // class PureVertexTransferTest
