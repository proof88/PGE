#pragma once

/*
    ###################################################################################
    PureHwVideoTest.h
    Unit test for PureHwVideo.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"
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
        finalize();   
    }

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState("4LLM0DUL3S", true);
        engine = NULL;
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureHwVideoTest::testCtor);
        addSubTest("testInitialize", (PFNUNITSUBTEST) &PureHwVideoTest::testInitialize);
        addSubTest("testDeinitialize", (PFNUNITSUBTEST) &PureHwVideoTest::testDeinitialize);
        addSubTest("testIsExtensionSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsExtensionSupported);
        addSubTest("testGetUsedVideoMemory", (PFNUNITSUBTEST) &PureHwVideoTest::testGetUsedVideoMemory);
        addSubTest("testGetUsedTextureMemory", (PFNUNITSUBTEST) &PureHwVideoTest::testGetUsedTextureMemory);
        addSubTest("testGetColorBufferPixelCount", (PFNUNITSUBTEST) &PureHwVideoTest::testGetColorBufferPixelCount);
        addSubTest("testGetColorBufferSize", (PFNUNITSUBTEST) &PureHwVideoTest::testGetColorBufferSize);
        addSubTest("testGetDepthBufferSize", (PFNUNITSUBTEST) &PureHwVideoTest::testGetDepthBufferSize);
        addSubTest("testGetStencilBufferSize", (PFNUNITSUBTEST) &PureHwVideoTest::testGetStencilBufferSize);
        addSubTest("testGetSampleBufferSize", (PFNUNITSUBTEST) &PureHwVideoTest::testGetSampleBufferSize);
        addSubTest("testGetUsedFrameBufferMemory", (PFNUNITSUBTEST) &PureHwVideoTest::testGetUsedFrameBufferMemory);
        addSubTest("testIsVSyncSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsVSyncSupported);
        addSubTest("testIsMultiTexturingSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsMultiTexturingSupported);
        addSubTest("testGetTextureUnitsCount", (PFNUNITSUBTEST) &PureHwVideoTest::testGetTextureUnitsCount);
        addSubTest("testIsHardwareMipMapGenerationSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsHardwareMipMapGenerationSupported);
        addSubTest("testIsNativeDIBFormatSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsNativeDIBFormatSupported);
        addSubTest("testIsAnisoFilteringSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsAnisoFilteringSupported);
        addSubTest("testGetMaximumAnisoFiltering", (PFNUNITSUBTEST) &PureHwVideoTest::testGetMaximumAnisoFiltering);
        addSubTest("testIsTextureCompressionSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsTextureCompressionSupported);
        addSubTest("testIsVTCSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsVTCSupported);
        addSubTest("testIs3DcSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIs3DcSupported);
        addSubTest("testIsRGTCSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsRGTCSupported);
        addSubTest("testIsLATCCSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsLATCSupported);
        addSubTest("testIsAdvancedPixelFormatSetSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsAdvancedPixelFormatSetSupported);
        addSubTest("testIsFullSceneAntiAliasingSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsFullSceneAntiAliasingSupported);
        addSubTest("testGetMaxSamplesCount", (PFNUNITSUBTEST) &PureHwVideoTest::testGetMaxSamplesCount);
        addSubTest("testIsSamplesSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsSamplesSupported);
        addSubTest("testIsMultiDrawArraysSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsMultiDrawArraysSupported);
        addSubTest("testIsDrawRangeElementsSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsDrawRangeElementsSupported);
        addSubTest("testIsCompiledVertexArraySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsCompiledVertexArraySupported);
        addSubTest("testIsNVVertexArrayRangeSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsNVVertexArrayRangeSupported);
        addSubTest("testIsNVElementArraySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsNVElementArraySupported);
        addSubTest("testIsATIVertexArrayObjectSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsATIVertexArrayObjectSupported);
        addSubTest("testIsATIMapObjectBufferSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsATIMapObjectBufferSupported);
        addSubTest("testIsATIElementArraySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsATIElementArraySupported);
        addSubTest("testIsVertexBufferObjectSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsVertexBufferObjectSupported);
        addSubTest("testIsATITruFormSupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsATITruFormSupported);
        addSubTest("testIsHwOcclusionQuerySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsHwOcclusionQuerySupported);
        addSubTest("testIsHwBooleanOcclusionQuerySupported", (PFNUNITSUBTEST) &PureHwVideoTest::testIsHwBooleanOcclusionQuerySupported);
        addSubTest("testGetVideocardOSName", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOSName);
        addSubTest("testGetVideocardOGLName", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOGLName);
        addSubTest("testGetVideocardOGLVersion", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOGLVersion);
        addSubTest("testGetVideocardOGLVendor", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOGLVendor);
        addSubTest("testGetVideocardOGLFeatureList", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardOGLFeatureList);
        addSubTest("testGetVideocardWGLFeatureList", (PFNUNITSUBTEST) &PureHwVideoTest::testGetVideocardWGLFeatureList);
        addSubTest("testWriteStats", (PFNUNITSUBTEST) &PureHwVideoTest::testWriteStats);
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

    virtual void tearDown() override
    {
        
    }

    virtual void finalize() override
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
    PureHwVideo& video;
    PGEcfgProfiles cfgProfiles;

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
            assertEquals((TPureUInt) 0, video.getUsedVideoMemory(), "getUsedVideoMemory()") &
            assertEquals((TPureUInt) 0, video.getUsedTextureMemory(), "getUsedTextureMemory()") &
            assertEquals((TPureUInt) 0, video.getColorBufferPixelCount(), "getColorBufferPixelCount()") &
            assertEquals((TPureUInt) 0, video.getColorBufferSize(), "getColorBufferSize()") &
            assertEquals((TPureUInt) 0, video.getDepthBufferSize(), "getDepthBufferSize()") &
            assertEquals((TPureUInt) 0, video.getStencilBufferSize(), "getStencilBufferSize()") &
            assertEquals((TPureUInt) 0, video.getSampleBufferSize(), "getSampleBufferSize()") &
            assertEquals((TPureUInt) 0, video.getUsedFrameBufferMemory(), "getUsedFrameBufferMemory()") &
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
        return assertGequals(video.getUsedTextureMemory(), (TPureULong) 0);
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
        return assertEquals((TPureUInt) 0, video.getStencilBufferSize());
    }

    bool testGetSampleBufferSize()
    {
        return assertEquals((TPureUInt) 0, video.getSampleBufferSize());
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
    
}; // class PureHwInfoTest
