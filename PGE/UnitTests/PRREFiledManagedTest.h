#pragma once

/*
    ###################################################################################
    PRREFiledManagedTest.h
    Unit test for PRREFiledManaged.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/PRREFiledManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class TestDummy2 :
    public PRREFiledManaged
{
public:

    TestDummy2()
    {
        SetFilename("alma");
    }
}; // class TestDummy2


class PRREFiledManagedTest :
    public UnitTest
{
public:

    PRREFiledManagedTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREFiledManagedTest::testCtor);
        AddSubTest("testSubclass", (PFNUNITSUBTEST) &PRREFiledManagedTest::testSubclass);
    } // PRREFiledManagedTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PRREFiledManagedTest(const PRREFiledManagedTest&)
    {};         

    PRREFiledManagedTest& operator=(const PRREFiledManagedTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PRREFiledManaged obj;
        return assertTrue(obj.getFilename().empty());
    }

    bool testSubclass()
    {
        const TestDummy2 obj;
        return assertEquals("alma", obj.getFilename());
    }


}; // class PRREFiledManagedTest
