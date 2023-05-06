#pragma once

/*
    ###################################################################################
    PureHwSystemMemoryTest.h
    Unit test for PureHwSystemMemory.
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

class PureHwSystemMemoryTest :
    public UnitTest
{
public:

    PureHwSystemMemoryTest() :
        UnitTest( __FILE__ ),
        hw( PureHwInfo::get() ),
        mem( PureHwSystemMemory::get() ),
        cfgProfiles("")
    {
        engine = NULL;
    }

    virtual ~PureHwSystemMemoryTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureHwSystemMemoryTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PureHwSystemMemoryTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PureHwSystemMemoryTest::testDeinitialize);
        AddSubTest("testGetFreeSystemMemory", (PFNUNITSUBTEST) &PureHwSystemMemoryTest::testGetFreeSystemMemory);
        AddSubTest("testGetTotalSystemMemory", (PFNUNITSUBTEST) &PureHwSystemMemoryTest::testGetTotalSystemMemory);
        AddSubTest("testFreeTotalSystemMemoryRelation", (PFNUNITSUBTEST) &PureHwSystemMemoryTest::testFreeTotalSystemMemoryRelation);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PureHwSystemMemoryTest::testWriteStats);
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

    virtual void Finalize()
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
    PureHwSystemMemory& mem;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PureHwSystemMemoryTest(const PureHwSystemMemoryTest&) :
        hw( PureHwInfo::get() ),
        mem( PureHwSystemMemory::get() ),
        cfgProfiles("")
    {};         

    PureHwSystemMemoryTest& operator=(const PureHwSystemMemoryTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertTrue(mem.isInitialized(), "isInitialized()");
    }

    bool testInitialize()
    {
        return assertTrue(mem.isInitialized());
    }

    bool testDeinitialize()
    {
        mem.Deinitialize();
        const bool l = assertFalse(mem.isInitialized(), "isInitialized()");

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return l;
    }

    bool testGetFreeSystemMemory()
    {
        return assertGreater(mem.getFreeSystemMemory(), (TPureULong) 0);
    }

    bool testGetTotalSystemMemory()
    {
        return assertGreater(mem.getTotalSystemMemory(), (TPureULong) 0);
    }

    bool testFreeTotalSystemMemoryRelation()
    {
        return assertLequals(mem.getFreeSystemMemory(), mem.getTotalSystemMemory());
    }

    bool testWriteStats()
    {
        mem.WriteStats();
        return true;
    }
    
}; // class PureHwAudioTest
