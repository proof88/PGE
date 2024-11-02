#pragma once

/*
    ###################################################################################
    PureManagerTest.h
    Unit test for PureManager.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PureManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureManagerTest :
    public UnitTest
{
public:

    PureManagerTest() :
        UnitTest( __FILE__ )
    {
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureManagerTest::testCtor);
        addSubTest("testGetCount", (PFNUNITSUBTEST) &PureManagerTest::testGetCount);
        addSubTest("testIsEmpty", (PFNUNITSUBTEST) &PureManagerTest::testIsEmpty);
        addSubTest("testGetSize", (PFNUNITSUBTEST) &PureManagerTest::testGetSize);
        addSubTest("testGetAttachedAt", (PFNUNITSUBTEST) &PureManagerTest::testGetAttachedAt);
        addSubTest("testGetAttachedIndex", (PFNUNITSUBTEST) &PureManagerTest::testGetAttachedIndex);
        addSubTest("testPreAlloc", (PFNUNITSUBTEST) &PureManagerTest::testPreAlloc);
        addSubTest("testHasAttached2", (PFNUNITSUBTEST) &PureManagerTest::testHasAttached2);
        addSubTest("testAttach", (PFNUNITSUBTEST) &PureManagerTest::testAttach);
        addSubTest("testDetach", (PFNUNITSUBTEST) &PureManagerTest::testDetach);
        addSubTest("testDeleteAttachedInstance", (PFNUNITSUBTEST) &PureManagerTest::testDeleteAttachedInstance);
        addSubTest("testDeleteAll", (PFNUNITSUBTEST) &PureManagerTest::testDeleteAll);
        addSubTest("testGetUsedSystemMemoryTotal", (PFNUNITSUBTEST) &PureManagerTest::testGetUsedSystemMemoryTotal);
        addSubTest("testWriteList", (PFNUNITSUBTEST) &PureManagerTest::testWriteList);
    } // PureManagerTest()

    virtual ~PureManagerTest()
    {
        
    }

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureManager::getLoggerModuleName(), true);
        //CConsole::getConsoleInstance().SetLoggingState(PureManaged::getLoggerModuleName(), true);
    }

    virtual void finalize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(PureManager::getLoggerModuleName(), false);
        CConsole::getConsoleInstance().SetLoggingState(PureManaged::getLoggerModuleName(), false);    
    }

protected:

private:

    // ---------------------------------------------------------------------------

    PureManagerTest(const PureManagerTest&)
    {};         

    PureManagerTest& operator=(const PureManagerTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PureManager mgr1;
        return true;
    }

    bool testGetCount()
    {
        const PureManager mgr;
        return assertEquals(0, mgr.getCount());
    }

    bool testIsEmpty()
    {
        const PureManager mgr;
        return assertTrue(mgr.isEmpty());
    }

    bool testGetSize()
    {
        const PureManager mgr;
        return assertGequals(mgr.getSize(), mgr.getCount());
    }

    bool testGetAttachedAt()
    {
        const PureManager mgr;
        return assertNull(mgr.getAttachedAt(0));
    }

    bool testGetAttachedIndex()
    {
        PureManager mgr;
        PureManaged* const mgd1 = new PureManaged();
        PureManaged* const mgd2 = new PureManaged();

        mgr.Attach(*mgd2);

        return assertEquals(-1, mgr.getAttachedIndex(*mgd1), "mgd1") &
            assertEquals(0, mgr.getAttachedIndex(*mgd2), "mgd2");
    }

    bool testPreAlloc()
    {
        PureManager mgr;
        const TPureInt prevSize = mgr.getSize();
        mgr.PreAlloc(10);

        return assertEquals(prevSize + 10, mgr.getSize());
    }

    bool testHasAttached2()
    {
        PureManager mgr;
        PureManaged* const mgd1 = new PureManaged();
        PureManaged* const mgd2 = new PureManaged();

        mgr.Attach(*mgd2);

        return assertFalse(mgr.hasAttached(*mgd1), "mgd1") &
            assertTrue(mgr.hasAttached(*mgd2), "mgd2");
    }

    bool testAttach()
    {
        PureManager mgr1;
        PureManager mgr2;
        PureManaged* const mgd = new PureManaged();

        mgr1.Attach(*mgd);
        mgr2.Attach(*mgd);

        return assertTrue(mgr1.hasAttached(*mgd), "mgr1.hasAttached()") &
            assertEquals(1, mgr1.getCount(), "mgr1.getCount()") &
            assertFalse(mgr2.hasAttached(*mgd), "mgr2.hasAttached()") &
            assertEquals(0, mgr2.getCount(), "mgr2.getCount()");
    }

    bool testDetach()
    {
        PureManager mgr;
        PureManaged* const mgd = new PureManaged();

        mgr.Attach(*mgd);
        mgr.Detach(*mgd);

        const bool l = assertEquals(0, mgr.getCount());
        mgr.Attach(*mgd);
        
        return l;
    }

    bool testDeleteAttachedInstance()
    {
        PureManager mgr;
        PureManaged* const mgd = new PureManaged();

        mgr.Attach(*mgd);
        mgr.DeleteAttachedInstance(*mgd);

        return assertEquals(0, mgr.getCount());
    }

    bool testDeleteAll()
    {
        PureManager mgr;
        PureManaged* const mgd1 = new PureManaged(), * const mgd2 = new PureManaged();

        mgr.DeleteAll();
        bool l = assertEquals(0, mgr.getCount(), "getCount 1");

        mgr.Attach(*mgd1);
        mgr.Attach(*mgd2);
        mgr.DeleteAll();
        l &= assertEquals(0, mgr.getCount(), "getCount 2");

        return l;
    }

    bool testGetUsedSystemMemoryTotal()
    {
        PureManager mgr;
        PureManaged* const mgd1 = new PureManaged(), * const mgd2 = new PureManaged();
        mgr.Attach(*mgd1);
        mgr.Attach(*mgd2);

        return assertGequals(mgr.getUsedSystemMemory(), mgd1->getUsedSystemMemory() + mgd2->getUsedSystemMemory());
    }

    bool testWriteList()
    {
        const PureManager mgr;
        mgr.WriteList();

        return true;
    }

}; // class PureManagerTest
