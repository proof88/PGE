#pragma once

/*
    ###################################################################################
    PRRECameraTest.h
    Unit test for PRRECamera.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/PRRECamera.h"
#include "../PRRE/include/external/Math/PRREMatrix.h"

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

    } // PRREPosUpTargetTest()

protected:

    virtual bool setUp()
    {
        return true;
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

}; // class PRRECameraTest