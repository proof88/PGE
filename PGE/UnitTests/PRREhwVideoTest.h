#pragma once

/*
    ###################################################################################
    PRREhwVideoTest.h
    Unit test for PRREhwVideo.
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

class PRREhwVideoTest :
    public UnitTest
{
public:

    PRREhwVideoTest() :
        UnitTest( __FILE__ ),
        hw( PRREhwInfo::get() ),
        video( PRREhwVideo::get() )
    {
        engine = NULL;
        
    }

    virtual ~PRREhwVideoTest()
    {
        Finalize();   
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREhwVideoTest::testCtor);
        AddSubTest("testInitialize", (PFNUNITSUBTEST) &PRREhwVideoTest::testInitialize);
        AddSubTest("testDeinitialize", (PFNUNITSUBTEST) &PRREhwVideoTest::testDeinitialize);
        AddSubTest("testIsExtensionSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsExtensionSupported);
        AddSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetUsedVideoMemory);
        AddSubTest("testGetUsedTextureMemory", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetUsedTextureMemory);
        AddSubTest("testGetColorBufferPixelCount", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetColorBufferPixelCount);
        AddSubTest("testGetColorBufferSize", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetColorBufferSize);
        AddSubTest("testGetDepthBufferSize", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetDepthBufferSize);
        AddSubTest("testGetStencilBufferSize", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetStencilBufferSize);
        AddSubTest("testGetSampleBufferSize", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetSampleBufferSize);
        AddSubTest("testGetUsedFrameBufferMemory", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetUsedFrameBufferMemory);
        AddSubTest("testIsVSyncSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsVSyncSupported);
        AddSubTest("testIsMultiTexturingSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsMultiTexturingSupported);
        AddSubTest("testGetTextureUnitsCount", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetTextureUnitsCount);
        AddSubTest("testIsHardwareMipMapGenerationSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsHardwareMipMapGenerationSupported);
        AddSubTest("testIsNativeDIBFormatSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsNativeDIBFormatSupported);
        AddSubTest("testIsAnisoFilteringSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsAnisoFilteringSupported);
        AddSubTest("testGetMaximumAnisoFiltering", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetMaximumAnisoFiltering);
        AddSubTest("testIsTextureCompressionSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsTextureCompressionSupported);
        AddSubTest("testIsVTCSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsVTCSupported);
        AddSubTest("testIs3DcSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIs3DcSupported);
        AddSubTest("testIsRGTCSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsRGTCSupported);
        AddSubTest("testIsLATCCSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsLATCSupported);
        AddSubTest("testIsAdvancedPixelFormatSetSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsAdvancedPixelFormatSetSupported);
        AddSubTest("testIsFullSceneAntiAliasingSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsFullSceneAntiAliasingSupported);
        AddSubTest("testGetMaxSamplesCount", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetMaxSamplesCount);
        AddSubTest("testIsSamplesSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsSamplesSupported);
        AddSubTest("testIsMultiDrawArraysSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsMultiDrawArraysSupported);
        AddSubTest("testIsDrawRangeElementsSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsDrawRangeElementsSupported);
        AddSubTest("testIsCompiledVertexArraySupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsCompiledVertexArraySupported);
        AddSubTest("testIsNVVertexArrayRangeSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsNVVertexArrayRangeSupported);
        AddSubTest("testIsNVElementArraySupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsNVElementArraySupported);
        AddSubTest("testIsATIVertexArrayObjectSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsATIVertexArrayObjectSupported);
        AddSubTest("testIsATIMapObjectBufferSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsATIMapObjectBufferSupported);
        AddSubTest("testIsATIElementArraySupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsATIElementArraySupported);
        AddSubTest("testIsVertexBufferObjectSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsVertexBufferObjectSupported);
        AddSubTest("testIsATITruFormSupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsATITruFormSupported);
        AddSubTest("testIsHwOcclusionQuerySupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsHwOcclusionQuerySupported);
        AddSubTest("testIsHwBooleanOcclusionQuerySupported", (PFNUNITSUBTEST) &PRREhwVideoTest::testIsHwBooleanOcclusionQuerySupported);
        AddSubTest("testGetVideocardOSName", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetVideocardOSName);
        AddSubTest("testGetVideocardOGLName", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetVideocardOGLName);
        AddSubTest("testGetVideocardOGLVersion", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetVideocardOGLVersion);
        AddSubTest("testGetVideocardOGLVendor", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetVideocardOGLVendor);
        AddSubTest("testGetVideocardOGLFeatureList", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetVideocardOGLFeatureList);
        AddSubTest("testGetVideocardWGLFeatureList", (PFNUNITSUBTEST) &PRREhwVideoTest::testGetVideocardWGLFeatureList);
        AddSubTest("testWriteStats", (PFNUNITSUBTEST) &PRREhwVideoTest::testWriteStats);
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
    PRREhwVideo& video;

    // ---------------------------------------------------------------------------

    PRREhwVideoTest(const PRREhwVideoTest&) :
        hw( PRREhwInfo::get() ),
        video( PRREhwVideo::get() )
    {};         

    PRREhwVideoTest& operator=(const PRREhwVideoTest&)
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
            assertEquals((TPRREuint) 0, video.getUsedVideoMemory(), "getUsedVideoMemory()") &
            assertEquals((TPRREuint) 0, video.getUsedTextureMemory(), "getUsedTextureMemory()") &
            assertEquals((TPRREuint) 0, video.getColorBufferPixelCount(), "getColorBufferPixelCount()") &
            assertEquals((TPRREuint) 0, video.getColorBufferSize(), "getColorBufferSize()") &
            assertEquals((TPRREuint) 0, video.getDepthBufferSize(), "getDepthBufferSize()") &
            assertEquals((TPRREuint) 0, video.getStencilBufferSize(), "getStencilBufferSize()") &
            assertEquals((TPRREuint) 0, video.getSampleBufferSize(), "getSampleBufferSize()") &
            assertEquals((TPRREuint) 0, video.getUsedFrameBufferMemory(), "getUsedFrameBufferMemory()") &
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
        return assertGreater(video.getUsedVideoMemory(), sizeof(PRREhwVideo));
    }

    bool testGetUsedTextureMemory()
    {
        return assertGequals(video.getUsedTextureMemory(), (TPRREulong) 0);
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
        return assertEquals((TPRREuint) 0, video.getStencilBufferSize());
    }

    bool testGetSampleBufferSize()
    {
        return assertEquals((TPRREuint) 0, video.getSampleBufferSize());
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
    
}; // class PRREhwInfoTest
