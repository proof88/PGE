#pragma once

/*
    ###################################################################################
    PurehwCentralProcessorTest.h
    Unit test for PureHwCentralProcessor.
    Made by PR00F88
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
        finalize();
    }

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        addSubTest("testCtor", (PFNUNITSUBTEST) &PurehwCentralProcessorTest::testCtor);
        addSubTest("testInitialize", (PFNUNITSUBTEST) &PurehwCentralProcessorTest::testInitialize);
        addSubTest("testDeinitialize", (PFNUNITSUBTEST) &PurehwCentralProcessorTest::testDeinitialize);
        addSubTest("testWriteStats", (PFNUNITSUBTEST) &PurehwCentralProcessorTest::testWriteStats);
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

    virtual void tearDown() override
    {
        
    }

    virtual void finalize() override
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
