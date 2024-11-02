#pragma once

/*
    ###################################################################################
    PureMatrixTest.h
    Unit test for PureMatrix.
    Made by PR00F88
    ###################################################################################
*/

#include "UnitTest.h"  // PCH
#include "../Pure/include/external/Math/PureMatrix.h"
#include "../Pure/include/external/Math/PureVector.h"

#ifndef E
#define E 0.0001f
#endif // EPSILON

class PureMatrixTest :
    public UnitTest
{
public:

    PureMatrixTest() :
        UnitTest( __FILE__ )
    {
        addSubTest("testCtor", (PFNUNITSUBTEST) &PureMatrixTest::testCtor);
        addSubTest("testCopyCtor", (PFNUNITSUBTEST) &PureMatrixTest::testCopyCtor);
        addSubTest("testCtorNotableZero", (PFNUNITSUBTEST) &PureMatrixTest::testCtorNotableZero);
        addSubTest("testCtorNotableIdentity", (PFNUNITSUBTEST) &PureMatrixTest::testCtorNotableIdentity);
        addSubTest("testStaticNotableZero", (PFNUNITSUBTEST) &PureMatrixTest::testStaticNotableZero);
        addSubTest("testStaticNotableIdentity", (PFNUNITSUBTEST) &PureMatrixTest::testStaticNotableIdentity);
        addSubTest("testGet", (PFNUNITSUBTEST) &PureMatrixTest::testGet);
        addSubTest("testSet", (PFNUNITSUBTEST) &PureMatrixTest::testSet);
        addSubTest("testGet2", (PFNUNITSUBTEST) &PureMatrixTest::testGet2);
        addSubTest("testIsZero", (PFNUNITSUBTEST) &PureMatrixTest::testIsZero);
        addSubTest("testSetZero", (PFNUNITSUBTEST) &PureMatrixTest::testSetZero);
        addSubTest("testIsIdentity", (PFNUNITSUBTEST) &PureMatrixTest::testIsIdentity);
        addSubTest("testSetIdentity", (PFNUNITSUBTEST) &PureMatrixTest::testSetIdentity);
        addSubTest("testTranspose", (PFNUNITSUBTEST) &PureMatrixTest::testTranspose);
        addSubTest("testGetTransposed", (PFNUNITSUBTEST) &PureMatrixTest::testGetTransposed);
        addSubTest("getDeterminantOfIdentity", (PFNUNITSUBTEST) &PureMatrixTest::getDeterminantOfIdentity);
        addSubTest("getDeterminantOfZero", (PFNUNITSUBTEST) &PureMatrixTest::getDeterminantOfZero);
        addSubTest("getDeterminantOfDegenerate", (PFNUNITSUBTEST) &PureMatrixTest::getDeterminantOfDegenerate);
        addSubTest("getDeterminantOfDegenTransposed", (PFNUNITSUBTEST) &PureMatrixTest::getDeterminantOfDegenTransposed);
        addSubTest("isIdentityInvertible", (PFNUNITSUBTEST) &PureMatrixTest::isIdentityInvertible);
        addSubTest("isZeroInvertible", (PFNUNITSUBTEST) &PureMatrixTest::isZeroInvertible);
        addSubTest("isDegenerateInvertible", (PFNUNITSUBTEST) &PureMatrixTest::isDegenerateInvertible);
        addSubTest("isDegenTransposeInvertible", (PFNUNITSUBTEST) &PureMatrixTest::isDegenTransposeInvertible);
        addSubTest("InvertIdentity", (PFNUNITSUBTEST) &PureMatrixTest::InvertIdentity);
        addSubTest("InvertZero", (PFNUNITSUBTEST) &PureMatrixTest::InvertZero);
        addSubTest("InvertDegenerate", (PFNUNITSUBTEST) &PureMatrixTest::InvertDegenerate);
        addSubTest("InvertInvertible", (PFNUNITSUBTEST) &PureMatrixTest::InvertInvertible);
        addSubTest("getInvertIdentity", (PFNUNITSUBTEST) &PureMatrixTest::getInvertIdentity);
        addSubTest("getInvertZero", (PFNUNITSUBTEST) &PureMatrixTest::getInvertZero);
        addSubTest("getInvertDegenerate", (PFNUNITSUBTEST) &PureMatrixTest::getInvertDegenerate);
        addSubTest("getInvertInvertible", (PFNUNITSUBTEST) &PureMatrixTest::getInvertInvertible);
        addSubTest("testOperatorEquals", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorEquals);
        addSubTest("testOperatorNotEquals", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorNotEquals);
        addSubTest("testOperatorAssignment", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorAssignment);
        addSubTest("testOperatorAddition", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorAddition);
        addSubTest("testOperatorAdditionAssignment", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorAdditionAssignment);
        addSubTest("testOperatorSubtraction", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorSubtraction);
        addSubTest("testOperatorSubtractionAssignment", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorSubtractionAssignment);
        addSubTest("testOperatorMultiplicationByMatrix", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorMultiplicationByMatrix);
        addSubTest("testOperatorMultiplicationAssignmentByMatrix", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorMultiplicationAssignmentByMatrix);
        addSubTest("testOperatorMultiplicationByScalar1", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorMultiplicationByScalar1);
        addSubTest("testOperatorMultiplicationByScalar2", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorMultiplicationByScalar2);
        addSubTest("testOperatorMultiplicationAssignmentByScalar", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorMultiplicationAssignmentByScalar);
        addSubTest("testOperatorAccessElement", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorAccessElement);
        addSubTest("testOperatorNegative", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorNegative);
        addSubTest("testOperatorMultiplicationByVectorRight", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorMultiplicationByVectorRight);
        addSubTest("testOperatorMultiplicationByVectorLeft", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorMultiplicationByVectorLeft);
        addSubTest("testOperatorMultiplicationAssignmentByVectorLeft", (PFNUNITSUBTEST) &PureMatrixTest::testOperatorMultiplicationAssignmentByVectorLeft);

    } // PureMatrixTest()

protected:

private:

    // ---------------------------------------------------------------------------

    PureMatrixTest(const PureMatrixTest&)
    {};         

    PureMatrixTest& operator=(const PureMatrixTest&)
    {
        return *this;
    };

    bool testCtor()
    {
        const PureMatrix mat;
        return assertTrue(mat.isIdentity());
    }

    bool testCopyCtor()
    {
        PureMatrix matSrc;

        matSrc.Set(0, 0, 99);
        matSrc.Set(1, 2, 80);

        const PureMatrix matDst(matSrc);

        return assertTrue(matSrc == matDst);
    }

    bool testCtorNotableZero()
    {
        const PureMatrix mat(PURE_MATRIX_ZERO);
        return assertTrue(mat.isZero());
    }

    bool testCtorNotableIdentity()
    {
        const PureMatrix mat(PURE_MATRIX_IDENTITY);
        return assertTrue(mat.isIdentity());
    }

    bool testStaticNotableZero()
    {
        const PureMatrix mat = PureMatrix::getZero();
        return assertTrue(mat.isZero());
    }

    bool testStaticNotableIdentity()
    {
        PureMatrix mat;
        mat.Set(1,1, 5);
        mat = PureMatrix::getIdentity();

        return assertTrue(mat.isIdentity());
    }

    bool testGet()
    {
        PureMatrix mat;
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
        PureMatrix mat;
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
        PureMatrix mat;    
        mat.Set(0,0, 6);

        return assertNotNull(mat.get(), "not NULL") &
            assertEquals(6, mat.get()[0], E, "(0,0)");
    }

    bool testIsZero()
    {
        PureMatrix mat(PURE_MATRIX_ZERO);
        return assertTrue(mat.isZero());
    }

    bool testSetZero()
    {
        PureMatrix mat;
        mat.Set(1,1, 5);
        mat.Set(2,3, 7);
        mat.SetZero();

        return assertTrue(mat.isZero());
    }

    bool testIsIdentity()
    {
        const PureMatrix mat(PURE_MATRIX_IDENTITY);
        return assertTrue(mat.isIdentity());
    }

    bool testTranspose()
    {
        PureMatrix mat;    
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
        PureMatrix mat, mattransposed;    
        
        mat.SetIdentity();
        mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,2,  6);  mat.Set(1,3,  7);
        mat.Set(2,0,  8);  mat.Set(2,1,  9);  mat.Set(2,3, 10);
        mat.Set(3,0, 11);  mat.Set(3,1, 12);  mat.Set(3,2, 13);
        
        mattransposed = mat;
        mattransposed.Transpose();

        return assertNotNull(mat.getTransposed(), "not NULL") &
            assertTrue(0 == memcmp(mat.getTransposed(), mattransposed.get(), sizeof(TPureFloat)*4*4), "memcmp");
    }

    bool getDeterminantOfIdentity()
    {
        PureMatrix mat;
        mat.SetIdentity();

        return assertEquals(1.0f, mat.getDeterminant(), E);
    }

    bool getDeterminantOfZero()
    {
        PureMatrix mat;
        mat.SetZero();

        return assertEquals(0.0f, mat.getDeterminant(), E);
    }

    bool getDeterminantOfDegenerate()
    {
        PureMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);

        return assertEquals(0.0f, mat.getDeterminant(), E);
    }

    bool getDeterminantOfDegenTransposed()
    {
        PureMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);

        const TPureFloat det = mat.getDeterminant();

        mat.Transpose();

        return assertEquals(det, mat.getDeterminant(), E);
    }

    bool isIdentityInvertible()
    {
        PureMatrix mat;
        mat.SetIdentity();

        return assertTrue( mat.isInvertible() );
    }

    bool isZeroInvertible()
    {
        PureMatrix mat;
        mat.SetZero();

        return assertFalse( mat.isInvertible() );
    }

    bool isDegenerateInvertible()
    {
        PureMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);

        return assertFalse( mat.isInvertible() );
    }

    bool isDegenTransposeInvertible()
    {
        PureMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);

        mat.Transpose();

        return assertFalse( mat.isInvertible() );
    }

    bool InvertIdentity()
    {
        PureMatrix mat;
        mat.SetIdentity();

        const TPureFloat det = mat.getDeterminant();
        const bool b1 = mat.invert();

        return assertEquals(1/det, mat.getDeterminant(), E, "det" ) &
            assertTrue(mat.isIdentity(), "isIdentity") &
            assertTrue(b1, "invert");
    }

    bool InvertZero()
    {
        PureMatrix mat;
        mat.SetZero();

        const TPureFloat det = mat.getDeterminant();
        const bool b1 = mat.invert();

        return assertEquals(det, mat.getDeterminant(), E, "det" ) &
            assertTrue(mat.isZero(), "isZero") &
            assertFalse(b1, "invert");
    }

    bool InvertDegenerate()
    {
        PureMatrix mat;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);
        const PureMatrix mat2 = mat;

        const TPureFloat det = mat.getDeterminant();
        const bool b1 = mat.invert();

        return assertEquals(det, mat.getDeterminant(), E, "det" ) &
            assertTrue(mat2 == mat, "mat2 == mat") &
            assertFalse(b1, "invert");
    }

    bool InvertInvertible()
    {
        PureMatrix mat;
        mat.Set(0,0,  2);  mat.Set(0,1,  2);  mat.Set(0,2, 10);  mat.Set(0,3,  2);
        mat.Set(1,0,  4);  mat.Set(1,1,  4);  mat.Set(1,2,  2);  mat.Set(1,3,  2);
        mat.Set(2,0,  6);  mat.Set(2,1,  2);  mat.Set(2,2,  6);  mat.Set(2,3,  2);
        mat.Set(3,0,  6);  mat.Set(3,1,  2);  mat.Set(3,2,  2);  mat.Set(3,3,  1);
        const PureMatrix mat2 = mat;

        const TPureFloat det = mat.getDeterminant();
        const bool b1 = mat.invert();
        const PureMatrix ident = mat*mat2;

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
        PureMatrix mat, matinverted;    
        
        mat.SetIdentity();
        matinverted = mat;
        const bool b1 = matinverted.invert();
        const TPureFloat* p = mat.getInverse();

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
        PureMatrix mat, matinverted;    
        
        mat.SetZero();
        matinverted = mat;
        const bool b1 = matinverted.invert();
        const TPureFloat* p = mat.getInverse();

        return assertNull(p, "NULL") &
            assertFalse(b1, "invert");
    }

    bool getInvertDegenerate()
    {
        PureMatrix mat, matinverted;
        mat.Set(0,0,  1);  mat.Set(0,1,  2);  mat.Set(0,2,  3);  mat.Set(0,3,  4);
        mat.Set(1,0,  5);  mat.Set(1,1,  6);  mat.Set(1,2,  7);  mat.Set(1,3,  8);
        mat.Set(2,0,  9);  mat.Set(2,1, 10);  mat.Set(2,2, 11);  mat.Set(2,3, 12);
        mat.Set(3,0, 13);  mat.Set(3,1, 14);  mat.Set(3,2, 15);  mat.Set(3,3, 16);
        matinverted = mat;

        const bool b1 = matinverted.invert();
        const TPureFloat* p = mat.getInverse();

        return assertNull(p, "NULL") &
            assertTrue(matinverted == mat, "matinverted == mat") &
            assertFalse(b1, "invert");
    }

    bool getInvertInvertible()
    {
        PureMatrix mat, matinverted;
        mat.Set(0,0,  2);  mat.Set(0,1,  2);  mat.Set(0,2, 10);  mat.Set(0,3,  2);
        mat.Set(1,0,  4);  mat.Set(1,1,  4);  mat.Set(1,2,  2);  mat.Set(1,3,  2);
        mat.Set(2,0,  6);  mat.Set(2,1,  2);  mat.Set(2,2,  6);  mat.Set(2,3,  2);
        mat.Set(3,0,  6);  mat.Set(3,1,  2);  mat.Set(3,2,  2);  mat.Set(3,3,  1);
        matinverted = mat;

        const bool b1 = matinverted.invert();
        const TPureFloat* p = mat.getInverse();

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
        PureMatrix mat;
        mat.Set(1,1, 5);
        mat.Set(2,3, 7);
        mat.SetIdentity();

        return assertTrue(mat.isIdentity());
    }

    bool testOperatorEquals()
    {
        const PureMatrix mat1;
        const PureMatrix mat2;

        return assertTrue(mat1 == mat2);
    }

    bool testOperatorNotEquals()
    {
        const PureMatrix mat1;
        const PureMatrix mat2;

        return assertFalse(mat1 != mat2);
    }

    bool testOperatorAssignment()
    {
        PureMatrix mat1;
        mat1.Set(1,1, 5);

        const PureMatrix mat2 = mat1;

        return assertTrue(mat1 == mat2);
    }

    bool testOperatorAddition()
    {
        PureMatrix matSrc;
        matSrc.Set(2,3, 2);

        const PureMatrix mat = matSrc + matSrc;

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
        PureMatrix mat;
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
        PureMatrix matSrc;
        matSrc.Set(2,3, 2);

        const PureMatrix mat = matSrc - matSrc;

        return assertTrue( mat.isZero() );
    }

    bool testOperatorSubtractionAssignment()
    {
        PureMatrix mat;
        mat.Set(2,3, 2);

        mat -= mat;

        return assertTrue( mat.isZero() );
    }

    bool testOperatorMultiplicationByMatrix()
    {
        PureMatrix matSrc;
        matSrc.Set(0,1, 2);
        matSrc.Set(0,2, 3);
        matSrc.Set(1,0, 4);
        matSrc.Set(1,1, 5);
        matSrc.Set(1,2, 6);
        matSrc.Set(2,0, 7);
        matSrc.Set(2,1, 8);
        matSrc.Set(2,2, 9);

        const PureMatrix matDst  = matSrc * PureMatrix::getIdentity();
        const PureMatrix matDst2 = matSrc * PureMatrix::getZero();

        return assertTrue(matSrc == matDst, "* getIdentity()") & assertTrue(matDst2.isZero(), "* getZero()");
    }

    bool testOperatorMultiplicationAssignmentByMatrix()
    {
        PureMatrix matSrc;
        matSrc.Set(0,1, 2);
        matSrc.Set(0,2, 3);
        matSrc.Set(1,0, 4);
        matSrc.Set(1,1, 5);
        matSrc.Set(1,2, 6);
        matSrc.Set(2,0, 7);
        matSrc.Set(2,1, 8);
        matSrc.Set(2,2, 9);

        const PureMatrix matExpected = matSrc;
        PureMatrix matDst = matSrc;
        matSrc *= PureMatrix::getIdentity();
        matDst *= PureMatrix::getZero();

        return assertTrue(matSrc == matExpected, "*= getIdentity()") & assertTrue(matDst.isZero(), "*= getZero()");
    }

    bool testOperatorMultiplicationByScalar1()
    {
        PureMatrix mat;
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
        PureMatrix mat;
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
        PureMatrix mat;
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
        PureMatrix mat;
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
        PureMatrix mat;
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
        PureMatrix mat;
        mat.Set(0,1, 2);
        mat.Set(0,2, 3);
        mat.Set(1,0, 4);
        mat.Set(1,1, 5);
        mat.Set(1,2, 6);
        mat.Set(2,0, 7);
        mat.Set(2,1, 8);
        mat.Set(2,2, 9);

        const PureVector vec(2, 0, 3);
        const PureVector vec2 = mat * vec;

        return assertEquals(mat.get(0,0)*vec.getX() + mat.get(0,1)*vec.getY() + mat.get(0,2)*vec.getZ(), vec2.getX(), "getX()") &
            assertEquals(mat.get(1,0)*vec.getX() + mat.get(1,1)*vec.getY() + mat.get(1,2)*vec.getZ(), vec2.getY(), "getY()") &
            assertEquals(mat.get(2,0)*vec.getX() + mat.get(2,1)*vec.getY() + mat.get(2,2)*vec.getZ(), vec2.getZ(), "getZ()");
    }

    bool testOperatorMultiplicationByVectorLeft()
    {
        PureMatrix mat;
        mat.Set(0,1, 2);
        mat.Set(0,2, 3);
        mat.Set(1,0, 4);
        mat.Set(1,1, 5);
        mat.Set(1,2, 6);
        mat.Set(2,0, 7);
        mat.Set(2,1, 8);
        mat.Set(2,2, 9);

        const PureVector vec(2, 0, 3);
        const PureVector vec2 = vec * mat;

        return assertEquals(mat.get(0,0)*vec.getX() + mat.get(1,0)*vec.getY() + mat.get(2,0)*vec.getZ(), vec2.getX(), "getX()") &
            assertEquals(mat.get(0,1)*vec.getX() + mat.get(1,1)*vec.getY() + mat.get(2,1)*vec.getZ(), vec2.getY(), "getY()") &
            assertEquals(mat.get(0,2)*vec.getX() + mat.get(1,2)*vec.getY() + mat.get(2,2)*vec.getZ(), vec2.getZ(), "getZ()");
    }

    bool testOperatorMultiplicationAssignmentByVectorLeft()
    {
        PureMatrix mat;
        mat.Set(0,1, 2);
        mat.Set(0,2, 3);
        mat.Set(1,0, 4);
        mat.Set(1,1, 5);
        mat.Set(1,2, 6);
        mat.Set(2,0, 7);
        mat.Set(2,1, 8);
        mat.Set(2,2, 9);

        const PureVector vec(2, 0, 3);
        PureVector vec2 = vec;
        vec2 *= mat;

        return assertEquals(mat.get(0,0)*vec.getX() + mat.get(1,0)*vec.getY() + mat.get(2,0)*vec.getZ(), vec2.getX(), "getX()") &
            assertEquals(mat.get(0,1)*vec.getX() + mat.get(1,1)*vec.getY() + mat.get(2,1)*vec.getZ(), vec2.getY(), "getY()") &
            assertEquals(mat.get(0,2)*vec.getX() + mat.get(1,2)*vec.getY() + mat.get(2,2)*vec.getZ(), vec2.getZ(), "getZ()");
    }


}; // class PureMatrixTest