#pragma once

/*
    ###################################################################################
    PRREOctreeTest.h
    Unit test for PRREOctree.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/internal/SpatialStructures/PRREOctree.h"

class PRREOctreeTest :
    public UnitTest
{
public:

    PRREOctreeTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREOctreeTest::testCtor1);
        AddSubTest("testInsertObject", (PFNUNITSUBTEST) &PRREOctreeTest::testInsertObject);
        AddSubTest("testRemoveObject", (PFNUNITSUBTEST) &PRREOctreeTest::testRemoveObject);
        AddSubTest("testFindObject", (PFNUNITSUBTEST) &PRREOctreeTest::testFindObject);
        AddSubTest("testGetDepthLevel", (PFNUNITSUBTEST) &PRREOctreeTest::testGetDepthLevel);
        AddSubTest("testGetMaxDepthLevel", (PFNUNITSUBTEST) &PRREOctreeTest::testGetMaxDepthLevel);
        AddSubTest("testGetNodeType", (PFNUNITSUBTEST) &PRREOctreeTest::testGetNodeType);
    } // PRREOctreeTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PRREOctreeTest(const PRREOctreeTest&)
    {}         

    PRREOctreeTest& operator=(const PRREOctreeTest&)
    {
        return *this;
    }

    bool testCtor1()
    {
        return false;
    }

    bool testInsertObject()
    {
        return false;
    }

    bool testRemoveObject()
    {
        return false;
    }

    bool testFindObject()
    {
        return false;
    }

    bool testGetDepthLevel()
    {
        return false;
    }

    bool testGetMaxDepthLevel()
    {
        return false;
    }

    bool testGetNodeType()
    {
        return false;
    }

}; // class PRREOctreeTest