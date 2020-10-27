#pragma once

/*
    ###################################################################################
    PRREMatrixTest.h
    Unit test for PRREMatrix.
    Made by PR00F88
    EMAIL : PR0o0o0o0o0o0o0o0o0o0oF88@gmail.com
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../PRRE/include/external/Math/PRREMatrix.h"
#include "../PRRE/include/external/Math/PRREVector.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PRREMatrixTest :
    public UnitTest
{
public:

    PRREMatrixTest() :
        UnitTest( __FILE__ )
    {
        AddSubTest("testCtor", (PFNUNITSUBTEST) &PRREMatrixTest::testCtor);
        AddSubTest("testCopyCtor", (PFNUNITSUBTEST) &PRREMatrixTest::testCopyCtor);
        AddSubTest("testCtorNotableZero", (PFNUNITSUBTEST) &PRREMatrixTest::testCtorNotableZero);
        AddSubTest("testCtorNotableIdentity", (PFNUNITSUBTEST) &PRREMatrixTest::testCtorNotableIdentity);
        AddSubTest("testStaticNotableZero", (PFNUNITSUBTEST) &PRREMatrixTest::testStaticNotableZero);
        AddSubTest("testStaticNotableIdentity", (PFNUNITSUBTEST) &PRREMatrixTest::testStaticNotableIdentity);
        AddSubTest("testGet", (PFNUNITSUBTEST) &PRREMatrixTest::testGet);
        AddSubTest("testSet", (PFNUNITSUBTEST) &PRREMatrixTest::testSet);
        AddSubTest("testGet2", (PFNUNITSUBTEST) &PRREMatrixTest::testGet2);
        AddSubTest("testIsZero", (PFNUNITSUBTEST) &PRREMatrixTest::testIsZero);
        AddSubTest("testSetZero", (PFNUNITSUBTEST) &PRREMatrixTest::testSetZero);
        AddSubTest("testIsIdentity", (PFNUNITSUBTEST) &PRREMatrixTest::testIsIdentity);
        AddSubTest("testSetIdentity", (PFNUNITSUBTEST) &PRREMatrixTest::testSetIdentity);
        AddSubTest("testTranspose", (PFNUNITSUBTEST) &PRREMatrixTest::testTranspose);
        AddSubTest("testGetTransposed", (PFNUNITSUBTEST) &PRREMatrixTest::testGetTransposed);
        AddSubTest("getDeterminantOfIdentity", (PFNUNITSUBTEST) &PRREMatrixTest::getDeterminantOfIdentity);
        AddSubTest("getDeterminantOfZero", (PFNUNITSUBTEST) &PRREMatrixTest::getDeterminantOfZero);
        AddSubTest("getDeterminantOfDegenerate", (PFNUNITSUBTEST) &PRREMatrixTest::getDeterminantOfDegenerate);
        AddSubTest("getDeterminantOfDegenTransposed", (PFNUNITSUBTEST) &PRREMatrixTest::getDeterminantOfDegenTransposed);
        AddSubTest("isIdentityInvertible", (PFNUNITSUBTEST) &PRREMatrixTest::isIdentityInvertible);
        AddSubTest("isZeroInvertible", (PFNUNITSUBTEST) &PRREMatrixTest::isZeroInvertible);
        AddSubTest("isDegenerateInvertible", (PFNUNITSUBTEST) &PRREMatrixTest::isDegenerateInvertible);
        AddSubTest("isDegenTransposeInvertible", (PFNUNITSUBTEST) &PRREMatrixTest::isDegenTransposeInvertible);
        AddSubTest("InvertIdentity", (PFNUNITSUBTEST) &PRREMatrixTest::InvertIdentity);
        AddSubTest("InvertZero", (PFNUNITSUBTEST) &PRREMatrixTest::InvertZero);
        AddSubTest("InvertDegenerate", (PFNUNITSUBTEST) &PRREMatrixTest::InvertDegenerate);
        AddSubTest("InvertInvertible", (PFNUNITSUBTEST) &PRREMatrixTest::InvertInvertible);
        AddSubTest("getInvertIdentity", (PFNUNITSUBTEST) &PRREMatrixTest::getInvertIdentity);
        AddSubTest("getInvertZero", (PFNUNITSUBTEST) &PRREMatrixTest::getInvertZero);
        AddSubTest("getInvertDegenerate", (PFNUNITSUBTEST) &PRREMatrixTest::getInvertDegenerate);
        AddSubTest("getInvertInvertible", (PFNUNITSUBTEST) &PRREMatrixTest::getInvertInvertible);
        AddSubTest("testOperatorEquals", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorEquals);
        AddSubTest("testOperatorNotEquals", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorNotEquals);
        AddSubTest("testOperatorAssignment", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorAssignment);
        AddSubTest("testOperatorAddition", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorAddition);
        AddSubTest("testOperatorAdditionAssignment", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorAdditionAssignment);
        AddSubTest("testOperatorSubtraction", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorSubtraction);
        AddSubTest("testOperatorSubtractionAssignment", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorSubtractionAssignment);
        AddSubTest("testOperatorMultiplicationByMatrix", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorMultiplicationByMatrix);
        AddSubTest("testOperatorMultiplicationAssignmentByMatrix", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorMultiplicationAssignmentByMatrix);
        AddSubTest("testOperatorMultiplicationByScalar1", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorMultiplicationByScalar1);
        AddSubTest("testOperatorMultiplicationByScalar2", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorMultiplicationByScalar2);
        AddSubTest("testOperatorMultiplicationAssignmentByScalar", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorMultiplicationAssignmentByScalar);
        AddSubTest("testOperatorAccessElement", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorAccessElement);
        AddSubTest("testOperatorNegative", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorNegative);
        AddSubTest("testOperatorMultiplicationByVectorRight", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorMultiplicationByVectorRight);
        AddSubTest("testOperatorMultiplicationByVectorLeft", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorMultiplicationByVectorLeft);
        AddSubTest("testOperatorMultiplicationAssignmentByVectorLeft", (PFNUNITSUBTEST) &PRREMatrixTest::testOperatorMultiplicationAssignmentByVectorLeft);

    } // PRREMatrixTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PRREMatrixTest(const PRREMatrixTest&)
    {};         

    PRREMatrixTest& operator=(const PRREMatrixTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PRREMatrix mat;
        return assertTrue(mat.isIdentity());
    }

    bool testCopyCtor()
    {
        PRREMatrix matSrc;

        matSrc.Set(0, 0, 99);
        matSrc.Set(1, 2, 80);

        const PRREMatrix matDst(matSrc);

        return assertTrue(matSrc == matDst);
    }

    bool testCtorNotableZero()
    {
        const PRREMatrix mat(PRRE_MATRIX_ZERO);
        return assertTrue(mat.isZero());
    }

    bool testCtorNotableIdentity()
    {
        const PRREMatrix mat(PRRE_MATRIX_IDENTITY);
        return assertTrue(mat.isIdentity());
    }

    bool testStaticNotableZero()
    {
        const PRREMatrix mat = PRREMatrix::getZero();
        return assertTrue(mat.isZero());
    }

    bool testStaticNotableIdentity()
    {
        PRREMatrix mat;
        mat.Set(1,1, 5);
        mat = PRREMatrix::getIdentity();

        return assertTrue(mat.isIdentity());
    }

    bool testGet()
    {
        PRREMatrix mat;
        mat.Set(1,1, 5);
        mat.Set(2,3, 7);

        return assertEquals(1, mat.get(0,0), E, "(0,0)") &
            assertEquals(5, mat.get(1,1), E, "(1,1)") &
            assertEquals(1, mat.get(2,2), E, "(2,2)") &
            assertEquals(1, mat.get(3,3), E, "(3,3)") &
            assertEquals(0, mat.get(0,1), E, "(0,1)") & assertEquals(0, mat.get(0,2), E, "(0,2)") & assertEquals(0, mat.get(0,3), E, "(0,3)") &
            assertEquals(0, mat.get(1,0), E, "(1,0)") & assertEquals(0, mat.get(1,2), E, "(1,2)") & assertEquals(0, mat.get(1,3), E, "(1,3)") &
            assertEquals(0, mat.get(2,0), E, "(2,0)") & assertEquals(0, mat.get(2,1), E, "(2,1)") & assertEquals(7, mat.get(2,3), E, "(2,3)") &
            assertEquals(0, mat.get(3,0), E, "(3,0)") & assertEquals(0, mat.get(3,1), E, "(3,1)") & assertEquals(0, mat.get(3,2), E, "(3,2)");
    }

    bool testSet()
    {
        PRREMatrix mat;
        mat.Set(1,1, 5);
        mat.Set(2,3, 7);

        return assertEquals(1, mat.get(0,0), E, "(0,0)") &
            assertEquals(5, mat.get(1,1), E, "(1,1)") &
            assertEquals(1, mat.get(2,2), E, "(2,2)") &
            assertEquals(1, mat.get(3,3), E, "(3,3)") &
            assertEquals(0, mat.get(0,1), E, "(0,1)") & assertEquals(0, mat.get(0,2), E, "(0,2)") & assertEquals(0, mat.get(0,3), E, "(0,3)") &
            assertEquals(0, mat.get(1,0), E, "(1,0)") & assertEquals(0, mat.get(1,2), E, "(1,2)") & assertEquals(0, mat.get(1,3), E, "(1,3)") &
            assertEquals(0, mat.get(2,0), E, "(2,0)") & assertEquals(0, mat.get(2,1), E, "(2,1)") & assertEquals(7, mat.get(2,3), E, "(2,3)") &
            assertEquals(0, mat.get(3,0), E, "(3,0)") & assertEquals(0, mat.get(3,1), E, "(3,1)") & assertEquals(0, mat.get(3,2), E, "(3,2)");
    }

    bool testGet2()
    {
        PRREMatrix mat;    
        mat.Set(0,0, 6);

        return assertNotNull(mat.get(), "not NULL") &
            assertEquals(6, mat.get()[0], E, "(0,0)");
    }

    bool testIsZero()
    {
        PRREMatrix mat(PRRE_MATRIX_ZERO);
        return assertTrue(mat.isZero());
    }

    bool testSetZero()
    {
        PRREMatrix mat;
        mat.Set(1,1, 5);
        mat.Set(2,3, 7);
        mat.SetZero();

        return assertTrue(mat.isZero());
    }

    bool testIsIdentity()
    {
        const PRREMatrix mat(PRRE_MATRIX_IDENTITY);
        return assertTrue(mat.isIdentity());
    }

    bool testTranspose()
    {
        PRREMatrix mat;    
        mat.SetIdentity();
        mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,2,  6);  mat.Set(1,3,  7);
        mat.Set(2,0,  8);  mat.Set(2,1,  9);  mat.Set(2,3, 10);
        mat.Set(3,0, 11);  mat.Set(3,1, 12);  mat.Set(3,2, 13);

        return assertEquals(1, mat.get(0,0), E, "(0,0)") &
            assertEquals( 1, mat.get(1,1), E, "(1,1)") &
            assertEquals( 1, mat.get(2,2), E, "(2,2)") &
            assertEquals( 1, mat.get(3,3), E, "(3,3)") &
            assertEquals( 2, mat.get(0,1), E, "(0,1)") &
            assertEquals( 3, mat.get(0,2), E, "(0,2)") &
            assertEquals( 4, mat.get(0,3), E, "(0,3)") &
            assertEquals( 5, mat.get(1,0), E, "(1,0)") &
            assertEquals( 6, mat.get(1,2), E, "(1,2)") &
            assertEquals( 7, mat.get(1,3), E, "(1,3)") &
            assertEquals( 8, mat.get(2,0), E, "(2,0)") &
            assertEquals( 9, mat.get(2,1), E, "(2,1)") &
            assertEquals(10, mat.get(2,3), E, "(2,3)") &
            assertEquals(11, mat.get(3,0), E, "(3,0)") &
            assertEquals(12, mat.get(3,1), E, "(3,1)") &
            assertEquals(13, mat.get(3,2), E, "(3,2)");
    }

    bool testGetTransposed()
    {
        PRREMatrix mat, mattransposed;    
        
        mat.SetIdentity();
        mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,2,  6);  mat.Set(1,3,  7);
        mat.Set(2,0,  8);  mat.Set(2,1,  9);  mat.Set(2,3, 10);
        mat.Set(3,0, 11);  mat.Set(3,1, 12);  mat.Set(3,2, 13);
        
        mattransposed = mat;
        mattransposed.Transpose();

        return assertNotNull(mat.getTransposed(), "not NULL") &
            assertTrue(0 == memcmp(mat.getTransposed(), mattransposed.get(), sizeof(TPRREfloat)*4*4), "memcmp");
    }

    bool getDeterminantOfIdentity()
    {
        PRREMatrix mat;
        mat.SetIdentity();

        return assertEquals(1.0f, mat.getDeterminant(), E);
    }

    bool getDeterminantOfZero()
    {
        PRREMatrix mat;
        mat.SetZero();

        return assertEquals(0.0f, mat.getDeterminant(), E);
    }

    bool getDeterminantOfDegenerate()
    {
        PRREMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);

        return assertEquals(0.0f, mat.getDeterminant(), E);
    }

    bool getDeterminantOfDegenTransposed()
    {
        PRREMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);

        const TPRREfloat det = mat.getDeterminant();

        mat.Transpose();

        return assertEquals(det, mat.getDeterminant(), E);
    }

    bool isIdentityInvertible()
    {
        PRREMatrix mat;
        mat.SetIdentity();

        return assertTrue( mat.isInvertible() );
    }

    bool isZeroInvertible()
    {
        PRREMatrix mat;
        mat.SetZero();

        return assertFalse( mat.isInvertible() );
    }

    bool isDegenerateInvertible()
    {
        PRREMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);

        return assertFalse( mat.isInvertible() );
    }

    bool isDegenTransposeInvertible()
    {
        PRREMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);

        mat.Transpose();

        return assertFalse( mat.isInvertible() );
    }

    bool InvertIdentity()
    {
        PRREMatrix mat;
        mat.SetIdentity();

        const TPRREfloat det = mat.getDeterminant();
        const bool b1 = mat.invert();

        return assertEquals(1/det, mat.getDeterminant(), E, "det" ) &
            assertTrue(mat.isIdentity(), "isIdentity") &
            assertTrue(b1, "invert");
    }

    bool InvertZero()
    {
        PRREMatrix mat;
        mat.SetZero();

        const TPRREfloat det = mat.getDeterminant();
        const bool b1 = mat.invert();

        return assertEquals(det, mat.getDeterminant(), E, "det" ) &
            assertTrue(mat.isZero(), "isZero") &
            assertFalse(b1, "invert");
    }

    bool InvertDegenerate()
    {
        PRREMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);
        const PRREMatrix mat2 = mat;

        const TPRREfloat det = mat.getDeterminant();
        const bool b1 = mat.invert();

        return assertEquals(det, mat.getDeterminant(), E, "det" ) &
            assertTrue(mat2 == mat, "mat2 == mat") &
            assertFalse(b1, "invert");
    }

    bool InvertInvertible()
    {
        PRREMatrix mat;
        mat.Set(0,0,  2);  mat.Set(0,1,  2);  mat.Set(0,2, 10);  mat.Set(0,3,  2);
        mat.Set(1,0,  4);  mat.Set(1,1,  4);  mat.Set(1,2,  2);  mat.Set(1,3,  2);
        mat.Set(2,0,  6);  mat.Set(2,1,  2);  mat.Set(2,2,  6);  mat.Set(2,3,  2);
        mat.Set(3,0,  6);  mat.Set(3,1,  2);  mat.Set(3,2,  2);  mat.Set(3,3,  1);
        const PRREMatrix mat2 = mat;

        const TPRREfloat det = mat.getDeterminant();
        const bool b1 = mat.invert();
        const PRREMatrix ident = mat*mat2;

        return assertEquals(1/det, mat.getDeterminant(), E, "det" ) &
            assertTrue(ident.isIdentity(), "identity") &
            assertTrue(b1, "invert") &
            assertEquals(-0.05f, mat.get(0,0), E, "(0,0)") &
            assertEquals( 0.2f,  mat.get(1,1), E, "(1,1)") &
            assertEquals(-0.2f,  mat.get(2,2), E, "(2,2)") &
            assertEquals(-1.8f,  mat.get(3,3), E, "(3,3)") &
            assertEquals(-0.1f,  mat.get(0,1), E, "(0,1)") &
            assertEquals( 0.05f, mat.get(0,2), E, "(0,2)") &
            assertEquals( 0.2f,  mat.get(0,3), E, "(0,3)") &
            assertEquals( 0.35f, mat.get(1,0), E, "(1,0)") &
            assertEquals(-0.85f, mat.get(1,2), E, "(1,2)") &
            assertEquals( 0.6f,  mat.get(1,3), E, "(1,3)") &
            assertEquals( 0.2f,  mat.get(2,0), E, "(2,0)") &
            assertEquals(-0.1f,  mat.get(2,1), E, "(2,1)") &
            assertEquals( 0.2f,  mat.get(2,3), E, "(2,3)") &
            assertEquals(-0.8f,  mat.get(3,0), E, "(3,0)") &
            assertEquals( 0.4f,  mat.get(3,1), E, "(3,1)") &
            assertEquals( 1.8f,  mat.get(3,2), E, "(3,2)");
    }

    bool getInvertIdentity()
    {
        PRREMatrix mat, matinverted;    
        
        mat.SetIdentity();
        matinverted = mat;
        const bool b1 = matinverted.invert();
        const TPRREfloat* p = mat.getInverse();

        return assertNotNull(p, "not NULL") &
            assertTrue(b1, "invert") &
            assertEquals( matinverted.get(0,0), p[0*4+0], E, "(0,0)") &
            assertEquals( matinverted.get(1,1), p[1*4+1], E, "(1,1)") &
            assertEquals( matinverted.get(2,2), p[2*4+2], E, "(2,2)") &
            assertEquals( matinverted.get(3,3), p[3*4+3], E, "(3,3)") &
            assertEquals( matinverted.get(0,1), p[0*4+1], E, "(0,1)") &
            assertEquals( matinverted.get(0,2), p[0*4+2], E, "(0,2)") &
            assertEquals( matinverted.get(0,3), p[0*4+3], E, "(0,3)") &
            assertEquals( matinverted.get(1,0), p[1*4+0], E, "(1,0)") &
            assertEquals( matinverted.get(1,2), p[1*4+2], E, "(1,2)") &
            assertEquals( matinverted.get(1,3), p[1*4+3], E, "(1,3)") &
            assertEquals( matinverted.get(2,0), p[2*4+0], E, "(2,0)") &
            assertEquals( matinverted.get(2,1), p[2*4+1], E, "(2,1)") &
            assertEquals( matinverted.get(2,3), p[2*4+3], E, "(2,3)") &
            assertEquals( matinverted.get(3,0), p[3*4+0], E, "(3,0)") &
            assertEquals( matinverted.get(3,1), p[3*4+1], E, "(3,1)") &
            assertEquals( matinverted.get(3,2), p[3*4+2], E, "(3,2)");
    }

    bool getInvertZero()
    {
        PRREMatrix mat, matinverted;    
        
        mat.SetZero();
        matinverted = mat;
        const bool b1 = matinverted.invert();
        const TPRREfloat* p = mat.getInverse();

        return assertNull(p, "NULL") &
            assertFalse(b1, "invert");
    }

    bool getInvertDegenerate()
    {
        PRREMatrix mat, matinverted;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);
        matinverted = mat;

        const bool b1 = matinverted.invert();
        const TPRREfloat* p = mat.getInverse();

        return assertNull(p, "NULL") &
            assertTrue(matinverted == mat, "matinverted == mat") &
            assertFalse(b1, "invert");
    }

    bool getInvertInvertible()
    {
        PRREMatrix mat, matinverted;
        mat.Set(0,0,  2);  mat.Set(0,1,  2);  mat.Set(0,2, 10);  mat.Set(0,3,  2);
        mat.Set(1,0,  4);  mat.Set(1,1,  4);  mat.Set(1,2,  2);  mat.Set(1,3,  2);
        mat.Set(2,0,  6);  mat.Set(2,1,  2);  mat.Set(2,2,  6);  mat.Set(2,3,  2);
        mat.Set(3,0,  6);  mat.Set(3,1,  2);  mat.Set(3,2,  2);  mat.Set(3,3,  1);
        matinverted = mat;

        const bool b1 = matinverted.invert();
        const TPRREfloat* p = mat.getInverse();

        return assertNotNull(p, "not NULL") &
            assertTrue(b1, "invert") &
            assertEquals(-0.05f, p[0*4+0], E, "(0,0)") &
            assertEquals( 0.2f,  p[1*4+1], E, "(1,1)") &
            assertEquals(-0.2f,  p[2*4+2], E, "(2,2)") &
            assertEquals(-1.8f,  p[3*4+3], E, "(3,3)") &
            assertEquals(-0.1f,  p[0*4+1], E, "(0,1)") &
            assertEquals( 0.05f, p[0*4+2], E, "(0,2)") &
            assertEquals( 0.2f,  p[0*4+3], E, "(0,3)") &
            assertEquals( 0.35f, p[1*4+0], E, "(1,0)") &
            assertEquals(-0.85f, p[1*4+2], E, "(1,2)") &
            assertEquals( 0.6f,  p[1*4+3], E, "(1,3)") &
            assertEquals( 0.2f,  p[2*4+0], E, "(2,0)") &
            assertEquals(-0.1f,  p[2*4+1], E, "(2,1)") &
            assertEquals( 0.2f,  p[2*4+3], E, "(2,3)") &
            assertEquals(-0.8f,  p[3*4+0], E, "(3,0)") &
            assertEquals( 0.4f,  p[3*4+1], E, "(3,1)") &
            assertEquals( 1.8f,  p[3*4+2], E, "(3,2)") &
            assertEquals( matinverted.get(0,0), p[0*4+0], E, "(0,0)") &
            assertEquals( matinverted.get(1,1), p[1*4+1], E, "(1,1)") &
            assertEquals( matinverted.get(2,2), p[2*4+2], E, "(2,2)") &
            assertEquals( matinverted.get(3,3), p[3*4+3], E, "(3,3)") &
            assertEquals( matinverted.get(0,1), p[0*4+1], E, "(0,1)") &
            assertEquals( matinverted.get(0,2), p[0*4+2], E, "(0,2)") &
            assertEquals( matinverted.get(0,3), p[0*4+3], E, "(0,3)") &
            assertEquals( matinverted.get(1,0), p[1*4+0], E, "(1,0)") &
            assertEquals( matinverted.get(1,2), p[1*4+2], E, "(1,2)") &
            assertEquals( matinverted.get(1,3), p[1*4+3], E, "(1,3)") &
            assertEquals( matinverted.get(2,0), p[2*4+0], E, "(2,0)") &
            assertEquals( matinverted.get(2,1), p[2*4+1], E, "(2,1)") &
            assertEquals( matinverted.get(2,3), p[2*4+3], E, "(2,3)") &
            assertEquals( matinverted.get(3,0), p[3*4+0], E, "(3,0)") &
            assertEquals( matinverted.get(3,1), p[3*4+1], E, "(3,1)") &
            assertEquals( matinverted.get(3,2), p[3*4+2], E, "(3,2)");
    }

    bool testSetIdentity()
    {
        PRREMatrix mat;
        mat.Set(1,1, 5);
        mat.Set(2,3, 7);
        mat.SetIdentity();

        return assertTrue(mat.isIdentity());
    }

    bool testOperatorEquals()
    {
        const PRREMatrix mat1;
        const PRREMatrix mat2;

        return assertTrue(mat1 == mat2);
    }

    bool testOperatorNotEquals()
    {
        const PRREMatrix mat1;
        const PRREMatrix mat2;

        return assertFalse(mat1 != mat2);
    }

    bool testOperatorAssignment()
    {
        PRREMatrix mat1;
        mat1.Set(1,1, 5);

        const PRREMatrix mat2 = mat1;

        return assertTrue(mat1 == mat2);
    }

    bool testOperatorAddition()
    {
        PRREMatrix matSrc;
        matSrc.Set(2,3, 2);

        const PRREMatrix mat = matSrc + matSrc;

        return assertEquals(2, mat.get(0,0), E, "(0,0)") &
            assertEquals(2, mat.get(1,1), E, "(1,1)") &
            assertEquals(2, mat.get(2,2), E, "(2,2)") &
            assertEquals(2, mat.get(3,3), E, "(3,3)") &
            assertEquals(0, mat.get(0,1), E, "(0,1)") & assertEquals(0, mat.get(0,2), E, "(0,2)") & assertEquals(0, mat.get(0,3), E, "(0,3)") &
            assertEquals(0, mat.get(1,0), E, "(1,0)") & assertEquals(0, mat.get(1,2), E, "(1,2)") & assertEquals(0, mat.get(1,3), E, "(1,3)") &
            assertEquals(0, mat.get(2,0), E, "(2,0)") & assertEquals(0, mat.get(2,1), E, "(2,1)") & assertEquals(4, mat.get(2,3), E, "(2,3)") &
            assertEquals(0, mat.get(3,0), E, "(3,0)") & assertEquals(0, mat.get(3,1), E, "(3,1)") & assertEquals(0, mat.get(3,2), E, "(3,2)");
    }

    bool testOperatorAdditionAssignment()
    {
        PRREMatrix mat;
        mat.Set(2,3, 2);

        mat += mat;

        return assertEquals(2, mat.get(0,0), E, "(0,0)") &
            assertEquals(2, mat.get(1,1), E, "(1,1)") &
            assertEquals(2, mat.get(2,2), E, "(2,2)") &
            assertEquals(2, mat.get(3,3), E, "(3,3)") &
            assertEquals(0, mat.get(0,1), E, "(0,1)") & assertEquals(0, mat.get(0,2), E, "(0,2)") & assertEquals(0, mat.get(0,3), E, "(0,3)") &
            assertEquals(0, mat.get(1,0), E, "(1,0)") & assertEquals(0, mat.get(1,2), E, "(1,2)") & assertEquals(0, mat.get(1,3), E, "(1,3)") &
            assertEquals(0, mat.get(2,0), E, "(2,0)") & assertEquals(0, mat.get(2,1), E, "(2,1)") & assertEquals(4, mat.get(2,3), E, "(2,3)") &
            assertEquals(0, mat.get(3,0), E, "(3,0)") & assertEquals(0, mat.get(3,1), E, "(3,1)") & assertEquals(0, mat.get(3,2), E, "(3,2)");
    }

    bool testOperatorSubtraction()
    {
        PRREMatrix matSrc;
        matSrc.Set(2,3, 2);

        const PRREMatrix mat = matSrc - matSrc;

        return assertTrue( mat.isZero() );
    }

    bool testOperatorSubtractionAssignment()
    {
        PRREMatrix mat;
        mat.Set(2,3, 2);

        mat -= mat;

        return assertTrue( mat.isZero() );
    }

    bool testOperatorMultiplicationByMatrix()
    {
        PRREMatrix matSrc;
        matSrc.Set(0,1, 2);
        matSrc.Set(0,2, 3);
        matSrc.Set(1,0, 4);
        matSrc.Set(1,1, 5);
        matSrc.Set(1,2, 6);
        matSrc.Set(2,0, 7);
        matSrc.Set(2,1, 8);
        matSrc.Set(2,2, 9);

        const PRREMatrix matDst  = matSrc * PRREMatrix::getIdentity();
        const PRREMatrix matDst2 = matSrc * PRREMatrix::getZero();

        return assertTrue(matSrc == matDst, "* getIdentity()") & assertTrue(matDst2.isZero(), "* getZero()");
    }

    bool testOperatorMultiplicationAssignmentByMatrix()
    {
        PRREMatrix matSrc;
        matSrc.Set(0,1, 2);
        matSrc.Set(0,2, 3);
        matSrc.Set(1,0, 4);
        matSrc.Set(1,1, 5);
        matSrc.Set(1,2, 6);
        matSrc.Set(2,0, 7);
        matSrc.Set(2,1, 8);
        matSrc.Set(2,2, 9);

        const PRREMatrix matExpected = matSrc;
        PRREMatrix matDst = matSrc;
        matSrc *= PRREMatrix::getIdentity();
        matDst *= PRREMatrix::getZero();

        return assertTrue(matSrc == matExpected, "*= getIdentity()") & assertTrue(matDst.isZero(), "*= getZero()");
    }

    bool testOperatorMultiplicationByScalar1()
    {
        PRREMatrix mat;
        mat(2,3) = 2;
        mat = mat * 3;

        return assertEquals(3, mat.get(0,0), E, "(0,0)") &
            assertEquals(3, mat.get(1,1), E, "(1,1)") &
            assertEquals(3, mat.get(2,2), E, "(2,2)") &
            assertEquals(3, mat.get(3,3), E, "(3,3)") &
            assertEquals(0, mat.get(0,1), E, "(0,1)") & assertEquals(0, mat.get(0,2), E, "(0,2)") & assertEquals(0, mat.get(0,3), E, "(0,3)") &
            assertEquals(0, mat.get(1,0), E, "(1,0)") & assertEquals(0, mat.get(1,2), E, "(1,2)") & assertEquals(0, mat.get(1,3), E, "(1,3)") &
            assertEquals(0, mat.get(2,0), E, "(2,0)") & assertEquals(0, mat.get(2,1), E, "(2,1)") & assertEquals(6, mat.get(2,3), E, "(2,3)") &
            assertEquals(0, mat.get(3,0), E, "(3,0)") & assertEquals(0, mat.get(3,1), E, "(3,1)") & assertEquals(0, mat.get(3,2), E, "(3,2)");
    }

    bool testOperatorMultiplicationByScalar2()
    {
        PRREMatrix mat;
        mat(2,3) = 2;
        mat = 3 * mat;

        return assertEquals(3, mat.get(0,0), E, "(0,0)") &
            assertEquals(3, mat.get(1,1), E, "(1,1)") &
            assertEquals(3, mat.get(2,2), E, "(2,2)") &
            assertEquals(3, mat.get(3,3), E, "(3,3)") &
            assertEquals(0, mat.get(0,1), E, "(0,1)") & assertEquals(0, mat.get(0,2), E, "(0,2)") & assertEquals(0, mat.get(0,3), E, "(0,3)") &
            assertEquals(0, mat.get(1,0), E, "(1,0)") & assertEquals(0, mat.get(1,2), E, "(1,2)") & assertEquals(0, mat.get(1,3), E, "(1,3)") &
            assertEquals(0, mat.get(2,0), E, "(2,0)") & assertEquals(0, mat.get(2,1), E, "(2,1)") & assertEquals(6, mat.get(2,3), E, "(2,3)") &
            assertEquals(0, mat.get(3,0), E, "(3,0)") & assertEquals(0, mat.get(3,1), E, "(3,1)") & assertEquals(0, mat.get(3,2), E, "(3,2)");
    }

    bool testOperatorMultiplicationAssignmentByScalar()
    {
        PRREMatrix mat;
        mat.Set(2,3, 2);
        mat *= 3;

        return assertEquals(3, mat.get(0,0), E, "(0,0)") &
            assertEquals(3, mat.get(1,1), E, "(1,1)") &
            assertEquals(3, mat.get(2,2), E, "(2,2)") &
            assertEquals(3, mat.get(3,3), E, "(3,3)") &
            assertEquals(0, mat.get(0,1), E, "(0,1)") & assertEquals(0, mat.get(0,2), E, "(0,2)") & assertEquals(0, mat.get(0,3), E, "(0,3)") &
            assertEquals(0, mat.get(1,0), E, "(1,0)") & assertEquals(0, mat.get(1,2), E, "(1,2)") & assertEquals(0, mat.get(1,3), E, "(1,3)") &
            assertEquals(0, mat.get(2,0), E, "(2,0)") & assertEquals(0, mat.get(2,1), E, "(2,1)") & assertEquals(6, mat.get(2,3), E, "(2,3)") &
            assertEquals(0, mat.get(3,0), E, "(3,0)") & assertEquals(0, mat.get(3,1), E, "(3,1)") & assertEquals(0, mat.get(3,2), E, "(3,2)");
    }

    bool testOperatorAccessElement()
    {
        PRREMatrix mat;
        mat(1,1) = 5;
        mat(2,3) = 7;

        return assertEquals(1, mat(0,0), E, "(0,0)") &
            assertEquals(5, mat(1,1), E, "(1,1)") &
            assertEquals(1, mat(2,2), E, "(2,2)") &
            assertEquals(1, mat(3,3), E, "(3,3)") &
            assertEquals(0, mat(0,1), E, "(0,1)") & assertEquals(0, mat(0,2), E, "(0,2)") & assertEquals(0, mat(0,3), E, "(0,3)") &
            assertEquals(0, mat(1,0), E, "(1,0)") & assertEquals(0, mat(1,2), E, "(1,2)") & assertEquals(0, mat(1,3), E, "(1,3)") &
            assertEquals(0, mat(2,0), E, "(2,0)") & assertEquals(0, mat(2,1), E, "(2,1)") & assertEquals(7, mat(2,3), E, "(2,3)") &
            assertEquals(0, mat(3,0), E, "(3,0)") & assertEquals(0, mat(3,1), E, "(3,1)") & assertEquals(0, mat(3,2), E, "(3,2)");
    }

    bool testOperatorNegative()
    {
        PRREMatrix mat;
        mat(2,3) = 5;

        mat = -mat;

        return assertEquals(-1, mat(0,0), E, "(0,0)") &
            assertEquals(-1, mat(1,1), E, "(1,1)") &
            assertEquals(-1, mat(2,2), E, "(2,2)") &
            assertEquals(-1, mat(3,3), E, "(3,3)") &
            assertEquals(0, mat(0,1), E, "(0,1)") & assertEquals(0, mat(0,2), E, "(0,2)") & assertEquals( 0, mat(0,3), E, "(0,3)") &
            assertEquals(0, mat(1,0), E, "(1,0)") & assertEquals(0, mat(1,2), E, "(1,2)") & assertEquals( 0, mat(1,3), E, "(1,3)") &
            assertEquals(0, mat(2,0), E, "(2,0)") & assertEquals(0, mat(2,1), E, "(2,1)") & assertEquals(-5, mat(2,3), E, "(2,3)") &
            assertEquals(0, mat(3,0), E, "(3,0)") & assertEquals(0, mat(3,1), E, "(3,1)") & assertEquals( 0, mat(3,2), E, "(3,2)");
    }

    bool testOperatorMultiplicationByVectorRight()
    {
        PRREMatrix mat;
        mat.Set(0,1, 2);
        mat.Set(0,2, 3);
        mat.Set(1,0, 4);
        mat.Set(1,1, 5);
        mat.Set(1,2, 6);
        mat.Set(2,0, 7);
        mat.Set(2,1, 8);
        mat.Set(2,2, 9);

        const PRREVector vec(2, 0, 3);
        const PRREVector vec2 = mat * vec;

        return assertEquals(mat.get(0,0)*vec.getX() + mat.get(0,1)*vec.getY() + mat.get(0,2)*vec.getZ(), vec2.getX(), "getX()") &
            assertEquals(mat.get(1,0)*vec.getX() + mat.get(1,1)*vec.getY() + mat.get(1,2)*vec.getZ(), vec2.getY(), "getY()") &
            assertEquals(mat.get(2,0)*vec.getX() + mat.get(2,1)*vec.getY() + mat.get(2,2)*vec.getZ(), vec2.getZ(), "getZ()");
    }

    bool testOperatorMultiplicationByVectorLeft()
    {
        PRREMatrix mat;
        mat.Set(0,1, 2);
        mat.Set(0,2, 3);
        mat.Set(1,0, 4);
        mat.Set(1,1, 5);
        mat.Set(1,2, 6);
        mat.Set(2,0, 7);
        mat.Set(2,1, 8);
        mat.Set(2,2, 9);

        const PRREVector vec(2, 0, 3);
        const PRREVector vec2 = vec * mat;

        return assertEquals(mat.get(0,0)*vec.getX() + mat.get(1,0)*vec.getY() + mat.get(2,0)*vec.getZ(), vec2.getX(), "getX()") &
            assertEquals(mat.get(0,1)*vec.getX() + mat.get(1,1)*vec.getY() + mat.get(2,1)*vec.getZ(), vec2.getY(), "getY()") &
            assertEquals(mat.get(0,2)*vec.getX() + mat.get(1,2)*vec.getY() + mat.get(2,2)*vec.getZ(), vec2.getZ(), "getZ()");
    }

    bool testOperatorMultiplicationAssignmentByVectorLeft()
    {
        PRREMatrix mat;
        mat.Set(0,1, 2);
        mat.Set(0,2, 3);
        mat.Set(1,0, 4);
        mat.Set(1,1, 5);
        mat.Set(1,2, 6);
        mat.Set(2,0, 7);
        mat.Set(2,1, 8);
        mat.Set(2,2, 9);

        const PRREVector vec(2, 0, 3);
        PRREVector vec2 = vec;
        vec2 *= mat;

        return assertEquals(mat.get(0,0)*vec.getX() + mat.get(1,0)*vec.getY() + mat.get(2,0)*vec.getZ(), vec2.getX(), "getX()") &
            assertEquals(mat.get(0,1)*vec.getX() + mat.get(1,1)*vec.getY() + mat.get(2,1)*vec.getZ(), vec2.getY(), "getY()") &
            assertEquals(mat.get(0,2)*vec.getX() + mat.get(1,2)*vec.getY() + mat.get(2,2)*vec.getZ(), vec2.getZ(), "getZ()");
    }


}; // class PRREMatrixTest