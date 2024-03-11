#pragma once

/*
    ###################################################################################
    PureColorTest.h
    Unit test for PureColor.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Material/PureColor.h"
#include <sstream>

#ifndef E2
#define E2 0.005f
#endif // EPSILON

class PureColorTest :
    public UnitTest
{
public:

    PureColorTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PureColorTest::testCtor1);
        AddSubTest("testCtor2", (PFNUNITSUBTEST) &PureColorTest::testCtor2);
        AddSubTest("testCopyCtor", (PFNUNITSUBTEST) &PureColorTest::testCopyCtor);
        AddSubTest("testGetRed", (PFNUNITSUBTEST) &PureColorTest::testGetRed);
        AddSubTest("testGetGreen", (PFNUNITSUBTEST) &PureColorTest::testGetGreen);
        AddSubTest("testGetBlue", (PFNUNITSUBTEST) &PureColorTest::testGetBlue);
        AddSubTest("testGetAlpha", (PFNUNITSUBTEST) &PureColorTest::testGetAlpha);
        AddSubTest("testGetRedAsFloat", (PFNUNITSUBTEST) &PureColorTest::testGetRedAsFloat);
        AddSubTest("testGetGreenAsFloat", (PFNUNITSUBTEST) &PureColorTest::testGetGreenAsFloat);
        AddSubTest("testGetBlueAsFloat", (PFNUNITSUBTEST) &PureColorTest::testGetBlueAsFloat);
        AddSubTest("testGetAlphaAsFloat", (PFNUNITSUBTEST) &PureColorTest::testGetAlphaAsFloat);
        AddSubTest("testSet", (PFNUNITSUBTEST) &PureColorTest::testSet);
        AddSubTest("testSetf", (PFNUNITSUBTEST) &PureColorTest::testSetf);
        AddSubTest("testSetRed", (PFNUNITSUBTEST) &PureColorTest::testSetRed);
        AddSubTest("testSetRedf", (PFNUNITSUBTEST) &PureColorTest::testSetRedf);
        AddSubTest("testSetGreen", (PFNUNITSUBTEST) &PureColorTest::testSetGreen);
        AddSubTest("testSetGreenf", (PFNUNITSUBTEST) &PureColorTest::testSetGreenf);
        AddSubTest("testSetBlue", (PFNUNITSUBTEST) &PureColorTest::testSetBlue);
        AddSubTest("testSetBluef", (PFNUNITSUBTEST) &PureColorTest::testSetBluef);
        AddSubTest("testSetAlpha", (PFNUNITSUBTEST) &PureColorTest::testSetAlpha);
        AddSubTest("testSetAlphaf", (PFNUNITSUBTEST) &PureColorTest::testSetAlphaf);
        AddSubTest("testOperatorEquals", (PFNUNITSUBTEST) &PureColorTest::testOperatorEquals);
        AddSubTest("testOperatorNotEquals", (PFNUNITSUBTEST) &PureColorTest::testOperatorNotEquals);
        AddSubTest("testOperatorAssignment", (PFNUNITSUBTEST) &PureColorTest::testOperatorAssignment);
        AddSubTest("testOperatorWriteToStream", (PFNUNITSUBTEST) &PureColorTest::testOperatorWriteToStream);

    } // PureColorTest()

protected:

    virtual bool setUp() override
    {
        return true;
    }

    virtual void TearDown() override
    {

    }

private:

    // ---------------------------------------------------------------------------

    PureColorTest(const PureColorTest&)
    {};         

    PureColorTest& operator=(const PureColorTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PureColor clr;

        return assertEquals(0, (int) clr.getRed(), "red") &
            assertEquals(0, (int) clr.getGreen(), "green") &
            assertEquals(0, (int) clr.getBlue(), "blue") &
            assertEquals(0, (int) clr.getAlpha(), "alpha");        
    }

    bool testCtor2()
    {
        const PureColor clr(1,2,3,4);

        return assertEquals(1, (int) clr.getRed(), "red") &
            assertEquals(2, (int) clr.getGreen(), "green") &
            assertEquals(3, (int) clr.getBlue(), "blue") &
            assertEquals(4, (int) clr.getAlpha(), "alpha");        
    }

    bool testCopyCtor()
    {
        const PureColor clr(1,2,3,4);
        const PureColor clr2(clr);

        return assertEquals(clr, clr2);
    }

    bool testGetRed()
    {
        const PureColor clr(1,2,3,4);

        return assertEquals(1, clr.getRed());
    }

    bool testGetGreen()
    {
        const PureColor clr(1,2,3,4);

        return assertEquals(2, clr.getGreen());
    }

    bool testGetBlue()
    {
        const PureColor clr(1,2,3,4);

        return assertEquals(3, clr.getBlue());
    }

    bool testGetAlpha()
    {
        const PureColor clr(1,2,3,4);

        return assertEquals(4, clr.getAlpha());
    }

    bool testGetRedAsFloat()
    {
        PureColor clr;
        clr.SetAsFloats(0.1f, 0.2f, 0.3f, 0.4f);

        return assertEquals(0.1f, clr.getRedAsFloat(), E2);
    }

    bool testGetGreenAsFloat()
    {
        PureColor clr;
        clr.SetAsFloats(0.1f, 0.2f, 0.3f, 0.4f);

        return assertEquals(0.2f, clr.getGreenAsFloat(), E2);
    }

    bool testGetBlueAsFloat()
    {
        PureColor clr;
        clr.SetAsFloats(0.1f, 0.2f, 0.3f, 0.4f);

        return assertEquals(0.3f, clr.getBlueAsFloat(), E2);
    }

    bool testGetAlphaAsFloat()
    {
        PureColor clr;
        clr.SetAsFloats(0.1f, 0.2f, 0.3f, 0.4f);

        return assertEquals(0.4f, clr.getAlphaAsFloat(), E2);
    }

    bool testSet()
    {
        PureColor clr;
        clr.Set(1, 2, 3, 4);

        return assertEquals(1, (int) clr.getRed(), "red") &
            assertEquals(2, (int) clr.getGreen(), "green") &
            assertEquals(3, (int) clr.getBlue(), "blue") &
            assertEquals(4, (int) clr.getAlpha(), "alpha");
    }

    bool testSetf()
    {
        PureColor clr;
        clr.SetAsFloats(-2.0f, 0.2f, 0.3f, 3.0f);

        return assertEquals(0.0f, clr.getRedAsFloat(), E2, "red") &
            assertEquals(0.2f, clr.getGreenAsFloat(), E2, "green") &
            assertEquals(0.3f, clr.getBlueAsFloat(), E2, "blue") &
            assertEquals(1.0f, clr.getAlphaAsFloat(), E2, "alpha");
    }

    bool testSetRed()
    {
        PureColor clr;
        clr.SetRed(1);

        return assertEquals(1, clr.getRed());
    }

    bool testSetRedf()
    {
        PureColor clr;
        clr.SetRedAsFloat(0.5f);

        return assertEquals(0.5f, clr.getRedAsFloat(), E2);
    }

    bool testSetGreen()
    {
        PureColor clr;
        clr.SetGreen(1);

        return assertEquals(1, clr.getGreen());
    }

    bool testSetGreenf()
    {
        PureColor clr;
        clr.SetGreenAsFloat(0.5f);

        return assertEquals(0.5f, clr.getGreenAsFloat(), E2);
    }

    bool testSetBlue()
    {
        PureColor clr;
        clr.SetBlue(1);

        return assertEquals(1, clr.getBlue());
    }

    bool testSetBluef()
    {
        PureColor clr;
        clr.SetBlueAsFloat(0.5f);

        return assertEquals(0.5f, clr.getBlueAsFloat(), E2);
    }

    bool testSetAlpha()
    {
        PureColor clr;
        clr.SetAlpha(1);

        return assertEquals(1, clr.getAlpha());
    }
    
    bool testSetAlphaf()
    {
        PureColor clr;
        clr.SetAlphaAsFloat(0.5f);

        return assertEquals(0.5f, clr.getAlphaAsFloat(), E2);
    }

    bool testOperatorEquals()
    {
        const PureColor clr1(1,2,3,4);
        const PureColor clr2(1,2,3,4);

        return assertEquals(clr1, clr2);
    }

    bool testOperatorNotEquals()
    {
        const PureColor clr1(1,2,3,4);
        const PureColor clr2(1,2,3,5);

        return assertNotEquals(clr1, clr2);
    }

    bool testOperatorAssignment()
    {
        const PureColor clr1(1,2,3,4);
        PureColor clr2 = clr1;

        return assertEquals(clr1, clr2);
    }

    bool testOperatorWriteToStream()
    {
        PureColor clr(200, 150, 100, 50);
        std::stringstream ssExpected, ssTested;
        ssExpected << "[200,150,100,50]";
        ssTested << clr;

        return assertEquals(ssExpected.str(), ssTested.str());
    }


}; // class PureColorTest