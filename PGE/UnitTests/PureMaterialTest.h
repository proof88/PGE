#pragma once

/*
    ###################################################################################
    PureMaterialTest.h
    Unit test for PureMaterial.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Material/PureMaterialManager.h"
#include "../Pure/include/external/PR00FsUltimateRenderingEngine.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

#ifndef BMP128x128x24
#define BMP128x128x24 "_res/proba128x128x24.bmp"
#endif 

class PureSampleUtiliserManaged :
    public PureManaged
{
public:
    PureSampleUtiliserManaged() {}; /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureSampleUtiliserManaged() {};

protected:

    // ---------------------------------------------------------------------------

    PureSampleUtiliserManaged(const PureSampleUtiliserManaged&) {};
    PureSampleUtiliserManaged& operator=(const PureSampleUtiliserManaged& ) { return *this; };

private:

    // ---------------------------------------------------------------------------

}; // class PureSampleUtiliserManaged

class PureSampleManagerForUtiliserManaged :
    public PureManager
{

public:
    PureSampleManagerForUtiliserManaged() : bHandleManagedPropertyChangedInvoked(false) {}; /* TODO: mark this as noexcept(false) when using newer compiler! */
    virtual ~PureSampleManagerForUtiliserManaged() {};

    PureSampleUtiliserManaged* createManaged()
    {
        PureSampleUtiliserManaged* pNewManaged = PGENULL;
        try
        {
            pNewManaged = new PureSampleUtiliserManaged();
            Attach( *pNewManaged );
        }
        catch (const std::bad_alloc&)
        {
            getConsole().EOLn("ERROR: failed to instantiate PureSampleUtiliserManaged!");
            getConsole().OLn("");
        }

        return pNewManaged;
    } // createManaged()

    virtual void HandleManagedPropertyChanged(PureManaged&)
    {
        bHandleManagedPropertyChangedInvoked = true;
    } // HandleManagedPropertyChanged()

    bool isHandleManagedPropertyChangedInvoked() const
    {
        return bHandleManagedPropertyChangedInvoked;
    }

    void ResetHandleManagedPropertyChangedInvoked()
    {
        bHandleManagedPropertyChangedInvoked = false;
    }

protected:

private:
    bool bHandleManagedPropertyChangedInvoked;

}; // class PureSampleManagerForUtiliserManaged

class PureMaterialTest :
    public UnitTest
{
public:

    PureMaterialTest() :
        UnitTest(__FILE__)
    {
        engine = NULL;
        tm = NULL;
        mat = NULL;
    }

    virtual ~PureMaterialTest()
    {
        Finalize();
    } // ~PureMaterialTest()

protected:

    virtual void Initialize() override
    {
        /*
        CConsole::getConsoleInstance().SetLoggingState(PureMaterial::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureMaterialManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureImage::getLoggerModuleName(), true);
        CConsole::getConsoleInstance().SetLoggingState(PureImageManager::getLoggerModuleName(), true);
        */
        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        engine = &PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler);
        engine->initialize(PURE_RENDERER_HW_FP, 800, 600, PURE_WINDOWED, 0, 32, 24, 0, 0);  // pretty standard display mode, should work on most systems
        tm = &engine->getTextureManager();
        mm = &engine->getMaterialManager();

        AddSubTest("testIsBlendFuncBlends", (PFNUNITSUBTEST) &PureMaterialTest::testIsBlendFuncBlends);
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureMaterialTest::testCtor);
        AddSubTest("testDtor", (PFNUNITSUBTEST) &PureMaterialTest::testDtor);
        AddSubTest("testAllocateArrays", (PFNUNITSUBTEST) &PureMaterialTest::testAllocateArrays);
        AddSubTest("testSetTexture", (PFNUNITSUBTEST) &PureMaterialTest::testSetTexture);
        AddSubTest("testGetTextureCount", (PFNUNITSUBTEST) &PureMaterialTest::testGetTextureCount);
        AddSubTest("testIsTextured", (PFNUNITSUBTEST) &PureMaterialTest::testIsTextured);
        AddSubTest("testIsSingleTextured", (PFNUNITSUBTEST) &PureMaterialTest::testIsSingleTextured);
        AddSubTest("testIsMultiTextured", (PFNUNITSUBTEST) &PureMaterialTest::testIsMultiTextured);
        AddSubTest("testSetSourceBlendFunc", (PFNUNITSUBTEST) &PureMaterialTest::testSetSourceBlendFunc);
        AddSubTest("testSetDestinationBlendFunc", (PFNUNITSUBTEST) &PureMaterialTest::testSetDestinationBlendFunc);
        AddSubTest("testSetBlendFuncs", (PFNUNITSUBTEST) &PureMaterialTest::testSetBlendFuncs);
        AddSubTest("testSetBlendMode", (PFNUNITSUBTEST) &PureMaterialTest::testSetBlendMode);
        AddSubTest("testCopyFromMaterial", (PFNUNITSUBTEST) &PureMaterialTest::testCopyFromMaterial);
        AddSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PureMaterialTest::testGetUsedSystemMemory);

    }

    virtual bool setUp() override
    {
        mat = mm->createMaterial();
        return assertNotNull(mat, "mat null");
    }

    virtual void TearDown() override
    {
        mm->DeleteAll();
        mat = NULL;
    }

    virtual void Finalize() override
    {
        tm = NULL;
        mm = NULL;
        mat = NULL;

        if ( engine )
        {
            engine->shutdown();
            engine = NULL;
        }
        CConsole::getConsoleInstance().SetLoggingState(PureMaterial::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureMaterialManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureTexture::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureTextureManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureImage::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureImageManager::getLoggerModuleName(), false);
    }

private:
    PR00FsUltimateRenderingEngine* engine;
    PureTextureManager* tm;
    PureMaterialManager* mm;
    PureMaterial* mat;
    PGEcfgProfiles cfgProfiles;

    // ---------------------------------------------------------------------------

    PureMaterialTest(const PureMaterialTest&) 
    {};         

    PureMaterialTest& operator=(const PureMaterialTest&)
    {
        return *this;
    };

    bool testIsBlendFuncBlends()
    {
        return assertFalse(PureMaterial::isBlendFuncReallyBlending(PURE_ONE, PURE_ZERO), "(ONE, ZERO)") &
            assertTrue(PureMaterial::isBlendFuncReallyBlending(PURE_ONE, PURE_ONE), "(ONE, ONE)") &
            assertTrue(PureMaterial::isBlendFuncReallyBlending(PURE_SRC_COLOR, PURE_ZERO), "(SRC_COLOR, ZERO)") &
            assertTrue(PureMaterial::isBlendFuncReallyBlending(PURE_DST_COLOR, PURE_ONE), "(DST_COLOR, ONE)") &
            assertTrue(PureMaterial::isBlendFuncReallyBlending(PURE_ZERO, PURE_DST_ALPHA), "(ZERO, DST_ALPHA)") &
            assertTrue(PureMaterial::isBlendFuncReallyBlending(PURE_ONE_MINUS_CONSTANT_COLOR, PURE_ONE), "(ONE_MINUS_CONSTANT_COLOR, ONE)") &
            assertTrue(PureMaterial::isBlendFuncReallyBlending(PURE_CONSTANT_COLOR, PURE_ONE), "(CONSTANT_COLOR, ONE)") &
            assertTrue(PureMaterial::isBlendFuncReallyBlending(PURE_ZERO, PURE_ONE_MINUS_DST_ALPHA), "(ZERO, ONE_MINUS_DST_ALPHA)") &
            assertTrue(PureMaterial::isBlendFuncReallyBlending(PURE_ZERO, PURE_CONSTANT_ALPHA), "(ZERO, CONSTANT_ALPHA)") &
            assertTrue(PureMaterial::isBlendFuncReallyBlending(PURE_SRC_ALPHA_SATURATE, PURE_ONE), "(SRC_ALPHA_SATURATE, ONE)");
        // not checking every combination, no problem
    }

    bool testCtor()
    {
        const PureColor clrWhite(255,255,255,255);

        const TPureUInt nMaxLayers = mm->getMaximumLayerCount();
        bool b = true;
        TPureUInt i = 0;

        while ( i < nMaxLayers )
        {
            // by default no array is allocated
            b &= assertEquals((TPureUInt) 0, mat->getColorsCount(i), "getColorsCount") &
                assertNull(mat->getColors(i), "getColors") &
                assertEquals((TPureUInt)0, mat->getIndicesCount(), "getIndicesCount") &
                assertEquals((TPureUInt)0, mat->getTexcoordsCount(i), "getTexcoordsCount") &
                assertNull(mat->getTexcoords(i), "getTexcoords") &
                assertNull(mat->getTexture(i), "getTexture") &
                assertTrue(clrWhite == mat->getTextureEnvColor(i), "getTextureEnvColor") &
                assertEquals(TPURE_BLENDFACTOR::PURE_ONE, mat->getSourceBlendFunc(i), "getSourceBlendFunc") &
                assertEquals(TPURE_BLENDFACTOR::PURE_ZERO, mat->getDestinationBlendFunc(i), "getDestinationBlendFunc") &
                assertEquals(TPURE_BLENDMODE::PURE_BM_NONE, mat->getBlendMode(i), "getBlendMode");
            i++;
        }
        std::stringstream ss;
        ss << "Failed in " << i << ". iteration" << std::endl;
        assertTrue(b, ss.str().c_str());

        return b &
            assertNotEquals(std::string::npos, mat->getName().find("Material "), "img128x128x24 name") &
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
        const TPureUInt nColors    = 5;
        const TPureUInt nTexCoords = 6;
        const TPureUInt nIndices   = 7;
        const TPureUInt nIndexSize = 4;

        const bool bAlloc = assertTrue(mat->allocateArrays(nColors, nTexCoords, nIndices, nIndexSize), "allocate");

        bool b = true;
        TPureUInt  i = 0;
        while ( i < mm->getMaximumLayerCount() )
        {
            b &= assertEquals(nColors, mat->getColorsCount(i), "getColorsCount") &
                assertNotNull(mat->getColors(i), "getColors") &
                assertEquals(nIndices, mat->getIndicesCount(), "getIndicesCount") &
                assertEquals(nTexCoords, mat->getTexcoordsCount(i), "getTexcoordsCount") &
                assertNotNull(mat->getTexcoords(i), "getTexcoords");
            i++;
        }

        std::stringstream ss;
        ss << "Failed in " << i << ". iteration" << std::endl;

        return bAlloc & assertTrue(b, ss.str().c_str());
    }

    bool testSetTexture()
    {
        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        PureTexture* tex128x128x24 = PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler).getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        bool b = assertTrue(mat->setTexture(tex128x128x24), "setTexture text128x128x24");
        b &= assertNotNull(mat->getTexture(), "1st tex 0") &
            assertNull(mat->getTexture(1), "1st tex 1");

        b &= assertTrue(mat->setTexture(NULL), "setTexture NULL");
        b &= assertTrue(mat->setTexture(tex128x128x24, 1), "setTexture text128x128x24, 1");
        b &= assertNotNull(mat->getTexture(1), "2nd tex 1") &
            assertNull(mat->getTexture(), "2nd tex 0");

        return b;
    }

    bool testGetTextureCount()
    {
        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        PureTexture* tex128x128x24 = PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler).getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        bool b = assertEquals((TPureUInt)0, mat->getTextureCount(), "1st");

        b &= assertTrue(mat->setTexture(tex128x128x24), "setTexture text128x128x24");
        b &= assertEquals((TPureUInt)1, mat->getTextureCount(), "2nd");

        b &= assertTrue(mat->setTexture(NULL), "setTexture NULL");
        b &= assertTrue(mat->setTexture(tex128x128x24, 1), "setTexture text128x128x24, 1");
        b &= assertEquals((TPureUInt)1, mat->getTextureCount(), "3rd");

        b &= assertTrue(mat->setTexture(tex128x128x24), "setTexture text128x128x24");
        b &= assertEquals((TPureUInt)2, mat->getTextureCount(), "4th");

        return b;
    }

    bool testIsTextured()
    {
        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        PureTexture* tex128x128x24 = PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler).getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        bool b = assertFalse(mat->isTextured(), "1st istex");

        b &= assertTrue(mat->setTexture(tex128x128x24), "setTexture text128x128x24");
        b &= assertTrue(mat->isTextured(), "2nd istex");

        b &= assertTrue(mat->setTexture(NULL), "setTexture NULL");
        b &= assertTrue(mat->setTexture(tex128x128x24, 1), "setTexture text128x128x24, 1");
        b &= assertTrue(mat->isTextured(), "3rd istex");

        return b;
    }

    bool testIsSingleTextured()
    {
        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        PureTexture* tex128x128x24 = PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler).getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        bool b = assertFalse(mat->isSingleTextured(), "1st");

        b &= assertTrue(mat->setTexture(tex128x128x24), "setTexture text128x128x24");
        b &= assertTrue(mat->isSingleTextured(), "2nd");

        b &= assertTrue(mat->setTexture(NULL), "setTexture NULL");
        b &= assertTrue(mat->setTexture(tex128x128x24, 1), "setTexture text128x128x24, 1");
        b &= assertTrue(mat->isSingleTextured(), "3rd");

        b &= assertTrue(mat->setTexture(tex128x128x24), "setTexture text128x128x24");
        b &= assertFalse(mat->isSingleTextured(), "4th");

        return b;
    }

    bool testIsMultiTextured()
    {
        PGEInputHandler& inputHandler = PGEInputHandler::createAndGet(cfgProfiles);
        PureTexture* tex128x128x24 = PR00FsUltimateRenderingEngine::createAndGet(cfgProfiles, inputHandler).getTextureManager().createFromFile(BMP128x128x24);
        if ( !tex128x128x24 )
            return assertNotNull(tex128x128x24, "tex");

        bool b = assertFalse(mat->isMultiTextured(), "1st");

        b &= assertTrue(mat->setTexture(tex128x128x24), "setTexture text128x128x24");
        b &= assertFalse(mat->isMultiTextured(), "2nd");

        b &= assertTrue(mat->setTexture(NULL), "setTexture NULL");
        b &= assertTrue(mat->setTexture(tex128x128x24, 1), "setTexture text128x128x24, 1");
        b &= assertFalse(mat->isMultiTextured(), "3rd");

        b &= assertTrue(mat->setTexture(tex128x128x24), "setTexture text128x128x24");
        b &= assertTrue(mat->isMultiTextured(), "4th");

        return b;
    }

    bool testSetSourceBlendFunc()
    {
        PureSampleManagerForUtiliserManaged sampleManager;
        PureSampleUtiliserManaged* sampleUtiliser = sampleManager.createManaged();
        if ( !assertNotNull(sampleUtiliser, "sampleUtiliser not null"))
        {
            return assertNotNull(sampleUtiliser, "sampleUtiliser is NULL");
        }

        mat->SetUtiliser(sampleUtiliser);
        bool b = assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 1");

        b &= assertTrue(mat->setSourceBlendFunc(TPURE_BLENDFACTOR::PURE_CONSTANT_ALPHA), "PURE_CONSTANT_ALPHA");
        b &= assertEquals( TPURE_BLENDFACTOR::PURE_CONSTANT_ALPHA, mat->getSourceBlendFunc(), "1st") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 2");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 3");

        b &= assertTrue(mat->setSourceBlendFunc(TPURE_BLENDFACTOR::PURE_CONSTANT_COLOR, 1), "PURE_CONSTANT_COLOR");
        b &= assertEquals( TPURE_BLENDFACTOR::PURE_CONSTANT_COLOR, mat->getSourceBlendFunc(1), "2nd") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 4");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 5");

        b &= assertFalse(mat->setSourceBlendFunc(TPURE_BLENDFACTOR::PURE_SRC_COLOR), "PURE_SRC_COLOR"); // invalid src param
        b &= assertEquals( TPURE_BLENDFACTOR::PURE_CONSTANT_COLOR, mat->getSourceBlendFunc(1), "3rd") &
            assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 6");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 7");

        b &= assertFalse(mat->setSourceBlendFunc(TPURE_BLENDFACTOR::PURE_ONE_MINUS_SRC_COLOR), "PURE_ONE_MINUS_SRC_COLOR"); // invalid src param
        b &= assertEquals( TPURE_BLENDFACTOR::PURE_CONSTANT_COLOR, mat->getSourceBlendFunc(1), "4th") &
            assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 8");

        return b;
    }

    bool testSetDestinationBlendFunc()
    {
        PureSampleManagerForUtiliserManaged sampleManager;
        PureSampleUtiliserManaged* sampleUtiliser = sampleManager.createManaged();
        if ( !assertNotNull(sampleUtiliser, "sampleUtiliser not null"))
        {
            return assertNotNull(sampleUtiliser, "sampleUtiliser is NULL");
        }

        mat->SetUtiliser(sampleUtiliser);
        bool b = assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 1");

        b &= assertTrue(mat->setDestinationBlendFunc(TPURE_BLENDFACTOR::PURE_CONSTANT_ALPHA), "PURE_CONSTANT_ALPHA");
        b &= assertEquals( TPURE_BLENDFACTOR::PURE_CONSTANT_ALPHA, mat->getDestinationBlendFunc(), "1st") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 2");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 3");

        b &= assertTrue(mat->setDestinationBlendFunc(TPURE_BLENDFACTOR::PURE_CONSTANT_COLOR, 1), "PURE_CONSTANT_COLOR");
        b &= assertEquals( TPURE_BLENDFACTOR::PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "2nd") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 4");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 5");

        b &= assertFalse(mat->setDestinationBlendFunc(TPURE_BLENDFACTOR::PURE_DST_COLOR, 1), "PURE_DST_COLOR"); // invalid dst param
        b &= assertEquals( TPURE_BLENDFACTOR::PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "3rd") &
            assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 6");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 7");

        b &= assertFalse(mat->setDestinationBlendFunc(TPURE_BLENDFACTOR::PURE_ONE_MINUS_DST_COLOR, 1), "PURE_ONE_MINUS_DST_COLOR"); // invalid dst param
        b &= assertEquals( TPURE_BLENDFACTOR::PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "4th") &
            assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 8");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 9");

        b &= assertFalse(mat->setDestinationBlendFunc(TPURE_BLENDFACTOR::PURE_SRC_ALPHA_SATURATE, 1), "PURE_SRC_ALPHA_SATURATE"); // invalid dst param
        b &= assertEquals( TPURE_BLENDFACTOR::PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "5th") &
            assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 10");

        return b;
    }

    bool testSetBlendFuncs()
    {
        PureSampleManagerForUtiliserManaged sampleManager;
        PureSampleUtiliserManaged* sampleUtiliser = sampleManager.createManaged();
        if ( !assertNotNull(sampleUtiliser, "sampleUtiliser not null"))
        {
            return assertNotNull(sampleUtiliser, "sampleUtiliser is NULL");
        }

        mat->SetUtiliser(sampleUtiliser);
        bool b = assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 1");

        b &= assertTrue(mat->setBlendFuncs(PURE_CONSTANT_ALPHA, PURE_CONSTANT_COLOR), "PURE_CONSTANT_ALPHA, PURE_CONSTANT_COLOR");
        b &= assertEquals( PURE_CONSTANT_ALPHA, mat->getSourceBlendFunc(), "1st src") &
            assertEquals( PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(), "1st dst") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 1");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 2");

        b &= assertTrue(mat->setBlendFuncs(PURE_CONSTANT_ALPHA, PURE_CONSTANT_COLOR, 1), "RRE_CONSTANT_ALPHA, PURE_CONSTANT_COLOR, 1");
        b &= assertEquals( PURE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "2nd src") &
            assertEquals( PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "2nd dst") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 3");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 4");

        b &= assertFalse(mat->setBlendFuncs(PURE_SRC_COLOR, PURE_CONSTANT_COLOR, 1), "PURE_SRC_COLOR, PURE_CONSTANT_COLOR, 1"); // invalid src param
        b &= assertEquals( PURE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "3rd src") &
            assertEquals( PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "3rd dst") &
            assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 5"); // srcBlendFunc is invalid, so dstBlendFunc will not be invoked at all thus no call to change handler!

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 6");

        b &= assertFalse(mat->setBlendFuncs(PURE_ONE_MINUS_SRC_COLOR, PURE_CONSTANT_COLOR, 1), "PURE_ONE_MINUS_SRC_COLOR, PURE_CONSTANT_COLOR, 1"); // invalid src param
        b &= assertEquals( PURE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "4th src") &
            assertEquals( PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "4th dst") &
            assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 7"); // srcBlendFunc is invalid, so dstBlendFunc will not be invoked at all thus no call to change handler!

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 8");

        b &= assertFalse(mat->setBlendFuncs(PURE_CONSTANT_ALPHA, PURE_DST_COLOR, 1), "PURE_CONSTANT_ALPHA, PURE_DST_COLOR, 1"); // invalid dst param
        b &= assertEquals( PURE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "5th src") &
            assertEquals( PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "5th dst") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 9"); // srcBlendFunc is valid, so that will be set and will invoke change handler!

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 10");

        b &= assertFalse(mat->setBlendFuncs(PURE_CONSTANT_ALPHA, PURE_ONE_MINUS_DST_COLOR, 1), "PURE_CONSTANT_ALPHA, PURE_ONE_MINUS_DST_COLOR, 1"); // invalid dst param
        b &= assertEquals( PURE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "6th src") &
            assertEquals( PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "6th dst") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 11"); // srcBlendFunc is valid, so that will be set and will invoke change handler!

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 12");

        b &= assertFalse(mat->setBlendFuncs(PURE_CONSTANT_ALPHA, PURE_SRC_ALPHA_SATURATE, 1), "PURE_CONSTANT_ALPHA, PURE_SRC_ALPHA_SATURATE, 1"); // invalid dst param
        b &= assertEquals( PURE_CONSTANT_ALPHA, mat->getSourceBlendFunc(1), "7th src") &
            assertEquals( PURE_CONSTANT_COLOR, mat->getDestinationBlendFunc(1), "7th dst") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 13"); // srcBlendFunc is valid, so that will be set and will invoke change handler!

        return b;
    }

    bool testSetBlendMode()
    {
        PureSampleManagerForUtiliserManaged sampleManager;
        PureSampleUtiliserManaged* sampleUtiliser = sampleManager.createManaged();
        if ( !assertNotNull(sampleUtiliser, "sampleUtiliser"))
        {
            return assertNotNull(sampleUtiliser, "sampleUtiliser");
        }

        mat->SetUtiliser(sampleUtiliser);
        bool b = assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 1");

        b &= assertTrue(mat->setBlendMode(TPURE_BLENDMODE::PURE_BM_STANDARD_TRANSPARENCY), "PURE_BM_STANDARD_TRANSPARENCY");
        b &= assertEquals( PURE_BM_STANDARD_TRANSPARENCY, mat->getBlendMode(), "1st mode") & 
            assertEquals( PURE_SRC_ALPHA, mat->getSourceBlendFunc(), "1st src") &
            assertEquals( PURE_ONE_MINUS_SRC_ALPHA, mat->getDestinationBlendFunc(), "1st dst") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 2");

        sampleManager.ResetHandleManagedPropertyChangedInvoked();
        b &= assertFalse(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 3");

        b &= assertTrue(mat->setBlendMode(TPURE_BLENDMODE::PURE_BM_NONE, 1), "PURE_BM_NONE, 1");
        b &= assertEquals( PURE_BM_NONE, mat->getBlendMode(1), "2nd mode") & 
            assertEquals( PURE_ONE, mat->getSourceBlendFunc(1), "2nd src") &
            assertEquals( PURE_ZERO, mat->getDestinationBlendFunc(1), "2nd dst") &
            assertTrue(sampleManager.isHandleManagedPropertyChangedInvoked(), "propChangedInvoked 4");

        return b;
    }

    bool testCopyFromMaterial()
    {
        PureMaterial* mat2 = mm->createMaterial();
        if ( !mat2 )
            return assertNotNull(mat2, "mat2");
        
        const bool bAllocMat2 = assertTrue(mat2->allocateArrays(2, 2, 2, 2), "allocate mat2");
        mat2->getColors()[0].green = 1.0f;
        mat2->getTexcoords()[1].u = 1.0f;
 
        bool b = assertFalse(mat->copyFromMaterial(*mat2, 0, 0), "1st");  // should fail due to non-matching array layout
       
        // make sure nothing has been changed in mat
        TPureUInt i = 0;
        while ( i < mm->getMaximumLayerCount() )
        {
            b &= assertEquals((TPureUInt) 0, mat->getColorsCount(i), "getColorsCount") &
                assertNull(mat->getColors(i), "getColors") &
                assertEquals((TPureUInt)0, mat->getIndicesCount(), "getIndicesCount") &
                assertEquals((TPureUInt)0, mat->getTexcoordsCount(i), "getTexcoordsCount") &
                assertNull(mat->getTexcoords(i), "getTexcoords");
            i++;
        }
        std::stringstream ss;
        ss << "Failed in " << i << ". iteration" << std::endl;
        assertTrue(b, ss.str().c_str());

        const bool bAllocMat = assertTrue(mat->allocateArrays(2, 2, 2, 2), "allocate mat");

        b &= assertTrue(mat->copyFromMaterial(*mat2, 0, 0), "2nd");  // should succeed now

        // sanity check for the layout before checking actual values in arrays
        i = 0;
        while ( i < mm->getMaximumLayerCount() )
        {
            b &= assertEquals((TPureUInt) 2, mat->getColorsCount(i), "getColorsCount") &
                assertNotNull(mat->getColors(i), "getColors") &
                assertEquals((TPureUInt)2, mat->getIndicesCount(), "getIndicesCount") &
                assertEquals((TPureUInt)2, mat->getTexcoordsCount(i), "getTexcoordsCount") &
                assertNotNull(mat->getTexcoords(i), "getTexcoords");
            i++;
        }
        ss.clear();
        ss << "Failed in " << i << ". iteration" << std::endl;
        assertTrue(b, ss.str().c_str());

        // we should have the same values in the 1st material too 
        b &= assertEquals(1.0f, mat->getColors()[0].green, "clr lvl0 green") &
            assertEquals(1.0f, mat->getTexcoords()[1].u, "texcoord lvl1 U");

        return b & bAllocMat & bAllocMat2;
    }

    bool testGetUsedSystemMemory()
    {
        return assertGreater(mat->getUsedSystemMemory(), sizeof(PureMaterial));
    }

}; // class PureMaterialTest