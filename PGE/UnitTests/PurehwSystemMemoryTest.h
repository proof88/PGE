#pragma once

/*
    ###################################################################################
    PurehwSystemMemoryTest.h
    Unit test for PurehwSystemMemory.
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

class PurehwSystemMemoryTest :
    public UnitTest
{
public:

    PurehwSystemMemoryTest() :
        UnitTest( __FILE__ ),
        hw( PureHwInfo::get() ),
        mem( PurehwSystemMemory::get() )
    {
        engine = NULL;
    }

    virtual ~PurehwSystemMemoryTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PurehwSystemMemoryTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PurehwSystemMemoryTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PurehwSystemMemoryTest::testDeinitialize);
        AddSubTest("testGetFreeSystemMemory", (PFNUNITSUBTEST) &PurehwSystemMemoryTest::testGetFreeSystemMemory);
        AddSubTest("testGetTotalSystemMemory", (PFNUNITSUBTEST) &PurehwSystemMemoryTest::testGetTotalSystemMemory);
        AddSubTest("testFreeTotalSystemMemoryRelation", (PFNUNITSUBTEST) &PurehwSystemMemoryTest::testFreeTotalSystemMemoryRelation);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PurehwSystemMemoryTest::testWriteStats);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsUltimateRenderingEngine::createAndGet();
            ret = (0 == engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0));  // pretty standard display mode, should work on most systems
        }
        return ret;
    }

    virtual void TearDown()
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
    PurehwSystemMemory& mem;

    // ---------------------------------------------------------------------------

    PurehwSystemMemoryTest(const PurehwSystemMemoryTest&) :
        hw( PureHwInfo::get() ),
        mem( PurehwSystemMemory::get() )
    {};         

    PurehwSystemMemoryTest& operator=(const PurehwSystemMemoryTest&)
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
    
}; // class PurehwAudioTest
