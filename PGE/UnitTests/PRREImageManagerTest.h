#pragma once

/*
    ###################################################################################
    PRREImageManagerTest.h
    Unit test for PRREImageManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGE/PRRE/PRREImageManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREImageManagerTest :
    public UnitTest
{
public:

    PRREImageManagerTest() :
        UnitTest( __FILE__ ) {};

protected:

    virtual void Initialize()
    {
        AddSubTest("testGetColorConversionSwapCount", (PFNUNITSUBTEST) &PRREImageManagerTest::testGetColorConversionSwapCount);
        AddSubTest("testGetMirroredPixelComponentOrder", (PFNUNITSUBTEST) &PRREImageManagerTest::testGetMirroredPixelComponentOrder);
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREImageManagerTest::testCtor1);
        AddSubTest("testCreateImageFromFileNull", (PFNUNITSUBTEST) &PRREImageManagerTest::testCreateImageFromFileNull);
        AddSubTest("testCreateImageFromFileNotExists", (PFNUNITSUBTEST) &PRREImageManagerTest::testCreateImageFromFileNotExists);
        AddSubTest("testCreateImageFromFileInvalidExtension", (PFNUNITSUBTEST) &PRREImageManagerTest::testCreateImageFromFileInvalidExtension);
        AddSubTest("testCreateImageFromFileInvalidFormat", (PFNUNITSUBTEST) &PRREImageManagerTest::testCreateImageFromFileInvalidFormat);
        AddSubTest("testCreateImageFromFileValid", (PFNUNITSUBTEST) &PRREImageManagerTest::testCreateImageFromFileValid);
        AddSubTest("testCreateBlankImageValid", (PFNUNITSUBTEST) &PRREImageManagerTest::testCreateBlankImageValid);
        AddSubTest("testCreateBlankImageInvalid", (PFNUNITSUBTEST) &PRREImageManagerTest::testCreateBlankImageInvalid);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREImageManagerTest::testWriteList);
    }

    virtual void SetUp()
    {}

    virtual void TearDown()
    {}


private:

    PRREImageManagerTest(const PRREImageManagerTest&)
    {};         

    PRREImageManagerTest& operator=(const PRREImageManagerTest&)
    {
        return *this;
    };

    bool testGetColorConversionSwapCount()
    {
        bool b = assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_RGB,  PRRE_RGB),  "RGB-RGB") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_RGBA, PRRE_RGBA), "RGBA-RGBA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_RBG,  PRRE_RBG),  "RBG-RBG") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_RBGA, PRRE_RBGA), "RBGA-RBGA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_BRG,  PRRE_BRG),  "BRG-BRG") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_BRGA, PRRE_BRGA), "BRGA-BRGA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_BGR,  PRRE_BGR),  "BGR-BGR") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_BGRA, PRRE_BGRA), "BGRA-BGRA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_GRB,  PRRE_GRB),  "GRB-GRB") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_GRBA, PRRE_GRBA), "GRBA-GRBA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_GBR,  PRRE_GBR),  "GBR-GBR") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_GBRA, PRRE_GBRA), "GBRA-GBRA");

        b &= assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_RGBA), "RGB-RGBA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_RBGA), "RBG-RBGA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_BRGA), "BRG-BRGA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_BGRA), "BGR-BGRA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_GRBA), "GRB-GRBA") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_GBRA), "GBR-GBRA");

        b &= assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_RGBA, PRRE_RGB), "RGBA-RGB") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_RBGA, PRRE_RBG), "RBGA-RBG") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_BRGA, PRRE_BRG), "BRGA-BRG") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_BGRA, PRRE_BGR), "BGRA-BGR") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_GRBA, PRRE_GRB), "GRBA-GRB") &
            assertEquals((TPRREuint) 0, PRREImageManager::getColorConversionSwapCount(PRRE_GBRA, PRRE_GBR), "GBRA-GBR");

        b &= assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_RBG),  "RGB-RBG") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_RBGA), "RGB-RBGA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_BRG),  "RGB-BRG") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_BRGA), "RGB-BRGA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_BGR),  "RGB-BGR") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_BGRA), "RGB-BGRA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_GRB),  "RGB-GRB") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_GRBA), "RGB-GRBA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_GBR),  "RGB-GBR") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_RGB, PRRE_GBRA), "RGB-GBRA");

        b &= assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_RGB),  "RBG-RGB") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_RGBA), "RBG-RGBA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_BRG),  "RBG-BRG") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_BRGA), "RBG-BRGA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_BGR),  "RBG-BGR") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_BGRA), "RBG-BGRA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_GRB),  "RBG-GRB") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_GRBA), "RBG-GRBA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_GBR),  "RBG-GBR") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_RBG, PRRE_GBRA), "RBG-GBRA");

        b &= assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_RGB),  "BRG-RGB") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_RGBA), "BRG-RGBA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_RBG),  "BRG-RBG") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_RBGA), "BRG-RBGA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_BGR),  "BRG-BGR") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_BGRA), "BRG-BGRA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_GRB),  "BRG-GRB") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_GRBA), "BRG-GRBA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_GBR),  "BRG-GBR") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_BRG, PRRE_GBRA), "BRG-GBRA");

        b &= assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_RGB),  "BGR-RGB") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_RGBA), "BGR-RGBA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_RBG),  "BGR-RBG") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_RBGA), "BGR-RBGA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_BRG),  "BGR-BRG") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_BRGA), "BGR-BRGA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_GRB),  "BGR-GRB") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_GRBA), "BGR-GRBA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_GBR),  "BGR-GBR") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_BGR, PRRE_GBRA), "BGR-GBRA");

        b &= assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_RGB),  "GRB-RGB") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_RGBA), "GRB-RGBA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_RBG),  "GRB-RBG") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_RBGA), "GRB-RBGA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_BRG),  "GRB-BRG") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_BRGA), "GRB-BRGA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_BGR),  "GRB-BGR") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_BGRA), "GRB-BGRA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_GBR),  "GRB-GBR") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GRB, PRRE_GBRA), "GRB-GBRA");

        b &= assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_RGB),  "GBR-RGB") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_RGBA), "GBR-RGBA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_RBG),  "GBR-RBG") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_RBGA), "GBR-RBGA") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_BRG),  "GBR-BRG") &
            assertEquals((TPRREuint) 2, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_BRGA), "GBR-BRGA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_BGR),  "GBR-BGR") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_BGRA), "GBR-BGRA") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_GRB),  "GBR-GRB") &
            assertEquals((TPRREuint) 1, PRREImageManager::getColorConversionSwapCount(PRRE_GBR, PRRE_GRBA), "GBR-GRBA");

        return b;
    }

    bool testGetMirroredPixelComponentOrder()
    {
        return assertEquals(PRRE_BGR,  PRREImageManager::getMirroredPixelComponentOrder(PRRE_RGB),  "RGB") &
            assertEquals(PRRE_BGRA, PRREImageManager::getMirroredPixelComponentOrder(PRRE_RGBA), "RGBA") & 
            assertEquals(PRRE_GBR,  PRREImageManager::getMirroredPixelComponentOrder(PRRE_RBG),  "RBG") &
            assertEquals(PRRE_GBRA, PRREImageManager::getMirroredPixelComponentOrder(PRRE_RBGA), "RBGA") &
            assertEquals(PRRE_GRB,  PRREImageManager::getMirroredPixelComponentOrder(PRRE_BRG),  "BRG") &
            assertEquals(PRRE_GRBA, PRREImageManager::getMirroredPixelComponentOrder(PRRE_BRGA), "BRGA") &
            assertEquals(PRRE_RGB,  PRREImageManager::getMirroredPixelComponentOrder(PRRE_BGR),  "BGR") &
            assertEquals(PRRE_RGBA, PRREImageManager::getMirroredPixelComponentOrder(PRRE_BGRA), "BGRA") &
            assertEquals(PRRE_BRG,  PRREImageManager::getMirroredPixelComponentOrder(PRRE_GRB),  "GRB") &
            assertEquals(PRRE_BRGA, PRREImageManager::getMirroredPixelComponentOrder(PRRE_GRBA), "GRBA") &
            assertEquals(PRRE_RBG,  PRREImageManager::getMirroredPixelComponentOrder(PRRE_GBR),  "GBR") &
            assertEquals(PRRE_RBGA, PRREImageManager::getMirroredPixelComponentOrder(PRRE_GBRA), "GBRA");
    }

    bool testCtor1()
    {
        const PRREImageManager im;
        return true;
    }

    bool testCreateImageFromFileNull()
    {
        PRREImageManager im;
        const PRREImage* const img = im.createFromFile(NULL);

        return assertNull(img, "img");
    }

    bool testCreateImageFromFileNotExists()
    {
        PRREImageManager im;
        const PRREImage* const img = im.createFromFile("kvkkkveamamvmkadmvmakmdvkmakdvma");

        return assertNull(img, "img");
    }

    bool testCreateImageFromFileInvalidExtension()
    {
        PRREImageManager im;
        const PRREImage* const img = im.createFromFile("_res/proba128x128x24");

        return assertNull(img, "img");
    }

    bool testCreateImageFromFileInvalidFormat()
    {
        PRREImageManager im;
        const PRREImage* const img = im.createFromFile("_res/proba128x128x24_err.bmp");

        return assertNull(img, "img");
    }

    bool testCreateImageFromFileValid()
    {
        PRREImageManager im;
        const PRREImage* const img = im.createFromFile("_res/proba128x128x24.bmp");

        return assertNotNull(img, "img");
    }

    bool testCreateBlankImageValid()
    {
        PRREImageManager im;
        const PRREImage* const img = im.createBlank(1024, 768, 24);

        return assertNotNull(img, "img");
    }

    bool testCreateBlankImageInvalid()
    {
        PRREImageManager im;
        const PRREImage* const img = im.createBlank(0, 768, 24);
        const PRREImage* const img2 = im.createBlank(1024, 0, 24);
        const PRREImage* const img3 = im.createBlank(1024, 768, 16);

        return assertNull(img, "img") &
            assertNull(img2, "img2")  &
            assertNull(img3, "img3");
    }

    bool testWriteList()
    {
        const PRREImageManager im;
        im.WriteList();

        return true;
    }

    
}; // class PRREImageManagerTest
