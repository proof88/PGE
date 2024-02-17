#pragma once

/*
    ###################################################################################
    PurehwCentralProcessorTest.h
    Unit test for PureHwCentralProcessor.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"
#include "../Pure/include/external/Hardware/PureHwInfo.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PurehwCentralProcessorTest :
    public UnitTest
{
public:

    PurehwCentralProcessorTest() :
        UnitTest( __FILE__ ),
        hw( PureHwInfo::get() ),
        proc( PureHwCentralProcessor::get() )
    {
        engine = NULL;
    }

    virtual ~PurehwCentralProcessorTest()
    {
        Finalize();
    }

protected:

    virtual void Initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PurehwCentralProcessorTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PurehwCentralProcessorTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PurehwCentralProcessorTest::testDeinitialize);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PurehwCentralProcessorTest::testWriteStats);
    }

    virtual bool setUp() override
    {
        bool ret = true;
        if ( engine == NULL )
        {
            PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
            engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
            ret = (0 == engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0));  // pretty standard display mode, should work on most systems
        }
        return ret;
    }

    virtual void TearDown() override
    {
        
    }

    virtual void Finalize() override
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", false);
    }

private:

    PR00FsUltimateRenderingEngine* engine;
    PureHwInfo& hw;
    PureHwCentralProcessor& proc;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PurehwCentralProcessorTest(const PurehwCentralProcessorTest&) :
        hw( PureHwInfo::get() ),
        proc( PureHwCentralProcessor::get() )
    {};         

    PurehwCentralProcessorTest& operator=(const PurehwCentralProcessorTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertTrue(proc.isInitialized(), "isInitialized()");
    }

    bool testInitialize()
    {
        return assertTrue(proc.isInitialized());
    }                                                                 

    bool testDeinitialize()
    {
        proc.Deinitialize();
        const bool l = assertFalse(proc.isInitialized(), "isInitialized()");

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return l;
    }

    bool testWriteStats()
    {
        proc.WriteStats();
        return true;
    }
    
}; // class PurehwCentralProcessorTest
