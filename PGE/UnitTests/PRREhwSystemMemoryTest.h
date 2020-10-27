#pragma once

/*
    ###################################################################################
    PRREhwSystemMemoryTest.h
    Unit test for PRREhwSystemMemory.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"
#include "../PRRE/include/external/Hardware/PRREhwInfo.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREhwSystemMemoryTest :
    public UnitTest
{
public:

    PRREhwSystemMemoryTest() :
        UnitTest( __FILE__ ),
        hw( PRREhwInfo::get() ),
        mem( PRREhwSystemMemory::get() )
    {
        engine = NULL;
    }

    ~PRREhwSystemMemoryTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREhwSystemMemoryTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PRREhwSystemMemoryTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PRREhwSystemMemoryTest::testDeinitialize);
        AddSubTest("testGetFreeSystemMemory", (PFNUNITSUBTEST) &PRREhwSystemMemoryTest::testGetFreeSystemMemory);
        AddSubTest("testGetTotalSystemMemory", (PFNUNITSUBTEST) &PRREhwSystemMemoryTest::testGetTotalSystemMemory);
        AddSubTest("testFreeTotalSystemMemoryRelation", (PFNUNITSUBTEST) &PRREhwSystemMemoryTest::testFreeTotalSystemMemoryRelation);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PRREhwSystemMemoryTest::testWriteStats);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret = (0 == engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0));  // pretty standard display mode, should work on most systems
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
    }

private:

    PR00FsReducedRenderingEngine* engine;
    PRREhwInfo& hw;
    PRREhwSystemMemory& mem;

    // ---------------------------------------------------------------------------

    PRREhwSystemMemoryTest(const PRREhwSystemMemoryTest&) :
        hw( PRREhwInfo::get() ),
        mem( PRREhwSystemMemory::get() )
    {};         

    PRREhwSystemMemoryTest& operator=(const PRREhwSystemMemoryTest&)
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
        return assertGreater(mem.getFreeSystemMemory(), (TPRREulong) 0);
    }

    bool testGetTotalSystemMemory()
    {
        return assertGreater(mem.getTotalSystemMemory(), (TPRREulong) 0);
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
    
}; // class PRREhwAudioTest
