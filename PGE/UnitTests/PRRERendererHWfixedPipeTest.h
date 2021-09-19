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
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testCtor);
        AddSubTest("testIsInitialized", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testIsInitialized);
        AddSubTest("testGetRenderHints", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testGetRenderHints);
        AddSubTest("testSetRenderHints", (PFNUNITSUBTEST) &PRRERendererHWfixedPipeTest::testSetRenderHints);

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
        }
        return ret;
    }

    virtual void TearDown()
    {
        Finalize();
        renderer = NULL;
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

        return assertEquals(0u, renderer->getRenderHints());
    }

    bool testWriteStats()
    {
        renderer->WriteStats();
        return true;
    }
   
}; // class PRRERendererHWfixedPipeTest
