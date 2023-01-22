#pragma once

/*
    ###################################################################################
    PureFiledManagerTest.h
    Unit test for PureFiledManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PureFiledManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class TestDummy :
    public PureFiledManaged
{
public:

    TestDummy()
    {
        SetFilename("alma");
    }
}; // class TestDummy


class PureFiledManagerTest :
    public UnitTest
{
public:

    PureFiledManagerTest() :
        UnitTest( __FILE__ )
    {
        Initialize();
    } // PureFiledManagerTest()

    virtual ~PureFiledManagerTest()
    {
        Finalize();
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureFiledManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureFiledManaged::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureManaged::getLoggerModuleName(), true);
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureFiledManagerTest::testCtor);
        AddSubTest("testGetByFilename", (PFNUNITSUBTEST) &PureFiledManagerTest::testGetByFilename);
        AddSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PureFiledManagerTest::testCreateFromFile);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PureFiledManagerTest::testWriteList);
    }

    virtual bool setUp()
    {
        return true;
    }

    virtual void TearDown()
    {
        Finalize();
    }

    virtual void Finalize()
    {
        CConsole::getConsoleInstance().SetLoggingState(PureFiledManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureFiledManaged::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureManaged::getLoggerModuleName(), false);
    }

private:

    PureFiledManagerTest(const PureFiledManagerTest&)
    {};         

    PureFiledManagerTest& operator=(const PureFiledManagerTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PureFiledManager mgr;
        return true;
    }

    bool testGetByFilename()
    {
        PureFiledManager mgr;
        TestDummy dummy;
        mgr.Attach(dummy);

        return assertEquals(&dummy, mgr.getByFilename("alma"), "dummy") &
            assertNull(mgr.getByFilename("asdasdlolol"), "null");
    }

    bool testCreateFromFile()
    {
        PureFiledManager mgr;

        return assertNull(mgr.createFromFile("asdasdlolol"));
    }

    bool testWriteList()
    {
        const PureFiledManager mgr;
        mgr.WriteList();

        return true;
    }


}; // class PureFiledManagedTest