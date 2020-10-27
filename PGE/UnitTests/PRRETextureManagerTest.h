#pragma once

/*
    ###################################################################################
    PRRETextureManagerTest.h
    Unit test for PRRETextureManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGEallHeaders.h"
#include "../PRRE/include/external/Material/PRRETextureManager.h"
#include "../PRRE/include/external/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

#ifndef PRREIMAGETEST_FILENAMES
#define PRREIMAGETEST_FILENAMES
#define BMP128x128x32op "_res/proba128x128x32_opaque.bmp"
#define BMP128x128x32tr "_res/proba128x128x32_transparent.bmp"
#define BMP128x128x24 "_res/proba128x128x24.bmp"
#define BMP128x128x8 "_res/proba128x128x8.bmp"
#define BMP128x128x4 "_res/proba128x128x4.bmp"
#define BMP128x128x1 "_res/proba128x128x1.bmp"
#endif

class PRRETextureManagerTest :
    public UnitTest
{
public:

    PRRETextureManagerTest() :
        UnitTest(__FILE__)
    {
        engine = NULL;
        im = NULL;
        tm = NULL;
    }

    ~PRRETextureManagerTest()
    {
        Finalize();
    } // ~PRRETextureManagerTest()

protected:

    virtual void Initialize()
    {
        engine = NULL;
        tm = NULL;
        AddSubTest("testGetMIPmapCount", (PFNUNITSUBTEST) &PRRETextureManagerTest::testGetMIPmapCount);
        AddSubTest("testGetWidthAtLevel", (PFNUNITSUBTEST) &PRRETextureManagerTest::testGetWidthAtLevel);
        AddSubTest("testGetHeightAtLevel", (PFNUNITSUBTEST) &PRRETextureManagerTest::testGetHeightAtLevel);
        AddSubTest("testGetMIPmappedTexSize", (PFNUNITSUBTEST) &PRRETextureManagerTest::testGetMIPmappedTexSize);
        AddSubTest("testGetPRREanisoTexFilteringNameFromFloat", (PFNUNITSUBTEST) &PRRETextureManagerTest::testGetPRREanisoTexFilteringNameFromFloat);
        AddSubTest("testGetPRREanisoTexFilteringNameFromUInt", (PFNUNITSUBTEST) &PRRETextureManagerTest::testGetPRREanisoTexFilteringNameFromUInt);
        AddSubTest("testGetFloatFromPRREanisoTexFilteringName", (PFNUNITSUBTEST) &PRRETextureManagerTest::testGetFloatFromPRREanisoTexFilteringName);
        AddSubTest("testGetUIntFromPRREanisoTexFilteringName", (PFNUNITSUBTEST) &PRRETextureManagerTest::testGetUIntFromPRREanisoTexFilteringName);
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRRETextureManagerTest::testCtor1);
        AddSubTest("testCtor2", (PFNUNITSUBTEST) &PRRETextureManagerTest::testCtor2);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PRRETextureManagerTest::testDtor);
        AddSubTest("testGetUsedTextureMemory", (PFNUNITSUBTEST) &PRRETextureManagerTest::testGetUsedTextureMemory);
        AddSubTest("testSetHardwareMipMapGenerationEnabled", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetHardwareMipMapGenerationEnabled);
        AddSubTest("testSetNativeDIBFormatSupportEnabled", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetNativeDIBFormatSupportEnabled);
        AddSubTest("testSetLazyInstancingEnabled", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetLazyInstancingEnabled);
        AddSubTest("testSetPixelPreservingEnabled", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetPixelPreservingEnabled);
        AddSubTest("testCreateTextureFromImage", (PFNUNITSUBTEST) &PRRETextureManagerTest::testCreateTextureFromImage);
        AddSubTest("testCreateTextureFromFile", (PFNUNITSUBTEST) &PRRETextureManagerTest::testCreateTextureFromFile);
        AddSubTest("testCreateTextureFromFileMIP", (PFNUNITSUBTEST) &PRRETextureManagerTest::testCreateTextureFromFileMIP);
        AddSubTest("testCreateTextureFromFileComp", (PFNUNITSUBTEST) &PRRETextureManagerTest::testCreateTextureFromFileComp);
        AddSubTest("testCreateTextureFromFileCompMIP", (PFNUNITSUBTEST) &PRRETextureManagerTest::testCreateTextureFromFileCompMIP);
        AddSubTest("testCreateTextureFromFileBorder", (PFNUNITSUBTEST) &PRRETextureManagerTest::testCreateTextureFromFileBorder);
        AddSubTest("testSetDefaultMinFilteringMode", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetDefaultMinFilteringMode);
        AddSubTest("testSetDefaultMagFilteringMode", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetDefaultMagFilteringMode);
        AddSubTest("testSetDefaultIsoFilteringMode", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetDefaultIsoFilteringMode);
        AddSubTest("testSetDefaultAnisoFilteringMode", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetDefaultAnisoFilteringMode);
        AddSubTest("testSetDefaultWrappingMode", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetDefaultWrappingMode);
        AddSubTest("testSetDefaultCompressionMode", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetDefaultCompressionMode);
        AddSubTest("testSetDefaultBorder", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetDefaultBorder);
        AddSubTest("testSetDefaultBorderColor", (PFNUNITSUBTEST) &PRRETextureManagerTest::testSetDefaultBorderColor);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRRETextureManagerTest::testWriteList);
    }

    virtual bool setUp()
    {
        bool ret = true;
        if ( engine == NULL )
        {
            engine = &PR00FsReducedRenderingEngine::createAndGet();
            ret &= assertEquals((TPRREuint)0, engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0), "engine" );  // pretty standard display mode, should work on most systems
            im = &engine->getImageManager();
            tm = &engine->getTextureManager();
            ret &= assertNotNull(im, "im null");
            ret &= assertNotNull(tm, "tm null");
        }
        return ret;
    }

    virtual void TearDown()
    {
        Finalize();
        im = NULL;
        tm = NULL;
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
    PRREImageManager* im;
    PRRETextureManager* tm;

    // ---------------------------------------------------------------------------

    PRRETextureManagerTest(const PRRETextureManagerTest&)
    {};         

    PRRETextureManagerTest& operator=(const PRRETextureManagerTest&)
    {
        return *this;
    };

    bool testGetMIPmapCount()
    {
        return assertEquals((TPRREuint) 1, PRRETextureManager::getMIPmapCount(1,1), "1") &
            assertEquals((TPRREuint) 2, PRRETextureManager::getMIPmapCount(2,2), "2") &
            assertEquals((TPRREuint) 9, PRRETextureManager::getMIPmapCount(256,256), "256") &
            assertEquals((TPRREuint) 10, PRRETextureManager::getMIPmapCount(512,256), "512x256") &
            assertEquals((TPRREuint) 0, PRRETextureManager::getMIPmapCount(0,256), "0");
    }

    bool testGetWidthAtLevel()
    {
        return assertEquals((TPRREuint) 256, PRRETextureManager::getWidthAtLevel(0,256,256), "0") &
            assertEquals((TPRREuint) 128, PRRETextureManager::getWidthAtLevel(1,256,256), "1") &
            assertEquals((TPRREuint) 1, PRRETextureManager::getWidthAtLevel(8,256,256), "8") &
            assertEquals((TPRREuint) 2, PRRETextureManager::getWidthAtLevel(8,512,256), "8 512x256") &
            assertEquals((TPRREuint) 1, PRRETextureManager::getWidthAtLevel(9,512,256), "9 512x256") &
            assertEquals((TPRREuint) 1, PRRETextureManager::getWidthAtLevel(10,512,256), "10 512x256");
    }

    bool testGetHeightAtLevel()
    {
        return assertEquals((TPRREuint) 256, PRRETextureManager::getHeightAtLevel(0,256,256), "0") &
            assertEquals((TPRREuint) 128, PRRETextureManager::getHeightAtLevel(1,256,256), "1") &
            assertEquals((TPRREuint) 1, PRRETextureManager::getHeightAtLevel(8,256,256), "8") &
            assertEquals((TPRREuint) 1, PRRETextureManager::getHeightAtLevel(8,512,256), "8 512x256") &
            assertEquals((TPRREuint) 1, PRRETextureManager::getHeightAtLevel(9,512,256), "9 512x256") &
            assertEquals((TPRREuint) 1, PRRETextureManager::getHeightAtLevel(10,512,256), "10");
    }

    bool testGetMIPmappedTexSize()
    {
        // because MIP mapping uses ~+33% memory
        return assertEquals((unsigned int) PFL::roundi(256*256*4*1.33333f), PRRETextureManager::getMIPmappedTexSize(256*256*4, 256,256));
    }

    bool testGetPRREanisoTexFilteringNameFromFloat()
    {
        return assertEquals( PRRE_ANISO_1X, PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(1.0f), "1") &
            assertEquals( PRRE_ANISO_2X, PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(2.0f), "2") &
            assertEquals( PRRE_ANISO_4X, PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(4.0f), "4") &
            assertEquals( PRRE_ANISO_8X, PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(8.0f), "8") &
            assertEquals(PRRE_ANISO_16X, PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(16.0f), "16") &
            assertEquals( PRRE_ANISO_1X, PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(0), "0") &
            assertEquals( PRRE_ANISO_1X, PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(-4), "-4") &
            assertEquals( PRRE_ANISO_1X, PRRETextureManager::getPRREanisoTexFilteringNameFromFloat(42), "42");
    }

    bool testGetPRREanisoTexFilteringNameFromUInt()
    {
        return assertEquals( PRRE_ANISO_1X, PRRETextureManager::getPRREanisoTexFilteringNameFromUInt(1), "1") &
            assertEquals( PRRE_ANISO_2X, PRRETextureManager::getPRREanisoTexFilteringNameFromUInt(2), "2") &
            assertEquals( PRRE_ANISO_4X, PRRETextureManager::getPRREanisoTexFilteringNameFromUInt(4), "4") &
            assertEquals( PRRE_ANISO_8X, PRRETextureManager::getPRREanisoTexFilteringNameFromUInt(8), "8") &
            assertEquals(PRRE_ANISO_16X, PRRETextureManager::getPRREanisoTexFilteringNameFromUInt(16), "16") &
            assertEquals( PRRE_ANISO_1X, PRRETextureManager::getPRREanisoTexFilteringNameFromUInt(0), "0") &
            assertEquals( PRRE_ANISO_1X, PRRETextureManager::getPRREanisoTexFilteringNameFromUInt(42), "42");
    }

    bool testGetFloatFromPRREanisoTexFilteringName()
    {
        return assertEquals( 1.0f, PRRETextureManager::getFloatFromPRREanisoTexFilteringName(PRRE_ANISO_1X), "1") &
            assertEquals( 2.0f, PRRETextureManager::getFloatFromPRREanisoTexFilteringName(PRRE_ANISO_2X), "2") &
            assertEquals( 4.0f, PRRETextureManager::getFloatFromPRREanisoTexFilteringName(PRRE_ANISO_4X), "4") &
            assertEquals( 8.0f, PRRETextureManager::getFloatFromPRREanisoTexFilteringName(PRRE_ANISO_8X), "8") &
            assertEquals(16.0f, PRRETextureManager::getFloatFromPRREanisoTexFilteringName(PRRE_ANISO_16X), "16");
    }

    bool testGetUIntFromPRREanisoTexFilteringName()
    {
        return assertEquals((TPRREuint) 1, PRRETextureManager::getUIntFromPRREanisoTexFilteringName(PRRE_ANISO_1X), "1") &
            assertEquals((TPRREuint) 2, PRRETextureManager::getUIntFromPRREanisoTexFilteringName(PRRE_ANISO_2X), "2") &
            assertEquals((TPRREuint) 4, PRRETextureManager::getUIntFromPRREanisoTexFilteringName(PRRE_ANISO_4X), "4") &
            assertEquals((TPRREuint) 8, PRRETextureManager::getUIntFromPRREanisoTexFilteringName(PRRE_ANISO_8X), "8") &
            assertEquals((TPRREuint)16, PRRETextureManager::getUIntFromPRREanisoTexFilteringName(PRRE_ANISO_16X), "16");
    }

    bool testCtor1()
    {
        PRREImage& img = *(im->createFromFile(BMP128x128x24));
        img.DetachFrom();
        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        tm = new PRRETextureManager();
        bool b = assertNull(tm->createFromFile(BMP128x128x24), "createTexture");
        b &= assertNull(tm->createTextureFromImage(img), "createImage");
        
        return b &
            assertEquals(PRRE_ANISO_1X, tm->getDefaultAnisoFilteringMode(), "Aniso") &
            assertEquals(PRRE_TC_NONE, tm->getDefaultCompressionMode(), "Compression") &
            assertEquals(PRRE_ISO_LINEAR, tm->getDefaultMinFilteringMode(), "MinFilter") &
            assertEquals(PRRE_ISO_LINEAR, tm->getDefaultMagFilteringMode(), "MagFilter") &
            assertFalse(tm->getDefaultBorder(), "Border") &
            assertTrue(tm->getDefaultBorderColor() == PRREColor(0,0,0,0), "BorderColor") &
            assertEquals(PRRE_ANISO_1X, tm->getMaximumAnisoFiltering(), "MaxAniso") &
            assertEquals(PRRE_TW_REPEAT, tm->getDefaultTextureWrappingModeS(), "Wrapping S") &
            assertEquals(PRRE_TW_REPEAT, tm->getDefaultTextureWrappingModeT(), "Wrapping T") &
            assertEquals((TPRREuint) 0, tm->getUsedTextureMemory(), "usedTexMem") &
            assertFalse(tm->isHardwareMipMapGenerationEnabled(), "HW MIP map gen") &
            assertFalse(tm->isNativeDIBFormatSupportEnabled(), "native DIB");
    }

    bool testCtor2()
    {
        // HW specific but should pass on main test machine
        return assertNotNull(tm) &
            assertEquals(PRRE_ANISO_1X, tm->getDefaultAnisoFilteringMode(), "Aniso") &
            assertEquals(PRRE_TC_NONE, tm->getDefaultCompressionMode(), "Compression") &
            assertEquals(PRRE_ISO_LINEAR, tm->getDefaultMinFilteringMode(), "MinFilter") &
            assertEquals(PRRE_ISO_LINEAR, tm->getDefaultMagFilteringMode(), "MagFilter") &
            assertFalse(tm->getDefaultBorder(), "Border") &
            assertTrue(tm->getDefaultBorderColor() == PRREColor(0,0,0,0), "BorderColor") &
            assertEquals(PRRE_ANISO_16X, tm->getMaximumAnisoFiltering(), "MaxAniso") &
            assertEquals(PRRE_TW_REPEAT, tm->getDefaultTextureWrappingModeS(), "Wrapping S") &
            assertEquals(PRRE_TW_REPEAT, tm->getDefaultTextureWrappingModeT(), "Wrapping T") &
            assertEquals((TPRREuint)0, tm->getUsedTextureMemory(), "usedTexMem") &
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
        const PRRETexture* const tex24 = tm->createFromFile(BMP128x128x24);
        const PRRETexture* const tex32 = tm->createFromFile(BMP128x128x32op);
        bool b = assertEquals(tex24->getUsedTextureMemory() + tex32->getUsedTextureMemory(), tm->getUsedTextureMemory(), "b1" );

        tm->DeleteAll();

        b &= assertEquals((TPRREuint)0, tm->getUsedTextureMemory(), "b2");

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
        const PRREImage& img = *(im->createFromFile(BMP128x128x24));
        const PRRETexture* const tex = tm->createTextureFromImage(img);

        const bool bKeep0 = assertNotEquals((const void*) PGENULL, (const void*) img.getPixels(), "bKeep0"); // the original img object has still the pixels even if the created texture doesn't
                                          
        TPRREint nTextures = tm->getCount();
        const PRRETexture* const tex2 = tm->createTextureFromImage(img);
        const bool b1 = assertNotEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertGreater(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(true);
        nTextures = tm->getCount();
        const PRRETexture* const tex3 = tm->createTextureFromImage(img);
        const bool b3 = assertEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertEquals(tm->getCount(), nTextures, "b4 count");
        tm->SetLazyInstancingEnabled(false);

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PRRETexture* const texNativeDIBoff = tm->createTextureFromImage(img);
        tm->SetNativeDIBFormatSupportEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PRRETexture* const texK1 = tm->createTextureFromImage(img);
        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        return assertNotNull(tex, "tex notnull") &
            b1 & b2 & b3 & b4 &
            assertNotNull(texNativeDIBoff, "nativeDIB") &
            bKeep0 & bKeep1 & bKeep2;
    }

    bool testCreateTextureFromFile()
    {
        const PRRETexture* const tex = tm->createFromFile(BMP128x128x24);

        TPRREint nTextures = tm->getCount();
        const PRRETexture* const tex2 = tm->createFromFile(BMP128x128x24);
        const bool b1 = assertNotEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertGreater(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(true);
        nTextures = tm->getCount();
        const PRRETexture* const tex3 = tm->createFromFile(BMP128x128x24);
        const bool b3 = assertEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertEquals(tm->getCount(), nTextures, "b4 count");
        tm->SetLazyInstancingEnabled(false);

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PRRETexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        tm->SetNativeDIBFormatSupportEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PRRETexture* const texK1 = tm->createFromFile(BMP128x128x24);
        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        return assertNotNull(tex, "tex notnull") &
            b1 & b2 & b3 & b4 &
            assertNotNull(texNativeDIBoff, "nativeDIB") &
            bKeep1 & bKeep2;
    }

    bool testCreateTextureFromFileMIP()
    {
        tm->SetDefaultIsoFilteringMode(PRRE_ISO_LINEAR_MIPMAP_LINEAR, PRRE_ISO_LINEAR);
        const PRRETexture* const tex = tm->createFromFile(BMP128x128x24);

        TPRREint nTextures = tm->getCount();
        const PRRETexture* const tex2 = tm->createFromFile(BMP128x128x24);
        const bool b1 = assertNotEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertGreater(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(true);
        nTextures = tm->getCount();
        const PRRETexture* const tex3 = tm->createFromFile(BMP128x128x24);
        const bool b3 = assertEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertEquals(tm->getCount(), nTextures, "b4 count");
        tm->SetLazyInstancingEnabled(false);

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PRRETexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        tm->SetNativeDIBFormatSupportEnabled(true);

        tm->SetHardwareMipMapGenerationEnabled(false);
        const PRRETexture* const texMipMapGenHWoff = tm->createFromFile(BMP128x128x24);
        tm->SetHardwareMipMapGenerationEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PRRETexture* const texK1 = tm->createFromFile(BMP128x128x24);
        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        return assertNotNull(tex, "tex notnull") &
            b1 & b2 & b3 & b4 &
            assertNotNull(texNativeDIBoff, "nativeDIB") &
            assertNotNull(texMipMapGenHWoff, "HWmipMapGenOff") &
            bKeep1 & bKeep2;
    }

    bool testCreateTextureFromFileComp()
    {
        tm->SetDefaultCompressionMode(PRRE_TC_S3TC_RGB_DXT1);
        const PRRETexture* const tex = tm->createFromFile(BMP128x128x24);

        TPRREint nTextures = tm->getCount();
        const PRRETexture* const tex2 = tm->createFromFile(BMP128x128x24);
        const bool b1 = assertNotEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertGreater(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(true);
        nTextures = tm->getCount();
        const PRRETexture* const tex3 = tm->createFromFile(BMP128x128x24);
        const bool b3 = assertEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertEquals(tm->getCount(), nTextures, "b4 count");
        tm->SetLazyInstancingEnabled(false);

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PRRETexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        tm->SetNativeDIBFormatSupportEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PRRETexture* const texK1 = tm->createFromFile(BMP128x128x24);
        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        tm->SetDefaultCompressionMode(PRRE_TC_S3TC_RGBA_DXT1); // check RGBA too
        const PRRETexture* const texComp2 = tm->createFromFile(BMP128x128x24);

        tm->SetDefaultCompressionMode(PRRE_TC_AUTO); // check auto too
        const PRRETexture* const texComp3 = tm->createFromFile(BMP128x128x24);

        return assertNotNull(tex, "tex notnull") &
            b1 & b2 & b3 & b4 &
            assertNotNull(texNativeDIBoff, "nativeDIB") &
            bKeep1 & bKeep2 &
            assertNotNull(texComp2) & assertNotNull(texComp3);
    }

    bool testCreateTextureFromFileCompMIP()
    {
        tm->SetDefaultIsoFilteringMode(PRRE_ISO_LINEAR_MIPMAP_LINEAR, PRRE_ISO_LINEAR);
        tm->SetDefaultCompressionMode(PRRE_TC_S3TC_RGB_DXT1);
        const PRRETexture* const tex = tm->createFromFile(BMP128x128x24);

        TPRREint nTextures = tm->getCount();
        const PRRETexture* const tex2 = tm->createFromFile(BMP128x128x24);
        const bool b1 = assertNotEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertGreater(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(true);
        nTextures = tm->getCount();
        const PRRETexture* const tex3 = tm->createFromFile(BMP128x128x24);
        const bool b3 = assertEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertEquals(tm->getCount(), nTextures, "b4 count");
        tm->SetLazyInstancingEnabled(false);

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PRRETexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        tm->SetNativeDIBFormatSupportEnabled(true);

        tm->SetHardwareMipMapGenerationEnabled(false);
        const PRRETexture* const texMipMapGenHWoff = tm->createFromFile(BMP128x128x24);
        tm->SetHardwareMipMapGenerationEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PRRETexture* const texK1 = tm->createFromFile(BMP128x128x24);
        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        tm->SetDefaultCompressionMode(PRRE_TC_S3TC_RGBA_DXT1); // check RGBA too
        const PRRETexture* const texComp2 = tm->createFromFile(BMP128x128x24);

        tm->SetDefaultCompressionMode(PRRE_TC_AUTO); // check auto too
        const PRRETexture* const texComp3 = tm->createFromFile(BMP128x128x24);

        return assertNotNull(tex, "tex notnull") &
            b1 & b2 & b3 & b4 &
            assertNotNull(texNativeDIBoff, "nativeDIB") &
            assertNotNull(texMipMapGenHWoff, "HWmipMapGenOff") &
            bKeep1 & bKeep2 &
            assertNotNull(texComp2) & assertNotNull(texComp3);
    }

    bool testCreateTextureFromFileBorder()
    {
        tm->SetDefaultBorder(true);
        PRREColor expColor(255,255,255,255);
        tm->getDefaultBorderColor() = expColor;

        const PRRETexture* const tex = tm->createFromFile(BMP128x128x24);

        TPRREint nTextures = tm->getCount();
        const PRRETexture* const tex2 = tm->createFromFile(BMP128x128x24);
        const bool b1 = assertNotEquals(tex, tex2, "tex == tex2");
        const bool b2 = assertGreater(tm->getCount(), nTextures, "b2 count");

        tm->SetLazyInstancingEnabled(true);
        nTextures = tm->getCount();
        const PRRETexture* const tex3 = tm->createFromFile(BMP128x128x24);
        const bool b3 = assertEquals(tex3, tex, "tex3 == tex");
        const bool b4 = assertEquals(tm->getCount(), nTextures, "b4 count");
        tm->SetLazyInstancingEnabled(false);

        tm->SetNativeDIBFormatSupportEnabled(false);
        const PRRETexture* const texNativeDIBoff = tm->createFromFile(BMP128x128x24);
        tm->SetNativeDIBFormatSupportEnabled(true);

        const bool bKeep1 = assertEquals((const void*) PGENULL, (const void*) tex->getPixels(), "bKeep1");
        tm->SetPixelPreservingEnabled(true);
        const PRRETexture* const texK1 = tm->createFromFile(BMP128x128x24);
        const bool bKeep2 = assertNotEquals((const void*) PGENULL, (const void*) texK1->getPixels(), "bKeep2");

        tm->SetDefaultCompressionMode(PRRE_TC_S3TC_RGBA_DXT1); // check RGBA too
        const PRRETexture* const texComp2 = tm->createFromFile(BMP128x128x24);

        tm->SetDefaultCompressionMode(PRRE_TC_AUTO); // check auto too
        const PRRETexture* const texComp3 = tm->createFromFile(BMP128x128x24);

        return assertNotNull(tex, "tex notnull") &
            b1 & b2 & b3 & b4 &
            assertNotNull(texNativeDIBoff, "nativeDIB") &
            bKeep1 & bKeep2 &
            assertNotNull(texComp2) & assertNotNull(texComp3) &
            assertTrue(tex->getBorderColor() == expColor, "tex clr") &
            assertTrue(tex2->getBorderColor() == expColor, "tex2 clr") &
            assertTrue(tex3->getBorderColor() == expColor, "tex3 clr") &
            assertTrue(texNativeDIBoff->getBorderColor() == expColor, "texNativeDIBoff clr") &
            assertTrue(texK1->getBorderColor() == expColor, "texK1 clr") &
            assertTrue(texComp2->getBorderColor() == expColor, "texComp2 clr") &
            assertTrue(texComp3->getBorderColor() == expColor, "texComp3 clr");
    }

    bool testSetDefaultMinFilteringMode()
    {
        tm->SetDefaultMinFilteringMode(PRRE_ISO_LINEAR_MIPMAP_LINEAR);
        return assertEquals(PRRE_ISO_LINEAR_MIPMAP_LINEAR, tm->getDefaultMinFilteringMode());
    }

    bool testSetDefaultMagFilteringMode()
    {
        tm->SetDefaultMagFilteringMode(PRRE_ISO_LINEAR_MIPMAP_LINEAR);
        const bool b1 = (tm->getDefaultMagFilteringMode() == PRRE_ISO_LINEAR);
        tm->SetDefaultMagFilteringMode(PRRE_ISO_LINEAR_MIPMAP_NEAREST);
        const bool b2 = (tm->getDefaultMagFilteringMode() == PRRE_ISO_LINEAR);
        tm->SetDefaultMagFilteringMode(PRRE_ISO_NEAREST);
        const bool b3 = (tm->getDefaultMagFilteringMode() == PRRE_ISO_NEAREST);
        return assertTrue(b1, "b1") & assertTrue(b2, "b2") & assertTrue(b3, "b3");
    }

    bool testSetDefaultIsoFilteringMode()
    {
        tm->SetDefaultIsoFilteringMode(PRRE_ISO_LINEAR, PRRE_ISO_LINEAR_MIPMAP_LINEAR);
        const bool b1 = tm->getDefaultMagFilteringMode() == PRRE_ISO_LINEAR;
        const bool b11 = tm->getDefaultMinFilteringMode() == PRRE_ISO_LINEAR;
        tm->SetDefaultIsoFilteringMode(PRRE_ISO_LINEAR, PRRE_ISO_LINEAR_MIPMAP_NEAREST);
        const bool b2 = tm->getDefaultMagFilteringMode() == PRRE_ISO_LINEAR;
        const bool b22 = tm->getDefaultMinFilteringMode() == PRRE_ISO_LINEAR;
        tm->SetDefaultIsoFilteringMode(PRRE_ISO_LINEAR, PRRE_ISO_NEAREST);
        const bool b3 = tm->getDefaultMagFilteringMode() == PRRE_ISO_NEAREST;
        const bool b33 = tm->getDefaultMinFilteringMode() == PRRE_ISO_LINEAR;
        return assertTrue(b1, "b1") & assertTrue(b2, "b2") & assertTrue(b3, "b3") &
            assertTrue(b11, "b11") & assertTrue(b22, "b22") & assertTrue(b33, "b33");
    }

    bool testSetDefaultAnisoFilteringMode()
    {
        tm->SetDefaultAnisoFilteringMode(PRRE_ANISO_16X);
        return assertEquals(PRRE_ANISO_16X, tm->getDefaultAnisoFilteringMode());
    }

    bool testSetDefaultWrappingMode()
    {
        tm->SetDefaultTextureWrappingMode(PRRE_TW_CLAMP, PRRE_TW_CLAMP_TO_EDGE);
        const bool b1 = assertEquals(PRRE_TW_CLAMP, tm->getDefaultTextureWrappingModeS(), "PRRE_TW_CLAMP");
        const bool b2 = assertEquals(PRRE_TW_CLAMP_TO_EDGE, tm->getDefaultTextureWrappingModeT(), "PRRE_TW_CLAMP_TO_EDGE");

        tm->SetDefaultTextureWrappingMode(PRRE_TW_CLAMP_TO_BORDER, PRRE_TW_MIRRORED_REPEAT);
        const bool b3 = assertEquals(PRRE_TW_CLAMP_TO_BORDER, tm->getDefaultTextureWrappingModeS(), "PRRE_TW_CLAMP_TO_BORDER");
        const bool b4 = assertEquals(PRRE_TW_MIRRORED_REPEAT, tm->getDefaultTextureWrappingModeT(), "PRRE_TW_MIRRORED_REPEAT");

        tm->SetDefaultTextureWrappingMode(PRRE_TW_REPEAT, PRRE_TW_REPEAT);
        const bool b5 = assertEquals(PRRE_TW_REPEAT, tm->getDefaultTextureWrappingModeS(), "PRRE_TW_REPEAT S");
        const bool b6 = assertEquals(PRRE_TW_REPEAT, tm->getDefaultTextureWrappingModeT(), "PRRE_TW_REPEAT T");

        return b1 & b2 & b3 & b4 & b5 & b6;
    }

    bool testSetDefaultCompressionMode()
    {
        tm->SetDefaultCompressionMode(PRRE_TC_S3TC_RGB_DXT1);
        return assertEquals(PRRE_TC_S3TC_RGB_DXT1, tm->getDefaultCompressionMode());
    }

    bool testSetDefaultBorder()
    {
        tm->SetDefaultBorder(true);
        return assertTrue( tm->getDefaultBorder() );
    }

    bool testSetDefaultBorderColor()
    {
        tm->getDefaultBorderColor().Set(1,1,1,1);

        return assertTrue( tm->getDefaultBorderColor() == PRREColor(1,1,1,1) );
    }

    bool testWriteList()
    {
        tm->WriteList();
        return true;
    }
    
}; // class PRRETextureManagerTest
