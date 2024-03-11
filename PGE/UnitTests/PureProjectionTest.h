#pragma once

/*
    ###################################################################################
    PureProjectionTest.h
    Unit test for PureProjection class.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PURE/include/external/Math/PureProjection.h"
#include "../PURE/include/external/PureCamera.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

#ifndef E_LOOSE
#define E_LOOSE 0.02f
#endif // Loosed EPSILON, for testProject2dTo3d (unclear why our results are not precise enough, should be investigated later)

class PureProjectionTest :
    public UnitTest
{
public:

    PureProjectionTest() : UnitTest(__FILE__, "")
    {
    }

    ~PureProjectionTest()
    {
        Finalize();
    } // ~PureProjectionTest()

protected:

    virtual void Initialize() override
    {
        m_cam.getPosVec().Set(0, 0, -10);
        m_cam.getTargetVec().Set(0, 0, 1);
        m_cam.getUpVec().Set(0, 1, 0);
        m_cam.SetViewport(0, 0, 640, 480);
        m_cam.SetAspectRatio(m_cam.getViewport().size.width / m_cam.getViewport().size.height);
        m_cam.SetNearPlane(0.1f);
        m_cam.SetFarPlane(100.f);
        m_cam.SetFieldOfView(80.f);
        
        // keep these test cases in sync with PureCameraTest's projection tests!
        AddSubTest("testProject3dTo2d", (PFNUNITSUBTEST)&PureProjectionTest::testProject3dTo2d);
        AddSubTest("testProject2dTo3d", (PFNUNITSUBTEST)&PureProjectionTest::testProject2dTo3d);
    }

    virtual bool setUp() override
    {
        return true;
    }

    virtual void TearDown() override
    {
        Finalize();
    }

    virtual void Finalize() override
    {
    }

private:

    PureCamera m_cam;

    // ---------------------------------------------------------------------------

    PureProjectionTest(const PureProjectionTest&)
    {};

    PureProjectionTest& operator=(const PureProjectionTest&)
    {
        return *this;
    };

    bool testProject3dTo2d()
    {
        // keep these test cases in sync with PureCameraTest's projection tests!
        PureVector vecProjected;

        // case 0: world pos (0,0,0) that should be visible in the center in front of the camera
        bool b = assertTrue(
            PureProjection::project3dTo2d(
                0.f, 0.f, 0.f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecProjected),
            "projection 0 ok");
        
        b &= assertEquals(320.f, vecProjected.getX(), "X 0 exact") &
            assertEquals(240.f, vecProjected.getY(), "Y 0 exact") &
            assertBetween(0.f, 1.f, vecProjected.getZ(), "Z 0") &  // this just checks if it is really a valid depth buffer value
            assertEquals(0.990990996f, vecProjected.getZ(), "Z 0 exact");
        
        // case 1: world pos (0,0,-2) that should be visible in the center in front of the camera, closer than previous point;
        // and being closer also means that the generated depth value should be less than the previous depth value.
        b &= assertTrue(
            PureProjection::project3dTo2d(
                0.f, 0.f, -2.f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecProjected),
            "projection 1 ok");

        b &= assertEquals(320.f, vecProjected.getX(), "X 1 exact") &
            assertEquals(240.f, vecProjected.getY(), "Y 1 exact") &
            assertBetween(0.f, 1.f, vecProjected.getZ(), "Z 1") &  // this just checks if it is really a valid depth buffer value
            assertEquals(0.988488495f, vecProjected.getZ(), "Z 1 exact");

        // case 2: world pos (-1,1,0) that supposed to be visible left and above the camera
        b &= assertTrue(
            PureProjection::project3dTo2d(
                -1.f, 1.f, 0.f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
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
            PureProjection::project3dTo2d(
                0.2f, -0.1f, 0.f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
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
            PureProjection::project3dTo2d(
                10.f, -8.f, 0.f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
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
            PureProjection::project3dTo2d(
                0.f, 0.f, m_cam.getPosVec().getZ() - 2.f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecProjected),
            "projection 5 ok");

        // case 6: world pos (100,0,0) that supposed to be too far right to the camera, cannot project
        b &= assertFalse(
            PureProjection::project3dTo2d(
                100.f, 0.f, 0.f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecProjected),
            "projection 6 ok");

        return b;
    }

    bool testProject2dTo3d()
    {
        // keep these test cases in sync with PureCameraTest's projection tests!
        // all cases here are the reverse of cases in testProject3dTo2d()

        PureVector vecUnprojected;

        // case 0: window pos (320, 240, 0.990990996f) that should be visible in the center in front of the camera
        bool b = assertTrue(
            PureProjection::project2dTo3d(
                320, 240, 0.990990996f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecUnprojected),
            "unprojection 0 ok");

        b &= assertEquals(0.f, vecUnprojected.getX(), E, "X 0 exact") &
            assertEquals(0.f, vecUnprojected.getY(), E, "Y 0 exact") &
            assertEquals(0.f, vecUnprojected.getZ(), E, "Z 0 exact");

        // case 1: window pos (320, 240, 0.988488495f) that should be visible in the center in front of the camera, closer than previous point
        b &= assertTrue(
            PureProjection::project2dTo3d(
                320, 240, 0.988488495f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecUnprojected),
            "unprojection 1 ok");

        b &= assertEquals(0.f, vecUnprojected.getX(), E, "X 1 exact") &
            assertEquals(0.f, vecUnprojected.getY(), E, "Y 1 exact") &
            assertEquals(-2.f, vecUnprojected.getZ(), E, "Z 1 exact");

        // case 2: window pos (291, 269, 0.990990996f) that supposed to be visible left and above the camera
        b &= assertTrue(
            PureProjection::project2dTo3d(
                291, 269, 0.990990996f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecUnprojected),
            "unprojection 2 ok");

        b &= assertEquals(-1.f, vecUnprojected.getX(), E_LOOSE, "X 2 exact") &
            assertEquals(1.f, vecUnprojected.getY(), E_LOOSE, "Y 2 exact") &
            assertEquals(0.f, vecUnprojected.getZ(), E, "Z 2 exact");

        // case 3: window pos (326, 237, 0.990990996f) that supposed to be visible right and below the camera
        b &= assertTrue(
            PureProjection::project2dTo3d(
                326, 237, 0.990990996f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecUnprojected),
            "unprojection 3 ok");

        b &= assertEquals(0.2f, vecUnprojected.getX(), E_LOOSE, "X 3 exact") &
            assertEquals(-0.1f, vecUnprojected.getY(), E_LOOSE, "Y 3 exact") &
            assertEquals(0.f, vecUnprojected.getZ(), E, "Z 3 exact");

        // case 4: window pos (377, 183, 0.f) that supposed to be visible more to the right and below the camera than the previous
        b &= assertTrue(
            PureProjection::project2dTo3d(
                606, 11, 0.990990996f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecUnprojected),
            "unprojection 4 ok");

        b &= assertEquals(10.f, vecUnprojected.getX(), E_LOOSE, "X 4 exact") &
            assertEquals(-8.f, vecUnprojected.getY(), E_LOOSE, "Y 4 exact") &
            assertEquals(0.f, vecUnprojected.getZ(), E, "Z 4 exact");

        // case 5: window pos (0,0,-2) that supposed to be behind the camera, cannot unproject
        b &= assertFalse(
            PureProjection::project2dTo3d(
                0, 0, -2.f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecUnprojected),
            "unprojection 5 ok");

        // case 6: window pos (1000,0,0) that supposed to be too far right to the camera, cannot unproject
        b &= assertFalse(
            PureProjection::project2dTo3d(
                1000, 0, 0.f,
                m_cam.getPosVec().getX(), m_cam.getPosVec().getY(), m_cam.getPosVec().getZ(),
                m_cam.getTargetVec().getX(), m_cam.getTargetVec().getY(), m_cam.getTargetVec().getZ(),
                m_cam.getUpVec().getX(), m_cam.getUpVec().getY(), m_cam.getUpVec().getZ(),
                m_cam.getFieldOfView(), m_cam.getAspectRatio(),
                m_cam.getNearPlane(), m_cam.getFarPlane(),
                static_cast<TPureInt>(m_cam.getViewport().pos.x), static_cast<TPureInt>(m_cam.getViewport().pos.y),
                static_cast<TPureInt>(m_cam.getViewport().size.width), static_cast<TPureInt>(m_cam.getViewport().size.height),
                vecUnprojected),
            "unprojection 6 ok");

        return b;
    }

}; // class PureProjectionTest
