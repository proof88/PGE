#pragma once

/*
    ###################################################################################
    PRREMaterialTest.h
    Unit test for PRREMaterial.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/Material/PRREMaterialManager.h"
#include "../PRRE/PR00FsReducedRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

#ifndef BMP128x128x24
#define BMP128x128x24 "_res/proba128x128x24.bmp"
#endif 

class PRREMaterialTest :
    public UnitTest
{
public:

    PRREMaterialTest() :
        UnitTest(__FILE__)
    {
        engine = NULL;
        tm = NULL;
        mat = NULL;
    }

    ~PRREMaterialTest()
    {
        Finalize();
    } // ~PRREMaterialTest()

protected:

    virtual void Initialize()
    {
        engine = &PR00FsReducedRenderingEngine::createAndGet();
        engine->initialize(PRRE_RENDERER_HW_FP, 800, 600, PRRE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        tm = &engine->getTextureManager();
        mm = &engine->getMaterialManager();

        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREMaterialTest::testCtor);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PRREMaterialTest::testDtor);
        AddSubTest("testAllocateArrays", (PFNUNITSUBTEST) &PRREMaterialTest::testAllocateArrays);
        AddSubTest("testSetTexture", (PFNUNITSUBTEST) &PRREMaterialTest::testSetTexture);
        AddSubTest("testGetTextureCount", (PFNUNITSUBTEST) &PRREMaterialTest::testGetTextureCount);
        AddSubTest("testIsTextured", (PFNUNITSUBTEST) &PRREMaterialTest::testIsTextured);
        AddSubTest("testIsSingleTextured", (PFNUNITSUBTEST) &PRREMaterialTest::testIsSingleTextured);
        AddSubTest("testIsMultiTextured", (PFNUNITSUBTEST) &PRREMaterialTest::testIsMultiTextured);
        AddSubTest("testSetSourceBlendFunc", (PFNUNITSUBTEST) &PRREMaterialTest::testSetSourceBlendFunc);
        AddSubTest("testSetDestinationBlendFunc", (PFNUNITSUBTEST) &PRREMaterialTest::testSetDestinationBlendFunc);
        AddSubTest("testSetBlendFuncs", (PFNUNITSUBTEST) &PRREMaterialTest::testSetBlendFuncs);
        AddSubTest("testSetBlendMode", (PFNUNITSUBTEST) &PRREMaterialTest::testSetBlendMode);
        AddSubTest("testCopyFromMaterial", (PFNUNITSUBTEST) &PRREMaterialTest::testCopyFromMaterial);
        AddSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PRREMaterialTest::testGetUsedSystemMemory);

    }

    virtual bool setUp()
    {
        mat = mm->createMaterial();
        return assertNotNull(mat, "mat null");
    }

    virtual void TearDown()
    {
        mm->DeleteAll();
        mat = NULL;
    }

    virtual void Finalize()
    {
        tm = NULL;
        mm = NULL;
        mat = NULL;

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
    }

private:
    PR00FsReducedRenderingEngine* engine;
    PRRETextureManager* tm;
    PRREMaterialManager* mm;
    PRREMaterial* mat;

    // ---------------------------------------------------------------------------

    PRREMaterialTest(const PRREMaterialTest&)
    {};         

    PRREMaterialTest& operator=(const PRREMaterialTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PRREColor clrWhite(255,255,255,255);

        const TPRREuint nMaxLayers = mm->getMaximumLayerCount();
        bool b = true;
        TPRREuint i = 0;

        while ( i < nMaxLayers )
        {
            // by default no array is allocated
            b = b & assertEquals((TPRREuint) 0, mat->getColorsCount(i), "getColorsCount") &
                assertNull(mat->getColors(i), "getColors") &
                assertEquals((TPRREuint)0, mat->getIndicesCount(), "getIndicesCount") &
                assertEquals((TPRREuint)0, mat->getTexcoordsCount(i), "getTexcoordsCount") &
                assertNull(mat->getTexcoords(i), "getTexcoords") &
                assertNull(mat->getTexture(i), "getTexture") &
                assertTrue(clrWhite == mat->getTextureEnvColor(i), "getTextureEnvColor") &
                assertEquals(TPRRE_BLENDFACTORS::PRRE_ONE, mat->getSourceBlendFunc(i), "getSourceBlendFunc") &
                assertEquals(TPRRE_BLENDFACTORS::PRRE_ZERO, mat->getDestinationBlendFunc(i), "getDestinationBlendFunc") &
                assertEquals(TPRRE_BLENDMODES::PRRE_BM_NONE, mat->getBlendMode(i), "getBlendMode");
            i++;
        }
        std::stringstream ss;
        ss << "Failed in " << i << ". iteration" << std::endl;
        assertTrue(b, ss.str().c_str());

        return b &
            assertFalse(mat->isTextured(), "isTextured") &
            assertFalse(mat->isSingleTextured(), "isSingleTextured") &
            assertFalse(mat->isMultiTextured(), "isMultiTextured");
    }

    bool testDtor()
    {
        delete mat;
        mat = NULL;

        return assertEquals(0, mm->getCount());
    }

    bool testAllocateArrays()
    {
        const TPRREuint nColors    = 5;
        const TPRREuint nTexCoords = 6;
        const TPRREuint nIndices   = 7;
        const TPRREuint nIndexSize = 4;

        mat->AllocateArrays(nColors, nTexCoords, nIndices, nIndexSize);

        bool b = true;
        TPRREuint  i = 0;
        while ( i < mm->getMaximumLayerCount() )
        {
            b = b & assertEquals(nColors, mat->getColorsCount(i), "getColorsCount") &
                assertNotNull(mat->getColors(i), "getColors") &
                assertEquals(nIndices, mat->getIndicesCount(), "getIndicesCount") &
                assertEquals(nTexCoords, mat->getTexcoordsCount(i), "getTexcoordsCount") &
                assertNotNull(mat->getTexcoords(i), "getTexcoords");
            i++;
        }

        std::stringstream ss;
        ss << "Failed in " << i << ". iteration" << std::endl;

        return assertTrue(b, ss.str().c_str());
    }

    bool testSetTexture()
    {
        PRRETexture* tex128x128x24 = PR00FsReducedRenderingEngine::createAndGet().getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        mat->SetTexture(tex128x128x24);
        bool b = assertNotNull(mat->getTexture(), "1st tex 0") &
            assertNull(mat->getTexture(1), "1st tex 1");

        mat->SetTexture(NULL);
        mat->SetTexture(tex128x128x24, 1);
        b = b & 
            assertNotNull(mat->getTexture(1), "2nd tex 1") &
            assertNull(mat->getTexture(), "2nd tex 0");

        return b;
    }

    bool testGetTextureCount()
    {
        PRRETexture* tex128x128x24 = PR00FsReducedRenderingEngine::createAndGet().getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        bool b = assertEquals((TPRREuint)0, mat->getTextureCount(), "1st");

        mat->SetTexture(tex128x128x24);
        b = b & assertEquals((TPRREuint)1, mat->getTextureCount(), "2nd");

        mat->SetTexture(NULL);
        mat->SetTexture(tex128x128x24, 1);
        b = b & assertEquals((TPRREuint)1, mat->getTextureCount(), "3rd");

        mat->SetTexture(tex128x128x24);
        b = b & assertEquals((TPRREuint)2, mat->getTextureCount(), "4th");

        return b;
    }

    bool testIsTextured()
    {
        PRRETexture* tex128x128x24 = PR00FsReducedRenderingEngine::createAndGet().getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        bool b = assertFalse(mat->isTextured(), "1st istex");

        mat->SetTexture(tex128x128x24);
        b = b & assertTrue(mat->isTextured(), "2nd istex");

        mat->SetTexture(NULL);
        mat->SetTexture(tex128x128x24, 1);
        b = b & assertTrue(mat->isTextured(), "3rd istex");

        return b;
    }

    bool testIsSingleTextured()
    {
        PRRETexture* tex128x128x24 = PR00FsReducedRenderingEngine::createAndGet().getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        bool b = assertFalse(mat->isSingleTextured(), "1st");

        mat->SetTexture(tex128x128x24);
        b = b & assertTrue(mat->isSingleTextured(), "2nd");

        mat->SetTexture(NULL);
        mat->SetTexture(tex128x128x24, 1);
        b = b & assertTrue(mat->isSingleTextured(), "3rd");

        mat->SetTexture(tex128x128x24);
        b = b & assertFalse(mat->isSingleTextured(), "4th");

        return b;
    }

    bool testIsMultiTextured()
    {
        PRRETexture* tex128x128x24 = PR00FsReducedRenderingEngine::createAndGet().getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        bool b = assertFalse(mat->isMultiTextured(), "1st");

        mat->SetTexture(tex128x128x24);
        b = b & assertFalse(mat->isMultiTextured(), "2nd");

        mat->SetTexture(NULL);
        mat->SetTexture(tex128x128x24, 1);
        b = b & assertFalse(mat->isMultiTextured(), "3rd");

        mat->SetTexture(tex128x128x24);
        b = b & assertTrue(mat->isMultiTextured(), "4th");

        return b;
    }

    bool testSetSourceBlendFunc()
    {
        mat->SetSourceBlendFunc(TPRRE_BLENDFACTORS::PRRE_CONSTANT_ALPHA);
        bool b = assertEquals( TPRRE_BLENDFACTORS::PRRE_CONSTANT_ALPHA, mat->getSourceBlendFunc(), "1st");

        mat->SetSourceBlendFunc(TPRRE_BLENDFACTORS::PRRE_CONSTANT_COLOR, 1);
        b = b & assertEquals( TPRRE_BLENDFACTORS::PRRE_CONSTANT_COLOR, mat->getSourceBlendFunc(1), "2nd");

        mat->SetSourceBlendFunc(TPRRE_BLENDFACTORS::PRRE_SRC_COLOR); // invalid param
        b = b & assertEquals( TPRRE_BLENDFACTORS::PRRE_CONSTANT_COLOR, mat->getSourceBlendFunc(1), "3rd");

         mat->SetSourceBlendFunc(TPRRE_BLENDFACTORS::PRRE_ONE_MINUS_SRC_COLOR); // invalid param
        b = b & assertEquals( TPRRE_BLENDFACTORS::PRRE_CONSTANT_COLOR, mat->getSourceBlendFunc(1), "4th");

        return b;
    }

    bool testSetDestinationBlendFunc()
    {
        mat->SetDestinationBlendFunc(TPRRE_BLENDFACTORS::PRRE_CONSTANT_ALPHA);
        bool b = assertEquals( TPRRE_BLENDFACTORS::PRRE_CONSTANT_ALPHA, mat->getDestinationBlendFunc(), "1st");

        mat->SetDestinationBlendFunc(TPRRE_BLENDFACTORS::PRRE_CONSTANT_COLOR, 1);
        b = b & assertEquals( TPRRE_BLENDFACTORS::PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "2nd");

        mat->SetDestinationBlendFunc(TPRRE_BLENDFACTORS::PRRE_DST_COLOR, 1); // invalid param
        b = b & assertEquals( TPRRE_BLENDFACTORS::PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "3rd");

        mat->SetDestinationBlendFunc(TPRRE_BLENDFACTORS::PRRE_ONE_MINUS_DST_COLOR, 1); // invalid param
        b = b & assertEquals( TPRRE_BLENDFACTORS::PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "4th");

        mat->SetDestinationBlendFunc(TPRRE_BLENDFACTORS::PRRE_SRC_ALPHA_SATURATE, 1); // invalid param
        b = b & assertEquals( TPRRE_BLENDFACTORS::PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "5th");

        return b;
    }

    bool testSetBlendFuncs()
    {
        mat->SetBlendFuncs(PRRE_CONSTANT_ALPHA, PRRE_CONSTANT_COLOR);
        bool b = assertEquals( PRRE_CONSTANT_ALPHA, mat->getSourceBlendFunc(), "1st src") &
            assertEquals( PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(), "1st dst");

        mat->SetBlendFuncs(PRRE_CONSTANT_ALPHA, PRRE_CONSTANT_COLOR, 1);
        b = b & assertEquals( PRRE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "2nd src") &
            assertEquals( PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "2nd dst");

        mat->SetBlendFuncs(PRRE_SRC_COLOR, PRRE_CONSTANT_COLOR, 1);
        b = b & assertEquals( PRRE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "3rd src") &
            assertEquals( PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "3rd dst");

        mat->SetBlendFuncs(PRRE_ONE_MINUS_SRC_COLOR, PRRE_CONSTANT_COLOR, 1);
        b = b & assertEquals( PRRE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "4th src") &
            assertEquals( PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "4th dst");

        mat->SetBlendFuncs(PRRE_CONSTANT_ALPHA, PRRE_DST_COLOR, 1);
        b = b & assertEquals( PRRE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "5th src") &
            assertEquals( PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "5th dst");

        mat->SetBlendFuncs(PRRE_CONSTANT_ALPHA, PRRE_ONE_MINUS_DST_COLOR, 1);
        b = b & assertEquals( PRRE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "6th src") &
            assertEquals( PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "6th dst");

        mat->SetBlendFuncs(PRRE_CONSTANT_ALPHA, PRRE_SRC_ALPHA_SATURATE, 1);
        b = b & assertEquals( PRRE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "7th src") &
            assertEquals( PRRE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "7th dst");

        return b;
    }

    bool testSetBlendMode()
    {
        mat->SetBlendMode(TPRRE_BLENDMODES::PRRE_BM_STANDARD_TRANSPARENCY);
        bool b = assertEquals( PRRE_BM_STANDARD_TRANSPARENCY, mat->getBlendMode(), "1st mode") & 
            assertEquals( PRRE_SRC_ALPHA, mat->getSourceBlendFunc(), "1st src") &
            assertEquals( PRRE_ONE_MINUS_SRC_ALPHA, mat->getDestinationBlendFunc(), "1st dst");

        mat->SetBlendMode(TPRRE_BLENDMODES::PRRE_BM_NONE, 1);
        b = b & assertEquals( PRRE_BM_NONE, mat->getBlendMode(1), "2nd mode") & 
            assertEquals( PRRE_ONE, mat->getSourceBlendFunc(1), "2nd src") &
            assertEquals( PRRE_ZERO, mat->getDestinationBlendFunc(1), "2nd dst");

        return b;
    }

    bool testCopyFromMaterial()
    {
        PRREMaterial* mat2 = mm->createMaterial();
        if ( !mat2 )
            return assertNotNull(mat2, "notNull 2");
        
        mat2->AllocateArrays(2, 2, 2, 2);
        mat2->getColors()[0].green = 1.0f;
        mat2->getTexcoords()[1].u = 1.0f;
 
        bool b = assertFalse(mat->copyFromMaterial(*mat2, 0, 0), "1st");  // should fail due to non-matching array layout
       
        // make sure nothing has been changed in mat
        TPRREuint i = 0;
        while ( i < mm->getMaximumLayerCount() )
        {
            b = b & assertEquals((TPRREuint) 0, mat->getColorsCount(i), "getColorsCount") &
                assertNull(mat->getColors(i), "getColors") &
                assertEquals((TPRREuint)0, mat->getIndicesCount(), "getIndicesCount") &
                assertEquals((TPRREuint)0, mat->getTexcoordsCount(i), "getTexcoordsCount") &
                assertNull(mat->getTexcoords(i), "getTexcoords");
            i++;
        }
        std::stringstream ss;
        ss << "Failed in " << i << ". iteration" << std::endl;
        assertTrue(b, ss.str().c_str());

        mat->AllocateArrays(2, 2, 2, 2);

        b = b & assertTrue(mat->copyFromMaterial(*mat2, 0, 0), "2nd");  // should succeed now

        // sanity check for the layout before checking actual values in arrays
        i = 0;
        while ( i < mm->getMaximumLayerCount() )
        {
            b = b & assertEquals((TPRREuint) 2, mat->getColorsCount(i), "getColorsCount") &
                assertNotNull(mat->getColors(i), "getColors") &
                assertEquals((TPRREuint)2, mat->getIndicesCount(), "getIndicesCount") &
                assertEquals((TPRREuint)2, mat->getTexcoordsCount(i), "getTexcoordsCount") &
                assertNotNull(mat->getTexcoords(i), "getTexcoords");
            i++;
        }
        ss.clear();
        ss << "Failed in " << i << ". iteration" << std::endl;
        assertTrue(b, ss.str().c_str());

        // we should have the same values in the 1st material too 
        b = b & assertEquals(1.0f, mat->getColors()[0].green, "clr lvl0 green") &
            assertEquals(1.0f, mat->getTexcoords()[1].u, "texcoord lvl1 U");

        return b;
    }

    bool testGetUsedSystemMemory()
    {
        return assertGreater(mat->getUsedSystemMemory(), sizeof(PRREMaterial));
    }

}; // class PRREMaterialTest