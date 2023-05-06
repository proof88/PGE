#pragma once

/*
    ###################################################################################
    PureScissorTest.h
    Unit test for Scissoring class.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../../../PFL/PFL/PFL.h"
#include "../Pure/include/internal/Math/PUREScissor.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureScissorTest :
    public UnitTest
{
public:

    PureScissorTest() : UnitTest(__FILE__, "")
    {
    }

    ~PureScissorTest()
    {
        Finalize();
    } // ~PureScissorTest()

protected:

    virtual void Initialize()
    {
        AddSubTest("testDoScissor", (PFNUNITSUBTEST) &PureScissorTest::testDoScissor);
    }

    virtual bool setUp() override
    {
        return true;    
    }

    virtual void TearDown() override
    {
        Finalize();
    }

    virtual void Finalize()
    {
    }



private:

    // ---------------------------------------------------------------------------

    PureScissorTest(const PureScissorTest&)
    {};         

    PureScissorTest& operator=(const PureScissorTest&)
    {
        return *this;
    };

    bool testDoScissor()
    {
        struct TXY_INT_TEST 
        {
            int x;
            int y;
        };

        const int CLIP_LEFT   =   0;
        const int CLIP_BOTTOM =   0;
        const int CLIP_TOP    = 200;
        const int CLIP_RIGHT  = 300;

        // First we define user lines as const, so we can compare the possibly updated points against these later.
        const TXY_INT_TEST CONST_LINE_INSIDE_P0 = {  0,  0 };
        const TXY_INT_TEST CONST_LINE_INSIDE_P1 = { 50, 50 };

        const TXY_INT_TEST CONST_LINE_INSIDE_REV_P0 = { 50, 50 };
        const TXY_INT_TEST CONST_LINE_INSIDE_REV_P1 = {  0,  0 };

        const TXY_INT_TEST CONST_LINE_INTERSECT_TOP_P0 = {  0,   0 };
        const TXY_INT_TEST CONST_LINE_INTERSECT_TOP_P1 = { 50, 400 };

        const TXY_INT_TEST CONST_LINE_INTERSECT_BOTTOM_LEFT_P0 = { -50, -50 };
        const TXY_INT_TEST CONST_LINE_INTERSECT_BOTTOM_LEFT_P1 = {  50,  50 };

        const TXY_INT_TEST CONST_LINE_OUTSIDE_REGION_BOTTOM_LEFT_P0 = { -50, -50 };
        const TXY_INT_TEST CONST_LINE_OUTSIDE_REGION_BOTTOM_LEFT_P1 = { -20, -20 };

        const TXY_INT_TEST CONST_LINE_OUTSIDE_REGION_TOP_RIGHT_P0 = { 350, 250 };
        const TXY_INT_TEST CONST_LINE_OUTSIDE_REGION_TOP_RIGHT_P1 = { 370, 260 };

        // And these are the lines given to the scissoring algorithm, these might be updated as a result of scissioring.
        // Some are expected to be unchanged, some are expected to be changed.
        TXY_INT_TEST line_inside_p0 = CONST_LINE_INSIDE_P0;
        TXY_INT_TEST line_inside_p1 = CONST_LINE_INSIDE_P1;

        TXY_INT_TEST line_inside_rev_p0 = CONST_LINE_INSIDE_REV_P0;
        TXY_INT_TEST line_inside_rev_p1 = CONST_LINE_INSIDE_REV_P1;

        TXY_INT_TEST line_intersect_top_p0 = CONST_LINE_INTERSECT_TOP_P0;
        TXY_INT_TEST line_intersect_top_p1 = CONST_LINE_INTERSECT_TOP_P1;

        TXY_INT_TEST line_intersect_bottom_left_p0 = CONST_LINE_INTERSECT_BOTTOM_LEFT_P0;
        TXY_INT_TEST line_intersect_bottom_left_p1 = CONST_LINE_INTERSECT_BOTTOM_LEFT_P1;

        TXY_INT_TEST line_outside_region_bottom_left_p0 = CONST_LINE_OUTSIDE_REGION_BOTTOM_LEFT_P0;
        TXY_INT_TEST line_outside_region_bottom_left_p1 = CONST_LINE_OUTSIDE_REGION_BOTTOM_LEFT_P1;

        TXY_INT_TEST line_outside_region_top_right_p0 = CONST_LINE_OUTSIDE_REGION_TOP_RIGHT_P0;
        TXY_INT_TEST line_outside_region_top_right_p1 = CONST_LINE_OUTSIDE_REGION_TOP_RIGHT_P1;


        bool b = true;
        

        // we expect immediate trivial accept without any change of coordinates
        b = b &
            assertTrue( PUREScissor::DoScissor(
                            line_inside_p0.x, line_inside_p0.y,
                            line_inside_p1.x, line_inside_p1.y,
                            CLIP_LEFT, CLIP_RIGHT, CLIP_BOTTOM, CLIP_TOP ), "line inside") &
            assertEquals(CONST_LINE_INSIDE_P0.x, line_inside_p0.x, "line inside p0.x") &
            assertEquals(CONST_LINE_INSIDE_P0.y, line_inside_p0.y, "line inside p0.y") &
            assertEquals(CONST_LINE_INSIDE_P1.x, line_inside_p1.x, "line inside p1.x") &
            assertEquals(CONST_LINE_INSIDE_P1.y, line_inside_p1.y, "line inside p1.y");

        // we expect immediate trivial accept without any change of coordinates
        b = b &
            assertTrue( PUREScissor::DoScissor(
                            line_inside_rev_p0.x, line_inside_rev_p0.y,
                            line_inside_rev_p1.x, line_inside_rev_p1.y,
                            CLIP_LEFT, CLIP_RIGHT, CLIP_BOTTOM, CLIP_TOP ), "line inside rev") &
            assertEquals(CONST_LINE_INSIDE_REV_P0.x, line_inside_rev_p0.x, "line inside rev p0.x") &
            assertEquals(CONST_LINE_INSIDE_REV_P0.y, line_inside_rev_p0.y, "line inside rev p0.y") &
            assertEquals(CONST_LINE_INSIDE_REV_P1.x, line_inside_rev_p1.x, "line inside rev p1.x") &
            assertEquals(CONST_LINE_INSIDE_REV_P1.y, line_inside_rev_p1.y, "line inside rev p1.y");

        
        float m = (line_intersect_top_p1.x - line_intersect_top_p0.x) / (float)(line_intersect_top_p1.y - line_intersect_top_p0.y);
	    const int CONST_EXP_LINE_INTERSECT_TOP_P1_X = (int) (m * (CLIP_TOP - line_intersect_top_p0.y) + line_intersect_top_p0.x);
        const int CONST_EXP_LINE_INTERSECT_TOP_P1_Y = CLIP_TOP;

        // only P1 is outside, expect P0 and P1 swap because P1 was falling outside top clip line
        b = b &
            assertTrue( PUREScissor::DoScissor(
                            line_intersect_top_p0.x, line_intersect_top_p0.y,
                            line_intersect_top_p1.x, line_intersect_top_p1.y,
                            CLIP_LEFT, CLIP_RIGHT, CLIP_BOTTOM, CLIP_TOP ), "line intersect top") &
            assertEquals(CONST_EXP_LINE_INTERSECT_TOP_P1_X, line_intersect_top_p0.x, "line intersect top p0.x") &
            assertEquals(CONST_EXP_LINE_INTERSECT_TOP_P1_Y, line_intersect_top_p0.y, "line intersect top p0.y") &
            assertEquals(CONST_LINE_INTERSECT_TOP_P0.x, line_intersect_top_p1.x, "line intersect top p1.x") &
            assertEquals(CONST_LINE_INTERSECT_TOP_P0.y, line_intersect_top_p1.y, "line intersect top p1.y");


        m = (line_intersect_bottom_left_p1.y - line_intersect_bottom_left_p0.y) / (float)(line_intersect_bottom_left_p1.x - line_intersect_bottom_left_p0.x);
	    const int CONST_EXP_LINE_INTERSECT_BOTTOM_LEFT_P0_X = CLIP_LEFT;
        const int CONST_EXP_LINE_INTERSECT_BOTTOM_LEFT_P0_Y = (int) (m * (CLIP_LEFT - line_intersect_bottom_left_p0.x) + line_intersect_bottom_left_p0.y);

        // only P0 is outside, do not expect P0 and P1 swap because P0 was falling outside top clip line
        b = b &
            assertTrue( PUREScissor::DoScissor(
                            line_intersect_bottom_left_p0.x, line_intersect_bottom_left_p0.y,
                            line_intersect_bottom_left_p1.x, line_intersect_bottom_left_p1.y,
                            CLIP_LEFT, CLIP_RIGHT, CLIP_BOTTOM, CLIP_TOP ), "line intersect bottom left") &
            assertEquals(CONST_EXP_LINE_INTERSECT_BOTTOM_LEFT_P0_X, line_intersect_bottom_left_p0.x, "line intersect bottom left p0.x") &
            assertEquals(CONST_EXP_LINE_INTERSECT_BOTTOM_LEFT_P0_Y, line_intersect_bottom_left_p0.y, "line intersect bottom left p0.y") &
            assertEquals(CONST_LINE_INTERSECT_BOTTOM_LEFT_P1.x, line_intersect_bottom_left_p1.x, "line intersect bottom left p1.x") &
            assertEquals(CONST_LINE_INTERSECT_BOTTOM_LEFT_P1.y, line_intersect_bottom_left_p1.y, "line intersect bottom left p1.y");


        // should be immediate trivial reject without update of coordinates
        b = b &
            assertFalse( PUREScissor::DoScissor(
                            line_outside_region_bottom_left_p0.x, line_outside_region_bottom_left_p0.y,
                            line_outside_region_bottom_left_p1.x, line_outside_region_bottom_left_p1.y,
                            CLIP_LEFT, CLIP_RIGHT, CLIP_BOTTOM, CLIP_TOP ), "line outside region bottom left") &
            assertEquals(CONST_LINE_OUTSIDE_REGION_BOTTOM_LEFT_P0.x, line_outside_region_bottom_left_p0.x, "line outside region bottom left p0.x") &
            assertEquals(CONST_LINE_OUTSIDE_REGION_BOTTOM_LEFT_P0.y, line_outside_region_bottom_left_p0.y, "line outside region bottom left p0.y") &
            assertEquals(CONST_LINE_OUTSIDE_REGION_BOTTOM_LEFT_P1.x, line_outside_region_bottom_left_p1.x, "line outside region bottom left p1.x") &
            assertEquals(CONST_LINE_OUTSIDE_REGION_BOTTOM_LEFT_P1.y, line_outside_region_bottom_left_p1.y, "line outside region bottom left p1.y");


        // should be immediate trivial reject without update of coordinates
        b = b &
            assertFalse( PUREScissor::DoScissor(
                            line_outside_region_top_right_p0.x, line_outside_region_top_right_p0.y,
                            line_outside_region_top_right_p1.x, line_outside_region_top_right_p1.y,
                            CLIP_LEFT, CLIP_RIGHT, CLIP_BOTTOM, CLIP_TOP ), "line outside region top right") &
            assertEquals(CONST_LINE_OUTSIDE_REGION_TOP_RIGHT_P0.x, line_outside_region_top_right_p0.x, "line outside region top right p0.x") &
            assertEquals(CONST_LINE_OUTSIDE_REGION_TOP_RIGHT_P0.y, line_outside_region_top_right_p0.y, "line outside region top right p0.y") &
            assertEquals(CONST_LINE_OUTSIDE_REGION_TOP_RIGHT_P1.x, line_outside_region_top_right_p1.x, "line outside region top right p1.x") &
            assertEquals(CONST_LINE_OUTSIDE_REGION_TOP_RIGHT_P1.y, line_outside_region_top_right_p1.y, "line outside region top right p1.y");

        return b;
    }

}; // class PureScissorTest
