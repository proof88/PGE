#pragma once

/*
    ###################################################################################
    PureAxisAlignedBoundingBoxTest.h
    Unit test for PureAxisAlignedBoundingBox.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/SpatialStructures/PureAxisAlignedBoundingBox.h"

class PureAxisAlignedBoundingBoxTest :
    public UnitTest
{
public:

    PureAxisAlignedBoundingBoxTest() :
        UnitTest( __FILE__ )
    {
        addSubTest("testCtor1", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testCtor1);
        addSubTest("testCtor2", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testCtor2);
        addSubTest("testIsPointInside", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testIsPointInside);
        addSubTest("testIsPointInside_no_bounds", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testIsPointInside_no_bounds);
        addSubTest("testIsBoxInside", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testIsBoxInside);
        addSubTest("testIsBoxInside_no_bounds", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testIsBoxInside_no_bounds);
        addSubTest("testExtendByPointOutside", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testExtendByPointOutside);
        addSubTest("testExtendByPointOutside_is_commutative", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testExtendByPointOutside_is_commutative);
        addSubTest("testExtendByPointOutside_no_bounds", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testExtendByPointOutside_no_bounds);
        addSubTest("testExtendByPointOutside_2", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testExtendByPointOutside_2);
        addSubTest("testExtendByPointInside", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testExtendByPointInside);
        addSubTest("testExtendByBoxOutside", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testExtendByBoxOutside);
        addSubTest("testExtendByBoxOutside_is_commutative", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testExtendByBoxOutside_is_commutative);
        addSubTest("testExtendByBoxOutside_no_bounds", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testExtendByBoxOutside_no_bounds);
        addSubTest("testExtendByBoxInside", (PFNUNITSUBTEST) &PureAxisAlignedBoundingBoxTest::testExtendByBoxInside);
    } // PureAxisAlignedBoundingBoxTest()

    ~PureAxisAlignedBoundingBoxTest() = default;

    PureAxisAlignedBoundingBoxTest(const PureAxisAlignedBoundingBoxTest&) = delete;
    PureAxisAlignedBoundingBoxTest& operator=(const PureAxisAlignedBoundingBoxTest&) = delete;
    PureAxisAlignedBoundingBoxTest(PureAxisAlignedBoundingBoxTest&&) = delete;
    PureAxisAlignedBoundingBoxTest& operator=(PureAxisAlignedBoundingBoxTest&&) = delete;

protected:

private:

    // ---------------------------------------------------------------------------

    bool testCtor1()
    {
        const PureVector pos(10.0f, 20.0f, 30.0f);
        const PureVector size(1.0f, 2.0f, 3.0f);
        const PureAxisAlignedBoundingBox bbox(pos, size);

        return assertEquals(pos, bbox.getPosVec(), "pos") &
            assertEquals(size, bbox.getSizeVec(), "size");    
    }

    bool testCtor2()
    {
        const PureAxisAlignedBoundingBox bbox;

        return assertEquals(PureVector(), bbox.getPosVec(), "pos") &
            assertEquals(PureVector(), bbox.getSizeVec(), "size");    
    }

    bool testIsPointInside()
    {
        const PureVector pos(10.0f, 20.0f, 30.0f);
        const PureVector size(1.0f, 2.0f, 3.0f);
        const PureAxisAlignedBoundingBox bbox(pos, size);

        const PureVector badPoint1(pos.getX() - size.getX()/2.f - 1, pos.getY(), pos.getZ());
        const PureVector badPoint2(pos.getX() + size.getX()/2.f + 1, pos.getY(), pos.getZ());
        const PureVector badPoint3(pos.getX(), pos.getY() - size.getY()/2.f - 1, pos.getZ());
        const PureVector badPoint4(pos.getX(), pos.getY() + size.getY()/2.f + 1, pos.getZ());
        const PureVector badPoint5(pos.getX(), pos.getY(), pos.getZ() - size.getZ()/2.f - 1);
        const PureVector badPoint6(pos.getX(), pos.getY(), pos.getZ() + size.getZ()/2.f + 1);

        const PureVector goodPoint1(pos.getX(), pos.getY(), pos.getZ());

        return assertFalse(bbox.isInside(badPoint1), "badPoint 1") &
            assertFalse(bbox.isInside(badPoint2), "badPoint 2") &
            assertFalse(bbox.isInside(badPoint3), "badPoint 3") &
            assertFalse(bbox.isInside(badPoint4), "badPoint 4") &
            assertFalse(bbox.isInside(badPoint5), "badPoint 5") &
            assertFalse(bbox.isInside(badPoint6), "badPoint 6") &
            assertTrue(bbox.isInside(goodPoint1), "goodPoint 1");
    }

    bool testIsPointInside_no_bounds()
    {
        const PureAxisAlignedBoundingBox bbox;

        return assertFalse(bbox.isInside(PureVector(1.f, 2.f, 3.f)), "badPoint 1") &
            assertFalse(bbox.isInside(PureVector()), "badPoint 2");
    }

    bool testIsBoxInside()
    {
        const PureVector pos(10.0f, 20.0f, 30.0f);
        const PureVector size(3.0f, 3.0f, 3.0f);
        const PureVector posOutsideBox(pos.getX() + 4.0f, pos.getY() - 2.0f, pos.getZ() + 5.0f);
        const PureVector sizeOutsideBox(3.0f, 5.0f, 3.0f);

        const PureVector posInsideBox1(pos);
        const PureVector sizeInsideBox1(size);

        const PureVector posInsideBox2(pos);
        const PureVector sizeInsideBox2(size.getX()-1.f, size.getY()-1.f, size.getZ()-1.f);

        const PureAxisAlignedBoundingBox bbox(pos, size);
        const PureAxisAlignedBoundingBox outsideBox(posOutsideBox, sizeOutsideBox);
        const PureAxisAlignedBoundingBox insideBox1(posInsideBox1, sizeInsideBox1);
        const PureAxisAlignedBoundingBox insideBox2(posInsideBox2, sizeInsideBox2);

        return assertTrue(bbox.isInside(insideBox1), "insideBox1") &
            assertTrue(bbox.isInside(insideBox2), "insideBox2") &
            assertFalse(bbox.isInside(outsideBox), "outsideBox");
    }

    bool testIsBoxInside_no_bounds()
    {
        const PureAxisAlignedBoundingBox bbox;
        const PureAxisAlignedBoundingBox outsideBox(PureVector(1.f, 2.f, 3.f), PureVector(1.f, 2.f, 3.f));

        return assertFalse(bbox.isInside(outsideBox), "outsideBox");
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

        const PureVector pos(10.0f, 20.0f, 30.0f);
        const PureVector size(1.0f, 3.0f, 3.0f);
        const PureVector newExpectedPos(10.75f, 20.0f, 34.25f);
        const PureVector newExpectedSize(2.5f, 3.0f, 11.5f);
        PureAxisAlignedBoundingBox bbox(pos, size);

        const PureVector newPoint(12.f, pos.getY(), 40.0f);

        bool b = assertFalse(bbox.isInside(newPoint), "newPoint before");

        bbox.ExtendBy(newPoint);

        b &= assertTrue(bbox.isInside(newPoint), "newPoint after");

        b &= assertEquals(newExpectedPos, bbox.getPosVec(), "new pos");
        b &= assertEquals(newExpectedSize, bbox.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByPointOutside_is_commutative()
    {
        PureAxisAlignedBoundingBox bbox_original_1(PureVector(10.f, 20.f, 30.f), PureVector(5.f, 6.f, 7.f));
        PureAxisAlignedBoundingBox bbox_original_2(bbox_original_1);

        const PureVector newPoint_1(12.f, 20.f, 40.0f);
        const PureVector newPoint_2(30.f, 40.f, 60.0f);

        bbox_original_1.ExtendBy(newPoint_1);
        bbox_original_1.ExtendBy(newPoint_2);

        bbox_original_2.ExtendBy(newPoint_2);
        bbox_original_2.ExtendBy(newPoint_1);

        bool b = assertEquals(bbox_original_1.getPosVec(), bbox_original_2.getPosVec(), "new pos");
        b &= assertEquals(bbox_original_1.getSizeVec(), bbox_original_2.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByPointOutside_no_bounds()
    {
        PureAxisAlignedBoundingBox bbox;

        const PureVector newPoint(12.f, 20.f, 40.0f);

        bool b = assertFalse(bbox.isInside(newPoint), "newPoint before");

        // nothing should happen, as an uninitialized box can be altered by another box only
        bbox.ExtendBy(newPoint);

        b &= assertFalse(bbox.isInside(newPoint), "newPoint after");

        b &= assertEquals(PureVector(), bbox.getPosVec(), "new pos");
        b &= assertEquals(PureVector(), bbox.getSizeVec(), "new size");

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

        const PureVector pos(10.0f, 20.0f, 30.0f);
        const PureVector size(3.0f, 3.0f, 3.0f);
        const PureVector newExpectedPos(6.75f, 18.25f, 25.75f);
        const PureVector newExpectedSize(9.5f, 6.5f, 11.5f);
        PureAxisAlignedBoundingBox bbox(pos, size);

        const PureVector newPoint(2.f, 15.f, 20.f);

        bool b = assertFalse(bbox.isInside(newPoint), "newPoint before");

        bbox.ExtendBy(newPoint);

        b &= assertTrue(bbox.isInside(newPoint), "newPoint after");

        b &= assertEquals(newExpectedPos, bbox.getPosVec(), "new pos");
        b &= assertEquals(newExpectedSize, bbox.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByPointInside()
    {
        const PureVector pos(10.0f, 20.0f, 30.0f);
        const PureVector size(1.0f, 3.0f, 3.0f);
        PureAxisAlignedBoundingBox bbox(pos, size);

        const PureVector newPoint(pos.getX() + size.getX()/2.f - 1, pos.getY(), pos.getZ());

        bbox.ExtendBy(newPoint);

        bool b = assertTrue(bbox.isInside(newPoint), "newPoint after");

        // pos and size doesnt change as point is inside even before extending
        b &= assertEquals(pos, bbox.getPosVec(), "new pos");
        b &= assertEquals(size, bbox.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByBoxOutside()
    {
        const PureVector pos(80.0f, 100.0f, 30.0f);
        const PureVector size(60.0f, 60.0f, 3.0f);
        const PureVector posExtender(175.0f, 145.0f, 40.0f);
        const PureVector sizeExtender(70.0f, 70.0f, 3.0f);

        const PureVector newExpectedPos(130.f, 125.0f, 35.0f);
        const PureVector newExpectedSize(160.0f, 110.0f, 13.0f);
        PureAxisAlignedBoundingBox bbox(pos, size);
        const PureAxisAlignedBoundingBox bboxExtender(posExtender, sizeExtender);

        bbox.ExtendBy(bboxExtender);

        bool b = assertEquals(newExpectedPos, bbox.getPosVec(), "new pos");
        b &= assertEquals(newExpectedSize, bbox.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByBoxOutside_is_commutative()
    {
        PureAxisAlignedBoundingBox bbox_original_1(PureVector(10.f, 20.f, 30.f), PureVector(5.f, 6.f, 7.f));
        PureAxisAlignedBoundingBox bbox_original_2(bbox_original_1);

        const PureAxisAlignedBoundingBox bboxExtender_1(PureVector(20.f, 30.f, 40.f), PureVector(5.f, 6.f, 7.f));
        const PureAxisAlignedBoundingBox bboxExtender_2(PureVector(50.f, 60.f, 70.f), PureVector(10.f, 12.f, 21.f));

        bbox_original_1.ExtendBy(bboxExtender_1);
        bbox_original_1.ExtendBy(bboxExtender_2);

        bbox_original_2.ExtendBy(bboxExtender_2);
        bbox_original_2.ExtendBy(bboxExtender_1);

        bool b = assertEquals(bbox_original_1.getPosVec(), bbox_original_2.getPosVec(), "new pos");
        b &= assertEquals(bbox_original_1.getSizeVec(), bbox_original_2.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByBoxOutside_no_bounds()
    {
        PureAxisAlignedBoundingBox bbox;
        const PureAxisAlignedBoundingBox bboxExtender(PureVector(20.f, 30.f, 40.f), PureVector(5.f, 6.f, 7.f));

        // bboxExtender is basically copied
        bbox.ExtendBy(bboxExtender);

        bool b = assertEquals(bboxExtender.getPosVec(), bbox.getPosVec(), "new pos");
        b &= assertEquals(bboxExtender.getSizeVec(), bbox.getSizeVec(), "new size");

        return b;
    }

    bool testExtendByBoxInside()
    {
        // in this test we dont expect extend because extender box is fully within bounds of original box
        const PureVector pos(10.0f, 20.0f, 30.0f);
        const PureVector size(3.0f, 3.0f, 3.0f);
        const PureVector posExtender(pos.getX(), pos.getY(), pos.getZ());
        const PureVector sizeExtender(size.getX()-1.f, size.getY()-1.f, size.getZ());
        PureAxisAlignedBoundingBox bbox(pos, size);
        const PureAxisAlignedBoundingBox bboxExtender(posExtender, sizeExtender);

        bbox.ExtendBy(bboxExtender);

        bool b = assertEquals(pos, bbox.getPosVec(), "new pos");
        b &= assertEquals(size, bbox.getSizeVec(), "new size");

        return b;
    }

}; // class PureAxisAlignedBoundingBoxTest