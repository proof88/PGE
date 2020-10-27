#pragma once

/*
    ###################################################################################
    PRREhwInfoTest.h
    Unit test for PRREhwInfo.
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

class PRREhwInfoTest :
    public UnitTest
{
public:

    PRREhwInfoTest() :
        UnitTest( __FILE__ ),
        hw( PRREhwInfo::get() )
    {
        engine = NULL;
    }

    ~PRREhwInfoTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREhwInfoTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PRREhwInfoTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PRREhwInfoTest::testDeinitialize);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PRREhwInfoTest::testWriteStats);
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

    // ---------------------------------------------------------------------------

    PRREhwInfoTest(const PRREhwInfoTest&) :
        hw( PRREhwInfo::get() )
    {};         

    PRREhwInfoTest& operator=(const PRREhwInfoTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertTrue(hw.isInitialized(), "isInitialized()") &
            assertTrue(hw.getCentralProcessor().isInitialized(), "getCentralProcessor()") &
            assertTrue(hw.getMemory().isInitialized(), "getMemory()") &
            assertTrue(hw.getVideo().isInitialized(), "getVideo()") &
            assertTrue(hw.getAudio().isInitialized(), "getAudio()");
    }

    bool testInitialize()
    {
        return assertTrue( hw.isInitialized() );
    }

    bool testDeinitialize()
    {
        hw.Deinitialize();
        const bool l = assertFalse(hw.isInitialized(), "isInitialized()") &
            assertFalse(hw.getCentralProcessor().isInitialized(), "getCentralProcessor()") &
            assertFalse(hw.getMemory().isInitialized(), "getMemory()") &
            assertFalse(hw.getVideo().isInitialized(), "getVideo()") &
            assertFalse(hw.getAudio().isInitialized(), "getAudio()");

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return l;
    }

    bool testWriteStats()
    {
        hw.WriteStats();
        return true;
    }
    
}; // class PRREhwInfoTest
