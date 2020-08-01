#pragma once

/*
    ###################################################################################
    PRREBaseClassTest.h
    Unit test for PRREBaseClass.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/PRREBaseClass.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREBaseClassTest :
    public UnitTest
{
public:

    PRREBaseClassTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREBaseClassTest::testCtor1);
    } // PRREBaseClassTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PRREBaseClassTest(const PRREBaseClassTest&)
    {};         

    PRREBaseClassTest& operator=(const PRREBaseClassTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PRREBaseClass obj;
        return true;
    }

}; // class PRREBaseClassTest
