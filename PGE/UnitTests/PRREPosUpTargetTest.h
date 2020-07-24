#pragma once

/*
    ###################################################################################
    PRREPosUpTargetTest.h
    Unit test for PRREPosUpTarget.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../../../PGE/PGE/PRRE/PRREPosUpTarget.h"
#include "../../../PGE/PGE/PRRE/PRRETransformMatrix.h"

#ifndef E2
#define E2 0.005f
#endif // EPSILON

class PRREPosUpTargetTest :
    public UnitTest
{
public:

    PRREPosUpTargetTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRREPosUpTargetTest::testCtor1);
        AddSubTest("testCopyCtor", (PFNUNITSUBTEST) &PRREPosUpTargetTest::testCopyCtor);
        AddSubTest("testMove", (PFNUNITSUBTEST) &PRREPosUpTargetTest::testMove);
        AddSubTest("testStrafe", (PFNUNITSUBTEST) &PRREPosUpTargetTest::testStrafe);
        AddSubTest("testElevate", (PFNUNITSUBTEST) &PRREPosUpTargetTest::testElevate);
        AddSubTest("testRotation", (PFNUNITSUBTEST) &PRREPosUpTargetTest::testRotation);

    } // PRREPosUpTargetTest()

protected:

    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

private:

    // ---------------------------------------------------------------------------

    PRREPosUpTargetTest(const PRREPosUpTargetTest&)
    {};         

    PRREPosUpTargetTest& operator=(const PRREPosUpTargetTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PRREPosUpTarget put;
        const PRREVector vUp(0, 1, 0);
        const PRREVector vForward(0, 0, 1);
        const PRREVector vPos(0, 0, 0);

        return assertTrue(vUp == put.getUpVec(), "up") &
            assertTrue(vForward == put.getTargetVec(), "target") &
            assertTrue(vPos == put.getPosVec(), "pos");
    }

    bool testCopyCtor()
    {
        PRRECamera put;
        put.getPosVec().Set(1, 2, 3);
        put.getTargetVec().Set(1, 2, 3);
        put.getUpVec().Set(1, 2, 3);

        const PRRECamera put2(put);

        return assertTrue(put.getUpVec() == put2.getUpVec(), "up") &
            assertTrue(put.getTargetVec() == put2.getTargetVec(), "target") &
            assertTrue(put.getPosVec() == put2.getPosVec(), "pos");
    }

    bool testMove()
    {
        const float MOVE = 5.f;
        const float ROT_X = 45.f;
        const float ROT_Y = 45.f;

        PRREPosUpTarget put;

        PRREVector vTargetOrig = put.getTargetVec();
        PRREVector vPosOrig    = put.getPosVec();
        PRREVector vUpOrig     = put.getUpVec();

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

        PRREVector vView = vTargetOrig - vPosOrig;
        vView.Normalize();
        const PRREVector vExpTarget = vTargetOrig + vView * MOVE;
        const PRREVector vExpPos    = vPosOrig + vView * MOVE;
        const PRREVector vExpUp     = vUpOrig;
        
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

        PRREPosUpTarget put;

        PRREVector vTargetOrig = put.getTargetVec();
        PRREVector vPosOrig    = put.getPosVec();
        PRREVector vUpOrig     = put.getUpVec();

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

        PRREVector vView = vTargetOrig - vPosOrig;
        vView.Normalize();
        PRREVector vStrafe = vUpOrig ^ vView;
        vStrafe.Normalize();
        const PRREVector vExpTarget = vTargetOrig + vStrafe * MOVE;
        const PRREVector vExpPos    = vPosOrig + vStrafe * MOVE;
        const PRREVector vExpUp     = vUpOrig;
        
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

        PRREPosUpTarget put;

        PRREVector vTargetOrig = put.getTargetVec();
        PRREVector vPosOrig    = put.getPosVec();
        PRREVector vUpOrig     = put.getUpVec();

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

        PRREPosUpTarget put;

        PRREVector vTargetOrig = put.getTargetVec();
        PRREVector vPosOrig    = put.getPosVec();
        PRREVector vUpOrig     = put.getUpVec();

        PRREVector vView = vTargetOrig - vPosOrig;
        vView.Normalize();
        PRRETransformMatrix mRotX, mRotY;
        mRotX.SetRotationX(ROT_X);
        mRotY.SetRotationY(ROT_Y);
        vView *= mRotX * mRotY;

        const PRREVector vExpTarget = vPosOrig + vView;
        const PRREVector vExpPos    = vPosOrig;
        const PRREVector vExpUp     = vUpOrig;
        
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

}; // class PRREPosUpTargetTest