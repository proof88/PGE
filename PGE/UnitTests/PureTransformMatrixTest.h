#pragma once

/*
    ###################################################################################
    PureTransformMatrixTest.h
    Unit test for PureTransformMatrix.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Math/PureTransformMatrix.h"
#include "../Pure/include/external/Math/PureVector.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureTransformMatrixTest :
    public UnitTest
{
public:

    PureTransformMatrixTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PureTransformMatrixTest::testCtor);
        AddSubTest("testCopyCtor", (PFNUNITSUBTEST) &PureTransformMatrixTest::testCopyCtor);
        AddSubTest("testCtorNotableZero", (PFNUNITSUBTEST) &PureTransformMatrixTest::testCtorNotableZero);
        AddSubTest("testCtorNotableIdentity", (PFNUNITSUBTEST) &PureTransformMatrixTest::testCtorNotableIdentity);
        AddSubTest("testStaticNotableZero", (PFNUNITSUBTEST) &PureTransformMatrixTest::testStaticNotableZero);
        AddSubTest("testStaticNotableIdentity", (PFNUNITSUBTEST) &PureTransformMatrixTest::testStaticNotableIdentity);
        AddSubTest("testIsZero", (PFNUNITSUBTEST) &PureTransformMatrixTest::testIsZero);
        AddSubTest("testIsIdentity", (PFNUNITSUBTEST) &PureTransformMatrixTest::testIsIdentity);
        AddSubTest("testSetRotationX", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetRotationX);
        AddSubTest("testSetRotationY", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetRotationY);
        AddSubTest("testSetRotationZ", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetRotationZ);
        AddSubTest("testSetTranslation", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetTranslation);
        AddSubTest("testGetScaleX", (PFNUNITSUBTEST) &PureTransformMatrixTest::testGetScaleX);
        AddSubTest("testGetScaleY", (PFNUNITSUBTEST) &PureTransformMatrixTest::testGetScaleY);
        AddSubTest("testGetScaleZ", (PFNUNITSUBTEST) &PureTransformMatrixTest::testGetScaleZ);
        AddSubTest("testSetScale", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetScale);
        AddSubTest("testSetScaleX", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetScaleX);
        AddSubTest("testSetScaleY", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetScaleY);
        AddSubTest("testSetScaleZ", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetScaleZ);
        AddSubTest("testSetMirrorXY", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetMirrorXY);
        AddSubTest("testSetMirrorXZ", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetMirrorXZ);
        AddSubTest("testSetMirrorYZ", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetMirrorYZ);
        AddSubTest("testSetFrustum", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetFrustum);
        AddSubTest("testSetFrustumByFovX", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetFrustumByFovX);
        AddSubTest("testSetFrustumByFovY", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetFrustumByFovY);
        AddSubTest("testSetLookAt", (PFNUNITSUBTEST) &PureTransformMatrixTest::testSetLookAt);

    } // PureTransformMatrixTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PureTransformMatrixTest(const PureTransformMatrixTest&)
    {};         

    PureTransformMatrixTest& operator=(const PureTransformMatrixTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PureTransformMatrix mat;
        return assertTrue(mat.isIdentity());
    }

    bool testCopyCtor()
    {
        PureTransformMatrix matSrc;

        matSrc.Set(0, 0, 99);
        matSrc.Set(1, 2, 80);

        const PureTransformMatrix matDst(matSrc);

        return assertTrue(matSrc == matDst);
    }

    bool testCtorNotableZero()
    {
        const PureTransformMatrix mat(PURE_MATRIX_ZERO);
        return assertTrue(mat.isZero());
    }

    bool testCtorNotableIdentity()
    {
        const PureTransformMatrix mat(PURE_MATRIX_IDENTITY);
        return assertTrue(mat.isIdentity());
    }

    bool testStaticNotableZero()
    {
        const PureTransformMatrix mat = PureTransformMatrix::getZero();
        return assertTrue(mat.isZero());
    }

    bool testStaticNotableIdentity()
    {
        PureTransformMatrix mat;
        mat.Set(1,1, 5);
        mat = PureTransformMatrix::getIdentity();

        return assertTrue(mat.isIdentity());
    }

    bool testIsZero()
    {
        PureTransformMatrix mat(PURE_MATRIX_ZERO);
        return assertTrue(mat.isZero());
    }

    bool testIsIdentity()
    {
        const PureTransformMatrix mat(PURE_MATRIX_IDENTITY);
        return assertTrue(mat.isIdentity());
    }

    bool testSetRotationX()
    {
        PureTransformMatrix mat;
        mat.SetRotationX(90);
        PureVector vec(0, 0, 1);

        vec = mat * vec;

        return assertEquals( 0, vec.getX(), E, "getX()") &
            assertEquals(-1, vec.getY(), E, "getY()") &
            assertEquals( 0, vec.getZ(), E, "getZ()");
    }

    bool testSetRotationY()
    {
        PureTransformMatrix mat;
        mat.SetRotationY(90);
        PureVector vec(0, 0, 1);

        vec = mat * vec;

        return assertEquals(-1, vec.getX(), E, "getX()") &
            assertEquals( 0, vec.getY(), E, "getY()") &
            assertEquals( 0, vec.getZ(), E, "getZ()");
    }

    bool testSetRotationZ()
    {
        PureTransformMatrix mat;
        mat.SetRotationZ(90);
        PureVector vec(0, 1, 0);

        vec = mat * vec;

        return assertEquals(-1, vec.getX(), E, "getX()") &
            assertEquals( 0, vec.getY(), E, "getY()") &
            assertEquals( 0, vec.getZ(), E, "getZ()");
    }

    bool testSetTranslation()
    {
        PureTransformMatrix mat;
        mat.SetTranslation(1, 2, 3);
        PureVector vec(1, 1, 1);

        vec = mat * vec;

        return assertEquals(2, vec.getX(), E, "getX()") &
            assertEquals(3, vec.getY(), E, "getY()") &
            assertEquals(4, vec.getZ(), E, "getZ()");
    }

    bool testGetScaleX()
    {
        PureTransformMatrix mat;
        mat.SetScaleX(4);

        return assertEquals(4, mat.getScaleX(), E);
    }

    bool testGetScaleY()
    {
        PureTransformMatrix mat;
        mat.SetScaleY(4);

        return assertEquals(4, mat.getScaleY(), E);
    }

    bool testGetScaleZ()
    {
        PureTransformMatrix mat;
        mat.SetScaleZ(4);

        return assertEquals(4, mat.getScaleZ(), E);
    }

    bool testSetScale()
    {
        PureTransformMatrix mat;
        mat.SetScale(2, 3, 4);

        return assertEquals(2, mat.getScaleX(), E, "getScaleX()") &
            assertEquals(3, mat.getScaleY(), E, "getScaleY()") &
            assertEquals(4, mat.getScaleZ(), E, "getScaleZ()");
    }

    bool testSetScaleX()
    {
        PureTransformMatrix mat;
        mat.SetScaleX(4);

        return assertEquals(4, mat.get(0,0), E);
    }

    bool testSetScaleY()
    {
        PureTransformMatrix mat;
        mat.SetScaleY(4);

        return assertEquals(4, mat.get(1,1), E);
    }

    bool testSetScaleZ()
    {
        PureTransformMatrix mat;
        mat.SetScaleZ(4);

        return assertEquals(4, mat.get(2,2), E);
    }

    bool testSetMirrorXY()
    {
        PureTransformMatrix mat;
        mat.SetMirrorXY();
        PureVector vec(1, 1, 1);

        vec = mat * vec;

        return assertEquals( 1, vec.getX(), E, "getX()") &
            assertEquals( 1, vec.getY(), E, "getY()") &
            assertEquals(-1, vec.getZ(), E, "getZ()");
    }

    bool testSetMirrorXZ()
    {
        PureTransformMatrix mat;
        mat.SetMirrorXZ();
        PureVector vec(1, 1, 1);

        vec = mat * vec;

        return assertEquals( 1, vec.getX(), E, "getX()") &
            assertEquals(-1, vec.getY(), E, "getY()") &
            assertEquals( 1, vec.getZ(), E, "getZ()");
    }

    bool testSetMirrorYZ()
    {
        PureTransformMatrix mat;
        mat.SetMirrorYZ();
        PureVector vec(1, 1, 1);

        vec = mat * vec;

        return assertEquals(-1, vec.getX(), E, "getX()") &
            assertEquals( 1, vec.getY(), E, "getY()") &
            assertEquals( 1, vec.getZ(), E, "getZ()");
    }

    bool testSetFrustum()
    {
        PureTransformMatrix mat;
        mat.SetFrustum(-1, 1, -1, 1, 1, 10);

        return assertGreater(mat.get(0,0), 0, "gen mat(0,0)") &
            assertGreater(mat.get(1,1), 0, "gen mat(1,1)") &
            assertGreater(mat.get(2,2), 0, "gen mat(2,2)") &
            assertEquals(1, mat.get(0,0), E, "spec mat(0,0)") &
            assertEquals(1, mat.get(1,1), E, "spec mat(1,1)") &
            assertEquals(1.2222f, mat.get(2,2), E, "spec mat(2,2)") &
            assertEquals(0, mat.get(3,3), E, "gen mat(3,3)") &
            assertLess(mat.get(2,3), 0, "gen mat(2,3)") &     /* mat[2][3] < 0 ensures we are compatible with left-handed coordinate system */
            assertEquals(-2.2222f, mat.get(2,3), E, "spec mat(2,3)") &
            assertEquals(1, mat.get(3,2), E, "mat(3,2)") &
            assertEquals(0, mat.get(0,1), E, "mat(0,1)") &
            assertEquals(0, mat.get(0,2), E, "mat(0,2)") &
            assertEquals(0, mat.get(0,3), E, "mat(0,3)") &
            assertEquals(0, mat.get(1,0), E, "mat(1,0)") &
            assertEquals(0, mat.get(1,2), E, "mat(1,2)") &
            assertEquals(0, mat.get(1,3), E, "mat(1,3)") &
            assertEquals(0, mat.get(2,0), E, "mat(2,0)") &
            assertEquals(0, mat.get(2,1), E, "mat(2,1)") &
            assertEquals(0, mat.get(3,0), E, "mat(3,0)") &
            assertEquals(0, mat.get(3,1), E, "mat(3,1)");
    }

    bool testSetFrustumByFovX()
    {
        PureTransformMatrix mat;
        mat.SetFrustumByFovX(80.0f, 1024/768.0f, 1.0f, 10.0f);

        return assertGreater(mat.get(0,0), 0, "gen mat(0,0)") &
            assertGreater(mat.get(1,1), 0, "gen mat(1,1)") &
            assertGreater(mat.get(2,2), 0, "gen mat(2,2)") &
            assertEquals(1.1917f, mat.get(0,0), E, "spec mat(0,0)") &
            assertEquals(1.5890f, mat.get(1,1), E, "spec mat(1,1)") &
            assertEquals(1.2222f, mat.get(2,2), E, "spec mat(2,2)") &
            assertEquals(0, mat.get(3,3), E, "gen mat(3,3)") &
            assertLess(mat.get(2,3), 0, "gen mat(2,3)") &     /* mat[2][3] < 0 ensures we are compatible with left-handed coordinate system */
            assertEquals(-2.2222f, mat.get(2,3), E, "spec mat(2,3)") &
            assertEquals(1, mat.get(3,2), E, "mat(3,2)") &
            assertEquals(0, mat.get(0,1), E, "mat(0,1)") &
            assertEquals(0, mat.get(0,2), E, "mat(0,2)") &
            assertEquals(0, mat.get(0,3), E, "mat(0,3)") &
            assertEquals(0, mat.get(1,0), E, "mat(1,0)") &
            assertEquals(0, mat.get(1,2), E, "mat(1,2)") &
            assertEquals(0, mat.get(1,3), E, "mat(1,3)") &
            assertEquals(0, mat.get(2,0), E, "mat(2,0)") &
            assertEquals(0, mat.get(2,1), E, "mat(2,1)") &
            assertEquals(0, mat.get(3,0), E, "mat(3,0)") &
            assertEquals(0, mat.get(3,1), E, "mat(3,1)");
    }

    bool testSetFrustumByFovY()
    {
        PureTransformMatrix mat;
        mat.SetIdentity();
        mat.SetFrustumByFovY(80.0f, 1024/768.0f, 1.0f, 10.0f);

        return assertGreater(mat.get(0,0), 0, "gen mat(0,0)") &
            assertGreater(mat.get(1,1), 0, "gen mat(1,1)") &
            assertGreater(mat.get(2,2), 0, "gen mat(2,2)") &
            assertEquals(0.8938f, mat.get(0,0), E, "spec mat(0,0)") &
            assertEquals(1.1917f, mat.get(1,1), E, "spec mat(1,1)") &
            assertEquals(1.2222f, mat.get(2,2), E, "spec mat(2,2)") &
            assertEquals(0, mat.get(3,3), E, "gen mat(3,3)") &
            assertLess(mat.get(2,3), 0, "gen mat(2,3)") &     /* mat[2][3] < 0 ensures we are compatible with left-handed coordinate system */
            assertEquals(-2.2222f, mat.get(2,3), E, "spec mat(2,3)") &
            assertEquals(1, mat.get(3,2), E, "mat(3,2)") &
            assertEquals(0, mat.get(0,1), E, "mat(0,1)") &
            assertEquals(0, mat.get(0,2), E, "mat(0,2)") &
            assertEquals(0, mat.get(0,3), E, "mat(0,3)") &
            assertEquals(0, mat.get(1,0), E, "mat(1,0)") &
            assertEquals(0, mat.get(1,2), E, "mat(1,2)") &
            assertEquals(0, mat.get(1,3), E, "mat(1,3)") &
            assertEquals(0, mat.get(2,0), E, "mat(2,0)") &
            assertEquals(0, mat.get(2,1), E, "mat(2,1)") &
            assertEquals(0, mat.get(3,0), E, "mat(3,0)") &
            assertEquals(0, mat.get(3,1), E, "mat(3,1)");
    }

    bool testSetLookAt()
    {
        PureVector vPos(0,0,0);
        PureVector vTarget(1,0,1);

        PureTransformMatrix mat;
        mat.SetLookAt(vPos, vTarget, PureVector(0,1,0));

        /* our matrix should contain following 3 vectors in first 3 columns, perpendicular to each other */
        const PureVector vRight( mat.get(0,0), mat.get(1,0), mat.get(2,0) );
        const PureVector vUp( mat.get(0,1), mat.get(1,1), mat.get(2,1) );
        const PureVector vForward( mat.get(0,2), mat.get(1,2), mat.get(2,2) );

        /* with given vPos and vTarget, the matrix should also define rotation on Y axis by 45° */
        /* I agree I shouldn't check again this if I already check above vectors in the matrix, but I don't care ... */
        PureTransformMatrix mRotation;
        mRotation.SetRotationY(45);

        /* and of course the matrix should contain our position as well, in last column, negated, since this would be a camera translation */
        const PureVector vNewPos( mat.get(0,3), mat.get(1,3), mat.get(2,3) );

        return assertEquals(0.0f, vRight * vUp, E, "vRight * vUp") &
            assertEquals(0.0f, vRight * vForward, E, "vRight * vForward") &
            assertEquals(0.0f, vForward * vUp, E, "vForward * vUp") &
            assertTrue( vPos == -vNewPos, "vPos == vNewPos" ) &
            assertEquals( 0.0f, mat.get(3,0), E, "mat(3,0)") &
            assertEquals( 0.0f, mat.get(3,1), E, "mat(3,1)") &
            assertEquals( 0.0f, mat.get(3,2), E, "mat(3,2)") &
            assertEquals( 1.0f, mat.get(3,3), E, "mat(3,3)") &
            assertEquals(mRotation.get(0,0), mat.get(0,0), E, "mat(0,0)") &
            assertEquals(mRotation.get(1,1), mat.get(1,1), E, "mat(1,1)") &
            assertEquals(mRotation.get(2,2), mat.get(2,2), E, "mat(2,2)") &
            assertEquals(mRotation.get(3,3), mat.get(3,3), E, "mat(3,3)") &
            assertEquals(mRotation.get(0,1), mat.get(0,1), E, "mat(0,1)") &
            assertEquals(mRotation.get(0,2), mat.get(0,2), E, "mat(0,2)") &
            assertEquals(mRotation.get(0,3), mat.get(0,3), E, "mat(0,3)") &
            assertEquals(mRotation.get(1,0), mat.get(1,0), E, "mat(1,0)") &
            assertEquals(mRotation.get(1,2), mat.get(1,2), E, "mat(1,2)") &
            assertEquals(mRotation.get(1,3), mat.get(1,3), E, "mat(1,3)") &
            assertEquals(mRotation.get(2,0), mat.get(2,0), E, "mat(2,0)") &
            assertEquals(mRotation.get(2,1), mat.get(2,1), E, "mat(2,1)") &
            assertEquals(mRotation.get(2,3), mat.get(2,3), E, "mat(2,3)") &
            assertEquals(mRotation.get(3,0), mat.get(3,0), E, "mat(3,0)") &
            assertEquals(mRotation.get(3,1), mat.get(3,1), E, "mat(3,1)") &
            assertEquals(mRotation.get(3,2), mat.get(3,2), E, "mat(3,2)");
    }

}; // class PureTransformMatrixTest