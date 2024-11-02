#pragma once

/*
    ###################################################################################
    PurePosUpTargetTest.h
    Unit test for PurePosUpTarget.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Math/PurePosUpTarget.h"
#include "../Pure/include/external/Math/PureTransformMatrix.h"

#ifndef E2
#define E2 0.005f
#endif // EPSILON

class PurePosUpTargetTest :
    public UnitTest
{
public:

    PurePosUpTargetTest() :
        UnitTest( __FILE__ )
    {
        addSubTest("testCtor1", (PFNUNITSUBTEST) &PurePosUpTargetTest::testCtor1);
        addSubTest("testCopyCtor", (PFNUNITSUBTEST) &PurePosUpTargetTest::testCopyCtor);
        addSubTest("testMove", (PFNUNITSUBTEST) &PurePosUpTargetTest::testMove);
        addSubTest("testStrafe", (PFNUNITSUBTEST) &PurePosUpTargetTest::testStrafe);
        addSubTest("testElevate", (PFNUNITSUBTEST) &PurePosUpTargetTest::testElevate);
        addSubTest("testRotation", (PFNUNITSUBTEST) &PurePosUpTargetTest::testRotation);

    } // PurePosUpTargetTest()

protected:

    virtual bool setUp() override
    {
        return true;
    }

    virtual void tearDown() override
    {

    }

private:

    // ---------------------------------------------------------------------------

    PurePosUpTargetTest(const PurePosUpTargetTest&)
    {};         

    PurePosUpTargetTest& operator=(const PurePosUpTargetTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PurePosUpTarget put;
        const PureVector vUp(0, 1, 0);
        const PureVector vForward(0, 0, 1);
        const PureVector vPos(0, 0, 0);

        return assertTrue(vUp == put.getUpVec(), "up") &
            assertTrue(vForward == put.getTargetVec(), "target") &
            assertTrue(vPos == put.getPosVec(), "pos");
    }

    bool testCopyCtor()
    {
        PurePosUpTarget put;
        put.getPosVec().Set(1, 2, 3);
        put.getTargetVec().Set(1, 2, 3);
        put.getUpVec().Set(1, 2, 3);

        const PurePosUpTarget put2(put);

        return assertTrue(put.getUpVec() == put2.getUpVec(), "up") &
            assertTrue(put.getTargetVec() == put2.getTargetVec(), "target") &
            assertTrue(put.getPosVec() == put2.getPosVec(), "pos");
    }

    bool testMove()
    {
        const float MOVE = 5.f;
        const float ROT_X = 45.f;
        const float ROT_Y = 45.f;

        PurePosUpTarget put;

        PureVector vTargetOrig = put.getTargetVec();
        PureVector vPosOrig    = put.getPosVec();
        PureVector vUpOrig     = put.getUpVec();

        put.Move(MOVE);

        bool b = assertEquals(vTargetOrig.getX(),        put.getTargetVec().getX(), E2, "target x 1") &
                 assertEquals(vTargetOrig.getY(),        put.getTargetVec().getY(), E2, "target y 1") &
                 assertEquals(vTargetOrig.getZ() + MOVE, put.getTargetVec().getZ(), E2, "target z 1") &
                 assertEquals(vPosOrig.getX(),           put.getPosVec().getX(), E2, "pos x 1") &
                 assertEquals(vPosOrig.getY(),           put.getPosVec().getY(), E2, "pos y 1") &
                 assertEquals(vPosOrig.getZ() + MOVE,    put.getPosVec().getZ(), E2, "pos z 1") &
                 assertEquals(vUpOrig.getX(),            put.getUpVec().getX(), E2, "up x 1") &
                 assertEquals(vUpOrig.getY(),            put.getUpVec().getY(), E2, "up y 1") &
                 assertEquals(vUpOrig.getZ(),            put.getUpVec().getZ(), E2, "up z 1");

        put.SetRotation(ROT_X, ROT_Y, 0);

        vTargetOrig = put.getTargetVec();
        vPosOrig    = put.getPosVec();
        vUpOrig     = put.getUpVec();

        PureVector vView = vTargetOrig - vPosOrig;
        vView.Normalize();
        const PureVector vExpTarget = vTargetOrig + vView * MOVE;
        const PureVector vExpPos    = vPosOrig + vView * MOVE;
        const PureVector vExpUp     = vUpOrig;
        
        put.Move(MOVE);
                                                                   
        b = b & assertEquals(vExpTarget.getX(), put.getTargetVec().getX(), E2, "target x 2") &
                assertEquals(vExpTarget.getY(), put.getTargetVec().getY(), E2, "target y 2") &
                assertEquals(vExpTarget.getZ(), put.getTargetVec().getZ(), E2, "target z 2") &
                assertEquals(vExpPos.getX(),    put.getPosVec().getX(), E2, "pos x 2") &
                assertEquals(vExpPos.getY(),    put.getPosVec().getY(), E2, "pos y 2") &
                assertEquals(vExpPos.getZ(),    put.getPosVec().getZ(), E2, "pos z 2") &
                assertEquals(vExpUp.getX(),     put.getUpVec().getX(), E2, "up x 2") &
                assertEquals(vExpUp.getY(),     put.getUpVec().getY(), E2, "up y 2") &
                assertEquals(vExpUp.getZ(),     put.getUpVec().getZ(), E2, "up z 2");

        return b;
    }

    bool testStrafe()
    {
        const float MOVE = 5.f;
        const float ROT_X = 45.f;
        const float ROT_Y = 45.f;

        PurePosUpTarget put;

        PureVector vTargetOrig = put.getTargetVec();
        PureVector vPosOrig    = put.getPosVec();
        PureVector vUpOrig     = put.getUpVec();

        put.Strafe(MOVE);

        bool b = assertEquals(vTargetOrig.getX() + MOVE, put.getTargetVec().getX(), E2, "target x 1") &
                 assertEquals(vTargetOrig.getY(),        put.getTargetVec().getY(), E2, "target y 1") &
                 assertEquals(vTargetOrig.getZ(),        put.getTargetVec().getZ(), E2, "target z 1") &
                 assertEquals(vPosOrig.getX() + MOVE,    put.getPosVec().getX(), E2, "pos x 1") &
                 assertEquals(vPosOrig.getY(),           put.getPosVec().getY(), E2, "pos y 1") &
                 assertEquals(vPosOrig.getZ(),           put.getPosVec().getZ(), E2, "pos z 1") &
                 assertEquals(vUpOrig.getX(),            put.getUpVec().getX(), E2, "up x 1") &
                 assertEquals(vUpOrig.getY(),            put.getUpVec().getY(), E2, "up y 1") &
                 assertEquals(vUpOrig.getZ(),            put.getUpVec().getZ(), E2, "up z 1");

        put.SetRotation(ROT_X, ROT_Y, 0);

        vTargetOrig = put.getTargetVec();
        vPosOrig    = put.getPosVec();
        vUpOrig     = put.getUpVec();

        PureVector vView = vTargetOrig - vPosOrig;
        vView.Normalize();
        PureVector vStrafe = vUpOrig ^ vView;
        vStrafe.Normalize();
        const PureVector vExpTarget = vTargetOrig + vStrafe * MOVE;
        const PureVector vExpPos    = vPosOrig + vStrafe * MOVE;
        const PureVector vExpUp     = vUpOrig;
        
        put.Strafe(MOVE);
                                                                   
        b = b & assertEquals(vExpTarget.getX(), put.getTargetVec().getX(), E2, "target x 2") &
                assertEquals(vExpTarget.getY(), put.getTargetVec().getY(), E2, "target y 2") &
                assertEquals(vExpTarget.getZ(), put.getTargetVec().getZ(), E2, "target z 2") &
                assertEquals(vExpPos.getX(),    put.getPosVec().getX(), E2, "pos x 2") &
                assertEquals(vExpPos.getY(),    put.getPosVec().getY(), E2, "pos y 2") &
                assertEquals(vExpPos.getZ(),    put.getPosVec().getZ(), E2, "pos z 2") &
                assertEquals(vExpUp.getX(),     put.getUpVec().getX(), E2, "up x 2") &
                assertEquals(vExpUp.getY(),     put.getUpVec().getY(), E2, "up y 2") &
                assertEquals(vExpUp.getZ(),     put.getUpVec().getZ(), E2, "up z 2");

        return b;
    }

    bool testElevate()
    {
        const float MOVE = 5.f;

        PurePosUpTarget put;

        PureVector vTargetOrig = put.getTargetVec();
        PureVector vPosOrig    = put.getPosVec();
        PureVector vUpOrig     = put.getUpVec();

        put.Elevate(MOVE);

        return assertEquals(vTargetOrig.getX(),        put.getTargetVec().getX(), E2, "target x") &
                 assertEquals(vTargetOrig.getY() + MOVE, put.getTargetVec().getY(), E2, "target y") &
                 assertEquals(vTargetOrig.getZ(),        put.getTargetVec().getZ(), E2, "target z") &
                 assertEquals(vPosOrig.getX(),           put.getPosVec().getX(), E2, "pos x") &
                 assertEquals(vPosOrig.getY() + MOVE,    put.getPosVec().getY(), E2, "pos y") &
                 assertEquals(vPosOrig.getZ(),           put.getPosVec().getZ(), E2, "pos z") &
                 assertEquals(vUpOrig.getX(),            put.getUpVec().getX(), E2, "up x") &
                 assertEquals(vUpOrig.getY(),            put.getUpVec().getY(), E2, "up y") &
                 assertEquals(vUpOrig.getZ(),            put.getUpVec().getZ(), E2, "up z");
    }

    bool testRotation()
    {
        const float ROT_X = 45.f;
        const float ROT_Y = 45.f;

        PurePosUpTarget put;

        PureVector vTargetOrig = put.getTargetVec();
        PureVector vPosOrig    = put.getPosVec();
        PureVector vUpOrig     = put.getUpVec();

        PureVector vView = vTargetOrig - vPosOrig;
        vView.Normalize();
        PureTransformMatrix mRotX, mRotY;
        mRotX.SetRotationX(ROT_X);
        mRotY.SetRotationY(ROT_Y);
        vView *= mRotX * mRotY;

        const PureVector vExpTarget = vPosOrig + vView;
        const PureVector vExpPos    = vPosOrig;
        const PureVector vExpUp     = vUpOrig;
        
        put.SetRotation(ROT_X, ROT_Y, 0);
                                                                   
        return assertEquals(vExpTarget.getX(), put.getTargetVec().getX(), E2, "target x") &
                assertEquals(vExpTarget.getY(), put.getTargetVec().getY(), E2, "target y") &
                assertEquals(vExpTarget.getZ(), put.getTargetVec().getZ(), E2, "target z") &
                assertEquals(vExpPos.getX(),    put.getPosVec().getX(), E2, "pos x") &
                assertEquals(vExpPos.getY(),    put.getPosVec().getY(), E2, "pos y") &
                assertEquals(vExpPos.getZ(),    put.getPosVec().getZ(), E2, "pos z") &
                assertEquals(vExpUp.getX(),     put.getUpVec().getX(), E2, "up x") &
                assertEquals(vExpUp.getY(),     put.getUpVec().getY(), E2, "up y") &
                assertEquals(vExpUp.getZ(),     put.getUpVec().getZ(), E2, "up z");
    }

}; // class PurePosUpTargetTest