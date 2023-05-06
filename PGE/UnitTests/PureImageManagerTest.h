#pragma once

/*
    ###################################################################################
    PureImageManagerTest.h
    Unit test for PureImageManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Material/PureImageManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureImageManagerTest :
    public UnitTest
{
public:

    PureImageManagerTest() :
        UnitTest( __FILE__ )
    {
        
    };

    virtual ~PureImageManagerTest()
    {
        
    }

protected:

    virtual void Initialize()
    {
        /*CConsole::getConsoleInstance().SetLoggingState(PureImage::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureImageManager::getLoggerModuleName(), true);*/
        AddSubTest("testGetColorConversionSwapCount", (PFNUNITSUBTEST) &PureImageManagerTest::testGetColorConversionSwapCount);
        AddSubTest("testGetMirroredPixelComponentOrder", (PFNUNITSUBTEST) &PureImageManagerTest::testGetMirroredPixelComponentOrder);
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PureImageManagerTest::testCtor1);
        AddSubTest("testCreateImageFromFileNull", (PFNUNITSUBTEST) &PureImageManagerTest::testCreateImageFromFileNull);
        AddSubTest("testCreateImageFromFileNotExists", (PFNUNITSUBTEST) &PureImageManagerTest::testCreateImageFromFileNotExists);
        AddSubTest("testCreateImageFromFileInvalidExtension", (PFNUNITSUBTEST) &PureImageManagerTest::testCreateImageFromFileInvalidExtension);
        AddSubTest("testCreateImageFromFileInvalidFormat", (PFNUNITSUBTEST) &PureImageManagerTest::testCreateImageFromFileInvalidFormat);
        AddSubTest("testCreateImageFromFileValid", (PFNUNITSUBTEST) &PureImageManagerTest::testCreateImageFromFileValid);
        AddSubTest("testCreateBlankImageValid", (PFNUNITSUBTEST) &PureImageManagerTest::testCreateBlankImageValid);
        AddSubTest("testCreateBlankImageInvalid", (PFNUNITSUBTEST) &PureImageManagerTest::testCreateBlankImageInvalid);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PureImageManagerTest::testWriteList);
    }

    virtual bool setUp() override
    {
        return true;
    }

    virtual void TearDown() override
    {}

    virtual void Finalize()
    {
        CConsole::getConsoleInstance().SetLoggingState(PureImage::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureImageManager::getLoggerModuleName(), false);    
    }


private:

    PureImageManagerTest(const PureImageManagerTest&)
    {};         

    PureImageManagerTest& operator=(const PureImageManagerTest&)
    {
        return *this;
    };

    bool testGetColorConversionSwapCount()
    {
        bool b = assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_RGB,  PURE_RGB),  "RGB-RGB") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_RGBA, PURE_RGBA), "RGBA-RGBA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_RBG,  PURE_RBG),  "RBG-RBG") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_RBGA, PURE_RBGA), "RBGA-RBGA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_BRG,  PURE_BRG),  "BRG-BRG") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_BRGA, PURE_BRGA), "BRGA-BRGA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_BGR,  PURE_BGR),  "BGR-BGR") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_BGRA, PURE_BGRA), "BGRA-BGRA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_GRB,  PURE_GRB),  "GRB-GRB") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_GRBA, PURE_GRBA), "GRBA-GRBA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_GBR,  PURE_GBR),  "GBR-GBR") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_GBRA, PURE_GBRA), "GBRA-GBRA");

        b &= assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_RGBA), "RGB-RGBA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_RBGA), "RBG-RBGA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_BRGA), "BRG-BRGA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_BGRA), "BGR-BGRA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_GRBA), "GRB-GRBA") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_GBRA), "GBR-GBRA");

        b &= assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_RGBA, PURE_RGB), "RGBA-RGB") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_RBGA, PURE_RBG), "RBGA-RBG") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_BRGA, PURE_BRG), "BRGA-BRG") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_BGRA, PURE_BGR), "BGRA-BGR") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_GRBA, PURE_GRB), "GRBA-GRB") &
            assertEquals((TPureUInt) 0, PureImageManager::getColorConversionSwapCount(PURE_GBRA, PURE_GBR), "GBRA-GBR");

        b &= assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_RBG),  "RGB-RBG") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_RBGA), "RGB-RBGA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_BRG),  "RGB-BRG") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_BRGA), "RGB-BRGA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_BGR),  "RGB-BGR") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_BGRA), "RGB-BGRA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_GRB),  "RGB-GRB") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_GRBA), "RGB-GRBA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_GBR),  "RGB-GBR") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_RGB, PURE_GBRA), "RGB-GBRA");

        b &= assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_RGB),  "RBG-RGB") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_RGBA), "RBG-RGBA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_BRG),  "RBG-BRG") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_BRGA), "RBG-BRGA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_BGR),  "RBG-BGR") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_BGRA), "RBG-BGRA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_GRB),  "RBG-GRB") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_GRBA), "RBG-GRBA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_GBR),  "RBG-GBR") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_RBG, PURE_GBRA), "RBG-GBRA");

        b &= assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_RGB),  "BRG-RGB") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_RGBA), "BRG-RGBA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_RBG),  "BRG-RBG") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_RBGA), "BRG-RBGA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_BGR),  "BRG-BGR") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_BGRA), "BRG-BGRA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_GRB),  "BRG-GRB") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_GRBA), "BRG-GRBA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_GBR),  "BRG-GBR") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_BRG, PURE_GBRA), "BRG-GBRA");

        b &= assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_RGB),  "BGR-RGB") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_RGBA), "BGR-RGBA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_RBG),  "BGR-RBG") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_RBGA), "BGR-RBGA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_BRG),  "BGR-BRG") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_BRGA), "BGR-BRGA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_GRB),  "BGR-GRB") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_GRBA), "BGR-GRBA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_GBR),  "BGR-GBR") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_BGR, PURE_GBRA), "BGR-GBRA");

        b &= assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_RGB),  "GRB-RGB") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_RGBA), "GRB-RGBA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_RBG),  "GRB-RBG") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_RBGA), "GRB-RBGA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_BRG),  "GRB-BRG") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_BRGA), "GRB-BRGA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_BGR),  "GRB-BGR") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_BGRA), "GRB-BGRA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_GBR),  "GRB-GBR") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GRB, PURE_GBRA), "GRB-GBRA");

        b &= assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_RGB),  "GBR-RGB") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_RGBA), "GBR-RGBA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_RBG),  "GBR-RBG") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_RBGA), "GBR-RBGA") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_BRG),  "GBR-BRG") &
            assertEquals((TPureUInt) 2, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_BRGA), "GBR-BRGA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_BGR),  "GBR-BGR") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_BGRA), "GBR-BGRA") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_GRB),  "GBR-GRB") &
            assertEquals((TPureUInt) 1, PureImageManager::getColorConversionSwapCount(PURE_GBR, PURE_GRBA), "GBR-GRBA");

        return b;
    }

    bool testGetMirroredPixelComponentOrder()
    {
        return assertEquals(PURE_BGR,  PureImageManager::getMirroredPixelComponentOrder(PURE_RGB),  "RGB") &
            assertEquals(PURE_BGRA, PureImageManager::getMirroredPixelComponentOrder(PURE_RGBA), "RGBA") & 
            assertEquals(PURE_GBR,  PureImageManager::getMirroredPixelComponentOrder(PURE_RBG),  "RBG") &
            assertEquals(PURE_GBRA, PureImageManager::getMirroredPixelComponentOrder(PURE_RBGA), "RBGA") &
            assertEquals(PURE_GRB,  PureImageManager::getMirroredPixelComponentOrder(PURE_BRG),  "BRG") &
            assertEquals(PURE_GRBA, PureImageManager::getMirroredPixelComponentOrder(PURE_BRGA), "BRGA") &
            assertEquals(PURE_RGB,  PureImageManager::getMirroredPixelComponentOrder(PURE_BGR),  "BGR") &
            assertEquals(PURE_RGBA, PureImageManager::getMirroredPixelComponentOrder(PURE_BGRA), "BGRA") &
            assertEquals(PURE_BRG,  PureImageManager::getMirroredPixelComponentOrder(PURE_GRB),  "GRB") &
            assertEquals(PURE_BRGA, PureImageManager::getMirroredPixelComponentOrder(PURE_GRBA), "GRBA") &
            assertEquals(PURE_RBG,  PureImageManager::getMirroredPixelComponentOrder(PURE_GBR),  "GBR") &
            assertEquals(PURE_RBGA, PureImageManager::getMirroredPixelComponentOrder(PURE_GBRA), "GBRA");
    }

    bool testCtor1()
    {
        const PureImageManager im;
        return true;
    }

    bool testCreateImageFromFileNull()
    {
        PureImageManager im;
        const PureImage* const img = im.createFromFile(NULL);

        return assertNull(img, "img");
    }

    bool testCreateImageFromFileNotExists()
    {
        PureImageManager im;
        const PureImage* const img = im.createFromFile("kvkkkveamamvmkadmvmakmdvkmakdvma");

        return assertNull(img, "img");
    }

    bool testCreateImageFromFileInvalidExtension()
    {
        PureImageManager im;
        const PureImage* const img = im.createFromFile("_res/proba128x128x24");

        return assertNull(img, "img");
    }

    bool testCreateImageFromFileInvalidFormat()
    {
        PureImageManager im;
        const PureImage* const img = im.createFromFile("_res/proba128x128x24_err.bmp");

        return assertNull(img, "img");
    }

    bool testCreateImageFromFileValid()
    {
        PureImageManager im;
        const PureImage* const img = im.createFromFile("_res/proba128x128x24.bmp");

        return assertNotNull(img, "img");
    }

    bool testCreateBlankImageValid()
    {
        PureImageManager im;
        const PureImage* const img = im.createBlank(1024, 768, 24);

        return assertNotNull(img, "img");
    }

    bool testCreateBlankImageInvalid()
    {
        PureImageManager im;
        const PureImage* const img = im.createBlank(0, 768, 24);
        const PureImage* const img2 = im.createBlank(1024, 0, 24);
        const PureImage* const img3 = im.createBlank(1024, 768, 16);

        return assertNull(img, "img") &
            assertNull(img2, "img2")  &
            assertNull(img3, "img3");
    }

    bool testWriteList()
    {
        const PureImageManager im;
        im.WriteList();

        return true;
    }

    
}; // class PureImageManagerTest
