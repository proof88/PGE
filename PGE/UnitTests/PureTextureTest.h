#pragma once

/*
    ###################################################################################
    PureTextureTest.h
    Unit test for PureTexture.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
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

class PureTextureTest :
    public UnitTest
{
public:

    PureTextureTest() :
        UnitTest(__FILE__)
    {
        engine = NULL;
        tm = NULL;
        tex128x128x24 = NULL;
        tex128x128x32opaque = NULL;
        tex128x128x32transp = NULL;
        tex128x128x8 = NULL;
        tex128x128x4 = NULL;
        tex128x128x1 = NULL;
        tex128x128x24_mip = NULL;
        tex128x128x24_cmp = NULL;
        tex128x128x24_mip_cmp = NULL;
        tex128x128x24_border = NULL;
        
    }

    virtual ~PureTextureTest()
    {
        finalize();
    } // ~PureTextureTest()

protected:

    virtual void initialize() override
    {
        /*CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureImage::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureImageManager::getLoggerModuleName(), true);*/

        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        tm = &engine->getTextureManager();
        tm->SetLazyInstancingEnabled(false);
        tex128x128x24 = NULL;
        tex128x128x32opaque = tm->createFromFile(BMP128x128x32op);
        tex128x128x32transp = tm->createFromFile(BMP128x128x32tr);
        tex128x128x8 = tm->createFromFile(BMP128x128x8);
        tex128x128x4 = tm->createFromFile(BMP128x128x4);
        tex128x128x1 = tm->createFromFile(BMP128x128x1);
        tm->setDefaultBorder(true);
        tex128x128x24_border = tm->createFromFile(BMP128x128x24);
        tm->setDefaultBorder(false);
        tm->setDefaultMinFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR);
        tex128x128x24_mip = tm->createFromFile(BMP128x128x24);
        tm->setDefaultCompressionMode(PURE_TC_S3TC_RGB_DXT1);
        tex128x128x24_mip_cmp = tm->createFromFile(BMP128x128x24);

        tm->setDefaultMinFilteringMode(PURE_ISO_LINEAR);
        tex128x128x24_cmp = tm->createFromFile(BMP128x128x24);
        tm->setDefaultCompressionMode(PURE_TC_S3TC_RGBA_DXT1);
        tex128x128x24_cmpRGBA = tm->createFromFile(BMP128x128x24);
        tex128x128x32_cmpRGBA = tm->createFromFile(BMP128x128x32tr);
        tm->setDefaultCompressionMode(PURE_TC_AUTO);
        tex128x128x24_cmpAUTO = tm->createFromFile(BMP128x128x24);
        tex128x128x32_cmpAUTO = tm->createFromFile(BMP128x128x32tr);
        tm->setDefaultCompressionMode(PURE_TC_NONE);
        tm->SetPixelPreservingEnabled(true);

        addSubTest("testCtor", (PFNUNITSUBTEST) &PureTextureTest::testCtor);
        addSubTest("testDtor", (PFNUNITSUBTEST) &PureTextureTest::testDtor);
        addSubTest("testGetName", (PFNUNITSUBTEST) &PureTextureTest::testGetName);
        addSubTest("testMethodsFromImage", (PFNUNITSUBTEST) &PureTextureTest::testMethodsFromImage);
        addSubTest("testGetInternalFormat", (PFNUNITSUBTEST) &PureTextureTest::testGetInternalFormat);
        addSubTest("testGetInternalNum", (PFNUNITSUBTEST) &PureTextureTest::testGetInternalNum);
        addSubTest("testSetMinFilteringMode", (PFNUNITSUBTEST) &PureTextureTest::testSetMinFilteringMode);
        addSubTest("testSetMagFilteringMode", (PFNUNITSUBTEST) &PureTextureTest::testSetMagFilteringMode);
        addSubTest("testSetIsoFilteringMode", (PFNUNITSUBTEST) &PureTextureTest::testSetIsoFilteringMode);
        addSubTest("testGetMIPmapCount", (PFNUNITSUBTEST) &PureTextureTest::testGetMIPmapCount);
        addSubTest("testSetAnisoFilteringMode", (PFNUNITSUBTEST) &PureTextureTest::testSetAnisoFilteringMode);
        addSubTest("testGetTextureWrappingModeS", (PFNUNITSUBTEST) &PureTextureTest::testGetTextureWrappingModeS);
        addSubTest("testGetTextureWrappingModeT", (PFNUNITSUBTEST) &PureTextureTest::testGetTextureWrappingModeT);
        addSubTest("testSetTextureWrappingMode", (PFNUNITSUBTEST) &PureTextureTest::testSetTextureWrappingMode);
        addSubTest("testGetBorder", (PFNUNITSUBTEST) &PureTextureTest::testGetBorder);
        addSubTest("testSetBorder", (PFNUNITSUBTEST) &PureTextureTest::testSetBorder);
        addSubTest("testGetBorderColor", (PFNUNITSUBTEST) &PureTextureTest::testGetBorderColor);
        addSubTest("testSetBorderColor", (PFNUNITSUBTEST) &PureTextureTest::testSetBorderColor);
        addSubTest("testUploadPixels", (PFNUNITSUBTEST) &PureTextureTest::testUploadPixels);
        addSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PureTextureTest::testGetUsedSystemMemory);
        addSubTest("testGetUsedTextureMemory", (PFNUNITSUBTEST) &PureTextureTest::testGetUsedTextureMemory);
    }

    virtual bool setUp() override
    {
        tex128x128x24 = tm->createFromFile(BMP128x128x24);
        return assertNotNull(tex128x128x24, "BMP128x128x24 null") &
            assertNotNull(tex128x128x32opaque, "tex128x128x32opaque null") &
            assertNotNull(tex128x128x32transp, "tex128x128x32transp null") &
            assertNotNull(tex128x128x8, "tex128x128x8 null") &
            assertNotNull(tex128x128x4, "tex128x128x4 null") &
            assertNotNull(tex128x128x1, "tex128x128x1 null") &
            assertNotNull(tex128x128x24_border, "tex128x128x24_border null") &
            assertNotNull(tex128x128x24_mip, "tex128x128x24_mip null") &
            assertNotNull(tex128x128x24_cmp, "tex128x128x24_cmp null") &
            assertNotNull(tex128x128x24_mip_cmp, "tex128x128x24_mip_cmp null") &
            assertNotNull(tex128x128x24_cmpRGBA, "tex128x128x24_cmpRGBA null") &
            assertNotNull(tex128x128x24_cmpAUTO, "tex128x128x24_cmpAUTO null") &
            assertNotNull(tex128x128x32_cmpRGBA, "tex128x128x32_cmpRGBA null") &
            assertNotNull(tex128x128x32_cmpAUTO, "tex128x128x32_cmpAUTO null");
    }

    virtual void tearDown() override
    {
        if ( tex128x128x24 )
        {
            delete tex128x128x24;
            tex128x128x24 = NULL;
        }
    }

    virtual void finalize() override
    {
        tex128x128x32opaque = NULL;
        tex128x128x32transp = NULL;
        tex128x128x8 = NULL;
        tex128x128x4 = NULL;
        tex128x128x1 = NULL;
        tex128x128x24_mip = NULL;
        tex128x128x24_cmp = NULL;
        tex128x128x24_mip_cmp = NULL;
        tex128x128x24_cmpRGBA = NULL;
        tex128x128x24_cmpAUTO = NULL;
        tex128x128x32_cmpRGBA = NULL;
        tex128x128x32_cmpAUTO = NULL;
        tex128x128x24_border = NULL;
        tm = NULL;

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
    PureTextureManager* tm;
    PureTexture* tex128x128x24;
    PureTexture* tex128x128x32opaque,
               * tex128x128x32transp,
               * tex128x128x8,
               * tex128x128x4,
               * tex128x128x1,
               * tex128x128x24_mip,
               * tex128x128x24_cmp,
               * tex128x128x24_mip_cmp,
               * tex128x128x24_cmpRGBA,
               * tex128x128x24_cmpAUTO,
               * tex128x128x32_cmpRGBA,
               * tex128x128x32_cmpAUTO,
               * tex128x128x24_border;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PureTextureTest(const PureTextureTest&)
    {};         

    PureTextureTest& operator=(const PureTextureTest&)
    {
        return *this;
    };

    bool testCtor()
    {

        if (!assertNotNull(tex128x128x24, "24") ||
            !assertNotNull(tex128x128x32opaque, "32op") ||
            !assertNotNull(tex128x128x32transp, "32tr") ||
            !assertNotNull(tex128x128x8, "8") ||
            !assertNotNull(tex128x128x4, "4") ||
            !assertNotNull(tex128x128x1, "1") ||
            !assertNotNull(tex128x128x24_border, "border") ||
            !assertNotNull(tex128x128x24_mip, "24mip") ||
            !assertNotNull(tex128x128x24_cmp, "24cmp") ||
            !assertNotNull(tex128x128x24_mip_cmp, "24mipcmp") ||
            !assertNotNull(tex128x128x24_cmpRGBA, "24cmpRGBA") ||
            !assertNotNull(tex128x128x24_cmpAUTO, "24cmpAUTO") ||
            !assertNotNull(tex128x128x32_cmpRGBA, "32cmpRGBA") ||
            !assertNotNull(tex128x128x32_cmpAUTO, "32cmpAUTO") )
        {
            return false;
        }

        return assertEquals(tm->getDefaultAnisoFilteringMode(), tex128x128x24->getAnisoFilteringMode(), "aniso") &
            assertEquals(tm->getDefaultMinFilteringMode(), tex128x128x24->getMinFilteringMode(), "24 minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x24->getMagFilteringMode(), "24 magfiltering") &
            assertEquals(tm->getDefaultMinFilteringMode(), tex128x128x32opaque->getMinFilteringMode(), "32op minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x32opaque->getMagFilteringMode(), "32op magfiltering") &
            assertEquals(tm->getDefaultMinFilteringMode(), tex128x128x32transp->getMinFilteringMode(), "32tr minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x32transp->getMagFilteringMode(), "32tr magfiltering") &
            assertEquals(tm->getDefaultMinFilteringMode(), tex128x128x8->getMinFilteringMode(), "8 minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x8->getMagFilteringMode(), "8 magfiltering") &
            assertEquals(tm->getDefaultMinFilteringMode(), tex128x128x4->getMinFilteringMode(), "4 minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x4->getMagFilteringMode(), "4 magfiltering") &
            assertEquals(tm->getDefaultMinFilteringMode(), tex128x128x1->getMinFilteringMode(), "1 minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x1->getMagFilteringMode(), "1 magfiltering") &
            assertEquals(tm->getDefaultMinFilteringMode(), tex128x128x24_border->getMinFilteringMode(), "border minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x24_border->getMagFilteringMode(), "border magfiltering") &
            assertEquals(PURE_ISO_LINEAR_MIPMAP_LINEAR, tex128x128x24_mip->getMinFilteringMode(), "24mip minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x24_mip->getMagFilteringMode(), "24mip magfiltering") &
            assertEquals(tm->getDefaultMinFilteringMode(), tex128x128x24_cmp->getMinFilteringMode(), "24cmp minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x24_cmp->getMagFilteringMode(), "24cmp magfiltering") &
            assertEquals(PURE_ISO_LINEAR_MIPMAP_LINEAR, tex128x128x24_mip_cmp->getMinFilteringMode(), "24mipcmp minfiltering") &
            assertEquals(tm->getDefaultMagFilteringMode(), tex128x128x24_mip_cmp->getMagFilteringMode(), "24mipcmp magfiltering");
    }

    bool testDtor()
    {
        delete tex128x128x24;
        tex128x128x24 = NULL;
        return assertEquals(13, tm->getCount());
    }

     bool testGetName()
    {
        return assertNotEquals(std::string::npos, tex128x128x24->getName().find("Texture "), "tex128x128x24 name") &
            assertNotEquals(std::string::npos, tex128x128x32opaque->getName().find("Texture "), "tex128x128x32opaque name") &
            assertNotEquals(std::string::npos, tex128x128x32transp->getName().find("Texture "), "tex128x128x32transp name") &
            assertNotEquals(std::string::npos, tex128x128x8->getName().find("Texture "), "tex128x128x8 name") &
            assertNotEquals(std::string::npos, tex128x128x4->getName().find("Texture "), "tex128x128x4 name") &
            assertNotEquals(std::string::npos, tex128x128x1->getName().find("Texture "), "tex128x128x1 name") &
            assertNotEquals(std::string::npos, tex128x128x24_border->getName().find("Texture "), "tex128x128x24_border name") &
            assertNotEquals(std::string::npos, tex128x128x24_mip->getName().find("Texture "), "tex128x128x24_mip name") &
            assertNotEquals(std::string::npos, tex128x128x24_cmp->getName().find("Texture "), "tex128x128x24_cmp name") &
            assertNotEquals(std::string::npos, tex128x128x24_mip_cmp->getName().find("Texture "), "tex128x128x24_mip_cmp name") &
            assertNotEquals(std::string::npos, tex128x128x24_cmpRGBA->getName().find("Texture "), "tex128x128x24_cmpRGBA name") &
            assertNotEquals(std::string::npos, tex128x128x24_cmpAUTO->getName().find("Texture "), "tex128x128x24_cmpAUTO name") &
            assertNotEquals(std::string::npos, tex128x128x32_cmpRGBA->getName().find("Texture "), "tex128x128x32_cmpRGBA name") &
            assertNotEquals(std::string::npos, tex128x128x32_cmpAUTO->getName().find("Texture "), "tex128x128x32_cmpAUTO name");
    }

    bool testMethodsFromImage()
    {
        return assertEquals((TPureUInt)24, tex128x128x24->getBitsPerPixels(), "bitsperpels 24") &
            assertEquals((TPureUInt)32, tex128x128x32transp->getBitsPerPixels(), "bitsperpels 32tr") &
            assertEquals((TPureUInt)32, tex128x128x32opaque->getBitsPerPixels(), "bitsperpels 32op") &
            assertEquals((TPureUInt)24, tex128x128x8->getBitsPerPixels(), "bitsperpels 8") &
            assertEquals((TPureUInt)24, tex128x128x4->getBitsPerPixels(), "bitsperpels 4") &
            assertEquals((TPureUInt)24, tex128x128x1->getBitsPerPixels(), "bitsperpels 1") &
            assertEquals((TPureUInt)24, tex128x128x24_border->getBitsPerPixels(), "bitsperpels border") &
            assertEquals((TPureUInt)24, tex128x128x24_mip->getBitsPerPixels(), "bitsperpels 24mip") &
            assertEquals((TPureUInt)24, tex128x128x24_cmp->getBitsPerPixels(), "bitsperpels 24cmp") &
            assertEquals((TPureUInt)24, tex128x128x24_mip_cmp->getBitsPerPixels(), "bitsperpels 24mipcmp") &
            assertEquals(std::string(BMP128x128x24), tex128x128x24->getFilename(), "filename 24") &
            assertEquals(std::string(BMP128x128x32tr), tex128x128x32transp->getFilename(), "filename 32tr") &
            assertEquals(std::string(BMP128x128x32op), tex128x128x32opaque->getFilename(), "filename 32op") &
            assertEquals(std::string(BMP128x128x8), tex128x128x8->getFilename(), "filename 8") &
            assertEquals(std::string(BMP128x128x4), tex128x128x4->getFilename(), "filename 4") &
            assertEquals(std::string(BMP128x128x1), tex128x128x1->getFilename(), "filename 1") &
            assertEquals(std::string(BMP128x128x24), tex128x128x24_border->getFilename(), "filename border") &
            assertEquals(std::string(BMP128x128x24), tex128x128x24_mip->getFilename(), "filename 24mip") &
            assertEquals(std::string(BMP128x128x24), tex128x128x24_cmp->getFilename(), "filename 24cmp") &
            assertEquals(std::string(BMP128x128x24), tex128x128x24_mip_cmp->getFilename(), "filename 24mipcmp") &
            assertEquals((TPureUInt)128, tex128x128x24->getWidth(), "width 24") &
            assertEquals((TPureUInt)128, tex128x128x32transp->getWidth(), "width 32tr") &
            assertEquals((TPureUInt)128, tex128x128x32opaque->getWidth(), "width 32op") &
            assertEquals((TPureUInt)128, tex128x128x8->getWidth(), "width 8") &
            assertEquals((TPureUInt)128, tex128x128x4->getWidth(), "width 4") &
            assertEquals((TPureUInt)128, tex128x128x1->getWidth(), "width 1") &
            assertEquals((TPureUInt)128, tex128x128x24_border->getWidth(), "width border") &
            assertEquals((TPureUInt)128, tex128x128x24_mip->getWidth(), "width 24mip") &
            assertEquals((TPureUInt)128, tex128x128x24_cmp->getWidth(), "width 24cmp") &
            assertEquals((TPureUInt)128, tex128x128x24_mip_cmp->getWidth(), "width 24mipcmp") &
            assertEquals((TPureUInt)128, tex128x128x24->getHeight(), "height 24") &
            assertEquals((TPureUInt)128, tex128x128x32transp->getHeight(), "height 32tr") &
            assertEquals((TPureUInt)128, tex128x128x32opaque->getHeight(), "height 32op") &
            assertEquals((TPureUInt)128, tex128x128x8->getHeight(), "height 8") &
            assertEquals((TPureUInt)128, tex128x128x4->getHeight(), "height 4") &
            assertEquals((TPureUInt)128, tex128x128x1->getHeight(), "height 1") &
            assertEquals((TPureUInt)128, tex128x128x24_border->getHeight(), "height border") &
            assertEquals((TPureUInt)128, tex128x128x24_mip->getHeight(), "height 24mip") &
            assertEquals((TPureUInt)128, tex128x128x24_cmp->getHeight(), "height 24cmp") &
            assertEquals((TPureUInt)128, tex128x128x24_mip_cmp->getHeight(), "height 24mipcmp");
    }

    bool testGetInternalFormat()
    {
        // TF_UNSURE is just sometimes because in some cases with some drivers, driver just returns generic values
        return assertEquals(PURE_TF_RGBA, tex128x128x24->getInternalFormat(), "24") &
            assertEquals(PURE_TF_RGBA, tex128x128x32transp->getInternalFormat(), "32tr") &
            assertEquals(PURE_TF_RGBA, tex128x128x32opaque->getInternalFormat(), "32op") &
            assertEquals(PURE_TF_RGBA, tex128x128x8->getInternalFormat(), "8") &
            assertEquals(PURE_TF_RGBA, tex128x128x4->getInternalFormat(), "4") &
            assertEquals(PURE_TF_RGBA, tex128x128x1->getInternalFormat(), "1") &
            assertEquals(PURE_TF_RGBA, tex128x128x24_border->getInternalFormat(), "border") &
            assertEquals(PURE_TF_RGBA, tex128x128x24_mip->getInternalFormat(), "24mip") &
            assertEquals(PURE_TF_DXT1, tex128x128x24_cmp->getInternalFormat(), "24cmp") &
            assertEquals(PURE_TF_DXT1, tex128x128x24_mip_cmp->getInternalFormat(), "24mipcmp") &
            assertEquals(PURE_TF_DXT1A, tex128x128x24_cmpRGBA->getInternalFormat(), "24cmpRGBA") &
            assertTrue((PURE_TF_DXT1 == tex128x128x24_cmpAUTO->getInternalFormat()) ||
                (PURE_TF_FXT1 == tex128x128x24_cmpAUTO->getInternalFormat()) ||
                (PURE_TF_UNSURE == tex128x128x24_cmpAUTO->getInternalFormat()), "24cmpAUTO") &
            assertEquals(PURE_TF_DXT1A, tex128x128x32_cmpRGBA->getInternalFormat(), "32cmpRGBA") &
            assertTrue((PURE_TF_DXT1A == tex128x128x32_cmpAUTO->getInternalFormat()) ||
                (PURE_TF_DXT3A == tex128x128x32_cmpAUTO->getInternalFormat()) ||
                (PURE_TF_DXT5A == tex128x128x32_cmpAUTO->getInternalFormat()) ||
                (PURE_TF_FXT1A == tex128x128x32_cmpAUTO->getInternalFormat()) ||
                (PURE_TF_UNSURE == tex128x128x32_cmpAUTO->getInternalFormat()), "32cmpAUTO");
    }

    bool testGetInternalNum()
    {
        return assertGreater(tex128x128x24->getInternalNum(),      (unsigned int) 0, "24") &
            assertGreater(tex128x128x32transp->getInternalNum(),   (unsigned int) 0, "32tr") &
            assertGreater(tex128x128x32opaque->getInternalNum(),   (unsigned int) 0, "32op") &
            assertGreater(tex128x128x8->getInternalNum(),          (unsigned int) 0, "8") &
            assertGreater(tex128x128x4->getInternalNum(),          (unsigned int) 0, "4") &
            assertGreater(tex128x128x1->getInternalNum(),          (unsigned int) 0, "1") &
            assertGreater(tex128x128x24_border->getInternalNum(),  (unsigned int) 0, "border") &
            assertGreater(tex128x128x24_mip->getInternalNum(),     (unsigned int) 0, "24mip") &
            assertGreater(tex128x128x24_cmp->getInternalNum(),     (unsigned int) 0, "24cmp") &
            assertGreater(tex128x128x24_mip_cmp->getInternalNum(), (unsigned int) 0, "24mipcmp") &
            assertGreater(tex128x128x24_cmpRGBA->getInternalNum(), (unsigned int) 0, "24cmpRGBA") &
            assertGreater(tex128x128x24_cmpAUTO->getInternalNum(), (unsigned int) 0, "24cmpAUTO") &
            assertGreater(tex128x128x32_cmpRGBA->getInternalNum(), (unsigned int) 0, "32cmpRGBA") &
            assertGreater(tex128x128x32_cmpAUTO->getInternalNum(), (unsigned int) 0, "32cmpAUTO");
    }

    bool testSetMinFilteringMode()
    {
        assertTrue(tex128x128x24->setMinFilteringMode(PURE_ISO_NEAREST), "set 1");
        bool b = assertEquals(PURE_ISO_NEAREST, tex128x128x24->getMinFilteringMode(), "b1");
        assertTrue(tex128x128x24->setMinFilteringMode(PURE_ISO_LINEAR), "set 2");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMinFilteringMode(), "b2");
        assertFalse(tex128x128x24->setMinFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR), "set 3");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMinFilteringMode(), "b3");
        assertFalse(tex128x128x24->setMinFilteringMode(PURE_ISO_LINEAR_MIPMAP_NEAREST), "set 4");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMinFilteringMode(), "b4");
        assertFalse(tex128x128x24->setMinFilteringMode(PURE_ISO_NEAREST_MIPMAP_LINEAR), "set 5");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMinFilteringMode(), "b5");
        assertFalse(tex128x128x24->setMinFilteringMode(PURE_ISO_NEAREST_MIPMAP_NEAREST), "set 6");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMinFilteringMode(), "b6");

        assertTrue(tex128x128x24_mip->setMinFilteringMode(PURE_ISO_NEAREST), "set 7");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24_mip->getMinFilteringMode(), "bA");
        assertTrue(tex128x128x24_mip->setMinFilteringMode(PURE_ISO_LINEAR), "set 8");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24_mip->getMinFilteringMode(), "bB");
        assertTrue(tex128x128x24_mip->setMinFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR), "set 9");
        b &= assertEquals(PURE_ISO_LINEAR_MIPMAP_LINEAR, tex128x128x24_mip->getMinFilteringMode(), "bC");
        assertTrue(tex128x128x24_mip->setMinFilteringMode(PURE_ISO_LINEAR_MIPMAP_NEAREST), "set 10");
        b &= assertEquals(PURE_ISO_LINEAR_MIPMAP_NEAREST, tex128x128x24_mip->getMinFilteringMode(), "bD");
        assertTrue(tex128x128x24_mip->setMinFilteringMode(PURE_ISO_NEAREST_MIPMAP_LINEAR), "set 11");
        b &= assertEquals(PURE_ISO_NEAREST_MIPMAP_LINEAR, tex128x128x24_mip->getMinFilteringMode(), "bE");
        assertTrue(tex128x128x24_mip->setMinFilteringMode(PURE_ISO_NEAREST_MIPMAP_NEAREST), "set 12");
        b &= assertEquals(PURE_ISO_NEAREST_MIPMAP_NEAREST, tex128x128x24_mip->getMinFilteringMode(), "bF");

        return b;
    }

    bool testSetMagFilteringMode()
    {
        bool b = assertTrue(tex128x128x24->setMagFilteringMode(PURE_ISO_NEAREST), "set 1");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24->getMagFilteringMode(), "b1");
        b &= assertTrue(tex128x128x24->setMagFilteringMode(PURE_ISO_LINEAR), "set 2");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMagFilteringMode(), "b2");
        b &= assertFalse(tex128x128x24->setMagFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR), "set 3");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMagFilteringMode(), "b3");
        b &= assertFalse(tex128x128x24->setMagFilteringMode(PURE_ISO_LINEAR_MIPMAP_NEAREST), "set 4");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMagFilteringMode(), "b4");
        b &= assertFalse(tex128x128x24->setMagFilteringMode(PURE_ISO_NEAREST_MIPMAP_LINEAR), "set 5");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMagFilteringMode(), "b5");
        b &= assertFalse(tex128x128x24->setMagFilteringMode(PURE_ISO_NEAREST_MIPMAP_NEAREST), "set 6");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMagFilteringMode(), "b6");

        return b;
    }

    bool testSetIsoFilteringMode()
    {
        // this should pass if SetMin...() and SetMag...() tests passed
        // testing not fully, only a little bit to make sure it is really okay ...
        bool b = assertTrue(tex128x128x24->setIsoFilteringMode(PURE_ISO_NEAREST, PURE_ISO_NEAREST), "set 1");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24->getMinFilteringMode(), "b1");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24->getMagFilteringMode(), "b2");
        b &= assertFalse(tex128x128x24->setIsoFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR, PURE_ISO_LINEAR), "set 2");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24->getMinFilteringMode(), "b3");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24->getMagFilteringMode(), "b4");
        b &= assertFalse(tex128x128x24->setIsoFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR, PURE_ISO_LINEAR_MIPMAP_LINEAR), "set 3");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24->getMinFilteringMode(), "b5");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24->getMagFilteringMode(), "b6");
        b &= assertTrue(tex128x128x24->setIsoFilteringMode(PURE_ISO_NEAREST, PURE_ISO_LINEAR), "set 4");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24->getMinFilteringMode(), "b7");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24->getMagFilteringMode(), "b8");

        b &= assertTrue(tex128x128x24_mip->setIsoFilteringMode(PURE_ISO_NEAREST, PURE_ISO_NEAREST), "set 5");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24_mip->getMinFilteringMode(), "b9");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24_mip->getMagFilteringMode(), "b10");
        b &= assertTrue(tex128x128x24_mip->setIsoFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR, PURE_ISO_LINEAR), "set 6");
        b &= assertEquals(PURE_ISO_LINEAR_MIPMAP_LINEAR, tex128x128x24_mip->getMinFilteringMode(), "b11");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24_mip->getMagFilteringMode(), "b12");
        b &= assertFalse(tex128x128x24_mip->setIsoFilteringMode(PURE_ISO_LINEAR_MIPMAP_LINEAR, PURE_ISO_LINEAR_MIPMAP_LINEAR), "set 7");
        b &= assertEquals(PURE_ISO_LINEAR_MIPMAP_LINEAR, tex128x128x24_mip->getMinFilteringMode(), "b13");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24_mip->getMagFilteringMode(), "b14");
        b &= assertTrue(tex128x128x24_mip->setIsoFilteringMode(PURE_ISO_NEAREST, PURE_ISO_LINEAR), "set 8");
        b &= assertEquals(PURE_ISO_NEAREST, tex128x128x24_mip->getMinFilteringMode(), "b15");
        b &= assertEquals(PURE_ISO_LINEAR, tex128x128x24_mip->getMagFilteringMode(), "b16");
        
        return b;
    }

    bool testGetMIPmapCount()
    {
        return assertEquals((TPureUInt)1, tex128x128x24->getMIPmapCount(), "24") &
            assertEquals((TPureUInt)1, tex128x128x32transp->getMIPmapCount(), "32tr") &
            assertEquals((TPureUInt)1, tex128x128x32opaque->getMIPmapCount(), "32op") &
            assertEquals((TPureUInt)1, tex128x128x8->getMIPmapCount(), "8") &
            assertEquals((TPureUInt)1, tex128x128x4->getMIPmapCount(), "4") &
            assertEquals((TPureUInt)1, tex128x128x1->getMIPmapCount(), "1") &
            assertEquals((TPureUInt)1, tex128x128x24_border->getMIPmapCount(), "border") &
            assertEquals(PureTextureManager::getMIPmapCount(128, 128), tex128x128x24_mip->getMIPmapCount(), "24mip") &
            assertEquals((TPureUInt)1, tex128x128x24_cmp->getMIPmapCount(), "24cmp") &
            assertEquals(PureTextureManager::getMIPmapCount(128, 128), tex128x128x24_mip_cmp->getMIPmapCount(), "24mipcmp");
    }

    bool testSetAnisoFilteringMode()
    {
        // these should be okay on main test machine
        bool b = assertTrue(tex128x128x24->setAnisoFilteringMode(PURE_ANISO_2X), "set 1");
        b &= assertEquals(PURE_ANISO_2X, tex128x128x24->getAnisoFilteringMode(), "b1");
        b &= assertTrue(tex128x128x24->setAnisoFilteringMode(PURE_ANISO_4X), "set 2");
        b &= assertEquals(PURE_ANISO_4X, tex128x128x24->getAnisoFilteringMode(), "b2");
        b &= assertTrue(tex128x128x24->setAnisoFilteringMode(PURE_ANISO_8X), "set 3");
        b &= assertEquals(PURE_ANISO_8X, tex128x128x24->getAnisoFilteringMode(), "b3");
        b &= assertTrue(tex128x128x24->setAnisoFilteringMode(PURE_ANISO_16X), "set 4");
        b &= assertEquals(PURE_ANISO_16X, tex128x128x24->getAnisoFilteringMode(), "b4");
        b &= assertTrue(tex128x128x24->setAnisoFilteringMode(PURE_ANISO_1X), "set 5");
        b &= assertEquals(PURE_ANISO_1X, tex128x128x24->getAnisoFilteringMode(), "b5");
        b &= assertTrue(tex128x128x24->setAnisoFilteringMode(PURE_ANISO_MAX), "set 6");
        b &= assertEquals(PURE_ANISO_MAX, tex128x128x24->getAnisoFilteringMode(), "b6");
        
        return b;
    }

    bool testGetTextureWrappingModeS()
    {
        return assertEquals(tm->getDefaultTextureWrappingModeS(), tex128x128x24->getTextureWrappingModeS(), "24") &
            assertEquals(tm->getDefaultTextureWrappingModeS(), tex128x128x32transp->getTextureWrappingModeS(), "32tr") &
            assertEquals(tm->getDefaultTextureWrappingModeS(), tex128x128x32opaque->getTextureWrappingModeS(), "32op") &
            assertEquals(tm->getDefaultTextureWrappingModeS(), tex128x128x8->getTextureWrappingModeS(), "8") &
            assertEquals(tm->getDefaultTextureWrappingModeS(), tex128x128x4->getTextureWrappingModeS(), "4") &
            assertEquals(tm->getDefaultTextureWrappingModeS(), tex128x128x1->getTextureWrappingModeS(), "1") &
            assertEquals(tm->getDefaultTextureWrappingModeS(), tex128x128x24_border->getTextureWrappingModeS(), "border");
    }

    bool testGetTextureWrappingModeT()
    {
        return assertEquals(tm->getDefaultTextureWrappingModeT(), tex128x128x24->getTextureWrappingModeT(), "24") &
            assertEquals(tm->getDefaultTextureWrappingModeT(), tex128x128x32transp->getTextureWrappingModeT(), "32tr") &
            assertEquals(tm->getDefaultTextureWrappingModeT(), tex128x128x32opaque->getTextureWrappingModeT(), "32op") &
            assertEquals(tm->getDefaultTextureWrappingModeT(), tex128x128x8->getTextureWrappingModeT(), "8") &
            assertEquals(tm->getDefaultTextureWrappingModeT(), tex128x128x4->getTextureWrappingModeT(), "4") &
            assertEquals(tm->getDefaultTextureWrappingModeT(), tex128x128x1->getTextureWrappingModeT(), "1") &
            assertEquals(tm->getDefaultTextureWrappingModeT(), tex128x128x24_border->getTextureWrappingModeT(), "border");
    }

    bool testSetTextureWrappingMode()
    {
        bool b = assertTrue(tex128x128x24->setTextureWrappingMode(PURE_TW_CLAMP, PURE_TW_CLAMP_TO_EDGE), "1");
        const bool b1 = assertEquals(PURE_TW_CLAMP, tex128x128x24->getTextureWrappingModeS(), "PURE_TW_CLAMP");
        const bool b2 = assertEquals(PURE_TW_CLAMP_TO_EDGE, tex128x128x24->getTextureWrappingModeT(), "PURE_TW_CLAMP_TO_EDGE");

        b &= assertTrue(tex128x128x24->setTextureWrappingMode(PURE_TW_CLAMP_TO_BORDER, PURE_TW_MIRRORED_REPEAT), "2");
        const bool b3 = assertEquals(PURE_TW_CLAMP_TO_BORDER, tex128x128x24->getTextureWrappingModeS(), "PURE_TW_CLAMP_TO_BORDER");
        const bool b4 = assertEquals(PURE_TW_MIRRORED_REPEAT, tex128x128x24->getTextureWrappingModeT(), "PURE_TW_MIRRORED_REPEAT");

        b &= assertTrue(tex128x128x24->setTextureWrappingMode(PURE_TW_REPEAT, PURE_TW_REPEAT), "3");
        const bool b5 = assertEquals(PURE_TW_REPEAT, tex128x128x24->getTextureWrappingModeS(), "PURE_TW_REPEAT S");
        const bool b6 = assertEquals(PURE_TW_REPEAT, tex128x128x24->getTextureWrappingModeT(), "PURE_TW_REPEAT T");

        return b & b1 & b2 & b3 & b4 & b5 & b6;
    }

    bool testGetBorder()
    {
        return assertFalse(tex128x128x24->getBorder(), "24") &
            assertFalse(tex128x128x32transp->getBorder(), "32tr") &
            assertFalse(tex128x128x32opaque->getBorder(), "32op") &
            assertFalse(tex128x128x8->getBorder(), "8") &
            assertFalse(tex128x128x4->getBorder(), "4") &
            assertFalse(tex128x128x1->getBorder(), "1") &
            assertTrue(tex128x128x24_border->getBorder(), "border") &
            assertFalse(tex128x128x24_mip->getBorder(), "24mip") &
            assertFalse(tex128x128x24_cmp->getBorder(), "24cmp") &
            assertFalse(tex128x128x24_mip_cmp->getBorder(), "24mipcmp") &
            assertFalse(tex128x128x24_cmpRGBA->getBorder(), "24cmpRGBA") &
            assertFalse(tex128x128x24_cmpAUTO->getBorder(), "24cmpAUTO") &
            assertFalse(tex128x128x32_cmpRGBA->getBorder(), "32cmpRGBA") &
            assertFalse(tex128x128x32_cmpAUTO->getBorder(), "32cmpAUTO");
    }

    bool testSetBorder()
    {
        bool b = assertTrue(tex128x128x24->setBorder(true), "setBorder");
        tex128x128x24->uploadPixels();

        return b & assertTrue(tex128x128x24->getBorder());
    }

    bool testGetBorderColor()
    {
        return assertTrue(tex128x128x24_border->getBorderColor() == PureColor(0,0,0,0), "border") &
            assertTrue(tex128x128x24->getBorderColor() == PureColor(0,0,0,0), "24");
    }

    bool testSetBorderColor()
    {
        bool b1 = assertTrue(tex128x128x24_border->setBorderColor(PureColor(255,255,255,255)), "b1");
        bool b2 = assertTrue(tex128x128x24->setBorderColor(PureColor(255,255,255,255)), "b2");

        return b1 & b2 &
            assertTrue(tex128x128x24_border->getBorderColor() == PureColor(255,255,255,255), "border") &
            assertTrue(tex128x128x24->getBorderColor() == PureColor(255,255,255,255), "24");
    }

    bool testUploadPixels()
    {
        bool b = assertTrue(tex128x128x24->setPixel(5, 10, 255, 255, 255, 255), "setPixel");
        b &= assertTrue(tex128x128x24->uploadPixels(), "24 start");

        tex128x128x24->DetachFrom();
        b &= assertFalse(tex128x128x24->uploadPixels(), "no manager");

        engine->getImageManager().Attach(*tex128x128x24);
        b &= assertFalse(tex128x128x24->uploadPixels(), "mgr is not TexManager");

        tex128x128x24->DetachFrom();
        engine->getTextureManager().Attach(*tex128x128x24);
        tex128x128x24->FlushResources();
        b &= assertFalse(tex128x128x24->uploadPixels(), "no pixels available");

        return b;
    }

    bool testGetUsedSystemMemory()
    {
        const TPureUInt before = tex128x128x24->getUsedSystemMemory();
        tex128x128x24->FlushResources();
        const bool b1 = assertGreater(before, tex128x128x24->getUsedSystemMemory(), "b1" );

        return b1 &
            assertGreater(tex128x128x24->getUsedSystemMemory(),       sizeof(PureTexture), "24") &
            assertGreater(tex128x128x32transp->getUsedSystemMemory(), sizeof(PureTexture), "32tr") &
            assertGreater(tex128x128x32opaque->getUsedSystemMemory(), sizeof(PureTexture), "32op") &
            assertGreater(tex128x128x8->getUsedSystemMemory(),        sizeof(PureTexture), "8") &
            assertGreater(tex128x128x4->getUsedSystemMemory(),        sizeof(PureTexture), "4") &
            assertGreater(tex128x128x1->getUsedSystemMemory(),        sizeof(PureTexture), "1") &
            assertGreater(tex128x128x24_border->getUsedSystemMemory(),  sizeof(PureTexture), "border") &
            assertGreater(tex128x128x24_mip->getUsedSystemMemory(),     sizeof(PureTexture), "24mip") &
            assertGreater(tex128x128x24_cmp->getUsedSystemMemory(),     sizeof(PureTexture), "24cmp") &
            assertGreater(tex128x128x24_mip_cmp->getUsedSystemMemory(), sizeof(PureTexture), "24mipcmp");
    }

    bool testGetUsedTextureMemory()
    {
        // MIP mapping uses ~+33% memory
        return assertEquals((TPureUInt)128*128*4, tex128x128x24->getUsedTextureMemory(), "24") &
            assertEquals((TPureUInt)128*128*4,    tex128x128x32transp->getUsedTextureMemory(), "32tr") &
            assertEquals((TPureUInt)128*128*4,    tex128x128x32opaque->getUsedTextureMemory(), "32op") &
            assertEquals((TPureUInt)128*128*4,    tex128x128x8->getUsedTextureMemory(), "8") &
            assertEquals((TPureUInt)128*128*4,    tex128x128x4->getUsedTextureMemory(), "4") &
            assertEquals((TPureUInt)128*128*4,    tex128x128x1->getUsedTextureMemory(), "1") &
            assertEquals((TPureUInt)128*128*4,    tex128x128x24_border->getUsedTextureMemory(), "border") &
            assertBetween((unsigned int) (128*128*4*1.32f), (unsigned int) (128*128*4*1.35f), tex128x128x24_mip->getUsedTextureMemory(), "24mip") &
            assertEquals(128*128*4/8.0f, (float) tex128x128x24_cmp->getUsedTextureMemory(), E, "24cmp") &
            assertBetween((unsigned int) (128*128*4*1.32f/8.0f), (unsigned int) (128*128*4*1.35f/8.0f), tex128x128x24_mip_cmp->getUsedTextureMemory(), "24mipcmp") &
            assertEquals(128*128*4/8.0f, tex128x128x24_cmpRGBA->getUsedTextureMemory(), "24cmpRGBA") &
            assertEquals(128*128*4/8.0f, tex128x128x24_cmpAUTO->getUsedTextureMemory(), "24cmpAUTO") &
            assertEquals(128*128*4/8.0f, tex128x128x32_cmpRGBA->getUsedTextureMemory(), "32cmpRGBA") &
            assertTrue(
                128*128*4/4.0f == tex128x128x32_cmpAUTO->getUsedTextureMemory() ||
                128*128*4/8.0f == tex128x128x32_cmpAUTO->getUsedTextureMemory(), "32cmpAUTO");
    }

}; // class PureTextureTest