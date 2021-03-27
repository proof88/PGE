#pragma once

/*
    ###################################################################################
    PRREFiledManagerTest.h
    Unit test for PRREFiledManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/PRREFiledManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class TestDummy :
    public PRREFiledManaged
{
public:

    TestDummy()
    {
        SetFilename("alma");
    }
}; // class TestDummy


class PRREFiledManagerTest :
    public UnitTest
{
public:

    PRREFiledManagerTest() :
        UnitTest( __FILE__ )
    {
        Initialize();
    } // PRREFiledManagerTest()

    virtual ~PRREFiledManagerTest()
    {
        Finalize();
    }

protected:

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PRREFiledManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREFiledManaged::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREManaged::getLoggerModuleName(), true);
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREFiledManagerTest::testCtor);
        AddSubTest("testGetByFilename", (PFNUNITSUBTEST) &PRREFiledManagerTest::testGetByFilename);
        AddSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PRREFiledManagerTest::testCreateFromFile);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREFiledManagerTest::testWriteList);
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
        CConsole::getConsoleInstance().SetLoggingState(PRREFiledManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREFiledManaged::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREManaged::getLoggerModuleName(), false);
    }

private:

    PRREFiledManagerTest(const PRREFiledManagerTest&)
    {};         

    PRREFiledManagerTest& operator=(const PRREFiledManagerTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PRREFiledManager mgr;
        return true;
    }

    bool testGetByFilename()
    {
        PRREFiledManager mgr;
        TestDummy dummy;
        mgr.Attach(dummy);

        return assertEquals(&dummy, mgr.getByFilename("alma"), "dummy") &
            assertNull(mgr.getByFilename("asdasdlolol"), "null");
    }

    bool testCreateFromFile()
    {
        PRREFiledManager mgr;

        return assertNull(mgr.createFromFile("asdasdlolol"));
    }

    bool testWriteList()
    {
        const PRREFiledManager mgr;
        mgr.WriteList();

        return true;
    }


}; // class PRREFiledManagedTest