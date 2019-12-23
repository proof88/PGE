#pragma once

/*
    ###################################################################################
    PRRECameraTest.h
    Unit test for PRRECameraTest.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PGE/PRRE/PRRECamera.h"

#ifndef E2
#define E2 0.005f
#endif // EPSILON

class PRRECameraTest :
    public UnitTest
{
public:

    PRRECameraTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor1", (PFNUNITSUBTEST) &PRRECameraTest::testCtor1);
        AddSubTest("testCopyCtor", (PFNUNITSUBTEST) &PRRECameraTest::testCopyCtor);
        AddSubTest("testSetviewPort", (PFNUNITSUBTEST) &PRRECameraTest::testSetviewPort);
        AddSubTest("testSetAspectRatioMode", (PFNUNITSUBTEST) &PRRECameraTest::testSetAspectRatioMode);
        AddSubTest("testSetAspectRatio", (PFNUNITSUBTEST) &PRRECameraTest::testSetAspectRatio);
        AddSubTest("testSetClearMode", (PFNUNITSUBTEST) &PRRECameraTest::testSetClearMode);
        AddSubTest("testSetNearPlane", (PFNUNITSUBTEST) &PRRECameraTest::testSetNearPlane);
        AddSubTest("testSetFarPlane", (PFNUNITSUBTEST) &PRRECameraTest::testSetFarPlane);
        AddSubTest("testSetFieldOfView", (PFNUNITSUBTEST) &PRRECameraTest::testSetFieldOfView);
        AddSubTest("testMove", (PFNUNITSUBTEST) &PRRECameraTest::testMove);
        AddSubTest("testStrafe", (PFNUNITSUBTEST) &PRRECameraTest::testStrafe);
        AddSubTest("testElevate", (PFNUNITSUBTEST) &PRRECameraTest::testElevate);
        AddSubTest("testRotation", (PFNUNITSUBTEST) &PRRECameraTest::testRotation);

    } // PRRECameraTest()

protected:

    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {

    }

private:

    // ---------------------------------------------------------------------------

    PRRECameraTest(const PRRECameraTest&)
    {};         

    PRRECameraTest& operator=(const PRRECameraTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PRRECamera cam;
        const PRREVector vUp(0, 1, 0);
        const PRREVector vForward(0, 0, 1);
        const PRREVector vPos(0, 0, 0);
        const PRREColor  clrBg(0, 0, 0);

        return assertEquals(0.0f, cam.getViewport().pos.x, "rect x") &
            assertEquals(0.0f, cam.getViewport().pos.y, "rect y") &
            assertGreater(cam.getViewport().size.width, 0.0f, "rect sx") &
            assertGreater(cam.getViewport().size.height, 0.0f, "rect sy") &
            assertTrue(vUp == cam.getUpVec(), "up") &
            assertTrue(vForward == cam.getTargetVec(), "target") &
            assertTrue(vPos == cam.getPosVec(), "pos") &
            assertEquals(cam.getViewport().size.width / cam.getViewport().size.height, cam.getAspectRatio(), "ratio") &
            assertEquals(TPRRE_ASPECTRATIO_MODE::PRRE_AM_FIX, cam.getAspectRatioMode(), "ratio mode") &
            assertTrue(clrBg == cam.getBackgroundColor(), "bg color") &
            assertEquals(PRRE_CLEAR_ZBUFFER_COLORBUFFER, cam.getClearMode(), "clear mode") &
            assertTrue(cam.getNearPlane() > 0.0f, "nearplane") &
            assertTrue(cam.getFarPlane() > cam.getNearPlane(), "farplane") &
            assertTrue(cam.getFieldOfView() > 0.0f && cam.getFieldOfView() <= 180.0f, "fov");
    }

    bool testCopyCtor()
    {
        PRRECamera cam;
        cam.getPosVec().Set(1, 2, 3);
        cam.getTargetVec().Set(1, 2, 3);
        cam.getUpVec().Set(1, 2, 3);
        cam.SetAspectRatio(3);
        cam.SetAspectRatioMode(PRRE_AM_DYNAMIC);
        cam.SetClearMode(PRRE_CLEAR_ZBUFFER);
        cam.SetFarPlane(20);
        cam.SetNearPlane(2);
        cam.SetFieldOfView(50);
        cam.SetRotation(1, 2, 3);
        cam.SetViewport(1, 1, 2, 2);
        cam.getBackgroundColor().Set(255, 255, 255, 255);

        const PRRECamera cam2(cam);

        return assertEquals(cam.getViewport().pos.x, cam2.getViewport().pos.x, "rect x") &
            assertEquals(cam.getViewport().pos.y, cam2.getViewport().pos.y, "rect y") &
            assertEquals(cam.getViewport().size.width, cam2.getViewport().size.width, "rect sx") &
            assertEquals(cam.getViewport().size.height,  cam2.getViewport().size.height, "rect sy") &
            assertTrue(cam.getUpVec() == cam2.getUpVec(), "up") &
            assertTrue(cam.getTargetVec() == cam2.getTargetVec(), "target") &
            assertTrue(cam.getPosVec() == cam2.getPosVec(), "pos") &
            assertEquals(cam.getAspectRatio(), cam2.getAspectRatio(), "ratio") &
            assertEquals(cam.getAspectRatioMode(), cam2.getAspectRatioMode(), "ratio mode") &
            assertTrue(cam.getBackgroundColor() == cam2.getBackgroundColor(), "bg color") &
            assertEquals(cam.getClearMode(), cam2.getClearMode(), "clear mode") &
            assertEquals(cam.getNearPlane(), cam2.getNearPlane(), "nearplane") &
            assertEquals(cam.getFarPlane(), cam2.getFarPlane(), "farplane") &
            assertEquals(cam.getFieldOfView(), cam2.getFieldOfView(), "fov");
    }

    bool testSetviewPort()
    {
        PRRECamera cam;
        cam.SetViewport(1, 1, 2, 2);

        bool b = assertEquals(1, cam.getViewport().pos.x, "rect x") &
               assertEquals(1, cam.getViewport().pos.y, "rect y") &
               assertEquals(2, cam.getViewport().size.width, "rect sx") &
               assertEquals(2, cam.getViewport().size.height, "rect sy");

        cam.SetViewport(0, 0, 2, 2);
        b = b & assertEquals(0, cam.getViewport().pos.x, "rect x 2 0") &
               assertEquals(0, cam.getViewport().pos.y, "rect y 2 0") &
               assertEquals(2, cam.getViewport().size.width, "rect sx 2 0") &
               assertEquals(2, cam.getViewport().size.height, "rect sy 2 0");

        cam.SetViewport(0, 0, 0, 2);
        b = b & assertEquals(0, cam.getViewport().pos.x, "rect x 3 0") &
               assertEquals(0, cam.getViewport().pos.y, "rect y 3 0") &
               assertEquals(2, cam.getViewport().size.width, "rect sx 3 0") &
               assertEquals(2, cam.getViewport().size.height, "rect sy 3 0");

        cam.SetViewport(0, 0, 2, 0);
        b = b & assertEquals(0, cam.getViewport().pos.x, "rect x 4 0") &
               assertEquals(0, cam.getViewport().pos.y, "rect y 4 0") &
               assertEquals(2, cam.getViewport().size.width, "rect sx 4 0") &
               assertEquals(2, cam.getViewport().size.height, "rect sy 4 0");

        return b;
    }

    bool testSetAspectRatioMode()
    {
        PRRECamera cam;
        cam.SetAspectRatioMode(PRRE_AM_DYNAMIC);

        return assertEquals(PRRE_AM_DYNAMIC, cam.getAspectRatioMode());
    }

    bool testSetAspectRatio()
    {
        PRRECamera cam;
        cam.SetAspectRatio(3.f);

        return assertEquals(3.f, cam.getAspectRatio());
    }

    bool testSetClearMode()
    {
        PRRECamera cam;
        cam.SetClearMode(PRRE_CLEAR_ZBUFFER);

        return assertEquals(PRRE_CLEAR_ZBUFFER, cam.getClearMode());
    }

    bool testSetNearPlane()
    {
        PRRECamera cam;
        cam.SetNearPlane(2);

        bool b = assertEquals(2.f, cam.getNearPlane(), "np 2");

        cam.SetNearPlane(cam.getFarPlane());
        b = b & assertEquals( 2.f, cam.getNearPlane(), "np far1");

        cam.SetNearPlane(cam.getFarPlane() + 1.f);
        b = b & assertEquals( 2.f, cam.getNearPlane(), "np far2");

        cam.SetNearPlane(0.f);
        b = b & assertEquals( 2.f, cam.getNearPlane(), "np 0");

        cam.SetNearPlane(-5.f);
        b = b & assertEquals( 2.f, cam.getNearPlane(), "np -5");

        return b;
    }

    bool testSetFarPlane()
    {
        PRRECamera cam;
        cam.SetFarPlane(20.f);

        bool b = assertEquals(20.f, cam.getFarPlane(), "fp 20");
        
        cam.SetFarPlane(cam.getNearPlane());
        b = b & assertEquals(20.f, cam.getFarPlane(), "fp np1");

        cam.SetFarPlane(cam.getNearPlane() - 1.f);
        b = b & assertEquals(20.f, cam.getFarPlane(), "fp np2");

        cam.SetFarPlane(0.f);
        b = b & assertEquals(20.f, cam.getFarPlane(), "fp 0");

        cam.SetFarPlane(-5.f);
        b = b & assertEquals(20.f, cam.getFarPlane(), "fp -5");

        return b;
    }

    bool testSetFieldOfView()
    {
        PRRECamera cam;
        PRRECamera cam2;
        
        cam.SetFieldOfView(50);
        bool b = assertEquals(50, cam.getFieldOfView());

        const float fFOV = cam2.getFieldOfView();

        cam2.SetFieldOfView(0);
        b = b & assertEquals(fFOV, cam2.getFieldOfView(), "fov 0");

        cam2.SetFieldOfView(-5);
        b = b & assertEquals(fFOV, cam2.getFieldOfView(), "fov -5");

        cam2.SetFieldOfView(181);
        b = b & assertEquals(fFOV, cam2.getFieldOfView(), "fov 181");

        cam2.SetFieldOfView(180.f);
        b = b & assertEquals(180.f, cam2.getFieldOfView(), "fov 180");

        return b;
    }

    bool testMove()
    {
        const float MOVE = 5.f;
        const float ROT_X = 45.f;
        const float ROT_Y = 45.f;

        PRRECamera cam;

        PRREVector vTargetOrig = cam.getTargetVec();
        PRREVector vPosOrig    = cam.getPosVec();
        PRREVector vUpOrig     = cam.getUpVec();

        cam.Move(MOVE);

        bool b = assertEquals(vTargetOrig.getX(),        cam.getTargetVec().getX(), E2, "target x 1") &
                 assertEquals(vTargetOrig.getY(),        cam.getTargetVec().getY(), E2, "target y 1") &
                 assertEquals(vTargetOrig.getZ() + MOVE, cam.getTargetVec().getZ(), E2, "target z 1") &
                 assertEquals(vPosOrig.getX(),           cam.getPosVec().getX(), E2, "pos x 1") &
                 assertEquals(vPosOrig.getY(),           cam.getPosVec().getY(), E2, "pos y 1") &
                 assertEquals(vPosOrig.getZ() + MOVE,    cam.getPosVec().getZ(), E2, "pos z 1") &
                 assertEquals(vUpOrig.getX(),            cam.getUpVec().getX(), E2, "up x 1") &
                 assertEquals(vUpOrig.getY(),            cam.getUpVec().getY(), E2, "up y 1") &
                 assertEquals(vUpOrig.getZ(),            cam.getUpVec().getZ(), E2, "up z 1");

        cam.SetRotation(ROT_X, ROT_Y, 0);

        vTargetOrig = cam.getTargetVec();
        vPosOrig    = cam.getPosVec();
        vUpOrig     = cam.getUpVec();

        PRREVector vView = vTargetOrig - vPosOrig;
        vView.Normalize();
        const PRREVector vExpTarget = vTargetOrig + vView * MOVE;
        const PRREVector vExpPos    = vPosOrig + vView * MOVE;
        const PRREVector vExpUp     = vUpOrig;
        
        cam.Move(MOVE);
                                                                   
        b = b & assertEquals(vExpTarget.getX(), cam.getTargetVec().getX(), E2, "target x 2") &
                assertEquals(vExpTarget.getY(), cam.getTargetVec().getY(), E2, "target y 2") &
                assertEquals(vExpTarget.getZ(), cam.getTargetVec().getZ(), E2, "target z 2") &
                assertEquals(vExpPos.getX(),    cam.getPosVec().getX(), E2, "pos x 2") &
                assertEquals(vExpPos.getY(),    cam.getPosVec().getY(), E2, "pos y 2") &
                assertEquals(vExpPos.getZ(),    cam.getPosVec().getZ(), E2, "pos z 2") &
                assertEquals(vExpUp.getX(),     cam.getUpVec().getX(), E2, "up x 2") &
                assertEquals(vExpUp.getY(),     cam.getUpVec().getY(), E2, "up y 2") &
                assertEquals(vExpUp.getZ(),     cam.getUpVec().getZ(), E2, "up z 2");

        return b;
    }

    bool testStrafe()
    {
        const float MOVE = 5.f;
        const float ROT_X = 45.f;
        const float ROT_Y = 45.f;

        PRRECamera cam;

        PRREVector vTargetOrig = cam.getTargetVec();
        PRREVector vPosOrig    = cam.getPosVec();
        PRREVector vUpOrig     = cam.getUpVec();

        cam.Strafe(MOVE);

        bool b = assertEquals(vTargetOrig.getX() + MOVE, cam.getTargetVec().getX(), E2, "target x 1") &
                 assertEquals(vTargetOrig.getY(),        cam.getTargetVec().getY(), E2, "target y 1") &
                 assertEquals(vTargetOrig.getZ(),        cam.getTargetVec().getZ(), E2, "target z 1") &
                 assertEquals(vPosOrig.getX() + MOVE,    cam.getPosVec().getX(), E2, "pos x 1") &
                 assertEquals(vPosOrig.getY(),           cam.getPosVec().getY(), E2, "pos y 1") &
                 assertEquals(vPosOrig.getZ(),           cam.getPosVec().getZ(), E2, "pos z 1") &
                 assertEquals(vUpOrig.getX(),            cam.getUpVec().getX(), E2, "up x 1") &
                 assertEquals(vUpOrig.getY(),            cam.getUpVec().getY(), E2, "up y 1") &
                 assertEquals(vUpOrig.getZ(),            cam.getUpVec().getZ(), E2, "up z 1");

        cam.SetRotation(ROT_X, ROT_Y, 0);

        vTargetOrig = cam.getTargetVec();
        vPosOrig    = cam.getPosVec();
        vUpOrig     = cam.getUpVec();

        PRREVector vView = vTargetOrig - vPosOrig;
        vView.Normalize();
        PRREVector vStrafe = vUpOrig ^ vView;
        vStrafe.Normalize();
        const PRREVector vExpTarget = vTargetOrig + vStrafe * MOVE;
        const PRREVector vExpPos    = vPosOrig + vStrafe * MOVE;
        const PRREVector vExpUp     = vUpOrig;
        
        cam.Strafe(MOVE);
                                                                   
        b = b & assertEquals(vExpTarget.getX(), cam.getTargetVec().getX(), E2, "target x 2") &
                assertEquals(vExpTarget.getY(), cam.getTargetVec().getY(), E2, "target y 2") &
                assertEquals(vExpTarget.getZ(), cam.getTargetVec().getZ(), E2, "target z 2") &
                assertEquals(vExpPos.getX(),    cam.getPosVec().getX(), E2, "pos x 2") &
                assertEquals(vExpPos.getY(),    cam.getPosVec().getY(), E2, "pos y 2") &
                assertEquals(vExpPos.getZ(),    cam.getPosVec().getZ(), E2, "pos z 2") &
                assertEquals(vExpUp.getX(),     cam.getUpVec().getX(), E2, "up x 2") &
                assertEquals(vExpUp.getY(),     cam.getUpVec().getY(), E2, "up y 2") &
                assertEquals(vExpUp.getZ(),     cam.getUpVec().getZ(), E2, "up z 2");

        return b;
    }

    bool testElevate()
    {
        const float MOVE = 5.f;

        PRRECamera cam;

        PRREVector vTargetOrig = cam.getTargetVec();
        PRREVector vPosOrig    = cam.getPosVec();
        PRREVector vUpOrig     = cam.getUpVec();

        cam.Elevate(MOVE);

        return assertEquals(vTargetOrig.getX(),        cam.getTargetVec().getX(), E2, "target x") &
                 assertEquals(vTargetOrig.getY() + MOVE, cam.getTargetVec().getY(), E2, "target y") &
                 assertEquals(vTargetOrig.getZ(),        cam.getTargetVec().getZ(), E2, "target z") &
                 assertEquals(vPosOrig.getX(),           cam.getPosVec().getX(), E2, "pos x") &
                 assertEquals(vPosOrig.getY() + MOVE,    cam.getPosVec().getY(), E2, "pos y") &
                 assertEquals(vPosOrig.getZ(),           cam.getPosVec().getZ(), E2, "pos z") &
                 assertEquals(vUpOrig.getX(),            cam.getUpVec().getX(), E2, "up x") &
                 assertEquals(vUpOrig.getY(),            cam.getUpVec().getY(), E2, "up y") &
                 assertEquals(vUpOrig.getZ(),            cam.getUpVec().getZ(), E2, "up z");
    }

    bool testRotation()
    {
        const float ROT_X = 45.f;
        const float ROT_Y = 45.f;

        PRRECamera cam;

        PRREVector vTargetOrig = cam.getTargetVec();
        PRREVector vPosOrig    = cam.getPosVec();
        PRREVector vUpOrig     = cam.getUpVec();

        PRREVector vView = vTargetOrig - vPosOrig;
        vView.Normalize();
        PRREMatrix mRotX, mRotY;
        mRotX.SetRotationX(ROT_X);
        mRotY.SetRotationY(ROT_Y);
        vView *= mRotX * mRotY;

        const PRREVector vExpTarget = vPosOrig + vView;
        const PRREVector vExpPos    = vPosOrig;
        const PRREVector vExpUp     = vUpOrig;
        
        cam.SetRotation(ROT_X, ROT_Y, 0);
                                                                   
        return assertEquals(vExpTarget.getX(), cam.getTargetVec().getX(), E2, "target x") &
                assertEquals(vExpTarget.getY(), cam.getTargetVec().getY(), E2, "target y") &
                assertEquals(vExpTarget.getZ(), cam.getTargetVec().getZ(), E2, "target z") &
                assertEquals(vExpPos.getX(),    cam.getPosVec().getX(), E2, "pos x") &
                assertEquals(vExpPos.getY(),    cam.getPosVec().getY(), E2, "pos y") &
                assertEquals(vExpPos.getZ(),    cam.getPosVec().getZ(), E2, "pos z") &
                assertEquals(vExpUp.getX(),     cam.getUpVec().getX(), E2, "up x") &
                assertEquals(vExpUp.getY(),     cam.getUpVec().getY(), E2, "up y") &
                assertEquals(vExpUp.getZ(),     cam.getUpVec().getZ(), E2, "up z");
    }

}; // class PRRECameraTest