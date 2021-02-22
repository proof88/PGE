#pragma once

/*
    ###################################################################################
    PRREManagerTest.h
    Unit test for PRREManager.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/PRREManager.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREManagerTest :
    public UnitTest
{
public:

    PRREManagerTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREManagerTest::testCtor);
        AddSubTest("testGetCount", (PFNUNITSUBTEST) &PRREManagerTest::testGetCount);
        AddSubTest("testIsEmpty", (PFNUNITSUBTEST) &PRREManagerTest::testIsEmpty);
        AddSubTest("testGetSize", (PFNUNITSUBTEST) &PRREManagerTest::testGetSize);
        AddSubTest("testGetAttachedAt", (PFNUNITSUBTEST) &PRREManagerTest::testGetAttachedAt);
        AddSubTest("testGetAttachedIndex", (PFNUNITSUBTEST) &PRREManagerTest::testGetAttachedIndex);
        AddSubTest("testPreAlloc", (PFNUNITSUBTEST) &PRREManagerTest::testPreAlloc);
        AddSubTest("testHasAttached2", (PFNUNITSUBTEST) &PRREManagerTest::testHasAttached2);
        AddSubTest("testAttach", (PFNUNITSUBTEST) &PRREManagerTest::testAttach);
        AddSubTest("testDetach", (PFNUNITSUBTEST) &PRREManagerTest::testDetach);
        AddSubTest("testDeleteAttachedInstance", (PFNUNITSUBTEST) &PRREManagerTest::testDeleteAttachedInstance);
        AddSubTest("testDeleteAll", (PFNUNITSUBTEST) &PRREManagerTest::testDeleteAll);
        AddSubTest("testGetUsedSystemMemoryTotal", (PFNUNITSUBTEST) &PRREManagerTest::testGetUsedSystemMemoryTotal);
        AddSubTest("testWriteList", (PFNUNITSUBTEST) &PRREManagerTest::testWriteList);
    } // PRREManagerTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PRREManagerTest(const PRREManagerTest&)
    {};         

    PRREManagerTest& operator=(const PRREManagerTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PRREManager mgr1;
        return true;
    }

    bool testGetCount()
    {
        const PRREManager mgr;
        return assertEquals(0, mgr.getCount());
    }

    bool testIsEmpty()
    {
        const PRREManager mgr;
        return assertTrue(mgr.isEmpty());
    }

    bool testGetSize()
    {
        const PRREManager mgr;
        return assertGequals(mgr.getSize(), mgr.getCount());
    }

    bool testGetAttachedAt()
    {
        const PRREManager mgr;
        return assertNull(mgr.getAttachedAt(0));
    }

    bool testGetAttachedIndex()
    {
        PRREManager mgr;
        PRREManaged* const mgd1 = new PRREManaged();
        PRREManaged* const mgd2 = new PRREManaged();

        mgr.Attach(*mgd2);

        return assertEquals(-1, mgr.getAttachedIndex(*mgd1), "mgd1") &
            assertEquals(0, mgr.getAttachedIndex(*mgd2), "mgd2");
    }

    bool testPreAlloc()
    {
        PRREManager mgr;
        const TPRREint prevSize = mgr.getSize();
        mgr.PreAlloc(10);

        return assertEquals(prevSize + 10, mgr.getSize());
    }

    bool testHasAttached2()
    {
        PRREManager mgr;
        PRREManaged* const mgd1 = new PRREManaged();
        PRREManaged* const mgd2 = new PRREManaged();

        mgr.Attach(*mgd2);

        return assertFalse(mgr.hasAttached(*mgd1), "mgd1") &
            assertTrue(mgr.hasAttached(*mgd2), "mgd2");
    }

    bool testAttach()
    {
        PRREManager mgr1;
        PRREManager mgr2;
        PRREManaged* const mgd = new PRREManaged();

        mgr1.Attach(*mgd);
        mgr2.Attach(*mgd);

        return assertTrue(mgr1.hasAttached(*mgd), "mgr1.hasAttached()") &
            assertEquals(1, mgr1.getCount(), "mgr1.getCount()") &
            assertFalse(mgr2.hasAttached(*mgd), "mgr2.hasAttached()") &
            assertEquals(0, mgr2.getCount(), "mgr2.getCount()");
    }

    bool testDetach()
    {
        PRREManager mgr;
        PRREManaged* const mgd = new PRREManaged();

        mgr.Attach(*mgd);
        mgr.Detach(*mgd);

        const bool l = assertEquals(0, mgr.getCount());
        mgr.Attach(*mgd);
        
        return l;
    }

    bool testDeleteAttachedInstance()
    {
        PRREManager mgr;
        PRREManaged* const mgd = new PRREManaged();

        mgr.Attach(*mgd);
        mgr.DeleteAttachedInstance(*mgd);

        return assertEquals(0, mgr.getCount());
    }

    bool testDeleteAll()
    {
        PRREManager mgr;
        PRREManaged* const mgd1 = new PRREManaged(), * const mgd2 = new PRREManaged();

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
        PRREManager mgr;
        PRREManaged* const mgd1 = new PRREManaged(), * const mgd2 = new PRREManaged();
        mgr.Attach(*mgd1);
        mgr.Attach(*mgd2);

        return assertGequals(mgr.getUsedSystemMemoryTotal(), mgd1->getUsedSystemMemory() + mgd2->getUsedSystemMemory());
    }

    bool testWriteList()
    {
        const PRREManager mgr;
        mgr.WriteList();

        return true;
    }

}; // class PRREManagerTest
