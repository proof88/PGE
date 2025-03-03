#pragma once

/*
    ###################################################################################
    PureTextureManagerTest.h
    Unit test for PureTextureManager.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGEallHeaders.h"
#include "../Pure/include/external/Material/PureTextureManager.h"
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

#ifndef PureIMAGETEST_FILENAMES
#define PureIMAGETEST_FILENAMES
#define BMP128x128x32op "_res/proba128x128x32_opaque.bmp"
#define BMP128x128x32tr "_res/proba128x128x32_transparent.bmp"
#define BMP128x128x24 "_res/proba128x128x24.bmp"
#define BMP128x128x8 "_res/proba128x128x8.bmp"
#define BMP128x128x4 "_res/proba128x128x4.bmp"
#define BMP128x128x1 "_res/proba128x128x1.bmp"
#endif

class PureTextureManagerTest :
    public UnitTest
{
public:

    PureTextureManagerTest() :
        UnitTest(__FILE__)
    {
        engine = NULL;
        im = NULL;
        tm = NULL;
        
    }

    virtual ~PureTextureManagerTest()
    {
        finalize();
    } // ~PureTextureManagerTest()

protected:

    virtual void initialize() override
    {
        /*CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureImage::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureImageManager::getLoggerModuleName(), true);*/
        
        engine = NULL;
        im = NULL;
        tm = NULL;
        addSubTest("testGetMIPmapCount", (PFNUNITSUBTEST) &PureTextureManagerTest::testGetMIPmapCount);
        addSubTest("testGetWidthAtLevel", (PFNUNITSUBTEST) &PureTextureManagerTest::testGetWidthAtLevel);
        addSubTest("testGetHeightAtLevel", (PFNUNITSUBTEST) &PureTextureManagerTest::testGetHeightAtLevel);
        addSubTest("testGetMIPmappedTexSize", (PFNUNITSUBTEST) &PureTextureManagerTest::testGetMIPmappedTexSize);
        addSubTest("testGetPureanisoTexFilteringNameFromFloat", (PFNUNITSUBTEST) &PureTextureManagerTest::testGetPureanisoTexFilteringNameFromFloat);
        addSubTest("testGetPureanisoTexFilteringNameFromUInt", (PFNUNITSUBTEST) &PureTextureManagerTest::testGetPureanisoTexFilteringNameFromUInt);
        addSubTest("testGetFloatFromPureanisoTexFilteringName", (PFNUNITSUBTEST) &PureTextureManagerTest::testGetFloatFromPureanisoTexFilteringName);
        addSubTest("testGetUIntFromPureanisoTexFilteringName", (PFNUNITSUBTEST) &PureTextureManagerTest::testGetUIntFromPureanisoTexFilteringName);
        addSubTest("testCtor1", (PFNUNITSUBTEST) &PureTextureManagerTest::testCtor1);
        addSubTest("testCtor2", (PFNUNITSUBTEST) &PureTextureManagerTest::testCtor2);
        addSubTest("testDtor", (PFNUNITSUBTEST) &PureTextureManagerTest::testDtor);
        addSubTest("testGetUsedTextureMemory", (PFNUNITSUBTEST) &PureTextureManagerTest::testGetUsedTextureMemory);
        addSubTest("testSetHardwareMipMapGenerationEnabled", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetHardwareMipMapGenerationEnabled);
        addSubTest("testSetNativeDIBFormatSupportEnabled", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetNativeDIBFormatSupportEnabled);
        addSubTest("testSetLazyInstancingEnabled", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetLazyInstancingEnabled);
        addSubTest("testSetPixelPreservingEnabled", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetPixelPreservingEnabled);
        addSubTest("testCreateTextureFromImage", (PFNUNITSUBTEST) &PureTextureManagerTest::testCreateTextureFromImage);
        addSubTest("testCreateTextureFromFile", (PFNUNITSUBTEST) &PureTextureManagerTest::testCreateTextureFromFile);
        addSubTest("testCreateTextureFromFileMIP", (PFNUNITSUBTEST) &PureTextureManagerTest::testCreateTextureFromFileMIP);
        addSubTest("testCreateTextureFromFileComp", (PFNUNITSUBTEST) &PureTextureManagerTest::testCreateTextureFromFileComp);
        addSubTest("testCreateTextureFromFileCompMIP", (PFNUNITSUBTEST) &PureTextureManagerTest::testCreateTextureFromFileCompMIP);
        addSubTest("testCreateTextureFromFileBorder", (PFNUNITSUBTEST) &PureTextureManagerTest::testCreateTextureFromFileBorder);
        addSubTest("testSetDefaultMinFilteringMode", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetDefaultMinFilteringMode);
        addSubTest("testSetDefaultMagFilteringMode", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetDefaultMagFilteringMode);
        addSubTest("testSetDefaultIsoFilteringMode", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetDefaultIsoFilteringMode);
        addSubTest("testSetDefaultAnisoFilteringMode", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetDefaultAnisoFilteringMode);
        addSubTest("testSetDefaultWrappingMode", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetDefaultWrappingMode);
        addSubTest("testSetDefaultCompressionMode", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetDefaultCompressionMode);
        addSubTest("testSetDefaultBorder", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetDefaultBorder);
        addSubTest("testSetDefaultBorderColor", (PFNUNITSUBTEST) &PureTextureManagerTest::testSetDefaultBorderColor);
        addSubTest("testWriteList", (PFNUNITSUBTEST) &PureTextureManagerTest::testWriteList);
    }

    virtual bool setUp() override
    {
        bool ret = true;
        if ( engine == NULL )
        {
            PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
            engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
            ret &= assertEquals((TPureUInt)0, engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0), "engine" );  // pretty standard display mode, should work on most systems
            im = &engine->getImageManager();
            tm = &engine->getTextureManager();
            ret &= assertNotNull(im, "im null");
            ret &= assertNotNull(tm, "tm null");
        }
        return ret;
    }

    virtual void tearDown() override
    {
        finalize();
        im = NULL;
        tm = NULL;
    }

    virtual void finalize() override
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureImage::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureImageManager::getLoggerModuleName(), false);
    }



private:
    PR00FsUltimateRenderingEngine* engine;
    PureImageManager* im;
    PureTextureManager* tm;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PureTextureManagerTest(const PureTextureManagerTest&)
    {};         

    PureTextureManagerTest& operator=(const PureTextureManagerTest&)
    {
        return *this;
    };

    bool testGetMIPmapCount()
    {
        return assertEquals((TPureUInt) 1, PureTextureManager::getMIPmapCount(1,1), "1") &
            assertEquals((TPureUInt) 2, PureTextureManager::getMIPmapCount(2,2), "2") &
            assertEquals((TPureUInt) 9, PureTextureManager::getMIPmapCount(256,256), "256") &
            assertEquals((TPureUInt) 10, PureTextureManager::getMIPmapCount(512,256), "512x256") &
            assertEquals((TPureUInt) 0, PureTextureManager::getMIPmapCount(0,256), "0");
    }

    bool testGetWidthAtLevel()
    {
        return assertEquals((TPureUInt) 256, PureTextureManager::getWidthAtLevel(0,256,256), "0") &
            assertEquals((TPureUInt) 128, PureTextureManager::getWidthAtLevel(1,256,256), "1") &
            assertEquals((TPureUInt) 1, PureTextureManager::getWidthAtLevel(8,256,256), "8") &
            assertEquals((TPureUInt) 2, PureTextureManager::getWidthAtLevel(8,512,256), "8 512x256") &
            assertEquals((TPureUInt) 1, PureTextureManager::getWidthAtLevel(9,512,256), "9 512x256") &
            assertEquals((TPureUInt) 1, PureTextureManager::getWidthAtLevel(10,512,256), "10 512x256");
    }

    bool testGetHeightAtLevel()
    {
        return assertEquals((TPureUInt) 256, PureTextureManager::getHeightAtLevel(0,256,256), "0") &
            assertEquals((TPureUInt) 128, PureTextureManager::getHeightAtLevel(1,256,256), "1") &
            assertEquals((TPureUInt) 1, PureTextureManager::getHeightAtLevel(8,256,256), "8") &
            assertEquals((TPureUInt) 1, PureTextureManager::getHeightAtLevel(8,512,256), "8 512x256") &
            assertEquals((TPureUInt) 1, PureTextureManager::getHeightAtLevel(9,512,256), "9 512x256") &
            assertEquals((TPureUInt) 1, PureTextureManager::getHeightAtLevel(10,512,256), "10");
    }

    bool testGetMIPmappedTexSize()
    {
        // because MIP mapping uses ~+33% memory
        return assertEquals((unsigned int) PFL::roundi(256*256*4*1.33333f), PureTextureManager::getMIPmappedTexSize(256*256*4, 256,256));
    }

    bool testGetPureanisoTexFilteringNameFromFloat()
    {
        return assertEquals( PURE_ANISO_1X, PureTextureManager::getPureanisoTexFilteringNameFromFloat(1.0f), "1") &
            assertEquals( PURE_ANISO_2X, PureTextureManager::getPureanisoTexFilteringNameFromFloat(2.0f), "2") &
            assertEquals( PURE_ANISO_4X, PureTextureManager::getPureanisoTexFilteringNameFromFloat(4.0f), "4") &
            assertEquals( PURE_ANISO_8X, PureTextureManager::getPureanisoTexFilteringNameFromFloat(8.0f), "8") &
            assertEquals(PURE_ANISO_16X, PureTextureManager::getPureanisoTexFilteringNameFromFloat(16.0f), "16") &
            assertEquals( PURE_ANISO_1X, PureTextureManager::getPureanisoTexFilteringNameFromFloat(0), "0") &
            assertEquals( PURE_ANISO_1X, PureTextureManager::getPureanisoTexFilteringNameFromFloat(-4), "-4") &
            assertEquals( PURE_ANISO_1X, PureTextureManager::getPureanisoTexFilteringNameFromFloat(42), "42");
    }

    bool testGetPureanisoTexFilteringNameFromUInt()
    {
        return assertEquals( PURE_ANISO_1X, PureTextureManager::getPureanisoTexFilteringNameFromUInt(1), "1") &
            assertEquals( PURE_ANISO_2X, PureTextureManager::getPureanisoTexFilteringNameFromUInt(2), "2") &
            assertEquals( PURE_ANISO_4X, PureTextureManager::getPureanisoTexFilteringNameFromUInt(4), "4") &
            assertEquals( PURE_ANISO_8X, PureTextureManager::getPureanisoTexFilteringNameFromUInt(8), "8") &
            assertEquals(PURE_ANISO_16X, PureTextureManager::getPureanisoTexFilteringNameFromUInt(16), "16") &
            assertEquals( PURE_ANISO_1X, PureTextureManager::getPureanisoTexFilteringNameFromUInt(0), "0") &
            assertEquals( PURE_ANISO_1X, PureTextureManager::getPureanisoTexFilteringNameFromUInt(42), "42");
    }

    bool testGetFloatFromPureanisoTexFilteringName()
    {
        return assertEquals( 1.0f, PureTextureManager::getFloatFromPureanisoTexFilteringName(PURE_ANISO_1X), "1") &
            assertEquals( 2.0f, PureTextureManager::getFloatFromPureanisoTexFilteringName(PURE_ANISO_2X), "2") &
            assertEquals( 4.0f, PureTextureManager::getFloatFromPureanisoTexFilteringName(PURE_ANISO_4X), "4") &
            assertEquals( 8.0f, PureTextureManager::getFloatFromPureanisoTexFilteringName(PURE_ANISO_8X), "8") &
            assertEquals(16.0f, PureTextureManager::getFloatFromPureanisoTexFilteringName(PURE_ANISO_16X), "16");
    }

    bool testGetUIntFromPureanisoTexFilteringName()
    {
        return assertEquals((TPureUInt) 1, PureTextureManager::getUIntFromPureanisoTexFilteringName(PURE_ANISO_1X), "1") &
            assertEquals((TPureUInt) 2, PureTextureManager::getUIntFromPureanisoTexFilteringName(PURE_ANISO_2X), "2") &
            assertEquals((TPureUInt) 4, PureTextureManager::getUIntFromPureanisoTexFilteringName(PURE_ANISO_4X), "4") &
            assertEquals((TPureUInt) 8, PureTextureManager::getUIntFromPureanisoTexFilteringName(PURE_ANISO_8X), "8") &
            assertEquals((TPureUInt)16, PureTextureManager::getUIntFromPureanisoTexFilteringName(PURE_ANISO_16X), "16");
    }

    bool testCtor1()
    {
        PureImage& img = *(im->createFromFile(BMP128x128x24));
        img.DetachFrom();
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        tm = new PureTextureManager();
        bool b = assertNull(tm->createFromFile(BMP128x128x24), "createTexture");
        b &= assertNull(tm->createTextureFromImage(img), "createImage");
        
        return b &
            assertEquals(PURE_ANISO_1X, tm->getDefaultAnisoFilteringMode(), "Aniso") &
            assertEquals(PURE_TC_NONE, tm->getDefaultCompressionMode(), "Compression") &
            assertEquals(PURE_ISO_LINEAR, tm->getDefaultMinFilteringMode(), "MinFilter") &
            assertEquals(PURE_ISO_LINEAR, tm->getDefaultMagFilteringMode(), "MagFilter") &
            assertFalse(tm->getDefaultBorder(), "Border") &
            assertTrue(tm->getDefaultBorderColor() == PureColor(0,0,0,0), "BorderColor") &
            assertEquals(PURE_ANISO_1X, tm->getMaximumAnisoFiltering(), "MaxAniso") &
            assertEquals(PURE_TW_REPEAT, tm->getDefaultTextureWrappingModeS(), "Wrapping S") &
            assertEquals(PURE_TW_REPEAT, tm->getDefaultTextureWrappingModeT(), "Wrapping T") &
            assertEquals((TPureUInt) 0, tm->getUsedTextureMemory(), "usedTexMem") &
            assertFalse(tm->isHardwareMipMapGenerationEnabled(), "HW MIP map gen") &
            assertFalse(tm->isNativeDIBFormatSupportEnabled(), "native DIB");
    }

    bool testCtor2()
    {
        if ( !assertNotNull(tm) )
        {
            return false;
        }

        // HW specific but should pass on main test machine
        return assertEquals(PURE_ANISO_1X, tm->getDefaultAnisoFilteringMode(), "Aniso") &
            assertEquals(PURE_TC_NONE, tm->getDefaultCompressionMode(), "Compression") &
            assertEquals(PURE_ISO_LINEAR, tm->getDefaultMinFilteringMode(), "MinFilter") &
            assertEquals(PURE_ISO_LINEAR, tm->getDefaultMagFilteringMode(), "MagFilter") &
            assertFalse(tm->getDefaultBorder(), "Border") &
            assertTrue(tm->getDefaultBorderColor() == PureColor(0,0,0,0), "BorderColor") &
            assertEquals(PURE_ANISO_16X, tm->getMaximumAnisoFiltering(), "MaxAniso") &
            assertEquals(PURE_TW_REPEAT, tm->getDefaultTextureWrappingModeS(), "Wrapping S") &
            assertEquals(PURE_TW_REPEAT, tm->getDefaultTextureWrappingModeT(), "Wrapping T") &
            assertEquals((TPureUInt)0, tm->getUsedTextureMemory(), "usedTexMem") &
            assertTrue(tm->isHardwareMipMapGenerationEnabled(), "HW MIP map gen") &
            assertTrue(tm->isNativeDIBFormatSupportEnabled(), "native DIB");
    }

    bool testDtor()
    {
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        return true;
    }

    bool testGetUsedTextureMemory()
    {
        const PureTexture* const tex24 = tm->createFromFile(BMP128x128x24);
        const PureTexture* const tex32 = tm->createFromFile(BMP128x128x32op);

        if ( !assertNotNull(tex24, "tex24 null") || !assertNotNull(tex32, "tex32 null") )
        {
            return false;
        }

        bool b = assertEquals(tex24->getUsedTextureMemory() + tex32->getUsedTextureMemory(), tm->getUsedTextureMemory(), "b1" );

        tm->DeleteAll();

        b &= assertEquals((TPureUInt)0, tm->getUsedTextureMemory(), "b2");

        return b;
    }

    bool testSetHardwareMipMapGenerationEnabled()
    {
        tm->SetHardwareMipMapGenerationEnabled(false);
        return assertFalse(tm->isHardwareMipMapGenerationEnabled());
    }

    bool testSetNativeDIBFormatSupportEnabled()
    {
        tm->SetNativeDIBFormatSupportEnabled(false);
        return assertFalse(tm->isNativeDIBFormatSupportEnabled());
    }

    bool testSetLazyInstancingEnabled()
    {
        tm->SetLazyInstancingEnabled(true);
        return assertTrue(tm->isLazyInstancingEnabled());
    }

    bool testSetPixelPreservingEnabled()
    {
        tm->SetPixelPreservingEnabled(true);
        return assertTrue(tm->isPixelPreservingEnabled());
    }

    bool testCreateTextureFromImage()
    {
        // lazy instancing is enabled by default
        const PureImage& img = *(im->createFromFile(BMP128x128x24));
        const PureTexture* const tex = tm->createTextureFromImage(img);

        const bool bKeep0 = assertNotEquals((const void*) PGENULL, (const void*) img.getPixels(), "bKeep0"); // the original img object has still the pixels even if the created texture doesn't
                                          
        TPureInt nTextures = tm->getCount();
        const PureTexture* const tex2 = tm->createTextureFromImage(img);
        if ( !assertNotNull(tex, "tex notnull") || !assertNotNull(tex2, "tex2") )
        {
            return false;
        }

        const bool b1 = assertEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertEquals(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(false);
        nTextures = tm->getCount();
        const PureTexture* const tex3 = tm->createTextureFromImage(img);
        if ( !assertNotNull(tex3, "tex3") )
        {
            return false;
        }

        const bool b3 = assertNotEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertGreater(tm->getCount(), nTextures, "b4 count");

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PureTexture* const texNativeDIBoff = tm->createTextureFromImage(img);
        if ( !assertNotNull(texNativeDIBoff, "texNativeDIBoff") )
        {
            return false;
        }

        tm->SetNativeDIBFormatSupportEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PureTexture* const texK1 = tm->createTextureFromImage(img);
        if ( !assertNotNull(texK1, "texK1") )
        {
            return false;
        }

        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        if ( !assertNotNull(tex, "tex notnull") || !assertNotNull(texNativeDIBoff, "nativeDIB") )
        {
            return false;
        }

        return b1 & b2 & b3 & b4 &
            bKeep0 & bKeep1 & bKeep2;
    }

    bool testCreateTextureFromFile()
    {
        // lazy instancing is enabled by default
        const PureTexture* const tex = tm->createFromFile(BMP128x128x24);

        TPureInt nTextures = tm->getCount();
        const PureTexture* const tex2 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(tex, "tex notnull") || !assertNotNull(tex2, "tex2") )
        {
            return false;
        }

        const bool b1 = assertEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertEquals(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(false);
        nTextures = tm->getCount();
        const PureTexture* const tex3 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(tex3, "tex3") )
        {
            return false;
        }

        const bool b3 = assertNotEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertGreater(tm->getCount(), nTextures, "b4 count");

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PureTexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texNativeDIBoff, "texNativeDIBoff") )
        {
            return false;
        }

        tm->SetNativeDIBFormatSupportEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PureTexture* const texK1 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texK1, "texK1") )
        {
            return false;
        }

        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        return b1 & b2 & b3 & b4 & bKeep1 & bKeep2;
    }

    bool testCreateTextureFromFileMIP()
    {
        // lazy instancing is enabled by default
        bool b = assertTrue(tm->setDefaultIsoFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR, PURE_ISO_LINEAR), "set 1");
        const PureTexture* const tex = tm->createFromFile(BMP128x128x24);

        TPureInt nTextures = tm->getCount();
        const PureTexture* const tex2 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(tex, "tex notnull") || !assertNotNull(tex2, "tex2") )
        {
            return false;
        }

        const bool b1 = assertEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertEquals(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(false);
        nTextures = tm->getCount();
        const PureTexture* const tex3 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(tex3, "tex3") )
        {
            return false;
        }

        const bool b3 = assertNotEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertGreater(tm->getCount(), nTextures, "b4 count");

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PureTexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texNativeDIBoff, "texNativeDIBoff") )
        {
            return false;
        }

        tm->SetNativeDIBFormatSupportEnabled(true);

        tm->SetHardwareMipMapGenerationEnabled(false);
        const PureTexture* const texMipMapGenHWoff = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texMipMapGenHWoff, "texMipMapGenHWoff") )
        {
            return false;
        }

        tm->SetHardwareMipMapGenerationEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PureTexture* const texK1 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texK1, "texK1") )
        {
            return false;
        }

        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        return b & b1 & b2 & b3 & b4 & bKeep1 & bKeep2;
    }

    bool testCreateTextureFromFileComp()
    {
        // lazy instancing is enabled by default
        bool b = assertTrue(tm->setDefaultCompressionMode(PURE_TC_S3TC_RGB_DXT1), "set 1");
        const PureTexture* const tex = tm->createFromFile(BMP128x128x24);

        TPureInt nTextures = tm->getCount();
        const PureTexture* const tex2 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(tex, "tex notnull") || !assertNotNull(tex2, "tex2") )
        {
            return false;
        }

        const bool b1 = assertEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertEquals(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(false);
        nTextures = tm->getCount();
        const PureTexture* const tex3 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(tex3, "tex3") )
        {
            return false;
        }

        const bool b3 = assertNotEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertGreater(tm->getCount(), nTextures, "b4 count");

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PureTexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texNativeDIBoff, "texNativeDIBoff") )
        {
            return false;
        }

        tm->SetNativeDIBFormatSupportEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PureTexture* const texK1 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texK1, "texK1") )
        {
            return false;
        }

        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        b &= assertTrue(tm->setDefaultCompressionMode(PURE_TC_S3TC_RGBA_DXT1), "set 2"); // check RGBA too
        const PureTexture* const texComp2 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texComp2, "texComp2") )
        {
            return false;
        }

        b &= assertTrue(tm->setDefaultCompressionMode(PURE_TC_AUTO), "set 3"); // check auto too
        const PureTexture* const texComp3 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texComp3, "texComp3") )
        {
            return false;
        }

        return b & b1 & b2 & b3 & b4 & bKeep1 & bKeep2;
    }

    bool testCreateTextureFromFileCompMIP()
    {
        // lazy instancing is enabled by default
        bool b = assertTrue(tm->setDefaultIsoFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR, PURE_ISO_LINEAR), "set 1");
        b &= assertTrue(tm->setDefaultCompressionMode(PURE_TC_S3TC_RGB_DXT1), "set 2");
        const PureTexture* const tex = tm->createFromFile(BMP128x128x24);

        TPureInt nTextures = tm->getCount();
        const PureTexture* const tex2 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(tex, "tex notnull") || !assertNotNull(tex2, "tex2") )
        {
            return false;
        }

        const bool b1 = assertEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertEquals(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(false);
        nTextures = tm->getCount();
        const PureTexture* const tex3 = tm->createFromFile(BMP128x128x24);
        const bool b3 = assertNotEquals(tex3, tex, "tex3 == tex");
        if ( !assertNotNull(tex3, "tex3") )
        {
            return false;
        }

        const bool b4 = assertGreater(tm->getCount(), nTextures, "b4 count");
        tm->SetLazyInstancingEnabled(false);

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PureTexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texNativeDIBoff, "texNativeDIBoff") )
        {
            return false;
        }

        tm->SetNativeDIBFormatSupportEnabled(true);

        tm->SetHardwareMipMapGenerationEnabled(false);
        const PureTexture* const texMipMapGenHWoff = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texMipMapGenHWoff, "texMipMapGenHWoff") )
        {
            return false;
        }

        tm->SetHardwareMipMapGenerationEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PureTexture* const texK1 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texK1, "texK1") )
        {
            return false;
        }

        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        b &= assertTrue(tm->setDefaultCompressionMode(PURE_TC_S3TC_RGBA_DXT1), "set 3"); // check RGBA too
        const PureTexture* const texComp2 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texComp2, "texComp2") )
        {
            return false;
        }

        b &= assertTrue(tm->setDefaultCompressionMode(PURE_TC_AUTO), "set 4"); // check auto too
        const PureTexture* const texComp3 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texComp3, "texComp3") )
        {
            return false;
        }

        return b & b1 & b2 & b3 & b4 & bKeep1 & bKeep2;
    }

    bool testCreateTextureFromFileBorder()
    {
        // lazy instancing is enabled by default
        bool b = assertTrue(tm->setDefaultBorder(true), "set 1");
        PureColor expColor(255,255,255,255);
        tm->getDefaultBorderColor() = expColor;

        const PureTexture* const tex = tm->createFromFile(BMP128x128x24);

        TPureInt nTextures = tm->getCount();
        const PureTexture* const tex2 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(tex, "tex notnull") || !assertNotNull(tex2, "tex2") )
        {
            return false;
        }

        const bool b1 = assertEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertEquals(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(false);
        nTextures = tm->getCount();
        const PureTexture* const tex3 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(tex3, "tex3") )
        {
            return false;
        }

        const bool b3 = assertNotEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertGreater(tm->getCount(), nTextures, "b4 count");

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PureTexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texNativeDIBoff, "texNativeDIBoff") )
        {
            return false;
        }

        tm->SetNativeDIBFormatSupportEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PureTexture* const texK1 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texK1, "texK1") )
        {
            return false;
        }

        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        // Border + compression made glTexImage2D to fail with INVALID_VALUE, because of the border parameter, thus
        // this test is disabled because it is very GL-implementation-dependent!
        // b &= assertTrue(tm->setDefaultCompressionMode(PURE_TC_S3TC_RGBA_DXT1); // check RGBA too
        // const PureTexture* const texComp2 = tm->createFromFile(BMP128x128x24);
        // if ( !assertNotNull(texComp2, "texComp2") )
        // {
        //     return false;
        // }

        b &= assertTrue(tm->setDefaultCompressionMode(PURE_TC_AUTO), "set 2"); // check auto too
        const PureTexture* const texComp3 = tm->createFromFile(BMP128x128x24);
        if ( !assertNotNull(texComp3, "texComp3") )
        {
            return false;
        }

        return b & b1 & b2 & b3 & b4 & bKeep1 & bKeep2 &
            assertTrue(tex->getBorderColor() == expColor, "tex clr") &
            assertTrue(tex2->getBorderColor() == expColor, "tex2 clr") &
            assertTrue(tex3->getBorderColor() == expColor, "tex3 clr") &
            assertTrue(texNativeDIBoff->getBorderColor() == expColor, "texNativeDIBoff clr") &
            assertTrue(texK1->getBorderColor() == expColor, "texK1 clr") &
            /*assertTrue(texComp2->getBorderColor() == expColor, "texComp2 clr") &*/
            assertTrue(texComp3->getBorderColor() == expColor, "texComp3 clr");
    }

    bool testSetDefaultMinFilteringMode()
    {
        bool b = assertTrue(tm->setDefaultMinFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR), "set");
        return b & assertEquals(PURE_ISO_LINEAR_MIPMAP_LINEAR, tm->getDefaultMinFilteringMode());
    }

    bool testSetDefaultMagFilteringMode()
    {
        bool b = assertFalse(tm->setDefaultMagFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR), "set 1");
        const bool b1 = (tm->getDefaultMagFilteringMode() == PURE_ISO_LINEAR);
        b &= assertFalse(tm->setDefaultMagFilteringMode(PURE_ISO_LINEAR_MIPMAP_NEAREST), "set 2");
        const bool b2 = (tm->getDefaultMagFilteringMode() == PURE_ISO_LINEAR);
        b &= assertTrue(tm->setDefaultMagFilteringMode(PURE_ISO_NEAREST), "set 3");
        const bool b3 = (tm->getDefaultMagFilteringMode() == PURE_ISO_NEAREST);

        return b & assertTrue(b1, "b1") & assertTrue(b2, "b2") & assertTrue(b3, "b3");
    }

    bool testSetDefaultIsoFilteringMode()
    {
        bool b = assertFalse(tm->setDefaultIsoFilteringMode(PURE_ISO_LINEAR, PURE_ISO_LINEAR_MIPMAP_LINEAR), "set 1");
        const bool b1 = tm->getDefaultMagFilteringMode() == PURE_ISO_LINEAR;
        const bool b11 = tm->getDefaultMinFilteringMode() == PURE_ISO_LINEAR;
        b &= assertFalse(tm->setDefaultIsoFilteringMode(PURE_ISO_LINEAR, PURE_ISO_LINEAR_MIPMAP_NEAREST), "set 2");
        const bool b2 = tm->getDefaultMagFilteringMode() == PURE_ISO_LINEAR;
        const bool b22 = tm->getDefaultMinFilteringMode() == PURE_ISO_LINEAR;
        b &= assertTrue(tm->setDefaultIsoFilteringMode(PURE_ISO_LINEAR, PURE_ISO_NEAREST), "set 3");
        const bool b3 = tm->getDefaultMagFilteringMode() == PURE_ISO_NEAREST;
        const bool b33 = tm->getDefaultMinFilteringMode() == PURE_ISO_LINEAR;

        return b &
            assertTrue(b1, "b1") & assertTrue(b2, "b2") & assertTrue(b3, "b3") &
            assertTrue(b11, "b11") & assertTrue(b22, "b22") & assertTrue(b33, "b33");
    }

    bool testSetDefaultAnisoFilteringMode()
    {
        bool b = assertTrue(tm->setDefaultAnisoFilteringMode(PURE_ANISO_16X), "set");
        return b & assertEquals(PURE_ANISO_16X, tm->getDefaultAnisoFilteringMode());
    }

    bool testSetDefaultWrappingMode()
    {
        bool b = assertTrue(tm->setDefaultTextureWrappingMode(PURE_TW_CLAMP, PURE_TW_CLAMP_TO_EDGE), "set 1");
        const bool b1 = assertEquals(PURE_TW_CLAMP, tm->getDefaultTextureWrappingModeS(), "PURE_TW_CLAMP");
        const bool b2 = assertEquals(PURE_TW_CLAMP_TO_EDGE, tm->getDefaultTextureWrappingModeT(), "PURE_TW_CLAMP_TO_EDGE");

        b &= assertTrue(tm->setDefaultTextureWrappingMode(PURE_TW_CLAMP_TO_BORDER, PURE_TW_MIRRORED_REPEAT), "set 2");
        const bool b3 = assertEquals(PURE_TW_CLAMP_TO_BORDER, tm->getDefaultTextureWrappingModeS(), "PURE_TW_CLAMP_TO_BORDER");
        const bool b4 = assertEquals(PURE_TW_MIRRORED_REPEAT, tm->getDefaultTextureWrappingModeT(), "PURE_TW_MIRRORED_REPEAT");

        b &= assertTrue(tm->setDefaultTextureWrappingMode(PURE_TW_REPEAT, PURE_TW_REPEAT), "set 3");
        const bool b5 = assertEquals(PURE_TW_REPEAT, tm->getDefaultTextureWrappingModeS(), "PURE_TW_REPEAT S");
        const bool b6 = assertEquals(PURE_TW_REPEAT, tm->getDefaultTextureWrappingModeT(), "PURE_TW_REPEAT T");

        return b & b1 & b2 & b3 & b4 & b5 & b6;
    }

    bool testSetDefaultCompressionMode()
    {
        bool b = assertTrue(tm->setDefaultCompressionMode(PURE_TC_S3TC_RGB_DXT1), "set");
        return b & assertEquals(PURE_TC_S3TC_RGB_DXT1, tm->getDefaultCompressionMode());
    }

    bool testSetDefaultBorder()
    {
        bool b = assertTrue(tm->setDefaultBorder(true), "set");
        return b & assertTrue( tm->getDefaultBorder() );
    }

    bool testSetDefaultBorderColor()
    {
        tm->getDefaultBorderColor().Set(1,1,1,1);

        return assertTrue( tm->getDefaultBorderColor() == PureColor(1,1,1,1) );
    }

    bool testWriteList()
    {
        tm->WriteList();
        return true;
    }
    
}; // class PureTextureManagerTest
