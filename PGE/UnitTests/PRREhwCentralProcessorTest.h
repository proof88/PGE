#pragma once

/*
    ###################################################################################
    PRREhwCentralProcessorTest.h
    Unit test for PRREhwCentralProcessor.
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

class PRREhwCentralProcessorTest :
    public UnitTest
{
public:

    PRREhwCentralProcessorTest() :
        UnitTest( __FILE__ ),
        hw( PRREhwInfo::get() ),
        proc( PRREhwCentralProcessor::get() )
    {
        engine = NULL;
    }

    ~PRREhwCentralProcessorTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREhwCentralProcessorTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PRREhwCentralProcessorTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PRREhwCentralProcessorTest::testDeinitialize);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PRREhwCentralProcessorTest::testWriteStats);
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
    PRREhwCentralProcessor& proc;

    // ---------------------------------------------------------------------------

    PRREhwCentralProcessorTest(const PRREhwCentralProcessorTest&) :
        hw( PRREhwInfo::get() ),
        proc( PRREhwCentralProcessor::get() )
    {};         

    PRREhwCentralProcessorTest& operator=(const PRREhwCentralProcessorTest&)
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
    
}; // class PRREhwCentralProcessorTest
