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
#include "../PRRE/PR00FsReducedRenderingEngine.h"
#include "../PRRE/Hardware/PRREhwInfo.h"

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

    ~PRREhwAudioTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREhwAudioTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PRREhwAudioTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PRREhwAudioTest::testDeinitialize);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PRREhwAudioTest::testWriteStats);
    }

    virtual void SetUp()
    {
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        }
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
