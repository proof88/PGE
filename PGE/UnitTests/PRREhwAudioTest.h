#pragma once

/*
    ###################################################################################
    PRREhwAudioTest.h
    Unit test for PRREhwAudio.
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

class PRREhwAudioTest :
    public UnitTest
{
public:

    PRREhwAudioTest() :
        UnitTest( __FILE__ ),
        hw( PRREhwInfo::get() ), 
        audio( PRREhwAudio::get() )
    {
        engine = NULL;
    }

    virtual ~PRREhwAudioTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREhwAudioTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PRREhwAudioTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PRREhwAudioTest::testDeinitialize);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PRREhwAudioTest::testWriteStats);
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
        CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", false);
    }

private:

    PR00FsReducedRenderingEngine* engine;
    PRREhwInfo& hw;
    PRREhwAudio& audio;

    // ---------------------------------------------------------------------------

    PRREhwAudioTest(const PRREhwAudioTest&) :
        hw( PRREhwInfo::get() ),
        audio( PRREhwAudio::get() )
    {};         

    PRREhwAudioTest& operator=(const PRREhwAudioTest&)
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
    
}; // class PRREhwAudioTest
