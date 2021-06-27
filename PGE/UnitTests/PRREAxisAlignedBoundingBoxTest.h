#pragma once

/*
    ###################################################################################
    PRREAxisAlignedBoundingBoxTest.h
    Unit test for PRREAxisAlignedBoundingBox.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/internal/SpatialStructures/PRREAxisAlignedBoundingBox.h"

#ifndef E2
#define E2 0.005f
#endif // EPSILON

class PRREAxisAlignedBoundingBoxTest :
    public UnitTest
{
public:

    PRREAxisAlignedBoundingBoxTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREAxisAlignedBoundingBoxTest::testCtor1);
        AddSubTest("testIsPointInside", (PFNUNITSUBTEST) &PRREAxisAlignedBoundingBoxTest::testIsPointInside);
        AddSubTest("testIsBoxInside", (PFNUNITSUBTEST) &PRREAxisAlignedBoundingBoxTest::testIsBoxInside);
        AddSubTest("testExtendByPointOutside", (PFNUNITSUBTEST) &PRREAxisAlignedBoundingBoxTest::testExtendByPointOutside);
        AddSubTest("testExtendByPointOutside_2", (PFNUNITSUBTEST) &PRREAxisAlignedBoundingBoxTest::testExtendByPointOutside_2);
        AddSubTest("testExtendByPointInside", (PFNUNITSUBTEST) &PRREAxisAlignedBoundingBoxTest::testExtendByPointInside);
        AddSubTest("testExtendByBoxOutside", (PFNUNITSUBTEST) &PRREAxisAlignedBoundingBoxTest::testExtendByBoxOutside);
        AddSubTest("testExtendByBoxInside", (PFNUNITSUBTEST) &PRREAxisAlignedBoundingBoxTest::testExtendByBoxInside);
    } // PRREAxisAlignedBoundingBoxTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PRREAxisAlignedBoundingBoxTest(const PRREAxisAlignedBoundingBoxTest&)
    {};         

    PRREAxisAlignedBoundingBoxTest& operator=(const PRREAxisAlignedBoundingBoxTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PRREVector pos(10.0f, 20.0f, 30.0f);
        const PRREVector size(1.0f, 2.0f, 3.0f);
        const PRREAxisAlignedBoundingBox bbox(pos, size);

        return assertEquals(pos, bbox.getPosVec(), "pos") &
            assertEquals(size, bbox.getSizeVec(), "size");    
    }

    bool testIsPointInside()
    {
        const PRREVector pos(10.0f, 20.0f, 30.0f);
        const PRREVector size(1.0f, 2.0f, 3.0f);
        const PRREAxisAlignedBoundingBox bbox(pos, size);

        const PRREVector badPoint1(pos.getX() - size.getX()/2.f - 1, pos.getY(), pos.getZ());
        const PRREVector badPoint2(pos.getX() + size.getX()/2.f + 1, pos.getY(), pos.getZ());
        const PRREVector badPoint3(pos.getX(), pos.getY() - size.getY()/2.f - 1, pos.getZ());
        const PRREVector badPoint4(pos.getX(), pos.getY() + size.getY()/2.f + 1, pos.getZ());
        const PRREVector badPoint5(pos.getX(), pos.getY(), pos.getZ() - size.getZ()/2.f - 1);
        const PRREVector badPoint6(pos.getX(), pos.getY(), pos.getZ() + size.getZ()/2.f + 1);

        const PRREVector goodPoint1(pos.getX(), pos.getY(), pos.getZ());

        return assertFalse(bbox.isInside(badPoint1), "badPoint 1") &
            assertFalse(bbox.isInside(badPoint2), "badPoint 2") &
            assertFalse(bbox.isInside(badPoint3), "badPoint 3") &
            assertFalse(bbox.isInside(badPoint4), "badPoint 4") &
            assertFalse(bbox.isInside(badPoint5), "badPoint 5") &
            assertFalse(bbox.isInside(badPoint6), "badPoint 6") &
            assertTrue(bbox.isInside(goodPoint1), "goodPoint 1");
    }

    bool testIsBoxInside()
    {
        const PRREVector pos(10.0f, 20.0f, 30.0f);
        const PRREVector size(3.0f, 3.0f, 3.0f);
        const PRREVector posOutsideBox(pos.getX() + 4.0f, pos.getY() - 2.0f, pos.getZ() + 5.0f);
        const PRREVector sizeOutsideBox(3.0f, 5.0f, 3.0f);

        const PRREVector posInsideBox1(pos);
        const PRREVector sizeInsideBox1(size);

        const PRREVector posInsideBox2(pos);
        const PRREVector sizeInsideBox2(size.getX()-1.f, size.getY()-1.f, size.getZ()-1.f);

        const PRREAxisAlignedBoundingBox bbox(pos, size);
        const PRREAxisAlignedBoundingBox outsideBox(posOutsideBox, sizeOutsideBox);
        const PRREAxisAlignedBoundingBox insideBox1(posInsideBox1, sizeInsideBox1);
        const PRREAxisAlignedBoundingBox insideBox2(posInsideBox2, sizeInsideBox2);

        return assertTrue(bbox.isInside(insideBox1), "insideBox1") &
            assertTrue(bbox.isInside(insideBox2), "insideBox2") &
            assertFalse(bbox.isInside(outsideBox), "outsideBox");
    }

    bool testExtendByPointOutside()
    {
        /*

        In this 2D picture, left side box is bbox, right dot is the extender point.
            x=   9.5 10.5
        |           x 
        |y= 20      x . 
        |           x  
        |              
        |              
        |              
        |y= 15
        |
        --------------------------------> x

        After extend, bbox should look like this in 2D:
            x=     10.75
        |           xxx
        |y= 20      xxx
        |           xxx
        |          
        |          
        |          
        |y= 15
        --------------------------------> x
        */

        const PRREVector pos(10.0f, 20.0f, 30.0f);
        const PRREVector size(1.0f, 3.0f, 3.0f);
        const PRREVector newExpectedPos(10.75f, 20.0f, 34.25f);
        const PRREVector newExpectedSize(2.5f, 3.0f, 11.5f);
        PRREAxisAlignedBoundingBox bbox(pos, size);

        const PRREVector newPoint(12.f, pos.getY(), 40.0f);

        bool b = assertFalse(bbox.isInside(newPoint), "newPoint before");

        bbox.ExtendBy(newPoint);

        b &= assertTrue(bbox.isInside(newPoint), "newPoint after");

        b &= assertEquals(newExpectedPos, bbox.getPosVec(), "new pos");
        b &= assertEquals(newExpectedSize, bbox.getSizeVec(), "new size");

        return b;
    }

     bool testExtendByPointOutside_2()
    {
        /*

        In this 2D picture, right side box is bbox, left dot is the extender point.
                      2       10
        |                    xxx
        | y= 20              xxx  
        |                    xxx 
        |               
        |               
        |               
        | y= 15       .
        |
        --------------------------------> x

        After extend, bbox should look like this in 2D:
                      2        11
        |             xxxxxxxxxx
        | y= 20       xxxxxxxxxx
        |             xxxxxxxxxx
        |             xxxxxxxxxx
        |             xxxxxxxxxx
        |             xxxxxxxxxx
        | y= 15       xxxxxxxxxx
        | 
        --------------------------------> x
        */

        const PRREVector pos(10.0f, 20.0f, 30.0f);
        const PRREVector size(3.0f, 3.0f, 3.0f);
        const PRREVector newExpectedPos(6.75f, 18.25f, 25.75f);
        const PRREVector newExpectedSize(9.5f, 6.5f, 11.5f);
        PRREAxisAlignedBoundingBox bbox(pos, size);

        const PRREVector newPoint(2.f, 15.f, 20.f);

        bool b = assertFalse(bbox.isInside(newPoint), "newPoint before");

        bbox.ExtendBy(newPoint);

        b &= assertTrue(bbox.isInside(newPoint), "newPoint after");

        b &= assertEquals(newExpectedPos, bbox.getPosVec(), "new pos");
        b &= assertEquals(newExpectedSize, bbox.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByPointInside()
    {
        const PRREVector pos(10.0f, 20.0f, 30.0f);
        const PRREVector size(1.0f, 3.0f, 3.0f);
        PRREAxisAlignedBoundingBox bbox(pos, size);

        const PRREVector newPoint(pos.getX() + size.getX()/2.f - 1, pos.getY(), pos.getZ());

        bbox.ExtendBy(newPoint);

        bool b = assertTrue(bbox.isInside(newPoint), "newPoint after");

        // pos and size doesnt change as point is inside even before extending
        b &= assertEquals(pos, bbox.getPosVec(), "new pos");
        b &= assertEquals(size, bbox.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByBoxOutside()
    {
        const PRREVector pos(80.0f, 100.0f, 30.0f);
        const PRREVector size(60.0f, 60.0f, 3.0f);
        const PRREVector posExtender(175.0f, 145.0f, 40.0f);
        const PRREVector sizeExtender(70.0f, 70.0f, 3.0f);

        const PRREVector newExpectedPos(130.f, 125.0f, 35.0f);
        const PRREVector newExpectedSize(160.0f, 110.0f, 13.0f);
        PRREAxisAlignedBoundingBox bbox(pos, size);
        const PRREAxisAlignedBoundingBox bboxExtender(posExtender, sizeExtender);

        bbox.ExtendBy(bboxExtender);

        bool b = assertEquals(newExpectedPos, bbox.getPosVec(), "new pos");
        b &= assertEquals(newExpectedSize, bbox.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByBoxInside()
    {
        // in this test we dont expect extend because extender box is fully within bounds of original box
        const PRREVector pos(10.0f, 20.0f, 30.0f);
        const PRREVector size(3.0f, 3.0f, 3.0f);
        const PRREVector posExtender(pos.getX(), pos.getY(), pos.getZ());
        const PRREVector sizeExtender(size.getX()-1.f, size.getY()-1.f, size.getZ());
        PRREAxisAlignedBoundingBox bbox(pos, size);
        const PRREAxisAlignedBoundingBox bboxExtender(posExtender, sizeExtender);

        bbox.ExtendBy(bboxExtender);

        bool b = assertEquals(pos, bbox.getPosVec(), "new pos");
        b &= assertEquals(size, bbox.getSizeVec(), "new size");

        return b;
    }

}; // class PRREAxisAlignedBoundingBoxTest