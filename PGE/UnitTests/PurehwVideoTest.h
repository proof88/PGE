#pragma once

/*
    ###################################################################################
    PureHwVideoTest.h
    Unit test for PureHwVideo.
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

class PureHwVideoTest :
    public UnitTest
{
public:

    PureHwVideoTest() :
        UnitTest( __FILE__ ),
        hw( PureHwInfo::get() ),
        video( PureHwVideo::get() )
    {
        engine = NULL;
        
    }

    virtual ~PureHwVideoTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureHwVideoTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PureHwVideoTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PureHwVideoTest::testDeinitialize);
        AddSubTest("testIsExtensionSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsExtensionSupported);
        AddSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PureHwVideoTest::testGetUsedVideoMemory);
        AddSubTest("testGetUsedTextureMemory", (PFNUNITSUBTEST) &PureHwVideoTest::testGetUsedTextureMemory);
        AddSubTest("testGetColorBufferPixelCount", (PFNUNITSUBTEST) &PureHwVideoTest::testGetColorBufferPixelCount);
        AddSubTest("testGetColorBufferSize", (PFNUNITSUBTEST) &PureHwVideoTest::testGetColorBufferSize);
        AddSubTest("testGetDepthBufferSize", (PFNUNITSUBTEST) &PureHwVideoTest::testGetDepthBufferSize);
        AddSubTest("testGetStencilBufferSize", (PFNUNITSUBTEST) &PureHwVideoTest::testGetStencilBufferSize);
        AddSubTest("testGetSampleBufferSize", (PFNUNITSUBTEST) &PureHwVideoTest::testGetSampleBufferSize);
        AddSubTest("testGetUsedFrameBufferMemory", (PFNUNITSUBTEST) &PureHwVideoTest::testGetUsedFrameBufferMemory);
        AddSubTest("testIsVSyncSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsVSyncSupported);
        AddSubTest("testIsMultiTexturingSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsMultiTexturingSupported);
        AddSubTest("testGetTextureUnitsCount", (PFNUNITSUBTEST) &PureHwVideoTest::testGetTextureUnitsCount);
        AddSubTest("testIsHardwareMipMapGenerationSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsHardwareMipMapGenerationSupported);
        AddSubTest("testIsNativeDIBFormatSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsNativeDIBFormatSupported);
        AddSubTest("testIsAnisoFilteringSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsAnisoFilteringSupported);
        AddSubTest("testGetMaximumAnisoFiltering", (PFNUNITSUBTEST) &PureHwVideoTest::testGetMaximumAnisoFiltering);
        AddSubTest("testIsTextureCompressionSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsTextureCompressionSupported);
        AddSubTest("testIsVTCSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsVTCSupported);
        AddSubTest("testIs3DcSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIs3DcSupported);
        AddSubTest("testIsRGTCSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsRGTCSupported);
        AddSubTest("testIsLATCCSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsLATCSupported);
        AddSubTest("testIsAdvancedPixelFormatSetSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsAdvancedPixelFormatSetSupported);
        AddSubTest("testIsFullSceneAntiAliasingSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsFullSceneAntiAliasingSupported);
        AddSubTest("testGetMaxSamplesCount", (PFNUNITSUBTEST) &PureHwVideoTest::testGetMaxSamplesCount);
        AddSubTest("testIsSamplesSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsSamplesSupported);
        AddSubTest("testIsMultiDrawArraysSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsMultiDrawArraysSupported);
        AddSubTest("testIsDrawRangeElementsSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsDrawRangeElementsSupported);
        AddSubTest("testIsCompiledVertexArraySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsCompiledVertexArraySupported);
        AddSubTest("testIsNVVertexArrayRangeSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsNVVertexArrayRangeSupported);
        AddSubTest("testIsNVElementArraySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsNVElementArraySupported);
        AddSubTest("testIsATIVertexArrayObjectSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsATIVertexArrayObjectSupported);
        AddSubTest("testIsATIMapObjectBufferSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsATIMapObjectBufferSupported);
        AddSubTest("testIsATIElementArraySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsATIElementArraySupported);
        AddSubTest("testIsVertexBufferObjectSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsVertexBufferObjectSupported);
        AddSubTest("testIsATITruFormSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsATITruFormSupported);
        AddSubTest("testIsHwOcclusionQuerySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsHwOcclusionQuerySupported);
        AddSubTest("testIsHwBooleanOcclusionQuerySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsHwBooleanOcclusionQuerySupported);
        AddSubTest("testGetVideocardOSName", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOSName);
        AddSubTest("testGetVideocardOGLName", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOGLName);
        AddSubTest("testGetVideocardOGLVersion", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOGLVersion);
        AddSubTest("testGetVideocardOGLVendor", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOGLVendor);
        AddSubTest("testGetVideocardOGLFeatureList", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOGLFeatureList);
        AddSubTest("testGetVideocardWGLFeatureList", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardWGLFeatureList);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PureHwVideoTest::testWriteStats);
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
    PureHwInfo& hw;
    PureHwVideo& video;

    // ---------------------------------------------------------------------------

    PureHwVideoTest(const PureHwVideoTest&) :
        hw( PureHwInfo::get() ),
        video( PureHwVideo::get() )
    {};         

    PureHwVideoTest& operator=(const PureHwVideoTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return assertTrue(video.isInitialized(), "isInitialized()");
    }

    bool testInitialize()
    {
        return assertTrue(video.isInitialized());
    }

    bool testDeinitialize()
    {
        hw.Deinitialize();
        const bool l = assertFalse(video.isInitialized(), "isInitialized()") &
            assertEquals((TPureuint) 0, video.getUsedVideoMemory(), "getUsedVideoMemory()") &
            assertEquals((TPureuint) 0, video.getUsedTextureMemory(), "getUsedTextureMemory()") &
            assertEquals((TPureuint) 0, video.getColorBufferPixelCount(), "getColorBufferPixelCount()") &
            assertEquals((TPureuint) 0, video.getColorBufferSize(), "getColorBufferSize()") &
            assertEquals((TPureuint) 0, video.getDepthBufferSize(), "getDepthBufferSize()") &
            assertEquals((TPureuint) 0, video.getStencilBufferSize(), "getStencilBufferSize()") &
            assertEquals((TPureuint) 0, video.getSampleBufferSize(), "getSampleBufferSize()") &
            assertEquals((TPureuint) 0, video.getUsedFrameBufferMemory(), "getUsedFrameBufferMemory()") &
            assertFalse(video.isVSyncSupported(), "isVSyncSupported()") &
            assertFalse(video.isMultiTexturingSupported(), "isMultiTexturingSupported()") &
            assertEquals(0, video.getTextureUnitsCount(), "getTextureUnitsCount()") &
            assertFalse(video.isHardwareMipMapGenerationSupported(), "isHardwareMipMapGenerationSupported()") &
            assertFalse(video.isNativeDIBFormatSupported(), "isNativeDIBFormatSupported()") &
            assertFalse(video.isAnisoFilteringSupported(), "isAnisoFilteringSupported()") &
            assertEquals(0.0f, video.getMaximumAnisoFiltering(), "getMaximumAnisoFiltering()") &
            assertFalse(video.isTextureCompressionSupported(), "isTextureCompressionSupported()") &
            assertFalse(video.isVTCSupported(), "isVTCSupported()") &
            assertFalse(video.is3DcSupported(), "is3DcSupported()") &
            assertFalse(video.isRGTCSupported(), "isRGTCSupported()") &
            assertFalse(video.isLATCSupported(), "isLATCSupported()") &
            assertFalse(video.isFullSceneAntiAliasingSupported(), "isFullSceneAntiAliasingSupported()") &
            assertEquals(0, video.getMaxSamplesCount(), "getMaxSamplesCount()") &
            assertFalse(video.isSamplesSupported(1), "isSamplesSupported()") &
            assertFalse(video.isMultiDrawArraysSupported(), "isMultiDrawArraysSupported()") &
            assertFalse(video.isDrawRangeElementsSupported(), "isDrawRangeElementsSupported()") &
            assertFalse(video.isCompiledVertexArraySupported(), "isCompiledVertexArraySupported()") &
            assertFalse(video.isNVVertexArrayRangeSupported(), "isNVVertexArrayRangeSupported()") &
            assertFalse(video.isNVElementArraySupported(), "isNVElementArraySupported()") &
            assertFalse(video.isATIVertexArrayObjectSupported(), "isATIVertexArrayObjectSupported()") &
            assertFalse(video.isATIMapObjectBufferSupported(), "isATIMapObjectBufferSupported()") &
            assertFalse(video.isATIElementArraySupported(), "isATIElementArraySupported()") &
            assertFalse(video.isVertexBufferObjectSupported(), "isVertexBufferObjectSupported()") &
            assertFalse(video.isATITruFormSupported(), "isATITruFormSupported()") &
            assertTrue(video.getVideocardOSName().empty(), "getVideocardOSName()") &
            assertTrue(video.getVideocardOGLName().empty(), "getVideocardOGLName()") &
            assertTrue(video.getVideocardOGLVersion().empty(), "getVideocardOGLVersion()") &
            assertTrue(video.getVideocardOGLVendor().empty(), "getVideocardOGLVendor()") &
            assertTrue(video.getVideocardOGLFeatureList().empty(), "getVideocardOGLFeatureList()") &
            assertTrue(video.getVideocardWGLFeatureList().empty(), "getVideocardWGLFeatureList()");
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return l;
    }

    bool testIsExtensionSupported()
    {
        return true; // we just don't test it
    }

    bool testGetUsedVideoMemory()
    {
        return assertGreater(video.getUsedVideoMemory(), sizeof(PureHwVideo));
    }

    bool testGetUsedTextureMemory()
    {
        return assertGequals(video.getUsedTextureMemory(), (TPureulong) 0);
    }

    bool testGetColorBufferPixelCount()
    {
        return assertEquals(engine->getScreen().getResWidth() * engine->getScreen().getResHeight(),
            video.getColorBufferPixelCount());
    }

    bool testGetColorBufferSize()
    {
        return assertEquals(video.getColorBufferPixelCount()*4*2, video.getColorBufferSize());
    }

    bool testGetDepthBufferSize()
    {
        return assertTrue(video.getColorBufferPixelCount()*4 == video.getDepthBufferSize() ||
            video.getColorBufferPixelCount()*3 == video.getDepthBufferSize());
    }

    bool testGetStencilBufferSize()
    {
        return assertEquals((TPureuint) 0, video.getStencilBufferSize());
    }

    bool testGetSampleBufferSize()
    {
        return assertEquals((TPureuint) 0, video.getSampleBufferSize());
    }
    
    bool testGetUsedFrameBufferMemory()
    {
        return assertEquals(video.getColorBufferSize() +
            video.getDepthBufferSize() +
            video.getStencilBufferSize() +
            video.getSampleBufferSize(),
            video.getUsedFrameBufferMemory());
    }

    bool testIsVSyncSupported()
    {
        return assertTrue(video.isVSyncSupported()); // should be okay on main test machine
    }

    bool testIsMultiTexturingSupported()
    {
        return assertTrue(video.isMultiTexturingSupported()); // should be okay on main test machine
    }

    bool testGetTextureUnitsCount()
    {
        return assertGreater(video.getTextureUnitsCount(), 1); // should be okay on main test machine
    }

    bool testIsHardwareMipMapGenerationSupported()
    {
        return assertTrue(video.isHardwareMipMapGenerationSupported()); // should be okay on main test machine
    }

    bool testIsNativeDIBFormatSupported()
    {
        return assertTrue(video.isNativeDIBFormatSupported()); // should be okay on main test machine
    }

    bool testIsAnisoFilteringSupported()
    {
        return assertTrue(video.isAnisoFilteringSupported()); // should be okay on main test machine
    }

    bool testGetMaximumAnisoFiltering()
    {
        return assertGreater(video.getMaximumAnisoFiltering(), 1.f); // should be okay on main test machine
    }

    bool testIsTextureCompressionSupported()
    {
        return assertTrue(video.isTextureCompressionSupported()); // should be okay on main test machine
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

    bool testIsAdvancedPixelFormatSetSupported()
    {
        return assertTrue( video.isAdvancedPixelFormatSetSupported() ); // should be okay on main test machine
    }

    bool testIsFullSceneAntiAliasingSupported()
    {
        return assertTrue( video.isFullSceneAntiAliasingSupported() ); // should be okay on main test machine
    }

    bool testGetMaxSamplesCount()
    {
        return assertGreater( video.getMaxSamplesCount(), 1 ); // should be okay on main test machine
    }

    bool testIsSamplesSupported()
    {
        return assertTrue(video.isSamplesSupported(2)); // should be okay on main test machine
    }

    bool testIsMultiDrawArraysSupported()
    {
        return assertTrue(video.isMultiDrawArraysSupported()); // should be okay on main test machine
    }

    bool testIsDrawRangeElementsSupported()
    {
        return assertTrue(video.isDrawRangeElementsSupported()); // should be okay on main test machine
    }

    bool testIsCompiledVertexArraySupported()
    {
        return assertTrue(video.isCompiledVertexArraySupported()); // should be okay on main test machine
    }

    bool testIsNVVertexArrayRangeSupported()
    {
        return true; // we just don't test this, should be okay on older hardware
    }

    bool testIsNVElementArraySupported()
    {
        return true; // we just don't test this, should be okay on older hardware
    }

    bool testIsATIVertexArrayObjectSupported()
    {
        return true; // we just don't test this, should be okay on older hardware
    }

    bool testIsATIMapObjectBufferSupported()
    {
        return true; // we just don't test this, should be okay on older hardware
    }

    bool testIsATIElementArraySupported()
    {
        return true; // we just don't test this, should be okay on older hardware
    }

    bool testIsVertexBufferObjectSupported()
    {
        return assertTrue(video.isVertexBufferObjectSupported()); // should be okay on main test machine
    }

    bool testIsATITruFormSupported()
    {
        return true; // we just don't test this, should be okay on older hardware
    }

    bool testIsHwOcclusionQuerySupported()
    {
        return assertTrue(video.isOcclusionQuerySupported()); // should be okay on main test machine
    }

    bool testIsHwBooleanOcclusionQuerySupported()
    {
        return assertTrue(video.isBooleanOcclusionQuerySupported()); // should be okay on main test machine
    }

    bool testGetVideocardOSName()
    {
        return assertFalse(video.getVideocardOSName().empty());
    }

    bool testGetVideocardOGLName()
    {
        return assertFalse(video.getVideocardOGLName().empty());
    }

    bool testGetVideocardOGLVersion()
    {
        return assertFalse(video.getVideocardOGLVersion().empty());
    }

    bool testGetVideocardOGLVendor()
    {
        return assertFalse(video.getVideocardOGLVendor().empty());
    }

    bool testGetVideocardOGLFeatureList()
    {
        return assertFalse(video.getVideocardOGLFeatureList().empty());
    }

    bool testGetVideocardWGLFeatureList()
    {
        return assertFalse(video.getVideocardWGLFeatureList().empty());
    }

    bool testWriteStats()
    {
        video.WriteStats();
        return true;
    }
    
}; // class PurehwInfoTest
