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

    virtual bool setUp()
    {
        return true;
    }

    virtual void TearDown()
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
        bool b = assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_RGB,  Pure_RGB),  "RGB-RGB") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_RGBA, Pure_RGBA), "RGBA-RGBA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_RBG,  Pure_RBG),  "RBG-RBG") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_RBGA, Pure_RBGA), "RBGA-RBGA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_BRG,  Pure_BRG),  "BRG-BRG") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_BRGA, Pure_BRGA), "BRGA-BRGA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_BGR,  Pure_BGR),  "BGR-BGR") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_BGRA, Pure_BGRA), "BGRA-BGRA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_GRB,  Pure_GRB),  "GRB-GRB") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_GRBA, Pure_GRBA), "GRBA-GRBA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_GBR,  Pure_GBR),  "GBR-GBR") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_GBRA, Pure_GBRA), "GBRA-GBRA");

        b &= assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_RGBA), "RGB-RGBA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_RBGA), "RBG-RBGA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_BRGA), "BRG-BRGA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_BGRA), "BGR-BGRA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_GRBA), "GRB-GRBA") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_GBRA), "GBR-GBRA");

        b &= assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_RGBA, Pure_RGB), "RGBA-RGB") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_RBGA, Pure_RBG), "RBGA-RBG") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_BRGA, Pure_BRG), "BRGA-BRG") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_BGRA, Pure_BGR), "BGRA-BGR") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_GRBA, Pure_GRB), "GRBA-GRB") &
            assertEquals((TPureuint) 0, PureImageManager::getColorConversionSwapCount(Pure_GBRA, Pure_GBR), "GBRA-GBR");

        b &= assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_RBG),  "RGB-RBG") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_RBGA), "RGB-RBGA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_BRG),  "RGB-BRG") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_BRGA), "RGB-BRGA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_BGR),  "RGB-BGR") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_BGRA), "RGB-BGRA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_GRB),  "RGB-GRB") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_GRBA), "RGB-GRBA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_GBR),  "RGB-GBR") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_RGB, Pure_GBRA), "RGB-GBRA");

        b &= assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_RGB),  "RBG-RGB") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_RGBA), "RBG-RGBA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_BRG),  "RBG-BRG") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_BRGA), "RBG-BRGA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_BGR),  "RBG-BGR") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_BGRA), "RBG-BGRA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_GRB),  "RBG-GRB") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_GRBA), "RBG-GRBA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_GBR),  "RBG-GBR") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_RBG, Pure_GBRA), "RBG-GBRA");

        b &= assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_RGB),  "BRG-RGB") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_RGBA), "BRG-RGBA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_RBG),  "BRG-RBG") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_RBGA), "BRG-RBGA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_BGR),  "BRG-BGR") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_BGRA), "BRG-BGRA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_GRB),  "BRG-GRB") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_GRBA), "BRG-GRBA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_GBR),  "BRG-GBR") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_BRG, Pure_GBRA), "BRG-GBRA");

        b &= assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_RGB),  "BGR-RGB") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_RGBA), "BGR-RGBA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_RBG),  "BGR-RBG") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_RBGA), "BGR-RBGA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_BRG),  "BGR-BRG") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_BRGA), "BGR-BRGA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_GRB),  "BGR-GRB") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_GRBA), "BGR-GRBA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_GBR),  "BGR-GBR") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_BGR, Pure_GBRA), "BGR-GBRA");

        b &= assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_RGB),  "GRB-RGB") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_RGBA), "GRB-RGBA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_RBG),  "GRB-RBG") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_RBGA), "GRB-RBGA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_BRG),  "GRB-BRG") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_BRGA), "GRB-BRGA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_BGR),  "GRB-BGR") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_BGRA), "GRB-BGRA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_GBR),  "GRB-GBR") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GRB, Pure_GBRA), "GRB-GBRA");

        b &= assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_RGB),  "GBR-RGB") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_RGBA), "GBR-RGBA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_RBG),  "GBR-RBG") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_RBGA), "GBR-RBGA") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_BRG),  "GBR-BRG") &
            assertEquals((TPureuint) 2, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_BRGA), "GBR-BRGA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_BGR),  "GBR-BGR") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_BGRA), "GBR-BGRA") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_GRB),  "GBR-GRB") &
            assertEquals((TPureuint) 1, PureImageManager::getColorConversionSwapCount(Pure_GBR, Pure_GRBA), "GBR-GRBA");

        return b;
    }

    bool testGetMirroredPixelComponentOrder()
    {
        return assertEquals(Pure_BGR,  PureImageManager::getMirroredPixelComponentOrder(Pure_RGB),  "RGB") &
            assertEquals(Pure_BGRA, PureImageManager::getMirroredPixelComponentOrder(Pure_RGBA), "RGBA") & 
            assertEquals(Pure_GBR,  PureImageManager::getMirroredPixelComponentOrder(Pure_RBG),  "RBG") &
            assertEquals(Pure_GBRA, PureImageManager::getMirroredPixelComponentOrder(Pure_RBGA), "RBGA") &
            assertEquals(Pure_GRB,  PureImageManager::getMirroredPixelComponentOrder(Pure_BRG),  "BRG") &
            assertEquals(Pure_GRBA, PureImageManager::getMirroredPixelComponentOrder(Pure_BRGA), "BRGA") &
            assertEquals(Pure_RGB,  PureImageManager::getMirroredPixelComponentOrder(Pure_BGR),  "BGR") &
            assertEquals(Pure_RGBA, PureImageManager::getMirroredPixelComponentOrder(Pure_BGRA), "BGRA") &
            assertEquals(Pure_BRG,  PureImageManager::getMirroredPixelComponentOrder(Pure_GRB),  "GRB") &
            assertEquals(Pure_BRGA, PureImageManager::getMirroredPixelComponentOrder(Pure_GRBA), "GRBA") &
            assertEquals(Pure_RBG,  PureImageManager::getMirroredPixelComponentOrder(Pure_GBR),  "GBR") &
            assertEquals(Pure_RBGA, PureImageManager::getMirroredPixelComponentOrder(Pure_GBRA), "GBRA");
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
