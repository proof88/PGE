#pragma once

/*
    ###################################################################################
    PRRETransformMatrixTest.h
    Unit test for PRRETransformMatrix.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../../../PGE/PGE/PRRE/PRRETransformMatrix.h"
#include "../../../PGE/PGE/PRRE/PRREVector.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRRETransformMatrixTest :
    public UnitTest
{
public:

    PRRETransformMatrixTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testCtor);
        AddSubTest("testCopyCtor", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testCopyCtor);
        AddSubTest("testCtorNotableZero", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testCtorNotableZero);
        AddSubTest("testCtorNotableIdentity", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testCtorNotableIdentity);
        AddSubTest("testStaticNotableZero", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testStaticNotableZero);
        AddSubTest("testStaticNotableIdentity", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testStaticNotableIdentity);
        AddSubTest("testIsZero", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testIsZero);
        AddSubTest("testIsIdentity", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testIsIdentity);
        AddSubTest("testSetRotationX", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetRotationX);
        AddSubTest("testSetRotationY", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetRotationY);
        AddSubTest("testSetRotationZ", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetRotationZ);
        AddSubTest("testSetTranslation", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetTranslation);
        AddSubTest("testGetScaleX", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testGetScaleX);
        AddSubTest("testGetScaleY", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testGetScaleY);
        AddSubTest("testGetScaleZ", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testGetScaleZ);
        AddSubTest("testSetScale", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetScale);
        AddSubTest("testSetScaleX", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetScaleX);
        AddSubTest("testSetScaleY", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetScaleY);
        AddSubTest("testSetScaleZ", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetScaleZ);
        AddSubTest("testSetMirrorXY", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetMirrorXY);
        AddSubTest("testSetMirrorXZ", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetMirrorXZ);
        AddSubTest("testSetMirrorYZ", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetMirrorYZ);
        AddSubTest("testSetFrustum", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetFrustum);
        AddSubTest("testSetFrustumByFovX", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetFrustumByFovX);
        AddSubTest("testSetFrustumByFovY", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetFrustumByFovY);
        AddSubTest("testSetLookAt", (PFNUNITSUBTEST) &PRRETransformMatrixTest::testSetLookAt);

    } // PRRETransformMatrixTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PRRETransformMatrixTest(const PRRETransformMatrixTest&)
    {};         

    PRRETransformMatrixTest& operator=(const PRRETransformMatrixTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PRRETransformMatrix mat;
        return assertTrue(mat.isIdentity());
    }

    bool testCopyCtor()
    {
        PRRETransformMatrix matSrc;

        matSrc.Set(0, 0, 99);
        matSrc.Set(1, 2, 80);

        const PRRETransformMatrix matDst(matSrc);

        return assertTrue(matSrc == matDst);
    }

    bool testCtorNotableZero()
    {
        const PRRETransformMatrix mat(PRRE_MATRIX_ZERO);
        return assertTrue(mat.isZero());
    }

    bool testCtorNotableIdentity()
    {
        const PRRETransformMatrix mat(PRRE_MATRIX_IDENTITY);
        return assertTrue(mat.isIdentity());
    }

    bool testStaticNotableZero()
    {
        const PRRETransformMatrix mat = PRRETransformMatrix::getZero();
        return assertTrue(mat.isZero());
    }

    bool testStaticNotableIdentity()
    {
        PRRETransformMatrix mat;
        mat.Set(1,1, 5);
        mat = PRRETransformMatrix::getIdentity();

        return assertTrue(mat.isIdentity());
    }

    bool testIsZero()
    {
        PRRETransformMatrix mat(PRRE_MATRIX_ZERO);
        return assertTrue(mat.isZero());
    }

    bool testIsIdentity()
    {
        const PRRETransformMatrix mat(PRRE_MATRIX_IDENTITY);
        return assertTrue(mat.isIdentity());
    }

    bool testSetRotationX()
    {
        PRRETransformMatrix mat;
        mat.SetRotationX(90);
        PRREVector vec(0, 0, 1);

        vec = mat * vec;

        return assertEquals( 0, vec.getX(), E, "getX()") &
            assertEquals(-1, vec.getY(), E, "getY()") &
            assertEquals( 0, vec.getZ(), E, "getZ()");
    }

    bool testSetRotationY()
    {
        PRRETransformMatrix mat;
        mat.SetRotationY(90);
        PRREVector vec(0, 0, 1);

        vec = mat * vec;

        return assertEquals(-1, vec.getX(), E, "getX()") &
            assertEquals( 0, vec.getY(), E, "getY()") &
            assertEquals( 0, vec.getZ(), E, "getZ()");
    }

    bool testSetRotationZ()
    {
        PRRETransformMatrix mat;
        mat.SetRotationZ(90);
        PRREVector vec(0, 1, 0);

        vec = mat * vec;

        return assertEquals(-1, vec.getX(), E, "getX()") &
            assertEquals( 0, vec.getY(), E, "getY()") &
            assertEquals( 0, vec.getZ(), E, "getZ()");
    }

    bool testSetTranslation()
    {
        PRRETransformMatrix mat;
        mat.SetTranslation(1, 2, 3);
        PRREVector vec(1, 1, 1);

        vec = mat * vec;

        return assertEquals(2, vec.getX(), E, "getX()") &
            assertEquals(3, vec.getY(), E, "getY()") &
            assertEquals(4, vec.getZ(), E, "getZ()");
    }

    bool testGetScaleX()
    {
        PRRETransformMatrix mat;
        mat.SetScaleX(4);

        return assertEquals(4, mat.getScaleX(), E);
    }

    bool testGetScaleY()
    {
        PRRETransformMatrix mat;
        mat.SetScaleY(4);

        return assertEquals(4, mat.getScaleY(), E);
    }

    bool testGetScaleZ()
    {
        PRRETransformMatrix mat;
        mat.SetScaleZ(4);

        return assertEquals(4, mat.getScaleZ(), E);
    }

    bool testSetScale()
    {
        PRRETransformMatrix mat;
        mat.SetScale(2, 3, 4);

        return assertEquals(2, mat.getScaleX(), E, "getScaleX()") &
            assertEquals(3, mat.getScaleY(), E, "getScaleY()") &
            assertEquals(4, mat.getScaleZ(), E, "getScaleZ()");
    }

    bool testSetScaleX()
    {
        PRRETransformMatrix mat;
        mat.SetScaleX(4);

        return assertEquals(4, mat.get(0,0), E);
    }

    bool testSetScaleY()
    {
        PRRETransformMatrix mat;
        mat.SetScaleY(4);

        return assertEquals(4, mat.get(1,1), E);
    }

    bool testSetScaleZ()
    {
        PRRETransformMatrix mat;
        mat.SetScaleZ(4);

        return assertEquals(4, mat.get(2,2), E);
    }

    bool testSetMirrorXY()
    {
        PRRETransformMatrix mat;
        mat.SetMirrorXY();
        PRREVector vec(1, 1, 1);

        vec = mat * vec;

        return assertEquals( 1, vec.getX(), E, "getX()") &
            assertEquals( 1, vec.getY(), E, "getY()") &
            assertEquals(-1, vec.getZ(), E, "getZ()");
    }

    bool testSetMirrorXZ()
    {
        PRRETransformMatrix mat;
        mat.SetMirrorXZ();
        PRREVector vec(1, 1, 1);

        vec = mat * vec;

        return assertEquals( 1, vec.getX(), E, "getX()") &
            assertEquals(-1, vec.getY(), E, "getY()") &
            assertEquals( 1, vec.getZ(), E, "getZ()");
    }

    bool testSetMirrorYZ()
    {
        PRRETransformMatrix mat;
        mat.SetMirrorYZ();
        PRREVector vec(1, 1, 1);

        vec = mat * vec;

        return assertEquals(-1, vec.getX(), E, "getX()") &
            assertEquals( 1, vec.getY(), E, "getY()") &
            assertEquals( 1, vec.getZ(), E, "getZ()");
    }

    bool testSetFrustum()
    {
        PRRETransformMatrix mat;
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
        PRRETransformMatrix mat;
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
        PRRETransformMatrix mat;
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
        PRREVector vPos(0,0,0);
        PRREVector vTarget(1,0,1);

        PRRETransformMatrix mat;
        mat.SetLookAt(vPos, vTarget, PRREVector(0,1,0));

        /* our matrix should contain following 3 vectors in first 3 columns, perpendicular to each other */
        const PRREVector vRight( mat.get(0,0), mat.get(1,0), mat.get(2,0) );
        const PRREVector vUp( mat.get(0,1), mat.get(1,1), mat.get(2,1) );
        const PRREVector vForward( mat.get(0,2), mat.get(1,2), mat.get(2,2) );

        /* with given vPos and vTarget, the matrix should also define rotation on Y axis by 45° */
        /* I agree I shouldn't check again this if I already check above vectors in the matrix, but I don't care ... */
        PRRETransformMatrix mRotation;
        mRotation.SetRotationY(45);

        /* and of course the matrix should contain our position as well, in last column, negated, since this would be a camera translation */
        const PRREVector vNewPos( mat.get(0,3), mat.get(1,3), mat.get(2,3) );

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

}; // class PRRETransformMatrixTest