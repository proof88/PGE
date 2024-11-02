#pragma once

/*
    ###################################################################################
    PureFiledManagerTest.h
    Unit test for PureFiledManager.
    Made by PR00F88
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
        initialize();
    } // PureFiledManagerTest()

    virtual ~PureFiledManagerTest()
    {
        finalize();
    }

protected:

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureFiledManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureFiledManaged::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureManaged::getLoggerModuleName(), true);
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureFiledManagerTest::testCtor);
        addSubTest("testGetByFilename", (PFNUNITSUBTEST) &PureFiledManagerTest::testGetByFilename);
        addSubTest("testCreateFromFile", (PFNUNITSUBTEST) &PureFiledManagerTest::testCreateFromFile);
        addSubTest("testWriteList", (PFNUNITSUBTEST) &PureFiledManagerTest::testWriteList);
    }

    virtual bool setUp() override
    {
        return true;
    }

    virtual void tearDown() override
    {
        finalize();
    }

    virtual void finalize() override
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