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
        addSubTest("testCtor1", (PFNUNITSUBTEST) &PureColorTest::testCtor1);
        addSubTest("testCtor2", (PFNUNITSUBTEST) &PureColorTest::testCtor2);
        addSubTest("testCopyCtor", (PFNUNITSUBTEST) &PureColorTest::testCopyCtor);
        addSubTest("testGetRed", (PFNUNITSUBTEST) &PureColorTest::testGetRed);
        addSubTest("testGetGreen", (PFNUNITSUBTEST) &PureColorTest::testGetGreen);
        addSubTest("testGetBlue", (PFNUNITSUBTEST) &PureColorTest::testGetBlue);
        addSubTest("testGetAlpha", (PFNUNITSUBTEST) &PureColorTest::testGetAlpha);
        addSubTest("testGetRedAsFloat", (PFNUNITSUBTEST) &PureColorTest::testGetRedAsFloat);
        addSubTest("testGetGreenAsFloat", (PFNUNITSUBTEST) &PureColorTest::testGetGreenAsFloat);
        addSubTest("testGetBlueAsFloat", (PFNUNITSUBTEST) &PureColorTest::testGetBlueAsFloat);
        addSubTest("testGetAlphaAsFloat", (PFNUNITSUBTEST) &PureColorTest::testGetAlphaAsFloat);
        addSubTest("testSet", (PFNUNITSUBTEST) &PureColorTest::testSet);
        addSubTest("testSetf", (PFNUNITSUBTEST) &PureColorTest::testSetf);
        addSubTest("testSetRed", (PFNUNITSUBTEST) &PureColorTest::testSetRed);
        addSubTest("testSetRedf", (PFNUNITSUBTEST) &PureColorTest::testSetRedf);
        addSubTest("testSetGreen", (PFNUNITSUBTEST) &PureColorTest::testSetGreen);
        addSubTest("testSetGreenf", (PFNUNITSUBTEST) &PureColorTest::testSetGreenf);
        addSubTest("testSetBlue", (PFNUNITSUBTEST) &PureColorTest::testSetBlue);
        addSubTest("testSetBluef", (PFNUNITSUBTEST) &PureColorTest::testSetBluef);
        addSubTest("testSetAlpha", (PFNUNITSUBTEST) &PureColorTest::testSetAlpha);
        addSubTest("testSetAlphaf", (PFNUNITSUBTEST) &PureColorTest::testSetAlphaf);
        addSubTest("testOperatorEquals", (PFNUNITSUBTEST) &PureColorTest::testOperatorEquals);
        addSubTest("testOperatorNotEquals", (PFNUNITSUBTEST) &PureColorTest::testOperatorNotEquals);
        addSubTest("testOperatorAssignment", (PFNUNITSUBTEST) &PureColorTest::testOperatorAssignment);
        addSubTest("testOperatorWriteToStream", (PFNUNITSUBTEST) &PureColorTest::testOperatorWriteToStream);

    } // PureColorTest()

protected:

    virtual bool setUp() override
    {
        return true;
    }

    virtual void tearDown() override
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