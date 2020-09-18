#pragma once

/*
    ###################################################################################
    PRREColorTest.h
    Unit test for PRREColor.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/Material/PRREColor.h"
#include <sstream>

#ifndef E2
#define E2 0.005f
#endif // EPSILON

class PRREColorTest :
    public UnitTest
{
public:

    PRREColorTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREColorTest::testCtor1);
        AddSubTest("testCtor2", (PFNUNITSUBTEST) &PRREColorTest::testCtor2);
        AddSubTest("testCopyCtor", (PFNUNITSUBTEST) &PRREColorTest::testCopyCtor);
        AddSubTest("testGetRed", (PFNUNITSUBTEST) &PRREColorTest::testGetRed);
        AddSubTest("testGetGreen", (PFNUNITSUBTEST) &PRREColorTest::testGetGreen);
        AddSubTest("testGetBlue", (PFNUNITSUBTEST) &PRREColorTest::testGetBlue);
        AddSubTest("testGetAlpha", (PFNUNITSUBTEST) &PRREColorTest::testGetAlpha);
        AddSubTest("testGetRedAsFloat", (PFNUNITSUBTEST) &PRREColorTest::testGetRedAsFloat);
        AddSubTest("testGetGreenAsFloat", (PFNUNITSUBTEST) &PRREColorTest::testGetGreenAsFloat);
        AddSubTest("testGetBlueAsFloat", (PFNUNITSUBTEST) &PRREColorTest::testGetBlueAsFloat);
        AddSubTest("testGetAlphaAsFloat", (PFNUNITSUBTEST) &PRREColorTest::testGetAlphaAsFloat);
        AddSubTest("testSet", (PFNUNITSUBTEST) &PRREColorTest::testSet);
        AddSubTest("testSetf", (PFNUNITSUBTEST) &PRREColorTest::testSetf);
        AddSubTest("testSetRed", (PFNUNITSUBTEST) &PRREColorTest::testSetRed);
        AddSubTest("testSetRedf", (PFNUNITSUBTEST) &PRREColorTest::testSetRedf);
        AddSubTest("testSetGreen", (PFNUNITSUBTEST) &PRREColorTest::testSetGreen);
        AddSubTest("testSetGreenf", (PFNUNITSUBTEST) &PRREColorTest::testSetGreenf);
        AddSubTest("testSetBlue", (PFNUNITSUBTEST) &PRREColorTest::testSetBlue);
        AddSubTest("testSetBluef", (PFNUNITSUBTEST) &PRREColorTest::testSetBluef);
        AddSubTest("testSetAlpha", (PFNUNITSUBTEST) &PRREColorTest::testSetAlpha);
        AddSubTest("testSetAlphaf", (PFNUNITSUBTEST) &PRREColorTest::testSetAlphaf);
        AddSubTest("testOperatorEquals", (PFNUNITSUBTEST) &PRREColorTest::testOperatorEquals);
        AddSubTest("testOperatorNotEquals", (PFNUNITSUBTEST) &PRREColorTest::testOperatorNotEquals);
        AddSubTest("testOperatorAssignment", (PFNUNITSUBTEST) &PRREColorTest::testOperatorAssignment);
        AddSubTest("testOperatorWriteToStream", (PFNUNITSUBTEST) &PRREColorTest::testOperatorWriteToStream);

    } // PRREColorTest()

protected:

    virtual bool setUp()
    {
        return true;
    }

    virtual void TearDown()
    {

    }

private:

    // ---------------------------------------------------------------------------

    PRREColorTest(const PRREColorTest&)
    {};         

    PRREColorTest& operator=(const PRREColorTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PRREColor clr;

        return assertEquals(0, (int) clr.getRed(), "red") &
            assertEquals(0, (int) clr.getGreen(), "green") &
            assertEquals(0, (int) clr.getBlue(), "blue") &
            assertEquals(0, (int) clr.getAlpha(), "alpha");        
    }

    bool testCtor2()
    {
        const PRREColor clr(1,2,3,4);

        return assertEquals(1, (int) clr.getRed(), "red") &
            assertEquals(2, (int) clr.getGreen(), "green") &
            assertEquals(3, (int) clr.getBlue(), "blue") &
            assertEquals(4, (int) clr.getAlpha(), "alpha");        
    }

    bool testCopyCtor()
    {
        const PRREColor clr(1,2,3,4);
        const PRREColor clr2(clr);

        return assertEquals(clr, clr2);
    }

    bool testGetRed()
    {
        const PRREColor clr(1,2,3,4);

        return assertEquals(1, clr.getRed());
    }

    bool testGetGreen()
    {
        const PRREColor clr(1,2,3,4);

        return assertEquals(2, clr.getGreen());
    }

    bool testGetBlue()
    {
        const PRREColor clr(1,2,3,4);

        return assertEquals(3, clr.getBlue());
    }

    bool testGetAlpha()
    {
        const PRREColor clr(1,2,3,4);

        return assertEquals(4, clr.getAlpha());
    }

    bool testGetRedAsFloat()
    {
        PRREColor clr;
        clr.SetAsFloats(0.1f, 0.2f, 0.3f, 0.4f);

        return assertEquals(0.1f, clr.getRedAsFloat(), E2);
    }

    bool testGetGreenAsFloat()
    {
        PRREColor clr;
        clr.SetAsFloats(0.1f, 0.2f, 0.3f, 0.4f);

        return assertEquals(0.2f, clr.getGreenAsFloat(), E2);
    }

    bool testGetBlueAsFloat()
    {
        PRREColor clr;
        clr.SetAsFloats(0.1f, 0.2f, 0.3f, 0.4f);

        return assertEquals(0.3f, clr.getBlueAsFloat(), E2);
    }

    bool testGetAlphaAsFloat()
    {
        PRREColor clr;
        clr.SetAsFloats(0.1f, 0.2f, 0.3f, 0.4f);

        return assertEquals(0.4f, clr.getAlphaAsFloat(), E2);
    }

    bool testSet()
    {
        PRREColor clr;
        clr.Set(1, 2, 3, 4);

        return assertEquals(1, (int) clr.getRed(), "red") &
            assertEquals(2, (int) clr.getGreen(), "green") &
            assertEquals(3, (int) clr.getBlue(), "blue") &
            assertEquals(4, (int) clr.getAlpha(), "alpha");
    }

    bool testSetf()
    {
        PRREColor clr;
        clr.SetAsFloats(-2.0f, 0.2f, 0.3f, 3.0f);

        return assertEquals(0.0f, clr.getRedAsFloat(), E2, "red") &
            assertEquals(0.2f, clr.getGreenAsFloat(), E2, "green") &
            assertEquals(0.3f, clr.getBlueAsFloat(), E2, "blue") &
            assertEquals(1.0f, clr.getAlphaAsFloat(), E2, "alpha");
    }

    bool testSetRed()
    {
        PRREColor clr;
        clr.SetRed(1);

        return assertEquals(1, clr.getRed());
    }

    bool testSetRedf()
    {
        PRREColor clr;
        clr.SetRedAsFloat(0.5f);

        return assertEquals(0.5f, clr.getRedAsFloat(), E2);
    }

    bool testSetGreen()
    {
        PRREColor clr;
        clr.SetGreen(1);

        return assertEquals(1, clr.getGreen());
    }

    bool testSetGreenf()
    {
        PRREColor clr;
        clr.SetGreenAsFloat(0.5f);

        return assertEquals(0.5f, clr.getGreenAsFloat(), E2);
    }

    bool testSetBlue()
    {
        PRREColor clr;
        clr.SetBlue(1);

        return assertEquals(1, clr.getBlue());
    }

    bool testSetBluef()
    {
        PRREColor clr;
        clr.SetBlueAsFloat(0.5f);

        return assertEquals(0.5f, clr.getBlueAsFloat(), E2);
    }

    bool testSetAlpha()
    {
        PRREColor clr;
        clr.SetAlpha(1);

        return assertEquals(1, clr.getAlpha());
    }
    
    bool testSetAlphaf()
    {
        PRREColor clr;
        clr.SetAlphaAsFloat(0.5f);

        return assertEquals(0.5f, clr.getAlphaAsFloat(), E2);
    }

    bool testOperatorEquals()
    {
        const PRREColor clr1(1,2,3,4);
        const PRREColor clr2(1,2,3,4);

        return assertEquals(clr1, clr2);
    }

    bool testOperatorNotEquals()
    {
        const PRREColor clr1(1,2,3,4);
        const PRREColor clr2(1,2,3,5);

        return assertNotEquals(clr1, clr2);
    }

    bool testOperatorAssignment()
    {
        const PRREColor clr1(1,2,3,4);
        PRREColor clr2 = clr1;

        return assertEquals(clr1, clr2);
    }

    bool testOperatorWriteToStream()
    {
        PRREColor clr(200, 150, 100, 50);
        std::stringstream ssExpected, ssTested;
        ssExpected << "[200,150,100,50]";
        ssTested << clr;

        return assertEquals(ssExpected.str(), ssTested.str());
    }


}; // class PRREColorTest