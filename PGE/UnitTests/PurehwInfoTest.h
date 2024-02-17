#pragma once

/*
    ###################################################################################
    PureHwInfoTest.h
    Unit test for PureHwInfo.
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

class PureHwInfoTest :
    public UnitTest
{
public:

    PureHwInfoTest() :
        UnitTest( __FILE__ ),
        hw( PureHwInfo::get() )
    {
        engine = NULL;
    }

    virtual ~PureHwInfoTest()
    {
        Finalize();
    }

protected:

    virtual void Initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureHwInfoTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PureHwInfoTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PureHwInfoTest::testDeinitialize);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PureHwInfoTest::testWriteStats);
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
        CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", false);    
    }

    virtual void Finalize() override
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
    }

private:

    PR00FsUltimateRenderingEngine* engine;
    PureHwInfo& hw;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PureHwInfoTest(const PureHwInfoTest&) :
        hw( PureHwInfo::get() )
    {};         

    PureHwInfoTest& operator=(const PureHwInfoTest&)
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
    
}; // class PureHwInfoTest
