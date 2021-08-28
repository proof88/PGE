#pragma once

/*
    ###################################################################################
    PRREManagedTest.h
    Unit test for PRREManaged.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/PRREManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREManagedTest :
    public UnitTest
{
public:

    PRREManagedTest() :
        UnitTest( __FILE__ )
    {
        
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREManagedTest::testCtor);
        AddSubTest("testGetUtiliser", (PFNUNITSUBTEST) &PRREManagedTest::testGetUtiliser);
        AddSubTest("testSetUtiliser", (PFNUNITSUBTEST) &PRREManagedTest::testSetUtiliser);
        AddSubTest("testDetachFrom", (PFNUNITSUBTEST) &PRREManagedTest::testDetachFrom);
        AddSubTest("testGetManager", (PFNUNITSUBTEST) &PRREManagedTest::testGetManager);
        AddSubTest("testGetName", (PFNUNITSUBTEST) &PRREManagedTest::testGetName);
        AddSubTest("testSetName", (PFNUNITSUBTEST) &PRREManagedTest::testSetName);
        AddSubTest("testFlushResources", (PFNUNITSUBTEST) &PRREManagedTest::testFlushResources);
        AddSubTest("testGetUsedSystemMemory", (PFNUNITSUBTEST) &PRREManagedTest::testGetUsedSystemMemory);
    } // PRREManagedTest()

    virtual ~PRREManagedTest()
    {
        
    }

    virtual void Initialize()
    {
        //CConsole::getConsoleInstance().SetLoggingState(PRREManaged::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PRREManager::getLoggerModuleName(), true);
    }

    virtual void Finalize()
    {
        CConsole::getConsoleInstance().SetLoggingState(PRREManaged::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PRREManager::getLoggerModuleName(), false);    
    }

protected:

private:

    // ---------------------------------------------------------------------------

    PRREManagedTest(const PRREManagedTest&)
    {};         

    PRREManagedTest& operator=(const PRREManagedTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PRREManaged mgd1;
        return true;
    }

    bool testGetUtiliser()
    {
        const PRREManaged mgd1;
        return assertNull(mgd1.getUtiliser(), "getUtiliser()");
    }

    bool testSetUtiliser()
    {
        PRREManaged mgd1;
        PRREManaged mgd2;

        // cannot set self
        mgd1.SetUtiliser(&mgd1);
        bool b = assertNull(mgd1.getUtiliser(), "getUtiliser() 1");

        mgd1.SetUtiliser(&mgd2);
        b &= assertEquals(&mgd2, mgd1.getUtiliser(), "getUtiliser() 2");

        return b;
    }

    bool testDetachFrom()
    {
        PRREManager mgr;
        PRREManaged mgd;
        mgd.DetachFrom();
        mgr.Attach(mgd);
        mgd.DetachFrom();

        return assertEquals(0, mgr.getCount(), "getCount()") &
            assertNull(mgd.getManager(), "getManager()");
    }

    bool testGetManager()
    {
        PRREManager mgr;
        PRREManaged mgd;

        bool l = assertNull(mgd.getManager(), "getManager() 1");

        mgr.Attach(mgd);
        l &= assertEquals(&mgr, mgd.getManager(), "getManager() 2");

        return l;
    }

    bool testGetName()
    {
        PRREManaged mgd;

        return assertEquals("", mgd.getName());
    }

    bool testSetName()
    {
        PRREManaged mgd;
        mgd.SetName("alma");

        return assertEquals("alma", mgd.getName());
    }

    bool testFlushResources()
    {
        PRREManaged mgd;
        mgd.FlushResources();

        return true;
    }

    bool testGetUsedSystemMemory()
    {
        const PRREManaged mgd;

        return assertGequals(mgd.getUsedSystemMemory(), sizeof(PRREManaged));
    }


}; // class PRREManagedTest
