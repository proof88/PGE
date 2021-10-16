#pragma once

/*
    ###################################################################################
    PRRERendererHWfixedPipeTest.h
    Unit test for PRRERendererHWfixedPipe.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/Render/PRREIRenderer.h"
#include "../PRRE/include/external/Render/PRRERendererHWfixedPipe.h"
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRRERendererHWfixedPipeTest :
    public UnitTest
{
public:

    PRRERendererHWfixedPipeTest() :
        UnitTest( __FILE__ )
    {
    };

    virtual ~PRRERendererHWfixedPipeTest()
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
        renderer = NULL;
        om = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testCtor);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testIsInitialized);
        AddSubTest("testGetRenderHints", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testGetRenderHints);
        AddSubTest("testSetRenderHints", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testSetRenderHints);
        // RenderScene() has no dedicated unit test but we can test different aspects e.g. if it properly reorders the objects, etc.
        AddSubTest("testRenderByZdistanceOrder", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testRenderByZdistanceOrder);
        AddSubTest("testLastFrameCounters", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testLastFrameCounters);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testWriteStats);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret &= assertEquals((TPRREuint)0, engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0), "engine");  // pretty standard display mode, should work on most systems
            renderer = engine->getRenderer();
            ret &= assertNotNull(renderer, "renderer null");
            om = &(engine->getObject3DManager());
            ret &= assertNotNull(om, "om null");
        }
        return ret;
    }

    virtual void TearDown()
    {
        Finalize();
        renderer = NULL;
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
    PRREIRenderer* renderer;
    PRREObject3DManager* om;

    // ---------------------------------------------------------------------------

    PRRERendererHWfixedPipeTest(const PRRERendererHWfixedPipeTest&)
    {};         

    PRRERendererHWfixedPipeTest& operator=(const PRRERendererHWfixedPipeTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertNotNull(renderer);
    }

    bool testIsInitialized()
    {
        // renderer's initialize() is implicitly invoked by our setUp()
        // renderer's shutdown() is implicitly invoked by our TearDown()
        return assertTrue( renderer->isInitialized() );
    }

    bool testGetRenderHints()
    {
        return assertEquals(PRRERendererHWfixedPipe::DefaultHints, renderer->getRenderHints());
    }

    bool testSetRenderHints()
    {
        renderer->SetRenderHints(0u);

        const TPRREbool b = assertEquals(0u, renderer->getRenderHints());

        // make sure we reset this to default because this setting is permanent across engine shutdown/reinit
        renderer->SetRenderHints(PRRERendererHWfixedPipe::DefaultHints);

        return b;
    }

    bool testRenderByZdistanceOrder()
    {
        const TPRRE_XYZ posStartFrom = {20.f, 0.f, 20.f};
        std::vector<PRREObject3D*> vCubes;
        for (int i = 0; i < 5; i++)
        {
            PRREObject3D* const cube = om->createCube(1.0f, PRRE_VMOD_DYNAMIC, PRRE_VREF_DIRECT, true);
            if ( !assertNotNull(cube, (std::string("cube ") + std::to_string(i) + " null").c_str()) )
            {
                return false;
            }
            vCubes.push_back( cube );
            cube->SetOccluder(false);  // we want them to be occludees now
            // initially we spatially position the cubes in farest to nearest order in the scene
            cube->getPosVec().Set(posStartFrom.x - i*2, posStartFrom.y, posStartFrom.z - i*2);
        }

        if ( !assertEquals(vCubes.size(), om->get3dOpaqueOccludees().size(), "vCubes vs get3dOpaqueOccludees size") )
        {
            return false;
        }

        // expecting the initial order to be same
        for (std::size_t i = 0; i < vCubes.size(); i++)
        {
            if ( !assertEquals(om->get3dOpaqueOccludees()[i], vCubes[i], ("initial cube order failed at " + std::to_string(i)).c_str()) )
            {
                return false;
            }
        }

        // now we trigger a reordering by a render call
        renderer->RenderScene();

        // expecting the order to be reverse now compared to the initial order due to cubes are reordered by distance to camera
        TPRREbool b = true;
        for (std::size_t i = 0; i < vCubes.size(); i++)
        {
            b &= assertEquals(om->get3dOpaqueOccludees()[i], vCubes[vCubes.size()-i-1], ("2nd cube order failed at " + std::to_string(i)).c_str());
        }

        // now we set the cubes blended, so we will expect the initial order again: from farest to nearest!
        // I intentionally do this 1 by 1 unordered so they are being put in another container unordered!
        vCubes[2]->getMaterial(false).setBlendMode(PRRE_BM_STANDARD_TRANSPARENCY);
        vCubes[0]->getMaterial(false).setBlendMode(PRRE_BM_STANDARD_TRANSPARENCY);
        vCubes[4]->getMaterial(false).setBlendMode(PRRE_BM_STANDARD_TRANSPARENCY);
        vCubes[1]->getMaterial(false).setBlendMode(PRRE_BM_STANDARD_TRANSPARENCY);
        vCubes[3]->getMaterial(false).setBlendMode(PRRE_BM_STANDARD_TRANSPARENCY);
        
        // sanity check
        b &= assertTrue(om->get3dOpaqueOccludees().empty(), "get3dOpaqueOccludees() empty");
        if ( !assertEquals(vCubes.size(), om->get3dBlendedOccludees().size(), "vCubes vs get3dBlendedOccludees size") )
        {
            return false;
        }

        // now we trigger a reordering by a render call
        renderer->RenderScene();

        for (std::size_t i = 0; i < vCubes.size(); i++)
        {
            b &= assertEquals(om->get3dBlendedOccludees()[i], vCubes[i], ("3rd cube order failed at " + std::to_string(i)).c_str());
        }

        // now we set the cubes opaqua again but they will be occluders too, so we will expect the same order as with opaqueOccludees again: nearest to farest!
        // I intentionally do this 1 by 1 unordered so they are being put in another container unordered!
        vCubes[2]->getMaterial(false).setBlendMode(PRRE_BM_NONE);
        vCubes[0]->getMaterial(false).setBlendMode(PRRE_BM_NONE);
        vCubes[4]->getMaterial(false).setBlendMode(PRRE_BM_NONE);
        vCubes[1]->getMaterial(false).setBlendMode(PRRE_BM_NONE);
        vCubes[3]->getMaterial(false).setBlendMode(PRRE_BM_NONE);
        vCubes[2]->SetOccluder(true);
        vCubes[0]->SetOccluder(true);
        vCubes[4]->SetOccluder(true);
        vCubes[1]->SetOccluder(true);
        vCubes[3]->SetOccluder(true);

        // sanity check
        b &= assertTrue(om->get3dBlendedOccludees().empty(), "get3dBlendedOccludees() empty");
        if ( !assertEquals(vCubes.size(), om->getOccluders().size(), "vCubes vs getOccluders size") )
        {
            return false;
        }

        // now we trigger a reordering by a render call
        renderer->RenderScene();

        for (std::size_t i = 0; i < vCubes.size(); i++)
        {
            b &= assertEquals(om->getOccluders()[i], vCubes[vCubes.size()-i-1], ("4th cube order failed at " + std::to_string(i)).c_str());
        }

        return b;
    }

    bool testLastFrameCounters()
    {
        bool b = true;
        try
        {
            PRRERendererHWfixedPipe& hwRenderer = dynamic_cast<PRRERendererHWfixedPipe&>(*renderer);
            
            // first check if they are all zeros right after initialization
            b &= assertEquals(0u, hwRenderer.getLastFrameObjectsVisible(), "getLastFrameObjectsVisible 1") &
                assertEquals(0u, hwRenderer.getLastFrameOccluders(), "getLastFrameOccluders 1") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesNonOcclusionTested(), "getLastFrameOccludeesNonOcclusionTested 1") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTested(), "getLastFrameOccludeesOcclusionTested 1") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndOccluded(), "getLastFrameOccludeesOcclusionTestedAndOccluded 1") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccluded(), "getLastFrameOccludeesOcclusionTestedAndNonOccluded 1") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway(), "getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway 1") &
                assertEquals(0u, hwRenderer.getLastFrameTransferredVertices(), "getLastFrameTransferredVertices 1") &
                assertEquals(0u, hwRenderer.getLastFrameTransferredTriangles(), "getLastFrameTransferredTriangles 1");

            // check if they are all zeros when there is nothing to render
            renderer->RenderScene();
            // first check if they are all zeros right after initialization
            b &= assertEquals(0u, hwRenderer.getLastFrameObjectsVisible(), "getLastFrameObjectsVisible 2") &
                assertEquals(0u, hwRenderer.getLastFrameOccluders(), "getLastFrameOccluders 2") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesNonOcclusionTested(), "getLastFrameOccludeesNonOcclusionTested 2") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTested(), "getLastFrameOccludeesOcclusionTested 2") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndOccluded(), "getLastFrameOccludeesOcclusionTestedAndOccluded 2") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccluded(), "getLastFrameOccludeesOcclusionTestedAndNonOccluded 2") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway(), "getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway 2") &
                assertEquals(0u, hwRenderer.getLastFrameTransferredVertices(), "getLastFrameTransferredVertices 2") &
                assertEquals(0u, hwRenderer.getLastFrameTransferredTriangles(), "getLastFrameTransferredTriangles 2");

            // now create some cubes and set different settings so we can have different expectations for the counters
            const TPRRE_XYZ posStartFrom = {20.f, 0.f, 20.f};
            std::vector<PRREObject3D*> vCubes;
            for (int i = 0; i < 5; i++)
            {
                PRREObject3D* const cube = om->createCube(1.0f, PRRE_VMOD_DYNAMIC, PRRE_VREF_DIRECT, true);
                if ( !assertNotNull(cube, (std::string("cube ") + std::to_string(i) + " null").c_str()) )
                {
                    return false;
                }
                vCubes.push_back( cube );
                vCubes[i]->SetOccluder(false);
                vCubes[i]->SetOcclusionTested(false);
                // we spatially position the cubes in farest to nearest order in the scene
                cube->getPosVec().Set(posStartFrom.x - i*2, posStartFrom.y, posStartFrom.z - i*2);
            }

            vCubes[0]->SetOccluder(true);
            vCubes[1]->SetOcclusionTested(true);
            vCubes[2]->SetOcclusionTested(true);
            vCubes[2]->SetRenderingAllowed(false);

            renderer->RenderScene();
            b &= assertEquals(4u /* vCubes[0,1,3,4] */, hwRenderer.getLastFrameObjectsVisible(), "getLastFrameObjectsVisible 3") &
                assertEquals(1u /* vCubes[0] */, hwRenderer.getLastFrameOccluders(), "getLastFrameOccluders 3") &
                assertEquals(2u /* vCubes[3,4] */, hwRenderer.getLastFrameOccludeesNonOcclusionTested(), "getLastFrameOccludeesNonOcclusionTested 3") &
                assertEquals(2u /* vCubes[1,2] */, hwRenderer.getLastFrameOccludeesOcclusionTested(), "getLastFrameOccludeesOcclusionTested 3") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndOccluded(), "getLastFrameOccludeesOcclusionTestedAndOccluded 3") &
                assertEquals(2u /* vCubes[1,2] */, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccluded(), "getLastFrameOccludeesOcclusionTestedAndNonOccluded 3") &
                assertEquals(1u /* vCubes[2] */, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway(), "getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway 3") &
                /* vertices and triangles should be transferred for the 4 visible cubes, with 1 bounding box of vCubes[1] due to occlusion test start
                   (vCubes[2] visibility is disabled hence occlusion test won't be started either for it) */
                assertEquals(hwRenderer.getLastFrameObjectsVisible()/*4*/ * 24 + 1 /* vCubes[1] */ * 24, hwRenderer.getLastFrameTransferredVertices(), "getLastFrameTransferredVertices 3") &
                assertEquals(hwRenderer.getLastFrameObjectsVisible()/*4*/ * 12 + 1 /* vCubes[1] */ * 12, hwRenderer.getLastFrameTransferredTriangles(), "getLastFrameTransferredTriangles 3");

            // deleting objects does not change renderer's last frame stats
            for (int i = 0; i < 5; i++)
                delete vCubes[i];

            b &= assertEquals(4u /* vCubes[0,1,3,4] */, hwRenderer.getLastFrameObjectsVisible(), "getLastFrameObjectsVisible 4") &
                assertEquals(1u /* vCubes[0] */, hwRenderer.getLastFrameOccluders(), "getLastFrameOccluders 4") &
                assertEquals(2u /* vCubes[3,4] */, hwRenderer.getLastFrameOccludeesNonOcclusionTested(), "getLastFrameOccludeesNonOcclusionTested 4") &
                assertEquals(2u /* vCubes[1,2] */, hwRenderer.getLastFrameOccludeesOcclusionTested(), "getLastFrameOccludeesOcclusionTested 4") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndOccluded(), "getLastFrameOccludeesOcclusionTestedAndOccluded 4") &
                assertEquals(2u /* vCubes[1,2] */, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccluded(), "getLastFrameOccludeesOcclusionTestedAndNonOccluded 4") &
                assertEquals(1u /* vCubes[2] */, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway(), "getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway 4") &
                /* vertices and triangles should be transferred for the 4 visible cubes, with 1 bounding box of vCubes[1] due to occlusion test start
                   (vCubes[2] visibility is disabled hence occlusion test won't be started either for it) */
                assertEquals(hwRenderer.getLastFrameObjectsVisible()/*4*/ * 24 + 1 /* vCubes[1] */ * 24, hwRenderer.getLastFrameTransferredVertices(), "getLastFrameTransferredVertices 4") &
                assertEquals(hwRenderer.getLastFrameObjectsVisible()/*4*/ * 12 + 1 /* vCubes[1] */ * 12, hwRenderer.getLastFrameTransferredTriangles(), "getLastFrameTransferredTriangles 4");

            // but triggering a render again actually changes everything back to 0
            renderer->RenderScene();
            b &= assertEquals(0u, hwRenderer.getLastFrameObjectsVisible(), "getLastFrameObjectsVisible 5") &
                assertEquals(0u, hwRenderer.getLastFrameOccluders(), "getLastFrameOccluders 5") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesNonOcclusionTested(), "getLastFrameOccludeesNonOcclusionTested 5") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTested(), "getLastFrameOccludeesOcclusionTested 5") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndOccluded(), "getLastFrameOccludeesOcclusionTestedAndOccluded 5") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccluded(), "getLastFrameOccludeesOcclusionTestedAndNonOccluded 5") &
                assertEquals(0u, hwRenderer.getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway(), "getLastFrameOccludeesOcclusionTestedAndNonOccludedButNonVisibleAnyway 5") &
                assertEquals(0u, hwRenderer.getLastFrameTransferredVertices(), "getLastFrameTransferredVertices 5") &
                assertEquals(0u, hwRenderer.getLastFrameTransferredTriangles(), "getLastFrameTransferredTriangles 5");                                                               
        }
        catch (const std::exception&)
        {
            b = assertTrue(false, "dynamic cast");
        }

        return b;
    }

    bool testWriteStats()
    {
        renderer->WriteStats();
        return true;
    }
   
}; // class PRRERendererHWfixedPipeTest
