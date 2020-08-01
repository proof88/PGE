#pragma once

/*
    ###################################################################################
    PRREImageTest.h
    Unit test for PRREImage.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/PRREImageManager.h"

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

class PRREImageTest :
    public UnitTest
{
public:

    PRREImageTest() :
        UnitTest( __FILE__ )
    {
        img1024x768x24blank = NULL;
        img128x128x32opaque = NULL;
        img128x128x32transp = NULL;
        img128x128x8 = NULL;
        img128x128x4 = NULL;
        img128x128x1 = NULL;
    }

    ~PRREImageTest()
    {
        delete img1024x768x24blank;
        delete img128x128x32opaque;
        delete img128x128x32transp;
        delete img128x128x8;
        delete img128x128x4;
        delete img128x128x1;
    }

protected:

    virtual void Initialize()
    {
        im = new PRREImageManager();
        img128x128x24 = NULL;
        img1024x768x24blank = im->createBlank(1024, 768, 24);
        img128x128x32opaque = im->createFromFile(BMP128x128x32op);
        img128x128x32transp = im->createFromFile(BMP128x128x32tr);
        img128x128x8  = im->createFromFile(BMP128x128x8);
        img128x128x4  = im->createFromFile(BMP128x128x4);
        img128x128x1  = im->createFromFile(BMP128x128x1);

        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREImageTest::testCtor);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PRREImageTest::testDtor);
        AddSubTest("testGetWidth", (PFNUNITSUBTEST) &PRREImageTest::testGetWidth);
        AddSubTest("testGetHeight", (PFNUNITSUBTEST) &PRREImageTest::testGetHeight);
        AddSubTest("testGetFilename", (PFNUNITSUBTEST) &PRREImageTest::testGetFilename);
        AddSubTest("testGetBitsPerPixels", (PFNUNITSUBTEST) &PRREImageTest::testGetBitsPerPixels);
        AddSubTest("testGetPixelComponentOrder", (PFNUNITSUBTEST) &PRREImageTest::testGetPixelComponentOrder);
        AddSubTest("testGetOriginalPixelComponentOrder", (PFNUNITSUBTEST) &PRREImageTest::testGetOriginalPixelComponentOrder);
        AddSubTest("testSetPixelComponentOrderRGB", (PFNUNITSUBTEST) &PRREImageTest::testSetPixelComponentOrderRGB);
        AddSubTest("testSetPixelComponentOrderRBG", (PFNUNITSUBTEST) &PRREImageTest::testSetPixelComponentOrderRBG);
        AddSubTest("testSetPixelComponentOrderGRB", (PFNUNITSUBTEST) &PRREImageTest::testSetPixelComponentOrderGRB);
        AddSubTest("testSetPixelComponentOrderGBR", (PFNUNITSUBTEST) &PRREImageTest::testSetPixelComponentOrderGBR);
        AddSubTest("testSetPixelComponentOrderBGR", (PFNUNITSUBTEST) &PRREImageTest::testSetPixelComponentOrderBGR);
        AddSubTest("testSetPixelComponentOrderBRG", (PFNUNITSUBTEST) &PRREImageTest::testSetPixelComponentOrderBRG);
        AddSubTest("testGetPixel", (PFNUNITSUBTEST) &PRREImageTest::testGetPixel);
        AddSubTest("testSetPixel1", (PFNUNITSUBTEST) &PRREImageTest::testSetPixel1);
        AddSubTest("testSetPixel2", (PFNUNITSUBTEST) &PRREImageTest::testSetPixel2);
        AddSubTest("testIsChanged", (PFNUNITSUBTEST) &PRREImageTest::testIsChanged);
        AddSubTest("testGetPixels", (PFNUNITSUBTEST) &PRREImageTest::testGetPixels);
        AddSubTest("testGetPixelsSize", (PFNUNITSUBTEST) &PRREImageTest::testGetPixelsSize);
        AddSubTest("testFlushResources", (PFNUNITSUBTEST) &PRREImageTest::testFlushResources);
        AddSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PRREImageTest::testGetUsedSystemMemory);
    }

    virtual void SetUp()
    {
        img128x128x24 = im->createFromFile(BMP128x128x24);
    }

    virtual void TearDown()
    {
        delete img128x128x24;
        img128x128x24 = NULL;
    }

private:
    PRREImageManager* im;
    PRREImage* img128x128x24;
    PRREImage* img128x128x32opaque;
    PRREImage* img1024x768x24blank;
    const PRREImage* img128x128x32transp,
                   * img128x128x8,
                   * img128x128x4,
                   * img128x128x1;

    // ---------------------------------------------------------------------------

    PRREImageTest(const PRREImageTest&)
    {};         

    PRREImageTest& operator=(const PRREImageTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        return true;        
    }

    bool testDtor()
    {
        delete img128x128x24;
        img128x128x24 = NULL;
        return assertEquals(6, im->getCount());
    }

    bool testGetWidth()
    {
        return assertEquals((TPRREuint) 128, img128x128x24->getWidth(), "24") &
            assertEquals((TPRREuint) 128, img128x128x32opaque->getWidth(), "32") &
            assertEquals((TPRREuint) 128, img128x128x32transp->getWidth(), "32") &
            assertEquals((TPRREuint) 128, img128x128x8->getWidth(), "8") &
            assertEquals((TPRREuint) 128, img128x128x4->getWidth(), "4") &
            assertEquals((TPRREuint) 128, img128x128x1->getWidth(), "1") &
            assertEquals((TPRREuint) 1024, img1024x768x24blank->getWidth(), "blank");
    }

    bool testGetHeight()
    {
        return assertEquals((TPRREuint) 128, img128x128x24->getHeight(), "24") &
            assertEquals((TPRREuint) 128, img128x128x32opaque->getHeight(), "32") &
            assertEquals((TPRREuint) 128, img128x128x32transp->getHeight(), "32") &
            assertEquals((TPRREuint) 128, img128x128x8->getHeight(), "8") &
            assertEquals((TPRREuint) 128, img128x128x4->getHeight(), "4") &
            assertEquals((TPRREuint) 128, img128x128x1->getHeight(), "1") &
            assertEquals((TPRREuint) 768, img1024x768x24blank->getHeight(), "blank");
    }

    bool testGetFilename()
    {
        return assertEquals(BMP128x128x32op, img128x128x32opaque->getFilename(), "32 op") &
            assertEquals(BMP128x128x32tr, img128x128x32transp->getFilename(), "32 tr") &
            assertEquals(BMP128x128x24, img128x128x24->getFilename(), "24") &
            assertEquals(BMP128x128x8, img128x128x8->getFilename(), "8") &
            assertEquals(BMP128x128x4, img128x128x4->getFilename(), "4") &
            assertEquals(BMP128x128x1, img128x128x1->getFilename(), "1") &
            assertEquals("", img1024x768x24blank->getFilename(), "blank");
    }

    bool testGetBitsPerPixels()
    {
        return assertEquals((TPRREuint) 24, img128x128x24->getBitsPerPixels(), "24") &
            assertEquals((TPRREuint) 32, img128x128x32opaque->getBitsPerPixels(), "32") &
            assertEquals((TPRREuint) 32, img128x128x32transp->getBitsPerPixels(), "32") &
            assertEquals((TPRREuint) 24, img128x128x8->getBitsPerPixels(), "8") &
            assertEquals((TPRREuint) 24, img128x128x4->getBitsPerPixels(), "4") &
            assertEquals((TPRREuint) 24, img128x128x1->getBitsPerPixels(), "1") &
            assertEquals((TPRREuint) 24, img1024x768x24blank->getBitsPerPixels(), "blank");
    }

    bool testGetPixelComponentOrder()
    {
        return assertEquals(PRRE_BGR,  img128x128x24->getPixelComponentOrder(), "24") &
            assertEquals(PRRE_BGRA, img128x128x32opaque->getPixelComponentOrder(), "32") &
            assertEquals(PRRE_BGRA, img128x128x32transp->getPixelComponentOrder(), "32") &
            assertEquals(PRRE_RGB,  img128x128x8->getPixelComponentOrder(), "8") &
            assertEquals(PRRE_RGB,  img128x128x4->getPixelComponentOrder(), "4") &
            assertEquals(PRRE_RGB,  img128x128x1->getPixelComponentOrder(), "1") &
            assertEquals(PRRE_RGB,  img1024x768x24blank->getPixelComponentOrder(), "blank");
    }

    bool testGetOriginalPixelComponentOrder()
    {
        return assertEquals(PRRE_BGR,  img128x128x24->getOriginalPixelComponentOrder(), "24") &
            assertEquals(PRRE_BGRA, img128x128x32opaque->getOriginalPixelComponentOrder(), "32") &
            assertEquals(PRRE_BGRA, img128x128x32transp->getOriginalPixelComponentOrder(), "32") &
            assertEquals(PRRE_RGB,  img128x128x8->getOriginalPixelComponentOrder(), "8") &
            assertEquals(PRRE_RGB,  img128x128x4->getOriginalPixelComponentOrder(), "4") &
            assertEquals(PRRE_RGB,  img128x128x1->getOriginalPixelComponentOrder(), "1") &
            assertEquals(PRRE_RGB,  img1024x768x24blank->getOriginalPixelComponentOrder(), "blank");
    }

    bool testSetPixelComponentOrderRGB()
    {
        const PRREColor clr24_20_bef  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_bef  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_bef = img128x128x24->getPixel(100, 110);
        img128x128x24->SetPixelComponentOrder(PRRE_RGB);
        const PRREColor clr24_20_aft  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_aft  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_aft = img128x128x24->getPixel(100, 110);
        
        return assertEquals(PRRE_RGB, img128x128x24->getPixelComponentOrder(), "order") &
            assertTrue(clr24_20_bef == clr24_20_aft, "20") &
            assertTrue(clr24_60_bef == clr24_60_aft, "60") &
            assertTrue(clr24_100_bef == clr24_100_aft, "100");
    }

    bool testSetPixelComponentOrderRBG()
    {
        const PRREColor clr24_20_bef  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_bef  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_bef = img128x128x24->getPixel(100, 110);
        img128x128x24->SetPixelComponentOrder(PRRE_RBG);
        const PRREColor clr24_20_aft  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_aft  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_aft = img128x128x24->getPixel(100, 110);
        
        return assertEquals(PRRE_RBG, img128x128x24->getPixelComponentOrder(), "order") &
            assertTrue(clr24_20_bef == clr24_20_aft, "20") &
            assertTrue(clr24_60_bef == clr24_60_aft, "60") &
            assertTrue(clr24_100_bef == clr24_100_aft, "100");
    }

    bool testSetPixelComponentOrderGRB()
    {
        const PRREColor clr24_20_bef  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_bef  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_bef = img128x128x24->getPixel(100, 110);
        img128x128x24->SetPixelComponentOrder(PRRE_GRB);
        const PRREColor clr24_20_aft  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_aft  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_aft = img128x128x24->getPixel(100, 110);
        
        return assertEquals(PRRE_GRB, img128x128x24->getPixelComponentOrder(), "order") &
            assertTrue(clr24_20_bef == clr24_20_aft, "20") &
            assertTrue(clr24_60_bef == clr24_60_aft, "60") &
            assertTrue(clr24_100_bef == clr24_100_aft, "100");
    }

    bool testSetPixelComponentOrderGBR()
    {
        const PRREColor clr24_20_bef  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_bef  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_bef = img128x128x24->getPixel(100, 110);
        img128x128x24->SetPixelComponentOrder(PRRE_GBR);
        const PRREColor clr24_20_aft  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_aft  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_aft = img128x128x24->getPixel(100, 110);
        
        return assertEquals(PRRE_GBR, img128x128x24->getPixelComponentOrder(), "order") &
            assertTrue(clr24_20_bef == clr24_20_aft, "20") &
            assertTrue(clr24_60_bef == clr24_60_aft, "60") &
            assertTrue(clr24_100_bef == clr24_100_aft, "100");
    }

    bool testSetPixelComponentOrderBGR()
    {
        const PRREColor clr24_20_bef  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_bef  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_bef = img128x128x24->getPixel(100, 110);
        img128x128x24->SetPixelComponentOrder(PRRE_BGR);
        const PRREColor clr24_20_aft  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_aft  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_aft = img128x128x24->getPixel(100, 110);
        
        return assertEquals(PRRE_BGR, img128x128x24->getPixelComponentOrder(), "order") &
            assertTrue(clr24_20_bef == clr24_20_aft, "20") &
            assertTrue(clr24_60_bef == clr24_60_aft, "60") &
            assertTrue(clr24_100_bef == clr24_100_aft, "100");
    }

    bool testSetPixelComponentOrderBRG()
    {
        const PRREColor clr24_20_bef  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_bef  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_bef = img128x128x24->getPixel(100, 110);
        img128x128x24->SetPixelComponentOrder(PRRE_BRG);
        const PRREColor clr24_20_aft  = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60_aft  = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100_aft = img128x128x24->getPixel(100, 110);
        
        return assertEquals(PRRE_BRG, img128x128x24->getPixelComponentOrder(), "order") &
            assertTrue(clr24_20_bef == clr24_20_aft, "20") &
            assertTrue(clr24_60_bef == clr24_60_aft, "60") &
            assertTrue(clr24_100_bef == clr24_100_aft, "100");
    }

    bool testGetPixel()
    {       
        /* For a long time, I did not understand why alpha values in clr32_*op and clr32_*tr were 0.
           Even when saving the picture with removed background in Photoshop, alpha values of non-transparent pixels were 0.
           Then I realized the trick: it is not enough to remove the background in PS, you also need to manually
           edit the transparency channel on the Channels tab. By default it is black (0) even when background is removed.
           I read the trick here: https://stackoverflow.com/questions/7369649/how-to-convert-32-bit-bmp-to-contain-alpha-channel
           Note that removing the background and saving to a different format like PNG would automatically saved alpha values
           properly without manually editing the transparency channel.
           Loading the BMP again in Photoshop will also work fine, although there will be white background but transparency channel
           will be properly loaded from the 32bit BMP. */

        /* Irfanview doesn't save alpha channel to BMP file at all, even if you open a 32bpp BMP. */

        // note: we don't test img1024x768x24blank here, as blank image content is undefined at the beginning
        const PRREColor clr32_20op  = img128x128x32opaque->getPixel(20,  110);
        const PRREColor clr32_60op  = img128x128x32opaque->getPixel(60,  110);
        const PRREColor clr32_100op = img128x128x32opaque->getPixel(100, 110);
        const PRREColor clr32_20tr  = img128x128x32transp->getPixel(20,  110);
        const PRREColor clr32_60tr  = img128x128x32transp->getPixel(60,  110);
        const PRREColor clr32_100tr = img128x128x32transp->getPixel(100, 110);
        const PRREColor clr24_20    = img128x128x24->getPixel(20,  110);
        const PRREColor clr24_60    = img128x128x24->getPixel(60,  110);
        const PRREColor clr24_100   = img128x128x24->getPixel(100, 110);
        const PRREColor clr8_20     = img128x128x8->getPixel(20,   110);
        const PRREColor clr8_60     = img128x128x8->getPixel(60,   110);
        const PRREColor clr8_100    = img128x128x8->getPixel(100,  110);
        const PRREColor clr4_20     = img128x128x4->getPixel(20,   110);
        const PRREColor clr4_60     = img128x128x4->getPixel(60,   110);
        const PRREColor clr4_100    = img128x128x4->getPixel(100,  110);
        const PRREColor clr1_20     = img128x128x1->getPixel(20,   110);
        const PRREColor clr1_60     = img128x128x1->getPixel(60,   110);
        const PRREColor clr1_100    = img128x128x1->getPixel(100,  110);

        const bool bReds = assertEquals(255, (int) clr32_20op.getRed(),  "r 32 op 20") &
            assertEquals(0,   (int) clr32_60op.getRed(),  "r 32 op 60") &
            assertEquals(0,   (int) clr32_100op.getRed(), "r 32 op 100") &
            assertEquals(255, (int) clr32_20tr.getRed(),  "r 32 tr 20") &
            assertEquals(0,   (int) clr32_60tr.getRed(),  "r 32 tr 60") &
            assertEquals(0,   (int) clr32_100tr.getRed(), "r 32 tr 100") &
            assertEquals(255, (int) clr24_20.getRed(),  "r 24 20") &
            assertEquals(0,   (int) clr24_60.getRed(),  "r 24 60") &
            assertEquals(0,   (int) clr24_100.getRed(), "r 24 100") &
            assertEquals(252, (int) clr8_20.getRed(),   "r 8 20") &
            assertEquals(4,   (int) clr8_60.getRed(),   "r 8 60") &
            assertEquals(4,   (int) clr8_100.getRed(),  "r 8 100") &
            assertEquals(252, (int) clr4_20.getRed(),   "r 4 20") &
            assertEquals(4,   (int) clr4_60.getRed(),   "r 4 60") &
            assertEquals(4,   (int) clr4_100.getRed(),  "r 4 100") &
            assertEquals(0,   (int) clr1_20.getRed(),   "r 1 20") &
            assertEquals(255, (int) clr1_60.getRed(),   "r 1 60") &
            assertEquals(0,   (int) clr1_100.getRed(),  "r 1 100");

        const bool bGreens = assertEquals(0,   (int) clr32_20op.getGreen(),  "g 32 op 20") &
            assertEquals(255, (int) clr32_60op.getGreen(),  "g 32 op 60") &
            assertEquals(0,   (int) clr32_100op.getGreen(), "g 32 op 100") &
            assertEquals(0,   (int) clr32_20tr.getGreen(),  "g 32 tr 20") &
            assertEquals(255, (int) clr32_60tr.getGreen(),  "g 32 tr 60") &
            assertEquals(0,   (int) clr32_100tr.getGreen(), "g 32 tr 100") &
            assertEquals(0,   (int) clr24_20.getGreen(),  "g 24 20") &
            assertEquals(255, (int) clr24_60.getGreen(),  "g 24 60") &
            assertEquals(0,   (int) clr24_100.getGreen(), "g 24 100") &
            assertEquals(2,   (int) clr8_20.getGreen(),   "g 8 20") &
            assertEquals(254, (int) clr8_60.getGreen(),   "g 8 60") &
            assertEquals(2,   (int) clr8_100.getGreen(),  "g 8 100") &
            assertEquals(2,   (int) clr4_20.getGreen(),   "g 4 20") &
            assertEquals(254, (int) clr4_60.getGreen(),   "g 4 60") &
            assertEquals(2,   (int) clr4_100.getGreen(),  "g 4 100") &
            assertEquals(0,   (int) clr1_20.getGreen(),   "g 1 20") &
            assertEquals(255, (int) clr1_60.getGreen(),   "g 1 60") &
            assertEquals(0,   (int) clr1_100.getGreen(),  "g 1 100");

        const bool bBlues = assertEquals(0,   (int) clr32_20op.getBlue(),  "b 32 op 20") &
            assertEquals(0,   (int) clr32_60op.getBlue(),  "b 32 op 60") &
            assertEquals(255, (int) clr32_100op.getBlue(), "b 32 op 100") &
            assertEquals(0,   (int) clr32_20tr.getBlue(),  "b 32 tr 20") &
            assertEquals(0,   (int) clr32_60tr.getBlue(),  "b 32 tr 60") &
            assertEquals(255, (int) clr32_100tr.getBlue(), "b 32 tr 100") &
            assertEquals(0,   (int) clr24_20.getBlue(),  "b 24 20") &
            assertEquals(0,   (int) clr24_60.getBlue(),  "b 24 60") &
            assertEquals(255, (int) clr24_100.getBlue(), "b 24 100") &
            assertEquals(4,   (int) clr8_20.getBlue(),   "b 8 20") &
            assertEquals(4,   (int) clr8_60.getBlue(),   "b 8 60") &
            assertEquals(252, (int) clr8_100.getBlue(),  "b 8 100") &
            assertEquals(4,   (int) clr4_20.getBlue(),   "b 4 20") &
            assertEquals(4,   (int) clr4_60.getBlue(),   "b 4 60") &
            assertEquals(252, (int) clr4_100.getBlue(),  "b 4 100") &
            assertEquals(0,   (int) clr1_20.getBlue(),   "b 1 20") &
            assertEquals(255, (int) clr1_60.getBlue(),   "b 1 60") &
            assertEquals(0,   (int) clr1_100.getBlue(),  "b 1 100");

        const bool bAlphas = assertEquals(255, (int) clr32_20op.getAlpha(),  "a 32 op 20") &
            assertEquals(255, (int) clr32_60op.getAlpha(),  "a 32 op 60") &
            assertEquals(255, (int) clr32_100op.getAlpha(), "a 32 op 100") &
            assertEquals(255, (int) clr32_20tr.getAlpha(),  "a 32 tr 20") &
            assertEquals(255, (int) clr32_60tr.getAlpha(),  "a 32 tr 60") &
            assertEquals(255, (int) clr32_100tr.getAlpha(), "a 32 tr 100") &
            assertEquals(255, (int) clr24_20.getAlpha(),  "a 24 20") &
            assertEquals(255, (int) clr24_60.getAlpha(),  "a 24 60") &
            assertEquals(255, (int) clr24_100.getAlpha(), "a 24 100")&
            assertEquals(255, (int) clr8_20.getAlpha(),   "a 8 20") &
            assertEquals(255, (int) clr8_60.getAlpha(),   "a 8 60") &
            assertEquals(255, (int) clr8_100.getAlpha(),  "a 8 100") &
            assertEquals(255, (int) clr4_20.getAlpha(),   "a 4 20") &
            assertEquals(255, (int) clr4_60.getAlpha(),   "a 4 60") &
            assertEquals(255, (int) clr4_100.getAlpha(),  "a 4 100") &
            assertEquals(255, (int) clr1_20.getAlpha(),   "a 1 20") &
            assertEquals(255, (int) clr1_60.getAlpha(),   "a 1 60") &
            assertEquals(255, (int) clr1_100.getAlpha(),  "a 1 100");

        return bReds && bGreens && bBlues && bAlphas;
    }

    bool testSetPixel1()
    {
        // note: we don't test img1024x768x24blank here, hopefully it works fine ... :)

        PRREColor clrOriginal = img128x128x32opaque->getPixel(10, 10);
        const PRREColor clrNew( (TPRREubyte)(255-clrOriginal.getRed()),
            (TPRREubyte)(255-clrOriginal.getGreen()),
            (TPRREubyte)(255-clrOriginal.getBlue()),
            (TPRREubyte)(255-clrOriginal.getAlpha()) );
        img128x128x32opaque->SetPixel(10, 10, clrNew.getRed(), clrNew.getGreen(), clrNew.getBlue(), clrNew.getAlpha());
        clrOriginal = img128x128x32opaque->getPixel(10, 10);

        return assertTrue(clrOriginal == clrNew);
    }

    bool testSetPixel2()
    {
        PRREColor clrOriginal = img128x128x32opaque->getPixel(10, 10);
        const PRREColor clrNew( (TPRREubyte)(255-clrOriginal.getRed()),
            (TPRREubyte)(255-clrOriginal.getGreen()),
            (TPRREubyte)(255-clrOriginal.getBlue()),
            (TPRREubyte)(255-clrOriginal.getAlpha()) );
        img128x128x32opaque->SetPixel(10, 10, clrNew);
        clrOriginal = img128x128x32opaque->getPixel(10, 10);

        return assertTrue(clrOriginal == clrNew);
    }

    bool testIsChanged()
    {
        bool b = assertFalse(img128x128x24->isChanged(), "b1");
        img128x128x24->SetPixelComponentOrder(PRRE_GBR);
        b &= assertTrue(img128x128x24->isChanged(), "b2");
        delete img128x128x24;
        
        img128x128x24 = im->createFromFile(BMP128x128x24);
        img128x128x24->SetPixel(5,5, 1,1,1);
        b &= assertTrue(img128x128x24->isChanged(), "b3");
        delete img128x128x24;

        img128x128x24 = im->createFromFile(BMP128x128x24);
        const PRREColor clr(1,1,1);
        img128x128x24->SetPixel(5,5, clr);
        b &= assertTrue(img128x128x24->isChanged(), "b4");
        delete img128x128x24;
        img128x128x24 = NULL;

        return b;
    }

    bool testGetPixels()
    {
        return assertNotNull(img128x128x24->getPixels(), "24") &
            assertNotNull(img1024x768x24blank->getPixels(), "blank");
    }

    bool testGetPixelsSize()
    {
        return assertEquals( img128x128x32opaque->getBitsPerPixels() *
                img128x128x32opaque->getWidth() *
                img128x128x32opaque->getHeight() / 8,
                img128x128x32opaque->getPixelsSize(), "32 op" ) &
            assertEquals( img128x128x32transp->getBitsPerPixels() *
                img128x128x32transp->getWidth() *
                img128x128x32transp->getHeight() / 8,
                img128x128x32transp->getPixelsSize(), "32 tr" ) &
            assertEquals( img128x128x24->getBitsPerPixels() *
                img128x128x24->getWidth() *
                img128x128x24->getHeight() / 8,
                img128x128x24->getPixelsSize(), "24" ) &
            assertEquals( img128x128x8->getBitsPerPixels() *
                img128x128x8->getWidth() *
                img128x128x8->getHeight() / 8,
                img128x128x8->getPixelsSize(), "8" ) &
            assertEquals( img128x128x4->getBitsPerPixels() *
                img128x128x4->getWidth() *
                img128x128x4->getHeight() / 8,
                img128x128x4->getPixelsSize(), "4" ) &
            assertEquals( img128x128x1->getBitsPerPixels() *
                img128x128x1->getWidth() *
                img128x128x1->getHeight() / 8,
                img128x128x1->getPixelsSize(), "1" ) &
            assertEquals( img1024x768x24blank->getBitsPerPixels() *
                img1024x768x24blank->getWidth() *
                img1024x768x24blank->getHeight() / 8,
                img1024x768x24blank->getPixelsSize(), "blank" );
    }

    bool testFlushResources()
    {
        const TPRREuint origSize = img128x128x24->getUsedSystemMemory();
        img128x128x24->FlushResources();
        const PRREColor clrBlack;

        return assertNull(img128x128x24->getPixels(), "getPixels()") & assertLess(img128x128x24->getUsedSystemMemory(), origSize, "mem") &
            assertTrue(clrBlack == img128x128x24->getPixel(5,5), "color") & assertEquals((TPRREuint) 0, img128x128x24->getPixelsSize(), "size");
    }

    bool testGetUsedSystemMemory()
    {
        return assertGreater(img128x128x32opaque->getUsedSystemMemory(), sizeof(PRREImage), "32 op") &
            assertGreater(img128x128x32transp->getUsedSystemMemory(), sizeof(PRREImage), "32 tr") &
            assertGreater(img128x128x24->getUsedSystemMemory(), sizeof(PRREImage), "24") &
            assertGreater(img128x128x8->getUsedSystemMemory(), sizeof(PRREImage), "8") &
            assertGreater(img128x128x4->getUsedSystemMemory(), sizeof(PRREImage), "4") &
            assertGreater(img128x128x1->getUsedSystemMemory(), sizeof(PRREImage), "1") &
            assertGreater(img1024x768x24blank->getUsedSystemMemory(), sizeof(PRREImage), "blank");
    }

}; // class PRREImageTest