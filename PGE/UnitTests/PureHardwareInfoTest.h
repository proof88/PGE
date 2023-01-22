#pragma once

/*
    ###################################################################################
    PRREHardwareInfoTest.h
    Unit test for PRREHardwareInfo.
    Made by PR00F88
    EMAIL #1 : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    EMAIL #2 : proof88@freemail.hu
    Skype    : proof88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGE/PRRE/PR00FsReducedRenderingEngine.h"
#include "../PGE/PRRE/PRREHardwareInfo.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREHardwareInfoTest : public UnitTest
{
public:

    PRREHardwareInfoTest() : UnitTest(__FILE__)
    {
        engine = NULL;
        hw = NULL;
    }

    ~PRREHardwareInfoTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        engine = NULL;
        hw = NULL;
        /**AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testCtor1);
        AddSubTest("testCtor2", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testCtor2);*/
        AddSubTest("testDeInitialize", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testDeInitialize);
        AddSubTest("testGetFreeSystemMemory", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetFreeSystemMemory);
        AddSubTest("testGetTotalSystemMemory", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetTotalSystemMemory);
        AddSubTest("testFreeTotalSystemMemoryRelation", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testFreeTotalSystemMemoryRelation);
        AddSubTest("testIsExtensionSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsExtensionSupported);
        AddSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetUsedVideoMemory);
        AddSubTest("testGetUsedTextureMemory", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetUsedTextureMemory);
        AddSubTest("testGetColorBufferPixelCount", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetColorBufferPixelCount);
        AddSubTest("testGetColorBufferSize", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetColorBufferSize);
        AddSubTest("testGetDepthBufferSize", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetDepthBufferSize);
        AddSubTest("testGetStencilBufferSize", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetStencilBufferSize);
        AddSubTest("testGetSampleBufferSize", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetSampleBufferSize);
        AddSubTest("testGetUsedFrameBufferMemory", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetUsedFrameBufferMemory);
        AddSubTest("testIsVSyncSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsVSyncSupported);
        AddSubTest("testIsMultiTexturingSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsMultiTexturingSupported);
        AddSubTest("testGetTextureUnitsCount", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetTextureUnitsCount);
        AddSubTest("testIsHardwareMipMapGenerationSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsHardwareMipMapGenerationSupported);
        AddSubTest("testIsNativeDIBFormatSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsNativeDIBFormatSupported);
        AddSubTest("testIsAnisoFilteringSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsAnisoFilteringSupported);
        AddSubTest("testGetMaximumAnisoFiltering", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetMaximumAnisoFiltering);
        AddSubTest("testIsTextureCompressionSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsTextureCompressionSupported);
        AddSubTest("testIsVTCSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsVTCSupported);
        AddSubTest("testIs3DcSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIs3DcSupported);
        AddSubTest("testIsRGTCSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsRGTCSupported);
        AddSubTest("testIsLATCCSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsLATCSupported);
        AddSubTest("testIsFullSceneAntiAliasingSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsFullSceneAntiAliasingSupported);
        AddSubTest("testGetMaxSamplesCount", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetMaxSamplesCount);
        AddSubTest("testIsSamplesSupported", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testIsSamplesSupported);
        AddSubTest("testGetVideocardOSName", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetVideocardOSName);
        AddSubTest("testGetVideocardOGLName", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetVideocardOGLName);
        AddSubTest("testGetVideocardOGLVersion", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetVideocardOGLVersion);
        AddSubTest("testGetVideocardOGLVendor", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetVideocardOGLVendor);
        AddSubTest("testGetVideocardOGLFeatureList", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetVideocardOGLFeatureList);
        AddSubTest("testGetVideocardWGLFeatureList", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testGetVideocardWGLFeatureList);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PRREHardwareInfoTest::testWriteStats);
    }

    virtual void SetUp()
    {
        if ( engine == NULL )
        {
            engine = PR00FsReducedRenderingEngine::createAndGet();
            engine->initialize(800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
            hw = PRREHardwareInfo::get();
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
    PRREHardwareInfo* hw;

    bool testCtor1()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        hw = PRREHardwareInfo::get();
        return assertNull(hw);
    }

    bool testCtor2()
    {
        return assertNotNull(hw, "notnull") &
            assertNotNull(&hw->getCentralProcessor(), "getCentralProcessor()") &
            assertNotNull(&hw->getMemory(), "getMemory()") &
            assertNotNull(&hw->getVideo(), "getVideo()") &
            assertNotNull(&hw->getAudio(), "getAudio()");
    }

    bool testDeInitialize()
    {
        hw->DeInitialize();
        const bool l = assertNull(hw->get(), "get()") &
                   assertNull(&hw->getCentralProcessor(), "getCentralProcessor()") &
                   assertNull(&hw->getMemory(), "getMemory()") &
                   assertNull(&hw->getVideo(), "getVideo()") &
                   assertNull(&hw->getAudio(), "getAudio()") /*&
                   // these will be moved to prrehwvideotest
                   assertEquals((TPRREuint) 0, hw->getVideo().getColorBufferPixelCount(), "getColorBufferPixelCount()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getColorBufferSize(), "getColorBufferSize()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getDepthBufferSize(), "getDepthBufferSize()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getStencilBufferSize(), "getStencilBufferSize()") &
                   assertEquals((TPRREuint) 0, hw->getMemory().getFreeSystemMemory(), "getFreeSystemMemory()") &
                   assertEquals((TPRREuint) 0, hw->getMemory().getTotalSystemMemory(), "getTotalSystemMemory()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getMaximumAnisoFiltering(), "getMaximumAnisoFiltering()") &
                   assertFalse(hw->getVideo().isAnisoFilteringSupported(), "isAnisoFilteringSupported()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getMaxSamplesCount(), "getMaxSamplesCount()") &
                   assertFalse(hw->getVideo().isSamplesSupported(1), "isSamplesSupported()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getSampleBufferSize(), "getSampleBufferSize()") &
                   assertFalse(hw->getVideo().isFullSceneAntiAliasingSupported(), "isFullSceneAntiAliasingSupported()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getTextureUnitsCount(), "getTextureUnitsCount()") &
                   assertFalse(hw->getVideo().isMultiTexturingSupported(), "isMultiTexturingSupported()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getUsedFrameBufferMemory(), "getUsedFrameBufferMemory()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getUsedTextureMemory(), "getUsedTextureMemory()") &
                   assertEquals((TPRREuint) 0, hw->getVideo().getUsedVideoMemory(), "getUsedVideoMemory()") &
                   assertFalse(hw->getVideo().isHardwareMipMapGenerationSupported(), "isHardwareMipMapGenerationSupported()") &
                   assertFalse(hw->getVideo().isNativeDIBFormatSupported(), "isNativeDIBFormatSupported()") &
                   assertFalse(hw->getVideo().isTextureCompressionSupported(), "isTextureCompressionSupported()") &
                   assertFalse(hw->getVideo().isVSyncSupported(), "isVSyncSupported()") &
                   assertFalse(hw->getVideo().isVTCSupported(), "isVTCSupported()") &
                   assertFalse(hw->getVideo().is3DcSupported(), "is3DcSupported()") &
                   assertFalse(hw->getVideo().isRGTCSupported(), "isRGTCSupported()") &
                   assertFalse(hw->getVideo().isLATCSupported(), "isLATCSupported()")*/;
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return l;
    }
    
    bool testGetFreeSystemMemory()
    {
        return assertTrue(hw->getMemory().getFreeSystemMemory() > 0);
    }

    bool testGetTotalSystemMemory()
    {
        return assertTrue(hw->getMemory().getTotalSystemMemory() > 0);
    }

    bool testFreeTotalSystemMemoryRelation()
    {
        return assertTrue(hw->getMemory().getFreeSystemMemory() <= hw->getMemory().getTotalSystemMemory());
    }

    bool testIsExtensionSupported()
    {
        return true; // we just don't test it
    }

    bool testGetUsedVideoMemory()
    {
        return assertTrue(hw->getVideo().getUsedVideoMemory() > 0);
    }

    bool testGetUsedTextureMemory()
    {
        return assertTrue(hw->getVideo().getUsedTextureMemory() >= 0);
    }

    bool testGetColorBufferPixelCount()
    {
        return assertEquals(engine->getScreen().getResWidth() * engine->getScreen().getResHeight(),
                            hw->getVideo().getColorBufferPixelCount());
    }

    bool testGetColorBufferSize()
    {
        return assertEquals(hw->getVideo().getColorBufferPixelCount()*4*2, hw->getVideo().getColorBufferSize());
    }

    bool testGetDepthBufferSize()
    {
        return assertTrue(hw->getVideo().getColorBufferPixelCount()*4 == hw->getVideo().getDepthBufferSize()
                          ||
                          hw->getVideo().getColorBufferPixelCount()*3 == hw->getVideo().getDepthBufferSize());
    }

    bool testGetStencilBufferSize()
    {
        return assertEquals((TPRREuint) 0, hw->getVideo().getStencilBufferSize());
    }

    bool testGetSampleBufferSize()
    {
        return assertEquals((TPRREuint) 0, hw->getVideo().getSampleBufferSize());
    }
    
    bool testGetUsedFrameBufferMemory()
    {
        return assertEquals(hw->getVideo().getColorBufferSize() +
                            hw->getVideo().getDepthBufferSize() +
                            hw->getVideo().getStencilBufferSize() +
                            hw->getVideo().getSampleBufferSize(),
                            hw->getVideo().getUsedFrameBufferMemory());
    }

    bool testIsVSyncSupported()
    {
        return assertTrue(hw->getVideo().isVSyncSupported()); // should be okay on main test machine
    }

    bool testIsMultiTexturingSupported()
    {
        return assertTrue(hw->getVideo().isMultiTexturingSupported()); // should be okay on main test machine
    }

    bool testGetTextureUnitsCount()
    {
        return assertTrue(hw->getVideo().getTextureUnitsCount() > 1); // should be okay on main test machine
    }

    bool testIsHardwareMipMapGenerationSupported()
    {
        return assertTrue(hw->getVideo().isHardwareMipMapGenerationSupported()); // should be okay on main test machine
    }

    bool testIsNativeDIBFormatSupported()
    {
        return assertTrue(hw->getVideo().isNativeDIBFormatSupported()); // should be okay on main test machine
    }

    bool testIsAnisoFilteringSupported()
    {
        return assertTrue(hw->getVideo().isAnisoFilteringSupported()); // should be okay on main test machine
    }

    bool testGetMaximumAnisoFiltering()
    {
        return assertTrue(hw->getVideo().getMaximumAnisoFiltering() > 1); // should be okay on main test machine
    }

    bool testIsTextureCompressionSupported()
    {
        return assertTrue(hw->getVideo().isTextureCompressionSupported()); // should be okay on main test machine
    }

    bool testIsVTCSupported()
    {
        return true; // we just don't test it
    }

    bool testIs3DcSupported()
    {
        return true; // we just don't test it
    }

    bool testIsRGTCSupported()
    {
        return true; // we just don't test it
    }

    bool testIsLATCSupported()
    {
        return true; // we just don't test it
    }

    bool testIsFullSceneAntiAliasingSupported()
    {
        return assertTrue(hw->getVideo().isFullSceneAntiAliasingSupported()); // should be okay on main test machine
    }

    bool testGetMaxSamplesCount()
    {
        return assertTrue(hw->getVideo().getMaxSamplesCount() > 1); // should be okay on main test machine
    }

    bool testIsSamplesSupported()
    {
        return assertTrue(hw->getVideo().isSamplesSupported(2)); // should be okay on main test machine
    }

    bool testGetVideocardOSName()
    {
        return assertFalse(hw->getVideo().getVideocardOSName().empty());
    }

    bool testGetVideocardOGLName()
    {
        return assertFalse(hw->getVideo().getVideocardOGLName().empty());
    }

    bool testGetVideocardOGLVersion()
    {
        return assertFalse(hw->getVideo().getVideocardOGLVersion().empty());
    }

    bool testGetVideocardOGLVendor()
    {
        return assertFalse(hw->getVideo().getVideocardOGLVendor().empty());
    }

    bool testGetVideocardOGLFeatureList()
    {
        return assertFalse(hw->getVideo().getVideocardOGLFeatureList().empty());
    }

    bool testGetVideocardWGLFeatureList()
    {
        return assertFalse(hw->getVideo().getVideocardWGLFeatureList().empty());
    }

    bool testWriteStats()
    {
        hw->WriteStats();
        return true;
    }
    
}; // class PRREHardwareInfoTest
