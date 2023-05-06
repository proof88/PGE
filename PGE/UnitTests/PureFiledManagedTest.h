#pragma once

/*
    ###################################################################################
    PureFiledManagedTest.h
    Unit test for PureFiledManaged.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PureFiledManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class TestDummy2 :
    public PureFiledManaged
{
public:

    TestDummy2()
    {
        SetFilename("alma");
    }
}; // class TestDummy2


class PureFiledManagedTest :
    public UnitTest
{
public:

    PureFiledManagedTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureFiledManagedTest::testCtor);
        AddSubTest("testSubclass", (PFNUNITSUBTEST) &PureFiledManagedTest::testSubclass);
    } // PureFiledManagedTest()

    virtual ~PureFiledManagedTest()
    {
        
    }

    virtual void Initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureFiledManaged::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureManaged::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureFiledManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureManager::getLoggerModuleName(), true);
    }

    virtual void Finalize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(PureFiledManaged::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureManaged::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureFiledManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureManager::getLoggerModuleName(), false);   
    }

protected:

private:

    // ---------------------------------------------------------------------------

    PureFiledManagedTest(const PureFiledManagedTest&)
    {};         

    PureFiledManagedTest& operator=(const PureFiledManagedTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PureFiledManaged obj;
        return assertTrue(obj.getFilename().empty());
    }

    bool testSubclass()
    {
        const TestDummy2 obj;
        return assertEquals("alma", obj.getFilename());
    }


}; // class PureFiledManagedTest
