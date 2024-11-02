#pragma once

/*
    ###################################################################################
    PureManagedTest.h
    Unit test for PureManaged.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PureManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureManagedTest :
    public UnitTest
{
public:

    PureManagedTest() :
        UnitTest( __FILE__ )
    {
        
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureManagedTest::testCtor);
        addSubTest("testGetUtiliser", (PFNUNITSUBTEST) &PureManagedTest::testGetUtiliser);
        addSubTest("testSetUtiliser", (PFNUNITSUBTEST) &PureManagedTest::testSetUtiliser);
        addSubTest("testDetachFrom", (PFNUNITSUBTEST) &PureManagedTest::testDetachFrom);
        addSubTest("testGetManager", (PFNUNITSUBTEST) &PureManagedTest::testGetManager);
        addSubTest("testGetName", (PFNUNITSUBTEST) &PureManagedTest::testGetName);
        addSubTest("testSetName", (PFNUNITSUBTEST) &PureManagedTest::testSetName);
        addSubTest("testFlushResources", (PFNUNITSUBTEST) &PureManagedTest::testFlushResources);
        addSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PureManagedTest::testGetUsedSystemMemory);
    } // PureManagedTest()

    virtual ~PureManagedTest()
    {
        
    }

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureManaged::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureManager::getLoggerModuleName(), true);
    }

    virtual void finalize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(PureManaged::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureManager::getLoggerModuleName(), false);    
    }

protected:

private:

    // ---------------------------------------------------------------------------

    PureManagedTest(const PureManagedTest&)
    {};         

    PureManagedTest& operator=(const PureManagedTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PureManaged mgd1;
        return true;
    }

    bool testGetUtiliser()
    {
        const PureManaged mgd1;
        return assertNull(mgd1.getUtiliser(), "getUtiliser()");
    }

    bool testSetUtiliser()
    {
        PureManaged mgd1;
        PureManaged mgd2;

        // cannot set self
        mgd1.SetUtiliser(&mgd1);
        bool b = assertNull(mgd1.getUtiliser(), "getUtiliser() 1");

        mgd1.SetUtiliser(&mgd2);
        b &= assertEquals(&mgd2, mgd1.getUtiliser(), "getUtiliser() 2");

        return b;
    }

    bool testDetachFrom()
    {
        PureManager mgr;
        PureManaged mgd;
        mgd.DetachFrom();
        mgr.Attach(mgd);
        mgd.DetachFrom();

        return assertEquals(0, mgr.getCount(), "getCount()") &
            assertNull(mgd.getManager(), "getManager()");
    }

    bool testGetManager()
    {
        PureManager mgr;
        PureManaged mgd;

        bool l = assertNull(mgd.getManager(), "getManager() 1");

        mgr.Attach(mgd);
        l &= assertEquals(&mgr, mgd.getManager(), "getManager() 2");

        return l;
    }

    bool testGetName()
    {
        PureManaged mgd;

        return assertEquals("", mgd.getName());
    }

    bool testSetName()
    {
        PureManaged mgd;
        mgd.SetName("alma");

        return assertEquals("alma", mgd.getName());
    }

    bool testFlushResources()
    {
        PureManaged mgd;
        mgd.FlushResources();

        return true;
    }

    bool testGetUsedSystemMemory()
    {
        const PureManaged mgd;

        return assertGequals(mgd.getUsedSystemMemory(), sizeof(PureManaged));
    }


}; // class PureManagedTest
