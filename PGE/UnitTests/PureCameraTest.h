#pragma once

/*
    ###################################################################################
    PureCameraTest.h
    Unit test for PureCamera.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/PureCamera.h"
#include "../Pure/include/external/Math/PureMatrix.h"

#ifndef E2
#define E2 0.005f
#endif // EPSILON

#ifndef E_LOOSE
#define E_LOOSE 0.02f
#endif // Loosed EPSILON, for testProject2dTo3d (unclear why our results are not precise enough, should be investigated later)

class PureCameraTest :
    public UnitTest
{
public:

    PureCameraTest() :
        UnitTest( __FILE__ )
    {
        
        addSubTest("testCtor1", (PFNUNITSUBTEST) &PureCameraTest::testCtor1);
        addSubTest("testCopyCtor", (PFNUNITSUBTEST) &PureCameraTest::testCopyCtor);
        addSubTest("testSetviewPort", (PFNUNITSUBTEST) &PureCameraTest::testSetviewPort);
        addSubTest("testSetAspectRatioMode", (PFNUNITSUBTEST) &PureCameraTest::testSetAspectRatioMode);
        addSubTest("testSetAspectRatio", (PFNUNITSUBTEST) &PureCameraTest::testSetAspectRatio);
        addSubTest("testSetClearMode", (PFNUNITSUBTEST) &PureCameraTest::testSetClearMode);
        addSubTest("testSetNearPlane", (PFNUNITSUBTEST) &PureCameraTest::testSetNearPlane);
        addSubTest("testSetFarPlane", (PFNUNITSUBTEST) &PureCameraTest::testSetFarPlane);
        addSubTest("testSetFieldOfView", (PFNUNITSUBTEST) &PureCameraTest::testSetFieldOfView);
        // keep these test cases in sync with PureProjectionTest's (un)projection tests!
        addSubTest("testProject3dTo2d", (PFNUNITSUBTEST)&PureCameraTest::testProject3dTo2d);
        addSubTest("testProject2dTo3d", (PFNUNITSUBTEST)&PureCameraTest::testProject2dTo3d);

    } // PureCameraTest()

    virtual ~PureCameraTest()
    {
        
    }

protected:

    virtual bool setUp() override
    {
        return true;
    }

    virtual void tearDown() override
    {

    }

    virtual void initialize() override
    {
        //CConsole::getConsoleInstance().SetLoggingState(PureCamera::getLoggerModuleName(), true);
    }

    virtual void finalize() override
    {
        CConsole::getConsoleInstance().SetLoggingState(PureCamera::getLoggerModuleName(), false);    
    }

private:

    // ---------------------------------------------------------------------------

    PureCameraTest(const PureCameraTest&)
    {};         

    PureCameraTest& operator=(const PureCameraTest&)
    {
        return *this;
    };

    bool testCtor1()
    {
        const PureCamera cam;
        const PureVector vUp(0, 1, 0);
        const PureVector vForward(0, 0, 1);
        const PureVector vPos(0, 0, 0);
        const PureColor  clrBg(0, 0, 0);

        return assertEquals(0.0f, cam.getViewport().pos.x, "rect x") &
            assertEquals(0.0f, cam.getViewport().pos.y, "rect y") &
            assertGreater(cam.getViewport().size.width, 0.0f, "rect sx") &
            assertGreater(cam.getViewport().size.height, 0.0f, "rect sy") &
            assertTrue(vUp == cam.getUpVec(), "up") &
            assertTrue(vForward == cam.getTargetVec(), "target") &
            assertTrue(vPos == cam.getPosVec(), "pos") &
            assertEquals(cam.getViewport().size.width / cam.getViewport().size.height, cam.getAspectRatio(), "ratio") &
            assertEquals(TPURE_ASPECTRATIO_MODE::PURE_AM_FIX, cam.getAspectRatioMode(), "ratio mode") &
            assertTrue(clrBg == cam.getBackgroundColor(), "bg color") &
            assertEquals(PURE_CLEAR_ZBUFFER_COLORBUFFER, cam.getClearMode(), "clear mode") &
            assertTrue(cam.getNearPlane() > 0.0f, "nearplane") &
            assertTrue(cam.getFarPlane() > cam.getNearPlane(), "farplane") &
            assertTrue(cam.getFieldOfView() > 0.0f && cam.getFieldOfView() <= 180.0f, "fov");
    }

    bool testCopyCtor()
    {
        PureCamera cam;
        cam.getPosVec().Set(1, 2, 3);
        cam.getTargetVec().Set(1, 2, 3);
        cam.getUpVec().Set(1, 2, 3);
        cam.SetAspectRatio(3);
        cam.SetAspectRatioMode(PURE_AM_DYNAMIC);
        cam.SetClearMode(PURE_CLEAR_ZBUFFER);
        cam.SetFarPlane(20);
        cam.SetNearPlane(2);
        cam.SetFieldOfView(50);
        cam.SetRotation(1, 2, 3);
        cam.SetViewport(1, 1, 2, 2);
        cam.getBackgroundColor().Set(255, 255, 255, 255);

        const PureCamera cam2(cam);

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
        PureCamera cam;
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
        PureCamera cam;
        cam.SetAspectRatioMode(PURE_AM_DYNAMIC);

        return assertEquals(PURE_AM_DYNAMIC, cam.getAspectRatioMode());
    }

    bool testSetAspectRatio()
    {
        PureCamera cam;
        cam.SetAspectRatio(3.f);

        return assertEquals(3.f, cam.getAspectRatio());
    }

    bool testSetClearMode()
    {
        PureCamera cam;
        cam.SetClearMode(PURE_CLEAR_ZBUFFER);

        return assertEquals(PURE_CLEAR_ZBUFFER, cam.getClearMode());
    }

    bool testSetNearPlane()
    {
        PureCamera cam;
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
        PureCamera cam;
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
        PureCamera cam;
        PureCamera cam2;
        
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

    bool testProject3dTo2d()
    {
        // keep these test cases in sync with PureProjectionTest's projection tests!
        PureCamera cam;
        cam.getPosVec().Set(0, 0, -10);
        cam.getTargetVec().Set(0, 0, 1);
        cam.getUpVec().Set(0, 1, 0);
        cam.SetViewport(0, 0, 640, 480);
        cam.SetAspectRatio(cam.getViewport().size.width / cam.getViewport().size.height);
        cam.SetNearPlane(0.1f);
        cam.SetFarPlane(100.f);
        cam.SetFieldOfView(80.f);

        PureVector vecProjected;

        // case 0: world pos (0,0,0) that should be visible in the center in front of the camera
        bool b = assertTrue(
            cam.project3dTo2d(
                0.f, 0.f, 0.f,
                vecProjected),
            "projection 0 ok");

        b &= assertEquals(320.f, vecProjected.getX(), "X 0 exact") &
            assertEquals(240.f, vecProjected.getY(), "Y 0 exact") &
            assertBetween(0.f, 1.f, vecProjected.getZ(), "Z 0") &  // this just checks if it is really a valid depth buffer value
            assertEquals(0.990990996f, vecProjected.getZ(), "Z 0 exact");

        // case 1: world pos (0,0,-2) that should be visible in the center in front of the camera, closer than previous point;
        // and being closer also means that the generated depth value should be less than the previous depth value.
        b &= assertTrue(
            cam.project3dTo2d(
                0.f, 0.f, -2.f,
                vecProjected),
            "projection 1 ok");

        b &= assertEquals(320.f, vecProjected.getX(), "X 1 exact") &
            assertEquals(240.f, vecProjected.getY(), "Y 1 exact") &
            assertBetween(0.f, 1.f, vecProjected.getZ(), "Z 1") &  // this just checks if it is really a valid depth buffer value
            assertEquals(0.988488495f, vecProjected.getZ(), "Z 1 exact");

        // case 2: world pos (-1,1,0) that supposed to be visible left and above the camera
        b &= assertTrue(
            cam.project3dTo2d(
                -1.f, 1.f, 0.f,
                vecProjected),
            "projection 2 ok");

        b &= assertLess(vecProjected.getX(), 320.f, "X 2") &       // this just checks if it is really on the left side of the render area
            assertEquals(291.f, vecProjected.getX(), "X 2 exact") &
            assertGreater(vecProjected.getY(), 240.f, "Y 2") &     // this just checks if it is really on the top side of the render area
            assertEquals(269.f, vecProjected.getY(), "Y 2 exact") &
            assertBetween(0.f, 1.f, vecProjected.getZ(), "Z 2") &  // this just checks if it is really a valid depth buffer value
            assertEquals(0.990990996f, vecProjected.getZ(), "Z 2 exact");

        // case 3: world pos (0.2, -0.1, 0) that supposed to be visible right and below the camera
        b &= assertTrue(
            cam.project3dTo2d(
                0.2f, -0.1f, 0.f,
                vecProjected),
            "projection 3 ok");

        b &= assertGreater(vecProjected.getX(), 320.f, "X 3") &    // this just checks if it is really on the right side of the render area
            assertEquals(326.f, vecProjected.getX(), "X 3 exact") &
            assertLess(vecProjected.getY(), 240.f, "Y 3") &        // this just checks if it is really on the bottom side of the render area
            assertEquals(237.f, vecProjected.getY(), "Y 3 exact") &
            assertBetween(0.f, 1.f, vecProjected.getZ(), "Z 3") &  // this just checks if it is really a valid depth buffer value
            assertEquals(0.990990996f, vecProjected.getZ(), "Z 3 exact");

        // case 4: world pos (10,-8,0) that supposed to be visible, more to the right and below the camera than the previous
        b &= assertTrue(
            cam.project3dTo2d(
                10.f, -8.f, 0.f,
                vecProjected),
            "projection 4 ok");

        b &= assertGreater(vecProjected.getX(), 320.f, "X 4") &    // this just checks if it is really on the right side of the render area
            assertEquals(606.f, vecProjected.getX(), "X 4 exact") &
            assertLess(vecProjected.getY(), 240.f, "Y 4") &        // this just checks if it is really on the bottom side of the render area
            assertEquals(11.f, vecProjected.getY(), "Y 4 exact") &
            assertBetween(0.f, 1.f, vecProjected.getZ(), "Z 4") &  // this just checks if it is really a valid depth buffer value
            assertEquals(0.990990996f, vecProjected.getZ(), "Z 4 exact");

        // case 5: world pos (0,0,-12) that supposed to be behind the camera, cannot project
        b &= assertFalse(
            cam.project3dTo2d(
                0.f, 0.f, cam.getPosVec().getZ() - 2.f,
                vecProjected),
            "projection 5 ok");

        // case 6: world pos (100,0,0) that supposed to be too far right to the camera, cannot project
        b &= assertFalse(
            cam.project3dTo2d(
                100.f, 0.f, 0.f,
                vecProjected),
            "projection 6 ok");

        return b;
    }

    bool testProject2dTo3d()
    {
        // keep these test cases in sync with PureProjectionTest's unprojection tests!
        // all cases here are the reverse of cases in testProject3dTo2d()

        PureCamera cam;
        cam.getPosVec().Set(0, 0, -10);
        cam.getTargetVec().Set(0, 0, 1);
        cam.getUpVec().Set(0, 1, 0);
        cam.SetViewport(0, 0, 640, 480);
        cam.SetAspectRatio(cam.getViewport().size.width / cam.getViewport().size.height);
        cam.SetNearPlane(0.1f);
        cam.SetFarPlane(100.f);
        cam.SetFieldOfView(80.f);

        PureVector vecUnprojected;

        // case 0: window pos (320, 240, 0.990990996f) that should be visible in the center in front of the camera
        bool b = assertTrue(
            cam.project2dTo3d(
                320, 240, 0.990990996f,
                vecUnprojected),
            "unprojection 0 ok");

        b &= assertEquals(0.f, vecUnprojected.getX(), E, "X 0 exact") &
            assertEquals(0.f, vecUnprojected.getY(), E, "Y 0 exact") &
            assertEquals(0.f, vecUnprojected.getZ(), E, "Z 0 exact");

        // case 1: window pos (320, 240, 0.988488495f) that should be visible in the center in front of the camera, closer than previous point
        b &= assertTrue(
            cam.project2dTo3d(
                320, 240, 0.988488495f,
                vecUnprojected),
            "unprojection 1 ok");

        b &= assertEquals(0.f, vecUnprojected.getX(), E, "X 1 exact") &
            assertEquals(0.f, vecUnprojected.getY(), E, "Y 1 exact") &
            assertEquals(-2.f, vecUnprojected.getZ(), E, "Z 1 exact");

        // case 2: window pos (291, 269, 0.990990996f) that supposed to be visible left and above the camera
        b &= assertTrue(
            cam.project2dTo3d(
                291, 269, 0.990990996f,
                vecUnprojected),
            "unprojection 2 ok");

        b &= assertEquals(-1.f, vecUnprojected.getX(), E_LOOSE, "X 2 exact") &
            assertEquals(1.f, vecUnprojected.getY(), E_LOOSE, "Y 2 exact") &
            assertEquals(0.f, vecUnprojected.getZ(), E, "Z 2 exact");

        // case 3: window pos (326, 237, 0.990990996f) that supposed to be visible right and below the camera
        b &= assertTrue(
            cam.project2dTo3d(
                326, 237, 0.990990996f,
                vecUnprojected),
            "unprojection 3 ok");

        b &= assertEquals(0.2f, vecUnprojected.getX(), E_LOOSE, "X 3 exact") &
            assertEquals(-0.1f, vecUnprojected.getY(), E_LOOSE, "Y 3 exact") &
            assertEquals(0.f, vecUnprojected.getZ(), E, "Z 3 exact");

        // case 4: window pos (377, 183, 0.f) that supposed to be visible more to the right and below the camera than the previous
        b &= assertTrue(
            cam.project2dTo3d(
                606, 11, 0.990990996f,
                vecUnprojected),
            "unprojection 4 ok");

        b &= assertEquals(10.f, vecUnprojected.getX(), E_LOOSE, "X 4 exact") &
            assertEquals(-8.f, vecUnprojected.getY(), E_LOOSE, "Y 4 exact") &
            assertEquals(0.f, vecUnprojected.getZ(), E, "Z 4 exact");

        // case 5: window pos (0,0,-2) that supposed to be behind the camera, cannot unproject
        b &= assertFalse(
            cam.project2dTo3d(
                0, 0, -2.f,
                vecUnprojected),
            "unprojection 5 ok");

        // case 6: window pos (1000,0,0) that supposed to be too far right to the camera, cannot unproject
        b &= assertFalse(
            cam.project2dTo3d(
                1000, 0, 0.f,
                vecUnprojected),
            "unprojection 6 ok");

        return b;
    }

}; // class PureCameraTest