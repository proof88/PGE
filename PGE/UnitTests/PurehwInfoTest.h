#pragma once

/*
    ###################################################################################
    PurehwInfoTest.h
    Unit test for PureHwInfo.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PR00FsReducedRenderingEngine.h"
#include "../Pure/include/external/Hardware/PureHwInfo.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PurehwInfoTest :
    public UnitTest
{
public:

    PurehwInfoTest() :
        UnitTest( __FILE__ ),
        hw( PureHwInfo::get() )
    {
        engine = NULL;
    }

    virtual ~PurehwInfoTest()
    {
        Finalize();
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PurehwInfoTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PurehwInfoTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PurehwInfoTest::testDeinitialize);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PurehwInfoTest::testWriteStats);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret = (0 == engine->initialize(Pure_RENDERER_HW_FP, 800, 600, Pure_WINDOWED, 0, 32, 24, 0, 0));  // pretty standard display mode, should work on most systems
        }
        return ret;
    }

    virtual void TearDown()
    {
        CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", false);    
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
    PureHwInfo& hw;

    // ---------------------------------------------------------------------------

    PurehwInfoTest(const PurehwInfoTest&) :
        hw( PureHwInfo::get() )
    {};         

    PurehwInfoTest& operator=(const PurehwInfoTest&)
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
    
}; // class PurehwInfoTest
