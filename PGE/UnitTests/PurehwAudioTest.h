#pragma once

/*
    ###################################################################################
    PureHwAudioTest.h
    Unit test for PureHwAudio.
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

class PureHwAudioTest :
    public UnitTest
{
public:

    PureHwAudioTest() :
        UnitTest( __FILE__ ),
        hw( PureHwInfo::get() ), 
        audio( PureHwAudio::get() )
    {
        engine = NULL;
    }

    virtual ~PureHwAudioTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureHwAudioTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PureHwAudioTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PureHwAudioTest::testDeinitialize);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PureHwAudioTest::testWriteStats);
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
    PureHwAudio& audio;

    // ---------------------------------------------------------------------------

    PureHwAudioTest(const PureHwAudioTest&) :
        hw( PureHwInfo::get() ),
        audio( PureHwAudio::get() )
    {};         

    PureHwAudioTest& operator=(const PureHwAudioTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertTrue(audio.isInitialized(), "isInitialized()");
    }

    bool testInitialize()
    {
        return assertTrue(audio.isInitialized());
    }

    bool testDeinitialize()
    {
        audio.Deinitialize();
        const bool l = assertFalse(audio.isInitialized(), "isInitialized()");

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return l;
    }

    bool testWriteStats()
    {
        audio.WriteStats();
        return true;
    }
    
}; // class PureHwAudioTest
